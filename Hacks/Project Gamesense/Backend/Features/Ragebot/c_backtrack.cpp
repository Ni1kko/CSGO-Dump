#include "c_backtrack.h"
#include "Math.h"



template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
CBacktrackHelper* g_BacktrackHelper = new CBacktrackHelper;

ConVar* minupdate;
ConVar* maxupdate;
ConVar* updaterate;
ConVar* interprate;
ConVar* cmin;
ConVar* cmax;
ConVar* interp;

float CBacktrackHelper::GetLerpTime() {
	static ConVar* cl_interp = Interfaces::Var->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces::Var->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces::Var->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces::Var->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces::Var->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces::Var->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces::Var->FindVar("sv_client_max_interp_ratio");
	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;
	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;
	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;
	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;
	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;
	else
		return v20;
}

bool CBacktrackHelper::IsTickValid(tick_record record)
{
	float correct = 0;

	auto NetChannel = Interfaces::Engine->GetNetChannelInfo();
	if (!NetChannel)
		return false;

	correct += NetChannel->GetAvgLatency(FLOW_OUTGOING);
	correct += NetChannel->GetAvgLatency(FLOW_INCOMING);
	correct += GetLerpTime();

	static ConVar* sv_maxunlag = Interfaces::Var->FindVar("sv_maxunlag");
	correct = clamp<float>(correct, 0.0f, sv_maxunlag->GetFloat());

	float deltaTime = correct - (Interfaces::Globals->curtime - record.m_flSimulationTime);

	float ping = 0.2f;

	return fabsf(deltaTime) < ping;
}

void CBacktrackHelper::UpdateBacktrackRecords(CBaseEntity* pPlayer)
{
	int i = pPlayer->Index();
	for (int j = g_BacktrackHelper->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		tick_record rec = g_BacktrackHelper->PlayerRecord[i].records.at(j);
		float ping =  0.2f;
		if (rec.m_flSimulationTime < Interfaces::Globals->curtime - ping)
			g_BacktrackHelper->PlayerRecord[i].records.erase(g_BacktrackHelper->PlayerRecord[i].records.begin() + j);

	}
	static Vector old_origin[64];
	if (PlayerRecord[i].records.size() > 0 && pPlayer->GetSimTime() == PlayerRecord[i].records.back().m_flSimulationTime) //already got such a record
		return;

	for (int j = g_BacktrackHelper->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		tick_record rec = g_BacktrackHelper->PlayerRecord[i].records.at(j);
		rec = g_BacktrackHelper->PlayerRecord[i].records.at(j);

		if (PlayerRecord[i].records.size() > 0 && rec.m_flSimulationTime > pPlayer->GetSimTime())//Invalid lag record, maybe from diffrent game?
		{
			PlayerRecord[i].records.clear();
			return;
		}

	}

	Vector cur_origin = pPlayer->GetOrigin();
	Vector v = cur_origin - old_origin[i];
	bool breaks_lagcomp = v.LengthSqr() > 4096.f;
	old_origin[i] = cur_origin;
	tick_record new_record;

	new_record.needs_extrapolation = breaks_lagcomp;
	static float OldLower[64];

	PlayerRecord[i].LowerBodyYawTarget = pPlayer->GetLowerBodyYaw();
	new_record.m_angEyeAngles = pPlayer->GetEyeAngles();
	new_record.m_flCycle = pPlayer->GetCycle();

	new_record.m_nFlags = pPlayer->GetFlags();
	new_record.m_absangles = pPlayer->GetAbsAngles();


	new_record.m_flSimulationTime = pPlayer->GetSimTime();
	new_record.m_flAnimTime = pPlayer->GetAnimationTime();
	new_record.m_vecAbsOrigin = pPlayer->GetAbsOrigin();
	new_record.bLowerBodyYawUpdated = false;
	new_record.m_nSequence = pPlayer->GetSequence();
	new_record.m_vecOrigin = pPlayer->GetOrigin();
	new_record.m_vecVelocity = pPlayer->GetVelocity();
	new_record.m_flUpdateTime = Interfaces::Globals->curtime;
	new_record.backtrack_time = new_record.m_flSimulationTime + GetLerpTime();
	new_record.m_vecMins = pPlayer->GetCollision()->VecMins();
	new_record.m_vecMax = pPlayer->GetCollision()->VecMaxs();
	//new_record.ragpos = pPlayer->get_ragdoll_pos();

	if (PlayerRecord[i].LowerBodyYawTarget != OldLower[i])
		new_record.bLowerBodyYawUpdated = true;

	for (int i = 0; i < 24; i++)
		new_record.m_flPoseParameter[i] = *(float*)((DWORD)pPlayer + offys.m_flPoseParameter + sizeof(float) * i);

	pPlayer->SetupBones(new_record.boneMatrix, 128, 256, Interfaces::Globals->curtime);

	OldLower[i] = PlayerRecord[i].LowerBodyYawTarget;
	PlayerRecord[i].records.push_back(new_record);
}

static std::deque<CIncomingSequence>sequences;
static int lastincomingsequencenumber;
int Real_m_nInSequencenumber;

void CBacktrackHelper::UpdateIncomingSequences()
{
	if (!Interfaces::ClientState || Interfaces::ClientState == 0)
		return;

	if (!Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected() || !G::LocalPlayer || !G::UserCmd)
		return;

	DWORD ClientState = (DWORD) * (DWORD*)Interfaces::ClientState;
	if (ClientState)
	{
		auto netchan = Interfaces::Engine->GetNetChannel();
		if (netchan)
		{
			if (netchan->m_nInSequenceNr > lastincomingsequencenumber)
			{
				lastincomingsequencenumber = netchan->m_nInSequenceNr;
				sequences.push_front(CIncomingSequence(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, Interfaces::Globals->realtime));
			}

			if (sequences.size() > 2048)
				sequences.pop_back();
		}
	}
}

void CBacktrackHelper::ClearIncomingSequences()
{
	sequences.clear();
}

void CBacktrackHelper::AddLatencyToNetchan(NetChannel* netchan, float Latency)
{
	for (auto& seq : sequences)
	{
		if (Interfaces::Globals->realtime - seq.curtime >= Latency || Interfaces::Globals->realtime - seq.curtime > 1)
		{
			netchan->m_nInReliableState = seq.inreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}
}
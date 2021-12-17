#include "../MISC/includes.h"
#include "../UTILS/INTERFACES.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"

#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/INetChannelInfo.h"
#include "../SDK/ConVar.h"
#include "../SDK/NetChannel.h"

#include "AutoWall.h"
#include "Aimbot.h"
#include "Backtracking.h"

#define TICK_INTERVAL			( g_pGlobalVarsBase->interval_per_tick )

#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

template<class T> const T&
clamp(const T& x, const T& upper, const T& lower) { return min(upper, max(x, lower)); }
SDK::CUserCmd* UserCmdForBacktracking = nullptr;

static std::deque<CIncomingSequence>sequences;


int CBacktrack::GetLatencyTicks()
{
	double v0;
	SDK::INetChannelInfo* v1;
	SDK::INetChannelInfo* v2;
	float v3;
	float v4;

	v1 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();
	v2 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);

	float interval_per_tick = 1.0f / INTERFACES::Globals->interval_per_tick;

	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
}

/*bool CBacktrack::IsTickValid(int tick)
{
	SDK::INetChannelInfo *nci = INTERFACES::Engine->GetNetChannelInfo();

	if (!nci)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, 1.f);

	float deltaTime = correct - (INTERFACES::Globals->curtime - TICKS_TO_TIME(tick));

	return fabsf(deltaTime) < 0.2f;
}*/
bool CBacktrack::IsTickValid(tick_record record)
{
	float correct = 0;

	correct += INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	correct += INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
	correct += backtracking->GetLerpTime();

	static SDK::ConVar* sv_maxunlag = INTERFACES::cvar->FindVar("sv_maxunlag");
	correct = clamp<float>(correct, 0.0f, sv_maxunlag->GetFloat());

	float deltaTime = correct - (INTERFACES::Globals->curtime - record.m_flSimulationTime);

	float latecy = SETTINGS::settings.FakeLatencyAmount;
	float ping = SETTINGS::settings.FakeLatency ? latecy : 0.2f;

	return fabsf(deltaTime) < ping;
}


/*float CBacktrack::GetEstimateServerTime()
{
	double v0;
	SDK::INetChannelInfo* v1;
	SDK::INetChannelInfo* v2;
	float v3;
	float v4;

	v1 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();
	v2 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);

	return v3 + v4 + TICKS_TO_TIME(1) + TICKS_TO_TIME(UserCmdForBacktracking->tick_count);
}*/
void CBacktrack::ClearIncomingSequences()
{
	sequences.clear();
}
float CBacktrack::GetNetworkLatency()
{
	SDK::INetChannelInfo* nci = INTERFACES::Engine->GetNetChannelInfo();
	if (nci) {
		float OutgoingLatency = nci->GetLatency(0);
		return OutgoingLatency;
	}
	return 0.0f;
}

static SDK::ConVar* minupdate;
static SDK::ConVar*  maxupdate;
static SDK::ConVar*  updaterate;
static SDK::ConVar* interprate;
static SDK::ConVar*  cmin;
static SDK::ConVar* cmax;
static SDK::ConVar* interp;

float CBacktrack::GetLerpTime()
{
	if (!minupdate)
		minupdate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	if (!maxupdate)
		maxupdate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	if (!updaterate)
		updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	if (!interprate)
		interprate = INTERFACES::cvar->FindVar("cl_interp_ratio");
	if (!cmin)
		cmin = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	if (!cmax)
		cmax = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
	if (!interp)
		interp = INTERFACES::cvar->FindVar("cl_interp");

	float UpdateRate = updaterate->GetInt();
	float LerpRatio = interprate->GetInt();

	return max(LerpRatio / UpdateRate, interp->GetInt());
}

void CBacktrack::UpdateBacktrackRecords(SDK::CBaseEntity* pPlayer)
{
	int i = pPlayer->GetIndex();
	for (int j = backtracking->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		tick_record rec = backtracking->PlayerRecord[i].records.at(j);
		float amount = SETTINGS::settings.FakeLatencyAmount;
		float ping = SETTINGS::settings.FakeLatency ? amount : 0.2f;
		if (rec.m_flSimulationTime < INTERFACES::Globals->curtime - ping)
			backtracking->PlayerRecord[i].records.erase(backtracking->PlayerRecord[i].records.begin() + j);

	}
	static Vector old_origin[64];
	if (PlayerRecord[i].records.size() > 0 && pPlayer->GetSimTime() == PlayerRecord[i].records.back().m_flSimulationTime) //already got such a record
		return;

	for (int j = backtracking->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		tick_record rec = backtracking->PlayerRecord[i].records.at(j);

		switch (SETTINGS::settings.TickType)
		{
		case 0:
			rec = backtracking->PlayerRecord[i].records.at(0);
			break;
		case 1:
			rec = backtracking->PlayerRecord[i].records.at(j);
			break;
		}

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

	PlayerRecord[i].LowerBodyYawTarget = pPlayer->LowerBodyYaw();
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
	new_record.m_flUpdateTime = INTERFACES::Globals->curtime;
	new_record.backtrack_time = new_record.m_flSimulationTime + GetLerpTime();
	new_record.m_vecMins = pPlayer->GetCollision()->VecMins();
	new_record.m_vecMax = pPlayer->GetCollision()->VecMaxs();
	new_record.ragpos = pPlayer->get_ragdoll_pos();

	if (PlayerRecord[i].LowerBodyYawTarget != OldLower[i])
		new_record.bLowerBodyYawUpdated = true;

	for (int i = 0; i < 24; i++)
		new_record.m_flPoseParameter[i] = *(float*)((DWORD)pPlayer + OFFSETS::m_flPoseParameter + sizeof(float) * i);

	pPlayer->SetupBones(new_record.boneMatrix, 128, 256, INTERFACES::Globals->curtime);

	OldLower[i] = PlayerRecord[i].LowerBodyYawTarget;
	PlayerRecord[i].records.push_back(new_record);
}


void CBacktrack::UpdateExtrapolationRecords(SDK::CBaseEntity* pPlayer)
{
	int index = pPlayer->GetIndex();
	if (pPlayer->GetSimTime() == this->SimRecord[index][0].simulation_time)
		return;

	for (int i = 7; i > 0; i--) {
		this->SimRecord[index][i].acceleration = this->SimRecord[index][i - 1].acceleration;
		this->SimRecord[index][i].origin = this->SimRecord[index][i - 1].origin;
		this->SimRecord[index][i].simulation_time = this->SimRecord[index][i - 1].simulation_time;
		this->SimRecord[index][i].update_time = this->SimRecord[index][i - 1].update_time;
		this->SimRecord[index][i].velocity = this->SimRecord[index][i - 1].velocity;
	}

	this->SimRecord[index][0].simulation_time = pPlayer->GetSimTime();
	this->SimRecord[index][0].update_time = INTERFACES::Globals->curtime;
	this->SimRecord[index][0].origin = pPlayer->GetVecOrigin();

	int lost_ticks = TIME_TO_TICKS(this->SimRecord[index][0].simulation_time) - TIME_TO_TICKS(this->SimRecord[index][1].simulation_time);

	this->SimRecord[index][0].simulation_time_increasment_per_tick = (this->SimRecord[index][0].simulation_time - this->SimRecord[index][1].simulation_time) / lost_ticks;

	Vector velocity = this->SimRecord[index][0].origin - this->SimRecord[index][1].origin;

	velocity /= lost_ticks;

	this->SimRecord[index][0].velocity = pPlayer->GetVelocity();
}


void CBacktrack::backtrack_player(SDK::CUserCmd* cmd)
{
	for (int i = 1; i < 65; ++i)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!entity)
			continue;

		if (!local_player)
			continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player)
			continue;

		if (is_teammate)
			continue;

		if (entity->GetHealth() <= 0)
			continue;

		if (local_player->GetHealth() <= 0)
			continue;

		if (entity->GetImmunity())
			continue;

		int index = entity->GetIndex();

		for (int i = 0; i < 20; i++)
		{
			backtrack_hitbox[index][i][cmd->command_number % 12] = aimbot->get_hitbox_pos(entity, i);
		}

		backtrack_simtime[index][cmd->command_number % 12] = entity->GetSimTime();

		for (int i = 0; i < 12; i++)
		{
			if (backtrack_simtime[index][i] != compensate[index][i])
			{
				if (i > 0 && i != 11)
				{
					oldest_tick[index] = i + 2;
				}
				else
				{
					oldest_tick[index] = 1;
				}
				compensate[index][i] = backtrack_simtime[index][i];
			}
		}
	}
}









void CBacktrack::UpdateIncomingSequences()
{
	auto nci = INTERFACES::Engine->GetNetChannel();
	if (!nci)
		return;

	if (nci->m_nInSequenceNr > last_incoming_sequence || !nci->m_nInSequenceNr)
	{
		last_incoming_sequence = nci->m_nInSequenceNr;

		sequence_records.push_front(Incoming_Sequence_Record(nci->m_nInReliableState, nci->m_nOutReliableState, nci->m_nInSequenceNr, INTERFACES::Globals->realtime));
	}

	if (sequence_records.size() > 2048)
		sequence_records.pop_back();
}

void CBacktrack::AddLatency(SDK::NetChannel* net_channel, float latency)
{
	for (const auto& sequence : sequence_records)
	{
		if (INTERFACES::Globals->realtime - sequence.time >= latency)
		{
			net_channel->m_nInReliableState = sequence.in_reliable_state;
			net_channel->m_nInSequenceNr = sequence.in_sequence_num;
			break;
		}
	}
}

inline Vector CBacktrack::angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(M_PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float CBacktrack::point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}
resolver_player_t * CBacktrack::get_records(int index) {
	if (index < 0)
		return nullptr;

	return &players[index];
}

void CBacktrack::run_legit(SDK::CUserCmd* cmd) //phook backtrack muahhahahahaaha
{
	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;
	SDK::player_info_t info;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity) continue;
		if (!INTERFACES::Engine->GetPlayerInfo(i, &info)) continue;
		if (entity->GetIsDormant()) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		if (local_player->GetHealth() <= 0) return;

		float simtime = entity->GetSimTime(); Vector hitboxPos = aimbot->get_hitbox_pos(entity, 0);
		headPositions[i][cmd->command_number % 12] = legit_backtrackdata{ simtime, hitboxPos };
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetPunchAngles() * 2.f));
		float FOVDistance = point_to_line(hitboxPos, local_player->GetEyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}

	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetPunchAngles() * 2.f));

		for (int t = 0; t < 12; ++t)
		{
			float tempFOVDistance = point_to_line(headPositions[bestTargetIndex][t].hitboxPos, local_player->GetEyePosition(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > local_player->GetSimTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
			}
		}

		if (cmd->buttons & IN_ATTACK)
			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}
bool CBacktrack::IsValid(SDK::CBaseEntity * player)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	if (!player)
		return false;

	if (player->GetIsDormant() || player->GetHealth() == 0 || player->GetFlags() & FL_FROZEN)
		return false;

	if (player->GetTeam() == local_player->GetTeam())
		return false;

	if (player->GetClientClass()->m_ClassID != 35)
		return false;

	if (player == local_player)
		return false;

	if (player->GetImmunity())
		return false;

	return true;
}

CBacktrack* backtracking = new CBacktrack();
legit_backtrackdata headPositions[64][12];
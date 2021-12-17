#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Movement.h"
#include "../FEATURES/bt.h"

CBacktrackHelper* g_BacktrackHelper = new CBacktrackHelper;

int CBacktrackHelper::GetLatencyTicks()
{
	double v0; // st7@0
SDK::INetChannelInfo* v1; // esi@1
SDK::INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();
	v2 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(0);
	v4 = v2->GetAvgLatency(1);

	float interval_per_tick = 1.0f / INTERFACES::Globals->interval_per_tick;

	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
}
bool CBacktrackHelper::IsTickValid(tick_record record)
{
	float correct = 0;

	correct += INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	correct += INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
	correct += g_BacktrackHelper->GetLerpTime();

	static SDK::ConVar* sv_maxunlag = INTERFACES::cvar->FindVar("sv_maxunlag");
	correct = clamp<float>(correct, 0.0f, sv_maxunlag->GetFloat());

	float deltaTime = correct - (INTERFACES::Globals->curtime - record.m_flSimulationTime);



	return fabsf(deltaTime) < 0.2f;
}
float CBacktrackHelper::GetEstimateServerTime()
{
	double v0; // st7@0
	SDK::INetChannelInfo* v1; // esi@1
	SDK::INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();
	v2 = (SDK::INetChannelInfo*)INTERFACES::Engine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(0);
	v4 = v2->GetAvgLatency(1);

	//return floorf(((v3 + v4) / INTERFACES::Globals->interval_per_tick) + 0.5f) + 1 + G::UserCmdForBacktracking->tick_count;*/

	return v3 + v4 + TICKS_TO_TIME(1) + TICKS_TO_TIME(GLOBAL::cmd->tick_count);

}
float CBacktrackHelper::GetNetworkLatency()
{
	// Get true latency
	SDK::INetChannelInfo *nci = INTERFACES::Engine->GetNetChannelInfo();
	if (nci)
	{
		//float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING); //ppl say use only this one, but meh
		float OutgoingLatency = nci->GetLatency(0);
		return OutgoingLatency;
	}
	return 0.0f;
}
SDK::ConVar* minupdate;
SDK::ConVar* maxupdate;
SDK::ConVar * updaterate;
SDK::ConVar * interprate;
SDK::ConVar* cmin;
SDK::ConVar* cmax;
SDK::ConVar* interp;
float CBacktrackHelper::GetLerpTime()
{
	if (!minupdate)
		minupdate = INTERFACES::cvar->FindVar(("sv_minupdaterate"));
	if (!maxupdate)
		maxupdate = INTERFACES::cvar->FindVar(("sv_maxupdaterate"));
	if (!updaterate)
		updaterate = INTERFACES::cvar->FindVar(("cl_updaterate"));
	if (!interprate)
		interprate = INTERFACES::cvar->FindVar(("cl_interp_ratio"));
	if (!cmin)
		cmin = INTERFACES::cvar->FindVar(("sv_client_min_interp_ratio"));
	if (!cmax)
		cmax = INTERFACES::cvar->FindVar(("sv_client_max_interp_ratio"));
	if (!interp)
		interp = INTERFACES::cvar->FindVar(("cl_interp"));

	float UpdateRate = updaterate->GetFloat();
	float LerpRatio = interprate->GetFloat();

	return max(LerpRatio / UpdateRate, interp->GetFloat());
}

void CBacktrackHelper::UpdateBacktrackRecords(SDK::CBaseEntity* pPlayer)
{
	int i = pPlayer->GetIndex();

	for (int j = g_BacktrackHelper->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		float lerptime = g_BacktrackHelper->GetLerpTime();
		float desired_time = g_BacktrackHelper->PlayerRecord[i].records.at(j).m_flSimulationTime + lerptime;
		float estimated_time = g_BacktrackHelper->GetEstimateServerTime();
		float SV_MAXUNLAG = 1.0f;
		float latency = g_BacktrackHelper->GetNetworkLatency();
		float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
		float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);
		float deltaTime = correct - (estimated_time + lerptime - desired_time);

		if (fabs(deltaTime) > 0.2f)
			g_BacktrackHelper->PlayerRecord[i].records.erase(g_BacktrackHelper->PlayerRecord[i].records.begin() + j);
	}

	static Vector old_origin[64];

	if (PlayerRecord[i].records.size() > 0 && pPlayer->GetSimTime() == PlayerRecord[i].records.back().m_flSimulationTime) //already got such a record
		return;

	if (PlayerRecord[i].records.size() > 0 && PlayerRecord[i].records.back().m_flSimulationTime > pPlayer->GetSimTime())//Invalid lag record, maybe from diffrent game?
	{
		PlayerRecord[i].records.clear();
		return;
	}

	Vector cur_origin = pPlayer->GetVecOrigin();
	Vector v = cur_origin - old_origin[i];
	bool breaks_lagcomp = v.LengthSqr() > 4096.f;
	old_origin[i] = cur_origin;
	tick_record new_record;

	new_record.needs_extrapolation = breaks_lagcomp;
	static float OldLower[64];

	PlayerRecord[i].LowerBodyYawTarget = pPlayer->GetLowerBodyYaw();
	new_record.m_angEyeAngles = pPlayer->GetEyeAngles();
	
	new_record.m_flSimulationTime = pPlayer->GetSimTime();

	new_record.bLowerBodyYawUpdated = false;

	new_record.m_vecOrigin = pPlayer->GetVecOrigin();
	new_record.m_vecVelocity = pPlayer->GetVelocity();
	new_record.m_flUpdateTime = INTERFACES::Globals->curtime;
	new_record.backtrack_time = new_record.m_flSimulationTime + GetLerpTime();

	if (PlayerRecord[i].LowerBodyYawTarget != OldLower[i] || (pPlayer->GetFlags() & FL_ONGROUND && pPlayer->GetVelocity().Length() > 29.f))
		new_record.bLowerBodyYawUpdated = true;

	for (int i = 0; i < 24; i++)
		new_record.m_flPoseParameter[i] = *(float*)((DWORD)pPlayer + OFFSETS::m_flPoseParameter + sizeof(float) * i);
	
	pPlayer->SetupBones(new_record.boneMatrix, 128, 0x00000100, INTERFACES::Globals->curtime);

	OldLower[i] = PlayerRecord[i].LowerBodyYawTarget;
	PlayerRecord[i].records.push_back(new_record);
}

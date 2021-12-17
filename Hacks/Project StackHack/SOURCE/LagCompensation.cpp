#include "LagCompensation.h"
#include "FEATURES\Aimbot.h"
#include "SDK\CBaseEntity.h"
#include "SDK\CGlobalVars.h"
#include "SDK\ConVar.h"

LagComp g_LagComp;

float LagComp::LerpTime() // GLAD https://github.com/sstokic-tgm/Gladiatorcheatz-v2.1/blob/eaa88bbb4eca71f8aebfed32a5b86300df8ce6a3/features/LagCompensation.cpp
{
	int updaterate = INTERFACES::cvar->FindVar("cl_updaterate")->GetInt();
	SDK::ConVar* minupdate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	SDK::ConVar* maxupdate = INTERFACES::cvar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = INTERFACES::cvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = INTERFACES::cvar->FindVar("cl_interp")->GetFloat();
	SDK::ConVar* cmin = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	SDK::ConVar* cmax = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clampa(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, (ratio / updaterate));
}

bool LagComp::ValidTick(int tick) // gucci i think cant remember
{
	auto nci =  INTERFACES::Engine->GetNetChannelInfo();

	if (!nci)
		return false;

	auto PredictedCmdArrivalTick = GLOBAL::originalCMD.tick_count + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
	auto Correct = clampa(LerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(PredictedCmdArrivalTick + TIME_TO_TICKS(LerpTime()) - (tick + TIME_TO_TICKS(LerpTime())));

	return (abs(Correct) <= 0.2f);
}

SDK::CBaseWeapon* GetActiveWeapon()
{
	SDK::CBaseEntity* xz = nullptr;
	if (!INTERFACES::Engine->IsConnected())
		return nullptr;
	if (!xz->IsAlive())
		return nullptr;

	HANDLE weaponData = *(HANDLE*)((DWORD)xz + OFFSETS::m_hActiveWeapon);
	return (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(weaponData);
}

void LagComp::StoreRecord(SDK::CBaseEntity* pEnt) // best lag comp in the world
{
	PlayerRecords Setup;

	auto pLocalEntity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (pEnt == nullptr) return;

	static float ShotTime[65];
	static float OldSimtime[65];

	if (pEnt != pLocalEntity)
		pEnt->FixSetupBones(g_LagComp.MatrixUWU[pEnt->GetIndex()]);

	if (PlayerRecord[pEnt->GetIndex()].size() == 0)
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.SimTime = pEnt->GetSimTime();

		memcpy(Setup.Matrix, g_LagComp.MatrixUWU[pEnt->GetIndex()], (sizeof(matrix3x4_t) * 128));

		Setup.Shot = false;

		PlayerRecord[pEnt->GetIndex()].push_back(Setup);
	}

	if (OldSimtime[pEnt->GetIndex()] != pEnt->GetSimTime())
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.SimTime = pEnt->GetSimTime();

		if (pEnt == pLocalEntity)
			pEnt->FixSetupBones(g_LagComp.MatrixUWU[pEnt->GetIndex()]);

		memcpy(Setup.Matrix, g_LagComp.MatrixUWU[pEnt->GetIndex()], (sizeof(matrix3x4_t) * 128));

		if (GetActiveWeapon() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->GetIndex()] != pEnt->IsKnifeorNade())
			{
				Setup.Shot = true;
				ShotTime[pEnt->GetIndex()] = GetActiveWeapon()->GetLastShotTime();
			}
			else
				Setup.Shot = false;
		}
		else
		{
			Setup.Shot = false;
			ShotTime[pEnt->GetIndex()] = 0.f;
		}

		PlayerRecord[pEnt->GetIndex()].push_back(Setup);

		OldSimtime[pEnt->GetIndex()] = pEnt->GetSimTime();
	}

	ShotTick[pEnt->GetIndex()] = -1;

	if (PlayerRecord[pEnt->GetIndex()].size() > 0)
		for (int tick = 0; tick < PlayerRecord[pEnt->GetIndex()].size(); tick++)
			if (!ValidTick(TIME_TO_TICKS(PlayerRecord[pEnt->GetIndex()].at(tick).SimTime)))
				PlayerRecord[pEnt->GetIndex()].erase(PlayerRecord[pEnt->GetIndex()].begin() + tick);
			else if (PlayerRecord[pEnt->GetIndex()].at(tick).Shot)
				ShotTick[pEnt->GetIndex()] = tick; // gets the newest shot tick
}

void LagComp::ClearRecords(int i)
{
	if (PlayerRecord[i].size() > 0)
	{
		for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
		{
			PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
		}
	}
}

template<class T, class U>
T LagComp::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
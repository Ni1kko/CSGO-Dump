#include "../Sp1Ky Private/backtrack.h"
#include "../Sp1Ky Private/aimbot.h"
#include "../Sp1Ky Private/helpers/math.hpp"
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
void TimeWarp::CreateMove(CUserCmd* userCMD)
{
	if (!g_Options.total_total_l_m_exi__kero) return;
	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;
	if (!g_LocalPlayer->IsAlive())
		return;
	for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
	{
		auto pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
		if (!pEntity || !g_LocalPlayer) continue;
		if (!pEntity->IsPlayer()) continue;
		if (pEntity == g_LocalPlayer) continue;
		if (pEntity->IsDormant()) continue;
		if (!pEntity->IsAlive()) continue;
		if (pEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) continue;
		float simtime = pEntity->m_flSimulationTime();
		Vector hitboxPos = pEntity->GetHitboxPos(0);
		QAngle viewangle = pEntity->m_angEyeAngles();
		Vector origin = pEntity->GetRenderOrigin();
		Vector headpos = pEntity->GetBonePos(8);
		TimeWarpData[i][userCMD->command_number % (NUM_OF_TICKS + 12)] = StoredData{ simtime, hitboxPos, origin, viewangle, headpos };
		Vector ViewDir;
		Math::AngleVectors(userCMD->viewangles + (g_LocalPlayer->m_aimPunchAngle() * 2.f), ViewDir);
		float FOVDistance = Math::DistancePointToLine(hitboxPos, g_LocalPlayer->GetEyePos(), ViewDir);
		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}
	float bestTargetSimTime = -1;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir;
		Math::AngleVectors(userCMD->viewangles + (g_LocalPlayer->m_aimPunchAngle() * 2.f), ViewDir);
		for (int t = 0; t < NUM_OF_TICKS; ++t)
		{
			float tempFOVDistance = Math::DistancePointToLine(TimeWarpData[bestTargetIndex][t].hitboxPos, g_LocalPlayer->GetEyePos(), ViewDir);
			if (tempFloat > tempFOVDistance && TimeWarpData[bestTargetIndex][t].simtime > g_LocalPlayer->m_flSimulationTime() - 1)
			{
				if (g_LocalPlayer->CanSeePlayer(static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(bestTargetIndex)), TimeWarpData[bestTargetIndex][t].hitboxPos))
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = TimeWarpData[bestTargetIndex][t].simtime;
				}
			}
		}
		if (bestTargetSimTime >= 0 && userCMD->buttons & IN_ATTACK)
			userCMD->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}






















































































































































































































































































































#include "LagComp.h"
#include "../Aimbot/Aimbot.h"
using namespace SDK;
using namespace Client;






void BackTrack::Update(int tick_count)
{
	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{
	int delta = latest_tick - tick;
	float deltaTime = delta * Interfaces::GlobalVars()->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void BackTrack::UpdateRecord(int i)
{
	CBaseEntity* pEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);
	if (pEntity && !pEntity->IsDead() && !pEntity->IsDormant())
	{
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = pEntity->GetHitboxPosition(0);
		}
	}
	else
	{
		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

void BackTrack::legitBackTrack(CUserCmd* cmd)
{
	
	//m_pLocal = nullptr;
	if (Settings::Aimbot::aim_Backtrack && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		PlayerInfo info;
		if (pLocal->IsDead())
			return;

		for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++)
		{
			auto entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!Interfaces::Engine()->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == pLocal->GetTeam())
				continue;

			if (!entity->IsDead())
			{

				

				//m_pLocal->WeaponType;
				float simtime = entity->GetSimTime();
				Vector hitboxPos = entity->GetHitboxPosition(0);
				

				headPositions[i][cmd->command_number % Settings::Aimbot::aim_Globalbacktrack] = backtrackData{ simtime, hitboxPos };

				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			//Repetimos el for para todos los tipos de arma
			/*if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
			{

				for (int t = 0; t < Settings::Aimbot::aim_backtrack[0]; ++t)
				{
					float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
					if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
					{
						tempFloat = tempFOVDistance;
						bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
					}
				}
			}
				
			if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
			{
				for (int t = 0; t < Settings::Aimbot::aim_backtrack[1]; ++t)
				{
					float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
					if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
					{
						tempFloat = tempFOVDistance;
						bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
					}
				}
			}
				
				if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
				{
					for (int t = 0; t < Settings::Aimbot::aim_backtrack[2]; ++t)
					{
						float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
						if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
						{
							tempFloat = tempFOVDistance;
							bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
						}
					}
				}
				
				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
				{
					for (int t = 0; t < Settings::Aimbot::aim_backtrack[3]; ++t)
					{
						float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
						if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
						{
							tempFloat = tempFOVDistance;
							bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
						}
					}
			}
				
				if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
				{
					for (int t = 0; t < Settings::Aimbot::aim_backtrack[4]; ++t)
					{
						float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
						if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
						{
							tempFloat = tempFOVDistance;
							bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
						}
					}
			}
				
				if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
				{
					for (int t = 0; t < Settings::Aimbot::aim_backtrack[5]; ++t)
					{
						float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
						if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
						{
							tempFloat = tempFOVDistance;
							bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
						}
					}
			}
				*/

			for (int t = 0; t < Settings::Aimbot::aim_Globalbacktrack; ++t)
			{
				float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			

			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
		}
	}
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][25]; //support for 128tick servers
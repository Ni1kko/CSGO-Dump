#include "LagComp.h"
#include "RageBot.h"
void BackTrack::Update(int tick_count)
{
    if (!g_Options.Ragebot.FakeLagFix)
        return;

    latest_tick = tick_count;
    for (int i = 0; i < 64; i++)
    {
        UpdateRecord(i);
    }
}

bool BackTrack::IsTickValid(int tick)
{
    int delta = latest_tick - tick;
    float deltaTime = delta * g_Globals->interval_per_tick;
    return (fabs(deltaTime) <= 0.2f);
}

void BackTrack::UpdateRecord(int i)
{
    C_BaseEntity* pEntity = g_EntityList->GetClientEntity(i);
    if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
    {
        float lby = pEntity->GetLowerBodyYaw();
        if (lby != records[i].lby)
        {
            records[i].tick_count = latest_tick;
            records[i].lby = lby;
            records[i].headPosition = GetHitboxPosition(pEntity, 0);
        }
    }
    else
    {
        records[i].tick_count = 0;
    }
}

bool BackTrack::RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint)
{
    if (IsTickValid(records[i].tick_count))
    {
        aimPoint = records[i].headPosition;
        cmd->tick_count = records[i].tick_count;
        return true;
    }
    return false;
}

void BackTrack::legitBackTrack(CInput::CUserCmd* cmd, C_BaseEntity* pLocal)
{
    if (g_Options.Legitbot.backtrack)
    {
        int bestTargetIndex = -1;
        float bestFov = FLT_MAX;
        player_info_t info;
        if (!pLocal->IsAlive())
            return;

        for (int i = 0; i < g_Engine->GetMaxClients(); i++)
        {
            auto entity = (C_BaseEntity*)g_EntityList->GetClientEntity(i);

            if (!entity || !pLocal)
                continue;

            if (entity == pLocal)
                continue;

            if (!g_Engine->GetPlayerInfo(i, &info))
                continue;

            if (entity->IsDormant())
                continue;

            if (entity->GetTeamNum() == pLocal->GetTeamNum())
                continue;

            if (entity->IsAlive())
            {
				
                float simtime = entity->GetSimulationTime();
                Vector hitboxPos = GetHitboxPosition(entity, 0);

                headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
                Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
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
            Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
            for (int t = 0; t < 12; ++t)
            {
                float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
                if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
                {
                    tempFloat = tempFOVDistance;
                    bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
                }
            }
            if (cmd->buttons & IN_ATTACK)
            {
                cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
            }
        }
    }
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];
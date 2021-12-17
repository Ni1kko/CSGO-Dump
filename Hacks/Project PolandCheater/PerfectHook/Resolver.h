#pragma once
#include "HookIncludes.h"


int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf( float(( float ) ( ( float ) ( latency ) / ( float ) ( ( uintptr_t ) &g_Globals->interval_per_tick ) ) + 0.5 ) + 1 + ( int ) ( (uintptr_t ) &g_Globals->tickcount ));
}

float oldlowerbodyyaw = 0;
/*void Resolver3()
{
    C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

    for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
    {
        C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);
        if (!pEnt) continue;

        if ((pEnt == pLocal)) continue;

        if (pEnt->IsDormant()) continue;

        player_info_t pTemp;

        if (!g_Engine->GetPlayerInfo(i, &pTemp))
            continue;
        auto FYaw = pEnt->GetLowerBodyYaw();
        auto pitch = pEnt->GetEyeAngles()->x;
        if (menu.Ragebot.Resolver)
        {
            pEnt->GetEyeAngles()->y = FYaw;
            float PlayerIsMoving = abs(pEnt->GetVelocity().Length2D());
            bool bLowerBodyUpdated = false;
            bool IsUsingFakeAngles = false;

            if (oldlowerbodyyaw != FYaw)
            {
                bLowerBodyUpdated = true;
            }
            float bodyeyedelta = fabs(pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw());
            if (PlayerIsMoving || bLowerBodyUpdated)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
            {
                pEnt->GetEyeAngles()->y = FYaw;
                oldlowerbodyyaw = pEnt->GetEyeAngles()->y;

                IsUsingFakeAngles = false;
            }
            else
            {
                if (bodyeyedelta > 35.0f)
                {
                    pEnt->GetEyeAngles()->y = oldlowerbodyyaw;
                    IsUsingFakeAngles = true;
                }
                else
                {
                    IsUsingFakeAngles = false;
                }
            }
            if (IsUsingFakeAngles)
            {

                pEnt->GetEyeAngles()->y += 45;


            }
        }
    }

}*/
inline float ClampYaw(float yaw) {
    while (yaw > 180.f)
        yaw -= 360.f;
    while (yaw < -180.f)
        yaw += 360.f;
    return yaw;
}
void Resolver3()
{



    if (g_Options.Ragebot.Resolver)
    {
        C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
        {
            for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
            {
                C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);
                if (!pEnt) continue;

                if (pEnt == pLocal) continue;

                if (pEnt->IsDormant()) continue;

                player_info_t pTemp;

                if (!g_Engine->GetPlayerInfo(i, &pTemp))
                    continue;

                if (pEnt->GetVelocity().Length2D() > 36)
                {
                    pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw();
                }
                else
                {
                    switch(pEnt->m_iShotsFired() % 4)
                    {
                    case 1:
                        pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw();
                        break;
                    case 2:
                        pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + 45;
                        break;
                    case 3:
                        pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() - 45;
                        break;
                    case 4:
                        pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + 180;
                        break;

                    }

                }
            }
        }
    }
}

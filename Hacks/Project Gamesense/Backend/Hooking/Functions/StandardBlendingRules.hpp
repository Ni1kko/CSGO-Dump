#pragma once

#include "../Hooks/Hooks.h"
#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"

typedef void(__fastcall* fnStandardBlendingRules)(CBaseEntity*, studiohdr_t*, Vector*, void*, float, int);
fnStandardBlendingRules oStandardBlendingRules = nullptr;

namespace Cheat
{
    namespace Hooked
    {
        void __fastcall StandardBlendingRules(CBaseEntity* player, void* edx, CStudioHdr* hdr, Vector* pos, Quaternion* q, float curTime, int boneMask)
        {
            if (Cheat::Settings->UnloadCheat)
            {
                Cheat::Settings->UnloadReadyTable.StandartBlendingRules = 0;
                oStandardBlendingRules(player, (studiohdr_t*)hdr, pos, q, curTime, boneMask);
                return;
            }
            Cheat::Settings->UnloadReadyTable.StandartBlendingRules = (int)GetTickCount();

            auto backup_effects = player->GetEffects();

            if (player == G::LocalPlayer)
                player->GetEffects() |= 8;

            oStandardBlendingRules(player, (studiohdr_t*)hdr, pos, q, curTime, boneMask);

            if (player == G::LocalPlayer)
                player->GetEffects() = backup_effects;
        }
    }
}
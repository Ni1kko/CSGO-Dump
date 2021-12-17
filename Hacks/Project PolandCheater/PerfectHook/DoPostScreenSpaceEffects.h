#pragma once
#include "HookIncludes.h"
using do_post_screen_space_effects_t = bool(__thiscall*)(void*, CViewSetup*);



bool _fastcall hkDoPostScreenSpaceEffects(void* ecx, void* edx, CViewSetup* pSetup)
{
    static auto ofunc = hooks::clientmode.get_original<do_post_screen_space_effects_t>(44);

    IMaterial *pMatGlowColor = g_MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);
    g_ModelRender->ForcedMaterialOverride(pMatGlowColor);

    if (g_Options.Visuals.Glow && g_GlowObjManager && g_Engine->IsConnected())
    {
        auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
        if (local)
        {
            for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); ++i)
            {
                if (g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
                    continue;

                CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &g_GlowObjManager->m_GlowObjectDefinitions[i];

                switch (glowEnt->getEnt()->GetClientClass()->m_ClassID)
                {
                default:
                        if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")))
                            glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                case 1:
                        glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                case 35:

                        if (g_Options.Visuals.Filter.EnemyOnly && glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
                            break;

                        if (glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
                            glowEnt->set(0.23921f, 0.55294f, 0.89019f, 0.7f);
                        else if (glowEnt->getEnt()->GetTeamNum() != local->GetTeamNum())
                            glowEnt->set(0.89019f, 0.23137f, 0.23137f, 0.7f);
                    break;
                case 39:
                        glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                }
            }
        }
    }
    return ofunc(ecx, pSetup);
}
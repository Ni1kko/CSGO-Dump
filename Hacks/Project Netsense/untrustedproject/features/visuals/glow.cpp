#include "glow.hpp"
#include "visuals.hpp"

Glow* g_Glow = new Glow();

Glow::Glow()
{
}

Glow::~Glow()
{
    // We cannot call shutdown here unfortunately.
    // Reason is not very straightforward but anyways:
    // - This destructor will be called when the dll unloads
    //   but it cannot distinguish between manual unload 
    //   (pressing the Unload button or calling FreeLibrary)
    //   or unload due to game exit.
    //   What that means is that this destructor will be called
    //   when the game exits.
    // - When the game is exiting, other dlls might already 
    //   have been unloaded before us, so it is not safe to 
    //   access intermodular variables or functions.
    //   
    //   Trying to call Shutdown here will crash CSGO when it is
    //   exiting (because we try to access g_GlowObjManager).
    //
}

void Glow::Shutdown()
{
    // Remove glow from all entities
    for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if (glowObject.IsUnused())
            continue;

        if (!entity || entity->IsDormant())
            continue;

        glowObject.m_flAlpha = 0.0f;
    }
}

void Glow::Run() // TODO: Fix that shit and support more targets
{
    for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if (glowObject.IsUnused())
            continue;

        if (!entity || entity->IsDormant())
            continue;

        auto class_id = entity->GetClientClass()->m_ClassID;
        auto color = Color(255, 255, 255);

        switch (class_id) {
        case ClassId_CCSPlayer:
        {
            auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

            if (entity->HasC4() && is_enemy && g_Visuals->glow.c4_carrier) {
                color = g_Visuals->glow.c4_carrier_clr;
                break;
            }

            if (!g_Visuals->glow.players || !entity->IsAlive())
                continue;

            if (!is_enemy && !g_Visuals->glow.on_team)
                continue;

            if (!g_LocalPlayer->CanSeePlayer(entity, entity->GetEyePos())) {
                if (is_enemy) {
                    color.SetColor(g_Visuals->glow.enemy_invis_clr.r(), g_Visuals->glow.enemy_invis_clr.g(), g_Visuals->glow.enemy_invis_clr.b(), g_Visuals->glow.enemy_invis_clr.a());
                }
                else {
                    color.SetColor(g_Visuals->glow.allies_invis_clr.r(), g_Visuals->glow.allies_invis_clr.g(), g_Visuals->glow.allies_invis_clr.b(), g_Visuals->glow.allies_invis_clr.a());
                }
            }
            else {
                if (is_enemy) {
                    color.SetColor(g_Visuals->glow.enemy_clr.r(), g_Visuals->glow.enemy_clr.g(), g_Visuals->glow.enemy_clr.b(), g_Visuals->glow.enemy_clr.a());
                }
                else {
                    color.SetColor(g_Visuals->glow.allies_clr.r(), g_Visuals->glow.allies_clr.g(), g_Visuals->glow.allies_clr.b(), g_Visuals->glow.allies_clr.a());
                }
            }
            break;
        }
        case ClassId_CChicken:
            if (!g_Visuals->glow.on_chickens)
                continue;
            entity->m_bShouldGlow() = true;
            color.SetColor(g_Visuals->glow.chickens_clr.r(), g_Visuals->glow.chickens_clr.g(), g_Visuals->glow.chickens_clr.b(), g_Visuals->glow.chickens_clr.a());
            break;
        case ClassId_CBaseAnimating:
            if (!g_Visuals->glow.defuse_kits)
                continue;
            color.SetColor(g_Visuals->glow.defuse_kits_clr.r(), g_Visuals->glow.defuse_kits_clr.g(), g_Visuals->glow.defuse_kits_clr.b(), g_Visuals->glow.defuse_kits_clr.a());
            break;
        case ClassId_CPlantedC4:
            if (!g_Visuals->glow.c4_planted)
                continue;
            color.SetColor(g_Visuals->glow.c4_planted_clr.r(), g_Visuals->glow.c4_planted_clr.g(), g_Visuals->glow.c4_planted_clr.b(), g_Visuals->glow.c4_planted_clr.a());
            break;
        default:
        {
            if (entity->IsWeapon()) {
                if (!g_Visuals->glow.weapons)
                    continue;
                color.SetColor(g_Visuals->glow.weapons_clr.r(), g_Visuals->glow.weapons_clr.g(), g_Visuals->glow.weapons_clr.b(), g_Visuals->glow.weapons_clr.a());
            }
            else
                continue;
        }
        }

        glowObject.m_flRed = color.r() / 255.0f;
        glowObject.m_flGreen = color.g() / 255.0f;
        glowObject.m_flBlue = color.b() / 255.0f;
        glowObject.m_flAlpha = color.a() / 255.f;
        glowObject.m_bRenderWhenOccluded = true;
        glowObject.m_bRenderWhenUnoccluded = false;
        glowObject.m_nGlowStyle = g_Visuals->glow.type;
        glowObject.m_bFullBloomRender = g_Visuals->glow.bloom;
    }
}
#include "hooks.hpp"

#include "Options.hpp"
#include "Menu.hpp"

#include "Helpers/math.hpp"
#include "Helpers/input_system.hpp"
#include "Helpers/utils.hpp"
#include "ValveSDK/csgostructs.hpp"

#include "features/bhop.hpp"
#include "features/visuals.hpp"
#include "features/LegitBot.h"
#include "features/AutoStrafe.hpp"
#include "features/KnifeChanger.h"
#include "features/GrenadePrediction.h"
#include "features/FakeLag.hpp"
#include "Proxies.h"
#include "Listener.hpp"
#define MAXBACKTRACKTICKS 32
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
backtrackData headPositions[64][16];
namespace hooks
{
    vfunc_hook client_hook;
    vfunc_hook d3d_hook;
    vfunc_hook panel_hook;
    vfunc_hook modelrender_hook;
    vfunc_hook vgui_surface_hook;
    vfunc_hook enginetrace_hook;
    vfunc_hook client_mode_hook;
    vfunc_hook engine_hook;
    vfunc_hook steam_networking;

    //--------------------------------------------------------------------------------
    void initialize()
    {
        g_ConfigScheme.load_config();

        menu::instance().initialize(g_D3DDevice9);

        client_hook.setup(g_CHLClient);
        client_mode_hook.setup(g_ClientMode);
        d3d_hook.setup(g_D3DDevice9);
        panel_hook.setup(g_VGuiPanel);
        modelrender_hook.setup(g_MdlRender);
        vgui_surface_hook.setup(g_VGuiSurface);
        enginetrace_hook.setup(g_EngineTrace);
        engine_hook.setup(g_EngineClient);


        steam_networking.setup(g_SteamNetworkingUtils);

        //steam_networking.hook_index(6, received_ticket);

        panel_hook.hook_index(41, paint_traverse);


        client_hook.hook_index(36, frame_stage_notify);
        client_hook.hook_index(21, create_move_proxy);

        vgui_surface_hook.hook_index(82, play_sound);

        d3d_hook.hook_index(16, reset);
        d3d_hook.hook_index(42, end_scene);

        modelrender_hook.hook_index(21, draw_model_execute);

        enginetrace_hook.hook_index(5, trace_ray);

        client_mode_hook.hook_index(18, override_view);
        client_mode_hook.hook_index(35, get_view_model_fov);
        client_mode_hook.hook_index(44, do_post_screen_space_effects);

        engine_hook.hook_index(93, is_hltv); // super private pvs-fix method, no leak/share or get raped

        //fnSequenceProxyFn = netvars.hook_prop("DT_BaseViewModel", "m_nSequence", sequence_hook);
        fnNoSmoke = netvars.hook_prop("DT_SmokeGrenadeProjectile", "m_bDidSmokeEffect", no_smoke_hook);


        item_purchase::singleton()->initialize();
        round_start::singleton()->initialize();
        visuals::create_fonts();
    }
    //--------------------------------------------------------------------------------
    void cleanup()
    {
        //netvars.hook_prop("DT_BaseViewModel", "m_nSequence", fnSequenceProxyFn);
        item_purchase::singleton()->remove();
        round_start::singleton()->remove();
        netvars.hook_prop("DT_SmokeGrenadeProjectile", "m_bDidSmokeEffect", fnNoSmoke);
        client_hook.unhook_all();
        client_mode_hook.unhook_all();
        d3d_hook.unhook_all();
        panel_hook.unhook_all();
        modelrender_hook.unhook_all();
        vgui_surface_hook.unhook_all();
        //game_events_hook.unhook_all();
        enginetrace_hook.unhook_all();
        engine_hook.unhook_all();
        steam_networking.unhook_all();
        menu::instance().shutdown();
    }
    //--------------------------------------------------------------------------------
    long __stdcall end_scene(IDirect3DDevice9* device)
    {
        static auto ofunc = d3d_hook.get_original<d3d9_endscene>(42);
        if (g_Options.esp_poland_filter_enemies_only.get_bool() && checkteams == false) checkteams = true;
        if(!g_Options.esp_poland_filter_enemies_only.get_bool() && checkteams == true) checkteams = false;
        menu::instance().render();

        return ofunc(device);
    }
    //--------------------------------------------------------------------------------
    long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        static auto ofunc = d3d_hook.get_original<d3d9_reset>(16);

        menu::instance().on_device_lost();
        visuals::create_fonts();

        auto hr = ofunc(device, pPresentationParameters);

        visuals::destroy_fonts();
        menu::instance().on_device_reset();

        return hr;
    }
    bool check_for_matchid = false;
    void __stdcall received_ticket(int a1)
    {
        static auto ofunc = steam_networking.get_original<ticket_t>(6);

        auto protobuf = **(uintptr_t**)(a1 + 0x2C);

        auto plebian = *(uint64_t*)(protobuf + 0x18);
        auto plebian_matchid = std::to_string(plebian);
        utils::console_print("matchid: %s \n", plebian_matchid.c_str());

        ofunc(a1);
    }
    //--------------------------------------------------------------------------------
    Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
    QAngle qAimAngles;
    Vector vForwardNorm;
    Vector vRightNorm;
    Vector vUpNorm;
    using tServerRankRevealAllFn = bool(__cdecl*)(int*);
    static tServerRankRevealAllFn ServerRankRevealAll = (tServerRankRevealAllFn)(PDWORD)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ?");
    int fArray[3] = { 0,0,0 };
    void __stdcall chlcreate_move(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
    {
        auto ofunc = client_hook.get_original<create_move_t>(21);

        ofunc(g_CHLClient, sequence_number, input_sample_frametime, active);

        auto local = C_BasePlayer::get_local_player();
        auto cmd = g_Input->GetUserCmd(0, sequence_number);
        auto verified = g_Input->GetVerifiedCmd(0, sequence_number);
        auto weapon = local->m_hActiveWeapon().Get();

        if (!cmd || cmd->command_number == 0 || !local)
            return;


            int bestTargetIndex = -1;
            float bestFov = FLT_MAX;
            player_info_t info;
            for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
            {
                auto entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

                if (!entity || !local)
                    continue;

                if (entity == local)
                    continue;
                
                if (!g_EngineClient->GetPlayerInfo(i, &info))
                    continue;

                if (entity->IsDormant())
                    continue;

                if (checkteams)
                {
                    if (entity->m_iTeamNum() == local->m_iTeamNum())
                        continue;
                }

                if (entity->is_alive())
                {
                    float simtime = entity->m_flSimulationTime();
                    Vector hitboxPos = entity->get_hitbox_pos(0);

                    headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
                    Vector ViewDir = math::angle_vector(cmd->viewangles + (local->m_aimPunchAngle() * 2.f));
                    float FOVDistance = math::distance_point_to_line(hitboxPos, local->get_eye_pos(), ViewDir);

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

                Vector ViewDir = math::angle_vector(cmd->viewangles + (local->m_aimPunchAngle() * 2.f));

                for (int t = 0; t <= 12; ++t)
                {
                    float tempFOVDistance = math::distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, local->get_eye_pos(), ViewDir);

                    if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > g_GlobalVars->curtime - 1)
                    {

                        tempFloat = tempFOVDistance;
                        bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
                    }
                }
                /*if (cmd->buttons & IN_ATTACK2)
                {
                    QAngle imgay;
                    math::vector_angles((headPositions[bestTargetIndex][12].hitboxPos - local->get_eye_pos()), imgay);
                    g_EngineClient->SetViewAngles(imgay);
                }*/
        
                if (cmd->buttons & IN_ATTACK)
                {
                    cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);

                }

            }
        

        fake_lag::instance().on_create_move(local, cmd, bSendPacket);
        bhop::instance().on_create_move(local, cmd);
        auto_strafe::instance().on_create_move(cmd, local);
        legitbot::instance().on_create_move(cmd, local, weapon);
        grenade_prediction::instance().Tick(cmd->buttons);


        if (cmd->buttons & IN_SCORE)
        {
            ServerRankRevealAll(fArray);
        }

        GetAutostrafeView();

        if (cmd->forwardmove > 450) cmd->forwardmove = 450;
        if (cmd->sidemove > 450) cmd->sidemove = 450;
        if (cmd->upmove > 450) cmd->upmove = 450;
        if (cmd->forwardmove < -450) cmd->forwardmove = -450;
        if (cmd->sidemove < -450) cmd->sidemove = -450;
        if (cmd->upmove < -450) cmd->upmove = -450;



        if (!math::sanitize_angles(cmd->viewangles))
            return;

        verified->m_cmd = *cmd;
        verified->m_crc = cmd->GetChecksum();
    }
    //--------------------------------------------------------------------------------
    __declspec(naked) void __stdcall create_move_proxy(int sequence_number, float input_sample_frametime, bool active)
    {
        __asm
        {
            push ebp
            mov  ebp, esp
            push ebx
            lea  ecx, [esp]
            push ecx
            push dword ptr[active]//active
            push dword ptr[input_sample_frametime]//input_sample_frametime
            push dword ptr[sequence_number]//sequence_number
            call chlcreate_move
            pop  ebx
            pop  ebp
            retn 0Ch
        }
    }
    //--------------------------------------------------------------------------------
    float __fastcall get_view_model_fov(void* ecx, void* edx)
    {
        static auto ofunc = client_mode_hook.get_original<get_view_model_fov_t>(35)(ecx);
        ofunc = g_Options.viewmodel_fov.get_int();
        return ofunc;
    }
    void __fastcall override_view(void* _this, void* _edx, CViewSetup* setup)
    {
        static auto ofunc = client_mode_hook.get_original<override_view_t>(18);
        auto local = C_BasePlayer::get_local_player();
        if (local && g_EngineClient->IsInGame())
        {
            static QAngle vecAngles;
            g_EngineClient->GetViewAngles(vecAngles);
            if (!local->m_bIsScoped())
                setup->fov = g_Options.fov_cs_debug.get_int();
        }

        grenade_prediction::instance().View(setup);
        ofunc(_this, setup);
    }    
    //--------------------------------------------------------------------------------
    bool _fastcall do_post_screen_space_effects(void* ecx, void* edx, CViewSetup* pSetup)
    {
        static auto ofunc = client_mode_hook.get_original<do_post_screen_space_effects_t>(44);
        if (g_Options.glow_enabled.get_bool())
        {
            auto local = C_BasePlayer::get_local_player();
            if (local != nullptr)
            {
                for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); ++i)
                {
                    if (g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
                        continue;

                    CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &g_GlowObjManager->m_GlowObjectDefinitions[i];

                    switch (glowEnt->getEnt()->GetClientClass()->m_ClassID)
                    {
                    default:
                        if (g_Options.glow_weapons.get_bool())
                        {
                            if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")))
                                glowEnt->set(1.0f, 1.0f, 0.0f, 0.8f);
                        }
                        break;
                    case 1:
                        if (g_Options.glow_weapons.get_bool())
                            glowEnt->set(1.0f, 1.0f, 0.0f, 0.8f);
                        break;
                    case 35:
                        if (g_Options.glow_players.get_bool())
                        {
                            if (g_Options.glow_enemies_only.get_bool() && glowEnt->getEnt()->m_iTeamNum() == local->m_iTeamNum())
                                break;

                            if (glowEnt->getEnt()->m_iTeamNum() == local->m_iTeamNum())
                                glowEnt->set(0.23921f, 0.55294f, 0.89019f, 0.8f);
                            else if (glowEnt->getEnt()->m_iTeamNum() != local->m_iTeamNum())
                                glowEnt->set(0.89019f, 0.23137f, 0.23137f, 0.8f);
                        }
                        break;
                    case 39:
                        if (g_Options.glow_weapons.get_bool())
                            glowEnt->set(1.0f, 1.0f, 0.0f, 0.8f);
                        break;
                    }
                }
            }
        }
        return ofunc(ecx, pSetup);
    }
    //--------------------------------------------------------------------------------

    auto linegoesthrusmoke = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
    auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
    void __stdcall paint_traverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
    {
        auto local = C_BasePlayer::get_local_player();
        if (local && local->is_alive() && g_Options.esp_poland_no_scope_border.get_bool() && !strcmp("HudZoom", g_VGuiPanel->GetName(panel))) {
            return;
        }
        static auto ofunc = panel_hook.get_original<paint_traverse_t>(41);
        static vgui::VPANEL panelId = 0;
        ofunc(g_VGuiPanel, panel, forceRepaint, allowForce);

        if (!panelId) {
            const auto panelName = g_VGuiPanel->GetName(panel);
            if (!strcmp(panelName, "FocusOverlayPanel")) {
                panelId = panel;
            }
        }
        else if (panelId == panel) {
            visuals::paint_traverse();
            if (g_Options.visual_no_smoke.get_bool()) *(int*)(smokecout) = 0;
            if (!local)
                return;


            grenade_prediction::instance().Paint();
            auto weapon = local->m_hActiveWeapon().Get();
            if (local && local->is_alive())
            {
                if (local->m_bIsScoped() && g_Options.esp_poland_no_scope_border.get_bool() && weapon->is_sniper())
                {
                    int width = 0;
                    int height = 0;
                    g_EngineClient->GetScreenSize(width, height);

                    int centerX = static_cast<int>(width * 0.5f);
                    int centerY = static_cast<int>(height * 0.5f);
                    g_VGuiSurface->DrawSetColor(Color(0, 0, 0, 255));
                    g_VGuiSurface->DrawLine(0, centerY, width, centerY);
                    g_VGuiSurface->DrawLine(centerX, 0, centerX, height);
                }
            }
        }
    }
    //--------------------------------------------------------------------------------

    void __stdcall frame_stage_notify(ClientFrameStage_t stage)
    {
        static auto ofunc = client_hook.get_original<fsn_t>(36);
        Vector AimPunchOld;
        Vector ViewPunchOld;
        Vector* AimPunch = nullptr;
        Vector* ViewPunch = nullptr;
        auto local = C_BasePlayer::get_local_player();
        if (g_EngineClient->IsInGame())
        {
            if (stage == FRAME_RENDER_START)
            {
                if (g_Options.no_visual_recoil.get_bool())
                {
                    if (local && local->is_alive())
                    {
                        AimPunch = (Vector*)((DWORD)local + 0x3018);
                        ViewPunch = (Vector*)((DWORD)local + 0x300C);
                        AimPunchOld = *AimPunch;
                        ViewPunchOld = *ViewPunch;

                        *AimPunch = Vector(0, 0, 0);
                        *ViewPunch = Vector(0, 0, 0);
                    }
                }
            }
        }
        ofunc(g_CHLClient, stage);
        if (AimPunch && ViewPunch && g_Options.no_visual_recoil.get_bool())
        {
            *AimPunch = AimPunchOld;
            *ViewPunch = ViewPunchOld;
        }
        if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
        {
            knife_changer();
        }

    }

    //--------------------------------------------------------------------------------

    void __stdcall draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
    {
       static auto ofunc = modelrender_hook.get_original<dme_t>(21);

       visuals::draw_model_execute(pInfo);

       ofunc(g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
       g_MdlRender->ForcedMaterialOverride(NULL);
    }
    //--------------------------------------------------------------------------------
    void __stdcall play_sound(const char* name)
    {
        static auto ofunc = vgui_surface_hook.get_original<play_sound_t>(82);

        ofunc(g_VGuiSurface, name);

        if (g_EngineClient->IsInGame()) return;

        if (!strcmp(name, "!UI/competitive_accept_beep.wav")) {
            static auto CServerConfirmedReservationCheckCallback__IsReady
                = (void(__cdecl*)(void))utils::pattern_scan(
                    GetModuleHandleW(L"client.dll"),
                    "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"
                );
            
            CServerConfirmedReservationCheckCallback__IsReady();

            FLASHWINFO fi;
            fi.cbSize = sizeof(FLASHWINFO);
            fi.hwnd = input_system::instance().get_window();
            fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
            fi.uCount = 0;
            fi.dwTimeout = 0;
            FlashWindowEx(&fi);
        }
    }



    //--------------------------------------------------------------------------------
    void __stdcall trace_ray(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace)
    {
        static auto ofunc = enginetrace_hook.get_original<trace_ray_t>(5);

        ofunc(g_EngineTrace, ray, fMask, pTraceFilter, pTrace);
    };
    //--------------------------------------------------------------------------------                                                     
    bool __fastcall is_hltv(void* ecx, void* edx)
    {
        uintptr_t pvs_entity;
        __asm
        {
            mov pvs_entity, edi
        }

        static auto ofunc = engine_hook.get_original<is_hltv_t>(93);
        void* returna = _ReturnAddress();
        static auto v1 = (DWORD)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7");
        if (reinterpret_cast<DWORD>(returna) == (v1))
        {
            if (pvs_entity)
            {
                *(int*)(pvs_entity + 0xA24) = -1;
                *(int*)(pvs_entity + 0xA2C) = *(int *)(pvs_entity + 0xA28);
                *(int*)(pvs_entity + 0xA28) = 0;
                return true;
            }
        }
        return ofunc(ecx);
    }
    //--------------------------------------------------------------------------------

}
bool checkteams = false;
#include "hooks.hpp"
#include <intrin.h>

#include "../menu/menu.hpp"
#include "../helpers/input.hpp"
#include "../helpers/utils.hpp"
#include "../features/visuals.hpp"
#include "../helpers/debug_console.h"
#include "../helpers/console.h"

//#include "../minhook/include/MinHook.h"
#pragma intrinsic(_ReturnAddress)

#include <chrono>
#include <thread>

namespace hooks
{
    vfunc_hook hlclient_hook;
    vfunc_hook direct3d_hook;
    vfunc_hook vguipanel_hook;
    vfunc_hook vguisurf_hook;
    vfunc_hook sound_hook;
    vfunc_hook mdlrender_hook;
    vfunc_hook clientmode_hook;
    vfunc_hook sv_cheats;
    vfunc_hook render_view_hook;
    vfunc_hook gameevents_hook;
    vfunc_hook clientstate_hook;
    vfunc_hook entity_renderable_hook;
    vfunc_hook clientstate2_hook;
    setup_bones_t o_setup_bones = nullptr;
    calc_view_t o_calc_view = nullptr;
    temp_entities_t o_temp_entities = nullptr;
    vfunc_hook engine_client_hook;
    vfunc_hook prediction;
    //get_eye_angles_t o_get_eye_angles = nullptr;
    post_data_update_t o_post_data_update = nullptr;

    HWND csgo_window = nullptr;
    WNDPROC original_wnd_proc = nullptr;
    vfunc_hook player_vtable_hook;

    //uintptr_t** test;

    void initialize ( )
    {
        while ( ! ( ( csgo_window = FindWindowA ( xor_str ( "Valve001" ), nullptr ) ) ) )
        {
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for ( 50ms );
        }

        //test = reinterpret_cast<uintptr_t**> ( GetModuleHandle ( L"client_panorama.dll" ) ) + 0x10A8F86C;

        //console::write_line ( ( int )test );

        //test[index::calc_view] = ( uintptr_t* )&get_test;

        debug_console::print ( xor_str ( "initializing" ) );

        hlclient_hook.setup ( interfaces::chl_client, xor_str ( "client_panorama.dll" ) );
        direct3d_hook.setup ( interfaces::d3d_device9, xor_str ( "shaderapidx9.dll" ) );
        vguipanel_hook.setup ( interfaces::vgui_panel, xor_str ( "vgui2.dll" ) );
        vguisurf_hook.setup ( interfaces::vgui_surface, xor_str ( "vguimatsurface.dll" ) );
        sound_hook.setup ( interfaces::engine_sound, xor_str ( "engine.dll" ) );
        mdlrender_hook.setup ( interfaces::mdl_render, xor_str ( "engine.dll" ) );
        clientmode_hook.setup ( interfaces::client_mode, xor_str ( "client_panorama.dll" ) );
        con_var* sv_cheats_con = interfaces::cvar->find_var ( xor_str ( "sv_cheats" ) );
        sv_cheats.setup ( sv_cheats_con );

        con_var* cam_ideallag_con = interfaces::cvar->find_var ( xor_str ( "cam_ideallag" ) );

        if ( cam_ideallag_con )
        {
            cam_ideallag_con->m_fnChangeCallbacks.m_Size = 0;
            cam_ideallag_con->set_value ( 0.f );
        }

        render_view_hook.setup ( interfaces::render_view, xor_str ( "engine.dll" ) );
        gameevents_hook.setup ( interfaces::game_events, xor_str ( "engine.dll" ) );
        clientstate_hook.setup ( interfaces::client_state, xor_str ( "client_panorama.dll" ) );
        engine_client_hook.setup ( interfaces::engine_client, xor_str ( "engine.dll" ) );
        prediction.setup ( interfaces::prediction, xor_str ( "client_panorama.dll" ) );
        //entity_renderable_hook.setup ( interfaces::dwCCSPlayerRenderablevftable, "client_panorama.dll" );

        //direct3d_hook.hook_index ( index::end_scene, hk_end_scene );
        direct3d_hook.hook_index ( index::reset, hk_reset );
        direct3d_hook.hook_index ( index::present, hk_present );

        prediction.hook_index ( index::in_prediction, hk_in_prediction );

        hlclient_hook.hook_index ( index::frame_stage_notify, hk_frame_stage_notify );
        hlclient_hook.hook_index ( index::create_move, hk_create_move_proxy );

        vguipanel_hook.hook_index ( index::paint_traverse, hk_paint_traverse );

        sound_hook.hook_index ( index::emit_sound1, hk_emit_sound1 );

        vguisurf_hook.hook_index ( index::lock_cursor, hk_lock_cursor );

        mdlrender_hook.hook_index ( index::draw_model_execute, hk_draw_model_execute );

        clientmode_hook.hook_index ( index::do_post_screen_space_effects, hk_do_post_screen_effects );
        clientmode_hook.hook_index ( index::override_view, hk_override_view );

        sv_cheats.hook_index ( index::sv_cheats_get_bool, hk_sv_cheats_get_bool );

        //render_view_hook.hook_index ( index::scene_end, hk_scene_end );

        gameevents_hook.hook_index ( index::fire_event, hk_fire_event );

        engine_client_hook.hook_index ( index::is_hltv, hk_is_hltv );

        //entity_renderable_hook.hook_index ( 13, hk_setup_bones );

        //clientstate_hook.hook_index( index::temp_entities_t, hk_temp_entities ); //--> needs to be done while ingame so like in painttraverse or shit xD

        setup_fire_events( );

        auto tmp_addr = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                               xor_str ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) );

        const auto dw_ccs_player_renderablevftable = *reinterpret_cast< DWORD* > ( tmp_addr + 0x4E );

        const auto player_vtable = reinterpret_cast< void* > ( ( tmp_addr + 0x45 ) + 0x2 );

        if ( dw_ccs_player_renderablevftable )
            o_setup_bones = vftable_hook::hook_manual< setup_bones_t > (
                reinterpret_cast< uintptr_t* > ( dw_ccs_player_renderablevftable ), index::setup_bones, reinterpret_cast< setup_bones_t > ( hk_setup_bones )
            );

        if ( player_vtable ) // todo fix this
        {
            player_vtable_hook.setup ( player_vtable );

            player_vtable_hook.hook_index ( index::get_eye_angles, hk_get_eye_angles );
        }

        if ( csgo_window )
            original_wnd_proc = reinterpret_cast< WNDPROC > ( SetWindowLongPtr ( csgo_window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR > ( hk_wnd_proc ) ) );

        debug_console::print ( xor_str ( "initialized" ) );
    }

    //--------------------------------------------------------------------------------
    void shutdown ( )
    {
        hlclient_hook.unhook_all( );
        direct3d_hook.unhook_all( );
        vguipanel_hook.unhook_all( );
        vguisurf_hook.unhook_all( );
        mdlrender_hook.unhook_all( );
        clientmode_hook.unhook_all( );
        auto tmp_addr = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                               xor_str ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) );

        const auto dw_ccs_player_renderablevftable = *reinterpret_cast< DWORD* > ( tmp_addr + 0x4E );

        //const auto player_vtable = *reinterpret_cast< DWORD* > ( ( tmp_addr + 0x45 ) + 0x2 );

        if ( dw_ccs_player_renderablevftable )
            vftable_hook::hook_manual< setup_bones_t > ( reinterpret_cast< uintptr_t* > ( dw_ccs_player_renderablevftable ), index::setup_bones,
                                                         o_setup_bones );

        //if ( player_vtable )
        //    vftable_hook::hook_manual< get_eye_angles_t > ( reinterpret_cast< uintptr_t* > ( player_vtable ), index::get_eye_angles, o_get_eye_angles );
        player_vtable_hook.unhook_all( );

        //#ifdef _DEBUG
        //const auto c_csplayer_vtable = ( g_utils.pattern_scan ( "client_panorama.dll", "E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? C6 87 ? ? ? ? ? C7 87 ? ? ? ? ? ? ? ?" ) + 0x52 ) + 0x2;

        ////E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? C6 87 ? ? ? ? ? C7 87 ? ? ? ? ? ? ? ?  + 35 + 2
        //if ( c_csplayer_vtable )
        //{
        //    console::write_line ( "hooked post_data_uodate" );
        //    vftable_hook::hook_manual<post_data_update_t> (
        //        reinterpret_cast<uintptr_t*> ( c_csplayer_vtable ), 7, o_post_data_update
        //    );
        //}

        //#endif

        SetWindowLongPtr ( csgo_window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR > ( original_wnd_proc ) );
        //vftable_hook::hook_manual<calc_view_t> ( ( uintptr_t* )dwCCSPlayerRenderablevftable, index::calc_view, o_calc_view );
    }

    //int64_t __fastcall hooks::hk_post_data_update ( void* thisptr, unsigned a2, int a3 )
    //{
    //    console::write_line ( "test" );
    //    return o_post_data_update ( thisptr, a2, a3 );
    //}
}

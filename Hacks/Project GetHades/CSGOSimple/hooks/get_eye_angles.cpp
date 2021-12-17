#include "hooks.hpp"
#include "../helpers/globals.h"
#include "../options.h"
#include "../features/features.h"

qangle_t* __fastcall hooks::hk_get_eye_angles ( uintptr_t ecx, uintptr_t edx )
{
    static auto ofunc = player_vtable_hook.get_original<get_eye_angles_t> ( index::get_eye_angles );

    if ( g_unload )
        return &qangle_t ( 0.f, 0.f, 0.f );

    if ( reinterpret_cast< C_BasePlayer* > ( ecx )->ent_index( ) != interfaces::engine_client->get_localplayer( ) )
        return ofunc ( ecx, edx );

    static auto ret_to_thirdperson_pitch = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                  xor_str ( "8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55 ?" ) );
    static auto ret_to_thirdperson_yaw = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "F3 0F 10 55 ? 51 8B 8E ? ? ? ?" ) );

    if ( !g_options.visuals_thirdperson_toggle )
        return ofunc ( ecx, edx );

    if ( uintptr_t ( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t > ( ret_to_thirdperson_pitch ) || uintptr_t ( _ReturnAddress( ) ) == reinterpret_cast
        < uintptr_t > ( ret_to_thirdperson_yaw ) )
    {
        //if ( g_options.ragebot_antiaim_desync_mode == xor_str ( "jitter" ) )
        //    return &qangle_t ( ang.pitch, g_features.anti_aim.m_real_yaw_jitter, 0.f );

        return &g_features.animations.m_current_real_angle;
    }

    return ofunc ( ecx, edx );
}

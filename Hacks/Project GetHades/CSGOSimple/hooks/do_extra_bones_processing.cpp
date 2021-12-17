#include "hooks.hpp"
#include "listener_entity.h"
#include "../helpers/console.h"

int __fastcall hooks::hk_calc_view ( void* ecx, double st0, int Angle, int a3, int a4, int a5, float* a6 )
{
    const auto e = reinterpret_cast< C_BasePlayer* > ( ecx );

    return g_listener_entity.m_track[ e->ent_index( ) ].vmt.get_original< hooks::calc_view_t > ( index::calc_view ) ( ecx, st0, Angle, a3, a4, a5, a6 );
}

void __fastcall hooks::hk_do_extra_bones_processing ( void* ecx, void*, CStudioHdr* hdr, Vector* pos, quaternion_t* q, matrix3x4_t* matrix,
                                                      void* bone_list, void* context )
{
    const auto e = reinterpret_cast< C_BasePlayer* > ( ecx );

    auto state = e->get_base_player_anim_state( );
    const auto val = reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( state ) + 292 );
    const auto backup = *val;
    auto backup_onground = false;

    if ( state )
    {
        backup_onground = state->m_bOnGround;
        state->m_bOnGround = false;

        if ( g_local->vec_velocity( ).length2d( ) < 0.1f )
            *val = 0.f;
    }

    g_listener_entity.m_track[ e->ent_index( ) ].vmt.get_original< hooks::do_extra_bones_processing_t > ( index::do_extra_bones_processing ) (
        ecx, hdr, pos, q, matrix, bone_list, context );

    if ( state )
    {
        *val = backup;
        state->m_bOnGround = backup_onground;
    }
}

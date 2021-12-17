#include "hooks.hpp"
#include "../helpers/console.h"
#include "listener_entity.h"

const auto ptr_setupbones = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                   xor_str ( "8B 40 ? FF D0 84 C0 74 ? F3 0F 10 05 ? ? ? ? EB ?" ) );

const auto ptr_calcview_unknown = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                         xor_str (
                                                             "FF D0 84 C0 75 ? A1 ? ? ? ? B9 ? ? ? ? FF 50 ? A1 ? ? ? ? 51 C7 04 24 ? ? ? ? B9 ? ? ? ? 53 56"
                                                         ) );

bool __fastcall hooks::hk_in_prediction ( void* p )
{
    const auto ofunc = prediction.get_original< in_prediction_t > ( index::in_prediction );

    //console::write_line ( ( int )_ReturnAddress() );

    if ( reinterpret_cast< uintptr_t > ( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t > ( ptr_setupbones ) + 5 )
        return false; // we don't want to predict in setupbones

    //if ( reinterpret_cast<uintptr_t> ( _ReturnAddress() ) == reinterpret_cast<uintptr_t> ( ptr_calcview_unknown ) )
    //{
    //    console::write_line ( "calcview" );
    //    return false;
    //}

    return ofunc ( p );
}

void __fastcall hooks::hk_setup_velocity ( )
{
}

Vector __fastcall hooks::hk_get_abs_origin ( void* ecx )
{
    const auto e = reinterpret_cast< C_BasePlayer* > ( ecx );

    if ( !e )
        g_listener_entity.m_track[ e->ent_index( ) ].vmt.get_original< get_abs_origin_t > ( index::get_abs_origin ) ( ecx );

    return e->vec_origin( );
}

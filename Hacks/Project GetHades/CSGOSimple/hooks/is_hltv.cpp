#include "hooks.hpp"
#include "../helpers/console.h"

auto ptr_accumulate_layers = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "84 C0 75 0D F6 87" ) );
auto setupvelocity_call = reinterpret_cast< void* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                             xor_str ( "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) ) );

bool __fastcall hooks::hk_is_hltv ( )
{
    const auto org_f = engine_client_hook.get_original< is_hltv_t > ( index::is_hltv );

    if ( reinterpret_cast< uintptr_t > ( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t > ( ptr_accumulate_layers ) )
        return true;

    if ( reinterpret_cast< uintptr_t > ( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t > ( setupvelocity_call ) )
        return true;

    return org_f( );
}

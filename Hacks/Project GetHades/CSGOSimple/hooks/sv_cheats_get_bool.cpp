#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

namespace hooks
{
    auto dw_cam_think = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "85 C0 75 30 38 86" ) );
    typedef bool ( __thiscall* svc_get_bool_t ) ( PVOID );

    bool __fastcall hk_sv_cheats_get_bool ( const PVOID con_var, void* edx )
    {
        static auto ofunc = sv_cheats.get_original< svc_get_bool_t > ( 13 );

        if ( !ofunc )
            return false;

        if ( g_unload )
            return ofunc ( con_var );

        if ( reinterpret_cast< DWORD > ( _ReturnAddress( ) ) == reinterpret_cast< DWORD > ( dw_cam_think ) )
            return true;

        return ofunc ( con_var );
    }
}

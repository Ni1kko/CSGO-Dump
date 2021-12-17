#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

void __fastcall hooks::hk_scene_end ( void* pEcx, void* pEdx )
{
    static auto ofunc = render_view_hook.get_original< scene_end > ( index::scene_end );

    if ( g_unload )
        return;

    ofunc ( pEcx, pEdx );
}

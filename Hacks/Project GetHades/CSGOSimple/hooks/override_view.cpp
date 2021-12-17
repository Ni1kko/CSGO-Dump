#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"
#include "../options.h"
#include "../helpers/console.h"
#include "../features/features.h"

void __stdcall hooks::hk_override_view ( CViewSetup* vs_view )
{
    static auto ofunc = clientmode_hook.get_original< override_view > ( index::override_view );

    if ( g_unload )
        return ofunc ( interfaces::client_mode, vs_view );;

    if ( interfaces::engine_client->is_in_game( ) && vs_view && g_local )
    {
        c_visuals::third_person( );

        if ( g_options.visuals_remove_scope_zoom && g_options.visuals_fov == vs_view->fov )
            vs_view->fov += 1.f;
        else if ( ! ( !g_options.visuals_remove_scope_zoom && g_local->is_scoped( ) ) )
            vs_view->fov = static_cast< float > ( g_options.visuals_fov );
    }

    ofunc ( interfaces::client_mode, vs_view );
}

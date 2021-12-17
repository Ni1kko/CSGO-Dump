#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

int __stdcall hooks::hk_do_post_screen_effects ( int a1 )
{
    auto ofunc = clientmode_hook.get_original< do_post_screen_effects > ( index::do_post_screen_space_effects );

    if ( g_unload )
        return ofunc ( interfaces::client_mode, a1 );

    //if (g_LocalPlayer && g_Options.glow_enabled)
    //	Glow::Get().Run();

    return ofunc ( interfaces::client_mode, a1 );
}

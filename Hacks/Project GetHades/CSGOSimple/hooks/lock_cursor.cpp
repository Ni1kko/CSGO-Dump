#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

#include "../menu/menu.hpp"

void __stdcall hooks::hk_lock_cursor ( )
{
    static auto ofunc = vguisurf_hook.get_original< lock_cursor_t > ( index::lock_cursor );

    if ( g_unload )
        return ofunc ( interfaces::vgui_surface );;

    if ( menu.is_visible( ) )
    {
        interfaces::vgui_surface->unlock_cursor( );
        interfaces::input_system->reset_input_state( );
        return;
    }

    ofunc ( interfaces::vgui_surface );
}

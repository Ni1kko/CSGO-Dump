#include "hooks.hpp"
#include "../menu/menu.hpp"
#include "../menu/hades_menu.h"
#include "../helpers/input.hpp"
#include "../features/features.h"

LRESULT __stdcall hooks::hk_wnd_proc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( !interfaces::engine_client )
        return CallWindowProcA ( original_wnd_proc, hWnd, uMsg, wParam, lParam );

    g_inputsys.process_message ( uMsg, wParam, lParam );

    static bool was_last_open = false;

    if ( menu.is_visible( ) && interfaces::engine_client->is_active_app( ) && was_last_open /* professional fix to be able to tab in again */ )
    {
        hades_menu.on_wnd_proc ( hWnd, uMsg, wParam, lParam );
        g_cursor.run_think ( uMsg, lParam );
        return true;
    }
    else
        g_features.keybind_system.on_wnd_proc ( hWnd, uMsg, wParam, lParam );

    was_last_open = menu.is_visible( );

    // Call original wndproc to make game use input again
    return CallWindowProcA ( original_wnd_proc, hWnd, uMsg, wParam, lParam );
}

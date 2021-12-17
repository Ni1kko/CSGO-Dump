#include "keybind_system.h"
#include "../options.h"

#include "../helpers/console.h"

void c_keybind_system::on_wnd_proc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( !interfaces::engine_client->is_in_game( ) )
        return;

    switch ( uMsg )
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        {
            handle_keypress ( wParam );
        }
        break;

    case WM_XBUTTONDOWN:
        handle_keypress ( HIWORD ( wParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
    }
}

void c_keybind_system::handle_keypress ( int key )
{
    // thirdperson toggle
    if ( key == g_options.visuals_thirdperson_key )
        g_options.visuals_thirdperson_toggle = !g_options.visuals_thirdperson_toggle;

    if ( key == g_options.ragebot_antiaim_manual_left_key )
    {
        if ( g_options.ragebot_manualaa_mode == 1 )
            g_options.ragebot_manualaa_mode = 0;
        else
            g_options.ragebot_manualaa_mode = 1;
    } //g_options.ragebot_manualaa_mode

    if ( key == g_options.ragebot_antiaim_manual_right_key )
    {
        if ( g_options.ragebot_manualaa_mode == 2 )
            g_options.ragebot_manualaa_mode = 0;
        else
            g_options.ragebot_manualaa_mode = 2;
    }

    if ( key == g_options.ragebot_antiaim_manual_back_key )
    {
        if ( g_options.ragebot_manualaa_mode == 3 )
            g_options.ragebot_manualaa_mode = 0;
        else
            g_options.ragebot_manualaa_mode = 3;
    }
}

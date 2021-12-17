#include "input.hpp"
#include "../valve_sdk/sdk.hpp"

c_input_sys g_inputsys;

c_input_sys::c_input_sys ( )
    : m_hTargetWindow ( nullptr ), m_ulOldWndProc ( 0 )
{
}

c_input_sys::~c_input_sys ( )
{
    if ( m_ulOldWndProc )
        SetWindowLongPtr ( m_hTargetWindow, GWLP_WNDPROC, m_ulOldWndProc );

    m_ulOldWndProc = 0;
}

bool c_input_sys::process_message ( const UINT u_msg, const WPARAM w_param, const LPARAM l_param )
{
    switch ( u_msg )
    {
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONDBLCLK:
    case WM_XBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    case WM_XBUTTONUP:
        return process_mouse_message ( u_msg, w_param );

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        return process_keybd_message ( u_msg, w_param );

    default:
        return false;
    }
}

bool c_input_sys::process_mouse_message ( const UINT u_msg, const WPARAM w_param )
{
    auto key = VK_LBUTTON;
    auto state = KeyState::none;

    switch ( u_msg )
    {
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        state = u_msg == WM_MBUTTONUP ? KeyState::up : KeyState::down;
        key = VK_MBUTTON;
        break;

    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        state = u_msg == WM_RBUTTONUP ? KeyState::up : KeyState::down;
        key = VK_RBUTTON;
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        state = u_msg == WM_LBUTTONUP ? KeyState::up : KeyState::down;
        key = VK_LBUTTON;
        break;

    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        state = u_msg == WM_XBUTTONUP ? KeyState::up : KeyState::down;
        key = ( HIWORD ( w_param ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
        break;

    default:
        return false;
    }

    if ( state == KeyState::up && m_iKeyMap[ key ] == KeyState::down )
        m_iKeyMap[ key ] = KeyState::pressed;
    else
        m_iKeyMap[ key ] = state;

    return true;
}

bool c_input_sys::process_keybd_message ( const UINT u_msg, const WPARAM w_param )
{
    const auto key = w_param;
    auto state = KeyState::none;

    switch ( u_msg )
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        state = KeyState::down;
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        state = KeyState::up;
        break;

    default:
        return false;
    }

    if ( state == KeyState::up && m_iKeyMap[ int ( key ) ] == KeyState::down )
    {
        m_iKeyMap[ int ( key ) ] = KeyState::pressed;

        auto& hotkey_callback = m_Hotkeys[ key ];

        if ( hotkey_callback )
            hotkey_callback( );
    }
    else
        m_iKeyMap[ int ( key ) ] = state;

    return true;
}

KeyState c_input_sys::get_key_state ( std::uint32_t vk )
{
    return m_iKeyMap[ vk ];
}

bool c_input_sys::is_key_down ( std::uint32_t vk )
{
    return m_iKeyMap[ vk ] == KeyState::down;
}

bool c_input_sys::was_key_pressed ( std::uint32_t vk )
{
    if ( m_iKeyMap[ vk ] == KeyState::pressed )
    {
        m_iKeyMap[ vk ] = KeyState::up;
        return true;
    }

    return false;
}

void c_input_sys::register_hotkey ( const std::uint32_t vk, std::function< void  ( void ) > f )
{
    m_Hotkeys[ vk ] = f;
}

void c_input_sys::remove_hotkey ( const std::uint32_t vk )
{
    m_Hotkeys[ vk ] = nullptr;
}

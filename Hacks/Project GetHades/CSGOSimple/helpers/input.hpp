#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <d3d9.h>

enum class KeyState
{
    none = 1,
    down,
    up,
    pressed /*Down and then up*/
};

DEFINE_ENUM_FLAG_OPERATORS ( KeyState );

class c_input_sys
{
public:
    c_input_sys ( );
    ~c_input_sys ( );

    HWND get_main_window ( ) const
    {
        return m_hTargetWindow;
    }

    KeyState get_key_state ( uint32_t vk );
    bool is_key_down ( uint32_t vk );
    bool was_key_pressed ( uint32_t vk );

    void register_hotkey ( uint32_t vk, std::function< void  ( void ) > f );
    void remove_hotkey ( uint32_t vk );

    bool process_message ( UINT u_msg, WPARAM w_param, LPARAM l_param );

private:
    bool process_mouse_message ( UINT u_msg, WPARAM w_param );
    bool process_keybd_message ( UINT u_msg, WPARAM w_param );

    HWND m_hTargetWindow;
    LONG_PTR m_ulOldWndProc;
    KeyState m_iKeyMap[256]{ };

    std::function< void  ( void ) > m_Hotkeys[256];
};

extern c_input_sys g_inputsys;

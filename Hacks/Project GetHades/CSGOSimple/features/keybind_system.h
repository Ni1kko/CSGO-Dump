#pragma once
#include "../valve_sdk/csgostructs.hpp"

class c_keybind_system
{
public:
    void on_wnd_proc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
private:
    static void handle_keypress ( int key );
};

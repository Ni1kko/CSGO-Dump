#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

#include "../singleton.hpp"

enum class key_state
{
    None = 1,
    Down,
    Up,
    Pressed /*Down and then up*/
};

enum mouse_button
{
    None,
    LeftButton,
    RightButton,
    CancelButton,
    MiddleButton,
    Mouse4,
    Mouse5,
    ButtonCount
};

DEFINE_ENUM_FLAG_OPERATORS(key_state);
DEFINE_ENUM_FLAG_OPERATORS(mouse_button);
extern bool PressedKeys[256]; // pseudo fix until you fix it you nigger
class input_system : public singleton<input_system>
{
    friend class singleton<input_system>;

    input_system();
    ~input_system();

public:

    void initialize();
    bool process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND get_window() const { return target_window; }

    key_state     mouse_get_state(mouse_button button);
    bool          mouse_is_down(mouse_button button);
    bool          mouse_was_pressed(mouse_button button);
    key_state     keybd_get_state(std::uint32_t vk);
    bool          keybd_is_down(std::uint32_t vk);
    bool          keybd_was_pressed(std::uint32_t vk);

    void register_hotkey(std::uint32_t vk, std::function<void(void)> f);
    void unregister_hotkey(std::uint32_t vk);

private:
    bool process_mouse_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool process_keybd_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

    static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool HandleInputMessage(HWND, WPARAM, LPARAM);
    HWND            target_window;
    WNDPROC         old_wndproc;
    key_state       buttons[int(mouse_button::ButtonCount)];
    key_state       keys[256];

    std::function<void(void)> hotkeys[256];
};
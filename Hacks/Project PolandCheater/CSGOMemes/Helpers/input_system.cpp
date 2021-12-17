#include "input_system.hpp"

#include "../ValveSDK/interfaces.hpp"
#include "../imgui/imgui.h"
#include "../imgui/directx9/imgui_impl_dx9.h"
#include "../Menu.hpp"
bool PressedKeys[256] = {};
LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    switch(msg) {
        case WM_LBUTTONDOWN:
            io.MouseDown[0] = true;
            return true;
        case WM_LBUTTONUP:
            io.MouseDown[0] = false;
            return true;
        case WM_RBUTTONDOWN:
            io.MouseDown[1] = true;
            return true;
        case WM_RBUTTONUP:
            io.MouseDown[1] = false;
            return true;
        case WM_MBUTTONDOWN:
            io.MouseDown[2] = true;
            return true;
        case WM_MBUTTONUP:
            io.MouseDown[2] = false;
            return true;
        case WM_XBUTTONDOWN:
            if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
                io.MouseDown[3] = true;
            else if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
                io.MouseDown[4] = true;
            return true;
        case WM_XBUTTONUP:
            if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
                io.MouseDown[3] = false;
            else if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
                io.MouseDown[4] = false;
            return true;
        case WM_MOUSEWHEEL:
            io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
            return true;
        case WM_MOUSEMOVE:
            io.MousePos.x = (signed short)(lParam);
            io.MousePos.y = (signed short)(lParam >> 16);
            return true;
        case WM_KEYDOWN:
            if (wParam < 256)
                io.KeysDown[wParam] = 1;

            return true;
        case WM_KEYUP:
            if (wParam < 256)
                io.KeysDown[wParam] = 0;
            return true;
        case WM_CHAR:
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if(wParam > 0 && wParam < 0x10000)
                io.AddInputCharacter((unsigned short)wParam);
            return true;
    }
    return 0;
}

input_system::input_system()
    : old_wndproc(nullptr)
{
}

input_system::~input_system()
{
    if(old_wndproc)
        SetWindowLongPtr(target_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(old_wndproc));
    old_wndproc = nullptr;
}

void input_system::initialize()
{
    D3DDEVICE_CREATION_PARAMETERS params;

    if(FAILED(g_D3DDevice9->GetCreationParameters(&params)))
        throw std::runtime_error("[input_system] GetCreationParameters failed.");

    target_window = params.hFocusWindow;
    old_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongW(target_window, GWLP_WNDPROC, reinterpret_cast<LONG>(WndProc)));
    if(old_wndproc == nullptr)
        throw std::runtime_error("[input_system] SetWindowLong failed.");
}

LRESULT __stdcall input_system::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    instance().process_message(msg, wParam, lParam);

    // Menu toggle
    if(instance().keybd_was_pressed(VK_INSERT)) {
        menu::instance().toggle();
    }

    switch (msg) { // gay ass key system to legitbot, atleast works :^)
    case WM_LBUTTONDOWN:
        PressedKeys[VK_LBUTTON] = true;
        break;
    case WM_LBUTTONUP:
        PressedKeys[VK_LBUTTON] = false;
        break;
    case WM_RBUTTONDOWN:
        PressedKeys[VK_RBUTTON] = true;
        break;
    case WM_RBUTTONUP:
        PressedKeys[VK_RBUTTON] = false;
        break;
    case WM_MBUTTONDOWN:
        PressedKeys[VK_MBUTTON] = true;
        break;
    case WM_MBUTTONUP:
        PressedKeys[VK_MBUTTON] = false;
        break;
    case WM_XBUTTONDOWN:
    {
        UINT button = GET_XBUTTON_WPARAM(wParam);
        if (button == XBUTTON1)
        {
            PressedKeys[VK_XBUTTON1] = true;
        }
        else if (button == XBUTTON2)
        {
            PressedKeys[VK_XBUTTON2] = true;
        }
        break;
    }
    case WM_XBUTTONUP:
    {
        UINT button = GET_XBUTTON_WPARAM(wParam);
        if (button == XBUTTON1)
        {
            PressedKeys[VK_XBUTTON1] = false;
        }
        else if (button == XBUTTON2)
        {
            PressedKeys[VK_XBUTTON2] = false;
        }
        break;
    }
    case WM_KEYDOWN:
        PressedKeys[wParam] = true;
        break;
    case WM_KEYUP:
        PressedKeys[wParam] = false;
        break;
    default: break;
    }

    //Processes the user input using ImGui
    if(menu::instance().visible() && ImGui_ImplDX9_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProcW(instance().old_wndproc, hWnd, msg, wParam, lParam);
}

bool input_system::process_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
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
            return process_mouse_message(uMsg, wParam, lParam);
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            return process_keybd_message(uMsg, wParam, lParam);
        default:
            return false;
    }
}

bool input_system::process_mouse_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto button = mouse_button::None;
    auto state = key_state::None;
    switch(uMsg) {
        case WM_MBUTTONDBLCLK:
            state = key_state::Pressed;
            button = mouse_button::MiddleButton;
            break;
        case WM_RBUTTONDBLCLK:
            state = key_state::Pressed;
            button = mouse_button::RightButton;
            break;
        case WM_LBUTTONDBLCLK:
            state = key_state::Pressed;
            button = mouse_button::LeftButton;
            break;
        case WM_XBUTTONDBLCLK:
            state = key_state::Pressed;
            button = (HIWORD(wParam) == XBUTTON1 ? mouse_button::Mouse4 : mouse_button::Mouse5);
            break;
        case WM_MBUTTONDOWN:
            state = key_state::Down;
            button = mouse_button::MiddleButton;
            break;
        case WM_RBUTTONDOWN:
            state = key_state::Down;
            button = mouse_button::RightButton;
            break;
        case WM_LBUTTONDOWN:
            state = key_state::Down;
            button = mouse_button::LeftButton;
            break;
        case WM_XBUTTONDOWN:
            state = key_state::Down;
            button = (HIWORD(wParam) == XBUTTON1 ? mouse_button::Mouse4 : mouse_button::Mouse5);
            break;
        case WM_MBUTTONUP:
            state = key_state::Up;
            button = mouse_button::MiddleButton;
            break;
        case WM_RBUTTONUP:
            state = key_state::Up;
            button = mouse_button::RightButton;
            break;
        case WM_LBUTTONUP:
            state = key_state::Up;
            button = mouse_button::LeftButton;
            break;
        case WM_XBUTTONUP:
            state = key_state::Up;
            button = (HIWORD(wParam) == XBUTTON1 ? mouse_button::Mouse4 : mouse_button::Mouse5);
            break;
        default:
            return false;
    }

    if(state == key_state::Up && buttons[int(button)] == key_state::Down)
        buttons[int(button)] = key_state::Pressed;
    else
        buttons[int(button)] = state;
    return true;
}

bool input_system::process_keybd_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto key = wParam;
    auto state = key_state::None;

    switch(uMsg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            state = key_state::Down;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            state = key_state::Up;
            break;
        default:
            return false;
    }

    if(state == key_state::Up && keys[int(key)] == key_state::Down) {
        keys[int(key)] = key_state::Pressed;
    } else {
        keys[int(key)] = state;
    }

    auto& hotkey_callback = hotkeys[key];
    if(hotkey_callback) {
        hotkey_callback();
    }

    return true;
}

key_state input_system::mouse_get_state(mouse_button button)
{
    return buttons[int(button)];
}
bool input_system::mouse_is_down(mouse_button button)
{
    return buttons[int(button)] == key_state::Down;
}
bool input_system::mouse_was_pressed(mouse_button button)
{
    if(buttons[int(button)] == key_state::Pressed) {
        buttons[int(button)] = key_state::Up;
        return true;
    }
    return false;
}
key_state input_system::keybd_get_state(std::uint32_t vk)
{
    return keys[vk];
}
bool input_system::keybd_is_down(std::uint32_t vk)
{
    return keys[vk] == key_state::Down;
}
bool input_system::keybd_was_pressed(std::uint32_t vk)
{
    if(keys[vk] == key_state::Pressed) {
        keys[vk] = key_state::Up;
        return true;
    }
    return false;
}

void input_system::register_hotkey(std::uint32_t vk, std::function<void(void)> f)
{
    hotkeys[vk] = f;
}
void input_system::unregister_hotkey(std::uint32_t vk)
{
    hotkeys[vk] = nullptr;
}
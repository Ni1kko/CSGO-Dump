#pragma once

#include "imgui/imgui.h"

struct IDirect3DDevice9;

class Menu
{
public:
    void Initialize();
    void Shutdown();

    void OnDeviceLost();
    void OnDeviceReset();

    void Render();

    void Toggle();

    bool IsVisible() const { return _visible; }

private:
    void CreateStyle();

    ImGuiStyle        _style;
    bool              _visible;
};

inline Menu* g_Menu;
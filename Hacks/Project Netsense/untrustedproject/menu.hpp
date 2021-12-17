#pragma once

#include <string>
#include "singleton.hpp"
#include "imgui/imgui.h"
#include "addons/animations/animations.hpp"
#include "addons/notifications/notifications.hpp"
#include "addons/ui_addons/ui_addons.hpp"
#include "valve_sdk/sdk.hpp"

//struct IDirect3DDevice9;
//IDirect3DTexture9* tImage = nullptr;
class Menu
    : public Singleton<Menu>
{
public:
    bool menu_opened;
    bool AimbotFirstTimeRender = true;
    bool TriggerFirstTimeRender = true;
    bool SkinsFirstTimeRender = true;
public:
    void Initialize();
    void Shutdown();
    void CreateStyle();
    void OnDeviceLost();
    void OnDeviceReset();

    void Render();

    void Toggle();

    bool IsVisible() const { return _visible; }

private:
    

    ImGuiStyle        _style;
    bool              _visible;
};
//extern Menu menu;
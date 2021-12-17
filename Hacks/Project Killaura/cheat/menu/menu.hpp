#pragma once

#include <string>
#include "../other/singleton.hpp"
#include "../../imgui/imgui.h"
#include "../Misc/misc.hpp"

struct IDirect3DDevice9;

extern ImFont* MainCaps;
extern ImFont* Main;
extern ImFont* FMenu;

class Menu : public Singleton<Menu> {
public:
	void Initialize();
	void Shutdown();

	void OnDeviceLost();
	void OnDeviceReset();

	void DrawRadar();
	void Render();

	void Toggle();

	bool IsVisible() const { return _opened; }
private:
	void CreateStyle();

	ImGuiStyle	Style;
	ImGuiStyle        _style;
	bool              _opened;
};
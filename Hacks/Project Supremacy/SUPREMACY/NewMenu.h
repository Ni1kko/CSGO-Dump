#pragma once

#include <string>
#include "includes.hpp"
//#include "depositories\imgui\imgui.h"
struct IDirect3DDevice9;

class NewMenu
	: public singleton<NewMenu>
{
public:
	void Initialize();
	void Shutdown();

	void OnDeviceLost();
	void OnDeviceReset();

	void Render();

	void Show();
	void Hide();
	void Toggle();

	bool IsVisible() const { return _visible; }

private:
	void CreateStyle();

	//ImGuiStyle        _style;
	bool              _visible;
	bool			cl_mouseenable;
};

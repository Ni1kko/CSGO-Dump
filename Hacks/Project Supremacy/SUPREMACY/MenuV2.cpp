//#include "depositories\imgui\imgui.h"
#include <cctype>
#include <shlobj.h>
#include <shlwapi.h>
#include <algorithm>
#define NOMINMAX
#include <Windows.h>
#include <chrono>
//#include "depositories\imgui\imgui_internal.h"
//#include "depositories\imgui\dx9\imgui_impl_dx9.h"
#include "NewMenu.h"
#include "menu/cmenu.hpp"

bool selected = false;
bool selected1 = false;
bool selected2 = false;
bool selected3 = false;
bool selected4 = false;

bool selected11 = false;
bool selected12 = false;
bool selected13 = false;
bool selected14 = false;
bool selected15 = false;

bool selected21 = false;
bool selected22 = false;
bool selected23 = false;
bool selected24 = false;

// =========================================================
//
// These are the tabs on the sidebar
//
// =========================================================
static char* sidebar_tabs[] = {
	"Aimbot",
	"Visuals",
	"Skins",
	"Misc",
	"Config",
	"Style"
};

static ConVar* cl_mouseenable = nullptr;

constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		//auto clr = ImVec4{
		//	v->r() / 255.0f,
		//	v->g() / 255.0f,
		//	v->b() / 255.0f,
		//	v->a() / 255.0f
		//};

		//if (ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
			//v->SetColor(clr.x, clr.y, clr.z, clr.w);
		//	return true;
		//}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}

int get_fps()
{
	using namespace std::chrono;
	static int count = 0;
	static auto last = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	static int fps = 0;

	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000) {
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}

//ImVec2 get_sidebar_size()
//{
	//constexpr float padding = 10.0f;
	//constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
	//constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

	//return ImVec2{ size_w, ImMax(325.0f, 380.0f) };
//}

void NewMenu::Render()
{
}

void NewMenu::Shutdown()
{
	//ImGui_ImplDX9_Shutdown();
	//cl_mouseenable->SetValue(true);
}

void NewMenu::OnDeviceLost()
{
	//ImGui_ImplDX9_InvalidateDeviceObjects();
}

void NewMenu::OnDeviceReset()
{
	//ImGui_ImplDX9_CreateDeviceObjects();
}
void NewMenu::Show()
{
	_visible = true;
	//cl_mouseenable->SetValue(false);
}

void NewMenu::Hide()
{
	_visible = false;
	//cl_mouseenable->SetValue(true);
}

void NewMenu::Toggle()
{
	_visible = !_visible;
	//cl_mouseenable->SetValue(!_visible);
}

void NewMenu::CreateStyle()
{
}
#pragma once
#include "../keybinds/keybinds.hpp"
#include "../../imgui/imgui.h"
#include "../../menu.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../imgui/imgui_internal.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <vector>

class Color;
namespace ImGui {
	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg = ImVec2(0, 0));
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
	bool BeginGroupBox(const char* name, const ImVec2& size_arg = ImVec2(0, 0));
	void EndGroupBox();
	bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0, 0));
	bool ListBox(const char* label, int* current_item, std::string items[], int items_count, int height_items);
	bool ListBox(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items);
	bool Combo(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items);
	bool SButtonEx(const char* label, bool inuse, const ImVec2& size_arg, ImGuiButtonFlags flags);
	bool SButton(const char* label, bool inuse, const ImVec2& size_arg);
	bool SubButtonEx(const char* label, bool inuse, const ImVec2& size_arg, ImGuiButtonFlags flags);
	bool SubButton(const char* label, bool inuse, const ImVec2& size_arg);
	bool ColorEdit(const char* label, Color& v);
	bool Keybind(const char* str_id, CKeyBind* kbind);
	//bool Keybind2(const char* str_id, CKeyBind* kbind);
	bool ButtonExT(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, int page, int in, bool border, ImColor clr);
	bool ButtonT(const char* label, const ImVec2& size_arg, int page, int in, ImColor clr, bool border);
	float CalcMaxPopupHeightFromItemCount(int items_count);
}
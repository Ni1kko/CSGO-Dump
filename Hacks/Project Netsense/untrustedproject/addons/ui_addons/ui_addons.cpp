#include "ui_addons.hpp"
#include <deque>
#include <algorithm>
#include <vector>
#include "../animations/animations.hpp"
#include "../../valve_sdk/misc/Color.hpp"
#include "../clip/clip.h"


bool ImGui::ToggleButton(const char* label, bool* v, const ImVec2& size_arg) {

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	int flags = 0;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	//if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = ImGui::GetColorU32((hovered && held || *v) ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
	ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	if (pressed)
		*v = !*v;

	return pressed;
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static auto vector_getter = [](void* vec, int idx, const char** out_text) {
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

bool ImGui::Combo(const char* label, int* currIndex, std::vector<std::string>& values) {
	if (values.empty()) { return false; }
	return ImGui::Combo(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size());
}

bool ImGui::BeginGroupBox(const char* name, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

	window->DC.CursorPos.y += GImGui->FontSize / 2;
	const ImVec2 content_avail = ImGui::GetContentRegionAvail();
	ImVec2 size = ImFloor(size_arg);
	if (size.x <= 0.0f) {
		size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x); // Arbitrary minimum zero-ish child size of 4.0f (0.0f causing too much issues)
	}
	if (size.y <= 0.0f) {
		size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
	}

	ImGui::SetNextWindowSize(size);
	bool ret;
	ImGui::Begin(name, &ret, flags);
	//bool ret = ImGui::Begin(name, NULL, size, -1.0f, flags);

	window = ImGui::GetCurrentWindow();

	auto padding = ImGui::GetStyle().WindowPadding;

	auto text_size = ImGui::CalcTextSize(name, NULL, true);

	if (text_size.x > 1.0f) {
		window->DrawList->PushClipRectFullScreen();
		//window->DrawList->AddRectFilled(window->DC.CursorPos - ImVec2{ 4, 0 }, window->DC.CursorPos + (text_size + ImVec2{ 4, 0 }), GetColorU32(ImGuiCol_ChildWindowBg));
		//RenderTextClipped(pos, pos + text_size, name, NULL, NULL, GetColorU32(ImGuiCol_Text));
		window->DrawList->PopClipRect();
	}
	//if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
	//	ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

	return ret;
}

void ImGui::EndGroupBox()
{
	ImGui::EndChild();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DC.CursorPosPrevLine.y -= GImGui->FontSize / 2;
}

/*static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}*/

const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};

static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

bool ImGui::Hotkey(const char* label, int* k, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id, false);
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			*k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = *k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (IsKeyPressedMap2(ImGuiKey_Escape)) {
			*k = 0;
			ImGui::ClearActiveID();
		}
		else {
			*k = key;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = "None";

	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	if (*k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, KeyNames[*k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, "<Press a key>");
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}




bool ImGui::ListBox(const char* label, int* current_item, std::string items[], int items_count, int height_items) {
	char** tmp;
	tmp = new char* [items_count];//(char**)malloc(sizeof(char*) * items_count);
	for (int i = 0; i < items_count; i++) {
		//tmp[i] = new char[items[i].size()];//(char*)malloc(sizeof(char*));
		tmp[i] = const_cast<char*>(items[i].c_str());
	}

	const bool value_changed = ImGui::ListBox(label, current_item, Items_ArrayGetter, static_cast<void*>(tmp), items_count, height_items);
	return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items)
{
	return ImGui::ListBox(label, current_item, [](void* data, int idx, const char** out_text)
		{
			*out_text = (*reinterpret_cast<std::function<const char* (int)>*>(data))(idx);
			return true;
		}, &lambda, items_count, height_in_items);
}

bool ImGui::Combo(const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items)
{
	return ImGui::Combo(label, current_item, [](void* data, int idx, const char** out_text)
		{
			*out_text = (*reinterpret_cast<std::function<const char* (int)>*>(data))(idx);
			return true;
		}, &lambda, items_count, height_in_items);
}
//#include "../../config.hpp"
#include "../../features/features.hpp"
bool ImGui::SButtonEx(const char* label, bool inuse, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	if (inuse) {
		// bg
		RenderFrame(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), GetColorU32(ImGuiCol_FrameBgHovered,0.0f));
	}

	// lines
	//auto col = GET_VALUE("accent_color", color);
	//if (c_animations::get().get_alpha() < col.a)
		//col.a = c_animations::get().get_alpha();

	//RenderFrame(ImVec2(bb.Min.x, bb.Min.y + 1), ImVec2(bb.Max.x, bb.Min.y), col);
	//RenderFrame(ImVec2(bb.Min.x, bb.Max.y - 1), ImVec2(bb.Max.x, bb.Max.y), col);
	/*if (pressed)
	{
		*v = !(*v);
		MarkItemEdited(id);
	}*/

	/*if (label_size.x > 0.0f)

	{*/


	int menur, menug, menub, menua;
	menur = g_Misc->options.cMenuCol[0] * 255;
	menug = g_Misc->options.cMenuCol[1] * 255;
	menub = g_Misc->options.cMenuCol[2] * 255;
	menua = g_Misc->options.cMenuCol[3] * 255;
	PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.8));
		if (inuse)
		{
			window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 2.f), bb.Max, ImColor(menur, menug, menub, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertFloat4ToU32(ImColor(menur, menug, menub, 255)));
			RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
			RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
			ImGui::PopStyleColor();

		}
	ImGui::PopStyleVar();

	//}
	
	//RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	return pressed;
}

bool ImGui::SButton(const char* label, bool inuse, const ImVec2& size_arg) {
	return SButtonEx(label, inuse, size_arg, 0);
}



/*bool ImGui::SubButtonEx(const char* label, bool inuse, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	if (inuse) {
		// bg
		RenderFrame(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), GetColorU32(ImGuiCol_FrameBgHovered, 0.0f));
	}

	

	int menur, menug, menub, menua;
	menur = g_Misc->options.cMenuCol[0] * 255;
	menug = g_Misc->options.cMenuCol[1] * 255;
	menub = g_Misc->options.cMenuCol[2] * 255;
	menua = g_Misc->options.cMenuCol[3] * 255;
	PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.8));
	if (inuse)
	{
		window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 2.f), bb.Max, ImColor(menur, menug, menub, 255));
		ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertFloat4ToU32(ImColor(menur, menug, menub, 255)));
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}
	else
	{
		window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 2.f), bb.Max, ImColor(0.59f, 0.59f, 0.59f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();

	}
	ImGui::PopStyleVar();

	//}

	//RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	return pressed;
}*/

bool ImGui::SubButtonEx(const char* label, const bool active, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		MarkItemEdited(id);



	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);

	PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.8));

	window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 2.f), bb.Max, ImColor(0.59f, 0.59f, 0.59f, 1.f));
	PushStyleColor(ImGuiCol_Text, ImVec4(0.59f, 0.59f, 0.59f, 1.f));
	RenderTextClipped(ImVec2(bb.Min.x + 5, bb.Min.y - 6), ImVec2(bb.Max.x + 5, bb.Max.y - 6), label, NULL, &label_size, style.ButtonTextAlign, &bb);
	PopStyleColor();
	if (active)
	{
		window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 2.f), bb.Max, ImColor(0.39f, 0.04f, 0.04f, 1.f));

		PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.15f, 0.15f, 1.f));
		RenderTextClipped(ImVec2(bb.Min.x + 5, bb.Min.y - 6), ImVec2(bb.Max.x + 5, bb.Max.y - 6), label, NULL, &label_size, style.ButtonTextAlign, &bb);
		PopStyleColor();
	}

	ImGui::PopStyleVar();

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	//IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
	return pressed;
}


bool ImGui::SubButton(const char* label, bool inuse, const ImVec2& size_arg) {
	return SubButtonEx(label, inuse, size_arg, 0);
}




bool ImGui::ColorEdit(const char* label, Color& v) {
	ImGui::PushID(label);
	auto clr = ImVec4{
		v.r() / 255.0f,
		v.g() / 255.0f,
		v.b() / 255.0f,
		v.a() / 255.0f
	};

	bool openPopup = ImGui::ColorButton("##btn", clr, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview);
	ImGui::SameLine();
	ImGui::TextUnformatted(label);

	if (openPopup)
		ImGui::OpenPopup("##popup");

	if (ImGui::BeginPopup("##popup")) {
		if (ImGui::ColorPicker4("##picker", &clr.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
			v.SetColor(
				clr.x,
				clr.y,
				clr.z,
				clr.w
			);
		}
		
		
		ImGui::Separator();
		ImGui::BeginColumns("##colsinpopup", 4, ImGuiColumnsFlags_NoBorder);
		ImGui::Text("R: %d", v.r());
		ImGui::NextColumn();
		ImGui::Text("G: %d", v.g());
		ImGui::NextColumn();
		ImGui::Text("B: %d", v.b());
		ImGui::NextColumn();
		ImGui::Text("A: %d", v.a());
		ImGui::EndColumns();
		ImGui::Separator();

		ImGui::Checkbox("Rainbow", &v.rainbow);

		ImGui::Separator();
		
		ImGui::BeginColumns("##colscopypaste", 2, ImGuiColumnsFlags_NoBorder);
		if (ImGui::Button("Copy", ImVec2(-1, 19))) {
			clip::set_text(
				std::to_string(v.r()) + "|" +
				std::to_string(v.g()) + "|" +
				std::to_string(v.b()) + "|" +
				std::to_string(v.a())
			);
		}
		ImGui::NextColumn();
		if (ImGui::Button("Paste", ImVec2(-1, 19))) {
			static auto split = [](std::string str, const char* del) -> std::vector<std::string> {
				char* pTempStr = _strdup(str.c_str());
				char* pWord = strtok(pTempStr, del);
				std::vector<std::string> dest;

				while (pWord != NULL)
				{
					dest.push_back(pWord);
					pWord = strtok(NULL, del);
				}

				free(pTempStr);

				return dest;
			};

			std::string colt = "255|255|255|255";

			if (clip::get_text(colt)) {
				std::vector<std::string> cols = split(colt, "|");
				if (cols.size() == 4) {
					v.SetColor(
						std::stoi(cols.at(0)),
						std::stoi(cols.at(1)),
						std::stoi(cols.at(2)),
						std::stoi(cols.at(3))
					);
				}
			}
		}
		ImGui::EndColumns();

		ImGui::EndPopup();
	}
	ImGui::PopID();

	return false;
}

const char* keys[] = {
	"[-]",
	"[M1]",
	"[M2]",
	"[CN]",
	"[M3]",
	"[M4]",
	"[M5]",
	"[-]",
	"[BAC]",
	"[TAB]",
	"[-]",
	"[-]",
	"[CLR]",
	"[RET]",
	"[-]",
	"[-]",
	"[SHI]",
	"[CTL]",
	"[MEN]",
	"[PAU]",
	"[CAP]",
	"[KAN]",
	"[-]",
	"[JUN]",
	"[FIN]",
	"[KAN]",
	"[-]",
	"[ESC]",
	"[CON]",
	"[NCO]",
	"[ACC]",
	"[MAD]",
	"[SPA]",
	"[PGU]",
	"[PGD]",
	"[END]",
	"[HOM]",
	"[LEF]",
	"[UP]",
	"[RIG]",
	"[DOW]",
	"[SEL]",
	"[PRI]",
	"[EXE]",
	"[PRI]",
	"[INS]",
	"[DEL]",
	"[HEL]",
	"[0]",
	"[1]",
	"[2]",
	"[3]",
	"[4]",
	"[5]",
	"[6]",
	"[7]",
	"[8]",
	"[9]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[A]",
	"[B]",
	"[C]",
	"[D]",
	"[E]",
	"[F]",
	"[G]",
	"[H]",
	"[I]",
	"[J]",
	"[K]",
	"[L]",
	"[M]",
	"[N]",
	"[O]",
	"[P]",
	"[Q]",
	"[R]",
	"[S]",
	"[T]",
	"[U]",
	"[V]",
	"[W]",
	"[X]",
	"[Y]",
	"[Z]",
	"[WIN]",
	"[WIN]",
	"[APP]",
	"[-]",
	"[SLE]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[MUL]",
	"[ADD]",
	"[SEP]",
	"[MIN]",
	"[DEC]",
	"[DIV]",
	"[F1]",
	"[F2]",
	"[F3]",
	"[F4]",
	"[F5]",
	"[F6]",
	"[F7]",
	"[F8]",
	"[F9]",
	"[F10]",
	"[F11]",
	"[F12]",
	"[F13]",
	"[F14]",
	"[F15]",
	"[F16]",
	"[F17]",
	"[F18]",
	"[F19]",
	"[F20]",
	"[F21]",
	"[F22]",
	"[F23]",
	"[F24]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[NUM]",
	"[SCR]",
	"[EQU]",
	"[MAS]",
	"[TOY]",
	"[OYA]",
	"[OYA]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[SHI]",
	"[SHI]",
	"[CTR]",
	"[CTR]",
	"[ALT]",
	"[ALT]"
};

float ImGui::CalcMaxPopupHeightFromItemCount(int items_count) {
	ImGuiContext& g = *GImGui;
	if (items_count <= 0)
		return FLT_MAX;
	return (19 * items_count + items_count % 4) + 5;
}

/*bool ImGui::Keybind2(const char* str_id, CKeyBind* kbind) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	SameLine(window->Size.x - 20);


	ImGuiContext& g = *GImGui;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(str_id);
	ImGuiIO* io = &GetIO();

	ImVec2 label_size = CalcTextSize(keys[kbind->key]);
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + label_size);
	ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(window->Pos.x + window->Size.x - window->DC.CursorPos.x, label_size.y ));
	ItemSize(total_bb, style.FramePadding.y);
	char buf_display[64] = "[-]";

	if (kbind->key != 0 && g.ActiveId != id)
		strcpy_s(buf_display, keys[kbind->key]);
	else if (g.ActiveId == id)
		strcpy_s(buf_display, "[-]");

	total_bb.Min.x -= label_size.x;
	frame_bb.Min.x -= label_size.x;

	if (!ItemAdd(total_bb, id, &frame_bb))
		return false;

	const bool hovered = IsItemHovered();
	const bool edit_requested = hovered && io->MouseClicked[0];
	const bool style_requested = hovered && io->MouseClicked[1];

	if (edit_requested) {
		if (g.ActiveId != id) {
			memset(io->MouseDown, 0, sizeof(io->MouseDown));
			memset(io->KeysDown, 0, sizeof(io->KeysDown));
			kbind->key = 0;
		}

		SetActiveID(id, window);
		FocusWindow(window);
	}
	else if (!hovered && io->MouseClicked[0] && g.ActiveId == id)
		ClearActiveID();

	bool value_changed = false;
	int key = kbind->key;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io->MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
				}
				value_changed = true;
				ClearActiveID();
			}
		}

		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io->KeysDown[i]) {
					key = i;
					value_changed = true;
					ClearActiveID();
				}
			}
		}

		if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
			kbind->key = 0;
			ClearActiveID();
		}
		else
			kbind->key = key;
	}
	else if (kbind->need_modes) {
		bool popup_open = IsPopupOpen(id);

		if (style_requested && !popup_open)
			OpenPopupEx(id);

		if (popup_open) {
			SetNextWindowSize(ImVec2(20, ImGui::CalcMaxPopupHeightFromItemCount(5)));

			char name[16];
			ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.CurrentPopupStack.Size); // Recycle windows based on depth

			// Peak into expected window size so we can position it
			if (ImGuiWindow* popup_window = FindWindowByName(name))
				if (popup_window->WasActive)
				{
					ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
					ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
					ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
					SetNextWindowPos(pos);
				}

			// Horizontally align ourselves with the framed text
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
			bool ret = Begin(name, NULL, window_flags);
			PopStyleVar();

			if (Selectable("Toggle", kbind->mode == KeyBindMode_Toggle))
				kbind->mode = KeyBindMode_Toggle;
			if (Selectable("Hold ON", kbind->mode == KeyBindMode_HoldOn))
				kbind->mode = KeyBindMode_HoldOn;
			if (Selectable("Toggle", kbind->mode == KeyBindMode2_HoldOff))
				kbind->mode = KeyBindMode2_HoldOff;
			if (Selectable("Always", kbind->mode == KeyBindMode_Always))
				kbind->mode = KeyBindMode_Always;

			EndPopup();
		}
	}

	window->DrawList->AddText(frame_bb.Min, g.ActiveId == id ? ImColor(255 / 255.f, 16 / 255.f, 16 / 255.f, g.Style.Alpha) : ImColor(90 / 255.f, 90 / 255.f, 90 / 255.f, g.Style.Alpha), buf_display);
	//PopFont();

	return value_changed;
}*/



bool ImGui::Keybind(const char* str_id, CKeyBind* kbind) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	SameLine(window->Size.x - 20);


	ImGuiContext& g = *GImGui;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(str_id);
	ImGuiIO* io = &GetIO();

	ImVec2 label_size = CalcTextSize(keys[kbind->key]);
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + label_size);
	ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(window->Pos.x + window->Size.x - window->DC.CursorPos.x, label_size.y));
	ItemSize(total_bb, style.FramePadding.y);
	char buf_display[64] = "[-]";

	if (kbind->key != 0 && g.ActiveId != id)
		strcpy_s(buf_display, keys[kbind->key]);
	else if (g.ActiveId == id)
		strcpy_s(buf_display, "[-]");

	total_bb.Min.x -= label_size.x;
	frame_bb.Min.x -= label_size.x;

	if (!ItemAdd(total_bb, id, &frame_bb))
		return false;

	const bool hovered = IsItemHovered();
	const bool edit_requested = hovered && io->MouseClicked[0];
	const bool style_requested = hovered && io->MouseClicked[1];

	if (edit_requested) {
		if (g.ActiveId != id) {
			memset(io->MouseDown, 0, sizeof(io->MouseDown));
			memset(io->KeysDown, 0, sizeof(io->KeysDown));
			kbind->key = 0;
		}

		SetActiveID(id, window);
		FocusWindow(window);
	}
	else if (!hovered && io->MouseClicked[0] && g.ActiveId == id)
		ClearActiveID();

	bool value_changed = false;
	int key = kbind->key;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io->MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
				}
				value_changed = true;
				ClearActiveID();
			}
		}

		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io->KeysDown[i]) {
					key = i;
					value_changed = true;
					ClearActiveID();
				}
			}
		}

		if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
			kbind->key = 0;
			ClearActiveID();
		}
		else
			kbind->key = key;
	}
	else if (kbind->need_modes) {
		bool popup_open = IsPopupOpen(id);

		if (style_requested && !popup_open)
			OpenPopupEx(id);

		if (popup_open) {
			SetNextWindowSize(ImVec2(100, ImGui::CalcMaxPopupHeightFromItemCount(5)));

			char name[16];
			ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.CurrentPopupStack.Size); // Recycle windows based on depth

			// Peak into expected window size so we can position it
			if (ImGuiWindow* popup_window = FindWindowByName(name))
				if (popup_window->WasActive)
				{
					ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
					ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
					ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
					SetNextWindowPos(pos);
				}

			// Horizontally align ourselves with the framed text
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
			bool ret = Begin(name, NULL, window_flags);
			PopStyleVar();

			if (Selectable("Toggle", kbind->mode == KeyBindMode_Toggle))
				kbind->mode = KeyBindMode_Toggle;
			if (Selectable("Hold ON", kbind->mode == KeyBindMode_HoldOn))
				kbind->mode = KeyBindMode_HoldOn;
			if (Selectable("Hold OFF", kbind->mode == KeyBindMode_HoldOff))
				kbind->mode = KeyBindMode_HoldOff;
			if (Selectable("Always", kbind->mode == KeyBindMode_Always))
				kbind->mode = KeyBindMode_Always;

			EndPopup();
		}
	}

	window->DrawList->AddText(frame_bb.Min, g.ActiveId == id ? ImColor(255 / 255.f, 16 / 255.f, 16 / 255.f, g.Style.Alpha) : ImColor(90 / 255.f, 90 / 255.f, 90 / 255.f, g.Style.Alpha), buf_display);
	//PopFont();

	return value_changed;
}




bool ImGui::ButtonExT(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, int page, int in, bool border, ImColor clr) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	if (page == in) {
		PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1));
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, ImVec2(0.5, 0.5), &bb);
		PopStyleColor();

		window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y) - ImVec2(0, 1), bb.Max - ImVec2(0, 1), clr);
	}
	else {
		PushStyleColor(ImGuiCol_Text, ImVec4(.6f, .6f, .6f, 1));
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, ImVec2(0.5, 0.5), &bb);
		PopStyleColor();
	}

	return pressed;
}

bool ImGui::ButtonT(const char* label, const ImVec2& size_arg, int page, int in, ImColor clr, bool border) {
	return ButtonExT(label, size_arg, 0, page, in, border, clr);
}
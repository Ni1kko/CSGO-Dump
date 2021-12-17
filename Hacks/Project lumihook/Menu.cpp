#include "Menu.hpp"

#include "Options.hpp"

#include "Structs.hpp"

#include "features/Miscellaneous.hpp"
#include "features/KitParser.hpp"
#include "features/Skinchanger.hpp"

#include "imgui/imgui_internal.h"

#include <functional>
#include <experimental/filesystem> // hack
#include "features/AntiAim.hpp"
#include "Install.hpp"

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}

	static bool ListBox(const char* label, int* current_item, std::function<const char*(int)> lambda, int items_count, int height_in_items)
	{
		return ImGui::ListBox(label, current_item, [](void* data, int idx, const char** out_text)
		{
			*out_text = (*reinterpret_cast< std::function<const char*(int)>* >(data))(idx);
			return true;
		}, &lambda, items_count, height_in_items);
	}

	bool LabelClick(const char* concatoff, const char* concaton, const char* label, bool* v, const char* unique_id)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		// The concatoff/on thingies were for my weapon config system so if we're going to make that, we still need this aids.
		char Buf[64];
		_snprintf(Buf, 62, "%s%s", ((*v) ? concatoff : concaton), label);

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(unique_id);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
		ItemSize(check_bb, style.FramePadding.y);

		ImRect total_bb = check_bb;
		if (label_size.x > 0)
			SameLine(0, style.ItemInnerSpacing.x);

		const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
		if (label_size.x > 0)
		{
			ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
			total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
		}

		if (!ItemAdd(total_bb, id))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
			*v = !(*v);

		if (label_size.x > 0.0f)
			RenderText(check_bb.GetTL(), Buf);

		return pressed;
	}

	void KeyBindButton(ButtonCode_t* key)
	{
		bool clicked = false;

		std::string text = g_InputSystem->ButtonCodeToString(*key);
		std::string unique_id = std::to_string((int)key);

		if (*key <= BUTTON_CODE_NONE)
			text = "Key not set";

		if (input_shouldListen && input_receivedKeyval == key) {
			clicked = true;
			text = "...";
		}
		text += "]";

		ImGui::SameLine();
		ImGui::LabelClick("[", "[", text.c_str(), &clicked, unique_id.c_str());

		if (clicked)
		{
			input_shouldListen = true;
			input_receivedKeyval = key;
		}

		if (*key == KEY_DELETE)
		{
			*key = BUTTON_CODE_NONE;
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Bind the \"del\" key to remove current bind.");
	}

	ImGuiID Colorpicker_Close = 0;
	__inline void CloseLeftoverPicker() { if (Colorpicker_Close) ImGui::ClosePopup(Colorpicker_Close); }
	void ColorPickerBox(const char* picker_idname, float col_ct[], float col_t[], float col_ct_invis[], float col_t_invis[], bool alpha = true)
	{
		ImGui::SameLine();
		static bool switch_entity_teams = false;
		static bool switch_color_vis = false;
		bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (switch_color_vis ? col_t : col_t_invis) : (switch_color_vis ? col_ct : col_ct_invis), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(36, 0));
		if (open_popup)
		{
			ImGui::OpenPopup(picker_idname);
			Colorpicker_Close = ImGui::GetID(picker_idname);
		}

		if (ImGui::BeginPopup(picker_idname))
		{
			const char* button_name0 = switch_entity_teams ? "Terrorists" : "Counter-Terrorists";
			if (ImGui::Button(button_name0, ImVec2(-1, 0)))
				switch_entity_teams = !switch_entity_teams;

			const char* button_name1 = switch_color_vis ? "Invisible" : "Visible";
			if (ImGui::Button(button_name1, ImVec2(-1, 0)))
				switch_color_vis = !switch_color_vis;

			std::string id_new = picker_idname;
			id_new += "##pickeritself_";

			ImGui::ColorPicker4(id_new.c_str(), switch_entity_teams ? (switch_color_vis ? col_t : col_t_invis) : (switch_color_vis ? col_ct : col_ct_invis), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0));
			ImGui::EndPopup();
		}
	}

	void LocalPlayerColorPickerBox(const char* picker_idname, float col_ct[], float col_t[], bool alpha = false)
	{
		ImGui::SameLine();
		static bool switch_entity_teams = false;
		static bool switch_color_vis = false;
		bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (col_t) : (col_ct), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(36, 0));
		if (open_popup)
		{
			ImGui::OpenPopup(picker_idname);
			Colorpicker_Close = ImGui::GetID(picker_idname);
		}

		if (ImGui::BeginPopup(picker_idname))
		{
			const char* button_name1 = switch_color_vis ? "Invisible" : "Visible";
			if (ImGui::Button(button_name1, ImVec2(-1, 0)))
				switch_color_vis = !switch_color_vis;

			std::string id_new = picker_idname;
			id_new += "##pickeritself_";

			ImGui::ColorPicker4(id_new.c_str(), switch_color_vis ? (col_t) : (col_ct), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0));
			ImGui::EndPopup();
		}
	}

	void CustomColorPickerBox(const char* picker_idname, float col_ct[], float col_t[], bool alpha = false)
	{
		ImGui::SameLine();
		static bool switch_entity_teams = false;
		static bool switch_color_vis = false;
		bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (col_t) : (col_ct), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(36, 0));
		if (open_popup)
		{
			ImGui::OpenPopup(picker_idname);
			Colorpicker_Close = ImGui::GetID(picker_idname);
		}

		if (ImGui::BeginPopup(picker_idname))
		{
			const char* button_name1 = switch_color_vis ? "Skybox" : "World";
			if (ImGui::Button(button_name1, ImVec2(-1, 0)))
				switch_color_vis = !switch_color_vis;

			std::string id_new = picker_idname;
			id_new += "##pickeritself_";

			ImGui::ColorPicker4(id_new.c_str(), switch_color_vis ? (col_t) : (col_ct), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0));
			ImGui::EndPopup();
		}
	}

	void ColorPickerBox(const char* picker_idname, float col_n[], bool alpha = true)
	{
		ImGui::SameLine();
		bool open_popup = ImGui::ColorButton(picker_idname, col_n, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(36, 0));
		if (open_popup)
		{
			ImGui::OpenPopup(picker_idname);
			Colorpicker_Close = ImGui::GetID(picker_idname);
		}

		if (ImGui::BeginPopup(picker_idname))
		{
			std::string id_new = picker_idname;
			id_new += "##pickeritself_";

			ImGui::ColorPicker4(id_new.c_str(), col_n, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0));
			ImGui::EndPopup();
		}
	}

	// This can be used anywhere, in group boxes etc.
	void SelectTabs(int *selected, const char* items[], int item_count, ImVec2 size = ImVec2(0, 0))
	{
		auto color_grayblue = GetColorU32(ImVec4(0.54, 0.54, 0.54, 0.30));
		auto color_deepblue = GetColorU32(ImVec4(0.54, 0.54, 0.54, 0.25));
		auto color_shade_hover = GetColorU32(ImVec4(1, 1, 1, 0.05));
		auto color_shade_clicked = GetColorU32(ImVec4(1, 1, 1, 0.1));
		auto color_black_outlines = GetColorU32(ImVec4(0, 0, 0, 1));

		ImGuiStyle &style = GetStyle();
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return;

		std::string names;
		for (int32_t i = 0; i < item_count; i++)
			names += items[i];

		ImGuiContext* g = GImGui;
		const ImGuiID id = window->GetID(names.c_str());
		const ImVec2 label_size = CalcTextSize(names.c_str(), NULL, true);

		ImVec2 Min = window->DC.CursorPos;
		ImVec2 Max = ((size.x <= 0 || size.y <= 0) ? ImVec2(Min.x + GetContentRegionMax().x - style.WindowPadding.x, Min.y + label_size.y * 2) : Min + size);

		ImRect bb(Min, Max);
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return;

		PushClipRect(ImVec2(Min.x, Min.y - 1), ImVec2(Max.x, Max.y + 1), false);

		window->DrawList->AddRectFilledMultiColor(Min, Max, color_grayblue, color_grayblue, color_deepblue, color_deepblue); // Main gradient.

		ImVec2 mouse_pos = GetMousePos();
		bool mouse_click = g->IO.MouseClicked[0];

		float TabSize = ceil((Max.x - Min.x) / item_count);

		for (int32_t i = 0; i < item_count; i++)
		{
			ImVec2 Min_cur_label = ImVec2(Min.x + (int)TabSize * i, Min.y);
			ImVec2 Max_cur_label = ImVec2(Min.x + (int)TabSize * i + (int)TabSize, Max.y);

			// Imprecision clamping. gay but works :^)
			Max_cur_label.x = (Max_cur_label.x >= Max.x ? Max.x : Max_cur_label.x);

			if (mouse_pos.x > Min_cur_label.x && mouse_pos.x < Max_cur_label.x &&
				mouse_pos.y > Min_cur_label.y && mouse_pos.y < Max_cur_label.y)
			{
				if (mouse_click)
					*selected = i;
				else if (i != *selected)
					window->DrawList->AddRectFilled(Min_cur_label, Max_cur_label, color_shade_hover);
			}

			if (i == *selected) {
				window->DrawList->AddRectFilled(Min_cur_label, Max_cur_label, color_shade_clicked);
				window->DrawList->AddRectFilledMultiColor(Min_cur_label, Max_cur_label, color_deepblue, color_deepblue, color_grayblue, color_grayblue);
				window->DrawList->AddLine(ImVec2(Min_cur_label.x - 1.5f, Min_cur_label.y - 1), ImVec2(Max_cur_label.x - 0.5f, Min_cur_label.y - 1), color_black_outlines);
			}
			else
				window->DrawList->AddLine(ImVec2(Min_cur_label.x - 1, Min_cur_label.y), ImVec2(Max_cur_label.x, Min_cur_label.y), color_black_outlines);
			window->DrawList->AddLine(ImVec2(Max_cur_label.x - 1, Max_cur_label.y), ImVec2(Max_cur_label.x - 1, Min_cur_label.y - 0.5f), color_black_outlines);

			const ImVec2 text_size = CalcTextSize(items[i], NULL, true);
			float pad_ = style.FramePadding.x + g->FontSize + style.ItemInnerSpacing.x;
			ImRect tab_rect(Min_cur_label, Max_cur_label);
			RenderTextClipped(Min_cur_label, Max_cur_label, items[i], NULL, &text_size, style.WindowTitleAlign, &tab_rect);
		}

		window->DrawList->AddLine(ImVec2(Min.x, Min.y - 0.5f), ImVec2(Min.x, Max.y), color_black_outlines);
		window->DrawList->AddLine(ImVec2(Min.x, Max.y), Max, color_black_outlines);
		PopClipRect();
	}
}

namespace GladiatorMenu
{
	ImFont* cheat_font;
	ImFont* title_font;
	
	void GUI_Init(HWND window, IDirect3DDevice9 *pDevice)
	{
		static int hue = 140;

		ImGui_ImplDX9_Init(window, pDevice);

		ImGuiStyle &style = ImGui::GetStyle();

		ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
		ImVec4 col_main = ImColor(147, 147, 147); //ImColor(9, 82, 128); button
		ImVec4 col_back = ImColor(89, 89, 89); // button non active
		ImVec4 col_area = ImColor(4, 32, 41); // button outline
		ImVec4 col_theme = ImColor(130, 177, 255); // keine ahnung xD

		ImVec4 col_title = ImColor(89,89,89);
		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.f, 1.f, 1.f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.65f, 0.65f, 0.30f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(8, 8);
		style.WindowRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.ChildWindowRounding = 3.0f;
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 0.0f;
		style.ItemSpacing = ImVec2(8, 4);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 3.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;
		style.WindowPadThickness = 4.f;

		d3dinit = true;
		cheat_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 13);
		title_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14);
	}

	void openMenu()
	{
		static bool bDown = false;
		static bool bClicked = false;
		static bool bPrevMenuState = menuOpen;

		if (pressedKey[VK_INSERT])
		{
			bClicked = false;
			bDown = true;
		}
		else if (!pressedKey[VK_INSERT] && bDown)
		{
			bClicked = true;
			bDown = false;
		}
		else
		{
			bClicked = false;
			bDown = false;
		}

		if (bClicked)
		{
			menuOpen = !menuOpen;
			ImGui::CloseLeftoverPicker();
		}

		if (bPrevMenuState != menuOpen)
		{
			// outdated (nkpl)
			//std::string msg = "cl_mouseenable " + std::to_string(!menuOpen);
			//g_EngineClient->ExecuteClientCmd(msg.c_str());
		}

		bPrevMenuState = menuOpen;
	}

	void mainWindow()
	{
		ImGui::SetNextWindowSize(ImVec2(860, 540), ImGuiSetCond_FirstUseEver);

		ImGui::PushFont(title_font);
		if (ImGui::Begin("lumihook fixed by soma#1678 | https://discord.com/invite/WPag8RJ", &menuOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_OnlyDragByTitleBar))
		{
			ImGui::BeginGroup();

			static int selected_Tab = 0;
			static const char* items[7] = { "Rage","Legit", "Visuals", "Misc", "Anti-Aim", "Resolver" ,"Skins" };
			ImGui::SelectTabs(&selected_Tab, items, 7);

			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::PushID(selected_Tab);		

			ImGui::PushFont(cheat_font);
			switch (selected_Tab)
			{
			case 0:
				ragebotTab();
				break;
			case 1:
				legitTab();
				break;
			case 2:
				visualTab();
				break;
			case 3:
				miscTab();
				break;
			case 4:
				hvhTab();
				break;
			case 5:
				resolverTab();
				break;
			case 6:
				skinchangerTab();
				break;
			}
			ImGui::PopFont();

			ImGui::PopID();
			ImGui::EndGroup();

			ImGui::End();
		}
		ImGui::PopFont();
	}

	void aimbotTab()
	{
		ImGui::Separator();
		static int selected_Tab = 0;
		static const char* items[9] = { "Pistol", "Deagle/R8" ,"Submachine-Gun", "Shotgun" ,"Machine-Gun", "Assault-Rifle", "Scout", "AWP" ,"Auto-Sniper" };
		ImGui::SelectTabs(&selected_Tab, items, 9);

		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::PushID(selected_Tab);

		ImGui::PushFont(cheat_font);
		ImGui::Separator();
		switch (selected_Tab)
		{
		case 0: // Pistol
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Pistol ##RagePistol", &g_Options.rage_mindmg_amount_pistol, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Pistol %##RagePistol", &g_Options.rage_hitchance_amount_pistol, 0.f, 100.0f, "%.2f");
			}			
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RagePistol", &g_Options.bRage_prioritize_pistol);
				ImGui::Combo("Select Hitbox##RagePistol", &g_Options.iRage_hitbox_pistol, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RagePistol", &g_Options.bRage_multipoint_pistol);
				ImGui::SliderFloat("Pointscale##RagePistol", &g_Options.rage_pointscale_amount_pistol, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTPISTOL", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesPistol[i]);
				}
				ImGui::EndChild();
			}
			break;
		case 1: // Deagle
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Deagle/R8 ##RageDeagR8", &g_Options.rage_mindmg_amount_deagr8, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Deagle/R8 %##RageDeagR8", &g_Options.rage_hitchance_amount_deagr8, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageDeagR8", &g_Options.bRage_prioritize_deagr8);
				ImGui::Combo("Select Hitbox##RageDeagR8", &g_Options.iRage_hitbox_deagr8, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageDeagR8", &g_Options.bRage_multipoint_deagr8);
				ImGui::SliderFloat("Pointscale##RageDeagR8", &g_Options.rage_pointscale_amount_deagr8, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTDEAGLER8", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesDeagR8[i]);
				}
				ImGui::EndChild();
			}
			break;

		case 2: // SMG
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("SMG ##RageSMG", &g_Options.rage_mindmg_amount_smg, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("SMG %##RageSMG", &g_Options.rage_hitchance_amount_smg, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageSMG", &g_Options.bRage_prioritize_smg);
				ImGui::Combo("Select Hitbox##RageSMG", &g_Options.iRage_hitbox_smg, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageSMG", &g_Options.bRage_multipoint_smg);
				ImGui::SliderFloat("Pointscale##RageSMG", &g_Options.rage_pointscale_amount_smg, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTSMG", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesSMG[i]);
				}
				ImGui::EndChild();
			}
			break;

		case 3: // Shotgun
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Shotgun ##RageSHOTGUN", &g_Options.rage_mindmg_amount_shotgun, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Shotgun %##RageSHOTGUN", &g_Options.rage_hitchance_amount_shotgun, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageSHOTGUN", &g_Options.bRage_prioritize_shotgun);
				ImGui::Combo("Select Hitbox##RageSHOTGUN", &g_Options.iRage_hitbox_shotgun, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageSHOTGUN", &g_Options.bRage_multipoint_shotgun);
				ImGui::SliderFloat("Pointscale##RageSHOTGUN", &g_Options.rage_pointscale_amount_shotgun, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTSHOTGUN", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesShotgun[i]);
				}
				ImGui::EndChild();
			}
			break;

		case 4: // MG
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("MG ##RageMG", &g_Options.rage_mindmg_amount_mg, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("MG %##RageMG", &g_Options.rage_hitchance_amount_mg, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageMG", &g_Options.bRage_prioritize_mg);
				ImGui::Combo("Select Hitbox##RageMG", &g_Options.iRage_hitbox_mg, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageMG", &g_Options.bRage_multipoint_mg);
				ImGui::SliderFloat("Pointscale##RageMG", &g_Options.rage_pointscale_amount_mg, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTMG", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesMG[i]);
				}
				ImGui::EndChild();
			}
			break;

		case 5: // ASSAULT RIFLE
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Assault Rifle ##RageAssaultRifle", &g_Options.rage_mindmg_amount_assaultrifle, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Assault Rifle %##RageAssaultRifle", &g_Options.rage_hitchance_amount_assaultrifle, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageAssaultRifle", &g_Options.bRage_prioritize_assaultrifle);
				ImGui::Combo("Select Hitbox##RageAssaultRifle", &g_Options.iRage_hitbox_assaultrifle, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageAssaultRifle", &g_Options.bRage_multipoint_assaultrifle);
				ImGui::SliderFloat("Pointscale##RageAssaultRifle", &g_Options.rage_pointscale_amount_assaultrifle, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTAssaultRifle", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesAssaultRifle[i]);
				}
				ImGui::EndChild();
			}
			break;
		case 6: // SCOUT
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Scout ##RageScout", &g_Options.rage_mindmg_amount_scout, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Scout %##RageScout", &g_Options.rage_hitchance_amount_scout, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageScout", &g_Options.bRage_prioritize_scout);
				ImGui::Combo("Select Hitbox##RageScout", &g_Options.iRage_hitbox_scout, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageScout", &g_Options.bRage_multipoint_scout);
				ImGui::SliderFloat("Pointscale##RageScout", &g_Options.rage_pointscale_amount_scout, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTScout", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesScout[i]);
				}
				ImGui::EndChild();
			}
			break;
		case 7: // AWP
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("AWP ##RageAWP", &g_Options.rage_mindmg_amount_awp, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("AWP %##RageAWP", &g_Options.rage_hitchance_amount_awp, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageAWP", &g_Options.bRage_prioritize_awp);
				ImGui::Combo("Select Hitbox##RageAWP", &g_Options.iRage_hitbox_awp, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageAWP", &g_Options.bRage_multipoint_awp);
				ImGui::SliderFloat("Pointscale##RageAWP", &g_Options.rage_pointscale_amount_awp, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTAWP", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesAWP[i]);
				}
				ImGui::EndChild();
			}
			break;
		case 8: // AUTO SNIPER
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Autowall");
				ImGui::SliderFloat("Auto ##RageAuto", &g_Options.rage_mindmg_amount_auto, 0.f, 120.f, "%.2f");
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Auto %##RageAuto", &g_Options.rage_hitchance_amount_auto, 0.f, 100.0f, "%.2f");
			}
			ImGui::EndColumns();
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Prioritize Selected Hitbox##RageAuto", &g_Options.bRage_prioritize_auto);
				ImGui::Combo("Select Hitbox##RageAuto", &g_Options.iRage_hitbox_auto, opt_AimHitboxSpot, 4);
				ImGui::Checkbox("Multipoint##RageAuto", &g_Options.bRage_multipoint_auto);
				ImGui::SliderFloat("Pointscale##RageAuto", &g_Options.rage_pointscale_amount_auto, 0.0f, 1.0f);
			}
			ImGui::NextColumn();
			{
				ImGui::Text("Hitboxes To Scan:");
				ImGui::BeginChild("#MULTIPOINTAuto", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
				{
					ImGui::Selectable(opt_MultiHitboxes[i], &g_Options.rage_multiHitboxesAuto[i]);
				}
				ImGui::EndChild();
			}
			break;
		}
		//ImGui::Separator();
		ImGui::PopFont();
		ImGui::PopID();
		ImGui::EndGroup();
	}

	void legitTab()
	{
		ImGui::BeginChild("LEGITBOTCHILD", ImVec2(0, 0), true);
		{
			ImGui::Text("Aim Assistance");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Aim-Assist##Legit", &g_Options.legit_enabled);
				ImGui::Checkbox("Aim on key only", &g_Options.legit_on_press);
				ImGui::KeyBindButton(&g_Options.legit_aim_keybind);
				ImGui::Checkbox("Silent##Legit", &g_Options.rage_silent);
				ImGui::Checkbox("RCS##Legit", &g_Options.legit_rcs);
				ImGui::Checkbox("RCS on key only", &g_Options.rcs_on_press);
				ImGui::KeyBindButton(&g_Options.rcs_keybind);
				ImGui::Checkbox("Trigger##Legit", &g_Options.legit_trigger);
				ImGui::Checkbox("Trigger on key only", &g_Options.trigger_on_press);
				ImGui::KeyBindButton(&g_Options.trigger_keybind);
				ImGui::Checkbox("Head only", &g_Options.legit_hsonly);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Aim Spot");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Pre Aim Spot");
				ImGui::NewLine();
				ImGui::Text("Bullet After Aim");
				ImGui::NewLine();
				ImGui::Text("After Aim Spot");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##PREAIMSPOT", &g_Options.legit_preaim, opt_AimSpot, 4);
				ImGui::NewLine();
				ImGui::SliderInt("##BULLETAFTERAIM", &g_Options.legit_aftershots, 3, 15);
				ImGui::NewLine();
				ImGui::Combo("##AFTERAIMSPOT", &g_Options.legit_afteraim, opt_AimSpot, 4);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Fov & Smooth");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Fov");
				ImGui::NewLine();
				ImGui::Text("Smooth");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::SliderFloat("##FOV", &g_Options.legit_fov, 0.f, 3.f, "%.2f");
				ImGui::NewLine();
				ImGui::SliderFloat("##SMOOTH", &g_Options.legit_smooth_factor, 1.f, 30.f, "%.2f");
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();

			ImGui::EndChild();
		}
	}

	void ragebotTab()
	{
		ImGui::Columns(1, NULL, false);
		{
			ImGui::BeginChild("RAGEBOTCHILD", ImVec2(0, 0), true);
			{
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Enable Rage##Rage", &g_Options.rage_enabled);
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Columns(2, NULL, true);
				{
					ImGui::Checkbox("Silent##Rage", &g_Options.rage_silent);
					ImGui::Checkbox("No Recoil##Rage", &g_Options.rage_norecoil);
					ImGui::Checkbox("Auto Shoot##Rage", &g_Options.rage_autoshoot);
					//ImGui::Checkbox("Resolver##Resolver", &g_Options.hvh_resolver);
					//ImGui::Checkbox("Resolver Flip##Resolver", &g_Options.hvh_resolver_override);
					//ImGui::KeyBindButton(&g_Options.hvh_resolver_override_key);
					ImGui::Checkbox("Position Adjustment (Backtrack)##Rage", &g_Options.rage_lagcompensation);
					ImGui::Checkbox("only shoot backtrack if needed##Rage", &g_Options.backtrack_bhd_wall_only);
				}
				ImGui::NextColumn();
				{
					ImGui::Checkbox("Auto Scope##Rage", &g_Options.rage_autoscope);
					ImGui::Checkbox("Auto Crouch##Rage", &g_Options.rage_autocrouch);
					ImGui::Checkbox("Auto Stop##Rage", &g_Options.rage_autostop);
					ImGui::Checkbox("Auto Revolver##Rage", &g_Options.rage_autocockrevolver);
					//ImGui::Checkbox("Aimstep##Rage", &g_Options.rage_aimstep);
					//ImGui::Checkbox("Auto Body-Aim##Rage", &g_Options.rage_autobaim);
					//ImGui::SliderInt("Body-Aim After Shots##Rage", &g_Options.rage_baim_after_x_shots, 0, 10);
				}

				ImGui::Columns(1);
				//ImGui::Separator();
				ImGui::Columns(1, NULL, false);
				{
					ImGui::PushItemWidth(235);
					//ImGui::Checkbox("Position Adjustment##Rage", &g_Options.rage_lagcompensation);
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				/*ImGui::Separator();
				ImGui::Columns(2, NULL, true);
				{
					ImGui::Text("Autowall");
					ImGui::SliderFloat("Auto ##Rage", &g_Options.rage_mindmg_amount_auto, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("AWP ##Rage", &g_Options.rage_mindmg_amount_awp, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("Scout ##Rage", &g_Options.rage_mindmg_amount_scout, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("Pistol ##Rage", &g_Options.rage_mindmg_amount_pistol, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("DeagR8 ##Rage", &g_Options.rage_mindmg_amount_deagr8, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("Shotgun ##Rage", &g_Options.rage_mindmg_amount_shotgun, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("MG ##Rage", &g_Options.rage_mindmg_amount_mg, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("SMG ##Rage", &g_Options.rage_mindmg_amount_smg, 0.f, 120.f, "%.2f");
					ImGui::SliderFloat("Assault Rifle ##Rage", &g_Options.rage_mindmg_amount_assaultrifle, 0.f, 120.f, "%.2f");
				}
				ImGui::NextColumn();
				{
					ImGui::Text("Hitchance");
					ImGui::SliderFloat("Auto %##Rage", &g_Options.rage_hitchance_amount_auto, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("AWP %##Rage", &g_Options.rage_hitchance_amount_awp, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("Scout %##Rage", &g_Options.rage_hitchance_amount_scout, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("Pistol %##Rage", &g_Options.rage_hitchance_amount_pistol, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("DeagR8 %##Rage", &g_Options.rage_hitchance_amount_deagr8, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("Shotgun %##Rage", &g_Options.rage_hitchance_amount_shotgun, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("MG %##Rage", &g_Options.rage_hitchance_amount_mg, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("SMG %##Rage", &g_Options.rage_hitchance_amount_smg, 0.f, 100.0f, "%.2f");
					ImGui::SliderFloat("Assault Rifle %##Rage", &g_Options.rage_hitchance_amount_assaultrifle, 0.f, 100.0f, "%.2f");
				}*/
				

				ImGui::Columns(1);
				//ImGui::Separator();
				//ImGui::Text("Target");
				//ImGui::Separator();
				ImGui::BeginGroup();

				aimbotTab();

				ImGui::Columns(1);
				ImGui::Separator();

				ImGui::EndChild();
			}
		}
	}

	void visualTab()
	{
		ImGui::Columns(2, NULL, false);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{
				ImGui::Text("ESP");
				ImGui::Separator();
				ImGui::Columns(1);
				{
					ImGui::Combo("Box Type##BOXTYPE", &g_Options.esp_player_boxtype, opt_EspType, 3);
					ImGui::ColorPickerBox("##Picker_box", g_Options.esp_player_bbox_color_ct, g_Options.esp_player_bbox_color_t, g_Options.esp_player_bbox_color_ct_visible, g_Options.esp_player_bbox_color_t_visible, false);
					ImGui::Combo("Bounds Style##BOUNDSTYPE", &g_Options.esp_player_boundstype, opt_BoundsType, 2);
					ImGui::SliderFloat("Fill Alpha##ESP", &g_Options.esp_fill_amount, 0.f, 255.f);
					ImGui::ColorPickerBox("##Picker_fill", g_Options.esp_player_fill_color_ct, g_Options.esp_player_fill_color_t, g_Options.esp_player_fill_color_ct_visible, g_Options.esp_player_fill_color_t_visible, false);
					//ImGui::Checkbox("Ignore Team##ESP", &g_Options.esp_enemies_only);
					ImGui::Checkbox("Farther ESP##ESP", &g_Options.esp_farther);
					ImGui::Checkbox("Name##ESP", &g_Options.esp_player_name);
					ImGui::Checkbox("Health##ESP", &g_Options.esp_player_health);
					ImGui::Checkbox("Weapon Name##ESP", &g_Options.esp_player_weapons);
					ImGui::Checkbox("Skeleton##ESP", &g_Options.esp_player_skelet);
					ImGui::Checkbox("Change World Color##ESP", &g_Options.visuals_nightmode);
					ImGui::CustomColorPickerBox("##visuals_others_nightmode_color", g_Options.visuals_others_nightmode_color, g_Options.visuals_others_skybox_color, false);
					//ImGui::SameLine();
					//ImGui::ColorPickerBox("##visuals_others_skybox_color", g_Options.visuals_others_skybox_color, false);
					ImGui::Checkbox("Asus Walls##ESP", &g_Options.visuals_asuswalls);
					if (g_Options.visuals_draw_xhair)
						ImGui::Separator();
					ImGui::Checkbox("Draw Crosshair", &g_Options.visuals_draw_xhair);
					if (g_Options.visuals_draw_xhair)
					{
						ImGui::Separator();
						ImGui::SliderInt("Crosshair X length", &g_Options.visuals_xhair_x, 0, 15);
						ImGui::SliderInt("Crosshair Y length", &g_Options.visuals_xhair_y, 0, 15);
						ImGui::NewLine();
						ImGui::SliderInt("Crosshair - Red", &g_Options.xhair_color[0], 0, 255);
						ImGui::SliderInt("Crosshair - Green", &g_Options.xhair_color[1], 0, 255);
						ImGui::SliderInt("Crosshair - Blue", &g_Options.xhair_color[2], 0, 255);

						ImGui::Separator();
					}
					ImGui::Checkbox("Backtracked Trail##ESP", &g_Options.esp_backtracked_player_skelet);
					ImGui::Checkbox("Lag Compensated Hitboxes##ESP", &g_Options.esp_lagcompensated_hitboxes);
					if (g_Options.esp_lagcompensated_hitboxes)
						ImGui::Combo("Type##ESP", &g_Options.esp_lagcompensated_hitboxes_type, std::vector<std::string>{ "Only Lag Compensated", "Lag(Red) & Non(Blue) Lag Compensated" });
					ImGui::Checkbox("Angle Lines##ESP", &g_Options.esp_player_anglelines);
					ImGui::Checkbox("Chams##ESP", &g_Options.esp_player_chams);
					if (g_Options.esp_player_chams_type > 1 || g_Options.esp_localplayer_chams_type > 1)
						ImGui::Text("set shader details to atleast high, to get the full effect of the new materials!!");
					ImGui::Combo("Chams Type##ESP", &g_Options.esp_player_chams_type, opt_Chams, 10);
					ImGui::ColorPickerBox("##Picker_chams_players", g_Options.esp_player_chams_color_ct, g_Options.esp_player_chams_color_t, g_Options.esp_player_chams_color_ct_visible, g_Options.esp_player_chams_color_t_visible, false);
					ImGui::Separator();
					ImGui::Checkbox("Chams for LocalPlayer", &g_Options.esp_localplayer_chams);
					if (g_Options.esp_localplayer_chams)
					{
						ImGui::Checkbox("XQZ - Chams", &g_Options.esp_localplayer_chams_xyz);
						ImGui::Combo("Real Material##ESP", &g_Options.esp_localplayer_chams_type, opt_Chams_Mat, 9);
						ImGui::LocalPlayerColorPickerBox("##esp_localplayer_chams_color_picker", g_Options.esp_localplayer_chams_color_visible, g_Options.esp_localplayer_chams_color_invisible, false);

						ImGui::Checkbox("Fake Angle Chams", &g_Options.fake_chams);
						if (g_Options.fake_chams)
						{
							ImGui::Separator();
							ImGui::Combo("Fake Material##FakeChams", &g_Options.esp_localplayer_fakechams_type, opt_Chams_Mat, 9);
							ImGui::LocalPlayerColorPickerBox("##esp_localplayer_fakechams_color_picker", g_Options.esp_localplayer_fakechams_color_visible, g_Options.esp_localplayer_fakechams_color_invisible, false);
						}
						ImGui::Separator();
						ImGui::Checkbox("Viewmodel Chams", &g_Options.esp_localplayer_viewmodel_chams);
						if (g_Options.esp_localplayer_viewmodel_chams)
						{
							ImGui::Separator();
							//ImGui::Checkbox("Wireframe", &g_Options.esp_localplayer_viewmodel_chams_wireframe);
							ImGui::Combo("Viewmodel Material##ViewmodelChams", &g_Options.esp_localplayer_viewmodel_materials, opt_Chams_Mat, 9);
							//ImGui::ColorPickerBox("##esp_localplayer_viewmodel_chams_color", g_Options.esp_localplayer_viewmodel_chams_color, true);
						}
					}
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Glow");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Enable Glow##Glow", &g_Options.glow_enabled);
					ImGui::Checkbox("Players##Glow", &g_Options.glow_players);
					ImGui::Combo("Glow Type##ESP_player", &g_Options.glow_players_style, opt_GlowStyles, 3);
					ImGui::ColorPickerBox("##Picker_glow_players", g_Options.glow_player_color_ct, g_Options.glow_player_color_t, g_Options.glow_player_color_ct_visible, g_Options.glow_player_color_t_visible);
					ImGui::Checkbox("Others##Glow", &g_Options.glow_others);
					ImGui::Combo("Glow type others##ESP_other", &g_Options.glow_others_style, opt_GlowStyles, 3);
				}
				
				ImGui::EndChild();
			}
		}
		ImGui::NextColumn();
		{
			ImGui::BeginChild("COL2", ImVec2(0, 0), true);
			{
				ImGui::Text("Others");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Hit Marker##Others", &g_Options.visuals_others_hitmarker);
					ImGui::Checkbox("Bullet Impacts##Others", &g_Options.visuals_others_bulletimpacts);
					ImGui::ColorPickerBox("##Picker_impacts", g_Options.visuals_others_bulletimpacts_color, false);
					ImGui::Separator();
					ImGui::Checkbox("Show Manual AA Status", &g_Options.visuals_manual_aa);
					ImGui::SliderFloat("Indicator Opacity", &g_Options.visuals_manual_aa_opacity, 0, 255);
					ImGui::Separator();
					ImGui::Checkbox("Grenade Prediction##Others", &g_Options.visuals_others_grenade_pred);
					//ImGui::Checkbox("Autothrow", &g_Options.autothrow_grenades);
					//ImGui::SliderInt("Autothrow Damage", &g_Options.autothrow_grenades_dmg, 0, 100);
					ImGui::Checkbox("Watermark##Others", &g_Options.visuals_others_watermark);
					ImGui::SliderFloat("Field of View##Others", &g_Options.visuals_others_player_fov, 0, 80);
					ImGui::SliderFloat("Viewmodel Field of View##Others", &g_Options.visuals_others_player_fov_viewmodel, 0, 80);
					ImGui::Separator();
					ImGui::Checkbox("Enable Custom Viewmodel Offset", &g_Options.change_viewmodel_offset);
					ImGui::SliderFloat("Viewmodel Offset X##ViewmodelX", &g_Options.viewmodel_offset_x, -30, 30);
					ImGui::SliderFloat("Viewmodel Offset Y##ViewmodelY", &g_Options.viewmodel_offset_y, -30, 30);
					ImGui::SliderFloat("Viewmodel Offset Z##ViewmodelZ", &g_Options.viewmodel_offset_z, -30, 30);
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Removals");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Flash##Removals", &g_Options.removals_flash);
					ImGui::Checkbox("Smoke##Removals", &g_Options.removals_smoke);
					if (g_Options.removals_smoke)
					{
						ImGui::Combo("Smoke Mode##Removals", &g_Options.removals_smoke_type, opt_nosmoketype, 2);
					}
					ImGui::Checkbox("Scope##Removals", &g_Options.removals_scope);
					ImGui::Checkbox("Zoom##Removals", &g_Options.removals_zoom);
					ImGui::Checkbox("Recoil##Removals", &g_Options.removals_novisualrecoil);
					ImGui::Checkbox("Crosshair##Removals", &g_Options.removals_crosshair);
					ImGui::Checkbox("Fog##Removals", &g_Options.fog_override);
					ImGui::Checkbox("Post-Processing##Removals", &g_Options.removals_postprocessing);
				}

				ImGui::Separator();
				ImGui::Text("Other esp");
				ImGui::Separator();
				ImGui::Combo("Dropped Weapons##ESP", &g_Options.esp_dropped_weapons, opt_WeaponBoxType, 4);
				if (g_Options.esp_dropped_weapons > 0)
				{
					ImGui::SliderInt("Drpd. Wep. Red##ESP", &g_Options.dropped_weapons_color[0], 0, 255);
					ImGui::SliderInt("Drpd. Wep. Green##ESP", &g_Options.dropped_weapons_color[1], 0, 255);
					ImGui::SliderInt("Drpd. Wep. Blue##ESP", &g_Options.dropped_weapons_color[2], 0, 255);
				}
				ImGui::Separator();
				ImGui::Checkbox("Planted C4##ESP", &g_Options.esp_planted_c4);
				ImGui::Checkbox("Grenade ESP##ESP", &g_Options.esp_grenades);
				ImGui::Combo("Grenade ESP type##ESP", &g_Options.esp_grenades_type, opt_GrenadeESPType, 4);
				ImGui::EndChild();
			}
		}
	}

	void miscTab()
	{
		ImGui::Columns(2, NULL, false);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{
				if (ImGui::Button("Unload"))
					Installer::UnloadLumi();
				ImGui::Text("Movement");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Bhop##Movement", &g_Options.misc_bhop);
					ImGui::Checkbox("Auto-Strafe##Movement", &g_Options.misc_autostrafe);
				}

				static char nName[127] = "";
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Nickname");
				ImGui::Separator();
				ImGui::Columns(2, NULL, true);
				{
					ImGui::PushItemWidth(-1);
					ImGui::InputText("##NNAMEINPUT", nName, 127);
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				{
					if (ImGui::Button("Set Nickname##Nichnamechanger"))
						Miscellaneous::Get().ChangeName(nName);

					ImGui::SameLine();
					if (ImGui::Button("No Name##Nichnamechanger", ImVec2(-1, 0)))
						Miscellaneous::Get().ChangeName("\n");
				}

				static char ClanChanger[127] = "";
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Clan Tag");
				ImGui::Separator();
				ImGui::Columns(2, NULL, true);
				{
					ImGui::PushItemWidth(-1);
					ImGui::InputText("##CLANINPUT", ClanChanger, 127);
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				{
					if (ImGui::Button("Set Clan-Tag"))
						Utils::SetClantag(ClanChanger);

				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Other");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Auto-Pistol##Other", &g_Options.misc_auto_pistol);
					ImGui::Checkbox("Chat Spammer##Other", &g_Options.misc_chatspamer);
					ImGui::Checkbox("Event Logs##Other", &g_Options.misc_logevents);
					ImGui::Checkbox("Spectator List##Other", &g_Options.misc_spectatorlist);
					ImGui::Checkbox("Clantag##Other", &g_Options.misc_animated_clantag);
					ImGui::Checkbox("Thirdperson##Other", &g_Options.misc_thirdperson);
					ImGui::KeyBindButton(&g_Options.misc_thirdperson_bind);
					ImGui::Checkbox("Infinite Duck", &g_Options.misc_infinite_duck);
					ImGui::Combo("Thirdperson Mode##AntiAim", &g_Options.hvh_show_real_angles, std::vector<std::string>{ "Real Angles", "Fake Angles" });
					ImGui::Checkbox("Slowwalk##Other", &g_Options.misc_fakewalk);
					ImGui::KeyBindButton(&g_Options.misc_fakewalk_bind);
					ImGui::SliderInt("Slowwalk Speed##Other", &g_Options.misc_fakewalk_speed, 0, 130);
				}

				ImGui::EndChild();
			}
		}
		ImGui::NextColumn();
		{
			ImGui::BeginChild("COL2", ImVec2(0, 0), true);
			{
				ImGui::Text("Fakelag");
				ImGui::Separator();
				ImGui::Columns(1, NULL, true);
				{
					ImGui::Checkbox("Enabled##Fakelag", &g_Options.misc_fakelag_enabled);
					ImGui::KeyBindButton(&g_Options.misc_fakeduck);
					ImGui::Checkbox("Adaptive##Fakelag", &g_Options.misc_fakelag_adaptive);
					ImGui::Combo("Activation Type##Fakelag", &g_Options.misc_fakelag_activation_type, std::vector<std::string>{ "Always", "While Moving", "In Air" });
					ImGui::SliderInt("Choke##Fakelag", &g_Options.misc_fakelag_value, 0, 14);
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("Config");
				ImGui::Separator();
				static std::vector<std::string> configItems = Config::Get().GetAllConfigs();
				static int configItemCurrent = -1;

				static char fName[128] = "default";

				ImGui::Columns(1, NULL, true);
				{
					if (ImGui::Button("Refresh##Config"))
						configItems = Config::Get().GetAllConfigs();

					ImGui::SameLine();
					if (ImGui::Button("Save##Config"))
					{
						if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
						{
							std::string fPath = std::string(Global::my_documents_folder) + "\\LumiHook$\\" + configItems[configItemCurrent] + ".1337";
							Config::Get().SaveConfig(fPath);
						}
					}

					ImGui::SameLine();
					if (ImGui::Button("Remove##Config"))
					{
						if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
						{
							std::string fPath = std::string(Global::my_documents_folder) + "\\LumiHook$\\" + configItems[configItemCurrent] + ".1337";
							std::remove(fPath.c_str());

							configItems = Config::Get().GetAllConfigs();
							configItemCurrent = -1;
						}
					}

					ImGui::PushItemWidth(138);
					{
						ImGui::InputText("", fName, 128);
					}
					ImGui::PopItemWidth();

					ImGui::SameLine();
					if (ImGui::Button("Add##Config"))
					{
						std::string fPath = std::string(Global::my_documents_folder) + "\\LumiHook$\\" + fName + ".1337";
						Config::Get().SaveConfig(fPath);

						configItems = Config::Get().GetAllConfigs();
						configItemCurrent = -1;
					}

					ImGui::PushItemWidth(178);
					{
						if (ImGui::ListBox("", &configItemCurrent, configItems, 5))
						{
							std::string fPath = std::string(Global::my_documents_folder) + "\\LumiHook$\\" + configItems[configItemCurrent] + ".1337";
							Config::Get().LoadConfig(fPath);
						}
					}
					ImGui::PopItemWidth();
					ImGui::Separator();
					ImGui::Text("super retarded shit");
					ImGui::Separator();
					ImGui::Checkbox("spoof cl_phys_timescale", &g_Options.cl_phys_timescale);
					ImGui::SliderFloat("cl_phys_timescale_value##cl_phys_timescale", &g_Options.cl_phys_timescale_value, 0, 1, "%.1f");
					ImGui::Separator();
					ImGui::Checkbox("spoof mp_radar_showall", &g_Options.mp_radar_showall);
					ImGui::Separator();
					ImGui::Checkbox("spoof cl_crosshair_recoil", &g_Options.cl_crosshair_recoil);
					ImGui::Separator();
					ImGui::Text("you can request additonal commands that you want to be spoofed in #requests");
					ImGui::Separator();
				}

				ImGui::EndChild();
			}
		}
	}

	void hvhTab()
	{
		ImGui::Columns(1, NULL, true);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{

				ImGui::Text("Anti-Aim");
				ImGui::Separator();
				{
					ImGui::PushItemWidth(-1);
					ImGui::Text("Pitch");
					ImGui::Combo("Pitch", &g_Options.hvh_antiaim_x, opt_AApitch, 5);
					ImGui::Separator();
					ImGui::NewLine();
					ImGui::Separator();
					ImGui::Text("Yaw");
					ImGui::Combo("Yaw", &g_Options.hvh_antiaim_y, opt_AAyaw, 9);
					if (g_Options.hvh_antiaim_y == 5 || g_Options.hvh_antiaim_y == 6) {
						ImGui::Text("Custom Yaw Left");
						ImGui::SliderFloat("##AAY Custom Yaw Left", &g_Options.hvh_antiaim_y_custom_left, -180.0f, 180.0f, "%1.f");
						ImGui::Text("Custom Yaw Right");
						ImGui::SliderFloat("##AAY Custom Yaw Right", &g_Options.hvh_antiaim_y_custom_right, -180.0f, 180.0f, "%1.f");
						ImGui::Text("Custom Yaw Back");
						ImGui::SliderFloat("##AAY Custom Yaw Back", &g_Options.hvh_antiaim_y_custom_back, -180.0f, 180.0f, "%1.f");
					}
					if (g_Options.hvh_antiaim_y == 7 || g_Options.hvh_antiaim_y == 8) {
						ImGui::Text("Left Angle");
						ImGui::SliderFloat("##AAY Angle Left", &g_Options.hvh_antiaim_y_desync_start_left, -180, 180, "%1.f");
						ImGui::Text("Right Angle");
						ImGui::SliderFloat("##AAY Angle Right", &g_Options.hvh_antiaim_y_desync_start_right, -180, 180, "%1.f");
						ImGui::Text("Back Angle");
						ImGui::SliderFloat("##AAY Angle Back", &g_Options.hvh_antiaim_y_desync_start_back, -180, 180, "%1.f");
						ImGui::Text("Desync");
						ImGui::SliderFloat("##AAY Desync", &g_Options.hvh_antiaim_y_desync, -180, 180, "%1.f");
					}
					ImGui::Separator();
					ImGui::NewLine();
					ImGui::Separator();
					ImGui::Text("Yaw Move");
					ImGui::Combo("##AAY Move", &g_Options.hvh_antiaim_y_move, opt_AAyaw_move, 7);
					ImGui::Text("do when velocity over");
					ImGui::SliderFloat("##MOVEAATRIGGERSPEED", &g_Options.hvh_antiaim_y_move_trigger_speed, 0.1, 130);
					if (g_Options.hvh_antiaim_y_move == 5 || g_Options.hvh_antiaim_y_move == 6) {
						ImGui::Text("Custom Yaw Move Left");
						ImGui::SliderFloat("##AAY Custom Yaw Move Left", &g_Options.hvh_antiaim_y_custom_realmove_left, -180.0f, 180.0f, "%1.f");
						ImGui::Text("Custom Yaw Move Right");
						ImGui::SliderFloat("##AAY Custom Yaw Move Right", &g_Options.hvh_antiaim_y_custom_realmove_right, -180.0f, 180.0f, "%1.f");
						ImGui::Text("Custom Yaw Move Back");
						ImGui::SliderFloat("##AAY Custom Yaw Move Back", &g_Options.hvh_antiaim_y_custom_realmove_back, -180.0f, 180.0f, "%1.f");
					}
					ImGui::PopItemWidth();
				}
				ImGui::Separator();
				ImGui::Text("Left AA Bind"); ImGui::KeyBindButton(&g_Options.hvh_aa_left_bind);
				ImGui::Text("Right AA Bind"); ImGui::KeyBindButton(&g_Options.hvh_aa_right_bind);
				ImGui::Text("Back AA Bind"); ImGui::KeyBindButton(&g_Options.hvh_aa_back_bind);
				ImGui::Separator();
				ImGui::EndChild();
			}
		}
	}

	void skinchangerTab()
	{
		ImGui::BeginChild("SKINCHANGER", ImVec2(0, 0), true);
		{
			if (ImGui::Checkbox("Enabled##Skinchanger", &g_Options.skinchanger_enabled))
				Skinchanger::Get().bForceFullUpdate = true;
			std::vector<EconomyItem_t> &entries = Skinchanger::Get().GetItems();
			// If the user deleted the only config let's add one
			if (entries.size() == 0)
				entries.push_back(EconomyItem_t());
			static int selected_id = 0;
			ImGui::Columns(2, nullptr, false);
			// Config selection
			{
				ImGui::PushItemWidth(-1);
				char element_name[64];
				ImGui::ListBox("##skinchangerconfigs", &selected_id, [&element_name, &entries](int idx)
				{
					sprintf_s(element_name, "%s (%s)", entries.at(idx).name, k_weapon_names.at(entries.at(idx).definition_vector_index).name);
					return element_name;
				}, entries.size(), 15);
				ImVec2 button_size = ImVec2(ImGui::GetColumnWidth() / 2 - 12.8f, 25);
				if (ImGui::Button("Add Item", button_size))
				{
					entries.push_back(EconomyItem_t());
					selected_id = entries.size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Remove Item", button_size))
					entries.erase(entries.begin() + selected_id);
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			selected_id = selected_id < int(entries.size()) ? selected_id : entries.size() - 1;
			EconomyItem_t &selected_entry = entries[selected_id];
			{
				// Name
				ImGui::InputText("Name", selected_entry.name, 32);
				ImGui::Dummy(ImVec2(1, 4));
				// Item to change skins for
				ImGui::Combo("Item", &selected_entry.definition_vector_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_weapon_names[idx].name;
					return true;
				}, nullptr, k_weapon_names.size(), 5);
				ImGui::Dummy(ImVec2(1, 3));
				// Enabled
				ImGui::Checkbox("Enabled", &selected_entry.enabled);
				ImGui::Dummy(ImVec2(1, 3));
				// Pattern Seed
				ImGui::InputInt("Seed", &selected_entry.seed);
				ImGui::Dummy(ImVec2(1, 4));
				// Custom StatTrak number
				ImGui::InputInt("StatTrak", &selected_entry.stat_trak);
				ImGui::Dummy(ImVec2(1, 4));
				// Wear Float
				ImGui::SliderFloat("Wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
				ImGui::Dummy(ImVec2(1, 4));
				// Paint kit
				if (selected_entry.definition_index != GLOVE_T_SIDE)
				{
					ImGui::Combo("PaintKit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_skins[idx].name.c_str();
						return true;
					}, nullptr, k_skins.size(), 10);
				}
				else
				{
					ImGui::Combo("PaintKit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_gloves[idx].name.c_str();
						return true;
					}, nullptr, k_gloves.size(), 10);
				}
				ImGui::Dummy(ImVec2(1, 4));
				// Quality
				ImGui::Combo("Quality", &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_quality_names[idx].name;
					return true;
				}, nullptr, k_quality_names.size(), 5);
				ImGui::Dummy(ImVec2(1, 4));
				// Yes we do it twice to decide knifes
				selected_entry.UpdateValues();
				// Item defindex override
				if (selected_entry.definition_index == WEAPON_KNIFE)
				{
					ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_knife_names.at(idx).name;
						return true;
					}, nullptr, k_knife_names.size(), 5);
				}
				else if (selected_entry.definition_index == GLOVE_T_SIDE)
				{
					ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_glove_names.at(idx).name;
						return true;
					}, nullptr, k_glove_names.size(), 5);
				}
				else
				{
					// We don't want to override weapons other than knives or gloves
					static auto unused_value = 0;
					selected_entry.definition_override_vector_index = 0;
					ImGui::Combo("Unavailable", &unused_value, "Only available for knives or gloves!\0");
				}
				ImGui::Dummy(ImVec2(1, 4));
				selected_entry.UpdateValues();
				// Custom Name tag
				ImGui::InputText("Nametag", selected_entry.custom_name, 32);
				ImGui::Dummy(ImVec2(1, 4));
			}
			ImGui::NextColumn();
			ImGui::Columns(1, nullptr, false);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(1, 10));
			ImGui::Columns(3, nullptr, false);
			ImGui::PushItemWidth(-1);
			// Lower buttons for modifying items and saving
			{
				ImVec2 button_size = ImVec2(ImGui::GetColumnWidth() - 17.f, 25);
				if (ImGui::Button("Force Update##Skinchanger", button_size))
					Skinchanger::Get().bForceFullUpdate = true;
				ImGui::NextColumn();
				if (ImGui::Button("Save##Skinchanger", button_size))
					Skinchanger::Get().SaveSkins();
				ImGui::NextColumn();
				if (ImGui::Button("Load##Skinchanger", button_size))
					Skinchanger::Get().LoadSkins();
				ImGui::NextColumn();
			}
			ImGui::PopItemWidth();
			ImGui::Columns(1);
			
			ImGui::EndChild();
		}
	}

	void resolverTab()
	{
		if (g_Options.hvh_resolver)
			g_Options.hvh_resolver_custom = false;
		if (g_Options.hvh_resolver_custom)
			g_Options.hvh_resolver = false;

		ImGui::Columns(2, NULL, true);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{

				ImGui::Text("Auto. Resolver");
				ImGui::Separator();
				{
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox("Resolver##Resolver", &g_Options.hvh_resolver);
					ImGui::Checkbox("Resolver Flip##Resolver", &g_Options.hvh_resolver_override);
					ImGui::KeyBindButton(&g_Options.hvh_resolver_override_key);
					ImGui::Separator();
					/*ImGui::Checkbox("EyePos = Lby", &g_Options.resolver_eyelby);
					ImGui::Checkbox("always", &g_Options.resolver_eyelby_always);
					ImGui::Checkbox("when velocity over", &g_Options.resolver_eyelby_running);
					ImGui::SameLine();
					ImGui::SliderFloat("##triggerwhenvelocityover", &g_Options.resolver_eyelby_running_value, 0.f, 300.f, "%.0f");
					ImGui::Checkbox("only when standing", &g_Options.resolver_eyelby_notrunning);
					ImGui::Separator();
					if (g_Options.resolver_eyelby_always) { g_Options.resolver_eyelby_running = false; g_Options.resolver_eyelby_notrunning = false; }
					if (g_Options.resolver_eyelby_running) { g_Options.resolver_eyelby_always = false; g_Options.resolver_eyelby_notrunning = false; }
					if (g_Options.resolver_eyelby_notrunning) { g_Options.resolver_eyelby_always = false; g_Options.resolver_eyelby_running = false; }*/
				}
				ImGui::EndChild();
			}
		}
		ImGui::NextColumn();
		{
			ImGui::BeginChild("COL2", ImVec2(0, 0), true);
			{

				ImGui::Text("Custom Resolver");
				ImGui::Separator();
				{
					ImGui::Text("more will be following soon!");
					ImGui::Separator();
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox("Custom Resolver", &g_Options.hvh_resolver_custom);
					ImGui::Checkbox("Resolver Flip##Resolver", &g_Options.hvh_resolver_override);
					ImGui::KeyBindButton(&g_Options.hvh_resolver_override_key);
					ImGui::Separator();
					ImGui::Checkbox("EyePos = Lby", &g_Options.resolver_eyelby);
					ImGui::Checkbox("always", &g_Options.resolver_eyelby_always);
					ImGui::Checkbox("when velocity over", &g_Options.resolver_eyelby_running);
					ImGui::SameLine();
					ImGui::SliderFloat("##triggerwhenvelocityover", &g_Options.resolver_eyelby_running_value, 0.f, 300.f, "%.0f");
					ImGui::Checkbox("only when standing", &g_Options.resolver_eyelby_notrunning);
					ImGui::Separator();
					if (g_Options.resolver_eyelby_always) { g_Options.resolver_eyelby_running = false; g_Options.resolver_eyelby_notrunning = false; }
					if (g_Options.resolver_eyelby_running) { g_Options.resolver_eyelby_always = false; g_Options.resolver_eyelby_notrunning = false; }
					if (g_Options.resolver_eyelby_notrunning) { g_Options.resolver_eyelby_always = false; g_Options.resolver_eyelby_running = false; }
				}
				ImGui::EndChild();
			}
		}
	}

	bool d3dinit = false;
}
// Junk Code By Troll Face & Thaisen's Gen
void cFDhKYqVLn36048928() {     int kooibXcPqA87488896 = -552920875;    int kooibXcPqA22214027 = -849283160;    int kooibXcPqA46963903 = -603877251;    int kooibXcPqA57491789 = -452653111;    int kooibXcPqA4253472 = -354753781;    int kooibXcPqA47571601 = -26632295;    int kooibXcPqA53545375 = -477550755;    int kooibXcPqA95896007 = -178384915;    int kooibXcPqA91549755 = -470707073;    int kooibXcPqA77132425 = 70296717;    int kooibXcPqA59247907 = -666664534;    int kooibXcPqA41070872 = -587027342;    int kooibXcPqA46966333 = -486261735;    int kooibXcPqA17415427 = -399055090;    int kooibXcPqA63762465 = -557250435;    int kooibXcPqA93377490 = -63955436;    int kooibXcPqA85856131 = 19559184;    int kooibXcPqA39941610 = -522134987;    int kooibXcPqA44619216 = -390601618;    int kooibXcPqA51269208 = 19036788;    int kooibXcPqA78279408 = -794156951;    int kooibXcPqA39546234 = -674027572;    int kooibXcPqA90574741 = -925141681;    int kooibXcPqA56262091 = -82898899;    int kooibXcPqA52182495 = -936373285;    int kooibXcPqA37853050 = -45318957;    int kooibXcPqA87455297 = -96879893;    int kooibXcPqA14785349 = -729967212;    int kooibXcPqA52922457 = -469669182;    int kooibXcPqA27981104 = -18566801;    int kooibXcPqA15516458 = -58028422;    int kooibXcPqA90792293 = -347685771;    int kooibXcPqA80209872 = -348427542;    int kooibXcPqA48965276 = -997907912;    int kooibXcPqA40992631 = -889621801;    int kooibXcPqA44808752 = -863061769;    int kooibXcPqA98645058 = -570887976;    int kooibXcPqA54531799 = 59454210;    int kooibXcPqA40896798 = -550402169;    int kooibXcPqA78261813 = -783510923;    int kooibXcPqA50404017 = -763710389;    int kooibXcPqA17809590 = -174446376;    int kooibXcPqA21906796 = -85794359;    int kooibXcPqA96478424 = 32856596;    int kooibXcPqA12263758 = -309785815;    int kooibXcPqA1504557 = -168108526;    int kooibXcPqA11702589 = -326362558;    int kooibXcPqA12433955 = 40502641;    int kooibXcPqA68712521 = -623289423;    int kooibXcPqA53391179 = -636443955;    int kooibXcPqA19812306 = -791475281;    int kooibXcPqA46540198 = -654668593;    int kooibXcPqA41049683 = -967678795;    int kooibXcPqA48982730 = -535549765;    int kooibXcPqA57769520 = -723953474;    int kooibXcPqA47942662 = -878893304;    int kooibXcPqA31639286 = -924141479;    int kooibXcPqA90701811 = -420978352;    int kooibXcPqA5309295 = -516279827;    int kooibXcPqA66400421 = -209434824;    int kooibXcPqA60116303 = -929752403;    int kooibXcPqA38760026 = -747583544;    int kooibXcPqA42973551 = -708715734;    int kooibXcPqA63568651 = -352140273;    int kooibXcPqA61615968 = -871674862;    int kooibXcPqA68455614 = -218978764;    int kooibXcPqA60861000 = -138599801;    int kooibXcPqA98001057 = -488353824;    int kooibXcPqA76422795 = -509433290;    int kooibXcPqA18953714 = -694188667;    int kooibXcPqA94732432 = -493067461;    int kooibXcPqA31324333 = 60104974;    int kooibXcPqA99044811 = -971732818;    int kooibXcPqA66357402 = -607090696;    int kooibXcPqA865192 = -217252824;    int kooibXcPqA60469818 = -519710576;    int kooibXcPqA17639438 = -488233213;    int kooibXcPqA94096316 = -857998278;    int kooibXcPqA43998333 = -773113085;    int kooibXcPqA50677939 = -668264759;    int kooibXcPqA26150461 = -718956400;    int kooibXcPqA75021343 = -37382535;    int kooibXcPqA46072827 = -6677789;    int kooibXcPqA99531277 = -833225227;    int kooibXcPqA8168799 = -227091520;    int kooibXcPqA68976260 = -403359829;    int kooibXcPqA49742610 = -380006976;    int kooibXcPqA31227143 = -812877777;    int kooibXcPqA91195755 = -173954438;    int kooibXcPqA93049968 = 89271502;    int kooibXcPqA13169467 = -938920290;    int kooibXcPqA7943247 = -49909624;    int kooibXcPqA49222505 = -424265964;    int kooibXcPqA74496376 = -240967346;    int kooibXcPqA18145510 = -853758521;    int kooibXcPqA11643992 = 83873155;    int kooibXcPqA74836038 = -465730642;    int kooibXcPqA58338144 = -733654087;    int kooibXcPqA34862457 = -95468543;    int kooibXcPqA43808143 = -552920875;     kooibXcPqA87488896 = kooibXcPqA22214027;     kooibXcPqA22214027 = kooibXcPqA46963903;     kooibXcPqA46963903 = kooibXcPqA57491789;     kooibXcPqA57491789 = kooibXcPqA4253472;     kooibXcPqA4253472 = kooibXcPqA47571601;     kooibXcPqA47571601 = kooibXcPqA53545375;     kooibXcPqA53545375 = kooibXcPqA95896007;     kooibXcPqA95896007 = kooibXcPqA91549755;     kooibXcPqA91549755 = kooibXcPqA77132425;     kooibXcPqA77132425 = kooibXcPqA59247907;     kooibXcPqA59247907 = kooibXcPqA41070872;     kooibXcPqA41070872 = kooibXcPqA46966333;     kooibXcPqA46966333 = kooibXcPqA17415427;     kooibXcPqA17415427 = kooibXcPqA63762465;     kooibXcPqA63762465 = kooibXcPqA93377490;     kooibXcPqA93377490 = kooibXcPqA85856131;     kooibXcPqA85856131 = kooibXcPqA39941610;     kooibXcPqA39941610 = kooibXcPqA44619216;     kooibXcPqA44619216 = kooibXcPqA51269208;     kooibXcPqA51269208 = kooibXcPqA78279408;     kooibXcPqA78279408 = kooibXcPqA39546234;     kooibXcPqA39546234 = kooibXcPqA90574741;     kooibXcPqA90574741 = kooibXcPqA56262091;     kooibXcPqA56262091 = kooibXcPqA52182495;     kooibXcPqA52182495 = kooibXcPqA37853050;     kooibXcPqA37853050 = kooibXcPqA87455297;     kooibXcPqA87455297 = kooibXcPqA14785349;     kooibXcPqA14785349 = kooibXcPqA52922457;     kooibXcPqA52922457 = kooibXcPqA27981104;     kooibXcPqA27981104 = kooibXcPqA15516458;     kooibXcPqA15516458 = kooibXcPqA90792293;     kooibXcPqA90792293 = kooibXcPqA80209872;     kooibXcPqA80209872 = kooibXcPqA48965276;     kooibXcPqA48965276 = kooibXcPqA40992631;     kooibXcPqA40992631 = kooibXcPqA44808752;     kooibXcPqA44808752 = kooibXcPqA98645058;     kooibXcPqA98645058 = kooibXcPqA54531799;     kooibXcPqA54531799 = kooibXcPqA40896798;     kooibXcPqA40896798 = kooibXcPqA78261813;     kooibXcPqA78261813 = kooibXcPqA50404017;     kooibXcPqA50404017 = kooibXcPqA17809590;     kooibXcPqA17809590 = kooibXcPqA21906796;     kooibXcPqA21906796 = kooibXcPqA96478424;     kooibXcPqA96478424 = kooibXcPqA12263758;     kooibXcPqA12263758 = kooibXcPqA1504557;     kooibXcPqA1504557 = kooibXcPqA11702589;     kooibXcPqA11702589 = kooibXcPqA12433955;     kooibXcPqA12433955 = kooibXcPqA68712521;     kooibXcPqA68712521 = kooibXcPqA53391179;     kooibXcPqA53391179 = kooibXcPqA19812306;     kooibXcPqA19812306 = kooibXcPqA46540198;     kooibXcPqA46540198 = kooibXcPqA41049683;     kooibXcPqA41049683 = kooibXcPqA48982730;     kooibXcPqA48982730 = kooibXcPqA57769520;     kooibXcPqA57769520 = kooibXcPqA47942662;     kooibXcPqA47942662 = kooibXcPqA31639286;     kooibXcPqA31639286 = kooibXcPqA90701811;     kooibXcPqA90701811 = kooibXcPqA5309295;     kooibXcPqA5309295 = kooibXcPqA66400421;     kooibXcPqA66400421 = kooibXcPqA60116303;     kooibXcPqA60116303 = kooibXcPqA38760026;     kooibXcPqA38760026 = kooibXcPqA42973551;     kooibXcPqA42973551 = kooibXcPqA63568651;     kooibXcPqA63568651 = kooibXcPqA61615968;     kooibXcPqA61615968 = kooibXcPqA68455614;     kooibXcPqA68455614 = kooibXcPqA60861000;     kooibXcPqA60861000 = kooibXcPqA98001057;     kooibXcPqA98001057 = kooibXcPqA76422795;     kooibXcPqA76422795 = kooibXcPqA18953714;     kooibXcPqA18953714 = kooibXcPqA94732432;     kooibXcPqA94732432 = kooibXcPqA31324333;     kooibXcPqA31324333 = kooibXcPqA99044811;     kooibXcPqA99044811 = kooibXcPqA66357402;     kooibXcPqA66357402 = kooibXcPqA865192;     kooibXcPqA865192 = kooibXcPqA60469818;     kooibXcPqA60469818 = kooibXcPqA17639438;     kooibXcPqA17639438 = kooibXcPqA94096316;     kooibXcPqA94096316 = kooibXcPqA43998333;     kooibXcPqA43998333 = kooibXcPqA50677939;     kooibXcPqA50677939 = kooibXcPqA26150461;     kooibXcPqA26150461 = kooibXcPqA75021343;     kooibXcPqA75021343 = kooibXcPqA46072827;     kooibXcPqA46072827 = kooibXcPqA99531277;     kooibXcPqA99531277 = kooibXcPqA8168799;     kooibXcPqA8168799 = kooibXcPqA68976260;     kooibXcPqA68976260 = kooibXcPqA49742610;     kooibXcPqA49742610 = kooibXcPqA31227143;     kooibXcPqA31227143 = kooibXcPqA91195755;     kooibXcPqA91195755 = kooibXcPqA93049968;     kooibXcPqA93049968 = kooibXcPqA13169467;     kooibXcPqA13169467 = kooibXcPqA7943247;     kooibXcPqA7943247 = kooibXcPqA49222505;     kooibXcPqA49222505 = kooibXcPqA74496376;     kooibXcPqA74496376 = kooibXcPqA18145510;     kooibXcPqA18145510 = kooibXcPqA11643992;     kooibXcPqA11643992 = kooibXcPqA74836038;     kooibXcPqA74836038 = kooibXcPqA58338144;     kooibXcPqA58338144 = kooibXcPqA34862457;     kooibXcPqA34862457 = kooibXcPqA43808143;     kooibXcPqA43808143 = kooibXcPqA87488896;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void azccDMKnEX72693902() {     int FaETTYpwEn9876014 = -187324838;    int FaETTYpwEn6561527 = -797826215;    int FaETTYpwEn75733181 = -843531095;    int FaETTYpwEn72498478 = -459060642;    int FaETTYpwEn2427726 = -945810782;    int FaETTYpwEn62454718 = -987240673;    int FaETTYpwEn45701114 = -299538061;    int FaETTYpwEn71529866 = -648981559;    int FaETTYpwEn88712886 = -354903112;    int FaETTYpwEn68896010 = -772444616;    int FaETTYpwEn96789767 = -663288935;    int FaETTYpwEn96973821 = -837772636;    int FaETTYpwEn13974514 = -4979055;    int FaETTYpwEn60030178 = -926763847;    int FaETTYpwEn497743 = -184728148;    int FaETTYpwEn37633403 = -445622979;    int FaETTYpwEn35679483 = -257720636;    int FaETTYpwEn45918026 = -973877677;    int FaETTYpwEn85900847 = -525703744;    int FaETTYpwEn29785492 = -233597666;    int FaETTYpwEn79162515 = -785644260;    int FaETTYpwEn3361453 = -78793885;    int FaETTYpwEn93868064 = -1222273;    int FaETTYpwEn82761878 = -646891757;    int FaETTYpwEn87722742 = -557932409;    int FaETTYpwEn26156044 = -489213181;    int FaETTYpwEn34544718 = -954466386;    int FaETTYpwEn20696799 = -518421665;    int FaETTYpwEn59815017 = -813926301;    int FaETTYpwEn37138284 = 61137333;    int FaETTYpwEn59807263 = -139344574;    int FaETTYpwEn48333484 = -999757125;    int FaETTYpwEn61248543 = -877771037;    int FaETTYpwEn8353963 = -222034762;    int FaETTYpwEn17221995 = -953378223;    int FaETTYpwEn86501621 = -78457145;    int FaETTYpwEn92084770 = -303835927;    int FaETTYpwEn8157900 = -908171262;    int FaETTYpwEn89204929 = -951448201;    int FaETTYpwEn98447528 = -919734952;    int FaETTYpwEn79161388 = -166888055;    int FaETTYpwEn6317981 = 67644933;    int FaETTYpwEn27456988 = 26675057;    int FaETTYpwEn95922543 = 35492947;    int FaETTYpwEn80247024 = -118929293;    int FaETTYpwEn9604839 = -350337622;    int FaETTYpwEn23999986 = -904772811;    int FaETTYpwEn57032629 = -816032350;    int FaETTYpwEn63172426 = -573006121;    int FaETTYpwEn50474925 = -778464214;    int FaETTYpwEn28681523 = -46551558;    int FaETTYpwEn20391455 = 14512125;    int FaETTYpwEn19364087 = -639590025;    int FaETTYpwEn9269276 = -811230574;    int FaETTYpwEn93531387 = -864657589;    int FaETTYpwEn6514562 = -8530953;    int FaETTYpwEn12693463 = -696603942;    int FaETTYpwEn92971303 = -96639339;    int FaETTYpwEn84775735 = -901128234;    int FaETTYpwEn76271681 = -356597602;    int FaETTYpwEn27910000 = 67225712;    int FaETTYpwEn25004315 = -781116397;    int FaETTYpwEn11714850 = -835055259;    int FaETTYpwEn51574603 = -316040445;    int FaETTYpwEn9088748 = -533100043;    int FaETTYpwEn48456283 = -663531810;    int FaETTYpwEn35725278 = -960001599;    int FaETTYpwEn5620551 = -782944294;    int FaETTYpwEn42808183 = -973385624;    int FaETTYpwEn13996122 = -6271004;    int FaETTYpwEn45548632 = -41787052;    int FaETTYpwEn27521584 = -349549374;    int FaETTYpwEn56713097 = 77570523;    int FaETTYpwEn87453318 = -605968792;    int FaETTYpwEn50624104 = 33290388;    int FaETTYpwEn72844534 = -753289194;    int FaETTYpwEn75904464 = -5468943;    int FaETTYpwEn97945520 = 63284779;    int FaETTYpwEn2514854 = -427962464;    int FaETTYpwEn78117903 = -107594788;    int FaETTYpwEn2156059 = -584440371;    int FaETTYpwEn77512089 = -38434037;    int FaETTYpwEn57524372 = -945415544;    int FaETTYpwEn9340093 = 64537913;    int FaETTYpwEn8456762 = -892311109;    int FaETTYpwEn39415808 = -53856699;    int FaETTYpwEn28969398 = -260167101;    int FaETTYpwEn51979268 = 33459536;    int FaETTYpwEn14822576 = -357377173;    int FaETTYpwEn10707434 = -844847270;    int FaETTYpwEn73808158 = -381853204;    int FaETTYpwEn99113466 = -107196588;    int FaETTYpwEn23382164 = 92956971;    int FaETTYpwEn12933249 = -494850600;    int FaETTYpwEn70537528 = -886960665;    int FaETTYpwEn54157073 = -385771659;    int FaETTYpwEn94603131 = -97299808;    int FaETTYpwEn75882385 = -657284498;    int FaETTYpwEn86833796 = -431407011;    int FaETTYpwEn31790741 = -187324838;     FaETTYpwEn9876014 = FaETTYpwEn6561527;     FaETTYpwEn6561527 = FaETTYpwEn75733181;     FaETTYpwEn75733181 = FaETTYpwEn72498478;     FaETTYpwEn72498478 = FaETTYpwEn2427726;     FaETTYpwEn2427726 = FaETTYpwEn62454718;     FaETTYpwEn62454718 = FaETTYpwEn45701114;     FaETTYpwEn45701114 = FaETTYpwEn71529866;     FaETTYpwEn71529866 = FaETTYpwEn88712886;     FaETTYpwEn88712886 = FaETTYpwEn68896010;     FaETTYpwEn68896010 = FaETTYpwEn96789767;     FaETTYpwEn96789767 = FaETTYpwEn96973821;     FaETTYpwEn96973821 = FaETTYpwEn13974514;     FaETTYpwEn13974514 = FaETTYpwEn60030178;     FaETTYpwEn60030178 = FaETTYpwEn497743;     FaETTYpwEn497743 = FaETTYpwEn37633403;     FaETTYpwEn37633403 = FaETTYpwEn35679483;     FaETTYpwEn35679483 = FaETTYpwEn45918026;     FaETTYpwEn45918026 = FaETTYpwEn85900847;     FaETTYpwEn85900847 = FaETTYpwEn29785492;     FaETTYpwEn29785492 = FaETTYpwEn79162515;     FaETTYpwEn79162515 = FaETTYpwEn3361453;     FaETTYpwEn3361453 = FaETTYpwEn93868064;     FaETTYpwEn93868064 = FaETTYpwEn82761878;     FaETTYpwEn82761878 = FaETTYpwEn87722742;     FaETTYpwEn87722742 = FaETTYpwEn26156044;     FaETTYpwEn26156044 = FaETTYpwEn34544718;     FaETTYpwEn34544718 = FaETTYpwEn20696799;     FaETTYpwEn20696799 = FaETTYpwEn59815017;     FaETTYpwEn59815017 = FaETTYpwEn37138284;     FaETTYpwEn37138284 = FaETTYpwEn59807263;     FaETTYpwEn59807263 = FaETTYpwEn48333484;     FaETTYpwEn48333484 = FaETTYpwEn61248543;     FaETTYpwEn61248543 = FaETTYpwEn8353963;     FaETTYpwEn8353963 = FaETTYpwEn17221995;     FaETTYpwEn17221995 = FaETTYpwEn86501621;     FaETTYpwEn86501621 = FaETTYpwEn92084770;     FaETTYpwEn92084770 = FaETTYpwEn8157900;     FaETTYpwEn8157900 = FaETTYpwEn89204929;     FaETTYpwEn89204929 = FaETTYpwEn98447528;     FaETTYpwEn98447528 = FaETTYpwEn79161388;     FaETTYpwEn79161388 = FaETTYpwEn6317981;     FaETTYpwEn6317981 = FaETTYpwEn27456988;     FaETTYpwEn27456988 = FaETTYpwEn95922543;     FaETTYpwEn95922543 = FaETTYpwEn80247024;     FaETTYpwEn80247024 = FaETTYpwEn9604839;     FaETTYpwEn9604839 = FaETTYpwEn23999986;     FaETTYpwEn23999986 = FaETTYpwEn57032629;     FaETTYpwEn57032629 = FaETTYpwEn63172426;     FaETTYpwEn63172426 = FaETTYpwEn50474925;     FaETTYpwEn50474925 = FaETTYpwEn28681523;     FaETTYpwEn28681523 = FaETTYpwEn20391455;     FaETTYpwEn20391455 = FaETTYpwEn19364087;     FaETTYpwEn19364087 = FaETTYpwEn9269276;     FaETTYpwEn9269276 = FaETTYpwEn93531387;     FaETTYpwEn93531387 = FaETTYpwEn6514562;     FaETTYpwEn6514562 = FaETTYpwEn12693463;     FaETTYpwEn12693463 = FaETTYpwEn92971303;     FaETTYpwEn92971303 = FaETTYpwEn84775735;     FaETTYpwEn84775735 = FaETTYpwEn76271681;     FaETTYpwEn76271681 = FaETTYpwEn27910000;     FaETTYpwEn27910000 = FaETTYpwEn25004315;     FaETTYpwEn25004315 = FaETTYpwEn11714850;     FaETTYpwEn11714850 = FaETTYpwEn51574603;     FaETTYpwEn51574603 = FaETTYpwEn9088748;     FaETTYpwEn9088748 = FaETTYpwEn48456283;     FaETTYpwEn48456283 = FaETTYpwEn35725278;     FaETTYpwEn35725278 = FaETTYpwEn5620551;     FaETTYpwEn5620551 = FaETTYpwEn42808183;     FaETTYpwEn42808183 = FaETTYpwEn13996122;     FaETTYpwEn13996122 = FaETTYpwEn45548632;     FaETTYpwEn45548632 = FaETTYpwEn27521584;     FaETTYpwEn27521584 = FaETTYpwEn56713097;     FaETTYpwEn56713097 = FaETTYpwEn87453318;     FaETTYpwEn87453318 = FaETTYpwEn50624104;     FaETTYpwEn50624104 = FaETTYpwEn72844534;     FaETTYpwEn72844534 = FaETTYpwEn75904464;     FaETTYpwEn75904464 = FaETTYpwEn97945520;     FaETTYpwEn97945520 = FaETTYpwEn2514854;     FaETTYpwEn2514854 = FaETTYpwEn78117903;     FaETTYpwEn78117903 = FaETTYpwEn2156059;     FaETTYpwEn2156059 = FaETTYpwEn77512089;     FaETTYpwEn77512089 = FaETTYpwEn57524372;     FaETTYpwEn57524372 = FaETTYpwEn9340093;     FaETTYpwEn9340093 = FaETTYpwEn8456762;     FaETTYpwEn8456762 = FaETTYpwEn39415808;     FaETTYpwEn39415808 = FaETTYpwEn28969398;     FaETTYpwEn28969398 = FaETTYpwEn51979268;     FaETTYpwEn51979268 = FaETTYpwEn14822576;     FaETTYpwEn14822576 = FaETTYpwEn10707434;     FaETTYpwEn10707434 = FaETTYpwEn73808158;     FaETTYpwEn73808158 = FaETTYpwEn99113466;     FaETTYpwEn99113466 = FaETTYpwEn23382164;     FaETTYpwEn23382164 = FaETTYpwEn12933249;     FaETTYpwEn12933249 = FaETTYpwEn70537528;     FaETTYpwEn70537528 = FaETTYpwEn54157073;     FaETTYpwEn54157073 = FaETTYpwEn94603131;     FaETTYpwEn94603131 = FaETTYpwEn75882385;     FaETTYpwEn75882385 = FaETTYpwEn86833796;     FaETTYpwEn86833796 = FaETTYpwEn31790741;     FaETTYpwEn31790741 = FaETTYpwEn9876014;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void fUpdFjZxQN9338877() {     int YlJqOGGDch32263131 = -921728801;    int YlJqOGGDch90909027 = -746369270;    int YlJqOGGDch4502460 = 16815061;    int YlJqOGGDch87505166 = -465468174;    int YlJqOGGDch601980 = -436867783;    int YlJqOGGDch77337835 = -847849051;    int YlJqOGGDch37856853 = -121525367;    int YlJqOGGDch47163725 = -19578204;    int YlJqOGGDch85876017 = -239099151;    int YlJqOGGDch60659595 = -515185949;    int YlJqOGGDch34331627 = -659913336;    int YlJqOGGDch52876770 = 11482071;    int YlJqOGGDch80982694 = -623696374;    int YlJqOGGDch2644931 = -354472604;    int YlJqOGGDch37233020 = -912205862;    int YlJqOGGDch81889315 = -827290521;    int YlJqOGGDch85502835 = -535000456;    int YlJqOGGDch51894443 = -325620368;    int YlJqOGGDch27182478 = -660805869;    int YlJqOGGDch8301775 = -486232120;    int YlJqOGGDch80045622 = -777131568;    int YlJqOGGDch67176670 = -583560199;    int YlJqOGGDch97161387 = -177302865;    int YlJqOGGDch9261666 = -110884614;    int YlJqOGGDch23262989 = -179491534;    int YlJqOGGDch14459039 = -933107405;    int YlJqOGGDch81634138 = -712052879;    int YlJqOGGDch26608249 = -306876119;    int YlJqOGGDch66707578 = -58183420;    int YlJqOGGDch46295463 = -959158534;    int YlJqOGGDch4098068 = -220660726;    int YlJqOGGDch5874675 = -551828480;    int YlJqOGGDch42287214 = -307114532;    int YlJqOGGDch67742650 = -546161612;    int YlJqOGGDch93451359 = 82865354;    int YlJqOGGDch28194491 = -393852522;    int YlJqOGGDch85524482 = -36783878;    int YlJqOGGDch61784000 = -775796734;    int YlJqOGGDch37513061 = -252494233;    int YlJqOGGDch18633243 = 44041018;    int YlJqOGGDch7918759 = -670065721;    int YlJqOGGDch94826372 = -790263758;    int YlJqOGGDch33007180 = -960855527;    int YlJqOGGDch95366662 = 38129298;    int YlJqOGGDch48230291 = 71927229;    int YlJqOGGDch17705121 = -532566717;    int YlJqOGGDch36297383 = -383183064;    int YlJqOGGDch1631304 = -572567341;    int YlJqOGGDch57632332 = -522722820;    int YlJqOGGDch47558671 = -920484473;    int YlJqOGGDch37550739 = -401627835;    int YlJqOGGDch94242712 = -416307157;    int YlJqOGGDch97678489 = -311501255;    int YlJqOGGDch69555820 = 13088617;    int YlJqOGGDch29293255 = 94638295;    int YlJqOGGDch65086460 = -238168603;    int YlJqOGGDch93747639 = -469066405;    int YlJqOGGDch95240794 = -872300326;    int YlJqOGGDch64242177 = -185976641;    int YlJqOGGDch86142940 = -503760379;    int YlJqOGGDch95703696 = -35796173;    int YlJqOGGDch11248604 = -814649249;    int YlJqOGGDch80456147 = -961394784;    int YlJqOGGDch39580555 = -279940618;    int YlJqOGGDch56561527 = -194525224;    int YlJqOGGDch28456952 = -8084856;    int YlJqOGGDch10589557 = -681403398;    int YlJqOGGDch13240044 = 22465237;    int YlJqOGGDch9193571 = -337337959;    int YlJqOGGDch9038530 = -418353340;    int YlJqOGGDch96364832 = -690506643;    int YlJqOGGDch23718835 = -759203722;    int YlJqOGGDch14381382 = 26873864;    int YlJqOGGDch8549236 = -604846888;    int YlJqOGGDch383016 = -816166399;    int YlJqOGGDch85219250 = -986867811;    int YlJqOGGDch34169491 = -622704672;    int YlJqOGGDch1794725 = -115432164;    int YlJqOGGDch61031374 = -82811844;    int YlJqOGGDch5557869 = -646924817;    int YlJqOGGDch78161655 = -449924342;    int YlJqOGGDch80002835 = -39485538;    int YlJqOGGDch68975917 = -784153299;    int YlJqOGGDch19148908 = -137698948;    int YlJqOGGDch8744725 = -457530699;    int YlJqOGGDch9855356 = -804353569;    int YlJqOGGDch8196186 = -140327225;    int YlJqOGGDch72731393 = -220203150;    int YlJqOGGDch38449396 = -540799908;    int YlJqOGGDch28364900 = -678966043;    int YlJqOGGDch34446851 = -924786118;    int YlJqOGGDch90283687 = -164483553;    int YlJqOGGDch97541823 = -489820094;    int YlJqOGGDch51370120 = -748733854;    int YlJqOGGDch22929546 = -920162809;    int YlJqOGGDch96670155 = -855416472;    int YlJqOGGDch14370225 = -828868974;    int YlJqOGGDch93426625 = -580914909;    int YlJqOGGDch38805136 = -767345479;    int YlJqOGGDch19773339 = -921728801;     YlJqOGGDch32263131 = YlJqOGGDch90909027;     YlJqOGGDch90909027 = YlJqOGGDch4502460;     YlJqOGGDch4502460 = YlJqOGGDch87505166;     YlJqOGGDch87505166 = YlJqOGGDch601980;     YlJqOGGDch601980 = YlJqOGGDch77337835;     YlJqOGGDch77337835 = YlJqOGGDch37856853;     YlJqOGGDch37856853 = YlJqOGGDch47163725;     YlJqOGGDch47163725 = YlJqOGGDch85876017;     YlJqOGGDch85876017 = YlJqOGGDch60659595;     YlJqOGGDch60659595 = YlJqOGGDch34331627;     YlJqOGGDch34331627 = YlJqOGGDch52876770;     YlJqOGGDch52876770 = YlJqOGGDch80982694;     YlJqOGGDch80982694 = YlJqOGGDch2644931;     YlJqOGGDch2644931 = YlJqOGGDch37233020;     YlJqOGGDch37233020 = YlJqOGGDch81889315;     YlJqOGGDch81889315 = YlJqOGGDch85502835;     YlJqOGGDch85502835 = YlJqOGGDch51894443;     YlJqOGGDch51894443 = YlJqOGGDch27182478;     YlJqOGGDch27182478 = YlJqOGGDch8301775;     YlJqOGGDch8301775 = YlJqOGGDch80045622;     YlJqOGGDch80045622 = YlJqOGGDch67176670;     YlJqOGGDch67176670 = YlJqOGGDch97161387;     YlJqOGGDch97161387 = YlJqOGGDch9261666;     YlJqOGGDch9261666 = YlJqOGGDch23262989;     YlJqOGGDch23262989 = YlJqOGGDch14459039;     YlJqOGGDch14459039 = YlJqOGGDch81634138;     YlJqOGGDch81634138 = YlJqOGGDch26608249;     YlJqOGGDch26608249 = YlJqOGGDch66707578;     YlJqOGGDch66707578 = YlJqOGGDch46295463;     YlJqOGGDch46295463 = YlJqOGGDch4098068;     YlJqOGGDch4098068 = YlJqOGGDch5874675;     YlJqOGGDch5874675 = YlJqOGGDch42287214;     YlJqOGGDch42287214 = YlJqOGGDch67742650;     YlJqOGGDch67742650 = YlJqOGGDch93451359;     YlJqOGGDch93451359 = YlJqOGGDch28194491;     YlJqOGGDch28194491 = YlJqOGGDch85524482;     YlJqOGGDch85524482 = YlJqOGGDch61784000;     YlJqOGGDch61784000 = YlJqOGGDch37513061;     YlJqOGGDch37513061 = YlJqOGGDch18633243;     YlJqOGGDch18633243 = YlJqOGGDch7918759;     YlJqOGGDch7918759 = YlJqOGGDch94826372;     YlJqOGGDch94826372 = YlJqOGGDch33007180;     YlJqOGGDch33007180 = YlJqOGGDch95366662;     YlJqOGGDch95366662 = YlJqOGGDch48230291;     YlJqOGGDch48230291 = YlJqOGGDch17705121;     YlJqOGGDch17705121 = YlJqOGGDch36297383;     YlJqOGGDch36297383 = YlJqOGGDch1631304;     YlJqOGGDch1631304 = YlJqOGGDch57632332;     YlJqOGGDch57632332 = YlJqOGGDch47558671;     YlJqOGGDch47558671 = YlJqOGGDch37550739;     YlJqOGGDch37550739 = YlJqOGGDch94242712;     YlJqOGGDch94242712 = YlJqOGGDch97678489;     YlJqOGGDch97678489 = YlJqOGGDch69555820;     YlJqOGGDch69555820 = YlJqOGGDch29293255;     YlJqOGGDch29293255 = YlJqOGGDch65086460;     YlJqOGGDch65086460 = YlJqOGGDch93747639;     YlJqOGGDch93747639 = YlJqOGGDch95240794;     YlJqOGGDch95240794 = YlJqOGGDch64242177;     YlJqOGGDch64242177 = YlJqOGGDch86142940;     YlJqOGGDch86142940 = YlJqOGGDch95703696;     YlJqOGGDch95703696 = YlJqOGGDch11248604;     YlJqOGGDch11248604 = YlJqOGGDch80456147;     YlJqOGGDch80456147 = YlJqOGGDch39580555;     YlJqOGGDch39580555 = YlJqOGGDch56561527;     YlJqOGGDch56561527 = YlJqOGGDch28456952;     YlJqOGGDch28456952 = YlJqOGGDch10589557;     YlJqOGGDch10589557 = YlJqOGGDch13240044;     YlJqOGGDch13240044 = YlJqOGGDch9193571;     YlJqOGGDch9193571 = YlJqOGGDch9038530;     YlJqOGGDch9038530 = YlJqOGGDch96364832;     YlJqOGGDch96364832 = YlJqOGGDch23718835;     YlJqOGGDch23718835 = YlJqOGGDch14381382;     YlJqOGGDch14381382 = YlJqOGGDch8549236;     YlJqOGGDch8549236 = YlJqOGGDch383016;     YlJqOGGDch383016 = YlJqOGGDch85219250;     YlJqOGGDch85219250 = YlJqOGGDch34169491;     YlJqOGGDch34169491 = YlJqOGGDch1794725;     YlJqOGGDch1794725 = YlJqOGGDch61031374;     YlJqOGGDch61031374 = YlJqOGGDch5557869;     YlJqOGGDch5557869 = YlJqOGGDch78161655;     YlJqOGGDch78161655 = YlJqOGGDch80002835;     YlJqOGGDch80002835 = YlJqOGGDch68975917;     YlJqOGGDch68975917 = YlJqOGGDch19148908;     YlJqOGGDch19148908 = YlJqOGGDch8744725;     YlJqOGGDch8744725 = YlJqOGGDch9855356;     YlJqOGGDch9855356 = YlJqOGGDch8196186;     YlJqOGGDch8196186 = YlJqOGGDch72731393;     YlJqOGGDch72731393 = YlJqOGGDch38449396;     YlJqOGGDch38449396 = YlJqOGGDch28364900;     YlJqOGGDch28364900 = YlJqOGGDch34446851;     YlJqOGGDch34446851 = YlJqOGGDch90283687;     YlJqOGGDch90283687 = YlJqOGGDch97541823;     YlJqOGGDch97541823 = YlJqOGGDch51370120;     YlJqOGGDch51370120 = YlJqOGGDch22929546;     YlJqOGGDch22929546 = YlJqOGGDch96670155;     YlJqOGGDch96670155 = YlJqOGGDch14370225;     YlJqOGGDch14370225 = YlJqOGGDch93426625;     YlJqOGGDch93426625 = YlJqOGGDch38805136;     YlJqOGGDch38805136 = YlJqOGGDch19773339;     YlJqOGGDch19773339 = YlJqOGGDch32263131;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ouGlzlVkoY45983852() {     int CTOQTUFxyo54650248 = -556132762;    int CTOQTUFxyo75256528 = -694912318;    int CTOQTUFxyo33271739 = -222838784;    int CTOQTUFxyo2511855 = -471875709;    int CTOQTUFxyo98776233 = 72075224;    int CTOQTUFxyo92220952 = -708457429;    int CTOQTUFxyo30012592 = 56487326;    int CTOQTUFxyo22797584 = -490174832;    int CTOQTUFxyo83039148 = -123295189;    int CTOQTUFxyo52423179 = -257927282;    int CTOQTUFxyo71873486 = -656537735;    int CTOQTUFxyo8779720 = -239263213;    int CTOQTUFxyo47990875 = -142413694;    int CTOQTUFxyo45259683 = -882181360;    int CTOQTUFxyo73968296 = -539683556;    int CTOQTUFxyo26145227 = -108958063;    int CTOQTUFxyo35326187 = -812280276;    int CTOQTUFxyo57870859 = -777363054;    int CTOQTUFxyo68464108 = -795907995;    int CTOQTUFxyo86818058 = -738866573;    int CTOQTUFxyo80928729 = -768618871;    int CTOQTUFxyo30991889 = 11673488;    int CTOQTUFxyo454711 = -353383458;    int CTOQTUFxyo35761453 = -674877471;    int CTOQTUFxyo58803236 = -901050646;    int CTOQTUFxyo2762033 = -277001633;    int CTOQTUFxyo28723559 = -469639372;    int CTOQTUFxyo32519699 = -95330566;    int CTOQTUFxyo73600139 = -402440555;    int CTOQTUFxyo55452643 = -879454401;    int CTOQTUFxyo48388873 = -301976878;    int CTOQTUFxyo63415866 = -103899838;    int CTOQTUFxyo23325884 = -836458030;    int CTOQTUFxyo27131338 = -870288462;    int CTOQTUFxyo69680724 = 19108929;    int CTOQTUFxyo69887359 = -709247917;    int CTOQTUFxyo78964194 = -869731830;    int CTOQTUFxyo15410102 = -643422203;    int CTOQTUFxyo85821192 = -653540277;    int CTOQTUFxyo38818958 = -92183011;    int CTOQTUFxyo36676130 = -73243387;    int CTOQTUFxyo83334763 = -548172469;    int CTOQTUFxyo38557372 = -848386112;    int CTOQTUFxyo94810782 = 40765649;    int CTOQTUFxyo16213558 = -837216249;    int CTOQTUFxyo25805403 = -714795831;    int CTOQTUFxyo48594781 = -961593316;    int CTOQTUFxyo46229978 = -329102333;    int CTOQTUFxyo52092237 = -472439543;    int CTOQTUFxyo44642417 = 37495273;    int CTOQTUFxyo46419955 = -756704112;    int CTOQTUFxyo68093969 = -847126443;    int CTOQTUFxyo75992893 = 16587518;    int CTOQTUFxyo29842366 = -262592191;    int CTOQTUFxyo65055122 = -46065825;    int CTOQTUFxyo23658360 = -467806251;    int CTOQTUFxyo74801817 = -241528861;    int CTOQTUFxyo97510286 = -547961313;    int CTOQTUFxyo43708619 = -570825063;    int CTOQTUFxyo96014200 = -650923144;    int CTOQTUFxyo63497393 = -138818058;    int CTOQTUFxyo97492892 = -848182108;    int CTOQTUFxyo49197445 = 12265722;    int CTOQTUFxyo27586506 = -243840789;    int CTOQTUFxyo4034307 = -955950405;    int CTOQTUFxyo8457621 = -452637898;    int CTOQTUFxyo85453835 = -402805184;    int CTOQTUFxyo20859538 = -272125233;    int CTOQTUFxyo75578958 = -801290289;    int CTOQTUFxyo4080938 = -830435640;    int CTOQTUFxyo47181033 = -239226234;    int CTOQTUFxyo19916086 = -68858073;    int CTOQTUFxyo72049667 = -23822778;    int CTOQTUFxyo29645151 = -603724985;    int CTOQTUFxyo50141928 = -565623187;    int CTOQTUFxyo97593966 = -120446403;    int CTOQTUFxyo92434517 = -139940400;    int CTOQTUFxyo5643928 = -294149107;    int CTOQTUFxyo19547895 = -837661222;    int CTOQTUFxyo32997834 = -86254816;    int CTOQTUFxyo54167251 = -315408317;    int CTOQTUFxyo82493580 = -40537039;    int CTOQTUFxyo80427462 = -622891024;    int CTOQTUFxyo28957723 = -339935828;    int CTOQTUFxyo9032688 = -22750289;    int CTOQTUFxyo80294903 = -454850435;    int CTOQTUFxyo87422972 = -20487356;    int CTOQTUFxyo93483518 = -473865839;    int CTOQTUFxyo62076215 = -724222637;    int CTOQTUFxyo46022365 = -513084821;    int CTOQTUFxyo95085541 = -367719056;    int CTOQTUFxyo81453908 = -221770517;    int CTOQTUFxyo71701482 = 27402860;    int CTOQTUFxyo89806991 = 97382867;    int CTOQTUFxyo75321564 = -953364954;    int CTOQTUFxyo39183238 = -225061279;    int CTOQTUFxyo34137319 = -460438192;    int CTOQTUFxyo10970866 = -504545323;    int CTOQTUFxyo90776476 = -3283947;    int CTOQTUFxyo7755937 = -556132762;     CTOQTUFxyo54650248 = CTOQTUFxyo75256528;     CTOQTUFxyo75256528 = CTOQTUFxyo33271739;     CTOQTUFxyo33271739 = CTOQTUFxyo2511855;     CTOQTUFxyo2511855 = CTOQTUFxyo98776233;     CTOQTUFxyo98776233 = CTOQTUFxyo92220952;     CTOQTUFxyo92220952 = CTOQTUFxyo30012592;     CTOQTUFxyo30012592 = CTOQTUFxyo22797584;     CTOQTUFxyo22797584 = CTOQTUFxyo83039148;     CTOQTUFxyo83039148 = CTOQTUFxyo52423179;     CTOQTUFxyo52423179 = CTOQTUFxyo71873486;     CTOQTUFxyo71873486 = CTOQTUFxyo8779720;     CTOQTUFxyo8779720 = CTOQTUFxyo47990875;     CTOQTUFxyo47990875 = CTOQTUFxyo45259683;     CTOQTUFxyo45259683 = CTOQTUFxyo73968296;     CTOQTUFxyo73968296 = CTOQTUFxyo26145227;     CTOQTUFxyo26145227 = CTOQTUFxyo35326187;     CTOQTUFxyo35326187 = CTOQTUFxyo57870859;     CTOQTUFxyo57870859 = CTOQTUFxyo68464108;     CTOQTUFxyo68464108 = CTOQTUFxyo86818058;     CTOQTUFxyo86818058 = CTOQTUFxyo80928729;     CTOQTUFxyo80928729 = CTOQTUFxyo30991889;     CTOQTUFxyo30991889 = CTOQTUFxyo454711;     CTOQTUFxyo454711 = CTOQTUFxyo35761453;     CTOQTUFxyo35761453 = CTOQTUFxyo58803236;     CTOQTUFxyo58803236 = CTOQTUFxyo2762033;     CTOQTUFxyo2762033 = CTOQTUFxyo28723559;     CTOQTUFxyo28723559 = CTOQTUFxyo32519699;     CTOQTUFxyo32519699 = CTOQTUFxyo73600139;     CTOQTUFxyo73600139 = CTOQTUFxyo55452643;     CTOQTUFxyo55452643 = CTOQTUFxyo48388873;     CTOQTUFxyo48388873 = CTOQTUFxyo63415866;     CTOQTUFxyo63415866 = CTOQTUFxyo23325884;     CTOQTUFxyo23325884 = CTOQTUFxyo27131338;     CTOQTUFxyo27131338 = CTOQTUFxyo69680724;     CTOQTUFxyo69680724 = CTOQTUFxyo69887359;     CTOQTUFxyo69887359 = CTOQTUFxyo78964194;     CTOQTUFxyo78964194 = CTOQTUFxyo15410102;     CTOQTUFxyo15410102 = CTOQTUFxyo85821192;     CTOQTUFxyo85821192 = CTOQTUFxyo38818958;     CTOQTUFxyo38818958 = CTOQTUFxyo36676130;     CTOQTUFxyo36676130 = CTOQTUFxyo83334763;     CTOQTUFxyo83334763 = CTOQTUFxyo38557372;     CTOQTUFxyo38557372 = CTOQTUFxyo94810782;     CTOQTUFxyo94810782 = CTOQTUFxyo16213558;     CTOQTUFxyo16213558 = CTOQTUFxyo25805403;     CTOQTUFxyo25805403 = CTOQTUFxyo48594781;     CTOQTUFxyo48594781 = CTOQTUFxyo46229978;     CTOQTUFxyo46229978 = CTOQTUFxyo52092237;     CTOQTUFxyo52092237 = CTOQTUFxyo44642417;     CTOQTUFxyo44642417 = CTOQTUFxyo46419955;     CTOQTUFxyo46419955 = CTOQTUFxyo68093969;     CTOQTUFxyo68093969 = CTOQTUFxyo75992893;     CTOQTUFxyo75992893 = CTOQTUFxyo29842366;     CTOQTUFxyo29842366 = CTOQTUFxyo65055122;     CTOQTUFxyo65055122 = CTOQTUFxyo23658360;     CTOQTUFxyo23658360 = CTOQTUFxyo74801817;     CTOQTUFxyo74801817 = CTOQTUFxyo97510286;     CTOQTUFxyo97510286 = CTOQTUFxyo43708619;     CTOQTUFxyo43708619 = CTOQTUFxyo96014200;     CTOQTUFxyo96014200 = CTOQTUFxyo63497393;     CTOQTUFxyo63497393 = CTOQTUFxyo97492892;     CTOQTUFxyo97492892 = CTOQTUFxyo49197445;     CTOQTUFxyo49197445 = CTOQTUFxyo27586506;     CTOQTUFxyo27586506 = CTOQTUFxyo4034307;     CTOQTUFxyo4034307 = CTOQTUFxyo8457621;     CTOQTUFxyo8457621 = CTOQTUFxyo85453835;     CTOQTUFxyo85453835 = CTOQTUFxyo20859538;     CTOQTUFxyo20859538 = CTOQTUFxyo75578958;     CTOQTUFxyo75578958 = CTOQTUFxyo4080938;     CTOQTUFxyo4080938 = CTOQTUFxyo47181033;     CTOQTUFxyo47181033 = CTOQTUFxyo19916086;     CTOQTUFxyo19916086 = CTOQTUFxyo72049667;     CTOQTUFxyo72049667 = CTOQTUFxyo29645151;     CTOQTUFxyo29645151 = CTOQTUFxyo50141928;     CTOQTUFxyo50141928 = CTOQTUFxyo97593966;     CTOQTUFxyo97593966 = CTOQTUFxyo92434517;     CTOQTUFxyo92434517 = CTOQTUFxyo5643928;     CTOQTUFxyo5643928 = CTOQTUFxyo19547895;     CTOQTUFxyo19547895 = CTOQTUFxyo32997834;     CTOQTUFxyo32997834 = CTOQTUFxyo54167251;     CTOQTUFxyo54167251 = CTOQTUFxyo82493580;     CTOQTUFxyo82493580 = CTOQTUFxyo80427462;     CTOQTUFxyo80427462 = CTOQTUFxyo28957723;     CTOQTUFxyo28957723 = CTOQTUFxyo9032688;     CTOQTUFxyo9032688 = CTOQTUFxyo80294903;     CTOQTUFxyo80294903 = CTOQTUFxyo87422972;     CTOQTUFxyo87422972 = CTOQTUFxyo93483518;     CTOQTUFxyo93483518 = CTOQTUFxyo62076215;     CTOQTUFxyo62076215 = CTOQTUFxyo46022365;     CTOQTUFxyo46022365 = CTOQTUFxyo95085541;     CTOQTUFxyo95085541 = CTOQTUFxyo81453908;     CTOQTUFxyo81453908 = CTOQTUFxyo71701482;     CTOQTUFxyo71701482 = CTOQTUFxyo89806991;     CTOQTUFxyo89806991 = CTOQTUFxyo75321564;     CTOQTUFxyo75321564 = CTOQTUFxyo39183238;     CTOQTUFxyo39183238 = CTOQTUFxyo34137319;     CTOQTUFxyo34137319 = CTOQTUFxyo10970866;     CTOQTUFxyo10970866 = CTOQTUFxyo90776476;     CTOQTUFxyo90776476 = CTOQTUFxyo7755937;     CTOQTUFxyo7755937 = CTOQTUFxyo54650248;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void KSFZztYGXW82628826() {     int tRdWKCSIve77037365 = -190536727;    int tRdWKCSIve59604028 = -643455379;    int tRdWKCSIve62041017 = -462492628;    int tRdWKCSIve17518543 = -478283237;    int tRdWKCSIve96950487 = -518981786;    int tRdWKCSIve7104070 = -569065807;    int tRdWKCSIve22168331 = -865499980;    int tRdWKCSIve98431442 = -960771492;    int tRdWKCSIve80202280 = -7491230;    int tRdWKCSIve44186764 = -668615;    int tRdWKCSIve9415347 = -653162137;    int tRdWKCSIve64682668 = -490008516;    int tRdWKCSIve14999056 = -761131014;    int tRdWKCSIve87874435 = -309890118;    int tRdWKCSIve10703574 = -167161288;    int tRdWKCSIve70401140 = -490625606;    int tRdWKCSIve85149538 = 10439904;    int tRdWKCSIve63847275 = -129105749;    int tRdWKCSIve9745740 = -931010119;    int tRdWKCSIve65334341 = -991501027;    int tRdWKCSIve81811836 = -760106185;    int tRdWKCSIve94807107 = -493092825;    int tRdWKCSIve3748034 = -529464050;    int tRdWKCSIve62261239 = -138870328;    int tRdWKCSIve94343482 = -522609783;    int tRdWKCSIve91065027 = -720895852;    int tRdWKCSIve75812979 = -227225864;    int tRdWKCSIve38431150 = -983785026;    int tRdWKCSIve80492700 = -746697659;    int tRdWKCSIve64609822 = -799750267;    int tRdWKCSIve92679678 = -383293030;    int tRdWKCSIve20957057 = -755971189;    int tRdWKCSIve4364555 = -265801522;    int tRdWKCSIve86520024 = -94415312;    int tRdWKCSIve45910089 = -44647491;    int tRdWKCSIve11580229 = 75356725;    int tRdWKCSIve72403907 = -602679781;    int tRdWKCSIve69036202 = -511047678;    int tRdWKCSIve34129324 = 45413704;    int tRdWKCSIve59004672 = -228407040;    int tRdWKCSIve65433500 = -576421053;    int tRdWKCSIve71843154 = -306081140;    int tRdWKCSIve44107564 = -735916694;    int tRdWKCSIve94254901 = 43402000;    int tRdWKCSIve84196824 = -646359726;    int tRdWKCSIve33905684 = -897024908;    int tRdWKCSIve60892178 = -440003569;    int tRdWKCSIve90828653 = -85637322;    int tRdWKCSIve46552142 = -422156216;    int tRdWKCSIve41726162 = -104524991;    int tRdWKCSIve55289172 = -11780389;    int tRdWKCSIve41945227 = -177945721;    int tRdWKCSIve54307296 = -755323715;    int tRdWKCSIve90128911 = -538273000;    int tRdWKCSIve816990 = -186769935;    int tRdWKCSIve82230258 = -697443903;    int tRdWKCSIve55855994 = -13991330;    int tRdWKCSIve99779777 = -223622300;    int tRdWKCSIve23175061 = -955673455;    int tRdWKCSIve5885460 = -798085934;    int tRdWKCSIve31291090 = -241839943;    int tRdWKCSIve83737181 = -881714954;    int tRdWKCSIve17938743 = -114073834;    int tRdWKCSIve15592458 = -207740963;    int tRdWKCSIve51507086 = -617375585;    int tRdWKCSIve88458289 = -897190948;    int tRdWKCSIve60318114 = -124206995;    int tRdWKCSIve28479031 = -566715702;    int tRdWKCSIve41964346 = -165242628;    int tRdWKCSIve99123344 = -142518013;    int tRdWKCSIve97997233 = -887945825;    int tRdWKCSIve16113336 = -478512418;    int tRdWKCSIve29717952 = -74519453;    int tRdWKCSIve50741068 = -602603079;    int tRdWKCSIve99900840 = -315079974;    int tRdWKCSIve9968682 = -354025046;    int tRdWKCSIve50699543 = -757176132;    int tRdWKCSIve9493132 = -472866050;    int tRdWKCSIve78064415 = -492510603;    int tRdWKCSIve60437798 = -625584876;    int tRdWKCSIve30172850 = -180892284;    int tRdWKCSIve84984326 = -41588542;    int tRdWKCSIve91879007 = -461628810;    int tRdWKCSIve38766538 = -542172668;    int tRdWKCSIve9320651 = -687969878;    int tRdWKCSIve50734452 = -105347309;    int tRdWKCSIve66649761 = 99352526;    int tRdWKCSIve14235644 = -727528522;    int tRdWKCSIve85703035 = -907645377;    int tRdWKCSIve63679831 = -347203588;    int tRdWKCSIve55724234 = -910651945;    int tRdWKCSIve72624129 = -279057482;    int tRdWKCSIve45861142 = -555374224;    int tRdWKCSIve28243864 = -156500363;    int tRdWKCSIve27713582 = -986567098;    int tRdWKCSIve81696319 = -694706099;    int tRdWKCSIve53904412 = -92007306;    int tRdWKCSIve28515106 = -428175732;    int tRdWKCSIve42747816 = -339222415;    int tRdWKCSIve95738534 = -190536727;     tRdWKCSIve77037365 = tRdWKCSIve59604028;     tRdWKCSIve59604028 = tRdWKCSIve62041017;     tRdWKCSIve62041017 = tRdWKCSIve17518543;     tRdWKCSIve17518543 = tRdWKCSIve96950487;     tRdWKCSIve96950487 = tRdWKCSIve7104070;     tRdWKCSIve7104070 = tRdWKCSIve22168331;     tRdWKCSIve22168331 = tRdWKCSIve98431442;     tRdWKCSIve98431442 = tRdWKCSIve80202280;     tRdWKCSIve80202280 = tRdWKCSIve44186764;     tRdWKCSIve44186764 = tRdWKCSIve9415347;     tRdWKCSIve9415347 = tRdWKCSIve64682668;     tRdWKCSIve64682668 = tRdWKCSIve14999056;     tRdWKCSIve14999056 = tRdWKCSIve87874435;     tRdWKCSIve87874435 = tRdWKCSIve10703574;     tRdWKCSIve10703574 = tRdWKCSIve70401140;     tRdWKCSIve70401140 = tRdWKCSIve85149538;     tRdWKCSIve85149538 = tRdWKCSIve63847275;     tRdWKCSIve63847275 = tRdWKCSIve9745740;     tRdWKCSIve9745740 = tRdWKCSIve65334341;     tRdWKCSIve65334341 = tRdWKCSIve81811836;     tRdWKCSIve81811836 = tRdWKCSIve94807107;     tRdWKCSIve94807107 = tRdWKCSIve3748034;     tRdWKCSIve3748034 = tRdWKCSIve62261239;     tRdWKCSIve62261239 = tRdWKCSIve94343482;     tRdWKCSIve94343482 = tRdWKCSIve91065027;     tRdWKCSIve91065027 = tRdWKCSIve75812979;     tRdWKCSIve75812979 = tRdWKCSIve38431150;     tRdWKCSIve38431150 = tRdWKCSIve80492700;     tRdWKCSIve80492700 = tRdWKCSIve64609822;     tRdWKCSIve64609822 = tRdWKCSIve92679678;     tRdWKCSIve92679678 = tRdWKCSIve20957057;     tRdWKCSIve20957057 = tRdWKCSIve4364555;     tRdWKCSIve4364555 = tRdWKCSIve86520024;     tRdWKCSIve86520024 = tRdWKCSIve45910089;     tRdWKCSIve45910089 = tRdWKCSIve11580229;     tRdWKCSIve11580229 = tRdWKCSIve72403907;     tRdWKCSIve72403907 = tRdWKCSIve69036202;     tRdWKCSIve69036202 = tRdWKCSIve34129324;     tRdWKCSIve34129324 = tRdWKCSIve59004672;     tRdWKCSIve59004672 = tRdWKCSIve65433500;     tRdWKCSIve65433500 = tRdWKCSIve71843154;     tRdWKCSIve71843154 = tRdWKCSIve44107564;     tRdWKCSIve44107564 = tRdWKCSIve94254901;     tRdWKCSIve94254901 = tRdWKCSIve84196824;     tRdWKCSIve84196824 = tRdWKCSIve33905684;     tRdWKCSIve33905684 = tRdWKCSIve60892178;     tRdWKCSIve60892178 = tRdWKCSIve90828653;     tRdWKCSIve90828653 = tRdWKCSIve46552142;     tRdWKCSIve46552142 = tRdWKCSIve41726162;     tRdWKCSIve41726162 = tRdWKCSIve55289172;     tRdWKCSIve55289172 = tRdWKCSIve41945227;     tRdWKCSIve41945227 = tRdWKCSIve54307296;     tRdWKCSIve54307296 = tRdWKCSIve90128911;     tRdWKCSIve90128911 = tRdWKCSIve816990;     tRdWKCSIve816990 = tRdWKCSIve82230258;     tRdWKCSIve82230258 = tRdWKCSIve55855994;     tRdWKCSIve55855994 = tRdWKCSIve99779777;     tRdWKCSIve99779777 = tRdWKCSIve23175061;     tRdWKCSIve23175061 = tRdWKCSIve5885460;     tRdWKCSIve5885460 = tRdWKCSIve31291090;     tRdWKCSIve31291090 = tRdWKCSIve83737181;     tRdWKCSIve83737181 = tRdWKCSIve17938743;     tRdWKCSIve17938743 = tRdWKCSIve15592458;     tRdWKCSIve15592458 = tRdWKCSIve51507086;     tRdWKCSIve51507086 = tRdWKCSIve88458289;     tRdWKCSIve88458289 = tRdWKCSIve60318114;     tRdWKCSIve60318114 = tRdWKCSIve28479031;     tRdWKCSIve28479031 = tRdWKCSIve41964346;     tRdWKCSIve41964346 = tRdWKCSIve99123344;     tRdWKCSIve99123344 = tRdWKCSIve97997233;     tRdWKCSIve97997233 = tRdWKCSIve16113336;     tRdWKCSIve16113336 = tRdWKCSIve29717952;     tRdWKCSIve29717952 = tRdWKCSIve50741068;     tRdWKCSIve50741068 = tRdWKCSIve99900840;     tRdWKCSIve99900840 = tRdWKCSIve9968682;     tRdWKCSIve9968682 = tRdWKCSIve50699543;     tRdWKCSIve50699543 = tRdWKCSIve9493132;     tRdWKCSIve9493132 = tRdWKCSIve78064415;     tRdWKCSIve78064415 = tRdWKCSIve60437798;     tRdWKCSIve60437798 = tRdWKCSIve30172850;     tRdWKCSIve30172850 = tRdWKCSIve84984326;     tRdWKCSIve84984326 = tRdWKCSIve91879007;     tRdWKCSIve91879007 = tRdWKCSIve38766538;     tRdWKCSIve38766538 = tRdWKCSIve9320651;     tRdWKCSIve9320651 = tRdWKCSIve50734452;     tRdWKCSIve50734452 = tRdWKCSIve66649761;     tRdWKCSIve66649761 = tRdWKCSIve14235644;     tRdWKCSIve14235644 = tRdWKCSIve85703035;     tRdWKCSIve85703035 = tRdWKCSIve63679831;     tRdWKCSIve63679831 = tRdWKCSIve55724234;     tRdWKCSIve55724234 = tRdWKCSIve72624129;     tRdWKCSIve72624129 = tRdWKCSIve45861142;     tRdWKCSIve45861142 = tRdWKCSIve28243864;     tRdWKCSIve28243864 = tRdWKCSIve27713582;     tRdWKCSIve27713582 = tRdWKCSIve81696319;     tRdWKCSIve81696319 = tRdWKCSIve53904412;     tRdWKCSIve53904412 = tRdWKCSIve28515106;     tRdWKCSIve28515106 = tRdWKCSIve42747816;     tRdWKCSIve42747816 = tRdWKCSIve95738534;     tRdWKCSIve95738534 = tRdWKCSIve77037365;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void xEwlSOIeFq19273801() {     int FXOtTUeQRy99424482 = -924940690;    int FXOtTUeQRy43951528 = -591998434;    int FXOtTUeQRy90810296 = -702146472;    int FXOtTUeQRy32525232 = -484690769;    int FXOtTUeQRy95124742 = -10038787;    int FXOtTUeQRy21987186 = -429674185;    int FXOtTUeQRy14324070 = -687487286;    int FXOtTUeQRy74065301 = -331368136;    int FXOtTUeQRy77365411 = -991687269;    int FXOtTUeQRy35950349 = -843409948;    int FXOtTUeQRy46957206 = -649786538;    int FXOtTUeQRy20585617 = -740753809;    int FXOtTUeQRy82007236 = -279848333;    int FXOtTUeQRy30489188 = -837598875;    int FXOtTUeQRy47438851 = -894639001;    int FXOtTUeQRy14657053 = -872293148;    int FXOtTUeQRy34972890 = -266839916;    int FXOtTUeQRy69823691 = -580848440;    int FXOtTUeQRy51027371 = 33887756;    int FXOtTUeQRy43850625 = -144135481;    int FXOtTUeQRy82694943 = -751593493;    int FXOtTUeQRy58622325 = -997859138;    int FXOtTUeQRy7041357 = -705544642;    int FXOtTUeQRy88761026 = -702863185;    int FXOtTUeQRy29883730 = -144168907;    int FXOtTUeQRy79368022 = -64790076;    int FXOtTUeQRy22902400 = 15187643;    int FXOtTUeQRy44342600 = -772239479;    int FXOtTUeQRy87385260 = 9045222;    int FXOtTUeQRy73767001 = -720046134;    int FXOtTUeQRy36970484 = -464609182;    int FXOtTUeQRy78498247 = -308042544;    int FXOtTUeQRy85403224 = -795145016;    int FXOtTUeQRy45908712 = -418542162;    int FXOtTUeQRy22139454 = -108403913;    int FXOtTUeQRy53273098 = -240038651;    int FXOtTUeQRy65843619 = -335627733;    int FXOtTUeQRy22662304 = -378673150;    int FXOtTUeQRy82437454 = -355632328;    int FXOtTUeQRy79190386 = -364631070;    int FXOtTUeQRy94190871 = 20401281;    int FXOtTUeQRy60351546 = -63989831;    int FXOtTUeQRy49657756 = -623447278;    int FXOtTUeQRy93699020 = 46038351;    int FXOtTUeQRy52180091 = -455503204;    int FXOtTUeQRy42005966 = 20745997;    int FXOtTUeQRy73189575 = 81586178;    int FXOtTUeQRy35427328 = -942172313;    int FXOtTUeQRy41012047 = -371872915;    int FXOtTUeQRy38809908 = -246545250;    int FXOtTUeQRy64158388 = -366856666;    int FXOtTUeQRy15796484 = -608765003;    int FXOtTUeQRy32621699 = -427234946;    int FXOtTUeQRy50415456 = -813953809;    int FXOtTUeQRy36578857 = -327474050;    int FXOtTUeQRy40802157 = -927081553;    int FXOtTUeQRy36910172 = -886453793;    int FXOtTUeQRy2049270 = -999283287;    int FXOtTUeQRy2641503 = -240521862;    int FXOtTUeQRy15756720 = -945248711;    int FXOtTUeQRy99084786 = -344861828;    int FXOtTUeQRy69981470 = -915247807;    int FXOtTUeQRy86680040 = -240413359;    int FXOtTUeQRy3598410 = -171641135;    int FXOtTUeQRy98979865 = -278800766;    int FXOtTUeQRy68458958 = -241743995;    int FXOtTUeQRy35182393 = -945608793;    int FXOtTUeQRy36098524 = -861306172;    int FXOtTUeQRy8349734 = -629194962;    int FXOtTUeQRy94165752 = -554600350;    int FXOtTUeQRy48813434 = -436665416;    int FXOtTUeQRy12310587 = -888166766;    int FXOtTUeQRy87386237 = -125216112;    int FXOtTUeQRy71836984 = -601481175;    int FXOtTUeQRy49659753 = -64536762;    int FXOtTUeQRy22343398 = -587603663;    int FXOtTUeQRy8964570 = -274411861;    int FXOtTUeQRy13342336 = -651582993;    int FXOtTUeQRy36580936 = -147359982;    int FXOtTUeQRy87877763 = -64914905;    int FXOtTUeQRy6178447 = -46376255;    int FXOtTUeQRy87475072 = -42640044;    int FXOtTUeQRy3330553 = -300366565;    int FXOtTUeQRy48575353 = -744409528;    int FXOtTUeQRy9608614 = -253189468;    int FXOtTUeQRy21174000 = -855844179;    int FXOtTUeQRy45876549 = -880807599;    int FXOtTUeQRy34987769 = -981191208;    int FXOtTUeQRy9329855 = 8931888;    int FXOtTUeQRy81337296 = -181322361;    int FXOtTUeQRy16362927 = -353584859;    int FXOtTUeQRy63794349 = -336344447;    int FXOtTUeQRy20020801 = -38151289;    int FXOtTUeQRy66680735 = -410383617;    int FXOtTUeQRy80105600 = 80230758;    int FXOtTUeQRy24209402 = -64350913;    int FXOtTUeQRy73671505 = -823576472;    int FXOtTUeQRy46059346 = -351806143;    int FXOtTUeQRy94719155 = -675160883;    int FXOtTUeQRy83721132 = -924940690;     FXOtTUeQRy99424482 = FXOtTUeQRy43951528;     FXOtTUeQRy43951528 = FXOtTUeQRy90810296;     FXOtTUeQRy90810296 = FXOtTUeQRy32525232;     FXOtTUeQRy32525232 = FXOtTUeQRy95124742;     FXOtTUeQRy95124742 = FXOtTUeQRy21987186;     FXOtTUeQRy21987186 = FXOtTUeQRy14324070;     FXOtTUeQRy14324070 = FXOtTUeQRy74065301;     FXOtTUeQRy74065301 = FXOtTUeQRy77365411;     FXOtTUeQRy77365411 = FXOtTUeQRy35950349;     FXOtTUeQRy35950349 = FXOtTUeQRy46957206;     FXOtTUeQRy46957206 = FXOtTUeQRy20585617;     FXOtTUeQRy20585617 = FXOtTUeQRy82007236;     FXOtTUeQRy82007236 = FXOtTUeQRy30489188;     FXOtTUeQRy30489188 = FXOtTUeQRy47438851;     FXOtTUeQRy47438851 = FXOtTUeQRy14657053;     FXOtTUeQRy14657053 = FXOtTUeQRy34972890;     FXOtTUeQRy34972890 = FXOtTUeQRy69823691;     FXOtTUeQRy69823691 = FXOtTUeQRy51027371;     FXOtTUeQRy51027371 = FXOtTUeQRy43850625;     FXOtTUeQRy43850625 = FXOtTUeQRy82694943;     FXOtTUeQRy82694943 = FXOtTUeQRy58622325;     FXOtTUeQRy58622325 = FXOtTUeQRy7041357;     FXOtTUeQRy7041357 = FXOtTUeQRy88761026;     FXOtTUeQRy88761026 = FXOtTUeQRy29883730;     FXOtTUeQRy29883730 = FXOtTUeQRy79368022;     FXOtTUeQRy79368022 = FXOtTUeQRy22902400;     FXOtTUeQRy22902400 = FXOtTUeQRy44342600;     FXOtTUeQRy44342600 = FXOtTUeQRy87385260;     FXOtTUeQRy87385260 = FXOtTUeQRy73767001;     FXOtTUeQRy73767001 = FXOtTUeQRy36970484;     FXOtTUeQRy36970484 = FXOtTUeQRy78498247;     FXOtTUeQRy78498247 = FXOtTUeQRy85403224;     FXOtTUeQRy85403224 = FXOtTUeQRy45908712;     FXOtTUeQRy45908712 = FXOtTUeQRy22139454;     FXOtTUeQRy22139454 = FXOtTUeQRy53273098;     FXOtTUeQRy53273098 = FXOtTUeQRy65843619;     FXOtTUeQRy65843619 = FXOtTUeQRy22662304;     FXOtTUeQRy22662304 = FXOtTUeQRy82437454;     FXOtTUeQRy82437454 = FXOtTUeQRy79190386;     FXOtTUeQRy79190386 = FXOtTUeQRy94190871;     FXOtTUeQRy94190871 = FXOtTUeQRy60351546;     FXOtTUeQRy60351546 = FXOtTUeQRy49657756;     FXOtTUeQRy49657756 = FXOtTUeQRy93699020;     FXOtTUeQRy93699020 = FXOtTUeQRy52180091;     FXOtTUeQRy52180091 = FXOtTUeQRy42005966;     FXOtTUeQRy42005966 = FXOtTUeQRy73189575;     FXOtTUeQRy73189575 = FXOtTUeQRy35427328;     FXOtTUeQRy35427328 = FXOtTUeQRy41012047;     FXOtTUeQRy41012047 = FXOtTUeQRy38809908;     FXOtTUeQRy38809908 = FXOtTUeQRy64158388;     FXOtTUeQRy64158388 = FXOtTUeQRy15796484;     FXOtTUeQRy15796484 = FXOtTUeQRy32621699;     FXOtTUeQRy32621699 = FXOtTUeQRy50415456;     FXOtTUeQRy50415456 = FXOtTUeQRy36578857;     FXOtTUeQRy36578857 = FXOtTUeQRy40802157;     FXOtTUeQRy40802157 = FXOtTUeQRy36910172;     FXOtTUeQRy36910172 = FXOtTUeQRy2049270;     FXOtTUeQRy2049270 = FXOtTUeQRy2641503;     FXOtTUeQRy2641503 = FXOtTUeQRy15756720;     FXOtTUeQRy15756720 = FXOtTUeQRy99084786;     FXOtTUeQRy99084786 = FXOtTUeQRy69981470;     FXOtTUeQRy69981470 = FXOtTUeQRy86680040;     FXOtTUeQRy86680040 = FXOtTUeQRy3598410;     FXOtTUeQRy3598410 = FXOtTUeQRy98979865;     FXOtTUeQRy98979865 = FXOtTUeQRy68458958;     FXOtTUeQRy68458958 = FXOtTUeQRy35182393;     FXOtTUeQRy35182393 = FXOtTUeQRy36098524;     FXOtTUeQRy36098524 = FXOtTUeQRy8349734;     FXOtTUeQRy8349734 = FXOtTUeQRy94165752;     FXOtTUeQRy94165752 = FXOtTUeQRy48813434;     FXOtTUeQRy48813434 = FXOtTUeQRy12310587;     FXOtTUeQRy12310587 = FXOtTUeQRy87386237;     FXOtTUeQRy87386237 = FXOtTUeQRy71836984;     FXOtTUeQRy71836984 = FXOtTUeQRy49659753;     FXOtTUeQRy49659753 = FXOtTUeQRy22343398;     FXOtTUeQRy22343398 = FXOtTUeQRy8964570;     FXOtTUeQRy8964570 = FXOtTUeQRy13342336;     FXOtTUeQRy13342336 = FXOtTUeQRy36580936;     FXOtTUeQRy36580936 = FXOtTUeQRy87877763;     FXOtTUeQRy87877763 = FXOtTUeQRy6178447;     FXOtTUeQRy6178447 = FXOtTUeQRy87475072;     FXOtTUeQRy87475072 = FXOtTUeQRy3330553;     FXOtTUeQRy3330553 = FXOtTUeQRy48575353;     FXOtTUeQRy48575353 = FXOtTUeQRy9608614;     FXOtTUeQRy9608614 = FXOtTUeQRy21174000;     FXOtTUeQRy21174000 = FXOtTUeQRy45876549;     FXOtTUeQRy45876549 = FXOtTUeQRy34987769;     FXOtTUeQRy34987769 = FXOtTUeQRy9329855;     FXOtTUeQRy9329855 = FXOtTUeQRy81337296;     FXOtTUeQRy81337296 = FXOtTUeQRy16362927;     FXOtTUeQRy16362927 = FXOtTUeQRy63794349;     FXOtTUeQRy63794349 = FXOtTUeQRy20020801;     FXOtTUeQRy20020801 = FXOtTUeQRy66680735;     FXOtTUeQRy66680735 = FXOtTUeQRy80105600;     FXOtTUeQRy80105600 = FXOtTUeQRy24209402;     FXOtTUeQRy24209402 = FXOtTUeQRy73671505;     FXOtTUeQRy73671505 = FXOtTUeQRy46059346;     FXOtTUeQRy46059346 = FXOtTUeQRy94719155;     FXOtTUeQRy94719155 = FXOtTUeQRy83721132;     FXOtTUeQRy83721132 = FXOtTUeQRy99424482;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void EglmAtEyNq55918775() {     int QNapHMwwps21811601 = -559344654;    int QNapHMwwps28299029 = -540541488;    int QNapHMwwps19579575 = -941800316;    int QNapHMwwps47531920 = -491098301;    int QNapHMwwps93298996 = -601095788;    int QNapHMwwps36870303 = -290282563;    int QNapHMwwps6479809 = -509474592;    int QNapHMwwps49699160 = -801964780;    int QNapHMwwps74528542 = -875883308;    int QNapHMwwps27713933 = -586151281;    int QNapHMwwps84499065 = -646410939;    int QNapHMwwps76488566 = -991499102;    int QNapHMwwps49015417 = -898565653;    int QNapHMwwps73103940 = -265307632;    int QNapHMwwps84174127 = -522116714;    int QNapHMwwps58912965 = -153960691;    int QNapHMwwps84796242 = -544119735;    int QNapHMwwps75800108 = 67408869;    int QNapHMwwps92309001 = -101214370;    int QNapHMwwps22366908 = -396769934;    int QNapHMwwps83578051 = -743080802;    int QNapHMwwps22437544 = -402625452;    int QNapHMwwps10334680 = -881625234;    int QNapHMwwps15260814 = -166856043;    int QNapHMwwps65423976 = -865728032;    int QNapHMwwps67671017 = -508684300;    int QNapHMwwps69991820 = -842398850;    int QNapHMwwps50254050 = -560693933;    int QNapHMwwps94277821 = -335211897;    int QNapHMwwps82924181 = -640342000;    int QNapHMwwps81261288 = -545925334;    int QNapHMwwps36039439 = -960113899;    int QNapHMwwps66441895 = -224488511;    int QNapHMwwps5297400 = -742669012;    int QNapHMwwps98368818 = -172160336;    int QNapHMwwps94965966 = -555434028;    int QNapHMwwps59283331 = -68575684;    int QNapHMwwps76288404 = -246298622;    int QNapHMwwps30745586 = -756678360;    int QNapHMwwps99376101 = -500855099;    int QNapHMwwps22948243 = -482776385;    int QNapHMwwps48859937 = -921898522;    int QNapHMwwps55207948 = -510977862;    int QNapHMwwps93143140 = 48674702;    int QNapHMwwps20163358 = -264646681;    int QNapHMwwps50106248 = -161483099;    int QNapHMwwps85486972 = -496824075;    int QNapHMwwps80026002 = -698707304;    int QNapHMwwps35471952 = -321589613;    int QNapHMwwps35893653 = -388565509;    int QNapHMwwps73027605 = -721932943;    int QNapHMwwps89647741 = 60415715;    int QNapHMwwps10936103 = -99146176;    int QNapHMwwps10702002 = 10365382;    int QNapHMwwps72340724 = -468178165;    int QNapHMwwps99374056 = -56719202;    int QNapHMwwps17964349 = -658916255;    int QNapHMwwps4318761 = -674944274;    int QNapHMwwps82107943 = -625370269;    int QNapHMwwps25627979 = 7588511;    int QNapHMwwps66878483 = -447883713;    int QNapHMwwps56225758 = -948780660;    int QNapHMwwps55421338 = -366752884;    int QNapHMwwps91604361 = -135541308;    int QNapHMwwps46452645 = 59774053;    int QNapHMwwps48459627 = -686297041;    int QNapHMwwps10046671 = -667010592;    int QNapHMwwps43718017 = -55896642;    int QNapHMwwps74735121 = 6852703;    int QNapHMwwps89208160 = -966682687;    int QNapHMwwps99629633 = 14614993;    int QNapHMwwps8507838 = -197821114;    int QNapHMwwps45054522 = -175912771;    int QNapHMwwps92932900 = -600359271;    int QNapHMwwps99418665 = -913993550;    int QNapHMwwps34718114 = -821182281;    int QNapHMwwps67229596 = -891647591;    int QNapHMwwps17191540 = -830299936;    int QNapHMwwps95097456 = -902209362;    int QNapHMwwps15317729 = -604244934;    int QNapHMwwps82184044 = 88139774;    int QNapHMwwps89965818 = -43691546;    int QNapHMwwps14782098 = -139104320;    int QNapHMwwps58384168 = -946646388;    int QNapHMwwps9896577 = -918409057;    int QNapHMwwps91613547 = -506341049;    int QNapHMwwps25103337 = -760967723;    int QNapHMwwps55739894 = -134853894;    int QNapHMwwps32956675 = -174490847;    int QNapHMwwps98994761 = -15441134;    int QNapHMwwps77001618 = -896517773;    int QNapHMwwps54964570 = -393631411;    int QNapHMwwps94180460 = -620928353;    int QNapHMwwps5117607 = -664266872;    int QNapHMwwps32497618 = 47028614;    int QNapHMwwps66722484 = -533995726;    int QNapHMwwps93438598 = -455145638;    int QNapHMwwps63603587 = -275436554;    int QNapHMwwps46690495 = 88900649;    int QNapHMwwps71703730 = -559344654;     QNapHMwwps21811601 = QNapHMwwps28299029;     QNapHMwwps28299029 = QNapHMwwps19579575;     QNapHMwwps19579575 = QNapHMwwps47531920;     QNapHMwwps47531920 = QNapHMwwps93298996;     QNapHMwwps93298996 = QNapHMwwps36870303;     QNapHMwwps36870303 = QNapHMwwps6479809;     QNapHMwwps6479809 = QNapHMwwps49699160;     QNapHMwwps49699160 = QNapHMwwps74528542;     QNapHMwwps74528542 = QNapHMwwps27713933;     QNapHMwwps27713933 = QNapHMwwps84499065;     QNapHMwwps84499065 = QNapHMwwps76488566;     QNapHMwwps76488566 = QNapHMwwps49015417;     QNapHMwwps49015417 = QNapHMwwps73103940;     QNapHMwwps73103940 = QNapHMwwps84174127;     QNapHMwwps84174127 = QNapHMwwps58912965;     QNapHMwwps58912965 = QNapHMwwps84796242;     QNapHMwwps84796242 = QNapHMwwps75800108;     QNapHMwwps75800108 = QNapHMwwps92309001;     QNapHMwwps92309001 = QNapHMwwps22366908;     QNapHMwwps22366908 = QNapHMwwps83578051;     QNapHMwwps83578051 = QNapHMwwps22437544;     QNapHMwwps22437544 = QNapHMwwps10334680;     QNapHMwwps10334680 = QNapHMwwps15260814;     QNapHMwwps15260814 = QNapHMwwps65423976;     QNapHMwwps65423976 = QNapHMwwps67671017;     QNapHMwwps67671017 = QNapHMwwps69991820;     QNapHMwwps69991820 = QNapHMwwps50254050;     QNapHMwwps50254050 = QNapHMwwps94277821;     QNapHMwwps94277821 = QNapHMwwps82924181;     QNapHMwwps82924181 = QNapHMwwps81261288;     QNapHMwwps81261288 = QNapHMwwps36039439;     QNapHMwwps36039439 = QNapHMwwps66441895;     QNapHMwwps66441895 = QNapHMwwps5297400;     QNapHMwwps5297400 = QNapHMwwps98368818;     QNapHMwwps98368818 = QNapHMwwps94965966;     QNapHMwwps94965966 = QNapHMwwps59283331;     QNapHMwwps59283331 = QNapHMwwps76288404;     QNapHMwwps76288404 = QNapHMwwps30745586;     QNapHMwwps30745586 = QNapHMwwps99376101;     QNapHMwwps99376101 = QNapHMwwps22948243;     QNapHMwwps22948243 = QNapHMwwps48859937;     QNapHMwwps48859937 = QNapHMwwps55207948;     QNapHMwwps55207948 = QNapHMwwps93143140;     QNapHMwwps93143140 = QNapHMwwps20163358;     QNapHMwwps20163358 = QNapHMwwps50106248;     QNapHMwwps50106248 = QNapHMwwps85486972;     QNapHMwwps85486972 = QNapHMwwps80026002;     QNapHMwwps80026002 = QNapHMwwps35471952;     QNapHMwwps35471952 = QNapHMwwps35893653;     QNapHMwwps35893653 = QNapHMwwps73027605;     QNapHMwwps73027605 = QNapHMwwps89647741;     QNapHMwwps89647741 = QNapHMwwps10936103;     QNapHMwwps10936103 = QNapHMwwps10702002;     QNapHMwwps10702002 = QNapHMwwps72340724;     QNapHMwwps72340724 = QNapHMwwps99374056;     QNapHMwwps99374056 = QNapHMwwps17964349;     QNapHMwwps17964349 = QNapHMwwps4318761;     QNapHMwwps4318761 = QNapHMwwps82107943;     QNapHMwwps82107943 = QNapHMwwps25627979;     QNapHMwwps25627979 = QNapHMwwps66878483;     QNapHMwwps66878483 = QNapHMwwps56225758;     QNapHMwwps56225758 = QNapHMwwps55421338;     QNapHMwwps55421338 = QNapHMwwps91604361;     QNapHMwwps91604361 = QNapHMwwps46452645;     QNapHMwwps46452645 = QNapHMwwps48459627;     QNapHMwwps48459627 = QNapHMwwps10046671;     QNapHMwwps10046671 = QNapHMwwps43718017;     QNapHMwwps43718017 = QNapHMwwps74735121;     QNapHMwwps74735121 = QNapHMwwps89208160;     QNapHMwwps89208160 = QNapHMwwps99629633;     QNapHMwwps99629633 = QNapHMwwps8507838;     QNapHMwwps8507838 = QNapHMwwps45054522;     QNapHMwwps45054522 = QNapHMwwps92932900;     QNapHMwwps92932900 = QNapHMwwps99418665;     QNapHMwwps99418665 = QNapHMwwps34718114;     QNapHMwwps34718114 = QNapHMwwps67229596;     QNapHMwwps67229596 = QNapHMwwps17191540;     QNapHMwwps17191540 = QNapHMwwps95097456;     QNapHMwwps95097456 = QNapHMwwps15317729;     QNapHMwwps15317729 = QNapHMwwps82184044;     QNapHMwwps82184044 = QNapHMwwps89965818;     QNapHMwwps89965818 = QNapHMwwps14782098;     QNapHMwwps14782098 = QNapHMwwps58384168;     QNapHMwwps58384168 = QNapHMwwps9896577;     QNapHMwwps9896577 = QNapHMwwps91613547;     QNapHMwwps91613547 = QNapHMwwps25103337;     QNapHMwwps25103337 = QNapHMwwps55739894;     QNapHMwwps55739894 = QNapHMwwps32956675;     QNapHMwwps32956675 = QNapHMwwps98994761;     QNapHMwwps98994761 = QNapHMwwps77001618;     QNapHMwwps77001618 = QNapHMwwps54964570;     QNapHMwwps54964570 = QNapHMwwps94180460;     QNapHMwwps94180460 = QNapHMwwps5117607;     QNapHMwwps5117607 = QNapHMwwps32497618;     QNapHMwwps32497618 = QNapHMwwps66722484;     QNapHMwwps66722484 = QNapHMwwps93438598;     QNapHMwwps93438598 = QNapHMwwps63603587;     QNapHMwwps63603587 = QNapHMwwps46690495;     QNapHMwwps46690495 = QNapHMwwps71703730;     QNapHMwwps71703730 = QNapHMwwps21811601;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void FqBfHgxxMc92563750() {     int dVanlVmAaD44198718 = -193748615;    int dVanlVmAaD12646530 = -489084537;    int dVanlVmAaD48348853 = -81454160;    int dVanlVmAaD62538608 = -497505835;    int dVanlVmAaD91473250 = -92152781;    int dVanlVmAaD51753420 = -150890941;    int dVanlVmAaD98635547 = -331461898;    int dVanlVmAaD25333019 = -172561409;    int dVanlVmAaD71691673 = -760079346;    int dVanlVmAaD19477518 = -328892614;    int dVanlVmAaD22040925 = -643035339;    int dVanlVmAaD32391515 = -142244387;    int dVanlVmAaD16023598 = -417282973;    int dVanlVmAaD15718692 = -793016388;    int dVanlVmAaD20909405 = -149594408;    int dVanlVmAaD3168878 = -535628233;    int dVanlVmAaD34619594 = -821399555;    int dVanlVmAaD81776524 = -384333817;    int dVanlVmAaD33590632 = -236316496;    int dVanlVmAaD883192 = -649404388;    int dVanlVmAaD84461158 = -734568105;    int dVanlVmAaD86252762 = -907391765;    int dVanlVmAaD13628003 = 42294174;    int dVanlVmAaD41760601 = -730848900;    int dVanlVmAaD964224 = -487287144;    int dVanlVmAaD55974011 = -952578528;    int dVanlVmAaD17081241 = -599985343;    int dVanlVmAaD56165500 = -349148380;    int dVanlVmAaD1170383 = -679469031;    int dVanlVmAaD92081360 = -560637867;    int dVanlVmAaD25552094 = -627241486;    int dVanlVmAaD93580629 = -512185256;    int dVanlVmAaD47480566 = -753832009;    int dVanlVmAaD64686087 = 33204138;    int dVanlVmAaD74598183 = -235916761;    int dVanlVmAaD36658836 = -870829423;    int dVanlVmAaD52723043 = -901523636;    int dVanlVmAaD29914505 = -113924091;    int dVanlVmAaD79053717 = -57724404;    int dVanlVmAaD19561816 = -637079128;    int dVanlVmAaD51705613 = -985954051;    int dVanlVmAaD37368328 = -679807233;    int dVanlVmAaD60758140 = -398508447;    int dVanlVmAaD92587259 = 51311053;    int dVanlVmAaD88146624 = -73790160;    int dVanlVmAaD58206530 = -343712213;    int dVanlVmAaD97784370 = 24765672;    int dVanlVmAaD24624677 = -455242296;    int dVanlVmAaD29931857 = -271306336;    int dVanlVmAaD32977399 = -530585763;    int dVanlVmAaD81896821 = 22990780;    int dVanlVmAaD63498998 = -370403571;    int dVanlVmAaD89250506 = -871057403;    int dVanlVmAaD70988546 = -265315426;    int dVanlVmAaD8102593 = -608882286;    int dVanlVmAaD57945955 = -286356850;    int dVanlVmAaD99018526 = -431378712;    int dVanlVmAaD6588253 = -350605260;    int dVanlVmAaD61574385 = 89781308;    int dVanlVmAaD35499240 = -139574254;    int dVanlVmAaD34672179 = -550905598;    int dVanlVmAaD42470047 = -982313518;    int dVanlVmAaD24162637 = -493092378;    int dVanlVmAaD79610312 = -99441479;    int dVanlVmAaD93925424 = -701651128;    int dVanlVmAaD28460296 = -30850083;    int dVanlVmAaD84910949 = -388412378;    int dVanlVmAaD51337510 = -350487111;    int dVanlVmAaD41120509 = -457099627;    int dVanlVmAaD84250569 = -278764986;    int dVanlVmAaD50445834 = -634104598;    int dVanlVmAaD4705089 = -607475465;    int dVanlVmAaD2722807 = -226609414;    int dVanlVmAaD14028816 = -599237368;    int dVanlVmAaD49177578 = -663450337;    int dVanlVmAaD47092830 = 45239127;    int dVanlVmAaD25494623 = -408883318;    int dVanlVmAaD21040744 = 90983120;    int dVanlVmAaD53613977 = -557058740;    int dVanlVmAaD42757693 = -43574932;    int dVanlVmAaD58189640 = -877344201;    int dVanlVmAaD92456564 = -44743047;    int dVanlVmAaD26233643 = 22157955;    int dVanlVmAaD68192983 = -48883269;    int dVanlVmAaD10184540 = -483628647;    int dVanlVmAaD62053096 = -156837915;    int dVanlVmAaD4330124 = -641127854;    int dVanlVmAaD76492019 = -388516583;    int dVanlVmAaD56583495 = -357913577;    int dVanlVmAaD16652228 = -949559912;    int dVanlVmAaD37640309 = -339450711;    int dVanlVmAaD46134791 = -450918376;    int dVanlVmAaD68340120 = -103705400;    int dVanlVmAaD43554478 = -918150151;    int dVanlVmAaD84889636 = 13826470;    int dVanlVmAaD9235566 = 96359467;    int dVanlVmAaD13205692 = -86714855;    int dVanlVmAaD81147827 = -199066968;    int dVanlVmAaD98661835 = -247037819;    int dVanlVmAaD59686328 = -193748615;     dVanlVmAaD44198718 = dVanlVmAaD12646530;     dVanlVmAaD12646530 = dVanlVmAaD48348853;     dVanlVmAaD48348853 = dVanlVmAaD62538608;     dVanlVmAaD62538608 = dVanlVmAaD91473250;     dVanlVmAaD91473250 = dVanlVmAaD51753420;     dVanlVmAaD51753420 = dVanlVmAaD98635547;     dVanlVmAaD98635547 = dVanlVmAaD25333019;     dVanlVmAaD25333019 = dVanlVmAaD71691673;     dVanlVmAaD71691673 = dVanlVmAaD19477518;     dVanlVmAaD19477518 = dVanlVmAaD22040925;     dVanlVmAaD22040925 = dVanlVmAaD32391515;     dVanlVmAaD32391515 = dVanlVmAaD16023598;     dVanlVmAaD16023598 = dVanlVmAaD15718692;     dVanlVmAaD15718692 = dVanlVmAaD20909405;     dVanlVmAaD20909405 = dVanlVmAaD3168878;     dVanlVmAaD3168878 = dVanlVmAaD34619594;     dVanlVmAaD34619594 = dVanlVmAaD81776524;     dVanlVmAaD81776524 = dVanlVmAaD33590632;     dVanlVmAaD33590632 = dVanlVmAaD883192;     dVanlVmAaD883192 = dVanlVmAaD84461158;     dVanlVmAaD84461158 = dVanlVmAaD86252762;     dVanlVmAaD86252762 = dVanlVmAaD13628003;     dVanlVmAaD13628003 = dVanlVmAaD41760601;     dVanlVmAaD41760601 = dVanlVmAaD964224;     dVanlVmAaD964224 = dVanlVmAaD55974011;     dVanlVmAaD55974011 = dVanlVmAaD17081241;     dVanlVmAaD17081241 = dVanlVmAaD56165500;     dVanlVmAaD56165500 = dVanlVmAaD1170383;     dVanlVmAaD1170383 = dVanlVmAaD92081360;     dVanlVmAaD92081360 = dVanlVmAaD25552094;     dVanlVmAaD25552094 = dVanlVmAaD93580629;     dVanlVmAaD93580629 = dVanlVmAaD47480566;     dVanlVmAaD47480566 = dVanlVmAaD64686087;     dVanlVmAaD64686087 = dVanlVmAaD74598183;     dVanlVmAaD74598183 = dVanlVmAaD36658836;     dVanlVmAaD36658836 = dVanlVmAaD52723043;     dVanlVmAaD52723043 = dVanlVmAaD29914505;     dVanlVmAaD29914505 = dVanlVmAaD79053717;     dVanlVmAaD79053717 = dVanlVmAaD19561816;     dVanlVmAaD19561816 = dVanlVmAaD51705613;     dVanlVmAaD51705613 = dVanlVmAaD37368328;     dVanlVmAaD37368328 = dVanlVmAaD60758140;     dVanlVmAaD60758140 = dVanlVmAaD92587259;     dVanlVmAaD92587259 = dVanlVmAaD88146624;     dVanlVmAaD88146624 = dVanlVmAaD58206530;     dVanlVmAaD58206530 = dVanlVmAaD97784370;     dVanlVmAaD97784370 = dVanlVmAaD24624677;     dVanlVmAaD24624677 = dVanlVmAaD29931857;     dVanlVmAaD29931857 = dVanlVmAaD32977399;     dVanlVmAaD32977399 = dVanlVmAaD81896821;     dVanlVmAaD81896821 = dVanlVmAaD63498998;     dVanlVmAaD63498998 = dVanlVmAaD89250506;     dVanlVmAaD89250506 = dVanlVmAaD70988546;     dVanlVmAaD70988546 = dVanlVmAaD8102593;     dVanlVmAaD8102593 = dVanlVmAaD57945955;     dVanlVmAaD57945955 = dVanlVmAaD99018526;     dVanlVmAaD99018526 = dVanlVmAaD6588253;     dVanlVmAaD6588253 = dVanlVmAaD61574385;     dVanlVmAaD61574385 = dVanlVmAaD35499240;     dVanlVmAaD35499240 = dVanlVmAaD34672179;     dVanlVmAaD34672179 = dVanlVmAaD42470047;     dVanlVmAaD42470047 = dVanlVmAaD24162637;     dVanlVmAaD24162637 = dVanlVmAaD79610312;     dVanlVmAaD79610312 = dVanlVmAaD93925424;     dVanlVmAaD93925424 = dVanlVmAaD28460296;     dVanlVmAaD28460296 = dVanlVmAaD84910949;     dVanlVmAaD84910949 = dVanlVmAaD51337510;     dVanlVmAaD51337510 = dVanlVmAaD41120509;     dVanlVmAaD41120509 = dVanlVmAaD84250569;     dVanlVmAaD84250569 = dVanlVmAaD50445834;     dVanlVmAaD50445834 = dVanlVmAaD4705089;     dVanlVmAaD4705089 = dVanlVmAaD2722807;     dVanlVmAaD2722807 = dVanlVmAaD14028816;     dVanlVmAaD14028816 = dVanlVmAaD49177578;     dVanlVmAaD49177578 = dVanlVmAaD47092830;     dVanlVmAaD47092830 = dVanlVmAaD25494623;     dVanlVmAaD25494623 = dVanlVmAaD21040744;     dVanlVmAaD21040744 = dVanlVmAaD53613977;     dVanlVmAaD53613977 = dVanlVmAaD42757693;     dVanlVmAaD42757693 = dVanlVmAaD58189640;     dVanlVmAaD58189640 = dVanlVmAaD92456564;     dVanlVmAaD92456564 = dVanlVmAaD26233643;     dVanlVmAaD26233643 = dVanlVmAaD68192983;     dVanlVmAaD68192983 = dVanlVmAaD10184540;     dVanlVmAaD10184540 = dVanlVmAaD62053096;     dVanlVmAaD62053096 = dVanlVmAaD4330124;     dVanlVmAaD4330124 = dVanlVmAaD76492019;     dVanlVmAaD76492019 = dVanlVmAaD56583495;     dVanlVmAaD56583495 = dVanlVmAaD16652228;     dVanlVmAaD16652228 = dVanlVmAaD37640309;     dVanlVmAaD37640309 = dVanlVmAaD46134791;     dVanlVmAaD46134791 = dVanlVmAaD68340120;     dVanlVmAaD68340120 = dVanlVmAaD43554478;     dVanlVmAaD43554478 = dVanlVmAaD84889636;     dVanlVmAaD84889636 = dVanlVmAaD9235566;     dVanlVmAaD9235566 = dVanlVmAaD13205692;     dVanlVmAaD13205692 = dVanlVmAaD81147827;     dVanlVmAaD81147827 = dVanlVmAaD98661835;     dVanlVmAaD98661835 = dVanlVmAaD59686328;     dVanlVmAaD59686328 = dVanlVmAaD44198718;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void vLBpypcYUO72070341() {     int qoMXwGQtOZ53786725 = -538160171;    int qoMXwGQtOZ87367774 = -352483835;    int qoMXwGQtOZ61991756 = -927005850;    int qoMXwGQtOZ39764193 = -320792747;    int qoMXwGQtOZ13816605 = -28103969;    int qoMXwGQtOZ92937331 = -733441629;    int qoMXwGQtOZ6678200 = -116850068;    int qoMXwGQtOZ12675866 = -706994447;    int qoMXwGQtOZ38930322 = -109809434;    int qoMXwGQtOZ98301424 = -641160056;    int qoMXwGQtOZ72736613 = -443917869;    int qoMXwGQtOZ84927313 = -456613598;    int qoMXwGQtOZ76594303 = -75534584;    int qoMXwGQtOZ16397806 = -578616050;    int qoMXwGQtOZ24682347 = -895512013;    int qoMXwGQtOZ68128816 = 68004312;    int qoMXwGQtOZ40396647 = 25865952;    int qoMXwGQtOZ50195521 = -975009453;    int qoMXwGQtOZ14568173 = -597913958;    int qoMXwGQtOZ20892505 = -426843605;    int qoMXwGQtOZ64509057 = -755671902;    int qoMXwGQtOZ77612693 = -62283497;    int qoMXwGQtOZ51917824 = -20698207;    int qoMXwGQtOZ11053476 = -696337790;    int qoMXwGQtOZ87495590 = -751792063;    int qoMXwGQtOZ96106514 = -422453406;    int qoMXwGQtOZ93198288 = -325464981;    int qoMXwGQtOZ65531075 = -81738712;    int qoMXwGQtOZ16802784 = 98350475;    int qoMXwGQtOZ667165 = -201910326;    int qoMXwGQtOZ92232260 = -106327357;    int qoMXwGQtOZ17250712 = -869180452;    int qoMXwGQtOZ53378133 = -711209148;    int qoMXwGQtOZ47564076 = -139531959;    int qoMXwGQtOZ32483091 = -208269150;    int qoMXwGQtOZ68783969 = -457193169;    int qoMXwGQtOZ33281431 = 56131007;    int qoMXwGQtOZ65671942 = -52454832;    int qoMXwGQtOZ92213656 = -942282678;    int qoMXwGQtOZ57228586 = -147749684;    int qoMXwGQtOZ94743011 = -189545521;    int qoMXwGQtOZ89569585 = -926988734;    int qoMXwGQtOZ51991582 = -280866092;    int qoMXwGQtOZ65866637 = -918809240;    int qoMXwGQtOZ44157505 = -936682480;    int qoMXwGQtOZ73534619 = -76712192;    int qoMXwGQtOZ87895333 = 81309227;    int qoMXwGQtOZ90550546 = -173947219;    int qoMXwGQtOZ17644491 = -284948758;    int qoMXwGQtOZ23201570 = -335619337;    int qoMXwGQtOZ98957283 = -336311204;    int qoMXwGQtOZ38268223 = 5670992;    int qoMXwGQtOZ83976455 = -151073370;    int qoMXwGQtOZ7075909 = -915384196;    int qoMXwGQtOZ7561285 = -268940465;    int qoMXwGQtOZ76174032 = -375876675;    int qoMXwGQtOZ35449951 = -231785628;    int qoMXwGQtOZ50938281 = -130668060;    int qoMXwGQtOZ52268603 = -569000685;    int qoMXwGQtOZ17710090 = -605650564;    int qoMXwGQtOZ99739042 = -307976648;    int qoMXwGQtOZ41147125 = 64888644;    int qoMXwGQtOZ95873081 = -705344922;    int qoMXwGQtOZ38263157 = -907899108;    int qoMXwGQtOZ6069165 = -434832700;    int qoMXwGQtOZ55485902 = -574737417;    int qoMXwGQtOZ31549180 = -745404450;    int qoMXwGQtOZ29030227 = -936002626;    int qoMXwGQtOZ83914714 = -270346901;    int qoMXwGQtOZ55898377 = -338318845;    int qoMXwGQtOZ34847386 = -988126695;    int qoMXwGQtOZ74724704 = -921679216;    int qoMXwGQtOZ57981865 = 67273224;    int qoMXwGQtOZ57339586 = -350164275;    int qoMXwGQtOZ26149493 = -137298085;    int qoMXwGQtOZ74939471 = -828683168;    int qoMXwGQtOZ25621111 = -781417405;    int qoMXwGQtOZ86051186 = -101888968;    int qoMXwGQtOZ66895970 = -759655311;    int qoMXwGQtOZ13960971 = -575079872;    int qoMXwGQtOZ8211182 = -403762633;    int qoMXwGQtOZ2647742 = -51517763;    int qoMXwGQtOZ47886584 = -796789955;    int qoMXwGQtOZ93601214 = -566030188;    int qoMXwGQtOZ1709881 = -865599123;    int qoMXwGQtOZ53964037 = -11998350;    int qoMXwGQtOZ33274257 = -618107083;    int qoMXwGQtOZ46302225 = -795824953;    int qoMXwGQtOZ40002791 = -870591495;    int qoMXwGQtOZ56309059 = -832392476;    int qoMXwGQtOZ33334019 = -125407542;    int qoMXwGQtOZ82343149 = -813200933;    int qoMXwGQtOZ13403340 = -483454148;    int qoMXwGQtOZ74503566 = -236632115;    int qoMXwGQtOZ57489543 = -839773036;    int qoMXwGQtOZ53595886 = -154434165;    int qoMXwGQtOZ93696503 = -121643812;    int qoMXwGQtOZ13728425 = -372966985;    int qoMXwGQtOZ59797473 = -383976541;    int qoMXwGQtOZ88671603 = -538160171;     qoMXwGQtOZ53786725 = qoMXwGQtOZ87367774;     qoMXwGQtOZ87367774 = qoMXwGQtOZ61991756;     qoMXwGQtOZ61991756 = qoMXwGQtOZ39764193;     qoMXwGQtOZ39764193 = qoMXwGQtOZ13816605;     qoMXwGQtOZ13816605 = qoMXwGQtOZ92937331;     qoMXwGQtOZ92937331 = qoMXwGQtOZ6678200;     qoMXwGQtOZ6678200 = qoMXwGQtOZ12675866;     qoMXwGQtOZ12675866 = qoMXwGQtOZ38930322;     qoMXwGQtOZ38930322 = qoMXwGQtOZ98301424;     qoMXwGQtOZ98301424 = qoMXwGQtOZ72736613;     qoMXwGQtOZ72736613 = qoMXwGQtOZ84927313;     qoMXwGQtOZ84927313 = qoMXwGQtOZ76594303;     qoMXwGQtOZ76594303 = qoMXwGQtOZ16397806;     qoMXwGQtOZ16397806 = qoMXwGQtOZ24682347;     qoMXwGQtOZ24682347 = qoMXwGQtOZ68128816;     qoMXwGQtOZ68128816 = qoMXwGQtOZ40396647;     qoMXwGQtOZ40396647 = qoMXwGQtOZ50195521;     qoMXwGQtOZ50195521 = qoMXwGQtOZ14568173;     qoMXwGQtOZ14568173 = qoMXwGQtOZ20892505;     qoMXwGQtOZ20892505 = qoMXwGQtOZ64509057;     qoMXwGQtOZ64509057 = qoMXwGQtOZ77612693;     qoMXwGQtOZ77612693 = qoMXwGQtOZ51917824;     qoMXwGQtOZ51917824 = qoMXwGQtOZ11053476;     qoMXwGQtOZ11053476 = qoMXwGQtOZ87495590;     qoMXwGQtOZ87495590 = qoMXwGQtOZ96106514;     qoMXwGQtOZ96106514 = qoMXwGQtOZ93198288;     qoMXwGQtOZ93198288 = qoMXwGQtOZ65531075;     qoMXwGQtOZ65531075 = qoMXwGQtOZ16802784;     qoMXwGQtOZ16802784 = qoMXwGQtOZ667165;     qoMXwGQtOZ667165 = qoMXwGQtOZ92232260;     qoMXwGQtOZ92232260 = qoMXwGQtOZ17250712;     qoMXwGQtOZ17250712 = qoMXwGQtOZ53378133;     qoMXwGQtOZ53378133 = qoMXwGQtOZ47564076;     qoMXwGQtOZ47564076 = qoMXwGQtOZ32483091;     qoMXwGQtOZ32483091 = qoMXwGQtOZ68783969;     qoMXwGQtOZ68783969 = qoMXwGQtOZ33281431;     qoMXwGQtOZ33281431 = qoMXwGQtOZ65671942;     qoMXwGQtOZ65671942 = qoMXwGQtOZ92213656;     qoMXwGQtOZ92213656 = qoMXwGQtOZ57228586;     qoMXwGQtOZ57228586 = qoMXwGQtOZ94743011;     qoMXwGQtOZ94743011 = qoMXwGQtOZ89569585;     qoMXwGQtOZ89569585 = qoMXwGQtOZ51991582;     qoMXwGQtOZ51991582 = qoMXwGQtOZ65866637;     qoMXwGQtOZ65866637 = qoMXwGQtOZ44157505;     qoMXwGQtOZ44157505 = qoMXwGQtOZ73534619;     qoMXwGQtOZ73534619 = qoMXwGQtOZ87895333;     qoMXwGQtOZ87895333 = qoMXwGQtOZ90550546;     qoMXwGQtOZ90550546 = qoMXwGQtOZ17644491;     qoMXwGQtOZ17644491 = qoMXwGQtOZ23201570;     qoMXwGQtOZ23201570 = qoMXwGQtOZ98957283;     qoMXwGQtOZ98957283 = qoMXwGQtOZ38268223;     qoMXwGQtOZ38268223 = qoMXwGQtOZ83976455;     qoMXwGQtOZ83976455 = qoMXwGQtOZ7075909;     qoMXwGQtOZ7075909 = qoMXwGQtOZ7561285;     qoMXwGQtOZ7561285 = qoMXwGQtOZ76174032;     qoMXwGQtOZ76174032 = qoMXwGQtOZ35449951;     qoMXwGQtOZ35449951 = qoMXwGQtOZ50938281;     qoMXwGQtOZ50938281 = qoMXwGQtOZ52268603;     qoMXwGQtOZ52268603 = qoMXwGQtOZ17710090;     qoMXwGQtOZ17710090 = qoMXwGQtOZ99739042;     qoMXwGQtOZ99739042 = qoMXwGQtOZ41147125;     qoMXwGQtOZ41147125 = qoMXwGQtOZ95873081;     qoMXwGQtOZ95873081 = qoMXwGQtOZ38263157;     qoMXwGQtOZ38263157 = qoMXwGQtOZ6069165;     qoMXwGQtOZ6069165 = qoMXwGQtOZ55485902;     qoMXwGQtOZ55485902 = qoMXwGQtOZ31549180;     qoMXwGQtOZ31549180 = qoMXwGQtOZ29030227;     qoMXwGQtOZ29030227 = qoMXwGQtOZ83914714;     qoMXwGQtOZ83914714 = qoMXwGQtOZ55898377;     qoMXwGQtOZ55898377 = qoMXwGQtOZ34847386;     qoMXwGQtOZ34847386 = qoMXwGQtOZ74724704;     qoMXwGQtOZ74724704 = qoMXwGQtOZ57981865;     qoMXwGQtOZ57981865 = qoMXwGQtOZ57339586;     qoMXwGQtOZ57339586 = qoMXwGQtOZ26149493;     qoMXwGQtOZ26149493 = qoMXwGQtOZ74939471;     qoMXwGQtOZ74939471 = qoMXwGQtOZ25621111;     qoMXwGQtOZ25621111 = qoMXwGQtOZ86051186;     qoMXwGQtOZ86051186 = qoMXwGQtOZ66895970;     qoMXwGQtOZ66895970 = qoMXwGQtOZ13960971;     qoMXwGQtOZ13960971 = qoMXwGQtOZ8211182;     qoMXwGQtOZ8211182 = qoMXwGQtOZ2647742;     qoMXwGQtOZ2647742 = qoMXwGQtOZ47886584;     qoMXwGQtOZ47886584 = qoMXwGQtOZ93601214;     qoMXwGQtOZ93601214 = qoMXwGQtOZ1709881;     qoMXwGQtOZ1709881 = qoMXwGQtOZ53964037;     qoMXwGQtOZ53964037 = qoMXwGQtOZ33274257;     qoMXwGQtOZ33274257 = qoMXwGQtOZ46302225;     qoMXwGQtOZ46302225 = qoMXwGQtOZ40002791;     qoMXwGQtOZ40002791 = qoMXwGQtOZ56309059;     qoMXwGQtOZ56309059 = qoMXwGQtOZ33334019;     qoMXwGQtOZ33334019 = qoMXwGQtOZ82343149;     qoMXwGQtOZ82343149 = qoMXwGQtOZ13403340;     qoMXwGQtOZ13403340 = qoMXwGQtOZ74503566;     qoMXwGQtOZ74503566 = qoMXwGQtOZ57489543;     qoMXwGQtOZ57489543 = qoMXwGQtOZ53595886;     qoMXwGQtOZ53595886 = qoMXwGQtOZ93696503;     qoMXwGQtOZ93696503 = qoMXwGQtOZ13728425;     qoMXwGQtOZ13728425 = qoMXwGQtOZ59797473;     qoMXwGQtOZ59797473 = qoMXwGQtOZ88671603;     qoMXwGQtOZ88671603 = qoMXwGQtOZ53786725;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void HiuZVsZJeL8715316() {     int QQlGmYRoGf76173842 = -172564134;    int QQlGmYRoGf71715274 = -301026889;    int QQlGmYRoGf90761035 = -66659694;    int QQlGmYRoGf54770881 = -327200279;    int QQlGmYRoGf11990859 = -619160970;    int QQlGmYRoGf7820449 = -594050006;    int QQlGmYRoGf98833938 = 61162626;    int QQlGmYRoGf88309724 = -77591091;    int QQlGmYRoGf36093453 = 5994527;    int QQlGmYRoGf90065009 = -383901389;    int QQlGmYRoGf10278473 = -440542270;    int QQlGmYRoGf40830262 = -707358892;    int QQlGmYRoGf43602483 = -694251904;    int QQlGmYRoGf59012558 = -6324807;    int QQlGmYRoGf61417624 = -522989726;    int QQlGmYRoGf12384729 = -313663230;    int QQlGmYRoGf90219998 = -251413868;    int QQlGmYRoGf56171938 = -326752144;    int QQlGmYRoGf55849803 = -733016083;    int QQlGmYRoGf99408787 = -679478058;    int QQlGmYRoGf65392164 = -747159210;    int QQlGmYRoGf41427912 = -567049810;    int QQlGmYRoGf55211147 = -196778799;    int QQlGmYRoGf37553263 = -160330647;    int QQlGmYRoGf23035837 = -373351188;    int QQlGmYRoGf84409509 = -866347629;    int QQlGmYRoGf40287709 = -83051474;    int QQlGmYRoGf71442525 = -970193165;    int QQlGmYRoGf23695345 = -245906644;    int QQlGmYRoGf9824345 = -122206193;    int QQlGmYRoGf36523066 = -187643509;    int QQlGmYRoGf74791902 = -421251807;    int QQlGmYRoGf34416804 = -140552643;    int QQlGmYRoGf6952763 = -463658809;    int QQlGmYRoGf8712456 = -272025572;    int QQlGmYRoGf10476839 = -772588545;    int QQlGmYRoGf26721143 = -776816945;    int QQlGmYRoGf19298043 = 79919696;    int QQlGmYRoGf40521787 = -243328710;    int QQlGmYRoGf77414300 = -283973713;    int QQlGmYRoGf23500382 = -692723187;    int QQlGmYRoGf78077976 = -684897425;    int QQlGmYRoGf57541774 = -168396676;    int QQlGmYRoGf65310756 = -916172889;    int QQlGmYRoGf12140772 = -745825957;    int QQlGmYRoGf81634901 = -258941287;    int QQlGmYRoGf192731 = -497101026;    int QQlGmYRoGf35149221 = 69517791;    int QQlGmYRoGf12104396 = -234665456;    int QQlGmYRoGf20285316 = -477639596;    int QQlGmYRoGf7826501 = -691387481;    int QQlGmYRoGf12119480 = -425148290;    int QQlGmYRoGf62290858 = -922984600;    int QQlGmYRoGf67362453 = -91065004;    int QQlGmYRoGf43323152 = -409644580;    int QQlGmYRoGf34745931 = -605514324;    int QQlGmYRoGf16504128 = -4248090;    int QQlGmYRoGf53207773 = -906329047;    int QQlGmYRoGf31735045 = -953849092;    int QQlGmYRoGf27581349 = -752813341;    int QQlGmYRoGf67532739 = -410998533;    int QQlGmYRoGf27391414 = 31355791;    int QQlGmYRoGf64614380 = -831684447;    int QQlGmYRoGf26269109 = -871799280;    int QQlGmYRoGf53541944 = -96257881;    int QQlGmYRoGf35486571 = 80709537;    int QQlGmYRoGf6413459 = -466806249;    int QQlGmYRoGf36649721 = -130593095;    int QQlGmYRoGf50300102 = -734299236;    int QQlGmYRoGf50940785 = -750401181;    int QQlGmYRoGf85663585 = -536846286;    int QQlGmYRoGf70921955 = -231333564;    int QQlGmYRoGf15650151 = 16576565;    int QQlGmYRoGf78435503 = -349042371;    int QQlGmYRoGf75908405 = -986754872;    int QQlGmYRoGf87314187 = 37738214;    int QQlGmYRoGf83886137 = -298653134;    int QQlGmYRoGf89900390 = -280605911;    int QQlGmYRoGf25412491 = -414504690;    int QQlGmYRoGf41400936 = -14409901;    int QQlGmYRoGf84216778 = -269246604;    int QQlGmYRoGf5138488 = -52569265;    int QQlGmYRoGf59338129 = -635527710;    int QQlGmYRoGf3410030 = -768267049;    int QQlGmYRoGf1997844 = -430818713;    int QQlGmYRoGf24403586 = -762495220;    int QQlGmYRoGf12501044 = -498267208;    int QQlGmYRoGf67054350 = 50512361;    int QQlGmYRoGf63629611 = 45985771;    int QQlGmYRoGf73966525 = -666511248;    int QQlGmYRoGf93972711 = -668340455;    int QQlGmYRoGf73513370 = -870487898;    int QQlGmYRoGf87562998 = 33768787;    int QQlGmYRoGf12940438 = -490515369;    int QQlGmYRoGf9881562 = -872975181;    int QQlGmYRoGf96108968 = -624078979;    int QQlGmYRoGf13463597 = -853212978;    int QQlGmYRoGf31272665 = -296597397;    int QQlGmYRoGf11768813 = -719915009;    int QQlGmYRoGf76654201 = -172564134;     QQlGmYRoGf76173842 = QQlGmYRoGf71715274;     QQlGmYRoGf71715274 = QQlGmYRoGf90761035;     QQlGmYRoGf90761035 = QQlGmYRoGf54770881;     QQlGmYRoGf54770881 = QQlGmYRoGf11990859;     QQlGmYRoGf11990859 = QQlGmYRoGf7820449;     QQlGmYRoGf7820449 = QQlGmYRoGf98833938;     QQlGmYRoGf98833938 = QQlGmYRoGf88309724;     QQlGmYRoGf88309724 = QQlGmYRoGf36093453;     QQlGmYRoGf36093453 = QQlGmYRoGf90065009;     QQlGmYRoGf90065009 = QQlGmYRoGf10278473;     QQlGmYRoGf10278473 = QQlGmYRoGf40830262;     QQlGmYRoGf40830262 = QQlGmYRoGf43602483;     QQlGmYRoGf43602483 = QQlGmYRoGf59012558;     QQlGmYRoGf59012558 = QQlGmYRoGf61417624;     QQlGmYRoGf61417624 = QQlGmYRoGf12384729;     QQlGmYRoGf12384729 = QQlGmYRoGf90219998;     QQlGmYRoGf90219998 = QQlGmYRoGf56171938;     QQlGmYRoGf56171938 = QQlGmYRoGf55849803;     QQlGmYRoGf55849803 = QQlGmYRoGf99408787;     QQlGmYRoGf99408787 = QQlGmYRoGf65392164;     QQlGmYRoGf65392164 = QQlGmYRoGf41427912;     QQlGmYRoGf41427912 = QQlGmYRoGf55211147;     QQlGmYRoGf55211147 = QQlGmYRoGf37553263;     QQlGmYRoGf37553263 = QQlGmYRoGf23035837;     QQlGmYRoGf23035837 = QQlGmYRoGf84409509;     QQlGmYRoGf84409509 = QQlGmYRoGf40287709;     QQlGmYRoGf40287709 = QQlGmYRoGf71442525;     QQlGmYRoGf71442525 = QQlGmYRoGf23695345;     QQlGmYRoGf23695345 = QQlGmYRoGf9824345;     QQlGmYRoGf9824345 = QQlGmYRoGf36523066;     QQlGmYRoGf36523066 = QQlGmYRoGf74791902;     QQlGmYRoGf74791902 = QQlGmYRoGf34416804;     QQlGmYRoGf34416804 = QQlGmYRoGf6952763;     QQlGmYRoGf6952763 = QQlGmYRoGf8712456;     QQlGmYRoGf8712456 = QQlGmYRoGf10476839;     QQlGmYRoGf10476839 = QQlGmYRoGf26721143;     QQlGmYRoGf26721143 = QQlGmYRoGf19298043;     QQlGmYRoGf19298043 = QQlGmYRoGf40521787;     QQlGmYRoGf40521787 = QQlGmYRoGf77414300;     QQlGmYRoGf77414300 = QQlGmYRoGf23500382;     QQlGmYRoGf23500382 = QQlGmYRoGf78077976;     QQlGmYRoGf78077976 = QQlGmYRoGf57541774;     QQlGmYRoGf57541774 = QQlGmYRoGf65310756;     QQlGmYRoGf65310756 = QQlGmYRoGf12140772;     QQlGmYRoGf12140772 = QQlGmYRoGf81634901;     QQlGmYRoGf81634901 = QQlGmYRoGf192731;     QQlGmYRoGf192731 = QQlGmYRoGf35149221;     QQlGmYRoGf35149221 = QQlGmYRoGf12104396;     QQlGmYRoGf12104396 = QQlGmYRoGf20285316;     QQlGmYRoGf20285316 = QQlGmYRoGf7826501;     QQlGmYRoGf7826501 = QQlGmYRoGf12119480;     QQlGmYRoGf12119480 = QQlGmYRoGf62290858;     QQlGmYRoGf62290858 = QQlGmYRoGf67362453;     QQlGmYRoGf67362453 = QQlGmYRoGf43323152;     QQlGmYRoGf43323152 = QQlGmYRoGf34745931;     QQlGmYRoGf34745931 = QQlGmYRoGf16504128;     QQlGmYRoGf16504128 = QQlGmYRoGf53207773;     QQlGmYRoGf53207773 = QQlGmYRoGf31735045;     QQlGmYRoGf31735045 = QQlGmYRoGf27581349;     QQlGmYRoGf27581349 = QQlGmYRoGf67532739;     QQlGmYRoGf67532739 = QQlGmYRoGf27391414;     QQlGmYRoGf27391414 = QQlGmYRoGf64614380;     QQlGmYRoGf64614380 = QQlGmYRoGf26269109;     QQlGmYRoGf26269109 = QQlGmYRoGf53541944;     QQlGmYRoGf53541944 = QQlGmYRoGf35486571;     QQlGmYRoGf35486571 = QQlGmYRoGf6413459;     QQlGmYRoGf6413459 = QQlGmYRoGf36649721;     QQlGmYRoGf36649721 = QQlGmYRoGf50300102;     QQlGmYRoGf50300102 = QQlGmYRoGf50940785;     QQlGmYRoGf50940785 = QQlGmYRoGf85663585;     QQlGmYRoGf85663585 = QQlGmYRoGf70921955;     QQlGmYRoGf70921955 = QQlGmYRoGf15650151;     QQlGmYRoGf15650151 = QQlGmYRoGf78435503;     QQlGmYRoGf78435503 = QQlGmYRoGf75908405;     QQlGmYRoGf75908405 = QQlGmYRoGf87314187;     QQlGmYRoGf87314187 = QQlGmYRoGf83886137;     QQlGmYRoGf83886137 = QQlGmYRoGf89900390;     QQlGmYRoGf89900390 = QQlGmYRoGf25412491;     QQlGmYRoGf25412491 = QQlGmYRoGf41400936;     QQlGmYRoGf41400936 = QQlGmYRoGf84216778;     QQlGmYRoGf84216778 = QQlGmYRoGf5138488;     QQlGmYRoGf5138488 = QQlGmYRoGf59338129;     QQlGmYRoGf59338129 = QQlGmYRoGf3410030;     QQlGmYRoGf3410030 = QQlGmYRoGf1997844;     QQlGmYRoGf1997844 = QQlGmYRoGf24403586;     QQlGmYRoGf24403586 = QQlGmYRoGf12501044;     QQlGmYRoGf12501044 = QQlGmYRoGf67054350;     QQlGmYRoGf67054350 = QQlGmYRoGf63629611;     QQlGmYRoGf63629611 = QQlGmYRoGf73966525;     QQlGmYRoGf73966525 = QQlGmYRoGf93972711;     QQlGmYRoGf93972711 = QQlGmYRoGf73513370;     QQlGmYRoGf73513370 = QQlGmYRoGf87562998;     QQlGmYRoGf87562998 = QQlGmYRoGf12940438;     QQlGmYRoGf12940438 = QQlGmYRoGf9881562;     QQlGmYRoGf9881562 = QQlGmYRoGf96108968;     QQlGmYRoGf96108968 = QQlGmYRoGf13463597;     QQlGmYRoGf13463597 = QQlGmYRoGf31272665;     QQlGmYRoGf31272665 = QQlGmYRoGf11768813;     QQlGmYRoGf11768813 = QQlGmYRoGf76654201;     QQlGmYRoGf76654201 = QQlGmYRoGf76173842;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ZakzdugMPP45360290() {     int SkvYQUvnYr98560959 = -906968097;    int SkvYQUvnYr56062775 = -249569944;    int SkvYQUvnYr19530314 = -306313538;    int SkvYQUvnYr69777569 = -333607811;    int SkvYQUvnYr10165113 = -110217971;    int SkvYQUvnYr22703565 = -454658384;    int SkvYQUvnYr90989677 = -860824680;    int SkvYQUvnYr63943583 = -548187735;    int SkvYQUvnYr33256584 = -978201512;    int SkvYQUvnYr81828594 = -126642722;    int SkvYQUvnYr47820333 = -437166671;    int SkvYQUvnYr96733211 = -958104185;    int SkvYQUvnYr10610664 = -212969224;    int SkvYQUvnYr1627311 = -534033564;    int SkvYQUvnYr98152900 = -150467439;    int SkvYQUvnYr56640641 = -695330773;    int SkvYQUvnYr40043350 = -528693688;    int SkvYQUvnYr62148354 = -778494835;    int SkvYQUvnYr97131434 = -868118209;    int SkvYQUvnYr77925071 = -932112512;    int SkvYQUvnYr66275271 = -738646519;    int SkvYQUvnYr5243130 = 28183877;    int SkvYQUvnYr58504470 = -372859391;    int SkvYQUvnYr64053050 = -724323504;    int SkvYQUvnYr58576083 = 5089688;    int SkvYQUvnYr72712504 = -210241853;    int SkvYQUvnYr87377129 = -940637967;    int SkvYQUvnYr77353975 = -758647619;    int SkvYQUvnYr30587906 = -590163763;    int SkvYQUvnYr18981524 = -42502060;    int SkvYQUvnYr80813870 = -268959661;    int SkvYQUvnYr32333093 = 26676838;    int SkvYQUvnYr15455474 = -669896138;    int SkvYQUvnYr66341450 = -787785659;    int SkvYQUvnYr84941820 = -335781995;    int SkvYQUvnYr52169708 = 12016078;    int SkvYQUvnYr20160855 = -509764896;    int SkvYQUvnYr72924144 = -887705776;    int SkvYQUvnYr88829918 = -644374742;    int SkvYQUvnYr97600015 = -420197742;    int SkvYQUvnYr52257753 = -95900853;    int SkvYQUvnYr66586368 = -442806116;    int SkvYQUvnYr63091966 = -55927260;    int SkvYQUvnYr64754876 = -913536538;    int SkvYQUvnYr80124038 = -554969435;    int SkvYQUvnYr89735182 = -441170382;    int SkvYQUvnYr12490128 = 24488722;    int SkvYQUvnYr79747896 = -787017200;    int SkvYQUvnYr6564302 = -184382154;    int SkvYQUvnYr17369062 = -619659855;    int SkvYQUvnYr16695717 = 53536242;    int SkvYQUvnYr85970737 = -855967572;    int SkvYQUvnYr40605262 = -594895830;    int SkvYQUvnYr27648999 = -366745813;    int SkvYQUvnYr79085019 = -550348695;    int SkvYQUvnYr93317830 = -835151974;    int SkvYQUvnYr97558304 = -876710553;    int SkvYQUvnYr55477264 = -581990034;    int SkvYQUvnYr11201486 = -238697499;    int SkvYQUvnYr37452609 = -899976118;    int SkvYQUvnYr35326436 = -514020418;    int SkvYQUvnYr13635703 = -2177061;    int SkvYQUvnYr33355678 = -958023972;    int SkvYQUvnYr14275061 = -835699453;    int SkvYQUvnYr1014724 = -857683061;    int SkvYQUvnYr15487240 = -363843510;    int SkvYQUvnYr81277737 = -188208047;    int SkvYQUvnYr44269214 = -425183565;    int SkvYQUvnYr16685490 = -98251570;    int SkvYQUvnYr45983193 = -62483518;    int SkvYQUvnYr36479786 = -85565877;    int SkvYQUvnYr67119206 = -640987912;    int SkvYQUvnYr73318435 = -34120094;    int SkvYQUvnYr99531419 = -347920467;    int SkvYQUvnYr25667318 = -736211660;    int SkvYQUvnYr99688903 = -195840403;    int SkvYQUvnYr42151164 = -915888864;    int SkvYQUvnYr93749594 = -459322854;    int SkvYQUvnYr83929011 = -69354070;    int SkvYQUvnYr68840901 = -553739930;    int SkvYQUvnYr60222376 = -134730575;    int SkvYQUvnYr7629234 = -53620767;    int SkvYQUvnYr70789674 = -474265465;    int SkvYQUvnYr13218845 = -970503909;    int SkvYQUvnYr2285807 = 3961698;    int SkvYQUvnYr94843133 = -412992090;    int SkvYQUvnYr91727831 = -378427332;    int SkvYQUvnYr87806475 = -203150325;    int SkvYQUvnYr87256431 = -137436964;    int SkvYQUvnYr91623990 = -500630021;    int SkvYQUvnYr54611403 = -111273369;    int SkvYQUvnYr64683591 = -927774863;    int SkvYQUvnYr61722658 = -549008278;    int SkvYQUvnYr51377310 = -744398624;    int SkvYQUvnYr62273579 = -906177325;    int SkvYQUvnYr38622051 = 6276208;    int SkvYQUvnYr33230691 = -484782144;    int SkvYQUvnYr48816906 = -220227808;    int SkvYQUvnYr63740152 = 44146523;    int SkvYQUvnYr64636799 = -906968097;     SkvYQUvnYr98560959 = SkvYQUvnYr56062775;     SkvYQUvnYr56062775 = SkvYQUvnYr19530314;     SkvYQUvnYr19530314 = SkvYQUvnYr69777569;     SkvYQUvnYr69777569 = SkvYQUvnYr10165113;     SkvYQUvnYr10165113 = SkvYQUvnYr22703565;     SkvYQUvnYr22703565 = SkvYQUvnYr90989677;     SkvYQUvnYr90989677 = SkvYQUvnYr63943583;     SkvYQUvnYr63943583 = SkvYQUvnYr33256584;     SkvYQUvnYr33256584 = SkvYQUvnYr81828594;     SkvYQUvnYr81828594 = SkvYQUvnYr47820333;     SkvYQUvnYr47820333 = SkvYQUvnYr96733211;     SkvYQUvnYr96733211 = SkvYQUvnYr10610664;     SkvYQUvnYr10610664 = SkvYQUvnYr1627311;     SkvYQUvnYr1627311 = SkvYQUvnYr98152900;     SkvYQUvnYr98152900 = SkvYQUvnYr56640641;     SkvYQUvnYr56640641 = SkvYQUvnYr40043350;     SkvYQUvnYr40043350 = SkvYQUvnYr62148354;     SkvYQUvnYr62148354 = SkvYQUvnYr97131434;     SkvYQUvnYr97131434 = SkvYQUvnYr77925071;     SkvYQUvnYr77925071 = SkvYQUvnYr66275271;     SkvYQUvnYr66275271 = SkvYQUvnYr5243130;     SkvYQUvnYr5243130 = SkvYQUvnYr58504470;     SkvYQUvnYr58504470 = SkvYQUvnYr64053050;     SkvYQUvnYr64053050 = SkvYQUvnYr58576083;     SkvYQUvnYr58576083 = SkvYQUvnYr72712504;     SkvYQUvnYr72712504 = SkvYQUvnYr87377129;     SkvYQUvnYr87377129 = SkvYQUvnYr77353975;     SkvYQUvnYr77353975 = SkvYQUvnYr30587906;     SkvYQUvnYr30587906 = SkvYQUvnYr18981524;     SkvYQUvnYr18981524 = SkvYQUvnYr80813870;     SkvYQUvnYr80813870 = SkvYQUvnYr32333093;     SkvYQUvnYr32333093 = SkvYQUvnYr15455474;     SkvYQUvnYr15455474 = SkvYQUvnYr66341450;     SkvYQUvnYr66341450 = SkvYQUvnYr84941820;     SkvYQUvnYr84941820 = SkvYQUvnYr52169708;     SkvYQUvnYr52169708 = SkvYQUvnYr20160855;     SkvYQUvnYr20160855 = SkvYQUvnYr72924144;     SkvYQUvnYr72924144 = SkvYQUvnYr88829918;     SkvYQUvnYr88829918 = SkvYQUvnYr97600015;     SkvYQUvnYr97600015 = SkvYQUvnYr52257753;     SkvYQUvnYr52257753 = SkvYQUvnYr66586368;     SkvYQUvnYr66586368 = SkvYQUvnYr63091966;     SkvYQUvnYr63091966 = SkvYQUvnYr64754876;     SkvYQUvnYr64754876 = SkvYQUvnYr80124038;     SkvYQUvnYr80124038 = SkvYQUvnYr89735182;     SkvYQUvnYr89735182 = SkvYQUvnYr12490128;     SkvYQUvnYr12490128 = SkvYQUvnYr79747896;     SkvYQUvnYr79747896 = SkvYQUvnYr6564302;     SkvYQUvnYr6564302 = SkvYQUvnYr17369062;     SkvYQUvnYr17369062 = SkvYQUvnYr16695717;     SkvYQUvnYr16695717 = SkvYQUvnYr85970737;     SkvYQUvnYr85970737 = SkvYQUvnYr40605262;     SkvYQUvnYr40605262 = SkvYQUvnYr27648999;     SkvYQUvnYr27648999 = SkvYQUvnYr79085019;     SkvYQUvnYr79085019 = SkvYQUvnYr93317830;     SkvYQUvnYr93317830 = SkvYQUvnYr97558304;     SkvYQUvnYr97558304 = SkvYQUvnYr55477264;     SkvYQUvnYr55477264 = SkvYQUvnYr11201486;     SkvYQUvnYr11201486 = SkvYQUvnYr37452609;     SkvYQUvnYr37452609 = SkvYQUvnYr35326436;     SkvYQUvnYr35326436 = SkvYQUvnYr13635703;     SkvYQUvnYr13635703 = SkvYQUvnYr33355678;     SkvYQUvnYr33355678 = SkvYQUvnYr14275061;     SkvYQUvnYr14275061 = SkvYQUvnYr1014724;     SkvYQUvnYr1014724 = SkvYQUvnYr15487240;     SkvYQUvnYr15487240 = SkvYQUvnYr81277737;     SkvYQUvnYr81277737 = SkvYQUvnYr44269214;     SkvYQUvnYr44269214 = SkvYQUvnYr16685490;     SkvYQUvnYr16685490 = SkvYQUvnYr45983193;     SkvYQUvnYr45983193 = SkvYQUvnYr36479786;     SkvYQUvnYr36479786 = SkvYQUvnYr67119206;     SkvYQUvnYr67119206 = SkvYQUvnYr73318435;     SkvYQUvnYr73318435 = SkvYQUvnYr99531419;     SkvYQUvnYr99531419 = SkvYQUvnYr25667318;     SkvYQUvnYr25667318 = SkvYQUvnYr99688903;     SkvYQUvnYr99688903 = SkvYQUvnYr42151164;     SkvYQUvnYr42151164 = SkvYQUvnYr93749594;     SkvYQUvnYr93749594 = SkvYQUvnYr83929011;     SkvYQUvnYr83929011 = SkvYQUvnYr68840901;     SkvYQUvnYr68840901 = SkvYQUvnYr60222376;     SkvYQUvnYr60222376 = SkvYQUvnYr7629234;     SkvYQUvnYr7629234 = SkvYQUvnYr70789674;     SkvYQUvnYr70789674 = SkvYQUvnYr13218845;     SkvYQUvnYr13218845 = SkvYQUvnYr2285807;     SkvYQUvnYr2285807 = SkvYQUvnYr94843133;     SkvYQUvnYr94843133 = SkvYQUvnYr91727831;     SkvYQUvnYr91727831 = SkvYQUvnYr87806475;     SkvYQUvnYr87806475 = SkvYQUvnYr87256431;     SkvYQUvnYr87256431 = SkvYQUvnYr91623990;     SkvYQUvnYr91623990 = SkvYQUvnYr54611403;     SkvYQUvnYr54611403 = SkvYQUvnYr64683591;     SkvYQUvnYr64683591 = SkvYQUvnYr61722658;     SkvYQUvnYr61722658 = SkvYQUvnYr51377310;     SkvYQUvnYr51377310 = SkvYQUvnYr62273579;     SkvYQUvnYr62273579 = SkvYQUvnYr38622051;     SkvYQUvnYr38622051 = SkvYQUvnYr33230691;     SkvYQUvnYr33230691 = SkvYQUvnYr48816906;     SkvYQUvnYr48816906 = SkvYQUvnYr63740152;     SkvYQUvnYr63740152 = SkvYQUvnYr64636799;     SkvYQUvnYr64636799 = SkvYQUvnYr98560959;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void jvDPJHeSYN82005265() {     int TUaIVJszYu20948078 = -541372060;    int TUaIVJszYu40410275 = -198112999;    int TUaIVJszYu48299592 = -545967382;    int TUaIVJszYu84784258 = -340015342;    int TUaIVJszYu8339367 = -701274973;    int TUaIVJszYu37586682 = -315266762;    int TUaIVJszYu83145416 = -682811986;    int TUaIVJszYu39577442 = 81215621;    int TUaIVJszYu30419716 = -862397551;    int TUaIVJszYu73592179 = -969384055;    int TUaIVJszYu85362192 = -433791072;    int TUaIVJszYu52636160 = -108849478;    int TUaIVJszYu77618844 = -831686543;    int TUaIVJszYu44242063 = 38257679;    int TUaIVJszYu34888178 = -877945152;    int TUaIVJszYu896554 = 23001685;    int TUaIVJszYu89866702 = -805973508;    int TUaIVJszYu68124770 = -130237525;    int TUaIVJszYu38413065 = 96779666;    int TUaIVJszYu56441354 = -84746966;    int TUaIVJszYu67158378 = -730133827;    int TUaIVJszYu69058348 = -476582436;    int TUaIVJszYu61797793 = -548939983;    int TUaIVJszYu90552836 = -188316361;    int TUaIVJszYu94116330 = -716469437;    int TUaIVJszYu61015498 = -654136077;    int TUaIVJszYu34466550 = -698224460;    int TUaIVJszYu83265425 = -547102073;    int TUaIVJszYu37480467 = -934420882;    int TUaIVJszYu28138703 = 37202074;    int TUaIVJszYu25104676 = -350275813;    int TUaIVJszYu89874284 = -625394516;    int TUaIVJszYu96494144 = -99239633;    int TUaIVJszYu25730138 = -11912509;    int TUaIVJszYu61171185 = -399538417;    int TUaIVJszYu93862577 = -303379298;    int TUaIVJszYu13600567 = -242712848;    int TUaIVJszYu26550245 = -755331248;    int TUaIVJszYu37138050 = 54579227;    int TUaIVJszYu17785730 = -556421772;    int TUaIVJszYu81015124 = -599078519;    int TUaIVJszYu55094759 = -200714807;    int TUaIVJszYu68642158 = 56542156;    int TUaIVJszYu64198995 = -910900187;    int TUaIVJszYu48107305 = -364112913;    int TUaIVJszYu97835464 = -623399478;    int TUaIVJszYu24787526 = -553921531;    int TUaIVJszYu24346571 = -543552191;    int TUaIVJszYu1024207 = -134098852;    int TUaIVJszYu14452807 = -761680114;    int TUaIVJszYu25564934 = -301540035;    int TUaIVJszYu59821994 = -186786854;    int TUaIVJszYu18919665 = -266807060;    int TUaIVJszYu87935544 = -642426622;    int TUaIVJszYu14846887 = -691052811;    int TUaIVJszYu51889729 = 35210376;    int TUaIVJszYu78612482 = -649173016;    int TUaIVJszYu57746756 = -257651021;    int TUaIVJszYu90667927 = -623545906;    int TUaIVJszYu47323868 = 52861104;    int TUaIVJszYu3120133 = -617042303;    int TUaIVJszYu99879991 = -35709914;    int TUaIVJszYu2096976 = 15636503;    int TUaIVJszYu2281013 = -799599625;    int TUaIVJszYu48487503 = -519108242;    int TUaIVJszYu95487908 = -808396556;    int TUaIVJszYu56142015 = 90390154;    int TUaIVJszYu51888707 = -719774034;    int TUaIVJszYu83070877 = -562203904;    int TUaIVJszYu41025601 = -474565854;    int TUaIVJszYu87295986 = -734285468;    int TUaIVJszYu63316457 = 49357740;    int TUaIVJszYu30986721 = -84816753;    int TUaIVJszYu20627336 = -346798563;    int TUaIVJszYu75426230 = -485668447;    int TUaIVJszYu12063619 = -429419021;    int TUaIVJszYu416191 = -433124593;    int TUaIVJszYu97598798 = -638039797;    int TUaIVJszYu42445532 = -824203449;    int TUaIVJszYu96280865 = 6930041;    int TUaIVJszYu36227973 = -214546;    int TUaIVJszYu10119980 = -54672269;    int TUaIVJszYu82241219 = -313003221;    int TUaIVJszYu23027660 = -72740769;    int TUaIVJszYu2573770 = -661257892;    int TUaIVJszYu65282681 = -63488960;    int TUaIVJszYu70954619 = -258587457;    int TUaIVJszYu8558601 = -456813012;    int TUaIVJszYu10883251 = -320859699;    int TUaIVJszYu9281456 = -334748794;    int TUaIVJszYu15250095 = -654206283;    int TUaIVJszYu55853811 = -985061827;    int TUaIVJszYu35882317 = -31785343;    int TUaIVJszYu89814181 = -998281878;    int TUaIVJszYu14665598 = -939379469;    int TUaIVJszYu81135132 = -463368606;    int TUaIVJszYu52997784 = -116351310;    int TUaIVJszYu66361146 = -143858219;    int TUaIVJszYu15711492 = -291791945;    int TUaIVJszYu52619397 = -541372060;     TUaIVJszYu20948078 = TUaIVJszYu40410275;     TUaIVJszYu40410275 = TUaIVJszYu48299592;     TUaIVJszYu48299592 = TUaIVJszYu84784258;     TUaIVJszYu84784258 = TUaIVJszYu8339367;     TUaIVJszYu8339367 = TUaIVJszYu37586682;     TUaIVJszYu37586682 = TUaIVJszYu83145416;     TUaIVJszYu83145416 = TUaIVJszYu39577442;     TUaIVJszYu39577442 = TUaIVJszYu30419716;     TUaIVJszYu30419716 = TUaIVJszYu73592179;     TUaIVJszYu73592179 = TUaIVJszYu85362192;     TUaIVJszYu85362192 = TUaIVJszYu52636160;     TUaIVJszYu52636160 = TUaIVJszYu77618844;     TUaIVJszYu77618844 = TUaIVJszYu44242063;     TUaIVJszYu44242063 = TUaIVJszYu34888178;     TUaIVJszYu34888178 = TUaIVJszYu896554;     TUaIVJszYu896554 = TUaIVJszYu89866702;     TUaIVJszYu89866702 = TUaIVJszYu68124770;     TUaIVJszYu68124770 = TUaIVJszYu38413065;     TUaIVJszYu38413065 = TUaIVJszYu56441354;     TUaIVJszYu56441354 = TUaIVJszYu67158378;     TUaIVJszYu67158378 = TUaIVJszYu69058348;     TUaIVJszYu69058348 = TUaIVJszYu61797793;     TUaIVJszYu61797793 = TUaIVJszYu90552836;     TUaIVJszYu90552836 = TUaIVJszYu94116330;     TUaIVJszYu94116330 = TUaIVJszYu61015498;     TUaIVJszYu61015498 = TUaIVJszYu34466550;     TUaIVJszYu34466550 = TUaIVJszYu83265425;     TUaIVJszYu83265425 = TUaIVJszYu37480467;     TUaIVJszYu37480467 = TUaIVJszYu28138703;     TUaIVJszYu28138703 = TUaIVJszYu25104676;     TUaIVJszYu25104676 = TUaIVJszYu89874284;     TUaIVJszYu89874284 = TUaIVJszYu96494144;     TUaIVJszYu96494144 = TUaIVJszYu25730138;     TUaIVJszYu25730138 = TUaIVJszYu61171185;     TUaIVJszYu61171185 = TUaIVJszYu93862577;     TUaIVJszYu93862577 = TUaIVJszYu13600567;     TUaIVJszYu13600567 = TUaIVJszYu26550245;     TUaIVJszYu26550245 = TUaIVJszYu37138050;     TUaIVJszYu37138050 = TUaIVJszYu17785730;     TUaIVJszYu17785730 = TUaIVJszYu81015124;     TUaIVJszYu81015124 = TUaIVJszYu55094759;     TUaIVJszYu55094759 = TUaIVJszYu68642158;     TUaIVJszYu68642158 = TUaIVJszYu64198995;     TUaIVJszYu64198995 = TUaIVJszYu48107305;     TUaIVJszYu48107305 = TUaIVJszYu97835464;     TUaIVJszYu97835464 = TUaIVJszYu24787526;     TUaIVJszYu24787526 = TUaIVJszYu24346571;     TUaIVJszYu24346571 = TUaIVJszYu1024207;     TUaIVJszYu1024207 = TUaIVJszYu14452807;     TUaIVJszYu14452807 = TUaIVJszYu25564934;     TUaIVJszYu25564934 = TUaIVJszYu59821994;     TUaIVJszYu59821994 = TUaIVJszYu18919665;     TUaIVJszYu18919665 = TUaIVJszYu87935544;     TUaIVJszYu87935544 = TUaIVJszYu14846887;     TUaIVJszYu14846887 = TUaIVJszYu51889729;     TUaIVJszYu51889729 = TUaIVJszYu78612482;     TUaIVJszYu78612482 = TUaIVJszYu57746756;     TUaIVJszYu57746756 = TUaIVJszYu90667927;     TUaIVJszYu90667927 = TUaIVJszYu47323868;     TUaIVJszYu47323868 = TUaIVJszYu3120133;     TUaIVJszYu3120133 = TUaIVJszYu99879991;     TUaIVJszYu99879991 = TUaIVJszYu2096976;     TUaIVJszYu2096976 = TUaIVJszYu2281013;     TUaIVJszYu2281013 = TUaIVJszYu48487503;     TUaIVJszYu48487503 = TUaIVJszYu95487908;     TUaIVJszYu95487908 = TUaIVJszYu56142015;     TUaIVJszYu56142015 = TUaIVJszYu51888707;     TUaIVJszYu51888707 = TUaIVJszYu83070877;     TUaIVJszYu83070877 = TUaIVJszYu41025601;     TUaIVJszYu41025601 = TUaIVJszYu87295986;     TUaIVJszYu87295986 = TUaIVJszYu63316457;     TUaIVJszYu63316457 = TUaIVJszYu30986721;     TUaIVJszYu30986721 = TUaIVJszYu20627336;     TUaIVJszYu20627336 = TUaIVJszYu75426230;     TUaIVJszYu75426230 = TUaIVJszYu12063619;     TUaIVJszYu12063619 = TUaIVJszYu416191;     TUaIVJszYu416191 = TUaIVJszYu97598798;     TUaIVJszYu97598798 = TUaIVJszYu42445532;     TUaIVJszYu42445532 = TUaIVJszYu96280865;     TUaIVJszYu96280865 = TUaIVJszYu36227973;     TUaIVJszYu36227973 = TUaIVJszYu10119980;     TUaIVJszYu10119980 = TUaIVJszYu82241219;     TUaIVJszYu82241219 = TUaIVJszYu23027660;     TUaIVJszYu23027660 = TUaIVJszYu2573770;     TUaIVJszYu2573770 = TUaIVJszYu65282681;     TUaIVJszYu65282681 = TUaIVJszYu70954619;     TUaIVJszYu70954619 = TUaIVJszYu8558601;     TUaIVJszYu8558601 = TUaIVJszYu10883251;     TUaIVJszYu10883251 = TUaIVJszYu9281456;     TUaIVJszYu9281456 = TUaIVJszYu15250095;     TUaIVJszYu15250095 = TUaIVJszYu55853811;     TUaIVJszYu55853811 = TUaIVJszYu35882317;     TUaIVJszYu35882317 = TUaIVJszYu89814181;     TUaIVJszYu89814181 = TUaIVJszYu14665598;     TUaIVJszYu14665598 = TUaIVJszYu81135132;     TUaIVJszYu81135132 = TUaIVJszYu52997784;     TUaIVJszYu52997784 = TUaIVJszYu66361146;     TUaIVJszYu66361146 = TUaIVJszYu15711492;     TUaIVJszYu15711492 = TUaIVJszYu52619397;     TUaIVJszYu52619397 = TUaIVJszYu20948078;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void SAGaOzDAik18650240() {     int DbqQhrfioV43335195 = -175776023;    int DbqQhrfioV24757776 = -146656053;    int DbqQhrfioV77068871 = -785621226;    int DbqQhrfioV99790946 = -346422874;    int DbqQhrfioV6513621 = -192331974;    int DbqQhrfioV52469799 = -175875140;    int DbqQhrfioV75301155 = -504799292;    int DbqQhrfioV15211301 = -389381023;    int DbqQhrfioV27582847 = -746593590;    int DbqQhrfioV65355763 = -712125388;    int DbqQhrfioV22904052 = -430415473;    int DbqQhrfioV8539109 = -359594772;    int DbqQhrfioV44627025 = -350403863;    int DbqQhrfioV86856815 = -489451078;    int DbqQhrfioV71623455 = -505422865;    int DbqQhrfioV45152466 = -358665858;    int DbqQhrfioV39690054 = 16746673;    int DbqQhrfioV74101186 = -581980216;    int DbqQhrfioV79694696 = -38322459;    int DbqQhrfioV34957638 = -337381419;    int DbqQhrfioV68041485 = -721621136;    int DbqQhrfioV32873567 = -981348750;    int DbqQhrfioV65091116 = -725020576;    int DbqQhrfioV17052624 = -752309219;    int DbqQhrfioV29656577 = -338028561;    int DbqQhrfioV49318493 = 1969699;    int DbqQhrfioV81555970 = -455810952;    int DbqQhrfioV89176875 = -335556526;    int DbqQhrfioV44373027 = -178678001;    int DbqQhrfioV37295883 = -983093793;    int DbqQhrfioV69395481 = -431591965;    int DbqQhrfioV47415475 = -177465871;    int DbqQhrfioV77532815 = -628583128;    int DbqQhrfioV85118825 = -336039359;    int DbqQhrfioV37400550 = -463294840;    int DbqQhrfioV35555446 = -618774675;    int DbqQhrfioV7040280 = 24339201;    int DbqQhrfioV80176345 = -622956720;    int DbqQhrfioV85446181 = -346466805;    int DbqQhrfioV37971445 = -692645801;    int DbqQhrfioV9772495 = -2256185;    int DbqQhrfioV43603151 = 41376502;    int DbqQhrfioV74192350 = -930988427;    int DbqQhrfioV63643114 = -908263836;    int DbqQhrfioV16090572 = -173256390;    int DbqQhrfioV5935747 = -805628573;    int DbqQhrfioV37084923 = -32331784;    int DbqQhrfioV68945245 = -300087182;    int DbqQhrfioV95484111 = -83815551;    int DbqQhrfioV11536553 = -903700373;    int DbqQhrfioV34434150 = -656616312;    int DbqQhrfioV33673252 = -617606136;    int DbqQhrfioV97234067 = 61281710;    int DbqQhrfioV48222089 = -918107431;    int DbqQhrfioV50608754 = -831756926;    int DbqQhrfioV10461628 = -194427274;    int DbqQhrfioV59666659 = -421635478;    int DbqQhrfioV60016247 = 66687992;    int DbqQhrfioV70134369 = 91605687;    int DbqQhrfioV57195128 = -94301673;    int DbqQhrfioV70913829 = -720064188;    int DbqQhrfioV86124280 = -69242766;    int DbqQhrfioV70838273 = -110703022;    int DbqQhrfioV90286963 = -763499798;    int DbqQhrfioV95960282 = -180533423;    int DbqQhrfioV75488576 = -152949602;    int DbqQhrfioV31006294 = -731011644;    int DbqQhrfioV59508200 = 85635496;    int DbqQhrfioV49456265 = 73843761;    int DbqQhrfioV36068009 = -886648191;    int DbqQhrfioV38112187 = -283005059;    int DbqQhrfioV59513708 = -360296608;    int DbqQhrfioV88655005 = -135513412;    int DbqQhrfioV41723252 = -345676658;    int DbqQhrfioV25185143 = -235125235;    int DbqQhrfioV24438335 = -662997638;    int DbqQhrfioV58681216 = 49639677;    int DbqQhrfioV1448003 = -816756740;    int DbqQhrfioV962053 = -479052829;    int DbqQhrfioV23720831 = -532399989;    int DbqQhrfioV12233571 = -965698517;    int DbqQhrfioV12610725 = -55723771;    int DbqQhrfioV93692764 = -151740976;    int DbqQhrfioV32836475 = -274977629;    int DbqQhrfioV2861733 = -226477482;    int DbqQhrfioV35722230 = -813985830;    int DbqQhrfioV50181407 = -138747581;    int DbqQhrfioV29310726 = -710475698;    int DbqQhrfioV34510071 = -504282434;    int DbqQhrfioV26938922 = -168867566;    int DbqQhrfioV75888787 = -97139197;    int DbqQhrfioV47024032 = 57651208;    int DbqQhrfioV10041977 = -614562407;    int DbqQhrfioV28251053 = -152165132;    int DbqQhrfioV67057615 = -972581613;    int DbqQhrfioV23648215 = -933013419;    int DbqQhrfioV72764877 = -847920476;    int DbqQhrfioV83905386 = -67488630;    int DbqQhrfioV67682832 = -627730413;    int DbqQhrfioV40601995 = -175776023;     DbqQhrfioV43335195 = DbqQhrfioV24757776;     DbqQhrfioV24757776 = DbqQhrfioV77068871;     DbqQhrfioV77068871 = DbqQhrfioV99790946;     DbqQhrfioV99790946 = DbqQhrfioV6513621;     DbqQhrfioV6513621 = DbqQhrfioV52469799;     DbqQhrfioV52469799 = DbqQhrfioV75301155;     DbqQhrfioV75301155 = DbqQhrfioV15211301;     DbqQhrfioV15211301 = DbqQhrfioV27582847;     DbqQhrfioV27582847 = DbqQhrfioV65355763;     DbqQhrfioV65355763 = DbqQhrfioV22904052;     DbqQhrfioV22904052 = DbqQhrfioV8539109;     DbqQhrfioV8539109 = DbqQhrfioV44627025;     DbqQhrfioV44627025 = DbqQhrfioV86856815;     DbqQhrfioV86856815 = DbqQhrfioV71623455;     DbqQhrfioV71623455 = DbqQhrfioV45152466;     DbqQhrfioV45152466 = DbqQhrfioV39690054;     DbqQhrfioV39690054 = DbqQhrfioV74101186;     DbqQhrfioV74101186 = DbqQhrfioV79694696;     DbqQhrfioV79694696 = DbqQhrfioV34957638;     DbqQhrfioV34957638 = DbqQhrfioV68041485;     DbqQhrfioV68041485 = DbqQhrfioV32873567;     DbqQhrfioV32873567 = DbqQhrfioV65091116;     DbqQhrfioV65091116 = DbqQhrfioV17052624;     DbqQhrfioV17052624 = DbqQhrfioV29656577;     DbqQhrfioV29656577 = DbqQhrfioV49318493;     DbqQhrfioV49318493 = DbqQhrfioV81555970;     DbqQhrfioV81555970 = DbqQhrfioV89176875;     DbqQhrfioV89176875 = DbqQhrfioV44373027;     DbqQhrfioV44373027 = DbqQhrfioV37295883;     DbqQhrfioV37295883 = DbqQhrfioV69395481;     DbqQhrfioV69395481 = DbqQhrfioV47415475;     DbqQhrfioV47415475 = DbqQhrfioV77532815;     DbqQhrfioV77532815 = DbqQhrfioV85118825;     DbqQhrfioV85118825 = DbqQhrfioV37400550;     DbqQhrfioV37400550 = DbqQhrfioV35555446;     DbqQhrfioV35555446 = DbqQhrfioV7040280;     DbqQhrfioV7040280 = DbqQhrfioV80176345;     DbqQhrfioV80176345 = DbqQhrfioV85446181;     DbqQhrfioV85446181 = DbqQhrfioV37971445;     DbqQhrfioV37971445 = DbqQhrfioV9772495;     DbqQhrfioV9772495 = DbqQhrfioV43603151;     DbqQhrfioV43603151 = DbqQhrfioV74192350;     DbqQhrfioV74192350 = DbqQhrfioV63643114;     DbqQhrfioV63643114 = DbqQhrfioV16090572;     DbqQhrfioV16090572 = DbqQhrfioV5935747;     DbqQhrfioV5935747 = DbqQhrfioV37084923;     DbqQhrfioV37084923 = DbqQhrfioV68945245;     DbqQhrfioV68945245 = DbqQhrfioV95484111;     DbqQhrfioV95484111 = DbqQhrfioV11536553;     DbqQhrfioV11536553 = DbqQhrfioV34434150;     DbqQhrfioV34434150 = DbqQhrfioV33673252;     DbqQhrfioV33673252 = DbqQhrfioV97234067;     DbqQhrfioV97234067 = DbqQhrfioV48222089;     DbqQhrfioV48222089 = DbqQhrfioV50608754;     DbqQhrfioV50608754 = DbqQhrfioV10461628;     DbqQhrfioV10461628 = DbqQhrfioV59666659;     DbqQhrfioV59666659 = DbqQhrfioV60016247;     DbqQhrfioV60016247 = DbqQhrfioV70134369;     DbqQhrfioV70134369 = DbqQhrfioV57195128;     DbqQhrfioV57195128 = DbqQhrfioV70913829;     DbqQhrfioV70913829 = DbqQhrfioV86124280;     DbqQhrfioV86124280 = DbqQhrfioV70838273;     DbqQhrfioV70838273 = DbqQhrfioV90286963;     DbqQhrfioV90286963 = DbqQhrfioV95960282;     DbqQhrfioV95960282 = DbqQhrfioV75488576;     DbqQhrfioV75488576 = DbqQhrfioV31006294;     DbqQhrfioV31006294 = DbqQhrfioV59508200;     DbqQhrfioV59508200 = DbqQhrfioV49456265;     DbqQhrfioV49456265 = DbqQhrfioV36068009;     DbqQhrfioV36068009 = DbqQhrfioV38112187;     DbqQhrfioV38112187 = DbqQhrfioV59513708;     DbqQhrfioV59513708 = DbqQhrfioV88655005;     DbqQhrfioV88655005 = DbqQhrfioV41723252;     DbqQhrfioV41723252 = DbqQhrfioV25185143;     DbqQhrfioV25185143 = DbqQhrfioV24438335;     DbqQhrfioV24438335 = DbqQhrfioV58681216;     DbqQhrfioV58681216 = DbqQhrfioV1448003;     DbqQhrfioV1448003 = DbqQhrfioV962053;     DbqQhrfioV962053 = DbqQhrfioV23720831;     DbqQhrfioV23720831 = DbqQhrfioV12233571;     DbqQhrfioV12233571 = DbqQhrfioV12610725;     DbqQhrfioV12610725 = DbqQhrfioV93692764;     DbqQhrfioV93692764 = DbqQhrfioV32836475;     DbqQhrfioV32836475 = DbqQhrfioV2861733;     DbqQhrfioV2861733 = DbqQhrfioV35722230;     DbqQhrfioV35722230 = DbqQhrfioV50181407;     DbqQhrfioV50181407 = DbqQhrfioV29310726;     DbqQhrfioV29310726 = DbqQhrfioV34510071;     DbqQhrfioV34510071 = DbqQhrfioV26938922;     DbqQhrfioV26938922 = DbqQhrfioV75888787;     DbqQhrfioV75888787 = DbqQhrfioV47024032;     DbqQhrfioV47024032 = DbqQhrfioV10041977;     DbqQhrfioV10041977 = DbqQhrfioV28251053;     DbqQhrfioV28251053 = DbqQhrfioV67057615;     DbqQhrfioV67057615 = DbqQhrfioV23648215;     DbqQhrfioV23648215 = DbqQhrfioV72764877;     DbqQhrfioV72764877 = DbqQhrfioV83905386;     DbqQhrfioV83905386 = DbqQhrfioV67682832;     DbqQhrfioV67682832 = DbqQhrfioV40601995;     DbqQhrfioV40601995 = DbqQhrfioV43335195;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void SmPPQyAPiu55295214() {     int FyjGGxGiFe65722312 = -910179986;    int FyjGGxGiFe9105276 = -95199108;    int FyjGGxGiFe5838150 = 74724930;    int FyjGGxGiFe14797635 = -352830406;    int FyjGGxGiFe4687876 = -783388975;    int FyjGGxGiFe67352916 = -36483518;    int FyjGGxGiFe67456894 = -326786598;    int FyjGGxGiFe90845159 = -859977668;    int FyjGGxGiFe24745978 = -630789629;    int FyjGGxGiFe57119348 = -454866721;    int FyjGGxGiFe60445911 = -427039873;    int FyjGGxGiFe64442058 = -610340065;    int FyjGGxGiFe11635206 = -969121183;    int FyjGGxGiFe29471568 = 82840165;    int FyjGGxGiFe8358732 = -132900578;    int FyjGGxGiFe89408378 = -740333400;    int FyjGGxGiFe89513405 = -260533147;    int FyjGGxGiFe80077603 = 66277093;    int FyjGGxGiFe20976327 = -173424584;    int FyjGGxGiFe13473921 = -590015873;    int FyjGGxGiFe68924592 = -713108444;    int FyjGGxGiFe96688785 = -386115063;    int FyjGGxGiFe68384439 = -901101168;    int FyjGGxGiFe43552411 = -216302076;    int FyjGGxGiFe65196824 = 40412314;    int FyjGGxGiFe37621488 = -441924525;    int FyjGGxGiFe28645391 = -213397445;    int FyjGGxGiFe95088325 = -124010980;    int FyjGGxGiFe51265588 = -522935121;    int FyjGGxGiFe46453062 = -903389659;    int FyjGGxGiFe13686287 = -512908117;    int FyjGGxGiFe4956666 = -829537226;    int FyjGGxGiFe58571485 = -57926623;    int FyjGGxGiFe44507512 = -660166209;    int FyjGGxGiFe13629915 = -527051262;    int FyjGGxGiFe77248315 = -934170051;    int FyjGGxGiFe479992 = -808608751;    int FyjGGxGiFe33802447 = -490582192;    int FyjGGxGiFe33754313 = -747512837;    int FyjGGxGiFe58157159 = -828869830;    int FyjGGxGiFe38529866 = -505433851;    int FyjGGxGiFe32111542 = -816532189;    int FyjGGxGiFe79742542 = -818519011;    int FyjGGxGiFe63087233 = -905627485;    int FyjGGxGiFe84073837 = 17600132;    int FyjGGxGiFe14036029 = -987857669;    int FyjGGxGiFe49382320 = -610742037;    int FyjGGxGiFe13543920 = -56622173;    int FyjGGxGiFe89944016 = -33532249;    int FyjGGxGiFe8620299 = 54279368;    int FyjGGxGiFe43303367 = 88307411;    int FyjGGxGiFe7524509 = 51574582;    int FyjGGxGiFe75548471 = -710629521;    int FyjGGxGiFe8508635 = -93788240;    int FyjGGxGiFe86370621 = -972461041;    int FyjGGxGiFe69033527 = -424064924;    int FyjGGxGiFe40720837 = -194097941;    int FyjGGxGiFe62285738 = -708972995;    int FyjGGxGiFe49600811 = -293242720;    int FyjGGxGiFe67066387 = -241464451;    int FyjGGxGiFe38707526 = -823086073;    int FyjGGxGiFe72368569 = -102775619;    int FyjGGxGiFe39579571 = -237042548;    int FyjGGxGiFe78292915 = -727399970;    int FyjGGxGiFe43433062 = -941958604;    int FyjGGxGiFe55489245 = -597502648;    int FyjGGxGiFe5870573 = -452413443;    int FyjGGxGiFe67127693 = -208954974;    int FyjGGxGiFe15841653 = -390108573;    int FyjGGxGiFe31110417 = -198730528;    int FyjGGxGiFe88928387 = -931724650;    int FyjGGxGiFe55710959 = -769950956;    int FyjGGxGiFe46323290 = -186210070;    int FyjGGxGiFe62819168 = -344554754;    int FyjGGxGiFe74944055 = 15417978;    int FyjGGxGiFe36813051 = -896576256;    int FyjGGxGiFe16946243 = -567596052;    int FyjGGxGiFe5297206 = -995473683;    int FyjGGxGiFe59478573 = -133902208;    int FyjGGxGiFe51160796 = 28269982;    int FyjGGxGiFe88239167 = -831182488;    int FyjGGxGiFe15101471 = -56775273;    int FyjGGxGiFe5144310 = 9521269;    int FyjGGxGiFe42645290 = -477214489;    int FyjGGxGiFe3149696 = -891697071;    int FyjGGxGiFe6161778 = -464482700;    int FyjGGxGiFe29408195 = -18907706;    int FyjGGxGiFe50062851 = -964138384;    int FyjGGxGiFe58136890 = -687705169;    int FyjGGxGiFe44596387 = -2986339;    int FyjGGxGiFe36527479 = -640072111;    int FyjGGxGiFe38194253 = 364243;    int FyjGGxGiFe84201635 = -97339472;    int FyjGGxGiFe66687925 = -406048387;    int FyjGGxGiFe19449634 = 94216243;    int FyjGGxGiFe66161297 = -302658233;    int FyjGGxGiFe92531970 = -479489642;    int FyjGGxGiFe1449627 = 8880959;    int FyjGGxGiFe19654172 = -963668881;    int FyjGGxGiFe28584593 = -910179986;     FyjGGxGiFe65722312 = FyjGGxGiFe9105276;     FyjGGxGiFe9105276 = FyjGGxGiFe5838150;     FyjGGxGiFe5838150 = FyjGGxGiFe14797635;     FyjGGxGiFe14797635 = FyjGGxGiFe4687876;     FyjGGxGiFe4687876 = FyjGGxGiFe67352916;     FyjGGxGiFe67352916 = FyjGGxGiFe67456894;     FyjGGxGiFe67456894 = FyjGGxGiFe90845159;     FyjGGxGiFe90845159 = FyjGGxGiFe24745978;     FyjGGxGiFe24745978 = FyjGGxGiFe57119348;     FyjGGxGiFe57119348 = FyjGGxGiFe60445911;     FyjGGxGiFe60445911 = FyjGGxGiFe64442058;     FyjGGxGiFe64442058 = FyjGGxGiFe11635206;     FyjGGxGiFe11635206 = FyjGGxGiFe29471568;     FyjGGxGiFe29471568 = FyjGGxGiFe8358732;     FyjGGxGiFe8358732 = FyjGGxGiFe89408378;     FyjGGxGiFe89408378 = FyjGGxGiFe89513405;     FyjGGxGiFe89513405 = FyjGGxGiFe80077603;     FyjGGxGiFe80077603 = FyjGGxGiFe20976327;     FyjGGxGiFe20976327 = FyjGGxGiFe13473921;     FyjGGxGiFe13473921 = FyjGGxGiFe68924592;     FyjGGxGiFe68924592 = FyjGGxGiFe96688785;     FyjGGxGiFe96688785 = FyjGGxGiFe68384439;     FyjGGxGiFe68384439 = FyjGGxGiFe43552411;     FyjGGxGiFe43552411 = FyjGGxGiFe65196824;     FyjGGxGiFe65196824 = FyjGGxGiFe37621488;     FyjGGxGiFe37621488 = FyjGGxGiFe28645391;     FyjGGxGiFe28645391 = FyjGGxGiFe95088325;     FyjGGxGiFe95088325 = FyjGGxGiFe51265588;     FyjGGxGiFe51265588 = FyjGGxGiFe46453062;     FyjGGxGiFe46453062 = FyjGGxGiFe13686287;     FyjGGxGiFe13686287 = FyjGGxGiFe4956666;     FyjGGxGiFe4956666 = FyjGGxGiFe58571485;     FyjGGxGiFe58571485 = FyjGGxGiFe44507512;     FyjGGxGiFe44507512 = FyjGGxGiFe13629915;     FyjGGxGiFe13629915 = FyjGGxGiFe77248315;     FyjGGxGiFe77248315 = FyjGGxGiFe479992;     FyjGGxGiFe479992 = FyjGGxGiFe33802447;     FyjGGxGiFe33802447 = FyjGGxGiFe33754313;     FyjGGxGiFe33754313 = FyjGGxGiFe58157159;     FyjGGxGiFe58157159 = FyjGGxGiFe38529866;     FyjGGxGiFe38529866 = FyjGGxGiFe32111542;     FyjGGxGiFe32111542 = FyjGGxGiFe79742542;     FyjGGxGiFe79742542 = FyjGGxGiFe63087233;     FyjGGxGiFe63087233 = FyjGGxGiFe84073837;     FyjGGxGiFe84073837 = FyjGGxGiFe14036029;     FyjGGxGiFe14036029 = FyjGGxGiFe49382320;     FyjGGxGiFe49382320 = FyjGGxGiFe13543920;     FyjGGxGiFe13543920 = FyjGGxGiFe89944016;     FyjGGxGiFe89944016 = FyjGGxGiFe8620299;     FyjGGxGiFe8620299 = FyjGGxGiFe43303367;     FyjGGxGiFe43303367 = FyjGGxGiFe7524509;     FyjGGxGiFe7524509 = FyjGGxGiFe75548471;     FyjGGxGiFe75548471 = FyjGGxGiFe8508635;     FyjGGxGiFe8508635 = FyjGGxGiFe86370621;     FyjGGxGiFe86370621 = FyjGGxGiFe69033527;     FyjGGxGiFe69033527 = FyjGGxGiFe40720837;     FyjGGxGiFe40720837 = FyjGGxGiFe62285738;     FyjGGxGiFe62285738 = FyjGGxGiFe49600811;     FyjGGxGiFe49600811 = FyjGGxGiFe67066387;     FyjGGxGiFe67066387 = FyjGGxGiFe38707526;     FyjGGxGiFe38707526 = FyjGGxGiFe72368569;     FyjGGxGiFe72368569 = FyjGGxGiFe39579571;     FyjGGxGiFe39579571 = FyjGGxGiFe78292915;     FyjGGxGiFe78292915 = FyjGGxGiFe43433062;     FyjGGxGiFe43433062 = FyjGGxGiFe55489245;     FyjGGxGiFe55489245 = FyjGGxGiFe5870573;     FyjGGxGiFe5870573 = FyjGGxGiFe67127693;     FyjGGxGiFe67127693 = FyjGGxGiFe15841653;     FyjGGxGiFe15841653 = FyjGGxGiFe31110417;     FyjGGxGiFe31110417 = FyjGGxGiFe88928387;     FyjGGxGiFe88928387 = FyjGGxGiFe55710959;     FyjGGxGiFe55710959 = FyjGGxGiFe46323290;     FyjGGxGiFe46323290 = FyjGGxGiFe62819168;     FyjGGxGiFe62819168 = FyjGGxGiFe74944055;     FyjGGxGiFe74944055 = FyjGGxGiFe36813051;     FyjGGxGiFe36813051 = FyjGGxGiFe16946243;     FyjGGxGiFe16946243 = FyjGGxGiFe5297206;     FyjGGxGiFe5297206 = FyjGGxGiFe59478573;     FyjGGxGiFe59478573 = FyjGGxGiFe51160796;     FyjGGxGiFe51160796 = FyjGGxGiFe88239167;     FyjGGxGiFe88239167 = FyjGGxGiFe15101471;     FyjGGxGiFe15101471 = FyjGGxGiFe5144310;     FyjGGxGiFe5144310 = FyjGGxGiFe42645290;     FyjGGxGiFe42645290 = FyjGGxGiFe3149696;     FyjGGxGiFe3149696 = FyjGGxGiFe6161778;     FyjGGxGiFe6161778 = FyjGGxGiFe29408195;     FyjGGxGiFe29408195 = FyjGGxGiFe50062851;     FyjGGxGiFe50062851 = FyjGGxGiFe58136890;     FyjGGxGiFe58136890 = FyjGGxGiFe44596387;     FyjGGxGiFe44596387 = FyjGGxGiFe36527479;     FyjGGxGiFe36527479 = FyjGGxGiFe38194253;     FyjGGxGiFe38194253 = FyjGGxGiFe84201635;     FyjGGxGiFe84201635 = FyjGGxGiFe66687925;     FyjGGxGiFe66687925 = FyjGGxGiFe19449634;     FyjGGxGiFe19449634 = FyjGGxGiFe66161297;     FyjGGxGiFe66161297 = FyjGGxGiFe92531970;     FyjGGxGiFe92531970 = FyjGGxGiFe1449627;     FyjGGxGiFe1449627 = FyjGGxGiFe19654172;     FyjGGxGiFe19654172 = FyjGGxGiFe28584593;     FyjGGxGiFe28584593 = FyjGGxGiFe65722312;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void nUXeQwBNut91940188() {     int sIdcbVPRAU88109429 = -544583949;    int sIdcbVPRAU93452776 = -43742163;    int sIdcbVPRAU34607429 = -164928914;    int sIdcbVPRAU29804323 = -359237937;    int sIdcbVPRAU2862130 = -274445976;    int sIdcbVPRAU82236033 = -997091896;    int sIdcbVPRAU59612633 = -148773904;    int sIdcbVPRAU66479018 = -230574312;    int sIdcbVPRAU21909109 = -514985668;    int sIdcbVPRAU48882933 = -197608054;    int sIdcbVPRAU97987771 = -423664274;    int sIdcbVPRAU20345007 = -861085358;    int sIdcbVPRAU78643386 = -487838502;    int sIdcbVPRAU72086319 = -444868592;    int sIdcbVPRAU45094009 = -860378291;    int sIdcbVPRAU33664291 = -22000942;    int sIdcbVPRAU39336757 = -537812967;    int sIdcbVPRAU86054019 = -385465598;    int sIdcbVPRAU62257958 = -308526709;    int sIdcbVPRAU91990204 = -842650326;    int sIdcbVPRAU69807700 = -704595753;    int sIdcbVPRAU60504003 = -890881376;    int sIdcbVPRAU71677762 = 22818240;    int sIdcbVPRAU70052198 = -780294933;    int sIdcbVPRAU737071 = -681146810;    int sIdcbVPRAU25924482 = -885818749;    int sIdcbVPRAU75734811 = 29016062;    int sIdcbVPRAU999776 = 87534567;    int sIdcbVPRAU58158149 = -867192240;    int sIdcbVPRAU55610242 = -823685526;    int sIdcbVPRAU57977091 = -594224269;    int sIdcbVPRAU62497857 = -381608580;    int sIdcbVPRAU39610156 = -587270118;    int sIdcbVPRAU3896200 = -984293060;    int sIdcbVPRAU89859279 = -590807685;    int sIdcbVPRAU18941185 = -149565428;    int sIdcbVPRAU93919703 = -541556702;    int sIdcbVPRAU87428547 = -358207664;    int sIdcbVPRAU82062443 = -48558869;    int sIdcbVPRAU78342873 = -965093860;    int sIdcbVPRAU67287236 = 91388483;    int sIdcbVPRAU20619933 = -574440880;    int sIdcbVPRAU85292734 = -706049595;    int sIdcbVPRAU62531353 = -902991134;    int sIdcbVPRAU52057104 = -891543346;    int sIdcbVPRAU22136311 = -70086764;    int sIdcbVPRAU61679717 = -89152290;    int sIdcbVPRAU58142594 = -913157164;    int sIdcbVPRAU84403921 = 16751053;    int sIdcbVPRAU5704044 = -87740891;    int sIdcbVPRAU52172583 = -266768866;    int sIdcbVPRAU81375766 = -379244700;    int sIdcbVPRAU53862874 = -382540751;    int sIdcbVPRAU68795179 = -369469048;    int sIdcbVPRAU22132489 = -13165156;    int sIdcbVPRAU27605426 = -653702574;    int sIdcbVPRAU21775014 = 33439596;    int sIdcbVPRAU64555230 = -384633981;    int sIdcbVPRAU29067253 = -678091127;    int sIdcbVPRAU76937647 = -388627228;    int sIdcbVPRAU6501223 = -926107958;    int sIdcbVPRAU58612857 = -136308471;    int sIdcbVPRAU8320869 = -363382073;    int sIdcbVPRAU66298867 = -691300143;    int sIdcbVPRAU90905841 = -603383785;    int sIdcbVPRAU35489914 = 57944306;    int sIdcbVPRAU80734851 = -173815241;    int sIdcbVPRAU74747187 = -503545443;    int sIdcbVPRAU82227040 = -854060908;    int sIdcbVPRAU26152825 = -610812864;    int sIdcbVPRAU39744587 = -480444241;    int sIdcbVPRAU51908210 = -79605304;    int sIdcbVPRAU3991576 = -236906729;    int sIdcbVPRAU83915084 = -343432850;    int sIdcbVPRAU24702968 = -834038810;    int sIdcbVPRAU49187767 = -30154873;    int sIdcbVPRAU75211269 = -84831782;    int sIdcbVPRAU9146410 = -74190627;    int sIdcbVPRAU17995094 = -888751588;    int sIdcbVPRAU78600760 = -511060047;    int sIdcbVPRAU64244764 = -696666459;    int sIdcbVPRAU17592217 = -57826775;    int sIdcbVPRAU16595855 = -929216486;    int sIdcbVPRAU52454105 = -679451349;    int sIdcbVPRAU3437659 = -456916661;    int sIdcbVPRAU76601325 = -114979570;    int sIdcbVPRAU8634983 = -999067830;    int sIdcbVPRAU70814976 = -117801070;    int sIdcbVPRAU81763710 = -871127904;    int sIdcbVPRAU62253853 = -937105112;    int sIdcbVPRAU97166170 = -83005025;    int sIdcbVPRAU29364474 = -56922721;    int sIdcbVPRAU58361295 = -680116537;    int sIdcbVPRAU5124797 = -659931641;    int sIdcbVPRAU71841651 = 61014098;    int sIdcbVPRAU8674379 = -772303046;    int sIdcbVPRAU12299064 = -111058808;    int sIdcbVPRAU18993868 = 85250547;    int sIdcbVPRAU71625511 = -199607349;    int sIdcbVPRAU16567191 = -544583949;     sIdcbVPRAU88109429 = sIdcbVPRAU93452776;     sIdcbVPRAU93452776 = sIdcbVPRAU34607429;     sIdcbVPRAU34607429 = sIdcbVPRAU29804323;     sIdcbVPRAU29804323 = sIdcbVPRAU2862130;     sIdcbVPRAU2862130 = sIdcbVPRAU82236033;     sIdcbVPRAU82236033 = sIdcbVPRAU59612633;     sIdcbVPRAU59612633 = sIdcbVPRAU66479018;     sIdcbVPRAU66479018 = sIdcbVPRAU21909109;     sIdcbVPRAU21909109 = sIdcbVPRAU48882933;     sIdcbVPRAU48882933 = sIdcbVPRAU97987771;     sIdcbVPRAU97987771 = sIdcbVPRAU20345007;     sIdcbVPRAU20345007 = sIdcbVPRAU78643386;     sIdcbVPRAU78643386 = sIdcbVPRAU72086319;     sIdcbVPRAU72086319 = sIdcbVPRAU45094009;     sIdcbVPRAU45094009 = sIdcbVPRAU33664291;     sIdcbVPRAU33664291 = sIdcbVPRAU39336757;     sIdcbVPRAU39336757 = sIdcbVPRAU86054019;     sIdcbVPRAU86054019 = sIdcbVPRAU62257958;     sIdcbVPRAU62257958 = sIdcbVPRAU91990204;     sIdcbVPRAU91990204 = sIdcbVPRAU69807700;     sIdcbVPRAU69807700 = sIdcbVPRAU60504003;     sIdcbVPRAU60504003 = sIdcbVPRAU71677762;     sIdcbVPRAU71677762 = sIdcbVPRAU70052198;     sIdcbVPRAU70052198 = sIdcbVPRAU737071;     sIdcbVPRAU737071 = sIdcbVPRAU25924482;     sIdcbVPRAU25924482 = sIdcbVPRAU75734811;     sIdcbVPRAU75734811 = sIdcbVPRAU999776;     sIdcbVPRAU999776 = sIdcbVPRAU58158149;     sIdcbVPRAU58158149 = sIdcbVPRAU55610242;     sIdcbVPRAU55610242 = sIdcbVPRAU57977091;     sIdcbVPRAU57977091 = sIdcbVPRAU62497857;     sIdcbVPRAU62497857 = sIdcbVPRAU39610156;     sIdcbVPRAU39610156 = sIdcbVPRAU3896200;     sIdcbVPRAU3896200 = sIdcbVPRAU89859279;     sIdcbVPRAU89859279 = sIdcbVPRAU18941185;     sIdcbVPRAU18941185 = sIdcbVPRAU93919703;     sIdcbVPRAU93919703 = sIdcbVPRAU87428547;     sIdcbVPRAU87428547 = sIdcbVPRAU82062443;     sIdcbVPRAU82062443 = sIdcbVPRAU78342873;     sIdcbVPRAU78342873 = sIdcbVPRAU67287236;     sIdcbVPRAU67287236 = sIdcbVPRAU20619933;     sIdcbVPRAU20619933 = sIdcbVPRAU85292734;     sIdcbVPRAU85292734 = sIdcbVPRAU62531353;     sIdcbVPRAU62531353 = sIdcbVPRAU52057104;     sIdcbVPRAU52057104 = sIdcbVPRAU22136311;     sIdcbVPRAU22136311 = sIdcbVPRAU61679717;     sIdcbVPRAU61679717 = sIdcbVPRAU58142594;     sIdcbVPRAU58142594 = sIdcbVPRAU84403921;     sIdcbVPRAU84403921 = sIdcbVPRAU5704044;     sIdcbVPRAU5704044 = sIdcbVPRAU52172583;     sIdcbVPRAU52172583 = sIdcbVPRAU81375766;     sIdcbVPRAU81375766 = sIdcbVPRAU53862874;     sIdcbVPRAU53862874 = sIdcbVPRAU68795179;     sIdcbVPRAU68795179 = sIdcbVPRAU22132489;     sIdcbVPRAU22132489 = sIdcbVPRAU27605426;     sIdcbVPRAU27605426 = sIdcbVPRAU21775014;     sIdcbVPRAU21775014 = sIdcbVPRAU64555230;     sIdcbVPRAU64555230 = sIdcbVPRAU29067253;     sIdcbVPRAU29067253 = sIdcbVPRAU76937647;     sIdcbVPRAU76937647 = sIdcbVPRAU6501223;     sIdcbVPRAU6501223 = sIdcbVPRAU58612857;     sIdcbVPRAU58612857 = sIdcbVPRAU8320869;     sIdcbVPRAU8320869 = sIdcbVPRAU66298867;     sIdcbVPRAU66298867 = sIdcbVPRAU90905841;     sIdcbVPRAU90905841 = sIdcbVPRAU35489914;     sIdcbVPRAU35489914 = sIdcbVPRAU80734851;     sIdcbVPRAU80734851 = sIdcbVPRAU74747187;     sIdcbVPRAU74747187 = sIdcbVPRAU82227040;     sIdcbVPRAU82227040 = sIdcbVPRAU26152825;     sIdcbVPRAU26152825 = sIdcbVPRAU39744587;     sIdcbVPRAU39744587 = sIdcbVPRAU51908210;     sIdcbVPRAU51908210 = sIdcbVPRAU3991576;     sIdcbVPRAU3991576 = sIdcbVPRAU83915084;     sIdcbVPRAU83915084 = sIdcbVPRAU24702968;     sIdcbVPRAU24702968 = sIdcbVPRAU49187767;     sIdcbVPRAU49187767 = sIdcbVPRAU75211269;     sIdcbVPRAU75211269 = sIdcbVPRAU9146410;     sIdcbVPRAU9146410 = sIdcbVPRAU17995094;     sIdcbVPRAU17995094 = sIdcbVPRAU78600760;     sIdcbVPRAU78600760 = sIdcbVPRAU64244764;     sIdcbVPRAU64244764 = sIdcbVPRAU17592217;     sIdcbVPRAU17592217 = sIdcbVPRAU16595855;     sIdcbVPRAU16595855 = sIdcbVPRAU52454105;     sIdcbVPRAU52454105 = sIdcbVPRAU3437659;     sIdcbVPRAU3437659 = sIdcbVPRAU76601325;     sIdcbVPRAU76601325 = sIdcbVPRAU8634983;     sIdcbVPRAU8634983 = sIdcbVPRAU70814976;     sIdcbVPRAU70814976 = sIdcbVPRAU81763710;     sIdcbVPRAU81763710 = sIdcbVPRAU62253853;     sIdcbVPRAU62253853 = sIdcbVPRAU97166170;     sIdcbVPRAU97166170 = sIdcbVPRAU29364474;     sIdcbVPRAU29364474 = sIdcbVPRAU58361295;     sIdcbVPRAU58361295 = sIdcbVPRAU5124797;     sIdcbVPRAU5124797 = sIdcbVPRAU71841651;     sIdcbVPRAU71841651 = sIdcbVPRAU8674379;     sIdcbVPRAU8674379 = sIdcbVPRAU12299064;     sIdcbVPRAU12299064 = sIdcbVPRAU18993868;     sIdcbVPRAU18993868 = sIdcbVPRAU71625511;     sIdcbVPRAU71625511 = sIdcbVPRAU16567191;     sIdcbVPRAU16567191 = sIdcbVPRAU88109429;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ycTfmacYyb28585164() {     int xvottHOGcT10496547 = -178987912;    int xvottHOGcT77800276 = 7714783;    int xvottHOGcT63376707 = -404582758;    int xvottHOGcT44811012 = -365645469;    int xvottHOGcT1036384 = -865502978;    int xvottHOGcT97119149 = -857700274;    int xvottHOGcT51768372 = 29238790;    int xvottHOGcT42112877 = -701170956;    int xvottHOGcT19072240 = -399181707;    int xvottHOGcT40646517 = 59650613;    int xvottHOGcT35529631 = -420288675;    int xvottHOGcT76247956 = -11830652;    int xvottHOGcT45651567 = -6555822;    int xvottHOGcT14701072 = -972577349;    int xvottHOGcT81829286 = -487856005;    int xvottHOGcT77920203 = -403668485;    int xvottHOGcT89160108 = -815092787;    int xvottHOGcT92030435 = -837208288;    int xvottHOGcT3539589 = -443628835;    int xvottHOGcT70506487 = 4715220;    int xvottHOGcT70690807 = -696083061;    int xvottHOGcT24319222 = -295647689;    int xvottHOGcT74971086 = -153262352;    int xvottHOGcT96551985 = -244287790;    int xvottHOGcT36277318 = -302705935;    int xvottHOGcT14227477 = -229712973;    int xvottHOGcT22824232 = -828570431;    int xvottHOGcT6911226 = -800919887;    int xvottHOGcT65050710 = -111449359;    int xvottHOGcT64767421 = -743981393;    int xvottHOGcT2267897 = -675540421;    int xvottHOGcT20039048 = 66320065;    int xvottHOGcT20648826 = -16613613;    int xvottHOGcT63284887 = -208419910;    int xvottHOGcT66088643 = -654564107;    int xvottHOGcT60634054 = -464960804;    int xvottHOGcT87359415 = -274504654;    int xvottHOGcT41054648 = -225833136;    int xvottHOGcT30370575 = -449604900;    int xvottHOGcT98528588 = -1317889;    int xvottHOGcT96044607 = -411789183;    int xvottHOGcT9128325 = -332349571;    int xvottHOGcT90842926 = -593580179;    int xvottHOGcT61975472 = -900354783;    int xvottHOGcT20040371 = -700686823;    int xvottHOGcT30236592 = -252315859;    int xvottHOGcT73977115 = -667562543;    int xvottHOGcT2741269 = -669692154;    int xvottHOGcT78863826 = 67034354;    int xvottHOGcT2787790 = -229761150;    int xvottHOGcT61041799 = -621845143;    int xvottHOGcT55227023 = -810063982;    int xvottHOGcT32177278 = -54451981;    int xvottHOGcT29081725 = -645149857;    int xvottHOGcT57894357 = -153869272;    int xvottHOGcT86177325 = -883340223;    int xvottHOGcT2829191 = -839022866;    int xvottHOGcT66824721 = -60294968;    int xvottHOGcT8533694 = 37060466;    int xvottHOGcT86808906 = -535790005;    int xvottHOGcT74294918 = 70870156;    int xvottHOGcT44857146 = -169841324;    int xvottHOGcT77062167 = -489721598;    int xvottHOGcT54304819 = -655200315;    int xvottHOGcT38378621 = -264808966;    int xvottHOGcT15490583 = -386608741;    int xvottHOGcT55599130 = -995217040;    int xvottHOGcT82366680 = -798135913;    int xvottHOGcT48612428 = -218013242;    int xvottHOGcT21195233 = 77104799;    int xvottHOGcT90560787 = -29163832;    int xvottHOGcT48105461 = -489259652;    int xvottHOGcT61659860 = -287603388;    int xvottHOGcT5011001 = -342310946;    int xvottHOGcT74461880 = -583495597;    int xvottHOGcT61562483 = -263733490;    int xvottHOGcT33476296 = -702067511;    int xvottHOGcT12995614 = -252907570;    int xvottHOGcT76511614 = -543600967;    int xvottHOGcT6040726 = 49609924;    int xvottHOGcT40250362 = -562150430;    int xvottHOGcT20082963 = -58878277;    int xvottHOGcT28047400 = -767954242;    int xvottHOGcT62262920 = -881688210;    int xvottHOGcT3725622 = -22136250;    int xvottHOGcT47040873 = -865476440;    int xvottHOGcT87861770 = -879227955;    int xvottHOGcT91567101 = -371463756;    int xvottHOGcT5390531 = 45449361;    int xvottHOGcT79911318 = -771223884;    int xvottHOGcT57804863 = -625937939;    int xvottHOGcT20534694 = -114209686;    int xvottHOGcT32520955 = -162893602;    int xvottHOGcT43561668 = -913814895;    int xvottHOGcT24233670 = 27811954;    int xvottHOGcT51187461 = -141947860;    int xvottHOGcT32066158 = -842627974;    int xvottHOGcT36538108 = -938379864;    int xvottHOGcT23596852 = -535545817;    int xvottHOGcT4549788 = -178987912;     xvottHOGcT10496547 = xvottHOGcT77800276;     xvottHOGcT77800276 = xvottHOGcT63376707;     xvottHOGcT63376707 = xvottHOGcT44811012;     xvottHOGcT44811012 = xvottHOGcT1036384;     xvottHOGcT1036384 = xvottHOGcT97119149;     xvottHOGcT97119149 = xvottHOGcT51768372;     xvottHOGcT51768372 = xvottHOGcT42112877;     xvottHOGcT42112877 = xvottHOGcT19072240;     xvottHOGcT19072240 = xvottHOGcT40646517;     xvottHOGcT40646517 = xvottHOGcT35529631;     xvottHOGcT35529631 = xvottHOGcT76247956;     xvottHOGcT76247956 = xvottHOGcT45651567;     xvottHOGcT45651567 = xvottHOGcT14701072;     xvottHOGcT14701072 = xvottHOGcT81829286;     xvottHOGcT81829286 = xvottHOGcT77920203;     xvottHOGcT77920203 = xvottHOGcT89160108;     xvottHOGcT89160108 = xvottHOGcT92030435;     xvottHOGcT92030435 = xvottHOGcT3539589;     xvottHOGcT3539589 = xvottHOGcT70506487;     xvottHOGcT70506487 = xvottHOGcT70690807;     xvottHOGcT70690807 = xvottHOGcT24319222;     xvottHOGcT24319222 = xvottHOGcT74971086;     xvottHOGcT74971086 = xvottHOGcT96551985;     xvottHOGcT96551985 = xvottHOGcT36277318;     xvottHOGcT36277318 = xvottHOGcT14227477;     xvottHOGcT14227477 = xvottHOGcT22824232;     xvottHOGcT22824232 = xvottHOGcT6911226;     xvottHOGcT6911226 = xvottHOGcT65050710;     xvottHOGcT65050710 = xvottHOGcT64767421;     xvottHOGcT64767421 = xvottHOGcT2267897;     xvottHOGcT2267897 = xvottHOGcT20039048;     xvottHOGcT20039048 = xvottHOGcT20648826;     xvottHOGcT20648826 = xvottHOGcT63284887;     xvottHOGcT63284887 = xvottHOGcT66088643;     xvottHOGcT66088643 = xvottHOGcT60634054;     xvottHOGcT60634054 = xvottHOGcT87359415;     xvottHOGcT87359415 = xvottHOGcT41054648;     xvottHOGcT41054648 = xvottHOGcT30370575;     xvottHOGcT30370575 = xvottHOGcT98528588;     xvottHOGcT98528588 = xvottHOGcT96044607;     xvottHOGcT96044607 = xvottHOGcT9128325;     xvottHOGcT9128325 = xvottHOGcT90842926;     xvottHOGcT90842926 = xvottHOGcT61975472;     xvottHOGcT61975472 = xvottHOGcT20040371;     xvottHOGcT20040371 = xvottHOGcT30236592;     xvottHOGcT30236592 = xvottHOGcT73977115;     xvottHOGcT73977115 = xvottHOGcT2741269;     xvottHOGcT2741269 = xvottHOGcT78863826;     xvottHOGcT78863826 = xvottHOGcT2787790;     xvottHOGcT2787790 = xvottHOGcT61041799;     xvottHOGcT61041799 = xvottHOGcT55227023;     xvottHOGcT55227023 = xvottHOGcT32177278;     xvottHOGcT32177278 = xvottHOGcT29081725;     xvottHOGcT29081725 = xvottHOGcT57894357;     xvottHOGcT57894357 = xvottHOGcT86177325;     xvottHOGcT86177325 = xvottHOGcT2829191;     xvottHOGcT2829191 = xvottHOGcT66824721;     xvottHOGcT66824721 = xvottHOGcT8533694;     xvottHOGcT8533694 = xvottHOGcT86808906;     xvottHOGcT86808906 = xvottHOGcT74294918;     xvottHOGcT74294918 = xvottHOGcT44857146;     xvottHOGcT44857146 = xvottHOGcT77062167;     xvottHOGcT77062167 = xvottHOGcT54304819;     xvottHOGcT54304819 = xvottHOGcT38378621;     xvottHOGcT38378621 = xvottHOGcT15490583;     xvottHOGcT15490583 = xvottHOGcT55599130;     xvottHOGcT55599130 = xvottHOGcT82366680;     xvottHOGcT82366680 = xvottHOGcT48612428;     xvottHOGcT48612428 = xvottHOGcT21195233;     xvottHOGcT21195233 = xvottHOGcT90560787;     xvottHOGcT90560787 = xvottHOGcT48105461;     xvottHOGcT48105461 = xvottHOGcT61659860;     xvottHOGcT61659860 = xvottHOGcT5011001;     xvottHOGcT5011001 = xvottHOGcT74461880;     xvottHOGcT74461880 = xvottHOGcT61562483;     xvottHOGcT61562483 = xvottHOGcT33476296;     xvottHOGcT33476296 = xvottHOGcT12995614;     xvottHOGcT12995614 = xvottHOGcT76511614;     xvottHOGcT76511614 = xvottHOGcT6040726;     xvottHOGcT6040726 = xvottHOGcT40250362;     xvottHOGcT40250362 = xvottHOGcT20082963;     xvottHOGcT20082963 = xvottHOGcT28047400;     xvottHOGcT28047400 = xvottHOGcT62262920;     xvottHOGcT62262920 = xvottHOGcT3725622;     xvottHOGcT3725622 = xvottHOGcT47040873;     xvottHOGcT47040873 = xvottHOGcT87861770;     xvottHOGcT87861770 = xvottHOGcT91567101;     xvottHOGcT91567101 = xvottHOGcT5390531;     xvottHOGcT5390531 = xvottHOGcT79911318;     xvottHOGcT79911318 = xvottHOGcT57804863;     xvottHOGcT57804863 = xvottHOGcT20534694;     xvottHOGcT20534694 = xvottHOGcT32520955;     xvottHOGcT32520955 = xvottHOGcT43561668;     xvottHOGcT43561668 = xvottHOGcT24233670;     xvottHOGcT24233670 = xvottHOGcT51187461;     xvottHOGcT51187461 = xvottHOGcT32066158;     xvottHOGcT32066158 = xvottHOGcT36538108;     xvottHOGcT36538108 = xvottHOGcT23596852;     xvottHOGcT23596852 = xvottHOGcT4549788;     xvottHOGcT4549788 = xvottHOGcT10496547;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void XVaDHqSYsa65230138() {     int tkDKpjvxaw32883665 = -913391875;    int tkDKpjvxaw62147777 = 59171728;    int tkDKpjvxaw92145985 = -644236602;    int tkDKpjvxaw59817700 = -372053000;    int tkDKpjvxaw99210637 = -356559979;    int tkDKpjvxaw12002267 = -718308652;    int tkDKpjvxaw43924111 = -892748516;    int tkDKpjvxaw17746736 = -71767600;    int tkDKpjvxaw16235371 = -283377746;    int tkDKpjvxaw32410102 = -783090720;    int tkDKpjvxaw73071490 = -416913076;    int tkDKpjvxaw32150905 = -262575945;    int tkDKpjvxaw12659748 = -625273142;    int tkDKpjvxaw57315824 = -400286106;    int tkDKpjvxaw18564563 = -115333718;    int tkDKpjvxaw22176116 = -785336027;    int tkDKpjvxaw38983461 = 7627393;    int tkDKpjvxaw98006851 = -188950979;    int tkDKpjvxaw44821220 = -578730960;    int tkDKpjvxaw49022771 = -247919234;    int tkDKpjvxaw71573914 = -687570370;    int tkDKpjvxaw88134440 = -800414003;    int tkDKpjvxaw78264409 = -329342944;    int tkDKpjvxaw23051773 = -808280647;    int tkDKpjvxaw71817564 = 75734941;    int tkDKpjvxaw2530472 = -673607197;    int tkDKpjvxaw69913652 = -586156924;    int tkDKpjvxaw12822676 = -589374340;    int tkDKpjvxaw71943271 = -455706478;    int tkDKpjvxaw73924600 = -664277259;    int tkDKpjvxaw46558702 = -756856573;    int tkDKpjvxaw77580238 = -585751290;    int tkDKpjvxaw1687497 = -545957108;    int tkDKpjvxaw22673574 = -532546760;    int tkDKpjvxaw42318008 = -718320530;    int tkDKpjvxaw2326923 = -780356181;    int tkDKpjvxaw80799128 = -7452605;    int tkDKpjvxaw94680749 = -93458608;    int tkDKpjvxaw78678706 = -850650932;    int tkDKpjvxaw18714303 = -137541918;    int tkDKpjvxaw24801978 = -914966849;    int tkDKpjvxaw97636715 = -90258262;    int tkDKpjvxaw96393118 = -481110762;    int tkDKpjvxaw61419591 = -897718432;    int tkDKpjvxaw88023637 = -509830301;    int tkDKpjvxaw38336874 = -434544955;    int tkDKpjvxaw86274512 = -145972796;    int tkDKpjvxaw47339943 = -426227145;    int tkDKpjvxaw73323731 = -982682344;    int tkDKpjvxaw99871535 = -371781409;    int tkDKpjvxaw69911016 = -976921420;    int tkDKpjvxaw29078281 = -140883264;    int tkDKpjvxaw10491681 = -826363211;    int tkDKpjvxaw89368270 = -920830666;    int tkDKpjvxaw93656224 = -294573387;    int tkDKpjvxaw44749224 = -12977873;    int tkDKpjvxaw83883368 = -611485329;    int tkDKpjvxaw69094213 = -835955955;    int tkDKpjvxaw88000135 = -347787942;    int tkDKpjvxaw96680166 = -682952783;    int tkDKpjvxaw42088615 = -32151729;    int tkDKpjvxaw31101435 = -203374177;    int tkDKpjvxaw45803465 = -616061123;    int tkDKpjvxaw42310770 = -619100488;    int tkDKpjvxaw85851400 = 73765853;    int tkDKpjvxaw95491251 = -831161787;    int tkDKpjvxaw30463408 = -716618838;    int tkDKpjvxaw89986173 = 7273618;    int tkDKpjvxaw14997816 = -681965577;    int tkDKpjvxaw16237641 = -334977538;    int tkDKpjvxaw41376988 = -677883423;    int tkDKpjvxaw44302712 = -898914000;    int tkDKpjvxaw19328146 = -338300047;    int tkDKpjvxaw26106917 = -341189042;    int tkDKpjvxaw24220793 = -332952385;    int tkDKpjvxaw73937198 = -497312108;    int tkDKpjvxaw91741321 = -219303241;    int tkDKpjvxaw16844818 = -431624513;    int tkDKpjvxaw35028135 = -198450347;    int tkDKpjvxaw33480691 = -489720105;    int tkDKpjvxaw16255959 = -427634401;    int tkDKpjvxaw22573709 = -59929779;    int tkDKpjvxaw39498945 = -606691997;    int tkDKpjvxaw72071735 = 16074930;    int tkDKpjvxaw4013585 = -687355840;    int tkDKpjvxaw17480422 = -515973310;    int tkDKpjvxaw67088558 = -759388079;    int tkDKpjvxaw12319227 = -625126442;    int tkDKpjvxaw29017350 = -137973373;    int tkDKpjvxaw97568784 = -605342657;    int tkDKpjvxaw18443555 = -68870852;    int tkDKpjvxaw11704915 = -171496650;    int tkDKpjvxaw6680614 = -745670667;    int tkDKpjvxaw81998540 = -67698150;    int tkDKpjvxaw76625687 = -5390190;    int tkDKpjvxaw93700543 = -611592673;    int tkDKpjvxaw51833251 = -474197140;    int tkDKpjvxaw54082348 = -862010275;    int tkDKpjvxaw75568191 = -871484285;    int tkDKpjvxaw92532385 = -913391875;     tkDKpjvxaw32883665 = tkDKpjvxaw62147777;     tkDKpjvxaw62147777 = tkDKpjvxaw92145985;     tkDKpjvxaw92145985 = tkDKpjvxaw59817700;     tkDKpjvxaw59817700 = tkDKpjvxaw99210637;     tkDKpjvxaw99210637 = tkDKpjvxaw12002267;     tkDKpjvxaw12002267 = tkDKpjvxaw43924111;     tkDKpjvxaw43924111 = tkDKpjvxaw17746736;     tkDKpjvxaw17746736 = tkDKpjvxaw16235371;     tkDKpjvxaw16235371 = tkDKpjvxaw32410102;     tkDKpjvxaw32410102 = tkDKpjvxaw73071490;     tkDKpjvxaw73071490 = tkDKpjvxaw32150905;     tkDKpjvxaw32150905 = tkDKpjvxaw12659748;     tkDKpjvxaw12659748 = tkDKpjvxaw57315824;     tkDKpjvxaw57315824 = tkDKpjvxaw18564563;     tkDKpjvxaw18564563 = tkDKpjvxaw22176116;     tkDKpjvxaw22176116 = tkDKpjvxaw38983461;     tkDKpjvxaw38983461 = tkDKpjvxaw98006851;     tkDKpjvxaw98006851 = tkDKpjvxaw44821220;     tkDKpjvxaw44821220 = tkDKpjvxaw49022771;     tkDKpjvxaw49022771 = tkDKpjvxaw71573914;     tkDKpjvxaw71573914 = tkDKpjvxaw88134440;     tkDKpjvxaw88134440 = tkDKpjvxaw78264409;     tkDKpjvxaw78264409 = tkDKpjvxaw23051773;     tkDKpjvxaw23051773 = tkDKpjvxaw71817564;     tkDKpjvxaw71817564 = tkDKpjvxaw2530472;     tkDKpjvxaw2530472 = tkDKpjvxaw69913652;     tkDKpjvxaw69913652 = tkDKpjvxaw12822676;     tkDKpjvxaw12822676 = tkDKpjvxaw71943271;     tkDKpjvxaw71943271 = tkDKpjvxaw73924600;     tkDKpjvxaw73924600 = tkDKpjvxaw46558702;     tkDKpjvxaw46558702 = tkDKpjvxaw77580238;     tkDKpjvxaw77580238 = tkDKpjvxaw1687497;     tkDKpjvxaw1687497 = tkDKpjvxaw22673574;     tkDKpjvxaw22673574 = tkDKpjvxaw42318008;     tkDKpjvxaw42318008 = tkDKpjvxaw2326923;     tkDKpjvxaw2326923 = tkDKpjvxaw80799128;     tkDKpjvxaw80799128 = tkDKpjvxaw94680749;     tkDKpjvxaw94680749 = tkDKpjvxaw78678706;     tkDKpjvxaw78678706 = tkDKpjvxaw18714303;     tkDKpjvxaw18714303 = tkDKpjvxaw24801978;     tkDKpjvxaw24801978 = tkDKpjvxaw97636715;     tkDKpjvxaw97636715 = tkDKpjvxaw96393118;     tkDKpjvxaw96393118 = tkDKpjvxaw61419591;     tkDKpjvxaw61419591 = tkDKpjvxaw88023637;     tkDKpjvxaw88023637 = tkDKpjvxaw38336874;     tkDKpjvxaw38336874 = tkDKpjvxaw86274512;     tkDKpjvxaw86274512 = tkDKpjvxaw47339943;     tkDKpjvxaw47339943 = tkDKpjvxaw73323731;     tkDKpjvxaw73323731 = tkDKpjvxaw99871535;     tkDKpjvxaw99871535 = tkDKpjvxaw69911016;     tkDKpjvxaw69911016 = tkDKpjvxaw29078281;     tkDKpjvxaw29078281 = tkDKpjvxaw10491681;     tkDKpjvxaw10491681 = tkDKpjvxaw89368270;     tkDKpjvxaw89368270 = tkDKpjvxaw93656224;     tkDKpjvxaw93656224 = tkDKpjvxaw44749224;     tkDKpjvxaw44749224 = tkDKpjvxaw83883368;     tkDKpjvxaw83883368 = tkDKpjvxaw69094213;     tkDKpjvxaw69094213 = tkDKpjvxaw88000135;     tkDKpjvxaw88000135 = tkDKpjvxaw96680166;     tkDKpjvxaw96680166 = tkDKpjvxaw42088615;     tkDKpjvxaw42088615 = tkDKpjvxaw31101435;     tkDKpjvxaw31101435 = tkDKpjvxaw45803465;     tkDKpjvxaw45803465 = tkDKpjvxaw42310770;     tkDKpjvxaw42310770 = tkDKpjvxaw85851400;     tkDKpjvxaw85851400 = tkDKpjvxaw95491251;     tkDKpjvxaw95491251 = tkDKpjvxaw30463408;     tkDKpjvxaw30463408 = tkDKpjvxaw89986173;     tkDKpjvxaw89986173 = tkDKpjvxaw14997816;     tkDKpjvxaw14997816 = tkDKpjvxaw16237641;     tkDKpjvxaw16237641 = tkDKpjvxaw41376988;     tkDKpjvxaw41376988 = tkDKpjvxaw44302712;     tkDKpjvxaw44302712 = tkDKpjvxaw19328146;     tkDKpjvxaw19328146 = tkDKpjvxaw26106917;     tkDKpjvxaw26106917 = tkDKpjvxaw24220793;     tkDKpjvxaw24220793 = tkDKpjvxaw73937198;     tkDKpjvxaw73937198 = tkDKpjvxaw91741321;     tkDKpjvxaw91741321 = tkDKpjvxaw16844818;     tkDKpjvxaw16844818 = tkDKpjvxaw35028135;     tkDKpjvxaw35028135 = tkDKpjvxaw33480691;     tkDKpjvxaw33480691 = tkDKpjvxaw16255959;     tkDKpjvxaw16255959 = tkDKpjvxaw22573709;     tkDKpjvxaw22573709 = tkDKpjvxaw39498945;     tkDKpjvxaw39498945 = tkDKpjvxaw72071735;     tkDKpjvxaw72071735 = tkDKpjvxaw4013585;     tkDKpjvxaw4013585 = tkDKpjvxaw17480422;     tkDKpjvxaw17480422 = tkDKpjvxaw67088558;     tkDKpjvxaw67088558 = tkDKpjvxaw12319227;     tkDKpjvxaw12319227 = tkDKpjvxaw29017350;     tkDKpjvxaw29017350 = tkDKpjvxaw97568784;     tkDKpjvxaw97568784 = tkDKpjvxaw18443555;     tkDKpjvxaw18443555 = tkDKpjvxaw11704915;     tkDKpjvxaw11704915 = tkDKpjvxaw6680614;     tkDKpjvxaw6680614 = tkDKpjvxaw81998540;     tkDKpjvxaw81998540 = tkDKpjvxaw76625687;     tkDKpjvxaw76625687 = tkDKpjvxaw93700543;     tkDKpjvxaw93700543 = tkDKpjvxaw51833251;     tkDKpjvxaw51833251 = tkDKpjvxaw54082348;     tkDKpjvxaw54082348 = tkDKpjvxaw75568191;     tkDKpjvxaw75568191 = tkDKpjvxaw92532385;     tkDKpjvxaw92532385 = tkDKpjvxaw32883665;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void jJuNSfaCMO1875113() {     int gnNgsWnITI55270782 = -547795839;    int gnNgsWnITI46495277 = -989371327;    int gnNgsWnITI20915265 = -883890446;    int gnNgsWnITI74824388 = -378460532;    int gnNgsWnITI97384891 = -947616980;    int gnNgsWnITI26885384 = -578917030;    int gnNgsWnITI36079850 = -714735822;    int gnNgsWnITI93380594 = -542364245;    int gnNgsWnITI13398502 = -167573785;    int gnNgsWnITI24173687 = -525832053;    int gnNgsWnITI10613351 = -413537477;    int gnNgsWnITI88053853 = -513321238;    int gnNgsWnITI79667928 = -143990461;    int gnNgsWnITI99930576 = -927994863;    int gnNgsWnITI55299840 = -842811431;    int gnNgsWnITI66432028 = -67003570;    int gnNgsWnITI88806812 = -269652427;    int gnNgsWnITI3983269 = -640693670;    int gnNgsWnITI86102850 = -713833085;    int gnNgsWnITI27539054 = -500553687;    int gnNgsWnITI72457021 = -679057678;    int gnNgsWnITI51949659 = -205180316;    int gnNgsWnITI81557732 = -505423536;    int gnNgsWnITI49551560 = -272273505;    int gnNgsWnITI7357812 = -645824184;    int gnNgsWnITI90833465 = -17501421;    int gnNgsWnITI17003072 = -343743416;    int gnNgsWnITI18734126 = -377828794;    int gnNgsWnITI78835831 = -799963597;    int gnNgsWnITI83081780 = -584573126;    int gnNgsWnITI90849506 = -838172725;    int gnNgsWnITI35121430 = -137822644;    int gnNgsWnITI82726167 = 24699398;    int gnNgsWnITI82062261 = -856673610;    int gnNgsWnITI18547373 = -782076952;    int gnNgsWnITI44019792 = 4248443;    int gnNgsWnITI74238840 = -840400557;    int gnNgsWnITI48306850 = 38915920;    int gnNgsWnITI26986838 = -151696964;    int gnNgsWnITI38900018 = -273765947;    int gnNgsWnITI53559349 = -318144515;    int gnNgsWnITI86145106 = -948166953;    int gnNgsWnITI1943311 = -368641346;    int gnNgsWnITI60863710 = -895082081;    int gnNgsWnITI56006904 = -318973779;    int gnNgsWnITI46437156 = -616774050;    int gnNgsWnITI98571909 = -724383049;    int gnNgsWnITI91938617 = -182762136;    int gnNgsWnITI67783636 = -932399042;    int gnNgsWnITI96955281 = -513801668;    int gnNgsWnITI78780232 = -231997697;    int gnNgsWnITI2929538 = -571702546;    int gnNgsWnITI88806083 = -498274441;    int gnNgsWnITI49654815 = -96511475;    int gnNgsWnITI29418092 = -435277502;    int gnNgsWnITI3321124 = -242615523;    int gnNgsWnITI64937545 = -383947791;    int gnNgsWnITI71363704 = -511616942;    int gnNgsWnITI67466577 = -732636349;    int gnNgsWnITI6551427 = -830115560;    int gnNgsWnITI9882312 = -135173614;    int gnNgsWnITI17345724 = -236907029;    int gnNgsWnITI14544763 = -742400648;    int gnNgsWnITI30316722 = -583000660;    int gnNgsWnITI33324180 = -687659328;    int gnNgsWnITI75491920 = -175714833;    int gnNgsWnITI5327687 = -438020637;    int gnNgsWnITI97605666 = -287316852;    int gnNgsWnITI81383203 = -45917911;    int gnNgsWnITI11280048 = -747059874;    int gnNgsWnITI92193188 = -226603014;    int gnNgsWnITI40499962 = -208568347;    int gnNgsWnITI76996430 = -388996706;    int gnNgsWnITI47202833 = -340067138;    int gnNgsWnITI73979705 = -82409173;    int gnNgsWnITI86311914 = -730890725;    int gnNgsWnITI50006348 = -836538970;    int gnNgsWnITI20694022 = -610341456;    int gnNgsWnITI93544655 = -953299727;    int gnNgsWnITI60920655 = 70949866;    int gnNgsWnITI92261556 = -293118372;    int gnNgsWnITI25064455 = -60981281;    int gnNgsWnITI50950490 = -445429752;    int gnNgsWnITI81880550 = -186161930;    int gnNgsWnITI4301548 = -252575430;    int gnNgsWnITI87919969 = -166470180;    int gnNgsWnITI46315346 = -639548204;    int gnNgsWnITI33071352 = -878789128;    int gnNgsWnITI52644170 = -321396108;    int gnNgsWnITI15226250 = -439461430;    int gnNgsWnITI79082246 = -611803766;    int gnNgsWnITI2875136 = -228783615;    int gnNgsWnITI80840273 = -228447732;    int gnNgsWnITI20435412 = -321581404;    int gnNgsWnITI29017706 = -38592334;    int gnNgsWnITI36213625 = 18762513;    int gnNgsWnITI71600344 = -105766306;    int gnNgsWnITI71626588 = -785640686;    int gnNgsWnITI27539531 = -107422753;    int gnNgsWnITI80514983 = -547795839;     gnNgsWnITI55270782 = gnNgsWnITI46495277;     gnNgsWnITI46495277 = gnNgsWnITI20915265;     gnNgsWnITI20915265 = gnNgsWnITI74824388;     gnNgsWnITI74824388 = gnNgsWnITI97384891;     gnNgsWnITI97384891 = gnNgsWnITI26885384;     gnNgsWnITI26885384 = gnNgsWnITI36079850;     gnNgsWnITI36079850 = gnNgsWnITI93380594;     gnNgsWnITI93380594 = gnNgsWnITI13398502;     gnNgsWnITI13398502 = gnNgsWnITI24173687;     gnNgsWnITI24173687 = gnNgsWnITI10613351;     gnNgsWnITI10613351 = gnNgsWnITI88053853;     gnNgsWnITI88053853 = gnNgsWnITI79667928;     gnNgsWnITI79667928 = gnNgsWnITI99930576;     gnNgsWnITI99930576 = gnNgsWnITI55299840;     gnNgsWnITI55299840 = gnNgsWnITI66432028;     gnNgsWnITI66432028 = gnNgsWnITI88806812;     gnNgsWnITI88806812 = gnNgsWnITI3983269;     gnNgsWnITI3983269 = gnNgsWnITI86102850;     gnNgsWnITI86102850 = gnNgsWnITI27539054;     gnNgsWnITI27539054 = gnNgsWnITI72457021;     gnNgsWnITI72457021 = gnNgsWnITI51949659;     gnNgsWnITI51949659 = gnNgsWnITI81557732;     gnNgsWnITI81557732 = gnNgsWnITI49551560;     gnNgsWnITI49551560 = gnNgsWnITI7357812;     gnNgsWnITI7357812 = gnNgsWnITI90833465;     gnNgsWnITI90833465 = gnNgsWnITI17003072;     gnNgsWnITI17003072 = gnNgsWnITI18734126;     gnNgsWnITI18734126 = gnNgsWnITI78835831;     gnNgsWnITI78835831 = gnNgsWnITI83081780;     gnNgsWnITI83081780 = gnNgsWnITI90849506;     gnNgsWnITI90849506 = gnNgsWnITI35121430;     gnNgsWnITI35121430 = gnNgsWnITI82726167;     gnNgsWnITI82726167 = gnNgsWnITI82062261;     gnNgsWnITI82062261 = gnNgsWnITI18547373;     gnNgsWnITI18547373 = gnNgsWnITI44019792;     gnNgsWnITI44019792 = gnNgsWnITI74238840;     gnNgsWnITI74238840 = gnNgsWnITI48306850;     gnNgsWnITI48306850 = gnNgsWnITI26986838;     gnNgsWnITI26986838 = gnNgsWnITI38900018;     gnNgsWnITI38900018 = gnNgsWnITI53559349;     gnNgsWnITI53559349 = gnNgsWnITI86145106;     gnNgsWnITI86145106 = gnNgsWnITI1943311;     gnNgsWnITI1943311 = gnNgsWnITI60863710;     gnNgsWnITI60863710 = gnNgsWnITI56006904;     gnNgsWnITI56006904 = gnNgsWnITI46437156;     gnNgsWnITI46437156 = gnNgsWnITI98571909;     gnNgsWnITI98571909 = gnNgsWnITI91938617;     gnNgsWnITI91938617 = gnNgsWnITI67783636;     gnNgsWnITI67783636 = gnNgsWnITI96955281;     gnNgsWnITI96955281 = gnNgsWnITI78780232;     gnNgsWnITI78780232 = gnNgsWnITI2929538;     gnNgsWnITI2929538 = gnNgsWnITI88806083;     gnNgsWnITI88806083 = gnNgsWnITI49654815;     gnNgsWnITI49654815 = gnNgsWnITI29418092;     gnNgsWnITI29418092 = gnNgsWnITI3321124;     gnNgsWnITI3321124 = gnNgsWnITI64937545;     gnNgsWnITI64937545 = gnNgsWnITI71363704;     gnNgsWnITI71363704 = gnNgsWnITI67466577;     gnNgsWnITI67466577 = gnNgsWnITI6551427;     gnNgsWnITI6551427 = gnNgsWnITI9882312;     gnNgsWnITI9882312 = gnNgsWnITI17345724;     gnNgsWnITI17345724 = gnNgsWnITI14544763;     gnNgsWnITI14544763 = gnNgsWnITI30316722;     gnNgsWnITI30316722 = gnNgsWnITI33324180;     gnNgsWnITI33324180 = gnNgsWnITI75491920;     gnNgsWnITI75491920 = gnNgsWnITI5327687;     gnNgsWnITI5327687 = gnNgsWnITI97605666;     gnNgsWnITI97605666 = gnNgsWnITI81383203;     gnNgsWnITI81383203 = gnNgsWnITI11280048;     gnNgsWnITI11280048 = gnNgsWnITI92193188;     gnNgsWnITI92193188 = gnNgsWnITI40499962;     gnNgsWnITI40499962 = gnNgsWnITI76996430;     gnNgsWnITI76996430 = gnNgsWnITI47202833;     gnNgsWnITI47202833 = gnNgsWnITI73979705;     gnNgsWnITI73979705 = gnNgsWnITI86311914;     gnNgsWnITI86311914 = gnNgsWnITI50006348;     gnNgsWnITI50006348 = gnNgsWnITI20694022;     gnNgsWnITI20694022 = gnNgsWnITI93544655;     gnNgsWnITI93544655 = gnNgsWnITI60920655;     gnNgsWnITI60920655 = gnNgsWnITI92261556;     gnNgsWnITI92261556 = gnNgsWnITI25064455;     gnNgsWnITI25064455 = gnNgsWnITI50950490;     gnNgsWnITI50950490 = gnNgsWnITI81880550;     gnNgsWnITI81880550 = gnNgsWnITI4301548;     gnNgsWnITI4301548 = gnNgsWnITI87919969;     gnNgsWnITI87919969 = gnNgsWnITI46315346;     gnNgsWnITI46315346 = gnNgsWnITI33071352;     gnNgsWnITI33071352 = gnNgsWnITI52644170;     gnNgsWnITI52644170 = gnNgsWnITI15226250;     gnNgsWnITI15226250 = gnNgsWnITI79082246;     gnNgsWnITI79082246 = gnNgsWnITI2875136;     gnNgsWnITI2875136 = gnNgsWnITI80840273;     gnNgsWnITI80840273 = gnNgsWnITI20435412;     gnNgsWnITI20435412 = gnNgsWnITI29017706;     gnNgsWnITI29017706 = gnNgsWnITI36213625;     gnNgsWnITI36213625 = gnNgsWnITI71600344;     gnNgsWnITI71600344 = gnNgsWnITI71626588;     gnNgsWnITI71626588 = gnNgsWnITI27539531;     gnNgsWnITI27539531 = gnNgsWnITI80514983;     gnNgsWnITI80514983 = gnNgsWnITI55270782;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void YDDKEoWTHP38520088() {     int tzvvfSEgBm77657899 = -182199802;    int tzvvfSEgBm30842778 = -937914381;    int tzvvfSEgBm49684543 = -23544290;    int tzvvfSEgBm89831077 = -384868064;    int tzvvfSEgBm95559146 = -438673982;    int tzvvfSEgBm41768501 = -439525408;    int tzvvfSEgBm28235589 = -536723129;    int tzvvfSEgBm69014453 = 87039111;    int tzvvfSEgBm10561633 = -51769825;    int tzvvfSEgBm15937272 = -268573386;    int tzvvfSEgBm48155210 = -410161878;    int tzvvfSEgBm43956803 = -764066532;    int tzvvfSEgBm46676109 = -762707781;    int tzvvfSEgBm42545329 = -355703620;    int tzvvfSEgBm92035117 = -470289144;    int tzvvfSEgBm10687941 = -448671112;    int tzvvfSEgBm38630164 = -546932247;    int tzvvfSEgBm9959685 = 7563640;    int tzvvfSEgBm27384482 = -848935210;    int tzvvfSEgBm6055338 = -753188141;    int tzvvfSEgBm73340128 = -670544986;    int tzvvfSEgBm15764877 = -709946629;    int tzvvfSEgBm84851055 = -681504128;    int tzvvfSEgBm76051347 = -836266362;    int tzvvfSEgBm42898058 = -267383308;    int tzvvfSEgBm79136460 = -461395644;    int tzvvfSEgBm64092492 = -101329909;    int tzvvfSEgBm24645576 = -166283247;    int tzvvfSEgBm85728392 = -44220716;    int tzvvfSEgBm92238959 = -504868992;    int tzvvfSEgBm35140312 = -919488877;    int tzvvfSEgBm92662620 = -789893999;    int tzvvfSEgBm63764837 = -504644097;    int tzvvfSEgBm41450949 = -80800460;    int tzvvfSEgBm94776737 = -845833375;    int tzvvfSEgBm85712661 = -311146934;    int tzvvfSEgBm67678552 = -573348508;    int tzvvfSEgBm1932951 = -928709552;    int tzvvfSEgBm75294969 = -552742995;    int tzvvfSEgBm59085732 = -409989977;    int tzvvfSEgBm82316720 = -821322181;    int tzvvfSEgBm74653498 = -706075644;    int tzvvfSEgBm7493503 = -256171930;    int tzvvfSEgBm60307830 = -892445730;    int tzvvfSEgBm23990171 = -128117256;    int tzvvfSEgBm54537438 = -799003146;    int tzvvfSEgBm10869307 = -202793301;    int tzvvfSEgBm36537292 = 60702873;    int tzvvfSEgBm62243541 = -882115741;    int tzvvfSEgBm94039026 = -655821927;    int tzvvfSEgBm87649449 = -587073974;    int tzvvfSEgBm76780794 = 97478172;    int tzvvfSEgBm67120487 = -170185671;    int tzvvfSEgBm9941361 = -372192283;    int tzvvfSEgBm65179959 = -575981617;    int tzvvfSEgBm61893022 = -472253173;    int tzvvfSEgBm45991723 = -156410254;    int tzvvfSEgBm73633196 = -187277929;    int tzvvfSEgBm46933019 = -17484756;    int tzvvfSEgBm16422686 = -977278338;    int tzvvfSEgBm77676008 = -238195499;    int tzvvfSEgBm3590013 = -270439882;    int tzvvfSEgBm83286060 = -868740173;    int tzvvfSEgBm18322674 = -546900833;    int tzvvfSEgBm80796959 = -349084509;    int tzvvfSEgBm55492589 = -620267879;    int tzvvfSEgBm80191965 = -159422435;    int tzvvfSEgBm5225160 = -581907322;    int tzvvfSEgBm47768591 = -509870246;    int tzvvfSEgBm6322456 = -59142211;    int tzvvfSEgBm43009389 = -875322605;    int tzvvfSEgBm36697213 = -618222695;    int tzvvfSEgBm34664716 = -439693365;    int tzvvfSEgBm68298749 = -338945234;    int tzvvfSEgBm23738617 = -931865960;    int tzvvfSEgBm98686630 = -964469343;    int tzvvfSEgBm8271375 = -353774700;    int tzvvfSEgBm24543226 = -789058399;    int tzvvfSEgBm52061176 = -608149106;    int tzvvfSEgBm88360620 = -468380163;    int tzvvfSEgBm68267153 = -158602344;    int tzvvfSEgBm27555200 = -62032783;    int tzvvfSEgBm62402035 = -284167507;    int tzvvfSEgBm91689365 = -388398790;    int tzvvfSEgBm4589511 = -917795019;    int tzvvfSEgBm58359517 = -916967050;    int tzvvfSEgBm25542134 = -519708328;    int tzvvfSEgBm53823477 = -32451814;    int tzvvfSEgBm76270989 = -504818843;    int tzvvfSEgBm32883715 = -273580202;    int tzvvfSEgBm39720939 = -54736680;    int tzvvfSEgBm94045356 = -286070580;    int tzvvfSEgBm54999932 = -811224796;    int tzvvfSEgBm58872283 = -575464658;    int tzvvfSEgBm81409724 = -71794478;    int tzvvfSEgBm78726707 = -450882300;    int tzvvfSEgBm91367437 = -837335472;    int tzvvfSEgBm89170829 = -709271098;    int tzvvfSEgBm79510870 = -443361221;    int tzvvfSEgBm68497581 = -182199802;     tzvvfSEgBm77657899 = tzvvfSEgBm30842778;     tzvvfSEgBm30842778 = tzvvfSEgBm49684543;     tzvvfSEgBm49684543 = tzvvfSEgBm89831077;     tzvvfSEgBm89831077 = tzvvfSEgBm95559146;     tzvvfSEgBm95559146 = tzvvfSEgBm41768501;     tzvvfSEgBm41768501 = tzvvfSEgBm28235589;     tzvvfSEgBm28235589 = tzvvfSEgBm69014453;     tzvvfSEgBm69014453 = tzvvfSEgBm10561633;     tzvvfSEgBm10561633 = tzvvfSEgBm15937272;     tzvvfSEgBm15937272 = tzvvfSEgBm48155210;     tzvvfSEgBm48155210 = tzvvfSEgBm43956803;     tzvvfSEgBm43956803 = tzvvfSEgBm46676109;     tzvvfSEgBm46676109 = tzvvfSEgBm42545329;     tzvvfSEgBm42545329 = tzvvfSEgBm92035117;     tzvvfSEgBm92035117 = tzvvfSEgBm10687941;     tzvvfSEgBm10687941 = tzvvfSEgBm38630164;     tzvvfSEgBm38630164 = tzvvfSEgBm9959685;     tzvvfSEgBm9959685 = tzvvfSEgBm27384482;     tzvvfSEgBm27384482 = tzvvfSEgBm6055338;     tzvvfSEgBm6055338 = tzvvfSEgBm73340128;     tzvvfSEgBm73340128 = tzvvfSEgBm15764877;     tzvvfSEgBm15764877 = tzvvfSEgBm84851055;     tzvvfSEgBm84851055 = tzvvfSEgBm76051347;     tzvvfSEgBm76051347 = tzvvfSEgBm42898058;     tzvvfSEgBm42898058 = tzvvfSEgBm79136460;     tzvvfSEgBm79136460 = tzvvfSEgBm64092492;     tzvvfSEgBm64092492 = tzvvfSEgBm24645576;     tzvvfSEgBm24645576 = tzvvfSEgBm85728392;     tzvvfSEgBm85728392 = tzvvfSEgBm92238959;     tzvvfSEgBm92238959 = tzvvfSEgBm35140312;     tzvvfSEgBm35140312 = tzvvfSEgBm92662620;     tzvvfSEgBm92662620 = tzvvfSEgBm63764837;     tzvvfSEgBm63764837 = tzvvfSEgBm41450949;     tzvvfSEgBm41450949 = tzvvfSEgBm94776737;     tzvvfSEgBm94776737 = tzvvfSEgBm85712661;     tzvvfSEgBm85712661 = tzvvfSEgBm67678552;     tzvvfSEgBm67678552 = tzvvfSEgBm1932951;     tzvvfSEgBm1932951 = tzvvfSEgBm75294969;     tzvvfSEgBm75294969 = tzvvfSEgBm59085732;     tzvvfSEgBm59085732 = tzvvfSEgBm82316720;     tzvvfSEgBm82316720 = tzvvfSEgBm74653498;     tzvvfSEgBm74653498 = tzvvfSEgBm7493503;     tzvvfSEgBm7493503 = tzvvfSEgBm60307830;     tzvvfSEgBm60307830 = tzvvfSEgBm23990171;     tzvvfSEgBm23990171 = tzvvfSEgBm54537438;     tzvvfSEgBm54537438 = tzvvfSEgBm10869307;     tzvvfSEgBm10869307 = tzvvfSEgBm36537292;     tzvvfSEgBm36537292 = tzvvfSEgBm62243541;     tzvvfSEgBm62243541 = tzvvfSEgBm94039026;     tzvvfSEgBm94039026 = tzvvfSEgBm87649449;     tzvvfSEgBm87649449 = tzvvfSEgBm76780794;     tzvvfSEgBm76780794 = tzvvfSEgBm67120487;     tzvvfSEgBm67120487 = tzvvfSEgBm9941361;     tzvvfSEgBm9941361 = tzvvfSEgBm65179959;     tzvvfSEgBm65179959 = tzvvfSEgBm61893022;     tzvvfSEgBm61893022 = tzvvfSEgBm45991723;     tzvvfSEgBm45991723 = tzvvfSEgBm73633196;     tzvvfSEgBm73633196 = tzvvfSEgBm46933019;     tzvvfSEgBm46933019 = tzvvfSEgBm16422686;     tzvvfSEgBm16422686 = tzvvfSEgBm77676008;     tzvvfSEgBm77676008 = tzvvfSEgBm3590013;     tzvvfSEgBm3590013 = tzvvfSEgBm83286060;     tzvvfSEgBm83286060 = tzvvfSEgBm18322674;     tzvvfSEgBm18322674 = tzvvfSEgBm80796959;     tzvvfSEgBm80796959 = tzvvfSEgBm55492589;     tzvvfSEgBm55492589 = tzvvfSEgBm80191965;     tzvvfSEgBm80191965 = tzvvfSEgBm5225160;     tzvvfSEgBm5225160 = tzvvfSEgBm47768591;     tzvvfSEgBm47768591 = tzvvfSEgBm6322456;     tzvvfSEgBm6322456 = tzvvfSEgBm43009389;     tzvvfSEgBm43009389 = tzvvfSEgBm36697213;     tzvvfSEgBm36697213 = tzvvfSEgBm34664716;     tzvvfSEgBm34664716 = tzvvfSEgBm68298749;     tzvvfSEgBm68298749 = tzvvfSEgBm23738617;     tzvvfSEgBm23738617 = tzvvfSEgBm98686630;     tzvvfSEgBm98686630 = tzvvfSEgBm8271375;     tzvvfSEgBm8271375 = tzvvfSEgBm24543226;     tzvvfSEgBm24543226 = tzvvfSEgBm52061176;     tzvvfSEgBm52061176 = tzvvfSEgBm88360620;     tzvvfSEgBm88360620 = tzvvfSEgBm68267153;     tzvvfSEgBm68267153 = tzvvfSEgBm27555200;     tzvvfSEgBm27555200 = tzvvfSEgBm62402035;     tzvvfSEgBm62402035 = tzvvfSEgBm91689365;     tzvvfSEgBm91689365 = tzvvfSEgBm4589511;     tzvvfSEgBm4589511 = tzvvfSEgBm58359517;     tzvvfSEgBm58359517 = tzvvfSEgBm25542134;     tzvvfSEgBm25542134 = tzvvfSEgBm53823477;     tzvvfSEgBm53823477 = tzvvfSEgBm76270989;     tzvvfSEgBm76270989 = tzvvfSEgBm32883715;     tzvvfSEgBm32883715 = tzvvfSEgBm39720939;     tzvvfSEgBm39720939 = tzvvfSEgBm94045356;     tzvvfSEgBm94045356 = tzvvfSEgBm54999932;     tzvvfSEgBm54999932 = tzvvfSEgBm58872283;     tzvvfSEgBm58872283 = tzvvfSEgBm81409724;     tzvvfSEgBm81409724 = tzvvfSEgBm78726707;     tzvvfSEgBm78726707 = tzvvfSEgBm91367437;     tzvvfSEgBm91367437 = tzvvfSEgBm89170829;     tzvvfSEgBm89170829 = tzvvfSEgBm79510870;     tzvvfSEgBm79510870 = tzvvfSEgBm68497581;     tzvvfSEgBm68497581 = tzvvfSEgBm77657899;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void skDvtRGlHI75165062() {     int DZPZoUyiRe45017 = -916603765;    int DZPZoUyiRe15190279 = -886457436;    int DZPZoUyiRe78453822 = -263198134;    int DZPZoUyiRe4837766 = -391275595;    int DZPZoUyiRe93733400 = 70269017;    int DZPZoUyiRe56651618 = -300133785;    int DZPZoUyiRe20391328 = -358710435;    int DZPZoUyiRe44648312 = -383557533;    int DZPZoUyiRe7724764 = 64034136;    int DZPZoUyiRe7700856 = -11314719;    int DZPZoUyiRe85697069 = -406786278;    int DZPZoUyiRe99859751 = 85188175;    int DZPZoUyiRe13684289 = -281425101;    int DZPZoUyiRe85160081 = -883412377;    int DZPZoUyiRe28770394 = -97766857;    int DZPZoUyiRe54943853 = -830338655;    int DZPZoUyiRe88453515 = -824212067;    int DZPZoUyiRe15936101 = -444179051;    int DZPZoUyiRe68666112 = -984037336;    int DZPZoUyiRe84571620 = 94177405;    int DZPZoUyiRe74223235 = -662032295;    int DZPZoUyiRe79580095 = -114712943;    int DZPZoUyiRe88144378 = -857584720;    int DZPZoUyiRe2551135 = -300259219;    int DZPZoUyiRe78438305 = -988942433;    int DZPZoUyiRe67439455 = -905289868;    int DZPZoUyiRe11181913 = -958916402;    int DZPZoUyiRe30557027 = 45262299;    int DZPZoUyiRe92620953 = -388477835;    int DZPZoUyiRe1396140 = -425164859;    int DZPZoUyiRe79431117 = 99194971;    int DZPZoUyiRe50203811 = -341965354;    int DZPZoUyiRe44803508 = 66012408;    int DZPZoUyiRe839637 = -404927310;    int DZPZoUyiRe71006102 = -909589797;    int DZPZoUyiRe27405530 = -626542310;    int DZPZoUyiRe61118264 = -306296460;    int DZPZoUyiRe55559052 = -796335024;    int DZPZoUyiRe23603101 = -953789027;    int DZPZoUyiRe79271446 = -546214006;    int DZPZoUyiRe11074091 = -224499847;    int DZPZoUyiRe63161889 = -463984335;    int DZPZoUyiRe13043695 = -143702514;    int DZPZoUyiRe59751949 = -889809378;    int DZPZoUyiRe91973437 = 62739266;    int DZPZoUyiRe62637720 = -981232241;    int DZPZoUyiRe23166705 = -781203554;    int DZPZoUyiRe81135967 = -795832118;    int DZPZoUyiRe56703446 = -831832439;    int DZPZoUyiRe91122772 = -797842186;    int DZPZoUyiRe96518665 = -942150251;    int DZPZoUyiRe50632052 = -333341110;    int DZPZoUyiRe45434890 = -942096902;    int DZPZoUyiRe70227905 = -647873092;    int DZPZoUyiRe941827 = -716685732;    int DZPZoUyiRe20464921 = -701890823;    int DZPZoUyiRe27045900 = 71127283;    int DZPZoUyiRe75902687 = -962938916;    int DZPZoUyiRe26399460 = -402333163;    int DZPZoUyiRe26293946 = -24441115;    int DZPZoUyiRe45469705 = -341217384;    int DZPZoUyiRe89834301 = -303972734;    int DZPZoUyiRe52027358 = -995079698;    int DZPZoUyiRe6328625 = -510801005;    int DZPZoUyiRe28269739 = -10509690;    int DZPZoUyiRe35493258 = 35179075;    int DZPZoUyiRe55056244 = -980824233;    int DZPZoUyiRe12844653 = -876497791;    int DZPZoUyiRe14153979 = -973822580;    int DZPZoUyiRe1364864 = -471224547;    int DZPZoUyiRe93825588 = -424042196;    int DZPZoUyiRe32894464 = 72122957;    int DZPZoUyiRe92333000 = -490390024;    int DZPZoUyiRe89394666 = -337823330;    int DZPZoUyiRe73497529 = -681322748;    int DZPZoUyiRe11061347 = -98047960;    int DZPZoUyiRe66536401 = -971010429;    int DZPZoUyiRe28392430 = -967775342;    int DZPZoUyiRe10577697 = -262998486;    int DZPZoUyiRe15800586 = 92289807;    int DZPZoUyiRe44272751 = -24086315;    int DZPZoUyiRe30045946 = -63084285;    int DZPZoUyiRe73853580 = -122905263;    int DZPZoUyiRe1498181 = -590635650;    int DZPZoUyiRe4877474 = -483014609;    int DZPZoUyiRe28799066 = -567463920;    int DZPZoUyiRe4768922 = -399868453;    int DZPZoUyiRe74575602 = -286114501;    int DZPZoUyiRe99897809 = -688241578;    int DZPZoUyiRe50541181 = -107698975;    int DZPZoUyiRe359631 = -597669594;    int DZPZoUyiRe85215577 = -343357544;    int DZPZoUyiRe29159592 = -294001861;    int DZPZoUyiRe97309155 = -829347913;    int DZPZoUyiRe33801742 = -104996623;    int DZPZoUyiRe21239790 = -920527114;    int DZPZoUyiRe11134531 = -468904638;    int DZPZoUyiRe6715070 = -632901509;    int DZPZoUyiRe31482211 = -779299689;    int DZPZoUyiRe56480179 = -916603765;     DZPZoUyiRe45017 = DZPZoUyiRe15190279;     DZPZoUyiRe15190279 = DZPZoUyiRe78453822;     DZPZoUyiRe78453822 = DZPZoUyiRe4837766;     DZPZoUyiRe4837766 = DZPZoUyiRe93733400;     DZPZoUyiRe93733400 = DZPZoUyiRe56651618;     DZPZoUyiRe56651618 = DZPZoUyiRe20391328;     DZPZoUyiRe20391328 = DZPZoUyiRe44648312;     DZPZoUyiRe44648312 = DZPZoUyiRe7724764;     DZPZoUyiRe7724764 = DZPZoUyiRe7700856;     DZPZoUyiRe7700856 = DZPZoUyiRe85697069;     DZPZoUyiRe85697069 = DZPZoUyiRe99859751;     DZPZoUyiRe99859751 = DZPZoUyiRe13684289;     DZPZoUyiRe13684289 = DZPZoUyiRe85160081;     DZPZoUyiRe85160081 = DZPZoUyiRe28770394;     DZPZoUyiRe28770394 = DZPZoUyiRe54943853;     DZPZoUyiRe54943853 = DZPZoUyiRe88453515;     DZPZoUyiRe88453515 = DZPZoUyiRe15936101;     DZPZoUyiRe15936101 = DZPZoUyiRe68666112;     DZPZoUyiRe68666112 = DZPZoUyiRe84571620;     DZPZoUyiRe84571620 = DZPZoUyiRe74223235;     DZPZoUyiRe74223235 = DZPZoUyiRe79580095;     DZPZoUyiRe79580095 = DZPZoUyiRe88144378;     DZPZoUyiRe88144378 = DZPZoUyiRe2551135;     DZPZoUyiRe2551135 = DZPZoUyiRe78438305;     DZPZoUyiRe78438305 = DZPZoUyiRe67439455;     DZPZoUyiRe67439455 = DZPZoUyiRe11181913;     DZPZoUyiRe11181913 = DZPZoUyiRe30557027;     DZPZoUyiRe30557027 = DZPZoUyiRe92620953;     DZPZoUyiRe92620953 = DZPZoUyiRe1396140;     DZPZoUyiRe1396140 = DZPZoUyiRe79431117;     DZPZoUyiRe79431117 = DZPZoUyiRe50203811;     DZPZoUyiRe50203811 = DZPZoUyiRe44803508;     DZPZoUyiRe44803508 = DZPZoUyiRe839637;     DZPZoUyiRe839637 = DZPZoUyiRe71006102;     DZPZoUyiRe71006102 = DZPZoUyiRe27405530;     DZPZoUyiRe27405530 = DZPZoUyiRe61118264;     DZPZoUyiRe61118264 = DZPZoUyiRe55559052;     DZPZoUyiRe55559052 = DZPZoUyiRe23603101;     DZPZoUyiRe23603101 = DZPZoUyiRe79271446;     DZPZoUyiRe79271446 = DZPZoUyiRe11074091;     DZPZoUyiRe11074091 = DZPZoUyiRe63161889;     DZPZoUyiRe63161889 = DZPZoUyiRe13043695;     DZPZoUyiRe13043695 = DZPZoUyiRe59751949;     DZPZoUyiRe59751949 = DZPZoUyiRe91973437;     DZPZoUyiRe91973437 = DZPZoUyiRe62637720;     DZPZoUyiRe62637720 = DZPZoUyiRe23166705;     DZPZoUyiRe23166705 = DZPZoUyiRe81135967;     DZPZoUyiRe81135967 = DZPZoUyiRe56703446;     DZPZoUyiRe56703446 = DZPZoUyiRe91122772;     DZPZoUyiRe91122772 = DZPZoUyiRe96518665;     DZPZoUyiRe96518665 = DZPZoUyiRe50632052;     DZPZoUyiRe50632052 = DZPZoUyiRe45434890;     DZPZoUyiRe45434890 = DZPZoUyiRe70227905;     DZPZoUyiRe70227905 = DZPZoUyiRe941827;     DZPZoUyiRe941827 = DZPZoUyiRe20464921;     DZPZoUyiRe20464921 = DZPZoUyiRe27045900;     DZPZoUyiRe27045900 = DZPZoUyiRe75902687;     DZPZoUyiRe75902687 = DZPZoUyiRe26399460;     DZPZoUyiRe26399460 = DZPZoUyiRe26293946;     DZPZoUyiRe26293946 = DZPZoUyiRe45469705;     DZPZoUyiRe45469705 = DZPZoUyiRe89834301;     DZPZoUyiRe89834301 = DZPZoUyiRe52027358;     DZPZoUyiRe52027358 = DZPZoUyiRe6328625;     DZPZoUyiRe6328625 = DZPZoUyiRe28269739;     DZPZoUyiRe28269739 = DZPZoUyiRe35493258;     DZPZoUyiRe35493258 = DZPZoUyiRe55056244;     DZPZoUyiRe55056244 = DZPZoUyiRe12844653;     DZPZoUyiRe12844653 = DZPZoUyiRe14153979;     DZPZoUyiRe14153979 = DZPZoUyiRe1364864;     DZPZoUyiRe1364864 = DZPZoUyiRe93825588;     DZPZoUyiRe93825588 = DZPZoUyiRe32894464;     DZPZoUyiRe32894464 = DZPZoUyiRe92333000;     DZPZoUyiRe92333000 = DZPZoUyiRe89394666;     DZPZoUyiRe89394666 = DZPZoUyiRe73497529;     DZPZoUyiRe73497529 = DZPZoUyiRe11061347;     DZPZoUyiRe11061347 = DZPZoUyiRe66536401;     DZPZoUyiRe66536401 = DZPZoUyiRe28392430;     DZPZoUyiRe28392430 = DZPZoUyiRe10577697;     DZPZoUyiRe10577697 = DZPZoUyiRe15800586;     DZPZoUyiRe15800586 = DZPZoUyiRe44272751;     DZPZoUyiRe44272751 = DZPZoUyiRe30045946;     DZPZoUyiRe30045946 = DZPZoUyiRe73853580;     DZPZoUyiRe73853580 = DZPZoUyiRe1498181;     DZPZoUyiRe1498181 = DZPZoUyiRe4877474;     DZPZoUyiRe4877474 = DZPZoUyiRe28799066;     DZPZoUyiRe28799066 = DZPZoUyiRe4768922;     DZPZoUyiRe4768922 = DZPZoUyiRe74575602;     DZPZoUyiRe74575602 = DZPZoUyiRe99897809;     DZPZoUyiRe99897809 = DZPZoUyiRe50541181;     DZPZoUyiRe50541181 = DZPZoUyiRe359631;     DZPZoUyiRe359631 = DZPZoUyiRe85215577;     DZPZoUyiRe85215577 = DZPZoUyiRe29159592;     DZPZoUyiRe29159592 = DZPZoUyiRe97309155;     DZPZoUyiRe97309155 = DZPZoUyiRe33801742;     DZPZoUyiRe33801742 = DZPZoUyiRe21239790;     DZPZoUyiRe21239790 = DZPZoUyiRe11134531;     DZPZoUyiRe11134531 = DZPZoUyiRe6715070;     DZPZoUyiRe6715070 = DZPZoUyiRe31482211;     DZPZoUyiRe31482211 = DZPZoUyiRe56480179;     DZPZoUyiRe56480179 = DZPZoUyiRe45017;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void czanOEUYCs11810037() {     int CQQQSfCKND22432134 = -551007728;    int CQQQSfCKND99537778 = -835000491;    int CQQQSfCKND7223101 = -502851978;    int CQQQSfCKND19844454 = -397683127;    int CQQQSfCKND91907654 = -520787984;    int CQQQSfCKND71534735 = -160742163;    int CQQQSfCKND12547067 = -180697741;    int CQQQSfCKND20282171 = -854154177;    int CQQQSfCKND4887896 = -920161903;    int CQQQSfCKND99464440 = -854056052;    int CQQQSfCKND23238929 = -403410679;    int CQQQSfCKND55762701 = -165557118;    int CQQQSfCKND80692469 = -900142420;    int CQQQSfCKND27774833 = -311121134;    int CQQQSfCKND65505671 = -825244570;    int CQQQSfCKND99199766 = -112006197;    int CQQQSfCKND38276868 = -1491886;    int CQQQSfCKND21912517 = -895921742;    int CQQQSfCKND9947744 = -19139461;    int CQQQSfCKND63087904 = -158457048;    int CQQQSfCKND75106342 = -653519603;    int CQQQSfCKND43395314 = -619479256;    int CQQQSfCKND91437701 = 66334688;    int CQQQSfCKND29050922 = -864252076;    int CQQQSfCKND13978552 = -610501558;    int CQQQSfCKND55742449 = -249184092;    int CQQQSfCKND58271333 = -716502895;    int CQQQSfCKND36468477 = -843192155;    int CQQQSfCKND99513514 = -732734954;    int CQQQSfCKND10553319 = -345460725;    int CQQQSfCKND23721923 = 17878819;    int CQQQSfCKND7745003 = -994036708;    int CQQQSfCKND25842178 = -463331087;    int CQQQSfCKND60228323 = -729054160;    int CQQQSfCKND47235467 = -973346220;    int CQQQSfCKND69098399 = -941937686;    int CQQQSfCKND54557977 = -39244411;    int CQQQSfCKND9185153 = -663960496;    int CQQQSfCKND71911231 = -254835059;    int CQQQSfCKND99457161 = -682438035;    int CQQQSfCKND39831462 = -727677513;    int CQQQSfCKND51670280 = -221893026;    int CQQQSfCKND18593887 = -31233097;    int CQQQSfCKND59196068 = -887173027;    int CQQQSfCKND59956704 = -846404212;    int CQQQSfCKND70738001 = -63461336;    int CQQQSfCKND35464102 = -259613807;    int CQQQSfCKND25734642 = -552367109;    int CQQQSfCKND51163351 = -781549137;    int CQQQSfCKND88206518 = -939862445;    int CQQQSfCKND5387882 = -197226528;    int CQQQSfCKND24483309 = -764160392;    int CQQQSfCKND23749294 = -614008132;    int CQQQSfCKND30514451 = -923553901;    int CQQQSfCKND36703694 = -857389848;    int CQQQSfCKND79036820 = -931528472;    int CQQQSfCKND8100077 = -801335179;    int CQQQSfCKND78172179 = -638599903;    int CQQQSfCKND5865902 = -787181570;    int CQQQSfCKND36165205 = -171603892;    int CQQQSfCKND13263402 = -444239269;    int CQQQSfCKND76078590 = -337505587;    int CQQQSfCKND20768656 = -21419223;    int CQQQSfCKND94334576 = -474701178;    int CQQQSfCKND75742518 = -771934871;    int CQQQSfCKND15493927 = -409373971;    int CQQQSfCKND29920523 = -702226032;    int CQQQSfCKND20464147 = -71088261;    int CQQQSfCKND80539366 = -337774914;    int CQQQSfCKND96407271 = -883306884;    int CQQQSfCKND44641789 = 27238213;    int CQQQSfCKND29091715 = -337531391;    int CQQQSfCKND50001285 = -541086683;    int CQQQSfCKND10490583 = -336701426;    int CQQQSfCKND23256442 = -430779535;    int CQQQSfCKND23436063 = -331626578;    int CQQQSfCKND24801428 = -488246159;    int CQQQSfCKND32241633 = -46492285;    int CQQQSfCKND69094217 = 82152135;    int CQQQSfCKND43240550 = -447040222;    int CQQQSfCKND20278348 = -989570286;    int CQQQSfCKND32536692 = -64135787;    int CQQQSfCKND85305125 = 38356982;    int CQQQSfCKND11306997 = -792872510;    int CQQQSfCKND5165437 = -48234198;    int CQQQSfCKND99238613 = -217960790;    int CQQQSfCKND83995709 = -280028577;    int CQQQSfCKND95327727 = -539777187;    int CQQQSfCKND23524630 = -871664313;    int CQQQSfCKND68198646 = 58182252;    int CQQQSfCKND60998322 = -40602508;    int CQQQSfCKND76385797 = -400644509;    int CQQQSfCKND3319251 = -876778926;    int CQQQSfCKND35746027 = 16768833;    int CQQQSfCKND86193760 = -138198767;    int CQQQSfCKND63752871 = -290171927;    int CQQQSfCKND30901624 = -100473804;    int CQQQSfCKND24259310 = -556531920;    int CQQQSfCKND83453550 = -15238157;    int CQQQSfCKND44462777 = -551007728;     CQQQSfCKND22432134 = CQQQSfCKND99537778;     CQQQSfCKND99537778 = CQQQSfCKND7223101;     CQQQSfCKND7223101 = CQQQSfCKND19844454;     CQQQSfCKND19844454 = CQQQSfCKND91907654;     CQQQSfCKND91907654 = CQQQSfCKND71534735;     CQQQSfCKND71534735 = CQQQSfCKND12547067;     CQQQSfCKND12547067 = CQQQSfCKND20282171;     CQQQSfCKND20282171 = CQQQSfCKND4887896;     CQQQSfCKND4887896 = CQQQSfCKND99464440;     CQQQSfCKND99464440 = CQQQSfCKND23238929;     CQQQSfCKND23238929 = CQQQSfCKND55762701;     CQQQSfCKND55762701 = CQQQSfCKND80692469;     CQQQSfCKND80692469 = CQQQSfCKND27774833;     CQQQSfCKND27774833 = CQQQSfCKND65505671;     CQQQSfCKND65505671 = CQQQSfCKND99199766;     CQQQSfCKND99199766 = CQQQSfCKND38276868;     CQQQSfCKND38276868 = CQQQSfCKND21912517;     CQQQSfCKND21912517 = CQQQSfCKND9947744;     CQQQSfCKND9947744 = CQQQSfCKND63087904;     CQQQSfCKND63087904 = CQQQSfCKND75106342;     CQQQSfCKND75106342 = CQQQSfCKND43395314;     CQQQSfCKND43395314 = CQQQSfCKND91437701;     CQQQSfCKND91437701 = CQQQSfCKND29050922;     CQQQSfCKND29050922 = CQQQSfCKND13978552;     CQQQSfCKND13978552 = CQQQSfCKND55742449;     CQQQSfCKND55742449 = CQQQSfCKND58271333;     CQQQSfCKND58271333 = CQQQSfCKND36468477;     CQQQSfCKND36468477 = CQQQSfCKND99513514;     CQQQSfCKND99513514 = CQQQSfCKND10553319;     CQQQSfCKND10553319 = CQQQSfCKND23721923;     CQQQSfCKND23721923 = CQQQSfCKND7745003;     CQQQSfCKND7745003 = CQQQSfCKND25842178;     CQQQSfCKND25842178 = CQQQSfCKND60228323;     CQQQSfCKND60228323 = CQQQSfCKND47235467;     CQQQSfCKND47235467 = CQQQSfCKND69098399;     CQQQSfCKND69098399 = CQQQSfCKND54557977;     CQQQSfCKND54557977 = CQQQSfCKND9185153;     CQQQSfCKND9185153 = CQQQSfCKND71911231;     CQQQSfCKND71911231 = CQQQSfCKND99457161;     CQQQSfCKND99457161 = CQQQSfCKND39831462;     CQQQSfCKND39831462 = CQQQSfCKND51670280;     CQQQSfCKND51670280 = CQQQSfCKND18593887;     CQQQSfCKND18593887 = CQQQSfCKND59196068;     CQQQSfCKND59196068 = CQQQSfCKND59956704;     CQQQSfCKND59956704 = CQQQSfCKND70738001;     CQQQSfCKND70738001 = CQQQSfCKND35464102;     CQQQSfCKND35464102 = CQQQSfCKND25734642;     CQQQSfCKND25734642 = CQQQSfCKND51163351;     CQQQSfCKND51163351 = CQQQSfCKND88206518;     CQQQSfCKND88206518 = CQQQSfCKND5387882;     CQQQSfCKND5387882 = CQQQSfCKND24483309;     CQQQSfCKND24483309 = CQQQSfCKND23749294;     CQQQSfCKND23749294 = CQQQSfCKND30514451;     CQQQSfCKND30514451 = CQQQSfCKND36703694;     CQQQSfCKND36703694 = CQQQSfCKND79036820;     CQQQSfCKND79036820 = CQQQSfCKND8100077;     CQQQSfCKND8100077 = CQQQSfCKND78172179;     CQQQSfCKND78172179 = CQQQSfCKND5865902;     CQQQSfCKND5865902 = CQQQSfCKND36165205;     CQQQSfCKND36165205 = CQQQSfCKND13263402;     CQQQSfCKND13263402 = CQQQSfCKND76078590;     CQQQSfCKND76078590 = CQQQSfCKND20768656;     CQQQSfCKND20768656 = CQQQSfCKND94334576;     CQQQSfCKND94334576 = CQQQSfCKND75742518;     CQQQSfCKND75742518 = CQQQSfCKND15493927;     CQQQSfCKND15493927 = CQQQSfCKND29920523;     CQQQSfCKND29920523 = CQQQSfCKND20464147;     CQQQSfCKND20464147 = CQQQSfCKND80539366;     CQQQSfCKND80539366 = CQQQSfCKND96407271;     CQQQSfCKND96407271 = CQQQSfCKND44641789;     CQQQSfCKND44641789 = CQQQSfCKND29091715;     CQQQSfCKND29091715 = CQQQSfCKND50001285;     CQQQSfCKND50001285 = CQQQSfCKND10490583;     CQQQSfCKND10490583 = CQQQSfCKND23256442;     CQQQSfCKND23256442 = CQQQSfCKND23436063;     CQQQSfCKND23436063 = CQQQSfCKND24801428;     CQQQSfCKND24801428 = CQQQSfCKND32241633;     CQQQSfCKND32241633 = CQQQSfCKND69094217;     CQQQSfCKND69094217 = CQQQSfCKND43240550;     CQQQSfCKND43240550 = CQQQSfCKND20278348;     CQQQSfCKND20278348 = CQQQSfCKND32536692;     CQQQSfCKND32536692 = CQQQSfCKND85305125;     CQQQSfCKND85305125 = CQQQSfCKND11306997;     CQQQSfCKND11306997 = CQQQSfCKND5165437;     CQQQSfCKND5165437 = CQQQSfCKND99238613;     CQQQSfCKND99238613 = CQQQSfCKND83995709;     CQQQSfCKND83995709 = CQQQSfCKND95327727;     CQQQSfCKND95327727 = CQQQSfCKND23524630;     CQQQSfCKND23524630 = CQQQSfCKND68198646;     CQQQSfCKND68198646 = CQQQSfCKND60998322;     CQQQSfCKND60998322 = CQQQSfCKND76385797;     CQQQSfCKND76385797 = CQQQSfCKND3319251;     CQQQSfCKND3319251 = CQQQSfCKND35746027;     CQQQSfCKND35746027 = CQQQSfCKND86193760;     CQQQSfCKND86193760 = CQQQSfCKND63752871;     CQQQSfCKND63752871 = CQQQSfCKND30901624;     CQQQSfCKND30901624 = CQQQSfCKND24259310;     CQQQSfCKND24259310 = CQQQSfCKND83453550;     CQQQSfCKND83453550 = CQQQSfCKND44462777;     CQQQSfCKND44462777 = CQQQSfCKND22432134;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void eqIhWugZhA48455011() {     int nJpFjEPpSs44819251 = -185411689;    int nJpFjEPpSs83885279 = -783543539;    int nJpFjEPpSs35992379 = -742505823;    int nJpFjEPpSs34851142 = -404090662;    int nJpFjEPpSs90081908 = -11844977;    int nJpFjEPpSs86417851 = -21350541;    int nJpFjEPpSs4702806 = -2685047;    int nJpFjEPpSs95916029 = -224750806;    int nJpFjEPpSs2051027 = -804357941;    int nJpFjEPpSs91228025 = -596797385;    int nJpFjEPpSs60780789 = -400035079;    int nJpFjEPpSs11665650 = -416302403;    int nJpFjEPpSs47700650 = -418859740;    int nJpFjEPpSs70389585 = -838829890;    int nJpFjEPpSs2240949 = -452722265;    int nJpFjEPpSs43455678 = -493673740;    int nJpFjEPpSs88100219 = -278771706;    int nJpFjEPpSs27888934 = -247664428;    int nJpFjEPpSs51229374 = -154241587;    int nJpFjEPpSs41604187 = -411091502;    int nJpFjEPpSs75989450 = -645006907;    int nJpFjEPpSs7210533 = -24245569;    int nJpFjEPpSs94731024 = -109745904;    int nJpFjEPpSs55550708 = -328244934;    int nJpFjEPpSs49518799 = -232060670;    int nJpFjEPpSs44045443 = -693078320;    int nJpFjEPpSs5360754 = -474089388;    int nJpFjEPpSs42379927 = -631646602;    int nJpFjEPpSs6406075 = 23007911;    int nJpFjEPpSs19710498 = -265756592;    int nJpFjEPpSs68012727 = -63437333;    int nJpFjEPpSs65286193 = -546108066;    int nJpFjEPpSs6880849 = -992674585;    int nJpFjEPpSs19617011 = 46818990;    int nJpFjEPpSs23464831 = 62897355;    int nJpFjEPpSs10791269 = -157333081;    int nJpFjEPpSs47997689 = -872192362;    int nJpFjEPpSs62811253 = -531585964;    int nJpFjEPpSs20219363 = -655881103;    int nJpFjEPpSs19642877 = -818662065;    int nJpFjEPpSs68588832 = -130855180;    int nJpFjEPpSs40178672 = 20198262;    int nJpFjEPpSs24144079 = 81236317;    int nJpFjEPpSs58640187 = -884536676;    int nJpFjEPpSs27939971 = -655547690;    int nJpFjEPpSs78838283 = -245690450;    int nJpFjEPpSs47761500 = -838024060;    int nJpFjEPpSs70333316 = -308902100;    int nJpFjEPpSs45623256 = -731265860;    int nJpFjEPpSs85290264 = 18117301;    int nJpFjEPpSs14257099 = -552302805;    int nJpFjEPpSs98334566 = -94979678;    int nJpFjEPpSs2063698 = -285919359;    int nJpFjEPpSs90800996 = -99234710;    int nJpFjEPpSs72465561 = -998093968;    int nJpFjEPpSs37608719 = -61166120;    int nJpFjEPpSs89154254 = -573797636;    int nJpFjEPpSs80441670 = -314260889;    int nJpFjEPpSs85332343 = -72029992;    int nJpFjEPpSs46036465 = -318766657;    int nJpFjEPpSs81057098 = -547261154;    int nJpFjEPpSs62322879 = -371038445;    int nJpFjEPpSs89509954 = -147758718;    int nJpFjEPpSs82340528 = -438601349;    int nJpFjEPpSs23215298 = -433360052;    int nJpFjEPpSs95494595 = -853927014;    int nJpFjEPpSs4784801 = -423627818;    int nJpFjEPpSs28083640 = -365678730;    int nJpFjEPpSs46924754 = -801727245;    int nJpFjEPpSs91449679 = -195389184;    int nJpFjEPpSs95457989 = -621481378;    int nJpFjEPpSs25288966 = -747185742;    int nJpFjEPpSs7669571 = -591783326;    int nJpFjEPpSs31586498 = -335579523;    int nJpFjEPpSs73015354 = -180236323;    int nJpFjEPpSs35810778 = -565205169;    int nJpFjEPpSs83066453 = -5481886;    int nJpFjEPpSs36090837 = -225209228;    int nJpFjEPpSs27610738 = -672697244;    int nJpFjEPpSs70680515 = -986370220;    int nJpFjEPpSs96283943 = -855054261;    int nJpFjEPpSs35027438 = -65187288;    int nJpFjEPpSs96756670 = -900380742;    int nJpFjEPpSs21115811 = -995109391;    int nJpFjEPpSs5453400 = -713453788;    int nJpFjEPpSs69678161 = -968457655;    int nJpFjEPpSs63222496 = -160188708;    int nJpFjEPpSs16079853 = -793439876;    int nJpFjEPpSs47151449 = 44912957;    int nJpFjEPpSs85856112 = -875936526;    int nJpFjEPpSs21637014 = -583535446;    int nJpFjEPpSs67556018 = -457931473;    int nJpFjEPpSs77478910 = -359555973;    int nJpFjEPpSs74182898 = -237114446;    int nJpFjEPpSs38585779 = -171400911;    int nJpFjEPpSs6265954 = -759816735;    int nJpFjEPpSs50668718 = -832043021;    int nJpFjEPpSs41803551 = -480162334;    int nJpFjEPpSs35424890 = -351176625;    int nJpFjEPpSs32445375 = -185411689;     nJpFjEPpSs44819251 = nJpFjEPpSs83885279;     nJpFjEPpSs83885279 = nJpFjEPpSs35992379;     nJpFjEPpSs35992379 = nJpFjEPpSs34851142;     nJpFjEPpSs34851142 = nJpFjEPpSs90081908;     nJpFjEPpSs90081908 = nJpFjEPpSs86417851;     nJpFjEPpSs86417851 = nJpFjEPpSs4702806;     nJpFjEPpSs4702806 = nJpFjEPpSs95916029;     nJpFjEPpSs95916029 = nJpFjEPpSs2051027;     nJpFjEPpSs2051027 = nJpFjEPpSs91228025;     nJpFjEPpSs91228025 = nJpFjEPpSs60780789;     nJpFjEPpSs60780789 = nJpFjEPpSs11665650;     nJpFjEPpSs11665650 = nJpFjEPpSs47700650;     nJpFjEPpSs47700650 = nJpFjEPpSs70389585;     nJpFjEPpSs70389585 = nJpFjEPpSs2240949;     nJpFjEPpSs2240949 = nJpFjEPpSs43455678;     nJpFjEPpSs43455678 = nJpFjEPpSs88100219;     nJpFjEPpSs88100219 = nJpFjEPpSs27888934;     nJpFjEPpSs27888934 = nJpFjEPpSs51229374;     nJpFjEPpSs51229374 = nJpFjEPpSs41604187;     nJpFjEPpSs41604187 = nJpFjEPpSs75989450;     nJpFjEPpSs75989450 = nJpFjEPpSs7210533;     nJpFjEPpSs7210533 = nJpFjEPpSs94731024;     nJpFjEPpSs94731024 = nJpFjEPpSs55550708;     nJpFjEPpSs55550708 = nJpFjEPpSs49518799;     nJpFjEPpSs49518799 = nJpFjEPpSs44045443;     nJpFjEPpSs44045443 = nJpFjEPpSs5360754;     nJpFjEPpSs5360754 = nJpFjEPpSs42379927;     nJpFjEPpSs42379927 = nJpFjEPpSs6406075;     nJpFjEPpSs6406075 = nJpFjEPpSs19710498;     nJpFjEPpSs19710498 = nJpFjEPpSs68012727;     nJpFjEPpSs68012727 = nJpFjEPpSs65286193;     nJpFjEPpSs65286193 = nJpFjEPpSs6880849;     nJpFjEPpSs6880849 = nJpFjEPpSs19617011;     nJpFjEPpSs19617011 = nJpFjEPpSs23464831;     nJpFjEPpSs23464831 = nJpFjEPpSs10791269;     nJpFjEPpSs10791269 = nJpFjEPpSs47997689;     nJpFjEPpSs47997689 = nJpFjEPpSs62811253;     nJpFjEPpSs62811253 = nJpFjEPpSs20219363;     nJpFjEPpSs20219363 = nJpFjEPpSs19642877;     nJpFjEPpSs19642877 = nJpFjEPpSs68588832;     nJpFjEPpSs68588832 = nJpFjEPpSs40178672;     nJpFjEPpSs40178672 = nJpFjEPpSs24144079;     nJpFjEPpSs24144079 = nJpFjEPpSs58640187;     nJpFjEPpSs58640187 = nJpFjEPpSs27939971;     nJpFjEPpSs27939971 = nJpFjEPpSs78838283;     nJpFjEPpSs78838283 = nJpFjEPpSs47761500;     nJpFjEPpSs47761500 = nJpFjEPpSs70333316;     nJpFjEPpSs70333316 = nJpFjEPpSs45623256;     nJpFjEPpSs45623256 = nJpFjEPpSs85290264;     nJpFjEPpSs85290264 = nJpFjEPpSs14257099;     nJpFjEPpSs14257099 = nJpFjEPpSs98334566;     nJpFjEPpSs98334566 = nJpFjEPpSs2063698;     nJpFjEPpSs2063698 = nJpFjEPpSs90800996;     nJpFjEPpSs90800996 = nJpFjEPpSs72465561;     nJpFjEPpSs72465561 = nJpFjEPpSs37608719;     nJpFjEPpSs37608719 = nJpFjEPpSs89154254;     nJpFjEPpSs89154254 = nJpFjEPpSs80441670;     nJpFjEPpSs80441670 = nJpFjEPpSs85332343;     nJpFjEPpSs85332343 = nJpFjEPpSs46036465;     nJpFjEPpSs46036465 = nJpFjEPpSs81057098;     nJpFjEPpSs81057098 = nJpFjEPpSs62322879;     nJpFjEPpSs62322879 = nJpFjEPpSs89509954;     nJpFjEPpSs89509954 = nJpFjEPpSs82340528;     nJpFjEPpSs82340528 = nJpFjEPpSs23215298;     nJpFjEPpSs23215298 = nJpFjEPpSs95494595;     nJpFjEPpSs95494595 = nJpFjEPpSs4784801;     nJpFjEPpSs4784801 = nJpFjEPpSs28083640;     nJpFjEPpSs28083640 = nJpFjEPpSs46924754;     nJpFjEPpSs46924754 = nJpFjEPpSs91449679;     nJpFjEPpSs91449679 = nJpFjEPpSs95457989;     nJpFjEPpSs95457989 = nJpFjEPpSs25288966;     nJpFjEPpSs25288966 = nJpFjEPpSs7669571;     nJpFjEPpSs7669571 = nJpFjEPpSs31586498;     nJpFjEPpSs31586498 = nJpFjEPpSs73015354;     nJpFjEPpSs73015354 = nJpFjEPpSs35810778;     nJpFjEPpSs35810778 = nJpFjEPpSs83066453;     nJpFjEPpSs83066453 = nJpFjEPpSs36090837;     nJpFjEPpSs36090837 = nJpFjEPpSs27610738;     nJpFjEPpSs27610738 = nJpFjEPpSs70680515;     nJpFjEPpSs70680515 = nJpFjEPpSs96283943;     nJpFjEPpSs96283943 = nJpFjEPpSs35027438;     nJpFjEPpSs35027438 = nJpFjEPpSs96756670;     nJpFjEPpSs96756670 = nJpFjEPpSs21115811;     nJpFjEPpSs21115811 = nJpFjEPpSs5453400;     nJpFjEPpSs5453400 = nJpFjEPpSs69678161;     nJpFjEPpSs69678161 = nJpFjEPpSs63222496;     nJpFjEPpSs63222496 = nJpFjEPpSs16079853;     nJpFjEPpSs16079853 = nJpFjEPpSs47151449;     nJpFjEPpSs47151449 = nJpFjEPpSs85856112;     nJpFjEPpSs85856112 = nJpFjEPpSs21637014;     nJpFjEPpSs21637014 = nJpFjEPpSs67556018;     nJpFjEPpSs67556018 = nJpFjEPpSs77478910;     nJpFjEPpSs77478910 = nJpFjEPpSs74182898;     nJpFjEPpSs74182898 = nJpFjEPpSs38585779;     nJpFjEPpSs38585779 = nJpFjEPpSs6265954;     nJpFjEPpSs6265954 = nJpFjEPpSs50668718;     nJpFjEPpSs50668718 = nJpFjEPpSs41803551;     nJpFjEPpSs41803551 = nJpFjEPpSs35424890;     nJpFjEPpSs35424890 = nJpFjEPpSs32445375;     nJpFjEPpSs32445375 = nJpFjEPpSs44819251;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void CDXmTJuFCN85099986() {     int ZdDMKNFgpR67206369 = -919815654;    int ZdDMKNFgpR68232779 = -732086600;    int ZdDMKNFgpR64761658 = -982159667;    int ZdDMKNFgpR49857831 = -410498190;    int ZdDMKNFgpR88256162 = -602901987;    int ZdDMKNFgpR1300969 = -981958919;    int ZdDMKNFgpR96858544 = -924672353;    int ZdDMKNFgpR71549887 = -695347466;    int ZdDMKNFgpR99214157 = -688553981;    int ZdDMKNFgpR82991609 = -339538718;    int ZdDMKNFgpR98322648 = -396659481;    int ZdDMKNFgpR67568598 = -667047705;    int ZdDMKNFgpR14708831 = 62422940;    int ZdDMKNFgpR13004338 = -266538648;    int ZdDMKNFgpR38976225 = -80199996;    int ZdDMKNFgpR87711591 = -875341282;    int ZdDMKNFgpR37923571 = -556051526;    int ZdDMKNFgpR33865350 = -699407123;    int ZdDMKNFgpR92511005 = -289343711;    int ZdDMKNFgpR20120471 = -663725955;    int ZdDMKNFgpR76872557 = -636494220;    int ZdDMKNFgpR71025750 = -529011882;    int ZdDMKNFgpR98024347 = -285826496;    int ZdDMKNFgpR82050495 = -892237791;    int ZdDMKNFgpR85059045 = -953619807;    int ZdDMKNFgpR32348439 = -36972540;    int ZdDMKNFgpR52450174 = -231675880;    int ZdDMKNFgpR48291377 = -420101062;    int ZdDMKNFgpR13298636 = -321249193;    int ZdDMKNFgpR28867678 = -186052459;    int ZdDMKNFgpR12303533 = -144753485;    int ZdDMKNFgpR22827384 = -98179418;    int ZdDMKNFgpR87919519 = -422018077;    int ZdDMKNFgpR79005698 = -277307860;    int ZdDMKNFgpR99694195 = -859065;    int ZdDMKNFgpR52484138 = -472728439;    int ZdDMKNFgpR41437401 = -605140314;    int ZdDMKNFgpR16437355 = -399211439;    int ZdDMKNFgpR68527494 = 43072878;    int ZdDMKNFgpR39828591 = -954886094;    int ZdDMKNFgpR97346203 = -634032846;    int ZdDMKNFgpR28687063 = -837710408;    int ZdDMKNFgpR29694271 = -906294265;    int ZdDMKNFgpR58084307 = -881900325;    int ZdDMKNFgpR95923237 = -464691167;    int ZdDMKNFgpR86938565 = -427919527;    int ZdDMKNFgpR60058896 = -316434313;    int ZdDMKNFgpR14931991 = -65437090;    int ZdDMKNFgpR40083161 = -680982534;    int ZdDMKNFgpR82374009 = -123902963;    int ZdDMKNFgpR23126315 = -907379082;    int ZdDMKNFgpR72185823 = -525798956;    int ZdDMKNFgpR80378100 = 42169408;    int ZdDMKNFgpR51087541 = -374915518;    int ZdDMKNFgpR8227429 = -38798078;    int ZdDMKNFgpR96180618 = -290803772;    int ZdDMKNFgpR70208431 = -346260104;    int ZdDMKNFgpR82711162 = 10078124;    int ZdDMKNFgpR64798785 = -456878384;    int ZdDMKNFgpR55907724 = -465929447;    int ZdDMKNFgpR48850794 = -650283039;    int ZdDMKNFgpR48567168 = -404571292;    int ZdDMKNFgpR58251252 = -274098273;    int ZdDMKNFgpR70346480 = -402501523;    int ZdDMKNFgpR70688077 = -94785233;    int ZdDMKNFgpR75495264 = -198480064;    int ZdDMKNFgpR79649079 = -145029629;    int ZdDMKNFgpR35703133 = -660269200;    int ZdDMKNFgpR13310142 = -165679583;    int ZdDMKNFgpR86492087 = -607471557;    int ZdDMKNFgpR46274190 = -170200969;    int ZdDMKNFgpR21486217 = -56840087;    int ZdDMKNFgpR65337855 = -642480001;    int ZdDMKNFgpR52682415 = -334457618;    int ZdDMKNFgpR22774267 = 70306890;    int ZdDMKNFgpR48185494 = -798783812;    int ZdDMKNFgpR41331480 = -622717618;    int ZdDMKNFgpR39940041 = -403926171;    int ZdDMKNFgpR86127258 = -327546624;    int ZdDMKNFgpR98120480 = -425700280;    int ZdDMKNFgpR72289542 = -720538228;    int ZdDMKNFgpR37518184 = -66238791;    int ZdDMKNFgpR8208216 = -739118528;    int ZdDMKNFgpR30924627 = -97346231;    int ZdDMKNFgpR5741363 = -278673378;    int ZdDMKNFgpR40117709 = -618954530;    int ZdDMKNFgpR42449284 = -40348826;    int ZdDMKNFgpR36831978 = 52897441;    int ZdDMKNFgpR70778269 = -138509783;    int ZdDMKNFgpR3513578 = -710055293;    int ZdDMKNFgpR82275706 = -26468335;    int ZdDMKNFgpR58726239 = -515218438;    int ZdDMKNFgpR51638569 = -942333056;    int ZdDMKNFgpR12619770 = -490997676;    int ZdDMKNFgpR90977796 = -204603055;    int ZdDMKNFgpR48779036 = -129461554;    int ZdDMKNFgpR70435811 = -463612135;    int ZdDMKNFgpR59347791 = -403792743;    int ZdDMKNFgpR87396229 = -687115093;    int ZdDMKNFgpR20427973 = -919815654;     ZdDMKNFgpR67206369 = ZdDMKNFgpR68232779;     ZdDMKNFgpR68232779 = ZdDMKNFgpR64761658;     ZdDMKNFgpR64761658 = ZdDMKNFgpR49857831;     ZdDMKNFgpR49857831 = ZdDMKNFgpR88256162;     ZdDMKNFgpR88256162 = ZdDMKNFgpR1300969;     ZdDMKNFgpR1300969 = ZdDMKNFgpR96858544;     ZdDMKNFgpR96858544 = ZdDMKNFgpR71549887;     ZdDMKNFgpR71549887 = ZdDMKNFgpR99214157;     ZdDMKNFgpR99214157 = ZdDMKNFgpR82991609;     ZdDMKNFgpR82991609 = ZdDMKNFgpR98322648;     ZdDMKNFgpR98322648 = ZdDMKNFgpR67568598;     ZdDMKNFgpR67568598 = ZdDMKNFgpR14708831;     ZdDMKNFgpR14708831 = ZdDMKNFgpR13004338;     ZdDMKNFgpR13004338 = ZdDMKNFgpR38976225;     ZdDMKNFgpR38976225 = ZdDMKNFgpR87711591;     ZdDMKNFgpR87711591 = ZdDMKNFgpR37923571;     ZdDMKNFgpR37923571 = ZdDMKNFgpR33865350;     ZdDMKNFgpR33865350 = ZdDMKNFgpR92511005;     ZdDMKNFgpR92511005 = ZdDMKNFgpR20120471;     ZdDMKNFgpR20120471 = ZdDMKNFgpR76872557;     ZdDMKNFgpR76872557 = ZdDMKNFgpR71025750;     ZdDMKNFgpR71025750 = ZdDMKNFgpR98024347;     ZdDMKNFgpR98024347 = ZdDMKNFgpR82050495;     ZdDMKNFgpR82050495 = ZdDMKNFgpR85059045;     ZdDMKNFgpR85059045 = ZdDMKNFgpR32348439;     ZdDMKNFgpR32348439 = ZdDMKNFgpR52450174;     ZdDMKNFgpR52450174 = ZdDMKNFgpR48291377;     ZdDMKNFgpR48291377 = ZdDMKNFgpR13298636;     ZdDMKNFgpR13298636 = ZdDMKNFgpR28867678;     ZdDMKNFgpR28867678 = ZdDMKNFgpR12303533;     ZdDMKNFgpR12303533 = ZdDMKNFgpR22827384;     ZdDMKNFgpR22827384 = ZdDMKNFgpR87919519;     ZdDMKNFgpR87919519 = ZdDMKNFgpR79005698;     ZdDMKNFgpR79005698 = ZdDMKNFgpR99694195;     ZdDMKNFgpR99694195 = ZdDMKNFgpR52484138;     ZdDMKNFgpR52484138 = ZdDMKNFgpR41437401;     ZdDMKNFgpR41437401 = ZdDMKNFgpR16437355;     ZdDMKNFgpR16437355 = ZdDMKNFgpR68527494;     ZdDMKNFgpR68527494 = ZdDMKNFgpR39828591;     ZdDMKNFgpR39828591 = ZdDMKNFgpR97346203;     ZdDMKNFgpR97346203 = ZdDMKNFgpR28687063;     ZdDMKNFgpR28687063 = ZdDMKNFgpR29694271;     ZdDMKNFgpR29694271 = ZdDMKNFgpR58084307;     ZdDMKNFgpR58084307 = ZdDMKNFgpR95923237;     ZdDMKNFgpR95923237 = ZdDMKNFgpR86938565;     ZdDMKNFgpR86938565 = ZdDMKNFgpR60058896;     ZdDMKNFgpR60058896 = ZdDMKNFgpR14931991;     ZdDMKNFgpR14931991 = ZdDMKNFgpR40083161;     ZdDMKNFgpR40083161 = ZdDMKNFgpR82374009;     ZdDMKNFgpR82374009 = ZdDMKNFgpR23126315;     ZdDMKNFgpR23126315 = ZdDMKNFgpR72185823;     ZdDMKNFgpR72185823 = ZdDMKNFgpR80378100;     ZdDMKNFgpR80378100 = ZdDMKNFgpR51087541;     ZdDMKNFgpR51087541 = ZdDMKNFgpR8227429;     ZdDMKNFgpR8227429 = ZdDMKNFgpR96180618;     ZdDMKNFgpR96180618 = ZdDMKNFgpR70208431;     ZdDMKNFgpR70208431 = ZdDMKNFgpR82711162;     ZdDMKNFgpR82711162 = ZdDMKNFgpR64798785;     ZdDMKNFgpR64798785 = ZdDMKNFgpR55907724;     ZdDMKNFgpR55907724 = ZdDMKNFgpR48850794;     ZdDMKNFgpR48850794 = ZdDMKNFgpR48567168;     ZdDMKNFgpR48567168 = ZdDMKNFgpR58251252;     ZdDMKNFgpR58251252 = ZdDMKNFgpR70346480;     ZdDMKNFgpR70346480 = ZdDMKNFgpR70688077;     ZdDMKNFgpR70688077 = ZdDMKNFgpR75495264;     ZdDMKNFgpR75495264 = ZdDMKNFgpR79649079;     ZdDMKNFgpR79649079 = ZdDMKNFgpR35703133;     ZdDMKNFgpR35703133 = ZdDMKNFgpR13310142;     ZdDMKNFgpR13310142 = ZdDMKNFgpR86492087;     ZdDMKNFgpR86492087 = ZdDMKNFgpR46274190;     ZdDMKNFgpR46274190 = ZdDMKNFgpR21486217;     ZdDMKNFgpR21486217 = ZdDMKNFgpR65337855;     ZdDMKNFgpR65337855 = ZdDMKNFgpR52682415;     ZdDMKNFgpR52682415 = ZdDMKNFgpR22774267;     ZdDMKNFgpR22774267 = ZdDMKNFgpR48185494;     ZdDMKNFgpR48185494 = ZdDMKNFgpR41331480;     ZdDMKNFgpR41331480 = ZdDMKNFgpR39940041;     ZdDMKNFgpR39940041 = ZdDMKNFgpR86127258;     ZdDMKNFgpR86127258 = ZdDMKNFgpR98120480;     ZdDMKNFgpR98120480 = ZdDMKNFgpR72289542;     ZdDMKNFgpR72289542 = ZdDMKNFgpR37518184;     ZdDMKNFgpR37518184 = ZdDMKNFgpR8208216;     ZdDMKNFgpR8208216 = ZdDMKNFgpR30924627;     ZdDMKNFgpR30924627 = ZdDMKNFgpR5741363;     ZdDMKNFgpR5741363 = ZdDMKNFgpR40117709;     ZdDMKNFgpR40117709 = ZdDMKNFgpR42449284;     ZdDMKNFgpR42449284 = ZdDMKNFgpR36831978;     ZdDMKNFgpR36831978 = ZdDMKNFgpR70778269;     ZdDMKNFgpR70778269 = ZdDMKNFgpR3513578;     ZdDMKNFgpR3513578 = ZdDMKNFgpR82275706;     ZdDMKNFgpR82275706 = ZdDMKNFgpR58726239;     ZdDMKNFgpR58726239 = ZdDMKNFgpR51638569;     ZdDMKNFgpR51638569 = ZdDMKNFgpR12619770;     ZdDMKNFgpR12619770 = ZdDMKNFgpR90977796;     ZdDMKNFgpR90977796 = ZdDMKNFgpR48779036;     ZdDMKNFgpR48779036 = ZdDMKNFgpR70435811;     ZdDMKNFgpR70435811 = ZdDMKNFgpR59347791;     ZdDMKNFgpR59347791 = ZdDMKNFgpR87396229;     ZdDMKNFgpR87396229 = ZdDMKNFgpR20427973;     ZdDMKNFgpR20427973 = ZdDMKNFgpR67206369;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void oCynLsmtxh21744961() {     int fUsDIyTHbq89593486 = -554219617;    int fUsDIyTHbq52580280 = -680629655;    int fUsDIyTHbq93530936 = -121813511;    int fUsDIyTHbq64864519 = -416905722;    int fUsDIyTHbq86430417 = -93958988;    int fUsDIyTHbq16184086 = -842567297;    int fUsDIyTHbq89014283 = -746659659;    int fUsDIyTHbq47183746 = -65944110;    int fUsDIyTHbq96377288 = -572750020;    int fUsDIyTHbq74755194 = -82280051;    int fUsDIyTHbq35864508 = -393283882;    int fUsDIyTHbq23471548 = -917792998;    int fUsDIyTHbq81717011 = -556294379;    int fUsDIyTHbq55619090 = -794247404;    int fUsDIyTHbq75711502 = -807677709;    int fUsDIyTHbq31967504 = -157008825;    int fUsDIyTHbq87746922 = -833331346;    int fUsDIyTHbq39841766 = -51149814;    int fUsDIyTHbq33792636 = -424445837;    int fUsDIyTHbq98636753 = -916360409;    int fUsDIyTHbq77755664 = -627981529;    int fUsDIyTHbq34840969 = 66221804;    int fUsDIyTHbq1317671 = -461907088;    int fUsDIyTHbq8550283 = -356230648;    int fUsDIyTHbq20599293 = -575178931;    int fUsDIyTHbq20651433 = -480866764;    int fUsDIyTHbq99539594 = 10737627;    int fUsDIyTHbq54202827 = -208555515;    int fUsDIyTHbq20191197 = -665506312;    int fUsDIyTHbq38024857 = -106348325;    int fUsDIyTHbq56594338 = -226069637;    int fUsDIyTHbq80368575 = -750250772;    int fUsDIyTHbq68958189 = -951361572;    int fUsDIyTHbq38394385 = -601434710;    int fUsDIyTHbq75923560 = -64615487;    int fUsDIyTHbq94177006 = -788123816;    int fUsDIyTHbq34877113 = -338088265;    int fUsDIyTHbq70063455 = -266836911;    int fUsDIyTHbq16835626 = -357973154;    int fUsDIyTHbq60014305 = 8889877;    int fUsDIyTHbq26103575 = -37210512;    int fUsDIyTHbq17195454 = -595619099;    int fUsDIyTHbq35244463 = -793824849;    int fUsDIyTHbq57528426 = -879263974;    int fUsDIyTHbq63906504 = -273834645;    int fUsDIyTHbq95038847 = -610148623;    int fUsDIyTHbq72356294 = -894844566;    int fUsDIyTHbq59530665 = -921972081;    int fUsDIyTHbq34543066 = -630699232;    int fUsDIyTHbq79457755 = -265923222;    int fUsDIyTHbq31995532 = -162455359;    int fUsDIyTHbq46037081 = -956618238;    int fUsDIyTHbq58692503 = -729741822;    int fUsDIyTHbq11374087 = -650596327;    int fUsDIyTHbq43989297 = -179502193;    int fUsDIyTHbq54752517 = -520441422;    int fUsDIyTHbq51262609 = -118722567;    int fUsDIyTHbq84980653 = -765582863;    int fUsDIyTHbq44265227 = -841726791;    int fUsDIyTHbq65778984 = -613092225;    int fUsDIyTHbq16644491 = -753304924;    int fUsDIyTHbq34811457 = -438104144;    int fUsDIyTHbq26992550 = -400437798;    int fUsDIyTHbq58352431 = -366401695;    int fUsDIyTHbq18160857 = -856210414;    int fUsDIyTHbq55495933 = -643033110;    int fUsDIyTHbq54513358 = -966431427;    int fUsDIyTHbq43322626 = -954859670;    int fUsDIyTHbq79695529 = -629631918;    int fUsDIyTHbq81534495 = 80446106;    int fUsDIyTHbq97090390 = -818920560;    int fUsDIyTHbq17683468 = -466494435;    int fUsDIyTHbq23006141 = -693176660;    int fUsDIyTHbq73778331 = -333335714;    int fUsDIyTHbq72533179 = -779149898;    int fUsDIyTHbq60560210 = 67637570;    int fUsDIyTHbq99596506 = -139953348;    int fUsDIyTHbq43789245 = -582643114;    int fUsDIyTHbq44643779 = 17603996;    int fUsDIyTHbq25560445 = -965030309;    int fUsDIyTHbq48295139 = -586022199;    int fUsDIyTHbq40008930 = -67290293;    int fUsDIyTHbq19659761 = -577856283;    int fUsDIyTHbq40733442 = -299583091;    int fUsDIyTHbq6029326 = -943892967;    int fUsDIyTHbq10557258 = -269451400;    int fUsDIyTHbq21676072 = 79491049;    int fUsDIyTHbq57584103 = -200765245;    int fUsDIyTHbq94405088 = -321932517;    int fUsDIyTHbq21171043 = -544174066;    int fUsDIyTHbq42914398 = -569401249;    int fUsDIyTHbq49896460 = -572505403;    int fUsDIyTHbq25798229 = -425110121;    int fUsDIyTHbq51056642 = -744880930;    int fUsDIyTHbq43369814 = -237805199;    int fUsDIyTHbq91292117 = -599106368;    int fUsDIyTHbq90202904 = -95181301;    int fUsDIyTHbq76892031 = -327423154;    int fUsDIyTHbq39367570 = 76946439;    int fUsDIyTHbq8410571 = -554219617;     fUsDIyTHbq89593486 = fUsDIyTHbq52580280;     fUsDIyTHbq52580280 = fUsDIyTHbq93530936;     fUsDIyTHbq93530936 = fUsDIyTHbq64864519;     fUsDIyTHbq64864519 = fUsDIyTHbq86430417;     fUsDIyTHbq86430417 = fUsDIyTHbq16184086;     fUsDIyTHbq16184086 = fUsDIyTHbq89014283;     fUsDIyTHbq89014283 = fUsDIyTHbq47183746;     fUsDIyTHbq47183746 = fUsDIyTHbq96377288;     fUsDIyTHbq96377288 = fUsDIyTHbq74755194;     fUsDIyTHbq74755194 = fUsDIyTHbq35864508;     fUsDIyTHbq35864508 = fUsDIyTHbq23471548;     fUsDIyTHbq23471548 = fUsDIyTHbq81717011;     fUsDIyTHbq81717011 = fUsDIyTHbq55619090;     fUsDIyTHbq55619090 = fUsDIyTHbq75711502;     fUsDIyTHbq75711502 = fUsDIyTHbq31967504;     fUsDIyTHbq31967504 = fUsDIyTHbq87746922;     fUsDIyTHbq87746922 = fUsDIyTHbq39841766;     fUsDIyTHbq39841766 = fUsDIyTHbq33792636;     fUsDIyTHbq33792636 = fUsDIyTHbq98636753;     fUsDIyTHbq98636753 = fUsDIyTHbq77755664;     fUsDIyTHbq77755664 = fUsDIyTHbq34840969;     fUsDIyTHbq34840969 = fUsDIyTHbq1317671;     fUsDIyTHbq1317671 = fUsDIyTHbq8550283;     fUsDIyTHbq8550283 = fUsDIyTHbq20599293;     fUsDIyTHbq20599293 = fUsDIyTHbq20651433;     fUsDIyTHbq20651433 = fUsDIyTHbq99539594;     fUsDIyTHbq99539594 = fUsDIyTHbq54202827;     fUsDIyTHbq54202827 = fUsDIyTHbq20191197;     fUsDIyTHbq20191197 = fUsDIyTHbq38024857;     fUsDIyTHbq38024857 = fUsDIyTHbq56594338;     fUsDIyTHbq56594338 = fUsDIyTHbq80368575;     fUsDIyTHbq80368575 = fUsDIyTHbq68958189;     fUsDIyTHbq68958189 = fUsDIyTHbq38394385;     fUsDIyTHbq38394385 = fUsDIyTHbq75923560;     fUsDIyTHbq75923560 = fUsDIyTHbq94177006;     fUsDIyTHbq94177006 = fUsDIyTHbq34877113;     fUsDIyTHbq34877113 = fUsDIyTHbq70063455;     fUsDIyTHbq70063455 = fUsDIyTHbq16835626;     fUsDIyTHbq16835626 = fUsDIyTHbq60014305;     fUsDIyTHbq60014305 = fUsDIyTHbq26103575;     fUsDIyTHbq26103575 = fUsDIyTHbq17195454;     fUsDIyTHbq17195454 = fUsDIyTHbq35244463;     fUsDIyTHbq35244463 = fUsDIyTHbq57528426;     fUsDIyTHbq57528426 = fUsDIyTHbq63906504;     fUsDIyTHbq63906504 = fUsDIyTHbq95038847;     fUsDIyTHbq95038847 = fUsDIyTHbq72356294;     fUsDIyTHbq72356294 = fUsDIyTHbq59530665;     fUsDIyTHbq59530665 = fUsDIyTHbq34543066;     fUsDIyTHbq34543066 = fUsDIyTHbq79457755;     fUsDIyTHbq79457755 = fUsDIyTHbq31995532;     fUsDIyTHbq31995532 = fUsDIyTHbq46037081;     fUsDIyTHbq46037081 = fUsDIyTHbq58692503;     fUsDIyTHbq58692503 = fUsDIyTHbq11374087;     fUsDIyTHbq11374087 = fUsDIyTHbq43989297;     fUsDIyTHbq43989297 = fUsDIyTHbq54752517;     fUsDIyTHbq54752517 = fUsDIyTHbq51262609;     fUsDIyTHbq51262609 = fUsDIyTHbq84980653;     fUsDIyTHbq84980653 = fUsDIyTHbq44265227;     fUsDIyTHbq44265227 = fUsDIyTHbq65778984;     fUsDIyTHbq65778984 = fUsDIyTHbq16644491;     fUsDIyTHbq16644491 = fUsDIyTHbq34811457;     fUsDIyTHbq34811457 = fUsDIyTHbq26992550;     fUsDIyTHbq26992550 = fUsDIyTHbq58352431;     fUsDIyTHbq58352431 = fUsDIyTHbq18160857;     fUsDIyTHbq18160857 = fUsDIyTHbq55495933;     fUsDIyTHbq55495933 = fUsDIyTHbq54513358;     fUsDIyTHbq54513358 = fUsDIyTHbq43322626;     fUsDIyTHbq43322626 = fUsDIyTHbq79695529;     fUsDIyTHbq79695529 = fUsDIyTHbq81534495;     fUsDIyTHbq81534495 = fUsDIyTHbq97090390;     fUsDIyTHbq97090390 = fUsDIyTHbq17683468;     fUsDIyTHbq17683468 = fUsDIyTHbq23006141;     fUsDIyTHbq23006141 = fUsDIyTHbq73778331;     fUsDIyTHbq73778331 = fUsDIyTHbq72533179;     fUsDIyTHbq72533179 = fUsDIyTHbq60560210;     fUsDIyTHbq60560210 = fUsDIyTHbq99596506;     fUsDIyTHbq99596506 = fUsDIyTHbq43789245;     fUsDIyTHbq43789245 = fUsDIyTHbq44643779;     fUsDIyTHbq44643779 = fUsDIyTHbq25560445;     fUsDIyTHbq25560445 = fUsDIyTHbq48295139;     fUsDIyTHbq48295139 = fUsDIyTHbq40008930;     fUsDIyTHbq40008930 = fUsDIyTHbq19659761;     fUsDIyTHbq19659761 = fUsDIyTHbq40733442;     fUsDIyTHbq40733442 = fUsDIyTHbq6029326;     fUsDIyTHbq6029326 = fUsDIyTHbq10557258;     fUsDIyTHbq10557258 = fUsDIyTHbq21676072;     fUsDIyTHbq21676072 = fUsDIyTHbq57584103;     fUsDIyTHbq57584103 = fUsDIyTHbq94405088;     fUsDIyTHbq94405088 = fUsDIyTHbq21171043;     fUsDIyTHbq21171043 = fUsDIyTHbq42914398;     fUsDIyTHbq42914398 = fUsDIyTHbq49896460;     fUsDIyTHbq49896460 = fUsDIyTHbq25798229;     fUsDIyTHbq25798229 = fUsDIyTHbq51056642;     fUsDIyTHbq51056642 = fUsDIyTHbq43369814;     fUsDIyTHbq43369814 = fUsDIyTHbq91292117;     fUsDIyTHbq91292117 = fUsDIyTHbq90202904;     fUsDIyTHbq90202904 = fUsDIyTHbq76892031;     fUsDIyTHbq76892031 = fUsDIyTHbq39367570;     fUsDIyTHbq39367570 = fUsDIyTHbq8410571;     fUsDIyTHbq8410571 = fUsDIyTHbq89593486;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void yzxcbmxRWw58389935() {     int woceKdKfqF11980604 = -188623580;    int woceKdKfqF36927780 = -629172709;    int woceKdKfqF22300216 = -361467355;    int woceKdKfqF79871207 = -423313254;    int woceKdKfqF84604671 = -685015989;    int woceKdKfqF31067203 = -703175675;    int woceKdKfqF81170022 = -568646965;    int woceKdKfqF22817605 = -536540754;    int woceKdKfqF93540419 = -456946059;    int woceKdKfqF66518779 = -925021384;    int woceKdKfqF73406368 = -389908283;    int woceKdKfqF79374496 = -68538292;    int woceKdKfqF48725192 = -75011699;    int woceKdKfqF98233842 = -221956161;    int woceKdKfqF12446780 = -435155422;    int woceKdKfqF76223416 = -538676367;    int woceKdKfqF37570275 = -10611166;    int woceKdKfqF45818182 = -502892504;    int woceKdKfqF75074267 = -559547962;    int woceKdKfqF77153037 = -68994863;    int woceKdKfqF78638771 = -619468837;    int woceKdKfqF98656187 = -438544509;    int woceKdKfqF4610995 = -637987680;    int woceKdKfqF35050070 = -920223505;    int woceKdKfqF56139539 = -196738056;    int woceKdKfqF8954428 = -924760988;    int woceKdKfqF46629015 = -846848866;    int woceKdKfqF60114277 = 2990031;    int woceKdKfqF27083758 = 90236569;    int woceKdKfqF47182037 = -26644192;    int woceKdKfqF885144 = -307385790;    int woceKdKfqF37909766 = -302322127;    int woceKdKfqF49996860 = -380705067;    int woceKdKfqF97783072 = -925561560;    int woceKdKfqF52152925 = -128371910;    int woceKdKfqF35869876 = -3519192;    int woceKdKfqF28316826 = -71036217;    int woceKdKfqF23689556 = -134462383;    int woceKdKfqF65143757 = -759019186;    int woceKdKfqF80200019 = -127334152;    int woceKdKfqF54860945 = -540388178;    int woceKdKfqF5703846 = -353527790;    int woceKdKfqF40794655 = -681355432;    int woceKdKfqF56972545 = -876627623;    int woceKdKfqF31889771 = -82978122;    int woceKdKfqF3139130 = -792377718;    int woceKdKfqF84653691 = -373254819;    int woceKdKfqF4129340 = -678507072;    int woceKdKfqF29002971 = -580415930;    int woceKdKfqF76541500 = -407943481;    int woceKdKfqF40864748 = -517531636;    int woceKdKfqF19888338 = -287437520;    int woceKdKfqF37006906 = -401653052;    int woceKdKfqF71660632 = -926277136;    int woceKdKfqF79751164 = -320206309;    int woceKdKfqF13324417 = -750079072;    int woceKdKfqF32316786 = -991185030;    int woceKdKfqF87250145 = -441243850;    int woceKdKfqF23731668 = -126575198;    int woceKdKfqF75650243 = -760255002;    int woceKdKfqF84438187 = -856326809;    int woceKdKfqF21055746 = -471636997;    int woceKdKfqF95733847 = -526777324;    int woceKdKfqF46358383 = -330301868;    int woceKdKfqF65633636 = -517635595;    int woceKdKfqF35496602 = 12413844;    int woceKdKfqF29377637 = -687833226;    int woceKdKfqF50942119 = -149450139;    int woceKdKfqF46080917 = 6415748;    int woceKdKfqF76576903 = -331636230;    int woceKdKfqF47906591 = -367640151;    int woceKdKfqF13880719 = -876148783;    int woceKdKfqF80674425 = -743873319;    int woceKdKfqF94874247 = -332213810;    int woceKdKfqF22292092 = -528606685;    int woceKdKfqF72934926 = -165941047;    int woceKdKfqF57861533 = -757189077;    int woceKdKfqF47638449 = -761360057;    int woceKdKfqF3160300 = -737245383;    int woceKdKfqF53000410 = -404360338;    int woceKdKfqF24300737 = -451506170;    int woceKdKfqF42499675 = -68341795;    int woceKdKfqF31111306 = -416594039;    int woceKdKfqF50542257 = -501819951;    int woceKdKfqF6317289 = -509112557;    int woceKdKfqF80996805 = 80051730;    int woceKdKfqF902860 = -900669075;    int woceKdKfqF78336228 = -454427931;    int woceKdKfqF18031909 = -505355252;    int woceKdKfqF38828509 = -378292839;    int woceKdKfqF3553091 = -12334163;    int woceKdKfqF41066680 = -629792367;    int woceKdKfqF99957887 = 92112814;    int woceKdKfqF89493513 = -998764184;    int woceKdKfqF95761832 = -271007344;    int woceKdKfqF33805200 = 31248819;    int woceKdKfqF9969998 = -826750467;    int woceKdKfqF94436271 = -251053565;    int woceKdKfqF91338909 = -258992029;    int woceKdKfqF96393168 = -188623580;     woceKdKfqF11980604 = woceKdKfqF36927780;     woceKdKfqF36927780 = woceKdKfqF22300216;     woceKdKfqF22300216 = woceKdKfqF79871207;     woceKdKfqF79871207 = woceKdKfqF84604671;     woceKdKfqF84604671 = woceKdKfqF31067203;     woceKdKfqF31067203 = woceKdKfqF81170022;     woceKdKfqF81170022 = woceKdKfqF22817605;     woceKdKfqF22817605 = woceKdKfqF93540419;     woceKdKfqF93540419 = woceKdKfqF66518779;     woceKdKfqF66518779 = woceKdKfqF73406368;     woceKdKfqF73406368 = woceKdKfqF79374496;     woceKdKfqF79374496 = woceKdKfqF48725192;     woceKdKfqF48725192 = woceKdKfqF98233842;     woceKdKfqF98233842 = woceKdKfqF12446780;     woceKdKfqF12446780 = woceKdKfqF76223416;     woceKdKfqF76223416 = woceKdKfqF37570275;     woceKdKfqF37570275 = woceKdKfqF45818182;     woceKdKfqF45818182 = woceKdKfqF75074267;     woceKdKfqF75074267 = woceKdKfqF77153037;     woceKdKfqF77153037 = woceKdKfqF78638771;     woceKdKfqF78638771 = woceKdKfqF98656187;     woceKdKfqF98656187 = woceKdKfqF4610995;     woceKdKfqF4610995 = woceKdKfqF35050070;     woceKdKfqF35050070 = woceKdKfqF56139539;     woceKdKfqF56139539 = woceKdKfqF8954428;     woceKdKfqF8954428 = woceKdKfqF46629015;     woceKdKfqF46629015 = woceKdKfqF60114277;     woceKdKfqF60114277 = woceKdKfqF27083758;     woceKdKfqF27083758 = woceKdKfqF47182037;     woceKdKfqF47182037 = woceKdKfqF885144;     woceKdKfqF885144 = woceKdKfqF37909766;     woceKdKfqF37909766 = woceKdKfqF49996860;     woceKdKfqF49996860 = woceKdKfqF97783072;     woceKdKfqF97783072 = woceKdKfqF52152925;     woceKdKfqF52152925 = woceKdKfqF35869876;     woceKdKfqF35869876 = woceKdKfqF28316826;     woceKdKfqF28316826 = woceKdKfqF23689556;     woceKdKfqF23689556 = woceKdKfqF65143757;     woceKdKfqF65143757 = woceKdKfqF80200019;     woceKdKfqF80200019 = woceKdKfqF54860945;     woceKdKfqF54860945 = woceKdKfqF5703846;     woceKdKfqF5703846 = woceKdKfqF40794655;     woceKdKfqF40794655 = woceKdKfqF56972545;     woceKdKfqF56972545 = woceKdKfqF31889771;     woceKdKfqF31889771 = woceKdKfqF3139130;     woceKdKfqF3139130 = woceKdKfqF84653691;     woceKdKfqF84653691 = woceKdKfqF4129340;     woceKdKfqF4129340 = woceKdKfqF29002971;     woceKdKfqF29002971 = woceKdKfqF76541500;     woceKdKfqF76541500 = woceKdKfqF40864748;     woceKdKfqF40864748 = woceKdKfqF19888338;     woceKdKfqF19888338 = woceKdKfqF37006906;     woceKdKfqF37006906 = woceKdKfqF71660632;     woceKdKfqF71660632 = woceKdKfqF79751164;     woceKdKfqF79751164 = woceKdKfqF13324417;     woceKdKfqF13324417 = woceKdKfqF32316786;     woceKdKfqF32316786 = woceKdKfqF87250145;     woceKdKfqF87250145 = woceKdKfqF23731668;     woceKdKfqF23731668 = woceKdKfqF75650243;     woceKdKfqF75650243 = woceKdKfqF84438187;     woceKdKfqF84438187 = woceKdKfqF21055746;     woceKdKfqF21055746 = woceKdKfqF95733847;     woceKdKfqF95733847 = woceKdKfqF46358383;     woceKdKfqF46358383 = woceKdKfqF65633636;     woceKdKfqF65633636 = woceKdKfqF35496602;     woceKdKfqF35496602 = woceKdKfqF29377637;     woceKdKfqF29377637 = woceKdKfqF50942119;     woceKdKfqF50942119 = woceKdKfqF46080917;     woceKdKfqF46080917 = woceKdKfqF76576903;     woceKdKfqF76576903 = woceKdKfqF47906591;     woceKdKfqF47906591 = woceKdKfqF13880719;     woceKdKfqF13880719 = woceKdKfqF80674425;     woceKdKfqF80674425 = woceKdKfqF94874247;     woceKdKfqF94874247 = woceKdKfqF22292092;     woceKdKfqF22292092 = woceKdKfqF72934926;     woceKdKfqF72934926 = woceKdKfqF57861533;     woceKdKfqF57861533 = woceKdKfqF47638449;     woceKdKfqF47638449 = woceKdKfqF3160300;     woceKdKfqF3160300 = woceKdKfqF53000410;     woceKdKfqF53000410 = woceKdKfqF24300737;     woceKdKfqF24300737 = woceKdKfqF42499675;     woceKdKfqF42499675 = woceKdKfqF31111306;     woceKdKfqF31111306 = woceKdKfqF50542257;     woceKdKfqF50542257 = woceKdKfqF6317289;     woceKdKfqF6317289 = woceKdKfqF80996805;     woceKdKfqF80996805 = woceKdKfqF902860;     woceKdKfqF902860 = woceKdKfqF78336228;     woceKdKfqF78336228 = woceKdKfqF18031909;     woceKdKfqF18031909 = woceKdKfqF38828509;     woceKdKfqF38828509 = woceKdKfqF3553091;     woceKdKfqF3553091 = woceKdKfqF41066680;     woceKdKfqF41066680 = woceKdKfqF99957887;     woceKdKfqF99957887 = woceKdKfqF89493513;     woceKdKfqF89493513 = woceKdKfqF95761832;     woceKdKfqF95761832 = woceKdKfqF33805200;     woceKdKfqF33805200 = woceKdKfqF9969998;     woceKdKfqF9969998 = woceKdKfqF94436271;     woceKdKfqF94436271 = woceKdKfqF91338909;     woceKdKfqF91338909 = woceKdKfqF96393168;     woceKdKfqF96393168 = woceKdKfqF11980604;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void PLJgsXRWDb95034910() {     int zzSEGDSfDB34367721 = -923027541;    int zzSEGDSfDB21275281 = -577715758;    int zzSEGDSfDB51069494 = -601121199;    int zzSEGDSfDB94877896 = -429720788;    int zzSEGDSfDB82778925 = -176072982;    int zzSEGDSfDB45950320 = -563784053;    int zzSEGDSfDB73325761 = -390634271;    int zzSEGDSfDB98451463 = 92862617;    int zzSEGDSfDB90703550 = -341142097;    int zzSEGDSfDB58282364 = -667762717;    int zzSEGDSfDB10948228 = -386532682;    int zzSEGDSfDB35277445 = -319283576;    int zzSEGDSfDB15733373 = -693729019;    int zzSEGDSfDB40848595 = -749664917;    int zzSEGDSfDB49182056 = -62633117;    int zzSEGDSfDB20479328 = -920343909;    int zzSEGDSfDB87393626 = -287890986;    int zzSEGDSfDB51794599 = -954635191;    int zzSEGDSfDB16355898 = -694650088;    int zzSEGDSfDB55669320 = -321629316;    int zzSEGDSfDB79521878 = -610956140;    int zzSEGDSfDB62471406 = -943310822;    int zzSEGDSfDB7904317 = -814068272;    int zzSEGDSfDB61549857 = -384216362;    int zzSEGDSfDB91679786 = -918297168;    int zzSEGDSfDB97257421 = -268655216;    int zzSEGDSfDB93718435 = -604435359;    int zzSEGDSfDB66025727 = -885464416;    int zzSEGDSfDB33976319 = -254020566;    int zzSEGDSfDB56339216 = 53059942;    int zzSEGDSfDB45175948 = -388701942;    int zzSEGDSfDB95450956 = -954393485;    int zzSEGDSfDB31035531 = -910048565;    int zzSEGDSfDB57171760 = -149688410;    int zzSEGDSfDB28382290 = -192128335;    int zzSEGDSfDB77562745 = -318914587;    int zzSEGDSfDB21756538 = -903984168;    int zzSEGDSfDB77315657 = -2087852;    int zzSEGDSfDB13451889 = -60065230;    int zzSEGDSfDB385735 = -263558182;    int zzSEGDSfDB83618316 = 56434156;    int zzSEGDSfDB94212236 = -111436502;    int zzSEGDSfDB46344847 = -568886018;    int zzSEGDSfDB56416665 = -873991272;    int zzSEGDSfDB99873037 = -992121601;    int zzSEGDSfDB11239412 = -974606832;    int zzSEGDSfDB96951089 = -951665072;    int zzSEGDSfDB48728014 = -435042064;    int zzSEGDSfDB23462876 = -530132653;    int zzSEGDSfDB73625246 = -549963734;    int zzSEGDSfDB49733964 = -872607913;    int zzSEGDSfDB93739594 = -718256807;    int zzSEGDSfDB15321310 = -73564279;    int zzSEGDSfDB31947177 = -101957945;    int zzSEGDSfDB15513032 = -460910429;    int zzSEGDSfDB71896315 = -979716719;    int zzSEGDSfDB13370964 = -763647486;    int zzSEGDSfDB89519636 = -116904837;    int zzSEGDSfDB3198110 = -511423621;    int zzSEGDSfDB85521503 = -907417767;    int zzSEGDSfDB52231884 = -959348694;    int zzSEGDSfDB7300035 = -505169856;    int zzSEGDSfDB64475145 = -653116818;    int zzSEGDSfDB34364335 = -294202039;    int zzSEGDSfDB13106416 = -179060776;    int zzSEGDSfDB15497271 = -432139198;    int zzSEGDSfDB4241915 = -409235012;    int zzSEGDSfDB58561613 = -444040609;    int zzSEGDSfDB12466305 = -457536583;    int zzSEGDSfDB71619311 = -743718530;    int zzSEGDSfDB98722790 = 83640258;    int zzSEGDSfDB10077970 = -185803134;    int zzSEGDSfDB38342711 = -794569962;    int zzSEGDSfDB15970163 = -331091907;    int zzSEGDSfDB72051004 = -278063473;    int zzSEGDSfDB85309642 = -399519639;    int zzSEGDSfDB16126559 = -274424805;    int zzSEGDSfDB51487652 = -940077000;    int zzSEGDSfDB61676820 = -392094762;    int zzSEGDSfDB80440375 = -943690337;    int zzSEGDSfDB306333 = -316990145;    int zzSEGDSfDB44990421 = -69393296;    int zzSEGDSfDB42562851 = -255331763;    int zzSEGDSfDB60351072 = -704056832;    int zzSEGDSfDB6605252 = -74332146;    int zzSEGDSfDB51436353 = -670445135;    int zzSEGDSfDB80129647 = -780829206;    int zzSEGDSfDB99088353 = -708090620;    int zzSEGDSfDB41658728 = -688777982;    int zzSEGDSfDB56485974 = -212411616;    int zzSEGDSfDB64191781 = -555267102;    int zzSEGDSfDB32236901 = -687079332;    int zzSEGDSfDB74117547 = -490664232;    int zzSEGDSfDB27930385 = -152647463;    int zzSEGDSfDB48153851 = -304209488;    int zzSEGDSfDB76318282 = -438395989;    int zzSEGDSfDB29737091 = -458319685;    int zzSEGDSfDB11980513 = -174683979;    int zzSEGDSfDB43310249 = -594930497;    int zzSEGDSfDB84375766 = -923027541;     zzSEGDSfDB34367721 = zzSEGDSfDB21275281;     zzSEGDSfDB21275281 = zzSEGDSfDB51069494;     zzSEGDSfDB51069494 = zzSEGDSfDB94877896;     zzSEGDSfDB94877896 = zzSEGDSfDB82778925;     zzSEGDSfDB82778925 = zzSEGDSfDB45950320;     zzSEGDSfDB45950320 = zzSEGDSfDB73325761;     zzSEGDSfDB73325761 = zzSEGDSfDB98451463;     zzSEGDSfDB98451463 = zzSEGDSfDB90703550;     zzSEGDSfDB90703550 = zzSEGDSfDB58282364;     zzSEGDSfDB58282364 = zzSEGDSfDB10948228;     zzSEGDSfDB10948228 = zzSEGDSfDB35277445;     zzSEGDSfDB35277445 = zzSEGDSfDB15733373;     zzSEGDSfDB15733373 = zzSEGDSfDB40848595;     zzSEGDSfDB40848595 = zzSEGDSfDB49182056;     zzSEGDSfDB49182056 = zzSEGDSfDB20479328;     zzSEGDSfDB20479328 = zzSEGDSfDB87393626;     zzSEGDSfDB87393626 = zzSEGDSfDB51794599;     zzSEGDSfDB51794599 = zzSEGDSfDB16355898;     zzSEGDSfDB16355898 = zzSEGDSfDB55669320;     zzSEGDSfDB55669320 = zzSEGDSfDB79521878;     zzSEGDSfDB79521878 = zzSEGDSfDB62471406;     zzSEGDSfDB62471406 = zzSEGDSfDB7904317;     zzSEGDSfDB7904317 = zzSEGDSfDB61549857;     zzSEGDSfDB61549857 = zzSEGDSfDB91679786;     zzSEGDSfDB91679786 = zzSEGDSfDB97257421;     zzSEGDSfDB97257421 = zzSEGDSfDB93718435;     zzSEGDSfDB93718435 = zzSEGDSfDB66025727;     zzSEGDSfDB66025727 = zzSEGDSfDB33976319;     zzSEGDSfDB33976319 = zzSEGDSfDB56339216;     zzSEGDSfDB56339216 = zzSEGDSfDB45175948;     zzSEGDSfDB45175948 = zzSEGDSfDB95450956;     zzSEGDSfDB95450956 = zzSEGDSfDB31035531;     zzSEGDSfDB31035531 = zzSEGDSfDB57171760;     zzSEGDSfDB57171760 = zzSEGDSfDB28382290;     zzSEGDSfDB28382290 = zzSEGDSfDB77562745;     zzSEGDSfDB77562745 = zzSEGDSfDB21756538;     zzSEGDSfDB21756538 = zzSEGDSfDB77315657;     zzSEGDSfDB77315657 = zzSEGDSfDB13451889;     zzSEGDSfDB13451889 = zzSEGDSfDB385735;     zzSEGDSfDB385735 = zzSEGDSfDB83618316;     zzSEGDSfDB83618316 = zzSEGDSfDB94212236;     zzSEGDSfDB94212236 = zzSEGDSfDB46344847;     zzSEGDSfDB46344847 = zzSEGDSfDB56416665;     zzSEGDSfDB56416665 = zzSEGDSfDB99873037;     zzSEGDSfDB99873037 = zzSEGDSfDB11239412;     zzSEGDSfDB11239412 = zzSEGDSfDB96951089;     zzSEGDSfDB96951089 = zzSEGDSfDB48728014;     zzSEGDSfDB48728014 = zzSEGDSfDB23462876;     zzSEGDSfDB23462876 = zzSEGDSfDB73625246;     zzSEGDSfDB73625246 = zzSEGDSfDB49733964;     zzSEGDSfDB49733964 = zzSEGDSfDB93739594;     zzSEGDSfDB93739594 = zzSEGDSfDB15321310;     zzSEGDSfDB15321310 = zzSEGDSfDB31947177;     zzSEGDSfDB31947177 = zzSEGDSfDB15513032;     zzSEGDSfDB15513032 = zzSEGDSfDB71896315;     zzSEGDSfDB71896315 = zzSEGDSfDB13370964;     zzSEGDSfDB13370964 = zzSEGDSfDB89519636;     zzSEGDSfDB89519636 = zzSEGDSfDB3198110;     zzSEGDSfDB3198110 = zzSEGDSfDB85521503;     zzSEGDSfDB85521503 = zzSEGDSfDB52231884;     zzSEGDSfDB52231884 = zzSEGDSfDB7300035;     zzSEGDSfDB7300035 = zzSEGDSfDB64475145;     zzSEGDSfDB64475145 = zzSEGDSfDB34364335;     zzSEGDSfDB34364335 = zzSEGDSfDB13106416;     zzSEGDSfDB13106416 = zzSEGDSfDB15497271;     zzSEGDSfDB15497271 = zzSEGDSfDB4241915;     zzSEGDSfDB4241915 = zzSEGDSfDB58561613;     zzSEGDSfDB58561613 = zzSEGDSfDB12466305;     zzSEGDSfDB12466305 = zzSEGDSfDB71619311;     zzSEGDSfDB71619311 = zzSEGDSfDB98722790;     zzSEGDSfDB98722790 = zzSEGDSfDB10077970;     zzSEGDSfDB10077970 = zzSEGDSfDB38342711;     zzSEGDSfDB38342711 = zzSEGDSfDB15970163;     zzSEGDSfDB15970163 = zzSEGDSfDB72051004;     zzSEGDSfDB72051004 = zzSEGDSfDB85309642;     zzSEGDSfDB85309642 = zzSEGDSfDB16126559;     zzSEGDSfDB16126559 = zzSEGDSfDB51487652;     zzSEGDSfDB51487652 = zzSEGDSfDB61676820;     zzSEGDSfDB61676820 = zzSEGDSfDB80440375;     zzSEGDSfDB80440375 = zzSEGDSfDB306333;     zzSEGDSfDB306333 = zzSEGDSfDB44990421;     zzSEGDSfDB44990421 = zzSEGDSfDB42562851;     zzSEGDSfDB42562851 = zzSEGDSfDB60351072;     zzSEGDSfDB60351072 = zzSEGDSfDB6605252;     zzSEGDSfDB6605252 = zzSEGDSfDB51436353;     zzSEGDSfDB51436353 = zzSEGDSfDB80129647;     zzSEGDSfDB80129647 = zzSEGDSfDB99088353;     zzSEGDSfDB99088353 = zzSEGDSfDB41658728;     zzSEGDSfDB41658728 = zzSEGDSfDB56485974;     zzSEGDSfDB56485974 = zzSEGDSfDB64191781;     zzSEGDSfDB64191781 = zzSEGDSfDB32236901;     zzSEGDSfDB32236901 = zzSEGDSfDB74117547;     zzSEGDSfDB74117547 = zzSEGDSfDB27930385;     zzSEGDSfDB27930385 = zzSEGDSfDB48153851;     zzSEGDSfDB48153851 = zzSEGDSfDB76318282;     zzSEGDSfDB76318282 = zzSEGDSfDB29737091;     zzSEGDSfDB29737091 = zzSEGDSfDB11980513;     zzSEGDSfDB11980513 = zzSEGDSfDB43310249;     zzSEGDSfDB43310249 = zzSEGDSfDB84375766;     zzSEGDSfDB84375766 = zzSEGDSfDB34367721;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void olXmiNEUCo31679885() {     int lgQRNzeuOJ56754838 = -557431506;    int lgQRNzeuOJ5622781 = -526258819;    int lgQRNzeuOJ79838772 = -840775043;    int lgQRNzeuOJ9884585 = -436128317;    int lgQRNzeuOJ80953179 = -767129992;    int lgQRNzeuOJ60833436 = -424392431;    int lgQRNzeuOJ65481500 = -212621577;    int lgQRNzeuOJ74085322 = -377734043;    int lgQRNzeuOJ87866681 = -225338137;    int lgQRNzeuOJ50045948 = -410504050;    int lgQRNzeuOJ48490087 = -383157084;    int lgQRNzeuOJ91180394 = -570028878;    int lgQRNzeuOJ82741553 = -212446338;    int lgQRNzeuOJ83463346 = -177373675;    int lgQRNzeuOJ85917333 = -790110848;    int lgQRNzeuOJ64735241 = -202011452;    int lgQRNzeuOJ37216978 = -565170806;    int lgQRNzeuOJ57771015 = -306377886;    int lgQRNzeuOJ57637529 = -829752212;    int lgQRNzeuOJ34185604 = -574263770;    int lgQRNzeuOJ80404985 = -602443454;    int lgQRNzeuOJ26286624 = -348077135;    int lgQRNzeuOJ11197641 = -990148864;    int lgQRNzeuOJ88049644 = -948209220;    int lgQRNzeuOJ27220033 = -539856305;    int lgQRNzeuOJ85560416 = -712549436;    int lgQRNzeuOJ40807856 = -362021852;    int lgQRNzeuOJ71937177 = -673918876;    int lgQRNzeuOJ40868879 = -598277669;    int lgQRNzeuOJ65496395 = -967235925;    int lgQRNzeuOJ89466753 = -470018094;    int lgQRNzeuOJ52992148 = -506464836;    int lgQRNzeuOJ12074201 = -339392056;    int lgQRNzeuOJ16560447 = -473815260;    int lgQRNzeuOJ4611655 = -255884755;    int lgQRNzeuOJ19255615 = -634309945;    int lgQRNzeuOJ15196250 = -636932120;    int lgQRNzeuOJ30941758 = -969713327;    int lgQRNzeuOJ61760019 = -461111249;    int lgQRNzeuOJ20571449 = -399782211;    int lgQRNzeuOJ12375687 = -446743510;    int lgQRNzeuOJ82720627 = -969345172;    int lgQRNzeuOJ51895039 = -456416600;    int lgQRNzeuOJ55860784 = -871354921;    int lgQRNzeuOJ67856304 = -801265078;    int lgQRNzeuOJ19339693 = -56835909;    int lgQRNzeuOJ9248487 = -430075324;    int lgQRNzeuOJ93326688 = -191577054;    int lgQRNzeuOJ17922782 = -479849327;    int lgQRNzeuOJ70708992 = -691983999;    int lgQRNzeuOJ58603181 = -127684190;    int lgQRNzeuOJ67590852 = -49076085;    int lgQRNzeuOJ93635712 = -845475513;    int lgQRNzeuOJ92233722 = -377638753;    int lgQRNzeuOJ51274899 = -601614539;    int lgQRNzeuOJ30468214 = -109354371;    int lgQRNzeuOJ94425140 = -536109955;    int lgQRNzeuOJ91789128 = -892565824;    int lgQRNzeuOJ82664551 = -896272012;    int lgQRNzeuOJ95392762 = 45419443;    int lgQRNzeuOJ20025581 = 37629421;    int lgQRNzeuOJ93544322 = -538702702;    int lgQRNzeuOJ33216443 = -779456374;    int lgQRNzeuOJ22370286 = -258102213;    int lgQRNzeuOJ60579195 = -940485957;    int lgQRNzeuOJ95497939 = -876692249;    int lgQRNzeuOJ79106193 = -130636823;    int lgQRNzeuOJ66181106 = -738631078;    int lgQRNzeuOJ78851692 = -921488921;    int lgQRNzeuOJ66661719 = -55800904;    int lgQRNzeuOJ49538991 = -565079332;    int lgQRNzeuOJ6275221 = -595457479;    int lgQRNzeuOJ96010995 = -845266637;    int lgQRNzeuOJ37066080 = -329970002;    int lgQRNzeuOJ21809917 = -27520260;    int lgQRNzeuOJ97684357 = -633098282;    int lgQRNzeuOJ74391585 = -891660536;    int lgQRNzeuOJ55336856 = -18793943;    int lgQRNzeuOJ20193341 = -46944142;    int lgQRNzeuOJ7880340 = -383020396;    int lgQRNzeuOJ76311930 = -182474112;    int lgQRNzeuOJ47481167 = -70444799;    int lgQRNzeuOJ54014396 = -94069549;    int lgQRNzeuOJ70159887 = -906293671;    int lgQRNzeuOJ6893215 = -739551736;    int lgQRNzeuOJ21875902 = -320942009;    int lgQRNzeuOJ59356435 = -660989324;    int lgQRNzeuOJ19840479 = -961753303;    int lgQRNzeuOJ65285548 = -872200722;    int lgQRNzeuOJ74143440 = -46530384;    int lgQRNzeuOJ24830474 = 1800009;    int lgQRNzeuOJ23407122 = -744366297;    int lgQRNzeuOJ48277207 = 26558685;    int lgQRNzeuOJ66367257 = -406530693;    int lgQRNzeuOJ545869 = -337411632;    int lgQRNzeuOJ18831364 = -908040808;    int lgQRNzeuOJ49504185 = -89888799;    int lgQRNzeuOJ29524753 = -98314388;    int lgQRNzeuOJ95281588 = -930868965;    int lgQRNzeuOJ72358364 = -557431506;     lgQRNzeuOJ56754838 = lgQRNzeuOJ5622781;     lgQRNzeuOJ5622781 = lgQRNzeuOJ79838772;     lgQRNzeuOJ79838772 = lgQRNzeuOJ9884585;     lgQRNzeuOJ9884585 = lgQRNzeuOJ80953179;     lgQRNzeuOJ80953179 = lgQRNzeuOJ60833436;     lgQRNzeuOJ60833436 = lgQRNzeuOJ65481500;     lgQRNzeuOJ65481500 = lgQRNzeuOJ74085322;     lgQRNzeuOJ74085322 = lgQRNzeuOJ87866681;     lgQRNzeuOJ87866681 = lgQRNzeuOJ50045948;     lgQRNzeuOJ50045948 = lgQRNzeuOJ48490087;     lgQRNzeuOJ48490087 = lgQRNzeuOJ91180394;     lgQRNzeuOJ91180394 = lgQRNzeuOJ82741553;     lgQRNzeuOJ82741553 = lgQRNzeuOJ83463346;     lgQRNzeuOJ83463346 = lgQRNzeuOJ85917333;     lgQRNzeuOJ85917333 = lgQRNzeuOJ64735241;     lgQRNzeuOJ64735241 = lgQRNzeuOJ37216978;     lgQRNzeuOJ37216978 = lgQRNzeuOJ57771015;     lgQRNzeuOJ57771015 = lgQRNzeuOJ57637529;     lgQRNzeuOJ57637529 = lgQRNzeuOJ34185604;     lgQRNzeuOJ34185604 = lgQRNzeuOJ80404985;     lgQRNzeuOJ80404985 = lgQRNzeuOJ26286624;     lgQRNzeuOJ26286624 = lgQRNzeuOJ11197641;     lgQRNzeuOJ11197641 = lgQRNzeuOJ88049644;     lgQRNzeuOJ88049644 = lgQRNzeuOJ27220033;     lgQRNzeuOJ27220033 = lgQRNzeuOJ85560416;     lgQRNzeuOJ85560416 = lgQRNzeuOJ40807856;     lgQRNzeuOJ40807856 = lgQRNzeuOJ71937177;     lgQRNzeuOJ71937177 = lgQRNzeuOJ40868879;     lgQRNzeuOJ40868879 = lgQRNzeuOJ65496395;     lgQRNzeuOJ65496395 = lgQRNzeuOJ89466753;     lgQRNzeuOJ89466753 = lgQRNzeuOJ52992148;     lgQRNzeuOJ52992148 = lgQRNzeuOJ12074201;     lgQRNzeuOJ12074201 = lgQRNzeuOJ16560447;     lgQRNzeuOJ16560447 = lgQRNzeuOJ4611655;     lgQRNzeuOJ4611655 = lgQRNzeuOJ19255615;     lgQRNzeuOJ19255615 = lgQRNzeuOJ15196250;     lgQRNzeuOJ15196250 = lgQRNzeuOJ30941758;     lgQRNzeuOJ30941758 = lgQRNzeuOJ61760019;     lgQRNzeuOJ61760019 = lgQRNzeuOJ20571449;     lgQRNzeuOJ20571449 = lgQRNzeuOJ12375687;     lgQRNzeuOJ12375687 = lgQRNzeuOJ82720627;     lgQRNzeuOJ82720627 = lgQRNzeuOJ51895039;     lgQRNzeuOJ51895039 = lgQRNzeuOJ55860784;     lgQRNzeuOJ55860784 = lgQRNzeuOJ67856304;     lgQRNzeuOJ67856304 = lgQRNzeuOJ19339693;     lgQRNzeuOJ19339693 = lgQRNzeuOJ9248487;     lgQRNzeuOJ9248487 = lgQRNzeuOJ93326688;     lgQRNzeuOJ93326688 = lgQRNzeuOJ17922782;     lgQRNzeuOJ17922782 = lgQRNzeuOJ70708992;     lgQRNzeuOJ70708992 = lgQRNzeuOJ58603181;     lgQRNzeuOJ58603181 = lgQRNzeuOJ67590852;     lgQRNzeuOJ67590852 = lgQRNzeuOJ93635712;     lgQRNzeuOJ93635712 = lgQRNzeuOJ92233722;     lgQRNzeuOJ92233722 = lgQRNzeuOJ51274899;     lgQRNzeuOJ51274899 = lgQRNzeuOJ30468214;     lgQRNzeuOJ30468214 = lgQRNzeuOJ94425140;     lgQRNzeuOJ94425140 = lgQRNzeuOJ91789128;     lgQRNzeuOJ91789128 = lgQRNzeuOJ82664551;     lgQRNzeuOJ82664551 = lgQRNzeuOJ95392762;     lgQRNzeuOJ95392762 = lgQRNzeuOJ20025581;     lgQRNzeuOJ20025581 = lgQRNzeuOJ93544322;     lgQRNzeuOJ93544322 = lgQRNzeuOJ33216443;     lgQRNzeuOJ33216443 = lgQRNzeuOJ22370286;     lgQRNzeuOJ22370286 = lgQRNzeuOJ60579195;     lgQRNzeuOJ60579195 = lgQRNzeuOJ95497939;     lgQRNzeuOJ95497939 = lgQRNzeuOJ79106193;     lgQRNzeuOJ79106193 = lgQRNzeuOJ66181106;     lgQRNzeuOJ66181106 = lgQRNzeuOJ78851692;     lgQRNzeuOJ78851692 = lgQRNzeuOJ66661719;     lgQRNzeuOJ66661719 = lgQRNzeuOJ49538991;     lgQRNzeuOJ49538991 = lgQRNzeuOJ6275221;     lgQRNzeuOJ6275221 = lgQRNzeuOJ96010995;     lgQRNzeuOJ96010995 = lgQRNzeuOJ37066080;     lgQRNzeuOJ37066080 = lgQRNzeuOJ21809917;     lgQRNzeuOJ21809917 = lgQRNzeuOJ97684357;     lgQRNzeuOJ97684357 = lgQRNzeuOJ74391585;     lgQRNzeuOJ74391585 = lgQRNzeuOJ55336856;     lgQRNzeuOJ55336856 = lgQRNzeuOJ20193341;     lgQRNzeuOJ20193341 = lgQRNzeuOJ7880340;     lgQRNzeuOJ7880340 = lgQRNzeuOJ76311930;     lgQRNzeuOJ76311930 = lgQRNzeuOJ47481167;     lgQRNzeuOJ47481167 = lgQRNzeuOJ54014396;     lgQRNzeuOJ54014396 = lgQRNzeuOJ70159887;     lgQRNzeuOJ70159887 = lgQRNzeuOJ6893215;     lgQRNzeuOJ6893215 = lgQRNzeuOJ21875902;     lgQRNzeuOJ21875902 = lgQRNzeuOJ59356435;     lgQRNzeuOJ59356435 = lgQRNzeuOJ19840479;     lgQRNzeuOJ19840479 = lgQRNzeuOJ65285548;     lgQRNzeuOJ65285548 = lgQRNzeuOJ74143440;     lgQRNzeuOJ74143440 = lgQRNzeuOJ24830474;     lgQRNzeuOJ24830474 = lgQRNzeuOJ23407122;     lgQRNzeuOJ23407122 = lgQRNzeuOJ48277207;     lgQRNzeuOJ48277207 = lgQRNzeuOJ66367257;     lgQRNzeuOJ66367257 = lgQRNzeuOJ545869;     lgQRNzeuOJ545869 = lgQRNzeuOJ18831364;     lgQRNzeuOJ18831364 = lgQRNzeuOJ49504185;     lgQRNzeuOJ49504185 = lgQRNzeuOJ29524753;     lgQRNzeuOJ29524753 = lgQRNzeuOJ95281588;     lgQRNzeuOJ95281588 = lgQRNzeuOJ72358364;     lgQRNzeuOJ72358364 = lgQRNzeuOJ56754838;}
// Junk Finished

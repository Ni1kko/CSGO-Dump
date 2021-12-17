#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "config.hpp"

#include "features/features.hpp"
#include "features/misc/SpecList.hpp"
/*#include "features/misc/misc.hpp"
#include "features/misc/misc.cpp"*/
//#include "lua_api/luaapi.hpp"

#include "render.hpp"
#include "globals.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "imgui\Stule.h"
/*#include "imgui/imgui.cpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"*/

void Menu::Initialize()
{
	CreateStyle();

    _visible = true;
}

void Menu::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}

static int page = 1;
void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = _visible;
	

	static bool once{ false };
	if (!once) {
		g_Notification.Push("NetSense","Welcome back");
		once = true;
	}
	g_SpecList->Draw();
	g_WaterMark->DrawMark();
	g_KeyBinds->Draw();

	CAnimations::Get().Begin(_visible);
	if (!_visible && CAnimations::Get().GetAlpha(true) <= 0.f) {
		CAnimations::Get().End();
		return;
	}

	ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);

	if (g_ChangeMenuSize) {
		ImGui::SetNextWindowSize(g_ChangeMenuSizeValue);
		g_ChangeMenuSize = false;
	} else {
		ImGui::SetNextWindowSizeConstraints(ImVec2(800, 450), ImVec2(4096, 4096));
	}

	Color line_color = g_Misc->colors.accent_color;
	if (CAnimations::Get().GetAlpha() < line_color.a())
		line_color.SetColor(line_color.r(), line_color.g(), line_color.b(), (int)CAnimations::Get().GetAlpha());
	ImVec4 accent = ImVec4(line_color.r() / 255.f, line_color.g() / 255.f, line_color.b() / 255.f, line_color.a() / 255.f);
	
	ImGui::PushStyleColor(ImGuiCol_CheckMark, accent);
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, accent);
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, accent);
	ImGui::PushStyleColor(ImGuiCol_DragDropTarget, accent);

	if (ImGui::Begin("##spectre_win", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar)) {
		//if (!menu_opened) return;
		/*ImVec2 pos;
		ImDrawList* draw;
		draw = ImGui::GetWindowDrawList();
		pos = ImGui::GetWindowPos();
		ImGui::SetWindowSize(ImVec2(644, 403));

		draw->AddImage(tImage, pos, ImVec2(pos.x + 644, pos.y + 603));*/
		

		if (g_ShowPrompt) {
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y / 2.f);
			auto tSize = ImGui::CalcTextSize(g_PromptTitle.c_str());
			auto cSize = ImGui::GetWindowSize().x / 2.f;
			ImGui::SetCursorPosX(cSize - (tSize.x / 2.f));
			ImGui::Text(g_PromptTitle.c_str());
			ImGui::SetCursorPosX(cSize - 102);
			if (ImGui::Button("Yes", ImVec2(100, 19)) || InputSys::Get().IsKeyDown(VK_RETURN)) {
				g_PromptCallback();
				g_ShowPrompt = false;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.f);
			if (ImGui::Button("No", ImVec2(100, 19)) || InputSys::Get().IsKeyDown(VK_ESCAPE))
				g_ShowPrompt = false;
			goto END; // ebat' kostil, ya hueyu
		}

		//auto backup = ImGui::GetStyle().WindowPadding.y;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::SetCursorPos(ImVec2(0, 0));

		ImVec2 tabButtonSize = ImVec2(70, 45 );
		//tabButtonSize.x = (ImGui::GetCurrentWindow()->Size.x - ImGui::GetCursorPosX()) / 8;
		int menur, menug, menub, menua, menua2;
		menur = g_Misc->options.cMenuCol[0] * 255;
		menug = g_Misc->options.cMenuCol[1] * 255;
		menub = g_Misc->options.cMenuCol[2] * 255;
		menua = g_Misc->options.cMenuCol[3] * 255;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX()+20);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY()-15);
		ImGui::PushFont(g_pMenuFont);
		ImGui::SButton("NetSense", true, ImVec2(100, 45));
		ImGui::PopFont();
		ImGui::SameLine(10);
		
		ImGui::SameLine();
		if (ImGui::SButton("Legit", page == 1, ImVec2(50, 45)))
			page = 1;
		ImGui::SameLine();
		if (ImGui::SButton("Triggerbot", page == 2, ImVec2(80, 45)))
			page = 2;
		ImGui::SameLine();
		if (ImGui::SButton("Visuals", page == 3, ImVec2(60, 45)))
			page = 3;
		ImGui::SameLine();
		if (ImGui::SButton("Skins", page == 6, ImVec2(50, 45)))
			page = 6;
		ImGui::SameLine();
		if (ImGui::SButton("Misc", page == 4, ImVec2(45, 45)))
			page = 4;
		ImGui::SameLine();
		if (ImGui::SButton("Config", page == 5, ImVec2(60, 45)))
			page = 5;

		ImVec2 p = ImGui::GetCursorScreenPos();
		p.x = ImGui::GetCurrentWindow()->Pos.x - 3.f;
		p.y -= 3.202f;
		ImGui::RenderFrame(p, ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 1), line_color);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		
		
		
		ImGui::PopStyleVar(2);

		ImGui::Dummy(ImVec2(0, 6));
		auto sz = ImGui::GetWindowSize().y - ImGui::GetCursorPosY() - 30;


		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImU32(ImColor(menur, menug, menub, 255)));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImU32(ImColor(menur, menug, menub, 255)));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImU32(ImColor(menur, menug, menub, 255)));

		if (page == 1) {
			g_Aimbot.MenuAimbot();
		}
		if (page == 2) {
			g_Aimbot.MenuTrigger();
		}
		if (page == 3) {
			g_Visuals->Menu();
		}
		if (page == 4) {
			g_Misc->Menu();
		}
		if (page == 5) {
			g_Config->Menu();
		}
		if (page == 6) {
			g_SkinChanger-> Menu();
		}
		if (page == 7) {
		}
				
		ImGui::PopStyleColor(3);
			/*case 7:
				//g_LuaApiManager->Menu();
				ImGui::Text("hhh");
				break;*/
			
		//}
		//ImGui::EndChild();

/*#ifdef _STABLE
		ImGui::Text("[stable]");
#elif _BETA
		ImGui::Text("[beta]");
#elif _DEBUG
		ImGui::Text("[dev]");
#endif

		std::string rightsidetext;
		rightsidetext += "logged in as ";
		rightsidetext += "huesos";
		rightsidetext += ", days left: ";
		rightsidetext += "1337";
		*/
		if (g_LocalPlayer && g_EngineClient && g_EngineClient->IsInGame() && g_EngineClient->IsConnected() && g_LocalPlayer->m_hActiveWeapon()) {
			short defindex = g_LocalPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex();
			std::string gname = g_CustomWeaponGroups->GetGroupName(defindex);
			if (gname.empty()) {
				ImGui::SameLine();
				ImGui::Text("   (!) Weapon not found");
			}
		}


		/*auto rightsidetext_size = ImGui::CalcTextSize(rightsidetext.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - rightsidetext_size.x - 10.f);
		ImGui::Text(rightsidetext.c_str());*/
	}
END:
	g_MenuSize = ImGui::GetCurrentWindow()->Size;
	ImGui::End();

	CAnimations::Get().End();

	
	ImGui::PopStyleColor(4);
}

void Menu::Toggle()
{
	_visible = !_visible;
	
	if (_visible) {
		AimbotFirstTimeRender = true;
		TriggerFirstTimeRender = true;
		SkinsFirstTimeRender = true;
	}
}




void Menu::CreateStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.SecondAlpha = 0.5f;
	style.WindowPadding = ImVec2(15, 0);
	style.WindowMinSize = ImVec2(32, 32);
	style.PopupRounding = 4.0f;
	style.AntiAliasedFill = 16.0f;
	style.AntiAliasedLines = 16.0f;
	style.WindowRounding = 7.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 3.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(6, 6);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 10.0f;
	style.ScrollbarSize = 3.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 3.0f;
	style.GrabRounding = 16.0f;
	style.ButtonTextAlign = ImVec2(0.0f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	//style.AntiAliasedLines = false;
	//style.CurveTessellationTol = 16.f;

	switch (g_Misc->options.newstyle) {
	case 0: imdraw::Black(); break;
	case 1: imdraw::White(); break;
	case 2: imdraw::Dark(); break;
	}

	/*colors[ImGuiCol_Text] = ImVec4(0.95, 0.96, 0.98, style.Alpha);
	colors[ImGuiCol_TextDisabled] = ImVec4(1.00, 1.00, 1.00, style.Alpha);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09, 0.10, 0.12, style.Alpha);
	colors[ImGuiCol_ChildBg] = ImVec4(0.13, 0.15, 0.17, style.Alpha);
	colors[ImGuiCol_PopupBg] = ImVec4(0.17, 0.20, 0.22, 0.95);
	colors[ImGuiCol_Border] = ImVec4(0.13, 0.15, 0.16, 0.50);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.13, 0.15, 0.16, 0.80);
	colors[ImGuiCol_FrameBg] = ImVec4(0.17, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.17, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09, 0.12, 0.14, 0.65);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 0.51);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08, 0.10, 0.12, style.Alpha);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15, 0.18, 0.22, style.Alpha);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.17, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00, 1.00, 1.00, style.Alpha);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28, 0.56, 1.00, style.Alpha);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37, 0.61, 1.00, style.Alpha);
	colors[ImGuiCol_Button] = ImVec4(0.20, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.20, 0.22, 0.50);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.20, 0.22, style.Alpha);
	colors[ImGuiCol_Header] = ImVec4(0.11, 0.20, 0.29, 0.30);///
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26, 0.59, 0.98, 0.30);// Selected
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26, 0.59, 0.98, 0.30);///
	colors[ImGuiCol_ResizeGrip] = ImColor(0.09, 0.10, 0.12, 0.0f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26, 0.59, 0.98, 0.0);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06, 0.05, 0.07, 0.00);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61, 0.61, 0.61, style.Alpha);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00, 0.43, 0.35, style.Alpha);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90, 0.70, 0.00, style.Alpha);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00, 0.60, 0.00, style.Alpha);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00, 1.00, 1.00, style.Alpha);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00, 0.98, 0.95, 0.73);*/


}


/*colors[ImGuiCol_Text] = ImVec4(0.95, 0.96, 0.98, 1.00);
colors[ImGuiCol_TextDisabled] = ImVec4(1.00, 1.00, 1.00, 1.00);
colors[ImGuiCol_WindowBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
colors[ImGuiCol_ChildWindowBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
colors[ImGuiCol_PopupBg] = ImVec4(0.08, 0.08, 0.08, 0.94);
colors[ImGuiCol_Border] = ImVec4(0.43, 0.43, 0.50, 0.50);
colors[ImGuiCol_BorderShadow] = ImVec4(0.00, 0.00, 0.00, 0.00);
colors[ImGuiCol_FrameBg] = ImVec4(0.20, 0.25, 0.29, 1.00);
colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12, 0.20, 0.28, 0.20);
colors[ImGuiCol_FrameBgActive] = ImVec4(0.09, 0.12, 0.14, 1.00);
colors[ImGuiCol_TitleBg] = ImVec4(0.09, 0.12, 0.14, 0.65);
colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 0.51);
colors[ImGuiCol_TitleBgActive] = ImVec4(0.08, 0.10, 0.12, 1.00);
colors[ImGuiCol_MenuBarBg] = ImVec4(0.15, 0.18, 0.22, 1.00);
colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02, 0.02, 0.02, 0.39);
colors[ImGuiCol_ScrollbarGrab] = ImVec4(menur, menug, menub, menua);
colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(menur, menug, menub, menua);
colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(menur, menug, menub, menua);
colors[ImGuiCol_CheckMark] = ImVec4(0.28, 0.56, 1.00, 1.00);
colors[ImGuiCol_SliderGrab] = ImVec4(0.28, 0.56, 1.00, 1.00);
colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37, 0.61, 1.00, 1.00);
colors[ImGuiCol_Button] = ImVec4(0.20, 0.25, 0.29, 1.00);
colors[ImGuiCol_ButtonHovered] = ImVec4(0.28, 0.56, 1.00, 1.00);
colors[ImGuiCol_ButtonActive] = ImVec4(1.00, 1.00, 1.00, 1.00);
colors[ImGuiCol_Header] = ImVec4(0.11, 0.20, 0.29, 0.70);
colors[ImGuiCol_HeaderHovered] = ImVec4(0.26, 0.59, 0.98, 0.80);
colors[ImGuiCol_HeaderActive] = ImVec4(0.26, 0.59, 0.98, 1.00);
colors[ImGuiCol_ResizeGrip] = ImVec4(0.26, 0.59, 0.98, 0.25);
colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26, 0.59, 0.98, 0.67);
colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06, 0.05, 0.07, 1.00);
colors[ImGuiCol_PlotLines] = ImVec4(0.61, 0.61, 0.61, 1.00);
colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00, 0.43, 0.35, 1.00);
colors[ImGuiCol_PlotHistogram] = ImVec4(0.90, 0.70, 0.00, 1.00);
colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00, 0.60, 0.00, 1.00);
colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00, 1.00, 1.00, 1.00);
colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00, 0.98, 0.95, 0.73);
*/
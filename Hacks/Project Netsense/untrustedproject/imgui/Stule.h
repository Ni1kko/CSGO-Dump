#pragma once

#include "imgui.h"

namespace imdraw
{
	void Black();
	void White();
	void Dark();

}

void imdraw::Black()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGuiStyle& style = ImGui::GetStyle();




	colors[ImGuiCol_Text] = ImVec4(0.95, 0.96, 0.98, style.Alpha);
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
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00, 0.98, 0.95, 0.73);

};
void imdraw::White()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGuiStyle& style = ImGui::GetStyle();

	colors[ImGuiCol_Text] = ImColor(0, 0, 0);
	//colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	//colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.55f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.78f, 0.78f, 0.51f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.61f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.90f, 0.90f, 0.90f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.92f, 0.92f, 0.78f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.71f, 0.78f, 0.69f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.725f, 0.805f, 0.702f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.793f, 0.900f, 0.836f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.71f, 0.78f, 0.69f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.78f, 0.69f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.78f, 0.69f, 1.00f);
	/*colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);*/
	colors[ImGuiCol_Separator] = ImVec4(0.18, 0.20, 0.22, 1.0f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	//colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);

};

void imdraw::Dark()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGuiStyle& style = ImGui::GetStyle();

	colors[ImGuiCol_Text] = ImVec4(0.95, 0.96, 0.98, 1.00);
	colors[ImGuiCol_TextDisabled] = ImVec4(1.00, 1.00, 1.00, 1.00);
	colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f);
	colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08, 0.08, 0.08, 0.94);
	colors[ImGuiCol_Border] = ImVec4(0.20f, 0.22f, 0.24f, 0.50);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00, 0.00, 0.00, 0.00);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20, 0.25, 0.29, 1.00);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12, 0.20, 0.28, 0.20);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09, 0.12, 0.14, 1.00);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09, 0.12, 0.14, 0.65);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 0.51);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08, 0.10, 0.12, 1.00);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15, 0.18, 0.22, 1.00);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02, 0.02, 0.02, 0.00);
	/*colors[ImGuiCol_ScrollbarGrab] = ImColor(menur, menug, menub, 255);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(menur, menug, menub, 255);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(menur, menug, menub, 255);*/
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
}








#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include "Tabs.h"
#include "Themes.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
void RenderInterface() {
	// Place all item settings under a collapsing header.


		static int page = 0;

		switch(g_Options.Menu.Theme)
		{
		case 0:
			RenderRay();
			break;
		case 1:
			RenderMono();
			break;
		case 2:
			RenderDefault();
			break;
		}
	


		ImGui::SetNextWindowSize(ImVec2(960, 446), ImGuiSetCond_FirstUseEver);
		if (ImGui::Begin(XorStr("big (large) xiaomi hook"), &g_Options.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | G::extra_flags))
		{
			const char* tabs[] = {
                "Rage",
				"Legit",
				"Visuals",
				"Misc",
                "Skinchanger"
			};

			for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
			{

				//ImGui::GetStyle().Colors[ImGuiCol_Button] = ImColor(54, 54, 54, 255);

				if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0)))
					page = i;

				//ImGui::GetStyle().Colors[ImGuiCol_Button] = ImColor(54, 54, 54, 255);

				if (i < IM_ARRAYSIZE(tabs) - 1)
					ImGui::SameLine();
			}

			ImGui::Separator();

			switch (page)
			{
            case 0:
                RenderRageBot();
                break;
			case 1:
                RenderLegit();
				break;
			case 2:
                RenderVisuals();
				break;
			case 3:
                RenderMisc();
				break;
			case 4:
                RenderSkinChanger();
				break;
			}
			ImGui::End();
		}
}

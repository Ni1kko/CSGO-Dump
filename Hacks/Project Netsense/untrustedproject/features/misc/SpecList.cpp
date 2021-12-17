#include "SpecList.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../render.hpp"

SpecList* g_SpecList = new SpecList();

struct spec_t {
	std::string name;
	std::string mode;
};

void SpecList::Draw() {
	if (!params.enabled) return;

	std::string specs = "";
	std::string modes = "";

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
		C_BasePlayer* pLocalEntity = g_LocalPlayer;

		if (g_LocalPlayer && !g_LocalPlayer->IsAlive())
			pLocalEntity = g_LocalPlayer->m_hObserverTarget();

		if (pLocalEntity) {
			for (int i = 0; i < g_EngineClient->GetMaxClients(); i++) {
				C_BasePlayer* pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);
				if (!pBaseEntity)										     continue;
				if (pBaseEntity->m_iHealth() > 0)							 continue;
				if (pBaseEntity == pLocalEntity)							 continue;
				if (pBaseEntity->IsDormant())								 continue;
				if (pBaseEntity->m_hObserverTarget() != pLocalEntity)		 continue;
				player_info_t pInfo;
				g_EngineClient->GetPlayerInfo(pBaseEntity->EntIndex(), &pInfo);
				if (pInfo.ishltv) continue;

				std::string mode = "unknown";
				switch (pBaseEntity->m_iObserverMode())
				{
				case OBS_MODE_CHASE:
					mode = ("3rd Person");
					break;
				case OBS_MODE_DEATHCAM:
					mode = ("DeathCams");
					break;
				case OBS_MODE_FIXED:
					mode = ("Fixed");
					break;
				case OBS_MODE_FREEZECAM:
					mode = ("FreezeCam");
					break;
				case OBS_MODE_IN_EYE:
					mode = ("1st Person");
					break;
				case OBS_MODE_NONE:
					mode = ("None");
					break;
				case OBS_MODE_ROAMING:
					mode = ("Roaming");
					break;
				}

				specs += pInfo.szName;
				specs += "\n";

				modes += mode;
				modes += "\n";
			}
		}
	}
	if (specs.size() > 0) specs += "\n";

	if (ImGui::Begin("##spectre_speclist", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar)) {
		if (!ImGui::IsMouseDragging())
			ImGui::SetWindowPos(ImVec2(params.x, params.y));

		ImGui::PushFont(g_pDefaultFont);
		ImGui::Text("Spectator List");
		ImGui::PopFont();
		auto p = ImGui::GetCursorPos();
		auto list = ImGui::GetWindowDrawList();
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		list->AddRectFilled(ImVec2(pos.x, pos.y + 32), ImVec2(pos.x + size.x, pos.y + 33), g_Misc->colors.accent_color);
		ImGui::Dummy(ImVec2(5, 4));



		ImVec2 pWsize = ImGui::CalcTextSize(specs.c_str());
		ImGui::SetWindowSize(ImVec2(200, 36 + pWsize.y));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::BeginGroup();
		ImGui::Columns(2, nullptr, false);

		ImGui::Text(specs.c_str());
		ImGui::NextColumn();
		ImGui::Text(modes.c_str());

		ImGui::EndGroup();


		ImGui::PopStyleVar();
	}
	auto p = ImGui::GetWindowPos();
	params.x = p.x;
	params.y = p.y;
	ImGui::End();
}



void SpecList::SetupValues() {
	g_Config->PushItem(&params.enabled, "spec_list", "enabled", params.enabled);
	g_Config->PushItem(&params.x, "spec_list", "x", params.x);
	g_Config->PushItem(&params.y, "spec_list", "y", params.y);


}

static int watermark_x = 285;
static int watermark_y = 60;
#include <iostream>
#include <chrono>
#include <ctime> 

WaterMark* g_WaterMark = new WaterMark();

void WaterMark::DrawMark()
{
	if (!params.enabled) return;

	int Width;
	int Height;
	g_EngineClient->GetScreenSize(Width, Height);
	SIZE sz;
	int CenterX = Width / 1.19;
	int CenterY = Height / 160;

	ImGuiContext& g = *GImGui;
	//ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowWidth() + 0, ImGui::GetWindowHeight() - 0));
	ImGui::SetNextWindowSize(ImVec2(watermark_x, watermark_y));
	//if (config.visuals.water)
	//{
	ImGui::SetNextWindowPos(ImVec2(CenterX, CenterY));
	ImGui::Begin("WaterMark", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavInputs);
	{
		//if (!ImGui::IsMouseDragging())
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
		int menur, menug, menub, menua, menua2;
		menur = g_Misc->options.cMenuCol[0] * 255;
		menug = g_Misc->options.cMenuCol[1] * 255;
		menub = g_Misc->options.cMenuCol[2] * 255;
		menua = g_Misc->options.cMenuCol[3] * 255;

		ImGui::PushFont(g_pSecondFont);
		ImGui::SameLine(90);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY());
		ImGui::TextColored(ImColor(menur, menug, menub, 255), "NetSense");
		ImGui::PopFont();
		ImVec2 p = ImGui::GetCursorScreenPos();
		auto wp = ImGui::GetWindowPos();
		auto ws = ImGui::GetWindowSize();
		//ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(wp.x, wp.y + 1), ImVec2(wp.x + ws.x, wp.y + 20), ImColor(0, 243, 255, 0), ImColor(0, 243, 255, 0), ImColor(0, 243, 255, 40), ImColor(0, 243, 255, 40));//âåðõíèé ãðàäèåíò
		std::string logo = "[BETA]";
		//_________________________________Time___________________________________//
		char timebuff[80];
		time_t now = time(0);   // get time now
		struct tm  tstruct;
		tstruct = *localtime(&now);
		strftime(timebuff, sizeof(timebuff), "%X", &tstruct);
		//_________________________________New___________________________________//



		auto fps = static_cast<int>(1.f / g_GlobalVars->frametime);
		auto tick_rate = (int)(1.f / g_GlobalVars->interval_per_tick);
		float latency = 0.0f;
		std::to_wstring(static_cast<int>(latency * 1000));
		//________________________________Render________________________________//
		std::stringstream ss;
		
		ss << "      " << timebuff << " | " << tick_rate << "  tick" << " | " << "FPS: " << fps << " | " << logo;
		//_____________________________Render ImGui_______________________________//
		ImGui::Text(ss.str().c_str());

		//}//ImGui::PopFont();
		//ImGui::EndChild();

		auto r = ImGui::GetWindowPos();
		params.x = r.x;
		params.y = r.y;
	}
	ImGui::End();



	//}

}

void WaterMark::SetupValuesMark() {
	g_Config->PushItem(&params.enabled, "spec_list", "enabled", params.enabled);
	g_Config->PushItem(&params.x, "spec_list", "x", params.x);
	g_Config->PushItem(&params.y, "spec_list", "y", params.y);
}
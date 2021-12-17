#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <thread>

#include "valve_sdk/csgostructs.hpp"
#include "features/inventory.hpp"
#include "features/skins.hpp"
#include "helpers/input.hpp"
#include "helpers/items.hpp"
#include "options.hpp"
#include "ui.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

#include "render.hpp"

std::string imData;
std::string link = "";
IDirect3DTexture9* skinImage = nullptr;

void downloadImage()
{
	imData = Preview::DownloadBytes(link.c_str());
	D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, imData.data(), imData.length(), 512, 384, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &skinImage);
}

enum class TAB : int
{
	RAGEBOT = 0,
	AIMBOT,
	VISUALS,
	CHANGERS,
	MISC,
	CONFIG
};

void ReadDirectory(const std::string& name, std::vector<std::string>& v)
{
	auto pattern(name);
	pattern.append("\\*.ini");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			v.emplace_back(data.cFileName);
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
}

namespace ImGuiEx
{
    inline bool ColorPicker4(const char* label, Color* v, bool show_alpha = true)
    {
        float clr[4] = {
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };

        if(ImGui::ColorEdit4(label, clr, show_alpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar)) {
            v->SetColor(clr[0], clr[1], clr[2], clr[3]);
            return true;
        }
        return false;
    }
    inline bool ColorPicker3(const char* label, Color* v)
    {
        return ColorPicker4(label, v, false);
    }
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for(auto i = 0; i < N; ++i) {
        if(ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if(sameline && i < N - 1)
            ImGui::SameLine();
    }
}

void Menu::Initialize()
{
	CreateStyle();

    _visible = false;
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

void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = _visible;

	ImGui::GetStyle() = _style;

    if(!_visible)
        return;

	static int definition_index = WEAPON_INVALID;
	int localPlayerID = g_EngineClient->GetLocalPlayer();
	auto localPlayer = C_BasePlayer::GetPlayerByIndex(localPlayerID);
	if (g_EngineClient->IsInGame() && localPlayer && localPlayer->IsAlive() && localPlayer->m_hActiveWeapon() && localPlayer->m_hActiveWeapon()->IsGun())
		definition_index = localPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex();
	else
		definition_index = WEAPON_INVALID;

	static TAB MenuTab = TAB::CONFIG;

	static auto wndSize = ImVec2(800, 600);
	ImGui::SetNextWindowSize(wndSize);
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
	if (ImGui::Begin("ikeaware", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings))
	{
		auto wndPos = ImGui::GetWindowPos();

		auto drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilledMultiColor(wndPos, ImVec2(wndPos.x + wndSize.x, wndPos.y + 40), ImColor(10 / 255.f, 21 / 255.f, 100 / 255.f, 1.f), ImColor(154 / 255.f, 22 / 255.f, 71 / 255.f, 1.f), ImColor(7 / 255.f, 7 / 255.f, 17 / 255.f, 1.f), ImColor(3 / 255.f, 79 / 255.f, 79 / 255.f, 1.f));
		static auto titleTextSize = ImGui::GetFont()->CalcTextSizeA(18.f, FLT_MAX, NULL, "ikeaware");
		drawList->AddText(ImVec2(wndPos.x + (wndSize.x - titleTextSize.x) / 2, wndPos.y + (40 - titleTextSize.y) / 2), ImColor(255, 255, 255, 255), "ikeaware");

		drawList->AddLine(ImVec2(wndPos.x, wndPos.y + 40), ImVec2(wndPos.x + wndSize.x, wndPos.y + 40), ImColor(_style.Colors[ImGuiCol_Border]));
		ImGui::SetCursorPosY(48);

		ImGui::BeginChild("tabs");
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
		
		if (MenuTab == TAB::RAGEBOT)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Ragebot", ImVec2(175, 40)))
			MenuTab = TAB::RAGEBOT;
		
		if (MenuTab == TAB::AIMBOT)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Legitbot", ImVec2(175, 40)))
			MenuTab = TAB::AIMBOT;

		if (MenuTab == TAB::VISUALS)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Visuals", ImVec2(175, 40)))
			MenuTab = TAB::VISUALS;

		if (MenuTab == TAB::CHANGERS)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Changers", ImVec2(175, 40)))
			MenuTab = TAB::CHANGERS;

		if (MenuTab == TAB::MISC)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Misc", ImVec2(175, 40)))
			MenuTab = TAB::MISC;

		if (MenuTab == TAB::CONFIG)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1, 0.12, 0.49, 1.000));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));

		if (ImGui::Button("   Configs", ImVec2(175, 40)))
			MenuTab = TAB::CONFIG;

		static int active_changers_tab = 0;
		static wskin weaponSkin;

		ImGui::PopStyleColor(9);
		ImGui::PopStyleVar(2);

		ImGui::Dummy(ImVec2(1, 220));
		if (ImGui::Button("VK GROUP", ImVec2(176, 40)))
			ShellExecute(nullptr, TEXT("open"), TEXT("https://vk.com/simv0lcrew"), nullptr, nullptr, 1);
		ImGui::EndChild();

		drawList->AddRectFilledMultiColor(ImVec2(wndPos.x, wndPos.y + 40 * int(MenuTab) + 48), ImVec2(wndPos.x + 9, wndPos.y + 40 * (int(MenuTab) + 1) + 48), ImColor(25 / 255.f, 31 / 255.f, 125 / 255.f, 1.f), ImColor(25 / 255.f, 31 / 255.f, 125 / 255.f, 0.f), ImColor(25 / 255.f, 31 / 255.f, 125 / 255.f, 0.f), ImColor(25 / 255.f, 31 / 255.f, 125 / 255.f, 1.f));

		drawList->AddLine(ImVec2(wndPos.x + 176, wndPos.y + 40), ImVec2(wndPos.x + 176, wndPos.y + wndSize.y), ImColor(_style.Colors[ImGuiCol_Border]));

		ImGui::SetCursorPos(ImVec2(175 + _style.FramePadding.x, 40 + _style.FramePadding.y));

		ImGui::BeginChild("content");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		{
			float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
			const auto style = ImGui::GetStyle();
			if (MenuTab == TAB::RAGEBOT)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
				ImGui::Columns(3, nullptr, false);
				ImGui::SetColumnOffset(1, group_w / 3.0f);
				ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
				ImGui::SetColumnOffset(3, group_w);

				ImGui::PushItemWidth(-1);
				//1 вкладка
				ImGui::Separator("General");
				ImGui::Checkbox("Enabled", &g_Options.ragebotconfig.bEnabled);
				const char* ragebotmode[] = {
						"Visualized",
						"Silent"
				};
				if (ImGui::BeginCombo("##rbottype", ragebotmode[g_Options.ragebotconfig.iAimbotMode], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(ragebotmode); i++)
					{
						if (ImGui::Selectable(ragebotmode[i], i == g_Options.ragebotconfig.iAimbotMode))
							g_Options.ragebotconfig.iAimbotMode = i;
					}

					ImGui::EndCombo();
				}

				const char* ragebotselectmode[] = {
						"FOV",
						"Distance",
						"Lowest health"
				};
				if (ImGui::BeginCombo("##rbotselecttype", ragebotselectmode[g_Options.ragebotconfig.iTargetSelection], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(ragebotselectmode); i++)
					{
						if (ImGui::Selectable(ragebotselectmode[i], i == g_Options.ragebotconfig.iTargetSelection))
							g_Options.ragebotconfig.iTargetSelection = i;
					}

					ImGui::EndCombo();
				}
				ImGui::SliderInt("##ragemaxfov", &g_Options.ragebotconfig.iMaxFOV, 0, 360, "Max FOV: %i");
				ImGui::Checkbox("Auto-fire", &g_Options.ragebotconfig.bAutoFire);
				ImGui::Checkbox("Friendly fire", &g_Options.ragebotconfig.bFriendlyFire);
				ImGui::Checkbox("Auto-scope", &g_Options.ragebotconfig.bAutoScope);
				ImGui::Checkbox("Auto-stop", &g_Options.ragebotconfig.bAutostop);
				
				ImGui::Checkbox("Automatic penetrate", &g_Options.ragebotconfig.bAutoWall);
				ImGui::SliderInt("##rageminDamage", &g_Options.ragebotconfig.flMinDmg, 0, 100, "Min Damage: %i");

				ImGui::Checkbox("Hitchance", &g_Options.ragebotconfig.bHitchance);
				ImGui::SliderInt("##rageminhitchance", &g_Options.ragebotconfig.flHitchanceAmt, 0, 99, "Hitchance: %i");

				ImGui::Checkbox("No-recoil", &g_Options.ragebotconfig.bRemoveRecoil);

				ImGui::NextColumn();

				ImGui::PushItemWidth(-1);

				//2 вкладка
				ImGui::Separator("Hitscan & other");
				if (ImGui::BeginCombo("##ragehitbox_filter", "Hitboxes", ImGuiComboFlags_NoArrowButton))
				{
					ImGui::Selectable("Head", &g_Options.ragebotconfig.head, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("Chest", &g_Options.ragebotconfig.chest, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("Body", &g_Options.ragebotconfig.body, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("Hands", &g_Options.ragebotconfig.hands, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("Legs", &g_Options.ragebotconfig.legs, ImGuiSelectableFlags_DontClosePopups);

					ImGui::EndCombo();
				}
				/*ImGui::SliderInt("##head", &g_Options.ragebotconfig.headmp, 0, 100, "Head pointscale: %i");
				ImGui::SliderInt("##chest", &g_Options.ragebotconfig.chestmp, 0, 100, "Chest pointscale: %i");
				ImGui::SliderInt("##body", &g_Options.ragebotconfig.bodymp, 0, 100, "Body pointscale: %i");
				ImGui::SliderInt("##hands", &g_Options.ragebotconfig.handsmp, 0, 100, "Hands pointscale: %i");
				ImGui::SliderInt("##legs", &g_Options.ragebotconfig.legsmp, 0, 100, "Legs pointscale: %i");*/

				ImGui::Checkbox("Baim when enemy in air", &g_Options.ragebotconfig.bPreferBaim);
				ImGui::Checkbox("Baim if lethal", &g_Options.ragebotconfig.bBaimIfDeadly);
				ImGui::SliderInt("##ragebaimonx", &g_Options.ragebotconfig.flBaimOnX, 0, 100, "Baim when enemy xp < %i");


				ImGui::NextColumn();

				ImGui::PushItemWidth(-1);
				//3 вкладка
				ImGui::Separator("Antiaim");
				ImGui::SliderInt("##ragepitch", &g_Options.ragebotconfig.pitch, -90, 90, "Custom pitch: %i");
				ImGui::SliderInt("##rageyaw", &g_Options.ragebotconfig.yaw, -180, 180, "Custom yaw: %i");
				ImGui::SliderInt("##ragejittermin", &g_Options.ragebotconfig.jittermin, -180, 180, "Jitter min: %i");
				ImGui::SliderInt("##ragejittermax", &g_Options.ragebotconfig.jittermax, -180, 180, "Jitter max: %i");
				ImGui::Checkbox("Desync animations", &g_Options.ragebotconfig.desync);
				const char* desynctype[] = {
						"Desync while breaking LBY",
						"Desync without breaking LBY"
				};
				if (ImGui::BeginCombo("##desynctype", desynctype[g_Options.ragebotconfig.desynctype], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(desynctype); i++)
					{
						if (ImGui::Selectable(desynctype[i], i == g_Options.ragebotconfig.desynctype))
							g_Options.ragebotconfig.desynctype = i;
					}

					ImGui::EndCombo();
				}
				ImGui::SliderInt("##ragebodylean", &g_Options.ragebotconfig.bodylean, 0, 100, "Body lean: %i");
				ImGui::SliderInt("##rageinvbodylean", &g_Options.ragebotconfig.invbodylean, 0, 100, "Inverted body lean: %i");
				ImGui::Hotkey("Switch side##desync", &g_Options.ragebotconfig.switchdesyncside);
				ImGui::Checkbox("Slow motion", &g_Options.ragebotconfig.slowwalk);
				ImGui::SliderInt("##slowwalkspeed", &g_Options.ragebotconfig.slowwalkspeed, 0, 100, "Slow motion speed: %i");
				ImGui::Hotkey("Hotkey##slowwalk", &g_Options.ragebotconfig.slowwalkkey);
				ImGui::PopItemWidth();

				ImGui::Columns(1, nullptr, false);
				ImGui::PopStyleVar();
			}
			if (MenuTab == TAB::AIMBOT)
			{
				if (definition_index == WEAPON_INVALID)
					drawList->AddText(ImGui::GetWindowPos() + ImGui::GetWindowSize() / 2.f - ImGui::CalcTextSize("Take a gun") / 2.f, ImColor(255, 255, 255, 255), "Take a gun");
				else
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnOffset(1, group_w / 2.0f);
					ImGui::SetColumnOffset(2, group_w);

					auto settings = &g_Options.weapons[definition_index].legit;

					ImGui::PushItemWidth(-1);

					ImGui::Separator("General");

					ImGui::Checkbox("Enabled", &settings->enabled);
					ImGui::Checkbox("Friendly fire", &settings->deathmatch);
					ImGui::Checkbox("Perfect silent", &settings->silent);
					ImGui::Checkbox("Flash check", &settings->flash_check);
					ImGui::Checkbox("Smoke check", &settings->smoke_check);
					ImGui::Checkbox("Auto-pistol", &settings->autopistol);

					if (ImGui::BeginCombo("##hitbox_filter", "Hitboxes", ImGuiComboFlags_NoArrowButton))
					{
						ImGui::Selectable("Head", &settings->hitboxes.head, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Chest", &settings->hitboxes.chest, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Hands", &settings->hitboxes.hands, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Legs", &settings->hitboxes.legs, ImGuiSelectableFlags_DontClosePopups);

						ImGui::EndCombo();
					}

					ImGui::SliderFloat("##fov", &settings->fov, 0.f, 20.f, "Fov: %.2f");
					if (settings->silent)
						ImGui::SliderFloat("##psilentFov", &settings->silent_fov, 0.f, 20.f, "Perfect silent fov: %.2f");
					ImGui::SliderFloat("##smooth", &settings->smooth, 1.f, 20.f, "Smooth: %.2f");

					ImGui::Separator("Delays");
					if (!settings->silent)
						ImGui::SliderInt("##shotDelay", &settings->shot_delay, 0, 1000, "Shot delay: %i");
					ImGui::SliderInt("##killDelay", &settings->kill_delay, 0, 1000, "Kill delay: %i");

					ImGui::PopItemWidth();

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);

					ImGui::Separator("RCS");

					ImGui::Checkbox("Enabled##rcs", &settings->rcs.enabled);

					const char* rcs_types[] = {
						"Type: Standalone",
						"Type: Aim"
					};


					if (ImGui::BeginCombo("##type", rcs_types[settings->rcs.type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(rcs_types); i++)
						{
							if (ImGui::Selectable(rcs_types[i], i == settings->rcs.type))
								settings->rcs.type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::SliderInt("##start", &settings->rcs.start, 1, 30, "Start: %i");
					ImGui::SliderInt("##x", &settings->rcs.x, 0, 100, "X: %i");
					ImGui::SliderInt("##y", &settings->rcs.y, 0, 100, "Y: %i");

					ImGui::Separator("Automatic penetrate");

					ImGui::Checkbox("Enabled##autowall", &settings->autowall.enabled);
					ImGui::SliderInt("##minDamage", &settings->autowall.min_damage, 0, 100, "Min Damage: %i");

					ImGui::Separator("Auto-fire");
					ImGui::Checkbox("Enabled##autofire", &settings->autofire.enabled);
					ImGui::Hotkey("Key##autofire", &settings->autofire.hotkey);

					ImGui::Columns(1, nullptr, false);
					ImGui::PopStyleVar();
				}
			}
			else if (MenuTab == TAB::VISUALS)
			{
				static char* esp_tab_names[] = { "Enemies", "Teammates", "Local", "Other" };
				static int   active_esp_tab = 0;

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				{
					render_tabs(esp_tab_names, active_esp_tab, group_w / _countof(esp_tab_names), 40.f, true);
				}
				ImGui::PopStyleVar();

				ImGui::Spacing();

				if (active_esp_tab == 0) {
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnOffset(1, group_w / 2.0f);
					ImGui::SetColumnOffset(2, group_w);
					ImGui::PushItemWidth(-1);
					ImGui::Separator("ESP");
					ImGui::Checkbox("Enabled##esp_emenies", &g_Options.esp.player_enemies);
					ImGui::Checkbox("Occluded##player_enemies_occluded", &g_Options.esp.player_enemies_occluded);
					ImGui::Checkbox("Boxes##esp_emenies", &g_Options.esp.player_boxes_enemies);
					ImGui::Checkbox("Names##esp_emenies", &g_Options.esp.player_names_enemies);
					ImGui::Checkbox("Health##esp_emenies", &g_Options.esp.player_health_enemies);
					ImGui::Checkbox("Armour##esp_emenies", &g_Options.esp.player_armour_enemies);
					ImGui::Checkbox("Weapon##esp_emenies", &g_Options.esp.player_weapons_enemies);
					ImGui::Checkbox("Sounds##esp_emenies", &g_Options.esp.sound_enemies);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_esp_sounds_enemies", &g_Options.colors.esp.sounds_enemies);
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::Separator("Colored models");
					ImGui::Checkbox("Visible##chams_enemies_visible", &g_Options.chams.player_enemies);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_chams_enemies_visible", &g_Options.colors.chams.player_enemy_visible);
					ImGui::Checkbox("Visible shine##chams_enemies_visible_shine", &g_Options.chams.player_enemies_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_chams_enemies_visible_shine", &g_Options.colors.chams.player_enemy_visible_shine);
					const char* chams_enemies_visible_type [] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams_enemies_visible_type", chams_enemies_visible_type[g_Options.chams.player_enemies_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(chams_enemies_visible_type); i++)
						{
							if (ImGui::Selectable(chams_enemies_visible_type[i], i == g_Options.chams.player_enemies_type))
								g_Options.chams.player_enemies_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Checkbox("Occluded##chams_enemies_occluded", &g_Options.chams.player_enemies_occluded);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_chams_enemies_occluded", &g_Options.colors.chams.player_enemy_occluded);
					ImGui::Checkbox("Occluded shine##chams_enemies_occluded_shine", &g_Options.chams.player_enemies_occluded_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_chams_enemies_occluded_shine", &g_Options.colors.chams.player_enemy_occluded_shine);
					const char* chams_enemies_occluded_type[] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams_enemies_occluded_type", chams_enemies_occluded_type[g_Options.chams.player_enemies_occluded_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(chams_enemies_occluded_type); i++)
						{
							if (ImGui::Selectable(chams_enemies_occluded_type[i], i == g_Options.chams.player_enemies_occluded_type))
								g_Options.chams.player_enemies_occluded_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Separator("Glow");
					ImGui::Checkbox("Enabled##glow_enemies", &g_Options.glow.enemies);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##color_glow_enemies", &g_Options.colors.glow.enemy, true);
					const char* glow_enemies_type[] = {
						"Outline outer",
						"Pulse",
						"Outline inner"
					};
					if (ImGui::BeginCombo("##glow_enemies_type", glow_enemies_type[g_Options.glow.enemies_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(glow_enemies_type); i++)
						{
							if (ImGui::Selectable(glow_enemies_type[i], i == g_Options.glow.enemies_type))
								g_Options.glow.enemies_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Columns(1, nullptr, false);
					ImGui::PopStyleVar();
				}
				else if (active_esp_tab == 1) {
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnOffset(1, group_w / 2.0f);
					ImGui::SetColumnOffset(2, group_w);
					ImGui::PushItemWidth(-1);
					ImGui::Separator("ESP");
					ImGui::Checkbox("Enabled##esp.player_teammates", &g_Options.esp.player_teammates);
					ImGui::Checkbox("Occluded##esp.player_teammates_occluded", &g_Options.esp.player_teammates_occluded);
					ImGui::Checkbox("Boxes##esp.player_boxes_teammates", &g_Options.esp.player_boxes_teammates);
					ImGui::Checkbox("Names##esp.player_names_teammates", &g_Options.esp.player_names_teammates);
					ImGui::Checkbox("Health##esp.player_health_teammates", &g_Options.esp.player_health_teammates);
					ImGui::Checkbox("Armour##esp.player_armour_teammates", &g_Options.esp.player_armour_teammates);
					ImGui::Checkbox("Weapon##esp.player_weapons_teammates", &g_Options.esp.player_weapons_teammates);
					ImGui::Checkbox("Sounds##esp.sound_teammates", &g_Options.esp.sound_teammates);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##esp.sounds_teammates", &g_Options.colors.esp.sounds_teammates);
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::Separator("Colored models");
					ImGui::Checkbox("Visible##chams.player_teammates", &g_Options.chams.player_teammates);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_ally_visible", &g_Options.colors.chams.player_ally_visible);
					ImGui::Checkbox("Visible shine##chams.player_teammates_shine", &g_Options.chams.player_teammates_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_ally_visible_shine", &g_Options.colors.chams.player_ally_visible_shine);
					const char* player_teammates_type[] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams.player_teammates_type", player_teammates_type[g_Options.chams.player_teammates_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(player_teammates_type); i++)
						{
							if (ImGui::Selectable(player_teammates_type[i], i == g_Options.chams.player_teammates_type))
								g_Options.chams.player_teammates_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Checkbox("Occluded##chams.player_teammates_occluded", &g_Options.chams.player_teammates_occluded);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_ally_occluded", &g_Options.colors.chams.player_ally_occluded);
					ImGui::Checkbox("Occluded shine##chams.player_teammates_occluded_shine", &g_Options.chams.player_teammates_occluded_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_ally_occluded_shine", &g_Options.colors.chams.player_ally_occluded_shine);
					const char* player_teammates_occluded_type[] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams.player_teammates_occluded_type", player_teammates_occluded_type[g_Options.chams.player_teammates_occluded_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(player_teammates_occluded_type); i++)
						{
							if (ImGui::Selectable(player_teammates_occluded_type[i], i == g_Options.chams.player_teammates_occluded_type))
								g_Options.chams.player_teammates_occluded_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Separator("Glow");
					ImGui::Checkbox("Enabled##glow.teammates", &g_Options.glow.teammates);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##glow.ally", &g_Options.colors.glow.ally, true);
					const char* teammates_type[] = {
						"Outline outer",
						"Pulse",
						"Outline inner"
					};
					if (ImGui::BeginCombo("##glow.teammates_type", teammates_type[g_Options.glow.teammates_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(teammates_type); i++)
						{
							if (ImGui::Selectable(teammates_type[i], i == g_Options.glow.teammates_type))
								g_Options.glow.teammates_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Columns(1, nullptr, false);
					ImGui::PopStyleVar();
				}
				else if (active_esp_tab == 2) {
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnOffset(1, group_w / 2.0f);
					ImGui::SetColumnOffset(2, group_w);
					ImGui::PushItemWidth(-1);
					ImGui::Separator("ESP");
					ImGui::Checkbox("Enabled##esp.player_local", &g_Options.esp.player_local);
					ImGui::Checkbox("Occluded##esp.player_local_occluded", &g_Options.esp.player_local_occluded);
					ImGui::Checkbox("Boxes##esp.player_boxes_local", &g_Options.esp.player_boxes_local);
					ImGui::Checkbox("Names##esp.player_names_local", &g_Options.esp.player_names_local);
					ImGui::Checkbox("Health##esp.player_health_local", &g_Options.esp.player_health_local);
					ImGui::Checkbox("Armour##esp.player_armour_local", &g_Options.esp.player_armour_local);
					ImGui::Checkbox("Weapon##esp.player_weapons_local", &g_Options.esp.player_weapons_local);
					ImGui::Checkbox("Sounds##esp.sound_local", &g_Options.esp.sound_local);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##esp.sounds_local", &g_Options.colors.esp.sounds_local);
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::Separator("Colored models");
					ImGui::Checkbox("Visible##chams.player_local", &g_Options.chams.player_local);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_local_visible", &g_Options.colors.chams.player_local_visible);
					ImGui::Checkbox("Visible shine##chams.player_local_shine", &g_Options.chams.player_local_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_local_visible_shine", &g_Options.colors.chams.player_local_visible_shine);
					const char* player_local_type[] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams.player_local_type", player_local_type[g_Options.chams.player_local_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(player_local_type); i++)
						{
							if (ImGui::Selectable(player_local_type[i], i == g_Options.chams.player_local_type))
								g_Options.chams.player_local_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Checkbox("Occluded##chams.player_local_occluded", &g_Options.chams.player_local_occluded);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_local_occluded", &g_Options.colors.chams.player_local_occluded);
					ImGui::Checkbox("Occluded shine##chams.player_local_occluded_shine", &g_Options.chams.player_local_occluded_shine);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##chams.player_local_occluded_shine", &g_Options.colors.chams.player_local_occluded_shine);
					const char* player_local_occluded_type[] = {
						"Normal", "Flat", "Glass", "Glow"
					};
					if (ImGui::BeginCombo("##chams.player_local_occluded_type", player_local_occluded_type[g_Options.chams.player_local_occluded_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(player_local_occluded_type); i++)
						{
							if (ImGui::Selectable(player_local_occluded_type[i], i == g_Options.chams.player_local_occluded_type))
								g_Options.chams.player_local_occluded_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Separator("Glow");
					ImGui::Checkbox("Enabled##glow.local", &g_Options.glow.local);
					ImGui::SameLine(group_w / 2.0f - 28.f);
					ImGuiEx::ColorPicker4("##glow.local", &g_Options.colors.glow.local, true);
					const char* local_type[] = {
						"Outline outer",
						"Pulse",
						"Outline inner"
					};
					if (ImGui::BeginCombo("##glow.local_type", local_type[g_Options.glow.local_type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(local_type); i++)
						{
							if (ImGui::Selectable(local_type[i], i == g_Options.glow.local_type))
								g_Options.glow.local_type = i;
						}

						ImGui::EndCombo();
					}
					ImGui::Columns(1, nullptr, false);
					ImGui::PopStyleVar();
				}
				else if (active_esp_tab == 3) {
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, group_w / 2.0f);
				ImGui::SetColumnOffset(2, group_w);
				ImGui::PushItemWidth(-1);
				ImGui::Separator("ESP");
				ImGui::Checkbox("Dropped weapons", &g_Options.esp.dropped_weapons);	
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##esp.weapons", &g_Options.colors.esp.weapons, true);
				ImGui::Checkbox("Defuse kits", &g_Options.esp.defuse_kit);		
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##esp.defuse", &g_Options.colors.esp.defuse, true);
				ImGui::Checkbox("Planted C4", &g_Options.esp.planted_c4);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##esp.c4", &g_Options.colors.esp.c4, true);
				ImGui::Checkbox("Items", &g_Options.esp.items);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##esp.item", &g_Options.colors.esp.item, true);
				
				ImGui::Separator("Glow");
				ImGui::Checkbox("Dropped weapons", &g_Options.glow.weapons);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##glow.weapons", &g_Options.colors.glow.weapons, true);
				ImGui::Checkbox("Chikens", &g_Options.glow.chickens);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##glow.chickens", &g_Options.colors.glow.chickens, true);
				ImGui::Checkbox("Planted C4", &g_Options.glow.planted_c4);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##glow.planted_c4", &g_Options.colors.glow.planted_c4, true);

				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Separator("Arms colored models");

				ImGui::Checkbox("Visible##arms", &g_Options.chams.arms);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##arms_visible", &g_Options.colors.chams.arms_visible);
				ImGui::Checkbox("Visible shine##arms_shine", &g_Options.chams.arms_shine);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##arms_visible_shine", &g_Options.colors.chams.arms_visible_shine);
				const char* arms_type[] = {
					"Normal", "Flat", "Glass", "Glow"
				};
				if (ImGui::BeginCombo("##arms_type", arms_type[g_Options.chams.arms_type], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(arms_type); i++)
					{
						if (ImGui::Selectable(arms_type[i], i == g_Options.chams.arms_type))
							g_Options.chams.arms_type = i;
					}

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Occluded##arms_occluded", &g_Options.chams.arms_occluded);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##arms_occluded", &g_Options.colors.chams.arms_occluded);
				ImGui::Checkbox("Occluded shine##arms_occluded_shine", &g_Options.chams.arms_occluded_shine);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##arms_occluded_shine", &g_Options.colors.chams.arms_occluded_shine);
				const char* arms_occluded_type[] = {
					"Normal", "Flat", "Glass", "Glow"
				};
				if (ImGui::BeginCombo("##arms_occluded_type", arms_occluded_type[g_Options.chams.arms_occluded_type], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(arms_occluded_type); i++)
					{
						if (ImGui::Selectable(arms_occluded_type[i], i == g_Options.chams.arms_occluded_type))
							g_Options.chams.arms_occluded_type = i;
					}

					ImGui::EndCombo();
				}
				ImGui::Separator("Weapon colored models");
				ImGui::Checkbox("Visible##weapon", &g_Options.chams.weapon);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##weapon_visible", &g_Options.colors.chams.weapon_visible);
				ImGui::Checkbox("Visible shine##weapon_shine", &g_Options.chams.weapon_shine);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##weapon_visible_shine", &g_Options.colors.chams.weapon_visible_shine);
				const char* weapon_type[] = {
					"Normal", "Flat", "Glass", "Glow"
				};
				if (ImGui::BeginCombo("##weapon_type", weapon_type[g_Options.chams.weapon_type], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(weapon_type); i++)
					{
						if (ImGui::Selectable(weapon_type[i], i == g_Options.chams.weapon_type))
							g_Options.chams.weapon_type = i;
					}

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Occluded##weapon_occluded", &g_Options.chams.weapon_occluded);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##weapon_occluded", &g_Options.colors.chams.weapon_occluded);
				ImGui::Checkbox("Occluded shine##weapon_occluded_shine", &g_Options.chams.weapon_occluded_shine);
				ImGui::SameLine(group_w / 2.0f - 28.f);
				ImGuiEx::ColorPicker4("##weapon_occluded_shine", &g_Options.colors.chams.weapon_occluded_shine);
				const char* weapon_occluded_type[] = {
					"Normal", "Flat", "Glass", "Glow"
				};
				if (ImGui::BeginCombo("##weapon_occluded_type", weapon_occluded_type[g_Options.chams.weapon_occluded_type], ImGuiComboFlags_NoArrowButton))
				{
					for (int i = 0; i < IM_ARRAYSIZE(weapon_occluded_type); i++)
					{
						if (ImGui::Selectable(weapon_occluded_type[i], i == g_Options.chams.weapon_occluded_type))
							g_Options.chams.weapon_occluded_type = i;
					}

					ImGui::EndCombo();
				}
				
				ImGui::Columns(1, nullptr, false);
				ImGui::PopStyleVar();
				}
			
			}
			else if (MenuTab == TAB::CHANGERS)
			{
				static char* changers_tab_names[] = { "Inventory##subtab", "Profile##subtab" };

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				{
					render_tabs(changers_tab_names, active_changers_tab, group_w / _countof(changers_tab_names), 40.f, true);
				}
				ImGui::PopStyleVar();

				ImGui::Spacing();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, group_w / 2.0f);
				ImGui::SetColumnOffset(2, group_w);

				if (active_changers_tab == 0)
				{
					if (weaponSkin.wId == WEAPON_NONE)
						weaponSkin.wId = WEAPON_DEAGLE;

					if (ImGui::BeginCombo("Weapon", k_inventory_names.at(weaponSkin.wId)))
					{
						for (const auto& weapon : k_inventory_names)
						{
							if (ImGui::Selectable(weapon.second, weaponSkin.wId == weapon.first))
							{
								weaponSkin.wId = weapon.first;
								weaponSkin.paintKit = 0;
								skinImage = nullptr;
							}
						}
						ImGui::EndCombo();
					}

					auto weaponName = weaponnames(weaponSkin.wId);

					if (ImGui::BeginCombo("Paint Kit", weaponSkin.paintKit > 0 ? g_Options.inventory.skinInfo[weaponSkin.paintKit].name.c_str() : ""))
					{
						int lastID = ImGui::GetItemID();

						for (auto skin : g_Options.inventory.skinInfo)
						{
							for (auto names : skin.second.weaponName)
							{
								if (weaponName != names)
									continue;

								ImGui::PushID(lastID++);

								if (ImGui::Selectable(skin.second.name.c_str(), skin.first == weaponSkin.paintKit))
									weaponSkin.paintKit = skin.first;

								ImGui::PopID();
							}							
						}
						ImGui::EndCombo();
					}

					ImGui::SliderFloat("Wear##new", &weaponSkin.wear, 0.f, 1.f, "%.5f");
					ImGui::InputInt("Seed##new", &weaponSkin.seed);

					if (ImGui::Button("Add##new"))
					{
						g_InventorySkins.insert({ RandomInt(20000, 200000), weaponSkin });
						g_Options.inventory.itemCount = g_InventorySkins.size();
					}
					ImGui::SameLine();
					if (ImGui::Button("Apply"))
					{
						Protobuf::SendClientHello();
						Protobuf::SendMatchmakingClient2GCHello();
					}

					ImGui::NextColumn();

					static int selectedId = 0;

					if(ImGui::ListBoxHeader("##skins", ImVec2(-1, 0)))
					{
						int lastID = ImGui::GetItemID();
						for (auto weapon : g_InventorySkins)
						{
							if (!weapon.second.wId || !weapon.second.paintKit)
								continue;

							ImGui::PushID(lastID++);
							
							if (ImGui::Selectable((k_inventory_names.at(weapon.second.wId) + std::string(" | ") + g_Options.inventory.skinInfo[weapon.second.paintKit].name).c_str(), selectedId == weapon.first))
								selectedId = weapon.first;
							ImGui::PopID();
						}

						ImGui::ListBoxFooter();
					}

					if(selectedId != 0)
					{
						ImGui::SliderFloat("Wear##existing", &g_InventorySkins[selectedId].wear, 0.f, 1.f, "%.5f");
						ImGui::InputInt("Seed##existing", &g_InventorySkins[selectedId].seed);

						if(ImGui::Button("Delete##existing", ImVec2(-1, 25)))
						{
							g_InventorySkins.erase(selectedId);
							g_Options.inventory.itemCount = g_InventorySkins.size();
						}
					}
				}
				else
				{
					const char* ranklist[] =
					{
						"OFF",
						"Silver I",
						"Silver II",
						"Silver III",
						"Silver IV",
						"Silver Elite",
						"Silver Elite Master",

						"Gold Nova I",
						"Gold Nova II",
						"Gold Nova III",
						"Gold Nova Master",
						"Master Guardian I",
						"Master Guardian II",

						"Master Guardian Elite",
						"Distinguished Master Guardian",
						"Legendary Eagle",
						"Legendary Eagle Master",
						"Supreme Master First Class",
						"Global Elite"

					};

					ImGui::Checkbox("Enabled", &g_Options.profile.enabled);

					ImGui::Combo("Rank", &g_Options.profile.rank_id, ranklist, IM_ARRAYSIZE(ranklist));
					ImGui::InputInt("Wins", &g_Options.profile.wins);
					ImGui::SliderInt("Level", &g_Options.profile.player_level, -1, 40);
					ImGui::SliderInt("XP", &g_Options.profile.player_xp, -1, 5000);
					ImGui::InputInt("Teaching", &g_Options.profile.teaching);
					ImGui::InputInt("Friendly", &g_Options.profile.friendly);
					ImGui::InputInt("Leader", &g_Options.profile.leader);
					if (ImGui::Button("Apply"))
					{
						Protobuf::SendClientHello();
						Protobuf::SendMatchmakingClient2GCHello();
					}
				}

				ImGui::PopStyleVar();
			}
			else if (MenuTab == TAB::MISC)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.FramePadding.x, style.ItemSpacing.y });
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, group_w / 2.0f);
				ImGui::SetColumnOffset(2, group_w);

				ImGui::PushItemWidth(-1);
				if (ImGui::BeginCombo("##ragehitbox_filter", "Removals", ImGuiComboFlags_NoArrowButton))
				{
					ImGui::Selectable("No hands", &g_Options.misc.removals.no_hands, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No sleeves", &g_Options.misc.removals.no_sleeves, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No weapon", &g_Options.misc.removals.no_weapon, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No smoke", &g_Options.misc.removals.no_smoke, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No scope overlay", &g_Options.misc.removals.no_scope, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No zoom", &g_Options.misc.removals.no_zoom, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No flash", &g_Options.misc.removals.no_flash, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No blur", &g_Options.misc.removals.no_blur, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No postprocess", &g_Options.misc.removals.no_postprocess, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No fog", &g_Options.misc.removals.no_fog, ImGuiSelectableFlags_DontClosePopups);
					ImGui::Selectable("No visual recoil", &g_Options.misc.removals.no_visual_recoil, ImGuiSelectableFlags_DontClosePopups);

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Bunny hop", &g_Options.misc.bhop);
				ImGui::Checkbox("Auto strafe", &g_Options.misc.autostrafe);
				ImGui::Checkbox("Rank reveal", &g_Options.misc.rank_reveal);
				ImGui::Checkbox("Watermark", &g_Options.misc.watermark);
				ImGui::Checkbox("Fake fps", &g_Options.misc.fakefps);
				ImGui::Checkbox("Auto accept", &g_Options.misc.autoaccept);
				ImGui::Checkbox("Night mode", &g_Options.misc.nightmode);
				ImGui::Checkbox("Sniper crosshair", &g_Options.misc.sniper_xhair);
				ImGui::Checkbox("Buy info", &g_Options.misc.buyinfo);
				ImGui::Checkbox("Spectator list", &g_Options.misc.spectator_list);

				ImGui::NextColumn();

				ImGui::PushItemWidth(-1);

				ImGui::Hotkey("Thirdperson switch", &g_Options.misc.thirdperson.hotkey);

				ImGui::Checkbox("Fakelags", &g_Options.misc.fakelag.enabled);
				ImGui::Checkbox("On key##fakelags", &g_Options.misc.fakelag.onkey);
				if (g_Options.misc.fakelag.onkey)
					ImGui::Hotkey("Key##fakelags", &g_Options.misc.fakelag.hotkey);
				ImGui::SliderInt("##ticks", &g_Options.misc.fakelag.ticks, 0, 16, "Ticks: %i");

				ImGui::Checkbox("Edge jump", &g_Options.misc.edgejump.enabled);
				ImGui::Checkbox("On key##edgejump", &g_Options.misc.edgejump.onkey);
				ImGui::Hotkey("Key##edgejump", &g_Options.misc.edgejump.hotkey);

				ImGui::Separator("Fov changers");

				ImGui::SliderInt("##World Fov", &g_Options.misc.fov.override, 0, 180, "World fov: %i");
				ImGui::SliderInt("##Viewmodel fov", &g_Options.misc.fov.viewmodel, 0, 180, "Viewmodel fov: %i");
				ImGui::SliderInt("##ViewModel Offset X", &g_Options.misc.fov.viewmodel_offset_x, -20, 20, "Viewmodel offset x: %i");
				ImGui::SliderInt("##ViewModel Offset Y", &g_Options.misc.fov.viewmodel_offset_y, -20, 20, "Viewmodel offset y: %i");
				ImGui::SliderInt("##ViewModel Offset Z", &g_Options.misc.fov.viewmodel_offset_z, -20, 20, "Viewmodel offset z: %i");

				ImGui::PopItemWidth();

				ImGui::Columns(1, nullptr, false);
				ImGui::PopStyleVar();
			}
			else if (MenuTab == TAB::CONFIG)
			{
				static int selected = 0;
				static char cfgName[64];

				std::vector<std::string> cfgList;
				ReadDirectory(g_Options.folder, cfgList);
				ImGui::PushItemWidth(300.f);
				if (!cfgList.empty())
				{
					if (ImGui::BeginCombo("Select Config", cfgList[selected].c_str()))
					{
						for (size_t i = 0; i < cfgList.size(); i++)
						{
							if (ImGui::Selectable(cfgList[i].c_str(), i == selected))
								selected = i;
						}
						ImGui::EndCombo();

					}
					if (ImGui::Button("Save Config"))
						g_Options.SaveSettings(cfgList[selected]);
					ImGui::SameLine();
					if (ImGui::Button("Load Config"))
						g_Options.LoadSettings(cfgList[selected]);
					ImGui::SameLine();
					if (ImGui::Button("Delete Config"))
					{
						g_Options.DeleteSettings(cfgList[selected]);
						selected = 0;
					}
					ImGui::Separator();
				}
				ImGui::InputText("##configname", cfgName, 64);
				ImGui::SameLine();
				if (ImGui::Button("Create Config"))
				{
					if (strlen(cfgName))
						g_Options.SaveSettings(cfgName + std::string(".ini"));
				}
				ImGui::PopItemWidth();
			}
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();

		if (MenuTab == TAB::CHANGERS && active_changers_tab == 0)
		{
			drawList->PushClipRectFullScreen();

			drawList->AddRectFilled(wndPos + ImVec2(_style.FramePadding.x + wndSize.x, 0), wndPos + ImVec2(wndSize.x + 250 + _style.FramePadding.x, 250), ImColor(_style.Colors[ImGuiCol_WindowBg]));
			
			static std::string weapon = "";
			static std::string kit = "";

			static auto paintKit = 0;
			if (paintKit != weaponSkin.paintKit && weaponSkin.paintKit != 0)
			{
				kit = g_Options.inventory.skinInfo[weaponSkin.paintKit].cdnName;
				weapon = weaponnames(weaponSkin.wId);
				link = Preview::FindUrl(kit, weapon);

				std::thread download(downloadImage);
				download.detach();
			}
			paintKit = weaponSkin.paintKit;

			if (skinImage && paintKit != 0)
				drawList->AddImage(skinImage, wndPos + ImVec2(_style.FramePadding.x + wndSize.x, 384 / 8), wndPos + ImVec2(wndSize.x + 250 + _style.FramePadding.x, 125 + 384 / 4));

			drawList->AddRect(wndPos + ImVec2(_style.FramePadding.x + wndSize.x, 0), wndPos + ImVec2(wndSize.x + 250 + _style.FramePadding.x, 250), ImColor(_style.Colors[ImGuiCol_ButtonHovered]));

			drawList->PopClipRect();
		}
	}
	ImGui::End();
}

void Menu::Toggle()
{
    _visible = !_visible;

	g_Input->m_mouse_initiated = !_visible;
	g_Input->m_mouse_active = !_visible;
	g_InputSystem->EnableInput(!_visible);

	g_InputSystem->ResetInputState();
}

void Menu::CreateStyle()
{
	_style.Alpha = 1.f;
	_style.WindowRounding = 0.f;
	_style.WindowPadding = { 0.f, 0.f };
	_style.WindowBorderSize = 1.f;
	_style.FramePadding = { 8.f, 8.f };
	_style.AntiAliasedFill = true;
	_style.AntiAliasedLines = true;

	_style.Colors[ImGuiCol_Column] = _style.Colors[ImGuiCol_Text];
	_style.Colors[ImGuiCol_CheckMark] = ImColor(25, 31, 125);
	_style.Colors[ImGuiCol_Header] = ImColor(20, 25, 100);
	_style.Colors[ImGuiCol_HeaderHovered] = ImColor(25, 31, 125);
	_style.Colors[ImGuiCol_HeaderActive] = ImColor(13, 17, 29);
	_style.Colors[ImGuiCol_FrameBg] = ImColor(9, 11, 20);
	_style.Colors[ImGuiCol_FrameBgHovered] = ImColor(13, 17, 29);
	_style.Colors[ImGuiCol_FrameBgActive] = ImColor(13, 17, 29);
	_style.Colors[ImGuiCol_Button] = ImColor(9, 11, 20);
	_style.Colors[ImGuiCol_ButtonHovered] = ImColor(13, 17, 29);
	_style.Colors[ImGuiCol_ButtonActive] = ImColor(25, 31, 125);
	_style.Colors[ImGuiCol_SliderGrab] = ImColor(13, 17, 29);
	_style.Colors[ImGuiCol_SliderGrabActive] = ImColor(25, 31, 125);
	_style.Colors[ImGuiCol_Border] = ImColor(25, 31, 125);
	_style.Colors[ImGuiCol_WindowBg] = ImColor(6, 8, 14, 220);

	/*_style.Colors[ImGuiCol_Text] = ImVec4(1.00, 1.00, 1.00, 1.00);
	_style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50, 0.50, 0.50, 1.00);
	_style.Colors[ImGuiCol_Border] = ImVec4(0.00, 0.545, 1.00, 1.00);
	_style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00, 0.00, 0.00, 0.00);
	_style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26, 0.59, 0.98, 0.40);
	_style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26, 0.59, 0.98, 0.67);
	_style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 0.51);
	_style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14, 0.14, 0.14, 1.00);
	_style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02, 0.02, 0.02, 0.53);
	_style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31, 0.31, 0.31, 1.00);
	_style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41, 0.41, 0.41, 1.00);
	_style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51, 0.51, 0.51, 1.00);
	_style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26, 0.59, 0.98, 1.00);
	_style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24, 0.52, 0.88, 1.00);
	_style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26, 0.59, 0.98, 1.00);
	_style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26, 0.59, 0.98, 1.00);
	_style.Colors[ImGuiCol_Separator] = _style.Colors[ImGuiCol_Border];
	_style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10, 0.40, 0.75, 0.78);
	_style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10, 0.40, 0.75, 1.00);
	_style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26, 0.59, 0.98, 0.25);
	_style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26, 0.59, 0.98, 0.67);
	_style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26, 0.59, 0.98, 0.95);
	_style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61, 0.61, 0.61, 1.00);
	_style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00, 0.43, 0.35, 1.00);
	_style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90, 0.70, 0.00, 1.00);
	_style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00, 0.60, 0.00, 1.00);
	_style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26, 0.59, 0.98, 0.35);
	_style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.000, 0.009, 0.120, 0.940);
	_style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20, 0.25, 0.30, 1.0);
	_style.Colors[ImGuiCol_Button] = ImVec4(0.260, 0.590, 0.980, 0.670);
	_style.Colors[ImGuiCol_Header] = ImVec4(0.260, 0.590, 0.980, 0.670);
	_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.260, 0.590, 0.980, 1.000);
	_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.000, 0.545, 1.000, 1.000);
	_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.060, 0.416, 0.980, 1.000);
	_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20, 0.25, 0.30, 1.0);
	_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.000, 0.009, 0.120, 0.940);
	_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.076, 0.143, 0.209, 1.000);*/
}
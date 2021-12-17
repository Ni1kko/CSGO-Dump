#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "config.hpp"
#include "features/visuals.hpp"
#include "ui.hpp"
#include "security.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using KeyBindPair_t = std::pair< int, const char* >;
std::vector< KeyBindPair_t > ButtonNames{
	{ 0, ("None") },
{ 1, ("Left Mouse") },
{ 2, ("Right Mouse") },
{ 3, ("Scroll Lock") },
{ 4, ("Middle Mouse") },
{ 5, ("X1 Mouse") },
{ 6, ("X2 Mouse") },
{ 8, ("Backspace") },
{ 9, ("Tab") },
{ 12, ("Num 5") },
{ 13, ("Enter") },
{ 16, ("Shift") },
{ 17, ("Ctrl") },
{ 18, ("Alt") },
{ 20, ("Caps Lock") },
{ 27, ("Esc") },
{ 32, ("Space") },
{ 33, ("Num 9") },
{ 34, ("Num 3") },
{ 35, ("Num 1") },
{ 36, ("Num 7") },
{ 37, ("Num 4") },
{ 38, ("Num 8") },
{ 39, ("Num 6") },
{ 40, ("Num 2") },
{ 44, ("Sys Req") },
{ 45, ("Num 0") },
{ 46, ("Num Del") },
{ 48, ("0") },
{ 49, ("1") },
{ 50, ("2") },
{ 51, ("3") },
{ 52, ("4") },
{ 53, ("5") },
{ 54, ("6") },
{ 55, ("7") },
{ 56, ("8") },
{ 57, ("9") },
{ 65, ("A") },
{ 66, ("B") },
{ 67, ("C") },
{ 68, ("D") },
{ 69, ("E") },
{ 70, ("F") },
{ 71, ("G") },
{ 72, ("H") },
{ 73, ("I") },
{ 74, ("J") },
{ 75, ("K") },
{ 76, ("L") },
{ 77, ("M") },
{ 78, ("N") },
{ 79, ("O") },
{ 80, ("P") },
{ 81, ("Q") },
{ 82, ("R") },
{ 83, ("S") },
{ 84, ("T") },
{ 85, ("U") },
{ 86, ("V") },
{ 87, ("W") },
{ 88, ("X") },
{ 89, ("Y") },
{ 90, ("Z") },
{ 96, ("Num 0") },
{ 97, ("Num 1") },
{ 98, ("Num 2") },
{ 99, ("Num 3") },
{ 100, ("Num 4") },
{ 101, ("Num 5") },
{ 102, ("Num 6") },
{ 103, ("Num 7") },
{ 104, ("Num 8") },
{ 105, ("Num 9") },
{ 106, ("Num *") },
{ 107, ("Num +") },
{ 109, ("Num -") },
{ 110, ("Num Del") },
{ 111, ("/") },
{ 112, ("F1") },
{ 113, ("F2") },
{ 114, ("F3") },
{ 115, ("F4") },
{ 116, ("F5") },
{ 117, ("F6") },
{ 118, ("F7") },
{ 119, ("F8") },
{ 120, ("F9") },
{ 121, ("F10") },
{ 122, ("F11") },
{ 123, ("F12") },
{ 144, ("Pause") },
{ 145, ("Scroll Lock") },
{ 161, ("Right Shift") },
{ 186, (";") },
{ 187, ("=") },
{ 188, (",") },
{ 189, ("-") },
{ 190, (".") },
{ 191, ("/") },
{ 192, ("`") },
{ 219, ("[") },
{ 220, ("\\") },
{ 221, ("]") },
{ 222, ("'") },
{ 226, ("\\") },
};

bool KeyBind(const char* name, int& keyBind) {
	auto result = std::find_if(ButtonNames.begin(), ButtonNames.end(), [&](const KeyBindPair_t& a) { return a.first == keyBind; });
	if (result == ButtonNames.end()) {
		result = ButtonNames.begin();
		keyBind = 0;
	}

	int key = result - ButtonNames.begin();
	auto comboRes = ImGui::Combo(
		name, &key, [](void* data, int32_t idx, const char** out_text) {
		*out_text = ButtonNames[idx].second;
		return true;
	},
		nullptr, ButtonNames.size());

	keyBind = ButtonNames[key].first;
	return comboRes;
}

#pragma region RenderTabs
template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h)
{
	bool values[N] = { false };
	values[activetab] = true;
	for (auto i = 0; i < N; ++i) {
		if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) activetab = i;
		if (i < N - 1) ImGui::SameLine();
	}
}
#pragma endregion
#pragma region Legit
static int weapon_index = 7;
static int weapon_vector_index = 0;
struct WeaponName_t {
	constexpr WeaponName_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};

std::vector< WeaponName_t> WeaponNames =
{
{ 7, "AK-47" },
{ 8, "AUG" },
{ 9, "AWP" },
{ 63, "CZ75 Auto" },
{ 1, "Desert Eagle" },
{ 2, "Dual Berettas" },
{ 10, "FAMAS" },
{ 3, "Five-SeveN" },
{ 11, "G3SG1" },
{ 13, "Galil AR" },
{ 4, "Glock-18" },
{ 14, "M249" },
{ 60, "M4A1-S" },
{ 16, "M4A4" },
{ 17, "MAC-10" },
{ 27, "MAG-7" },
{ 33, "MP7" },
{ WEAPON_MP5, "MP5" },
{ 34, "MP9" },
{ 28, "Negev" },
{ 35, "Nova" },
{ 32, "P2000" },
{ 36, "P250" },
{ 19, "P90" },
{ 26, "PP-Bizon" },
{ 64, "R8 Revolver" },
{ 29, "Sawed-Off" },
{ 38, "SCAR-20" },
{ 40, "SSG 08" },
{ 39, "SG 553" },
{ 30, "Tec-9" },
{ 24, "UMP-45" },
{ 61, "USP-S" },
{ 25, "XM1014" },
};

void RenderCurrentWeaponButton(float width) {
	ImGui::SameLine();
	if (!g_EngineClient->IsConnected() || !g_LocalPlayer || !g_LocalPlayer->IsAlive()) {
		ImGui::Button("Current", ImVec2(width, 0.0f));
		return;
	}

	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon || !weapon->IsWeapon()) {
		ImGui::Button("Current", ImVec2(width, 0.0f));
		return;
	}

	if (ImGui::Button("Current", ImVec2(width, 0.0f))) {
		int wpn_idx = weapon->m_Item().m_iItemDefinitionIndex();
		auto wpn_it = std::find_if(WeaponNames.begin(), WeaponNames.end(), [wpn_idx](const WeaponName_t& a) {
			return a.definition_index == wpn_idx;
		});
		if (wpn_it != WeaponNames.end()) {
			weapon_index = wpn_idx;
			weapon_vector_index = std::abs(std::distance(WeaponNames.begin(), wpn_it));
		}
	}
}

void RenderLegitTab() {
	ImGui::Columns(3, NULL, false);

	float item_width = ImGui::GetColumnWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x;
	ImGui::PushItemWidth(item_width);
	if (ImGui::Combo(
		"##weapon_aimbot", &weapon_vector_index,
		[](void* data, int32_t idx, const char** out_text) {
		auto vec = reinterpret_cast<std::vector< WeaponName_t >*>(data);
		*out_text = vec->at(idx).name;
		return true;
	}, (void*)(&WeaponNames), WeaponNames.size())) {
		weapon_index = WeaponNames[weapon_vector_index].definition_index;
	}

	ImGui::PopItemWidth();

	RenderCurrentWeaponButton(item_width - ImGui::GetStyle().ItemSpacing.x);

	auto settings = &g_Options.legitbot_items[weapon_index];
	ImGui::BeginChild("##aimbot.general", ImVec2(0, 0), true);
	{
		ImGui::Text("General");
		ImGui::Separator();
		ImGui::PushItemWidth(-1);
		ImGui::Checkbox("Enabled", &settings->enabled);
		ImGui::Checkbox("Friendly fire", &settings->deathmatch);

		if (weapon_index == WEAPON_P250 ||
			weapon_index == WEAPON_USP_SILENCER ||
			weapon_index == WEAPON_GLOCK ||
			weapon_index == WEAPON_FIVESEVEN ||
			weapon_index == WEAPON_TEC9 ||
			weapon_index == WEAPON_DEAGLE ||
			weapon_index == WEAPON_ELITE ||
			weapon_index == WEAPON_HKP2000) {
			ImGui::Checkbox("Autopistol", &settings->autopistol);
		}

		ImGui::Checkbox("Smoke check", &settings->smoke_check);
		ImGui::Checkbox("Flash check ", &settings->flash_check);
		ImGui::Checkbox("Jump check", &settings->jump_check);
		ImGui::Checkbox("Autowall", &settings->autowall);
		ImGui::Checkbox("Silent", &settings->silent);
		ImGui::Checkbox("Auto Stop", &settings->autostop);

		if (weapon_index == WEAPON_AWP || weapon_index == WEAPON_SSG08 ||
			weapon_index == WEAPON_AUG || weapon_index == WEAPON_SG556) {
			ImGui::Checkbox("Only In Zoom", &settings->only_in_zoom);
		}

		ImGui::Checkbox("On Key", &settings->on_key);

		if (settings->on_key) {
			ImGui::Text("Bind");
			ImGui::SameLine();
			KeyBind("##Key bind", settings->key);
		}

		ImGui::Checkbox("Auto Fire", &settings->autofire);
		if (settings->autofire) {
			ImGui::Text("Bind");
			ImGui::SameLine();
			KeyBind("##Auto Fire bind", settings->autofire_key);
		}

		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild("##aimbot.misc", ImVec2(0, 0), true);
	{
		ImGui::Text("Misc");
		ImGui::Separator();
		ImGui::PushItemWidth(-1);

		static char* priorities[] = {
			"Fov",
			"Health",
			"Damage",
			"Distance"
		};

		static char* aim_types[] = {
			"Hitbox",
			"Nearest"
		};

		static char* smooth_types[] = {
			"Static",
			"Dynamic"
		};

		static char* fov_types[] = {
			"Static",
			"Dynamic"
		};

		static char* hitbox_list[] = {
			"Head",
			"Neck",
			"Pelvis",
			"Stomach",
			"Lower chest",
			"Chest",
			"Upper chest",
		};

		ImGui::Text("Aim Type:");
		ImGui::Combo("##aimbot.aim_type", &settings->aim_type, aim_types, IM_ARRAYSIZE(aim_types));

		if (settings->aim_type == 0) {
			ImGui::Text("Hitbox:");
			ImGui::Combo("##aimbot.hitbox", &settings->hitbox, hitbox_list, IM_ARRAYSIZE(hitbox_list));
		}

		ImGui::Text("Priority:");
		ImGui::Combo("##aimbot.priority", &settings->priority, priorities, IM_ARRAYSIZE(priorities));
		ImGui::Text("Fov Type:");
		ImGui::Combo("##aimbot.fov_type", &settings->fov_type, fov_types, IM_ARRAYSIZE(fov_types));
		ImGui::Text("Smooth Type:");
		ImGui::Combo("##aimbot.smooth_type", &settings->smooth_type, smooth_types, IM_ARRAYSIZE(smooth_types));
		ImGui::SliderFloat("##aimbot.fov", &settings->fov, 0, 30, "Fov: %.2f");

		if (settings->silent) {
			ImGui::SliderFloat("##aimbot.silent_fov", &settings->silent_fov, 0, 20, "Silent Fov: %.2f");
		}

		ImGui::SliderFloat("##aimbot.smooth", &settings->smooth, 1, 20, "Smooth: %.2f");

		if (!settings->silent) {
			ImGui::SliderInt("##aimbot.shot_delay", &settings->shot_delay, 0, 100, "Shot Delay: %.0f");
		}

		ImGui::SliderInt("##aimbot.kill_delay", &settings->kill_delay, 0, 1000, "Kill Delay: %.0f");

		if (settings->autowall) {
			ImGui::SliderInt("##aimbot.min_damage", &settings->min_damage, 1, 100, "Min Damage: %.0f");
		}

		ImGui::SliderFloat("##aimbot_backtrack_time", &settings->backtrack_time, 0.f, 0.2f, "Backtrack: %.3f (seconds)");

		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild("##aimbot.rcs", ImVec2(0, 0), true);
	{
		ImGui::Text("RCS");
		ImGui::Separator();
		ImGui::PushItemWidth(-1);
		ImGui::Checkbox("Enabled##aimbot.rcs", &settings->rcs);
		ImGui::Text("RCS Type:");
		static char* rcs_types[] = {
			"Standalone",
			"Aim"
		};
		ImGui::Combo("##aimbot.rcs_type", &settings->rcs_type, rcs_types, IM_ARRAYSIZE(rcs_types));
		ImGui::Checkbox("RCS Custom Fov", &settings->rcs_fov_enabled);
		if (settings->rcs_fov_enabled) {
			ImGui::SliderFloat("##aimbot.rcs_fov", &settings->rcs_fov, 0, 20, "RCS Fov: %.2f");
		}
		ImGui::Checkbox("RCS Custom Smooth", &settings->rcs_smooth_enabled);
		if (settings->rcs_smooth_enabled) {
			ImGui::SliderFloat("##aimbot.rcs_smooth", &settings->rcs_smooth, 1, 15, "RCS Smooth: %.2f");
		}
		ImGui::SliderInt("##aimbot.rcs_x", &settings->rcs_x, 0, 100, "RCS X: %.0f");
		ImGui::SliderInt("##aimbot.rcs_y", &settings->rcs_y, 0, 100, "RCS Y: %.0f");
		ImGui::SliderInt("##aimbot.rcs_start", &settings->rcs_start, 1, 30, "RCS Start: %.0f");
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::Columns(1, NULL, false);
}
#pragma endregion
#pragma region Visuals
void RenderVisualsTab() {
	static int ESP_Tab = 0;

	auto& style = ImGui::GetStyle();
	float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	{
		render_tabs(Esp_tabs, ESP_Tab, group_w / _countof(Esp_tabs), 25.0f);
	}
	ImGui::PopStyleVar();
	ImGui::BeginGroupBox("##body_content");
	{
		if (ESP_Tab == 0) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
			{
				ImGui::Columns(3, nullptr, false);
				ImGui::Checkbox("Enabled", &g_Options.esp_enabled);
				ImGui::Checkbox("Enemy only", &g_Options.esp_enemies_only);
				ImGui::Checkbox("Skeleton", &g_Options.esp_player_skeleton);
				ImGui::Checkbox("Box", &g_Options.esp_player_boxes);
				if (g_Options.esp_player_boxes) ImGui::Combo("Box type", &g_Options.esp_player_boxes_type, Box_type, IM_ARRAYSIZE(Box_type), 2);
				ImGui::Checkbox("Name", &g_Options.esp_player_names);
				ImGui::Checkbox("Health", &g_Options.esp_player_health);
				ImGui::Checkbox("Armour", &g_Options.esp_player_armour);
				ImGui::Checkbox("Weapon", &g_Options.esp_player_weapons);
				ImGui::NextColumn();

				ImGui::Checkbox("Dropped Weapons", &g_Options.esp_dropped_weapons);
				ImGui::Checkbox("Defuse Kit", &g_Options.esp_defuse_kit);
				ImGui::Checkbox("Planted C4", &g_Options.esp_planted_c4);
				ImGui::Checkbox( "Item Esp", &g_Options.esp_items );
				ImGui::NextColumn();

				ImGui::PushItemWidth(100);
				ImGui::ColorEdit4("Allies Visible", g_Options.color_esp_ally_visible, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Enemies Visible", g_Options.color_esp_enemy_visible, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Allies Occluded", g_Options.color_esp_ally_occluded, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Enemies Occluded", g_Options.color_esp_enemy_occluded, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Dropped Weapons", g_Options.color_esp_weapons, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Defuse Kit", g_Options.color_esp_defuse, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Planted C4", g_Options.color_esp_c4, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Item Esp", g_Options.color_esp_item, ImGuiColorEditFlags_NoInputs);
				ImGui::PopItemWidth();
				ImGui::Columns(1, nullptr, false);
			}
			ImGui::PopStyleVar();
		}
		else if (ESP_Tab == 1) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
			{
				ImGui::Columns(2, nullptr, false);
				ImGui::Checkbox("Enabled", &g_Options.glow_enabled);
				ImGui::Checkbox("Enemy only", &g_Options.glow_enemies_only);
				ImGui::Checkbox("Players", &g_Options.glow_players);
				ImGui::Checkbox("Chickens", &g_Options.glow_chickens);
				ImGui::Checkbox("C4 Carrier", &g_Options.glow_c4_carrier);
				ImGui::Checkbox("Planted C4", &g_Options.glow_planted_c4);
				ImGui::Checkbox("Defuse Kits", &g_Options.glow_defuse_kits);
				ImGui::Checkbox("Weapons", &g_Options.glow_weapons);
				ImGui::NextColumn();

				ImGui::PushItemWidth(100);
				ImGui::ColorEdit4("Ally", g_Options.color_glow_ally, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Enemy", g_Options.color_glow_enemy, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Chickens", g_Options.color_glow_chickens, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("C4 Carrier", g_Options.color_glow_c4_carrier, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Planted C4", g_Options.color_glow_planted_c4, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Defuse Kits", g_Options.color_glow_defuse, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Weapons", g_Options.color_glow_weapons, ImGuiColorEditFlags_NoInputs);
				ImGui::PopItemWidth();
				ImGui::Columns(1, nullptr, false);
			}
			ImGui::PopStyleVar();
		}
		else if (ESP_Tab == 2) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
			{
				ImGui::Columns(2, nullptr, false);
				ImGui::Checkbox("Enabled", &g_Options.chams_player_enabled);
				ImGui::Checkbox("Enemy only", &g_Options.chams_player_enemies_only);
				ImGui::Checkbox("Wireframe", &g_Options.chams_player_wireframe);
				ImGui::Checkbox("Flat", &g_Options.chams_player_flat);
				ImGui::Checkbox("Ignore-Z", &g_Options.chams_player_ignorez);
				ImGui::Checkbox("Glass", &g_Options.chams_player_glass);
				ImGui::Checkbox("Disable model occulusion", &g_Options.chams_disable_occulusion);
				ImGui::Combo("Backtrack chams", &g_Options.chams_backtrack, std::vector<std::string>{ "Off", "Last tick", "All ticks" });
				ImGui::Checkbox("Backtrack flat chams", &g_Options.chams_backtrack_flat);
				ImGui::NextColumn();

				ImGui::PushItemWidth(100);
				ImGui::ColorEdit4("Ally (Visible)", g_Options.color_chams_player_ally_visible, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Ally (Occluded)", g_Options.color_chams_player_ally_occluded, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Enemy (Visible)", g_Options.color_chams_player_enemy_visible, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Enemy (Occluded)", g_Options.color_chams_player_enemy_occluded, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Bactrack color", g_Options.color_chams_backtrack, ImGuiColorEditFlags_NoInputs);
				ImGui::PopItemWidth();
				ImGui::Columns(1, nullptr, false);
			}
			ImGui::PopStyleVar();
		}
		else if (ESP_Tab == 3) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
			{
				ImGui::Columns(2, nullptr, false);

				ImGui::Checkbox("Hitmarker", &g_Options.esp_hitmarker);
				ImGui::Checkbox("Angle lines", &g_Options.esp_angle_lines);
				ImGui::Checkbox("Choke indicator", &g_Options.esp_choke_indicator);
				ImGui::Checkbox("Grenade prediction", &g_Options.esp_grenade_prediction);
				ImGui::Checkbox("Draw FOV", &g_Options.other_drawfov);
				ImGui::Checkbox("Draw backtrack dots", &g_Options.esp_backtrack);
				ImGui::Checkbox("No hands", &g_Options.other_no_hands);
				ImGui::Checkbox("No smoke", &g_Options.other_no_smoke);
				ImGui::Checkbox("No flash", &g_Options.other_no_flash);
				ImGui::Checkbox("Visualize sounds", &g_Options.esp_sounds);
				ImGui::SliderFloat("Duration", &g_Options.esp_sounds_time, 0.5f, 3.0f, "%.3f ms");
				ImGui::SliderFloat("Radius", &g_Options.esp_sounds_radius, 15.0f, 150.0f, "%.1f");
				ImGui::ColorEdit4("Sound color", g_Options.color_esp_sounds, ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
				ImGui::ColorEdit4("Grenade prediction", g_Options.color_grenade_prediction, ImGuiColorEditFlags_NoInputs);

				ImGui::NextColumn();

				ImGui::Checkbox("Nightmode", &g_Options.other_nightmode);
				if (g_Options.other_nightmode)
					ImGui::SliderFloat("Size", &g_Options.other_nightmode_size, 0.05f, 1.0f, "%.3f");


				ImGui::SliderFloat("Red", &g_Options.other_mat_ambient_light_r, 0, 1);
				ImGui::SliderFloat("Green", &g_Options.other_mat_ambient_light_g, 0, 1);
				ImGui::SliderFloat("Blue", &g_Options.other_mat_ambient_light_b, 0, 1);
				ImGui::Checkbox("Rainbow", &g_Options.other_mat_ambient_light_rainbow);



				ImGui::Columns(1, nullptr, false);
			}
			ImGui::PopStyleVar();
		}
	}
	ImGui::EndGroupBox();
}
#pragma endregion
#pragma region Misc
void RenderMiscTab() {
	auto& style = ImGui::GetStyle();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::Checkbox("Bunny hop", &g_Options.misc_bhop);
		ImGui::Checkbox("Auto strafe", &g_Options.misc_autostrafe);
		if (g_Options.misc_autostrafe)
			ImGui::SliderFloat("Retrack speed", &g_Options.misc_retrack_speed, 2.0f, 8.0f, "%.1f");

		ImGui::Checkbox("Edge jump", &g_Options.misc_edgejump);
		ImGui::Checkbox("Spectator list", &g_Options.misc_spectatorlist);
		ImGui::Checkbox("Radar", &g_Options.misc_radar);
		if (g_Options.misc_radar) {
			ImGui::SameLine();
			ImGui::Checkbox("Enemy only", &g_Options.misc_radar_enemyonly);
		}

		ImGui::Checkbox("Rank reveal", &g_Options.misc_showranks);
		ImGui::Checkbox("Clantag", &g_Options.misc_clantag);
		ImGui::Checkbox("Watermark", &g_Options.misc_watermark);
		ImGui::Checkbox("Third Person", &g_Options.misc_thirdperson);
		if (g_Options.misc_thirdperson)
		{
			ImGui::SliderFloat("Third Person Dist", &g_Options.misc_thirdperson_dist, 25.f, 250.f);
			KeyBind("Third Person Bind", g_Options.misc_thirdperson_bind); ImGui::NewLine();
		}

		ImGui::Combo("Desync [BETA]", &g_Options.misc_desync, std::vector<std::string>{ "Off", "Static", "Balance" });
		if (g_Options.misc_desync)
			KeyBind("Desync Flip bind", g_Options.misc_desync_bind);

		ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetTextLineHeightWithSpacing()));
		ImGui::Text("v0.46");

		ImGui::NextColumn();
		ImGui::PushItemWidth(158);
		{
			ImGui::SliderInt("Override FOV", &g_Options.misc_override_fov, 90, 150);
			ImGui::SliderInt("Viewmodel FOV", &g_Options.misc_viewmodel_fov, 68, 120);

			//KeyBind("Fake duck", g_Options.misc_fakeduck_bind);
			//ImGui::SliderInt("Fake duck ticks", &g_Options.misc_fakeduck_ticks, 2, 14);
			//ImGui::Checkbox("Block attack when fake ducked", &g_Options.misc_block_ducked_attack);

			ImGui::Checkbox("Fakelag Enabled", &g_Options.fakelag_enabled);
			if (g_Options.fakelag_enabled)
			{
				ImGui::Checkbox("When standing", &g_Options.fakelag_standing);
				ImGui::Checkbox("When moving", &g_Options.fakelag_moving);
				//ImGui::Checkbox("When Unducking", &g_Options.fakelag_unducking);
				ImGui::Combo("Mode", &g_Options.fakelag_mode, std::vector<std::string>{"Factor", "Switch", "Adaptive", "Random", "Legit peek"});
				ImGui::SliderInt("Factor", &g_Options.fakelag_factor, 1, 15);
			}

			KeyBind("Slowwalk bind", g_Options.misc_slowwalk_bind);

			if (g_Options.misc_slowwalk_bind)
				ImGui::SliderFloat("Slowwalk speed", &g_Options.misc_slowwalk_speed, 0.f, 100.f, "%.0f%%");
		}
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize("Unload").y - style.ItemSpacing.y * 2.f - 3));
		ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Unload").x - style.ItemSpacing.x * 2.f + 3, 0));
		ImGui::SameLine();

		if (ImGui::Button("Unload"))
			g_Unload = true;

		ImGui::Columns(1, nullptr, false);
	}
	ImGui::PopStyleVar();
}
#pragma endregion
#pragma region Skins
void RenderSkinsTab() {
	if (k_skins.size() == 0) {
		initialize_kits();
	}
	auto& entries = g_Options.skins.m_items;
	static auto definition_vector_index = 0;
	ImGui::Columns(2, nullptr, false);
	{
		ImGui::PushItemWidth(-1);
		const auto wnd = ImGui::GetCurrentWindowRead();
		float height = (wnd->Pos.y + wnd->Size.y) - wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;

		ImGui::ListBoxHeader("##config", ImVec2(0.f, height));
		{
			for (size_t w = 0; w < k_weapon_names.size(); w++) {
				if (ImGui::Selectable(k_weapon_names[w].name, definition_vector_index == w)) {
					definition_vector_index = w;
				}
			}
		}
		ImGui::ListBoxFooter();
		if (ImGui::Button("Update"))
			g_ClientState->ForceFullUpdate();
		ImGui::PopItemWidth();
	}
	ImGui::NextColumn();
	{
		auto& selected_entry = entries[k_weapon_names[definition_vector_index].definition_index];
		selected_entry.definition_index = k_weapon_names[definition_vector_index].definition_index;
		selected_entry.definition_vector_index = definition_vector_index;
		ImGui::Checkbox("Enabled", &selected_entry.enabled);
		ImGui::InputInt("Seed", &selected_entry.seed);
		ImGui::InputInt("StatTrak", &selected_entry.stat_trak);
		ImGui::SliderFloat("Wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
		if (selected_entry.definition_index != GLOVE_T_SIDE) {
			ImGui::Combo("Paint Kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = k_skins[idx].name.c_str();
				return true;
			}, nullptr, k_skins.size(), 20);
			selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
		}
		else {
			ImGui::Combo("Paint Kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = k_gloves[idx].name.c_str();
				return true;
			}, nullptr, k_gloves.size(), 20);
			selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
		}
		if (selected_entry.definition_index == WEAPON_KNIFE) {
			ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = k_knife_names.at(idx).name;
				return true;
			}, nullptr, k_knife_names.size(), 10);
			selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
		}
		else if (selected_entry.definition_index == GLOVE_T_SIDE) {
			ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = k_glove_names.at(idx).name;
				return true;
			}, nullptr, k_glove_names.size(), 10);
			selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
		}
		else {
			static auto unused_value = 0;
			selected_entry.definition_override_vector_index = 0;
		}
		//ImGui::InputText( "Name Tag", selected_entry.custom_name, 32 );
	}
	ImGui::Columns(1, nullptr, false);
}
#pragma endregion
#pragma region Options
void RenderOptionsTab() {
	static std::vector<std::string> configs;

	static auto load_configs = []() {
		std::vector<std::string> items = {};

		std::string path("C:\\Amnesia");
		if (!fs::is_directory(path))
			fs::create_directories(path);

		for (auto& p : fs::directory_iterator(path))
			items.push_back(p.path().string().substr(path.length() + 1));

		return items;
	};

	static auto is_configs_loaded = false;
	if (!is_configs_loaded) {
		is_configs_loaded = true;
		configs = load_configs();
	}

	static std::string current_config;

	static char config_name[32];

	ImGui::InputText("##config_name", config_name, sizeof(config_name));
	ImGui::SameLine();
	if (ImGui::Button("Create")) {
		current_config = std::string(config_name);

		Config->Save(current_config + ".ini");
		is_configs_loaded = false;
		memset(config_name, 0, 32);
	}



	ImGui::ListBoxHeader("##configs");
	{
		for (auto& config : configs) {
			if (ImGui::Selectable(config.c_str(), config == current_config)) {
				current_config = config;
			}
		}
	}
	ImGui::ListBoxFooter();


	if (!current_config.empty()) {

		if (ImGui::Button("Load"))
			Config->Load(current_config);
		ImGui::SameLine();

		if (ImGui::Button("Save"))
			Config->Save(current_config);
		ImGui::SameLine();

		if (ImGui::Button("Delete") && fs::remove("C:\\Amnesia\\" + current_config)) {
			current_config.clear();
			is_configs_loaded = false;
		}

		ImGui::SameLine();
	}

	if (ImGui::Button("Refresh"))
		is_configs_loaded = false;
}
#pragma endregion
#pragma region Radar
void Menu::DrawRadar() {
	auto visible = Color::Red;
	auto Tvisible = Color::White;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldPadding = style.WindowPadding;
	float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
	style.WindowPadding = ImVec2(0, 0);

	//style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	//style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Radar", nullptr, ImVec2(150, 170), 0.7F, ImGuiWindowFlags_NoCollapse))
		//if (ImGui::Begin("Radar", &_visible, ImVec2(200, 200)), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)
	{
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
		windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), ImColor(64, 72, 95), 1.5f);
		windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), ImColor(64, 72, 95), 1.5f);
		static auto GetU32 = [](Color _color) {
			return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
				+ (_color[0] & 0xff);
		};

		static auto RotatePoint = [](Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom) {
			float r_1, r_2;
			float x_1, y_1;

			r_1 = -(EntityPos.y - LocalPlayerPos.y);
			r_2 = EntityPos.x - LocalPlayerPos.x;
			float Yaw = angle - 90.0f;

			float yawToRadian = DEG2RAD(Yaw);
			x_1 = r_2 * cosf(yawToRadian) - r_1 * sin(yawToRadian) / 20.0f;
			y_1 = r_2 * sinf(yawToRadian) + r_1 * cos(yawToRadian) / 20.0f;

			x_1 *= zoom;
			y_1 *= zoom;

			int sizX = sizeX / 2;
			int sizY = sizeY / 2;

			x_1 += sizX;
			y_1 += sizY;

			if (x_1 < 5)
				x_1 = 5;

			if (x_1 > sizeX - 5)
				x_1 = sizeX - 5;

			if (y_1 < 5)
				y_1 = 5;

			if (y_1 > sizeY - 5)
				y_1 = sizeY - 5;

			x_1 += posX;
			y_1 += posY;

			return Vector2D(x_1, y_1);
		};

		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
			if (g_LocalPlayer) {
				Vector LocalPos = g_LocalPlayer->GetEyePos();
				QAngle ang;
				g_EngineClient->GetViewAngles(&ang);

				for (int i = 1; i <= g_GlobalVars->maxClients; ++i) {
					auto player = C_BasePlayer::GetPlayerByIndex(i);
					if (!player || player->IsDormant() || !player->IsAlive())
						continue;

					bool bIsEnemy = g_LocalPlayer->m_iTeamNum() != player->m_iTeamNum();

					if (g_Options.misc_radar_enemyonly && !bIsEnemy)
						continue;

					bool viewCheck = false;
					Vector2D EntityPos = RotatePoint(Vector(player->m_vecOrigin().x, player->m_vecOrigin().y, 0), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.yaw, 0.5f);

					//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
					ImU32 clr = GetU32(bIsEnemy ? (visible) : Tvisible);

					int s = 4;
					windowDrawList->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, clr);
				}
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	style.WindowPadding = oldPadding;
	style.Colors[ImGuiCol_WindowBg].w = oldAlpha;
}
#pragma endregion

void Menu::Initialize() {
	CreateStyle();

	_visible = false;
}

void Menu::Shutdown() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset() {
	ImGui_ImplDX9_CreateDeviceObjects();
}

bool FileExists(const TCHAR *fileName)
{
	DWORD fileAttr;
	fileAttr = GetFileAttributes(fileName);
	if (0xFFFFFFFF == fileAttr && GetLastError() == ERROR_FILE_NOT_FOUND)
		return false;
	return true;
}
void RenderEmptyTab()
{
	
		ImGui::Text("Is empty <3");
		//ImGui::Text("%.2f x, %.2f y", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	
}

void Menu::Render() {
	ImGui::GetIO().MouseDrawCursor = _visible;

	if (g_Options.misc_spectatorlist)
		Visuals::Get().RenderSpectatorList();

	if (g_Options.misc_radar)
		DrawRadar();

	if (!_visible)
		return;

	static int selected_tab = 0;

	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	float x = w * 0.5f - 300.0f;
	float y = h * 0.5f - 200.0f;

	ImGui::SetNextWindowPos(ImVec2{ x, y }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 600, 400 }, ImGuiSetCond_Once);

#ifdef _DEBUG
	ImGui::ShowDemoWindow();
#endif

	if (ImGui::Begin("Amnesia", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		{
			ImGui::Image(brand_img, ImVec2(142.0f, 40.0f)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(_style.WindowPadding.x, 0.0f)); ImGui::SameLine();
			auto TabsW = (ImGui::GetCurrentWindow()->Size.x - _style.WindowPadding.x * 2.0f - 150.0f) / _countof(tabs);
			render_tabs(tabs, selected_tab, TabsW, 40.0f);
		}
		ImGui::PopStyleVar();

		ImGui::BeginGroupBox("##empty");
		{
			if (selected_tab == 0)		RenderLegitTab();
			else if (selected_tab == 1)	RenderVisualsTab();
			else if (selected_tab == 2)	RenderMiscTab();
			else if (selected_tab == 3)	RenderSkinsTab();
			else if (selected_tab == 4)	RenderOptionsTab();
		}
		ImGui::EndGroupBox();

		ImGui::End();
	}
}

void Menu::Toggle() {
	_visible = !_visible;
}

void Menu::CreateStyle() {
	ImGui::StyleColorsAmnesia(); // why nut? xd

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX); // this for print hex, without - rgba
	ImGui::GetStyle() = _style;
}


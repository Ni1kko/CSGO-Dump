#pragma once

#include <string>
#include <map>
#include "valve_sdk/Misc/Color.hpp"
#define FLOAT_TO_CLR(clr) clr[0] * 255.0f, clr[1] * 255.0f, clr[2] * 255.0f, clr[3] * 255.0f

extern std::map<int, const char*> weapon_names;

#include "kit_parser.hpp"
#include "item_definitions.hpp"
struct item_setting
{
	char name[32] = "Default";
	bool enabled = false;
	int definition_vector_index = 0;
	int definition_index = 1;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	float wear = 0.0f;
	//char custom_name[32] = "";
};

struct legitbot_settings {
	bool enabled = false;
	bool deathmatch = false;
	bool autopistol = false;
	bool smoke_check = false;
	bool flash_check = false;
	bool jump_check = false;
	bool autowall = false;
	bool silent = false;
	bool autofire = false;
	bool on_key = true;
	bool rcs = false;
	bool rcs_fov_enabled = false;
	bool rcs_smooth_enabled = false;
	bool autostop = false;
	float backtrack_time = 0.0f;
	bool only_in_zoom = true;
	int autofire_key = 1;
	int key = 1;
	int aim_type = 1;
	int priority = 0;
	int fov_type = 0;
	int smooth_type = 0;
	int rcs_type = 0;
	int hitbox = 1;
	float fov = 0.f;
	float silent_fov = 0.f;
	float rcs_fov = 0.f;
	float smooth = 1;
	float rcs_smooth = 1;
	int shot_delay = 0;
	int kill_delay = 0;
	int rcs_x = 100;
	int rcs_y = 100;
	int rcs_start = 1;
	int min_damage = 1;
};

#define OPTION(type, var, val) type var = val

class Options
{
public:

	// AIMBOT

	std::map<int, legitbot_settings> legitbot_items = { };

	// ESP

	OPTION(bool, esp_enabled, false);
	OPTION(bool, esp_enemies_only, false);
	OPTION(bool, esp_player_skeleton, false);
	OPTION(bool, esp_player_boxes, false);
	OPTION(int, esp_player_boxes_type, 0);
	OPTION(bool, esp_player_names, false);
	OPTION(bool, esp_player_health, false);
	OPTION(bool, esp_player_armour, false);
	OPTION(bool, esp_player_weapons, false);
	OPTION(bool, esp_dropped_weapons, false);
	OPTION(bool, esp_defuse_kit, false);
	OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_items, false);
	OPTION(bool, esp_grenade_prediction, false);
	OPTION(bool, esp_backtrack, false);
	OPTION(bool, esp_sounds, false);
	OPTION(float, esp_sounds_time, 1.0f);
	OPTION(float, esp_sounds_radius, 1.0f);
	OPTION(bool, esp_angle_lines, false);
	OPTION(bool, esp_choke_indicator, false);
	OPTION(bool, esp_hitmarker, false);

	// GLOW

	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_enemies_only, false);
	OPTION(bool, glow_players, false);
	OPTION(bool, glow_chickens, false);
	OPTION(bool, glow_c4_carrier, false);
	OPTION(bool, glow_planted_c4, false);
	OPTION(bool, glow_defuse_kits, false);
	OPTION(bool, glow_weapons, false);

	// CHAMS

	OPTION(bool, chams_player_enabled, false);
	OPTION(bool, chams_player_enemies_only, false);
	OPTION(bool, chams_player_wireframe, false);
	OPTION(bool, chams_player_flat, false);
	OPTION(bool, chams_player_ignorez, false);
	OPTION(bool, chams_player_glass, false);
	OPTION(bool, chams_disable_occulusion, false);
	OPTION(int, chams_backtrack, false);
	OPTION(bool, chams_backtrack_flat, false);
	OPTION(bool, chams_desync, false);

	// OTHER

	OPTION(bool, other_nightmode, false);
	OPTION(float, other_nightmode_size, 0.05f);
	OPTION(float, other_mat_ambient_light_r, 0.0f);
	OPTION(float, other_mat_ambient_light_g, 0.0f);
	OPTION(float, other_mat_ambient_light_b, 0.0f);
	OPTION(bool, other_mat_ambient_light_rainbow, false);
	OPTION(bool, other_drawfov, false);
	OPTION(bool, other_no_hands, false);
	OPTION(bool, other_no_smoke, false);
	OPTION(bool, other_no_flash, false);

	// SKINCHANGER

	struct {
		std::map<int, item_setting> m_items;
		std::unordered_map<std::string, std::string> m_icon_overrides;
		auto get_icon_override(const std::string original) const -> const char*
		{
			return m_icon_overrides.count(original) ? m_icon_overrides.at(original).data() : nullptr;
		}
	} skins;

	// MISC

	OPTION(bool, misc_bhop, false);
	OPTION(bool, misc_autostrafe, false);
	OPTION(bool, misc_edgejump, false);
	OPTION(bool, misc_spectatorlist, false);
	OPTION(bool, misc_radar, false);
	OPTION(bool, misc_radar_enemyonly, false);
	OPTION(bool, misc_thirdperson, false);
	OPTION(int, misc_thirdperson_bind, 0);
	OPTION(bool, misc_showranks, true);
	OPTION(bool, misc_clantag, false);
	OPTION(bool, misc_watermark, false);
	OPTION(int, misc_desync, 0);
	OPTION(int, misc_desync_bind, 0);
	//OPTION(int, misc_fakeduck_bind, 0);
	//OPTION(int, misc_fakeduck_ticks, 2);
	//OPTION(bool, misc_block_ducked_attack, false);
	OPTION(float, misc_thirdperson_dist, 50.f);
	OPTION(int, misc_override_fov, 90);
	OPTION(int, misc_viewmodel_fov, 68);
	OPTION(float, misc_retrack_speed, 2.0f);
	OPTION(int, misc_slowwalk_bind, 0);
	OPTION(float, misc_slowwalk_speed, 15.0f);
	OPTION(bool, fakelag_enabled, false);
	OPTION(bool, fakelag_standing, false);
	OPTION(bool, fakelag_moving, false);
	OPTION(bool, fakelag_unducking, false);
	OPTION(int, fakelag_mode, 0);
	OPTION(int, fakelag_factor, 0);

	// COLORS

	float color_esp_ally_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_enemy_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_ally_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_enemy_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_weapons[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_defuse[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_c4[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_item[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_grenade_prediction[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_sounds[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	float color_glow_ally[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_enemy[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_chickens[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_c4_carrier[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_planted_c4[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_defuse[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_glow_weapons[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	float color_chams_player_ally_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_player_ally_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_player_enemy_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_player_enemy_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_backtrack[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_desync[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

static char* tabs[] = { "Legit", "Visual", "Misc", "Skin", "Config" };
static char* Esp_tabs[] = { "ESP", "Glow", "Chams", "Other" };
static char* Box_type[] = { "Normal", "Edge" };
//static char* Languages[] = { "Normal", "Edge" };
extern Options g_Options;
extern bool   g_Unload;
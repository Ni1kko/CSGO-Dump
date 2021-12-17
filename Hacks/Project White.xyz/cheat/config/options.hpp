#pragma once

#include <string>
#include <set>
#include <map>
#include "../../sdk/Misc/Color.hpp"
#include "../skins/kit_parser.hpp"
#include "../skins/item_definitions.hpp"

extern std::map<int, const char*> weapon_names;

struct item_setting {
	char name[32] = "Default";
	int stickers_place = 0;
	int definition_vector_index = 0;
	int definition_index = 0;
	bool enabled_stickers = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	bool stat_trak = 0;
	float wear = FLT_MIN;
	char custom_name[32] = "";
};

struct statrack_setting {
	int definition_index = 1;
	struct {
		int counter = 0;
	}statrack_new;
};

struct legitbot_settings {
	bool enabled = false;
	bool legitresolver = false;
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

#include <set>
#include <unordered_map>
struct skinInfo {
	int seed = -1;
	int paintkit;
	std::string tagName;
};

#define OPTION(type, var, val) type var = val

class Options {
public:
	// AIMBOT
	std::map<int, legitbot_settings> legitbot_items = { };

	// ESP
	OPTION(bool, esp_enabled, false);
	OPTION(bool, esp_enemies_only, false);
	OPTION(bool, esp_visible_only, false);
	OPTION(bool, esp_player_skeleton, false);
	OPTION(bool, esp_player_boxes, false);
	OPTION(int,  esp_player_boxes_type, 0);
	OPTION(int,  boxes_rounding, 5);
	OPTION(bool, esp_player_names, false);
	OPTION(bool, esp_player_health, false);
	OPTION(bool, esp_player_armour, false);
	OPTION(bool, esp_player_weapons, false);
	OPTION(bool, esp_dropped_weapons, false);
	OPTION(bool, esp_defuse_kit, false);
	OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_c4, false);
	OPTION(bool, esp_items, false);
	OPTION(int,  esp_drop_distance, 3500);
	OPTION(bool, esp_grenade_prediction, false);
	OPTION(bool, esp_angle_lines, false);
	OPTION(bool, esp_hitmarker, false);
	OPTION(bool, esp_dangerzone, false);

	OPTION(bool, esp_flags, false);
	bool esp_player_flags_selected[7];

	OPTION(bool, esp_crosshair, false);
	OPTION(bool, esp_snipercrosshair, false);
	OPTION(bool, esp_recoilcrosshair, false);

	OPTION(bool,  esp_sounds, false);
	OPTION(float, esp_sounds_time, 0.5f);
	OPTION(float, esp_sounds_radius, 15.0f);

	OPTION(bool, aspect_ratio, false);
	OPTION(int,  aspect_ratio_value, 0);

	// GLOW
	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_enemies_only, false);
	OPTION(bool, glow_visible_only, false);
	OPTION(bool, glow_players, false);
	OPTION(bool, glow_rainbow, false);
	OPTION(bool, glow_chickens, false);
	OPTION(bool, glow_c4_carrier, false);
	OPTION(bool, glow_planted_c4, false);
	OPTION(bool, glow_defuse_kits, false);
	OPTION(bool, glow_weapons, false);
	OPTION(bool, glow_pulsing, false);
	OPTION(int,  glow_style, 0);

	// CHAMS
	OPTION(bool, chams_player_enabled, false);
	OPTION(int,  chams_material, 0);
	OPTION(bool, chams_player_enemies_only, false);
	OPTION(bool, chams_player_wireframe, false);
	OPTION(bool, chams_player_ignorez, false);

	OPTION(bool, chams_sleeve_enabled, false);	
	OPTION(int,  chams_material_sleeve, 0);
	OPTION(bool, chams_sleeve_rainbow, false);

	OPTION(bool, chams_weapon_enabled, false);
	OPTION(int,  chams_material_weapon, 0);
	OPTION(bool, chams_weapon_rainbow, false);

	OPTION(bool, chams_arms_enabled, false);
	OPTION(int,  chams_material_arms, 0);
	OPTION(bool, chams_arms_rainbow, false);

	OPTION(int,  chams_backtrack, false);
	OPTION(int,  chams_material_backtrack, 0);
	OPTION(bool, chams_backtrack_wireframe, false);
	OPTION(bool, chams_backtrack_rainbow, false);

//	OPTION(bool, chams_desync, false);

	// OTHER
	OPTION(bool, misc_nightmode, false);
	OPTION(bool, other_drawfov, false);
	OPTION(bool, other_no_hands, false);
	OPTION(bool, other_no_smoke, false);
	OPTION(bool, other_no_flash, false);
	OPTION(bool, backdrop, false);
	bool fps_boost_selected[4];
	float color_menu[4]{ 87 / 255.f, 48 / 255.f , 161 / 255.f, 1.0f };

	// SKINCHANGER
	struct {
		bool show_cur = true;

		std::map<int, item_setting> m_items;
		std::unordered_map<std::string, std::string> m_icon_overrides;
		std::map<int, statrack_setting> statrack_items = { };
		auto get_icon_override(const std::string original) const -> const char* {
			return m_icon_overrides.count(original) ? m_icon_overrides.at(original).data() : nullptr;
		}
	} skins;

	// MISC
	OPTION(bool,  misc_bhop, false);
	OPTION(float, misc_bhop_speed, 2.0f);
	OPTION(bool,  misc_autostrafe, false);
	OPTION(bool,  misc_edgejump, false);
	OPTION(int,   misc_edgejump_key, false);
	OPTION(bool,  misc_spectatorlist, false);
	OPTION(bool,  misc_radar, false);
	OPTION(bool,  misc_engine_radar, false);
	OPTION(bool,  misc_thirdperson, false);
	OPTION(int,   misc_thirdperson_bind, 0);
	OPTION(bool,  misc_chat_spam, false);
	OPTION(bool,  misc_radio_spam, false);
	OPTION(bool,  misc_name_spam, false);
	OPTION(bool,  misc_showranks, true);
	OPTION(bool, legitresolver, false);
	OPTION(bool,  misc_autoaccept, false);
	OPTION(bool,  misc_clantag, false);
	OPTION(bool,  misc_watermark, false);
	OPTION(bool,  misc_spamrandom, false);
	OPTION(int,   misc_desync, 0);
	OPTION(int,   misc_desync_bind, 0);
	OPTION(float, misc_thirdperson_dist, 50.f);
	OPTION(int,   misc_override_fov, 90);
	OPTION(int,   misc_viewmodel_fov, 68);
	OPTION(bool,  misc_leftknife, false);

	OPTION(bool, misc_model, false);
	OPTION(int,  misc_model_type_ct, 0);
	OPTION(int,  misc_model_type_tt, 0);

	// COLORS
	float color_esp_ally_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_enemy_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_ally_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_enemy_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_weapons[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_hitmarker[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_defuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_c4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_item[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_grenade_prediction[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_sounds[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_player_skeleton[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_backtrack[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_crosshair[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float color_glow_ally[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_ally_invis[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_enemy[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_enemy_invis[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_chickens[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_c4_carrier[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_planted_c4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_defuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_glow_weapons[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float color_chams_player_ally_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_player_ally_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_player_enemy_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_player_enemy_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float color_chams_arms[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_sleeve[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_weapon[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_backtrack[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_chams_desync[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

static int active_tab = 0;
static int visualtab = 0;
static int misctab = 0;

static std::string tabs[] = { "LegitBot", "Visual", "Skins", "Misc" };
static std::string tabs_esp[] = { "Esp", "Glow", "Chams", "Color" };
static std::string tabs_misc[] = { "Misc", "Config" };

static char* priorities[] = { "Fov", "Health", "Damage", "Distance" };
static char* aim_types[] = { "Hitbox", "Nearest" };
static char* hitbox_list[] = { "Head", "Neck", "Pelvis", "Stomach", "Lower chest", "Chest", "Upper chest" };
static char* rcs_types[] = { "Standalone", "When Aiming" };
static char* Box_type[] = { "Normal", "Rounded", "Edge" };
static char* dpi_scale[] = { "normal", "150%", "200%" };
static char* desync[] = { "Off", "Static", "Balance"};
static char* chams_tick[] = { "Disable", "Last", "All" };
static char* chams_material[] = { "Regular", "Flat", "Glow" };
static char* glow_style[] = { "Normal", "Skinny", "Pulsating" };
static char* esp_flags_vec[] = { "Money", "Flashed", "Armour", "Scoped", "Defusing", "Has C4", "Has defuser" };
static char* fps_boost_vec[] = { "Disable Panorama Blur", "No 3d Sky", "No Shadows", "Disable PostProcessing" };
static char* agent_types[] = { "Default", "Special Agent Ava","Operator", "Markus Delrow", "Michael Syfers",
						"B Squadron Officer","Seal Team 6 Soldier","Buckshot", "Lt.Commander Ricksaw",
						"Third Commando Company", "'Two Times' McCoy", "Dragomir", "Rezan The Ready", "'The Doctor' Romanov",
						"Maximus", "Blackwolf", "The Elite Mr.Muhlik", "Ground Rebel", "Osiris", "Prof.Shahmat", "Enforcer ",
						"Slingshot"," Soldier" };

extern Options Cfg;
extern bool   g_Unload;
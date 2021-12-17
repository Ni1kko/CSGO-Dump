#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include "valve_sdk/Misc/Color.hpp"
#include "addons/keybinds/keybinds.hpp"

struct weapon_name_t {
	constexpr weapon_name_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};
struct weapon_info_t {
	constexpr weapon_info_t(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};
struct legitbot_settings {
	bool autopistol = false;
	bool smoke_check = false;
	bool flash_check = false;
	bool jump_check = false;
	bool autowall = false;
	float autowall_int = 0.f;
	bool silent = false;
	bool psilent = false;
	bool resolver = false;
	bool rcs = false;
	bool rcs_fov_enabled = false;
	bool rcs_smooth_enabled = false;
	bool autostop = false;
	bool only_in_zoom = true;
	int aim_type = 1;
	int priority = 0;
	int fov_type = 0;
	int smooth_type = 0;
	float curve = 0.f;
	bool humanize = false;
	int rcs_type = 0;
	int hitbox = 1;
	float fov = 0.f;

	bool trigger_enable = false;
	bool trigger_check_smoke = false;
	bool trigger_check_flash = false;
	float trigger_hit_chance = 0.f;
	float trigger_delay = 0.f;
	bool trigger_hitgroup_head = false;
	bool trigger_hitgroup_chest = false;
	bool trigger_hitgroup_stomach = false;
	bool trigger_hitgroup_left_arm = false;
	bool trigger_hitgroup_right_arm = false;
	bool trigger_hitgroup_left_leg = false;
	bool trigger_hitgroup_right_leg = false;

	bool enable_backtrack = false;
	int  backtrack_ticks = 0;

	bool hitchance = false;
	float hitchance_amount = 0.f;

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

namespace values {
	extern const char* box_types[];
	extern const char* bind_types[];
	extern const char* menu_styles[];
	extern const char* aim_types[];
	extern const char* smooth_types[];
	extern const char* hitbox_list[];
	extern const char* rcs_types[];
	extern const char* priorities[];
	extern const char* fov_types[];
	extern const char* show_spread_types[];
	extern const char* knifebot_filters[];
	extern const char* knifebot_modes[];
	extern const char* glow_modes[];
	extern const char* legitaa_modes[];
	extern const char* resolver_modes[];
	extern const char* soundesp_modes[];

	extern const std::map<size_t, weapon_info_t> WeaponInfo;
	extern const std::vector<weapon_name_t> KnifeNames;
	extern const std::vector<weapon_name_t> GloveNames;
	extern std::vector<weapon_name_t> WeaponNames;
	extern std::vector<weapon_name_t> WeaponNamesFull;
	extern std::map<short, std::string> WNames;


	extern const char* GetWeaponNameById(int id);
	extern int GetKnifeDefinitionIndex(int si);
	extern int GetGloveDefinitionIndex(int si);
};

extern bool g_Unload;

extern bool g_ChangeMenuSize;
extern ImVec2 g_MenuSize;
extern ImVec2 g_ChangeMenuSizeValue;

extern bool g_ShowPrompt;
extern std::string g_PromptTitle;
extern std::function<void()> g_PromptCallback;
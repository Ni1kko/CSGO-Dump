#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include "valve_sdk/Misc/Color.hpp"

template <typename T>
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

struct legitbot_s
{
	bool enabled = false;
	bool deathmatch = false;
	bool silent = false;
	bool flash_check = false;
	bool smoke_check = false;
	bool autopistol = false;
	
	float fov = 0.f;
	float silent_fov = 0.f;
	float smooth = 1.f;

	int shot_delay = 0;
	int kill_delay = 0;

	struct
	{
		bool head = true;
		bool chest = true;
		bool hands = true;
		bool legs = true;
	} hitboxes;

	struct
	{
		bool enabled = false;
		int start = 1;
		int type = 0;
		int x = 100;
		int y = 100;
	} rcs;

	struct
	{
		bool enabled = false;
		int min_damage = 0;
	} autowall;

	struct
	{
		bool enabled = false;
		int hotkey = 0;
	} autofire;
};

struct skins_s
{
	struct sides
	{
		bool enabled = false;
		int definitionIndex = 0;
		int paintKit = 0;
		int seed = 0;
		int stattrak = -1;
		int quality = 4;
		float wear = FLT_MIN;
	};

	sides t;
	sides ct;
};

struct weapons
{
	legitbot_s legit;
	skins_s skins;
};

struct skinInfo
{
	std::string name;
	std::string cdnName;
	std::vector<std::string> weaponName;
	int rarity;
};

class Options
{
public:
	
	std::map<short, weapons> weapons;

	struct
	{
		bool player_enemies = false;
		bool player_teammates = false;
		bool player_local = false;
		bool player_enemies_occluded = false;
		bool player_teammates_occluded = false;
		bool player_local_occluded = false;

		bool player_boxes_enemies = false;
		bool player_boxes_teammates = false;
		bool player_boxes_local = false;

		bool player_names_enemies = false;
		bool player_names_teammates = false;
		bool player_names_local = false;

		bool player_health_enemies = false;
		bool player_health_teammates = false;
		bool player_health_local = false;

		bool player_armour_enemies = false;
		bool player_armour_teammates = false;
		bool player_armour_local = false;

		bool player_weapons_enemies = false;
		bool player_weapons_teammates = false;
		bool player_weapons_local = false;

		bool dropped_weapons = false;
		bool defuse_kit = false;
		bool planted_c4 = false;
		bool items = false;

		bool sound_enemies = false;
		bool sound_teammates = false;
		bool sound_local = false;
	} esp;

	struct
	{
		bool enemies = false;
		int enemies_type;

		bool teammates = false;
		int teammates_type;

		bool local = false;
		int local_type;

		bool chickens = false;
		int chickens_type;

		bool planted_c4 = false;
		int planted_c4_type;

		bool weapons = false;
		int weapons_type;
	} glow;

	struct
	{
		bool player_enemies = false;
		bool player_enemies_shine = false;
		int player_enemies_type;

		bool player_enemies_occluded = false;
		bool player_enemies_occluded_shine = false;
		int player_enemies_occluded_type;

		bool player_teammates = false;
		bool player_teammates_shine = false;
		int player_teammates_type;

		bool player_teammates_occluded = false;
		bool player_teammates_occluded_shine = false;
		int player_teammates_occluded_type;

		bool player_local = false;
		bool player_local_shine = false;
		int player_local_type;

		bool player_local_occluded = false;
		bool player_local_occluded_shine = false;
		int player_local_occluded_type;

		bool weapon = false;
		bool weapon_shine = false;
		int weapon_type;

		bool weapon_occluded = false;
		bool weapon_occluded_shine = false;
		int weapon_occluded_type;

		bool arms = false;
		bool arms_shine = false;
		int arms_type;

		bool arms_occluded = false;
		bool arms_occluded_shine = false;
		int arms_occluded_type;
	} chams;

	struct
	{
		std::unordered_map<int, skinInfo> skinInfo;
		int itemCount = 0;
	} inventory;
	struct
	{
		bool bEnabled = false;
		int iTargetSelection;
		int iMaxFOV = 39;
		bool bFriendlyFire = false;
		bool bAutoFire = false;
		bool bPreferBaim = false;
		bool bAutoWall = false;
		int iAimbotMode;
		bool bHitchance = false;
		int flHitchanceAmt = 0;
		int flMinDmg = 0;
		bool bAutoScope = false;
		int flBaimOnX = 0;
		bool bBaimIfDeadly = false;
		bool bAutostop = false;
		bool bRemoveRecoil = false;
		bool head = false;
		bool chest = false;
		bool body = false;
		bool hands = false;
		bool legs = false;
		int headmp = 0;
		int chestmp = 0;
		int bodymp = 0;
		int handsmp = 0;
		int legsmp = 0;
		int pitch = 0;
		int yaw = 0;
		int jittermin = 0;
		int jittermax = 0;
		bool desync = false;
		int desynctype;
		int bodylean = 100;
		int invbodylean = 100;
		int switchdesyncside = 0;
		bool slowwalk = false;
		int slowwalkspeed = 100;
		int slowwalkkey = 0;
	} ragebotconfig;
	struct
	{
		bool enabled = false;

		int friendly = -1;
		int teaching = -1;
		int leader = -1;

		int rank_id = 0;
		int wins = -1;

		int player_level = -1;
		int player_xp = -1;
	} profile;

	struct
	{
		bool bhop = false;
		bool autostrafe = false;

		bool rank_reveal = false;
		bool watermark = true;
		bool fakefps = false;
		bool autoaccept = false;
		bool nightmode = false;
		bool sniper_xhair = false;
		bool buyinfo = false;
		bool spectator_list = false;

		struct
		{
			bool no_hands = false;
			bool no_sleeves = false;
			bool no_weapon = false;
			bool no_smoke = false;
			bool no_scope = false;
			bool no_zoom = false;
			bool no_flash = false;
			bool no_blur = false;
			bool no_postprocess = false;
			bool no_fog = false;
			bool no_visual_recoil = false;
		} removals;

		struct
		{
			bool thirdpersonenabled = false;
			int hotkey = 0;
		} thirdperson;

		struct
		{
			bool enabled = false;
			bool onkey = false;
			int hotkey = 0;
			int ticks = 0;
		} fakelag;

		struct
		{
			bool enabled = false;
			bool onkey = false;
			int hotkey = 0;
		} edgejump;

		struct
		{
			int override = 90;
			int viewmodel = 68;
			int viewmodel_offset_x = 3;
			int viewmodel_offset_y = 3;
			int viewmodel_offset_z = -2;
		} fov;
	} misc;

	struct {
		struct {
			Color weapons = { 255, 255, 255 };
			Color defuse = { 255, 255, 255 };
			Color c4 = { 255, 255, 255 };
			Color item = { 255, 255, 255 };
			Color sounds_enemies = { 255, 255, 255 };
			Color sounds_teammates = { 255, 255, 255 };
			Color sounds_local = { 255, 255, 255 };
		} esp;

		struct {
			Color ally = { 255, 255, 255 };
			Color enemy = { 255, 255, 255 };
			Color local = { 255, 255, 255 };
			Color chickens = { 255, 255, 255 };
			Color planted_c4 = { 255, 255, 255 };
			Color weapons = { 255, 255, 255 };
		} glow;

		struct {
			Color player_ally_visible = { 255, 255, 255 };
			Color player_ally_visible_shine = { 255, 255, 255 };

			Color player_ally_occluded = { 255, 255, 255 };
			Color player_ally_occluded_shine = { 255, 255, 255 };

			Color player_enemy_visible = { 255, 255, 255 };
			Color player_enemy_visible_shine = { 255, 255, 255 };

			Color player_enemy_occluded = { 255, 255, 255 };
			Color player_enemy_occluded_shine = { 255, 255, 255 };

			Color player_local_visible = { 255, 255, 255 };
			Color player_local_visible_shine = { 255, 255, 255 };

			Color player_local_occluded = { 255, 255, 255 };
			Color player_local_occluded_shine = { 255, 255, 255 };

			Color arms_visible = { 255, 255, 255 };
			Color arms_visible_shine = { 255, 255, 255 };

			Color arms_occluded = { 255, 255, 255 };
			Color arms_occluded_shine = { 255, 255, 255 };

			Color weapon_visible = { 255, 255, 255 };
			Color weapon_visible_shine = { 255, 255, 255 };

			Color weapon_occluded = { 255, 255, 255 };
			Color weapon_occluded_shine = { 255, 255, 255 };
		} chams;
	} colors;

protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
private:
	void SetupValue(int& value, std::string category, std::string name);
	void SetupValue(bool& value, std::string category, std::string name);
	void SetupValue(float& value, std::string category, std::string name);
	void SetupColor(Color& value, const std::string& name);
	void SetupRagebot();
	void SetupWeapons();
	void SetupVisuals();
	void SetupMisc();
	void SetupColors();
public:
	void Initialize();
	void LoadSettings(const std::string& szIniFile);
	void SaveSettings(const std::string& szIniFile);
	void DeleteSettings(const std::string& szIniFile);

	std::string folder;
};

inline Options g_Options;
inline bool   g_Unload;
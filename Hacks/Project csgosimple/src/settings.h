#pragma once

#include "imgui/imgui.h"
#include "valve_sdk/csgostructs.hpp"

struct aimbot_group
{
	std::string name;
	std::vector<int> weapons = { };
};

enum settings_type_t : int
{
	separately = 0,
	subgroups,
	for_all,
	groups,
};

enum trigger_type
{
	in_fov = 0,
	in_crosshair,
};

enum fov_type_t : int
{
	statical = 0,
	dynamic,
	rage,
};

#pragma pack(push, 1)
struct weapon_setting 
{
	bool enabled = false;	
	bool autopistol = false;
	bool check_smoke = false;
	bool check_flash = false;
	bool check_zoom = true;
	bool check_air = true;
	bool by_damage = false;
	bool back_shot = false;

	int min_damage = 0;
	int min_hitchanse = 0;

	int rcs_type = 0;

	struct 
	{
		bool enabled = false;
		bool bt2 = false;
		int ticks = 6;
		bool legit = false;
		float time = 0.2f;
	} backtrack;

	bool autodelay = false;

	struct
	{
		bool enabled = false;
		bool in_radius = false;
		int delay = 0;
		int delay_btw_shots = 50;
		int hitchance = 1;
	} trigger;

	struct
	{
		bool enabled = false;
		bool always = false;
		float fov = 0.f;
		int chanse = 7;
		bool with_smooth = true;
	} silent;

	struct {
		bool enabled = false;
		bool first_bullet = false;
		bool humanize = false;
		bool standalone = true;
		bool rcs2 = false;

		float pitch = 2.0f;
		float yaw = 2.0f;
	} recoil;

	struct
	{
		bool head = true;
		bool neck = true;
		bool body = true;
		bool hands = true;
		bool legs = true;
	} hitboxes;

	bool dynamic_fov = false;
	float fov = 0.f;
	float smooth = 1;
	int shot_delay = 0;
	int kill_delay = 600;
};
#pragma pack(pop)

enum fake_lag_types : int
{
	lag_always = 0,
	lag_only_in_air,
	lag_when_pick,
	lag_by_button,
	lag_adaptive,
};

namespace settings 
{
	void load(std::string name);
	void save(std::string name);

	namespace movement
	{
		extern bool engine_prediction;
	}

	namespace esp 
	{
		extern bool enabled;
		extern bool visible_only;
		extern bool boxes;
		extern int box_type;
		extern bool names;
		extern bool health;
		extern int health_position;
		extern bool armour;
		extern int armour_position;
		extern bool weapons;
		extern bool ammo;
		extern bool offscreen;
		extern bool sound;
		extern bool snaplines;
		extern bool is_scoped;
		extern bool is_flashed;
		extern bool is_defusing;
		extern bool dormant;
		extern bool beams;
		extern int esp_on_chance;
		extern int esp_off_chance;
		extern bool is_reloading;
		extern float mfts;
		extern bool mat_force;
		extern bool drawFov;
		extern bool bomb_esp;
		extern bool kevlarinfo;
		extern bool haskit;
		extern bool money;
		extern bool ammo;
		extern bool antiobs;

		extern Color visible_color;
		extern Color occluded_color;
	}

	namespace glow
	{

		extern bool GlowEnemyEnabled;
		extern bool GlowTeamEnabled;
		extern bool GlowC4PlantedEnabled;
		extern bool GlowDroppedWeaponsEnabled;
		extern bool GlowNadesEnabled;

		extern float GlowEnemy[4];
		extern float GlowTeam[4];

		extern float GlowC4Planted[4];
		extern float GlowDroppedWeapons[4];
		extern float GlowNades[4];
	}

	namespace chams
	{
		extern bool enabled;
		extern bool visible_only;
		extern bool wireframe;
		extern bool flat;
		extern bool desync;
		extern bool localnew;
		extern bool teamnew;
		extern bool enemynew;
		extern bool btchams;
		extern bool xqz;
		extern int bttype;
		extern bool btflat;
		extern Color btcolor;

		extern float clr_weapon_chams[4];
		extern float clr_weapon_dropped_chams[4];
		extern float clr_weapon_dropped_chams_xqz[4];

		extern Color wepcolor;
		extern bool wepchams;
		extern bool wep_droppedchams;
		extern bool wep_droppedchams_xqz;

		extern bool nade_chams;
		extern bool nade_chams_xqz;
		extern float clr_nade_chams_xqz[4];
		extern float clr_nade_chams[4];

		extern bool plantedc4_chams;
		extern bool plantedc4_chams_xqz;
		extern float clr_plantedc4_chams[4];
		extern float clr_plantedc4_chams_xqz[4];

		extern int localmodenew;
		extern int teammodenew;
		extern int enemymodenew;

		extern Color visible_color;
		extern Color occluded_color;

		extern Color LocalColor_vis;
		extern Color LocalColor_invis;

		extern Color TeamColor_vis;
		extern Color TeamColor_invis;

		extern Color EnemyColor_vis;
		extern Color EnemyColor_invis;

		extern Color LocalColor_XQZ;
		extern Color TeamColor_XQZ;
		extern Color EnemyColor_XQZ;

		extern int matmode;

		namespace arms
		{
			extern bool enabled;
			extern bool wireframe;

			extern ImVec4 color;
		}
	}

	namespace visuals 
	{
		extern bool grenade_prediction;
		extern bool world_grenades;
		extern bool sniper_crosshair;
		extern bool defuse_kit;
		extern bool planted_c4;
		extern bool dropped_weapons;
		extern bool night_mode;
		extern bool pitch;
		extern int hitsound;
		extern bool skychanger;
		extern int skychanger_mode;
		extern bool newstyle;
		extern bool rcs_cross;
		extern float radius;
		extern int rcs_cross_mode;
		extern bool a;
		extern bool b;
		extern bool choke;
		extern bool skeletons;

		extern Color recoilcolor;
		extern Color floatc;
		extern Color skeletoncolor;

		extern ImVec4 sky;
	}

	namespace desync
	{
		extern bool enabled;
		extern bool yaw_flip;
		extern bool resolver;
		extern int desync_mode;
		extern bool enabled2;
	}

	namespace aimbot 
	{
		extern int setting_type;

		extern std::vector<aimbot_group> m_groups;
		extern std::map<int, weapon_setting> m_items;
	}

	namespace fake_lags
	{
		extern int type;
		extern int factor;
		extern bool enabled;
	}

	namespace lightning_shot
	{
		extern bool enabled;
		extern int delay;
		extern bool check_zoom;
	}

	namespace misc 
	{
		extern bool bhop;
		extern bool auto_strafe;
		extern bool no_flash;
		extern float viewmodel_fov;
		extern bool moon_walk;
		extern bool deathmatch;
		extern bool knife_bot;
		extern bool radar;
		extern bool human_bhop;
		extern int bhop_hit_chance;
		extern int hops_restricted_limit;
		extern int max_hops_hit;
		extern int debug_fov;
		extern bool triggerbot_in_zoom_only;
		extern bool esp_random;
		extern bool lknife;
		extern bool noscope;
		extern bool selfnade;
		extern bool lefthandknife;
		extern bool legitpeek;
	}
}


#pragma once

enum aimbot_modes {
	aimbot_legit,
	aimbot_rage
};
enum rage_aimbot_hitboxes {
	head,
	neck,
	upper_chest,
	chest,
	lower_chest,
	pelvis,
	legs,
	toes,
	hitbox_size
};
enum rage_aimbot_bodyaim {
	lethal,
	in_air,
	slowwalking,
	baim_size
};

struct variables_t {
	int aimbot_mode = 0;
	struct {
		bool enabled = false;
		bool silent = false;
		bool resolver = false;
		bool backtracking = false;
		bool auto_scope = false;
		bool auto_stop = false;
		struct {
			bool hitboxes[rage_aimbot_hitboxes::hitbox_size];
			bool multipoints[rage_aimbot_hitboxes::hitbox_size];
			bool preffer_baim[rage_aimbot_bodyaim::baim_size];
			float hitchance = 0.f;
			float backtrack_hitchance = 0.f;
			float minimim_damage_autowall = 0.f;
			float minimim_damage_visible = 0.f;
		} general_cfg;
	} rage_aimbot;
	struct {
		bool ignore_flash = false;
		bool ignore_smoke = false;
		struct {
			bool enabled = false;
			int mode = 0;
			int hitpoint = 0;
			float fov = 0.f;
			float smooth = 0.f;
		} rifles;
		struct {
			bool enabled = false;
			int mode = 0;
			int hitpoint = 0;
			float fov = 0.f;
			float smooth = 0.f;
		} snipers;
		struct {
			bool enabled = false;
			int mode = 0;
			int hitpoint = 0;
			float fov = 0.f;
			float smooth = 0.f;
		} pistols;
	} aimbot;
	struct {
		struct {
			bool enabled = false;
			int key = 0;
			bool ignore_flash = false;
			bool ignore_smoke = false;
			int hitpoint = 0;
			int delay = 0;
			int minimim_damage = 0;
		} rifles;
		struct {
			bool enabled = false;
			int key = 0;
			bool ignore_flash = false;
			bool ignore_smoke = false;
			int hitpoint = 0;
			int delay = 0;
			int minimim_damage = 0;
		} snipers;
		struct {
			bool enabled = false;
			int key = 0;
			bool ignore_flash = false;
			bool ignore_smoke = false;
			int hitpoint = 0;
			int delay = 0;
			int minimim_damage = 0;
		} pistols;
	} triggerbot;
	struct {
		bool enabled = false;
		int range = 200;
		bool aimatbt = false;
	} backtracking;
	struct {
		bool dead_only = false;
		int key_bind = 0;
		bool box = false;
		float box_clr[3] = { 1.f, 1.f, 1.f };
		bool name = false;
		float name_clr[3] = { 1.f, 1.f, 1.f };
		bool weapon = false;
		float weapon_clr[3] = { 1.f, 1.f, 1.f };
		bool health = false;
		bool ammo = false;
		float ammo_clr[3] = { 0.313f, 0.509f, 0.784f };
		bool skeleton = false;
		float skeleton_clr[3] = { 1.f, 1.f, 1.f };
	} pl_esp;
	struct {
		int fov = 0;
		int viewmodel_fov = 0;
		int nightmode = 0;
		bool sniper_crosshair = false;
		bool bomb_timer = false;
	} visuals;
	struct {
		struct {
			bool enabled = false;
			float clr[3] = { 0.3f, 0.8f, 0.6f };
			int bloom = 100;
		} glow;
		struct {
			bool enabled = false;
			bool flat = false;
			bool ignore_z = false;
			float clr[3] = { 0.35f, 0.35f, 0.7f };
			float clr_z[3] = { 0.f, 0.f, 0.f };
			int transparency = 0;
			bool backtrack = false;
			float clr_backtrack[3] = { 0.13f, 0.13f, 0.13f };
			int transparency_backtrack = 25;
			bool glow_overlay = false;
			float glow_clr[3] = { 0.f, 0.45f, 0.7f };
			int transparency_glow = 25;
		} chams;
	} pl_mdl;
	struct {
		int config_file = 0;
		bool bunny_hop = false;
		bool hitmarker{ false };
		int hitmarker_sound{ 0 };
		int bunny_hop_chance = 70;
		int bunny_hop_max_landed = 3;
		int bunny_hop_max_restricted = 6;
		bool radar = false;
		bool reveal_ranks = false;
		bool antiaim = false;
		int antiaim_invert = 0x12;
		bool antiaim_arrows = false;
		bool disable_post_processing = false;
		bool infduck= false ;
		bool bEdgeJump= false ;
		int edgejumpkey= 0;
		int edgebugkey = 0;
		bool bDuckbug = false ;
		bool disable_shadow = false;
		bool remove_3dsky = false;
		bool remove_bloom = false;
		bool nade_predict = false;

		bool thirdperson = false;
		int thirdperson_key = 0x56;
		float desyncclr1[3] = { 0.2f, 0.36f, 0.92f };
		float desyncclr2[3] = { 0.f, 0.f, 0.f };
		int viewmodel[3];
		bool clantag = false;
		bool chatspam = false;
		bool speclist = false;
	} misc;
	struct {
		bool enabled = false;
		int knife_model = 0;
		int knife_skin = 0;

		int glove_model = 0;
		int hydra_skin = 0;
		int bloodhound_skin = 0;
		int driver_skin = 0;
		int handwrap_skin = 0;
		int moto_skin = 0;
		int specialist_skin = 0;
		int sport_skin = 0;

		int paint_kit_index_usp = 0;
		int paint_kit_index_p2000 = 0;
		int paint_kit_index_glock = 0;
		int paint_kit_index_p250 = 0;
		int paint_kit_index_fiveseven = 0;
		int paint_kit_index_tec = 0;
		int paint_kit_index_cz = 0;
		int paint_kit_index_duals = 0;
		int paint_kit_index_deagle = 0;
		int paint_kit_index_revolver = 0;
		int paint_kit_index_famas = 0;
		int paint_kit_index_galil = 0;
		int paint_kit_index_m4a4 = 0;
		int paint_kit_index_m4a1 = 0;
		int paint_kit_index_ak47 = 0;
		int paint_kit_index_sg553 = 0;
		int paint_kit_index_aug = 0;
		int paint_kit_index_ssg08 = 0;
		int paint_kit_index_awp = 0;
		int paint_kit_index_scar = 0;
		int paint_kit_index_g3sg1 = 0;
		int paint_kit_index_sawoff = 0;
		int paint_kit_index_m249 = 0;
		int paint_kit_index_negev = 0;
		int paint_kit_index_mag7 = 0;
		int paint_kit_index_xm1014 = 0;
		int paint_kit_index_nova = 0;
		int paint_kit_index_bizon = 0;
		int paint_kit_index_mp5sd = 0;
		int paint_kit_index_mp7 = 0;
		int paint_kit_index_mp9 = 0;
		int paint_kit_index_mac10 = 0;
		int paint_kit_index_p90 = 0;
		int paint_kit_index_ump45 = 0;
	} skins;
};

extern variables_t variables;

namespace config {
	void save();
	void load();
}
#pragma once
#include "../../singleton.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../globals.hpp"
#include "../../addons/keybinds/keybinds.hpp"


class c_misc {
private:
	float stab_dist = 32.f;
	float slash_dist = 48.f;
private:
	struct misc_options_t {
		bool show_spread = false;
		int  show_spread_type = 0;
		float cMenuCol[4] = { 0.00f, 1.0f, 1.0f ,1.0f };
		int newstyle = 0;
		bool slow_walk = false;
		CKeyBind slow_walk_bind = CKeyBind(&CheatGVars::SlowWalking, &slow_walk, "Slow Walk");
		float slow_walk_speed = 15.f;



		bool edge_jump = false;
		bool edge_jumping = false;
		CKeyBind edge_jump_key = CKeyBind(&edge_jumping, &edge_jump, "Edge Jump");

		bool knife_bot = false;
		bool customize_viewmodel = false;
		bool RageAimbotHead = false;
		bool RageAimbotBody = false;
		bool RageAimbotLegs =false;
		bool RageAimbotToes = false;
		bool RageAimbotSafePoint= false;
		bool RageAimbotDelayShot = false;
		bool RageAntiaimEnabled = false;
		float RageAimbotMinDmg = 30;
		float RageAimbotHitchance = 60;
		bool rage_enable = false;
		bool rage_silent = false;
		bool RageAimbotResolver = false;
		int RageAimbotBaimAfter = 2;
		int RageAimbotHeadScale = 40;
		int RageAimbotBodyScale = 60;
		int customx = 0;
		int customy = 0;
		int customz = 0;
		bool velocity = false;
		float misc_aspect_ratio = 0.f;
		int  knife_bot_dist = 81;
		int  knife_bot_filter = 0;
		int  knife_bot_mode = 0;

		bool show_fov = false;

		float alpha_modulate = 255.f;

		bool no_zoom = false;
		bool view_fov_changer = false;
		float view_fov = 90.f;

		bool radar = false;

		bool rank_reveal = false;

		bool bhop = false;

		int  bhop_chance = 100;
		bool autob = false;
		bool show_crosshair = false;

		bool legit_aa = false;
		int legit_aa_mode = 0;
		bool aa_arrows = false;
		int aa_arrows_length = 30;
		int fakelag_limit = 0;
		bool legit_aa_flip = false;
		CKeyBind legit_aa_filp_key = CKeyBind(&legit_aa_flip, &legit_aa, "LegitAA Flip");

		bool nosmoke = false;
		
		bool autostrafe = false;
		bool strafing = false;
		CKeyBind autostrafe_key = CKeyBind(&strafing, &autostrafe, "AutoStrafe");

		bool infinite_duck = false;

		bool block_botting = false;
		CKeyBind blockbot_key = CKeyBind(&block_botting, nullptr, "BlockBot");

		bool no_visual_recoil = false;

		bool molotov_timer = false;

		float nightmode_value = 1.1f;

		bool damage_indicator = false;

		bool grenade_prediction = false;

		bool chams_backtrack_all_ticks = true;

		bool eventlogger_hurt = false;
		bool eventlogger_player_buy = false;
		bool eventlogger_planting = false;
		bool eventlogger_defusing = false;

		bool third_person = false;
		CKeyBind third_person_bind = CKeyBind(&third_person, nullptr, "3rd Person");
		//int third_person = 0;
		float third_person_dist = 100.f;

		bool resolver_enabled = false;
		int resolver_mode = 0;

		bool jump_bugging = false;
		CKeyBind jump_bug_key = CKeyBind(&jump_bugging, nullptr, "JumpBug");

		bool show_keybinds = false;
	};
	struct misc_colors_t {
		Color accent_color = Color(255, 255, 255, 100);
		
		Color spread_color = Color(255, 255, 255);

		Color fov_default = Color(255, 255, 255);
		Color fov_silent = Color(255, 255, 255);

		Color damage_indicator_color = Color(255, 255, 255);

		Color grenade_prediction_color = Color(255, 255, 255);

		Color molotov_timer_color = Color(255, 255, 255);
	};
public:
	misc_options_t options;
	misc_colors_t colors;
private:
	void bhop(CUserCmd* cmd);
	void autostrafe(CUserCmd* cmd);
	void jump_bug(CUserCmd* cmd);
	void BuyInfo();
	void knife_bot(CUserCmd* cmd, bool* bSendPacket);
	void slow_walk(CUserCmd* cmd);
private:
	void show_fov();
	void CustomFov();
private:
	void show_spread();
private:
	void ThirdPerson();
public:
	void create_move(CUserCmd* cmd, bool* bSendPacket);
	void create_move_prediction(CUserCmd* cmd, bool* bSendPacket);
	void velocity_ind();
	void paint();
	void end_scene();
	void fire_event(IGameEvent* pEvent);
	void override_view();
	void frame_stage(ClientFrameStage_t stage);
public:
	void Menu();
	void SetupValues();
};

extern c_misc* g_Misc;
#pragma once
#include "../SDK/singleton.h"
class c_config : public singleton< c_config > {
private:

public:

	bool legit_aimbot_enabled = false;
	bool legit_aimbot_backtrack = false;
	int legit_aimbot_fov = 0;
	int legit_aimbot_priority_hitbox = 0;
	bool rcs = false;
	int rcs_x = 0;
	int rcs_y = 0;
	int linear_progression_threshold = 0;
	bool prefer_bodyaim[6] = { false, false, false, false, false };
	int bodyaim_mode = 0;
	/*--------------------------------------*/
	bool aimbot_enabled = false;	// AIMBOT SETTINGS
	/*--------------------------------------*/

	bool aimbot_norecoil = false;
	bool aimbot_silentaim = false;
	int aimbot_headpointscale = 0;
	int aimbot_bodypointscale = 0;
	int aimbot_fov = 0;
	bool aimbot_resolver = false;

	bool autostop = false;
	bool autorevolver = false;
	bool stop_inbetween_shots = false;
	int autostop_mode = 0;
	bool auto_scope = false;
	bool auto_crouch = false;
	bool accuracy_boost = false;
	bool fakelag_prediction = false;

	int auto_mindamage = 0;
	int auto_hitchance = 0;

	int scout_mindamage = 0;
	int scout_hitchance = 0;

	int awp_mindamage = 0;
	int awp_hitchance = 0;

	int heavy_pistol_mindamage = 0;
	int heavy_pistol_hitchance = 0;

	int other_mindamage = 0;
	int other_hitchance = 0;

	bool prefer_bodyaim_if_low_velocity = false;
	bool smart_bodyaim = false;

	bool antiaim_enabled = false;
	bool antiaim_freestanding = false;
	int antiaim_freestanding_mode = 0;
	bool extrapolated = false;
	int desync = 0;
	bool on_shot_aa = false;
	int on_shot_aa_type = 0;
	int fakelag = 0;
	int fakeduck_test = 0;
	int variance = 0;
	int pitch_standing;
	int yaw_standing;
	int pitch_moving;
	int yaw_moving;
	int pitch_air;
	int yaw_air;
	int aa_jitter = 0;

	/*--------------------------------------*/
	bool visuals_enabled = false;	// VISUAL SETTINGS
	/*--------------------------------------*/
	bool remove_visual_recoil = false;
	bool draw_spread = false;

	/*
	Enemies
	*/
	bool name_enemies = false;
	bool box_enemies = false;
	bool weapon_enemies = false;
	bool show_icon_when_possible_enemies = false;
	int health_enemies = 0;
	bool glow_enemies = false;
	bool chams_enemies = false;
	bool chams_xqz_enemies = false;
	bool flags_enemy = false;
	bool ammo_enemy = false;

	bool fov_arrows_enemy = false;
	bool fov_arrows_always_indicate = false;
	int fov_arrows_enemy_size = 0;
	int fov_arrows_enemy_distance  = 0;

	int name_esp_color_r = 255, name_esp_color_g = 255, name_esp_color_b = 255, name_esp_color_a = 255;
	int box_esp_color_r = 255, box_esp_color_g = 255, box_esp_color_b = 255, box_esp_color_a = 255;
	int weapon_esp_color_r = 255, weapon_esp_color_g = 255, weapon_esp_color_b = 255, weapon_esp_color_a = 255;
	int glow_esp_color_r = 255, glow_esp_color_g = 255, glow_esp_color_b = 255, glow_esp_color_a = 255;

	int chams_esp_color_r = 255, chams_esp_color_g = 255, chams_esp_color_b = 255, chams_esp_color_a = 255;
	int chams_xqz_esp_color_r = 255, chams_xqz_esp_color_g = 255, chams_xqz_esp_color_b = 255, chams_xqz_esp_color_a = 255;

	int fov_arrows_esp_color_r = 255, fov_arrows_esp_color_g = 255, fov_arrows_esp_color_b = 255, fov_arrows_esp_color_a = 255;
	/*
	Teammates
	*/
	bool name_teammates = false;
	int box_teammates = 0;
	bool weapon_teammates = false;
	bool show_icon_when_possible_teammates = false;
	bool health_teammates = false;
	bool glow_teammates = false;
	int glow_alpha_teammates = 0;
	bool chams_teammates = false;

	/*
	Local
	*/
	bool name_local = false;
	int box_local = 0;
	bool weapon_local = false;
	bool show_icon_when_possible_local = false;
	bool health_local = false;
	int glow_alpha_local = 0;
	bool chams_local = false;

	int fov = 0;
	bool fov_while_zoomed = false;
	int viewmodel_fov = 0;
	int thirdperson_distance = 0;
	bool remove_scope = false;
	bool dynamic_scope = false;
	bool force_crosshair = false;
	bool nightmode = false;
	bool hitmarker = false;
	int hitmarker_sound = 0;
	bool no_smoke = false;
	bool transparent_props = false;
	bool wireframe_arms = false;
	int hand_chams = 0;
	bool local_chams = false;
	bool local_glow = false;
	int local_chams_color_r = 255, local_chams_color_g = 255, local_chams_color_b = 255, local_chams_color_a = 255;
	int local_glow_color_r = 255, local_glow_color_g = 255, local_glow_color_b = 255, local_glow_color_a = 255;
	int hand_chams_color_r = 255, hand_chams_color_g = 255, hand_chams_color_b = 255, hand_chams_color_a = 255;
	bool event_logger[3] = { false, false, false };
	bool visual_preserve_killfeed = false;
	bool visual_spectator_list = false;
	bool penetration_crosshair = false;
	bool remove_flash = false;
	bool visuals_radar;
	bool grenade_prediction = false;
	/*--------------------------------------*/
												// MISC SETTINGS
	/*--------------------------------------*/

	bool misc_save_config = false;
	bool misc_load_config = false;
	bool misc_export_to_clipboard_config = false;
	bool misc_import_from_clipboard_config = false;

	bool misc_bhop = false;
	bool misc_autostrafe = false;
	bool misc_fastcrouch = false;
	bool misc_radiospam = false;
	bool misc_clantag = false;
	bool misc_antiuntrusted = false;
	int slowwalk_speed = 0;

	int thirdperson_bind = -1;
	int slowwalk_bind = -1;
	int left_bind = -1;
	int right_bind = -1;
	int back_bind = -1;
	int fakeduck_bind = -1;
	int manual_swap_bind = -1;
	int knife_model = 0;
	int knife_skin = 0;
	int ssg08_skin = 0;

	int m4a4_skin;

	int m4a1_s_skin;
	int ak47_skin;
	int aug_skin;
	int famas_skin;
	int g3sg1_skin;
	int galilar_skin;
	int scar20_skin;
	int sg556_skin;
	int bizon_skin;
	int mac10_skin;
	int mp7_skin;
	int mp9_skin;
	int p90_skin;
	int ump45_skin;
	int mp5sd_skin;
	int m249_skin;
	int mag7_skin;
	int negev_skin;
	int nova_skin;
	int sawedoff_skin;
	int xm1014_skin;
	int usps_skin;
	int deagle_skin;
	int dualelites_skin;
	int fiveseven_skin;
	int glock_skin;
	int p2000_skin;
	int p250_skin;
	int tec9_skin;
	int cz75_skin;
	int r8_skin;

	int menu_color_r = 255, menu_color_g = 255, menu_color_b = 255, menu_color_a = 255;
};
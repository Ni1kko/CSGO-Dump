#pragma once

#include "interfaces/IInputSystem.h"

#define OPTION(type, var, value) type var = value

class Options
{

public:

	OPTION(bool, misc_autoaccept, false);
	OPTION(bool, misc_revealAllRanks, false);
	OPTION(bool, esp_enemies_only, true);
	OPTION(bool, esp_farther, false);
	OPTION(float, esp_fill_amount, 0.f);
	float esp_player_fill_color_t[4] = { 1.f, 0.f, 0.3f, 1.f };
	float esp_player_fill_color_ct[4] = { 0.f, 0.2f, 1.f, 1.f };
	float esp_player_fill_color_t_visible[4] = { 1.f, 1.f, 0.0f, 1.f };
	float esp_player_fill_color_ct_visible[4] = { 0.f, 0.7f, 1.f, 0.85f };
	OPTION(int, esp_player_boundstype, 0);
	OPTION(int, esp_player_boxtype, 0);
	float esp_player_bbox_color_t[4] = { 1.f, 0.f, 0.3f, 1.f };
	float esp_player_bbox_color_ct[4] = { 0.f, 0.2f, 1.f, 1.f };
	float esp_player_bbox_color_t_visible[4] = { 1.f, 1.f, 0.0f, 1.f };
	float esp_player_bbox_color_ct_visible[4] = { 0.f, 0.7f, 1.f, 0.85f };
	OPTION(bool, esp_player_name, false);
	OPTION(bool, esp_player_health, false);
	OPTION(bool, esp_player_weapons, false);
	OPTION(bool, esp_player_ammo, false);
	OPTION(bool, esp_player_snaplines, false);
	OPTION(bool, esp_player_chams, false);
	OPTION(int, esp_player_chams_type, 0);
	OPTION(bool, esp_localplayer_chams, false);
	OPTION(bool, fake_chams, false);
	OPTION(bool, backtrack_bhd_wall_only, false);
	OPTION(bool, esp_localplayer_chams_xyz, false);
	OPTION(int, esp_localplayer_chams_type, 0);
	OPTION(int, esp_localplayer_fakechams_type, 0);
	OPTION(int, esp_localplayer_viewmodel_materials, 0);
	OPTION(bool, esp_localplayer_viewmodel_chams, false);
	float esp_player_chams_color_t_visible[4] = { 1.f, 1.f, 0.0f, 1.f };
	float esp_player_chams_color_ct_visible[4] = { 0.15f, 0.7f, 1.f, 1.0f };
	float esp_localplayer_chams_color_visible[4] = { 0.15f, 0.7f, 1.f, 1.0f };
	float esp_localplayer_chams_color_invisible[4] = { 0.f, 0.2f, 1.f, 1.f };
	float esp_localplayer_fakechams_color_visible[4] = { 1.f, 0.f, 0.f, 1.0f };
	float esp_localplayer_viewmodel_chams_color[4] = { 1.f, 0.f, 0.f, 1.0f };
	float esp_localplayer_fakechams_color_invisible[4] = { 1.f, 0.f, 0.f, 1.f };
	float esp_player_chams_color_t[4] = { 1.f, 0.f, 0.3f, 1.f };
	float esp_player_chams_color_ct[4] = { 0.f, 0.2f, 1.f, 1.f };
	//OPTION(float, esp_localplayer_chams_alpha, 0.f);
	OPTION(bool, esp_localplayer_viewmodel_chams_wireframe, false);
	OPTION(bool, esp_player_skelet, false);
	OPTION(bool, esp_player_anglelines, false);
	OPTION(int, esp_dropped_weapons, 0);
	OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_grenades, false);
	OPTION(bool, autothrow_grenades, false);
	OPTION(int, autothrow_grenades_dmg, 0);
	OPTION(int, esp_grenades_type, 0);
	OPTION(float, visuals_others_player_fov, 0);
	OPTION(float, slowwalk_value, 0);
	OPTION(float, visuals_others_player_fov_viewmodel, 0);

	OPTION(float, viewmodel_offset_x, 3);
	OPTION(float, viewmodel_offset_y, 6);
	OPTION(float, viewmodel_offset_z, -2);

	OPTION(bool, visuals_others_watermark, true);
	OPTION(bool, visuals_manual_aa, false);
	OPTION(float, visuals_manual_aa_opacity, 200);
	OPTION(bool, visuals_nightmode, false);
	OPTION(bool, visuals_asuswalls, false);
	OPTION(int, visuals_weapon_type, 0);
	OPTION(bool, removals_zoom, false);
	OPTION(float, visuals_asuswalls_value, 90);	
	OPTION(bool, visuals_asuswalls_staticprops, false);
	OPTION(bool, visuals_asuswalls_world, false);
	OPTION(bool, visuals_others_grenade_pred, false);
	OPTION(int, visuals_others_sky, 0);
	OPTION(int, visuals_nightmode_type, 0);
	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_players, false);
	float glow_player_color_t[4] = { 1.f, 0.f, 0.3f, 1.f };
	float glow_player_color_ct[4] = { 0.f, 0.2f, 1.f, 1.f };
	float glow_player_color_t_visible[4] = { 1.f, 1.f, 0.0f, 1.f };
	float glow_player_color_ct_visible[4] = { 0.f, 0.7f, 1.f, 0.85f };
	float nightmode_color[4] = { 0.15f, 0.15f, 0.15f, 1.f };
	int dropped_weapons_color[4] = { 255, 50, 50, 255 };
	OPTION(int, glow_players_style, 0);
	OPTION(bool, glow_others, false);
	OPTION(int, glow_others_style, 0);
	OPTION(bool, visuals_others_hitmarker, false);
	OPTION(bool, visuals_others_bulletimpacts, false);
	float visuals_others_bulletimpacts_color[4] = { 0.012f, 0.788f, 0.663f, 1.f };
	int xhair_color[4] = { 255, 255, 255, 255 };
	OPTION(bool, removals_scope, false);
	OPTION(bool, removals_novisualrecoil, false);
	OPTION(bool, rage_aimstep, false);
	OPTION(bool, removals_postprocessing, false);
	OPTION(bool, removals_flash, false);
	OPTION(bool, removals_smoke, false);
	OPTION(int, removals_smoke_type, 0);

//#ifdef NIGHTMODE
	//OPTION(bool, visuals_others_nightmode, false);
	float visuals_others_nightmode_color[4] = { 0.15f, 0.15f, 0.15f, 1.f };
	float visuals_others_skybox_color[4] = { 0.15f, 0.15f, 0.15f, 1.f };
//#endif


	OPTION(bool, misc_bhop, false);
	OPTION(bool, misc_autostrafe, false);
	OPTION(bool, removals_crosshair, false);
	OPTION(bool, misc_auto_pistol, false);
	OPTION(bool, misc_infinite_duck, false);
	OPTION(bool, misc_chatspamer, false);
	OPTION(bool, misc_aimware_chatspamer, false);
	OPTION(bool, misc_thirdperson, false);
	OPTION(ButtonCode_t, misc_thirdperson_bind, BUTTON_CODE_NONE);
	OPTION(ButtonCode_t, misc_fakeduck, BUTTON_CODE_NONE);
	OPTION(bool, misc_fakewalk, false);
	OPTION(int, misc_fakewalk_speed, 0);
	OPTION(ButtonCode_t, misc_fakewalk_bind, BUTTON_CODE_NONE);
	OPTION(bool, legit_on_press, false);
	OPTION(ButtonCode_t, hvh_aa_left_bind, BUTTON_CODE_NONE);
	OPTION(ButtonCode_t, hvh_aa_right_bind, BUTTON_CODE_NONE);
	OPTION(ButtonCode_t, hvh_aa_back_bind, BUTTON_CODE_NONE);
	OPTION(ButtonCode_t, menu_open_alternative, BUTTON_CODE_NONE);
	OPTION(bool, misc_fakelag_enabled, false);
	OPTION(bool, misc_namestealer, false);
	OPTION(int, misc_fakelag_value, 0);
	OPTION(int, misc_fakelag_activation_type, 0);
	OPTION(bool, misc_fakelag_adaptive, false);
	OPTION(bool, misc_animated_clantag, false);
	OPTION(bool, misc_become_gay, false);
	OPTION(bool, misc_aimware_clantag, false);
	OPTION(bool, misc_spectatorlist, false);
	OPTION(bool, misc_instant_defuse_plant, false);
	OPTION(bool, misc_logevents, false);
	OPTION(bool, legit_enabled, false);
	OPTION(bool, legit_on_key, false);
	OPTION(ButtonCode_t, legit_aim_keybind, BUTTON_CODE_NONE);
	OPTION(bool, legit_rcs, false);
	OPTION(bool, legit_trigger, false);
	OPTION(bool, legit_trigger_with_aimkey, false);
	OPTION(int, legit_preaim, 0);

	OPTION(bool, cl_phys_timescale, false);
	OPTION(bool, mp_radar_showall, false);
	OPTION(bool, cl_crosshair_recoil, false);
	OPTION(bool, fog_override, false);
	OPTION(float, cl_phys_timescale_value, 0.1f);
	OPTION(bool, change_viewmodel_offset, false);

	OPTION(bool, legit_hsonly, false);
	OPTION(int, legit_aftershots, 3);
	OPTION(int, legit_afteraim, 0);
	OPTION(bool, visuals_draw_xhair, false);
	OPTION(int, visuals_xhair_x, 1);
	OPTION(int, visuals_xhair_y, 1);
	OPTION(float, legit_smooth_factor, 1.f);
	OPTION(float, legit_fov, 1.f);
	OPTION(bool, rage_enabled, false);
	OPTION(ButtonCode_t, rage_aimkey, BUTTON_CODE_NONE);
	//OPTION(bool, rage_usekey, false);
	OPTION(bool, rage_silent, false);
	OPTION(bool, rage_norecoil, false);
	OPTION(bool, rage_autoshoot, false);
	OPTION(bool, rage_autoscope, false);
	OPTION(bool, rage_autocrouch, false);
	OPTION(bool, rage_autostop, false);
	OPTION(bool, rage_autobaim, false);
	OPTION(bool, rage_autocockrevolver, false);
	OPTION(int, rage_baim_after_x_shots, 0);
	OPTION(bool, rage_lagcompensation, false);
	//OPTION(int, rage_lagcompensation_type, 0);
	OPTION(bool, trigger_on_press, false);
	OPTION(ButtonCode_t, trigger_keybind, BUTTON_CODE_NONE);
	OPTION(bool, rcs_on_press, false);
	OPTION(ButtonCode_t, rcs_keybind, BUTTON_CODE_NONE);
	OPTION(int, test_slider, 0);
	OPTION(bool, rage_fixup_entities, false);
	OPTION(float, rage_mindmg, 0.f);
	/*OPTION(float, rage_mindmg_auto, 0.f);
	OPTION(float, rage_mindmg_scout, 0.f);
	OPTION(float, rage_mindmg_assaultrifle, 0.f);
	OPTION(float, rage_mindmg_mg, 0.f);
	OPTION(float, rage_mindmg_smg, 0.f);
	OPTION(float, rage_mindmg_pistol, 0.f);
	OPTION(float, rage_mindmg_awp, 0.f);
	OPTION(float, rage_mindmg_shotgun, 0.f);
	OPTION(float, rage_mindmg_deagr8, 0.f);*/
	OPTION(float, rage_hitchance_amount, 0.f);
	OPTION(float, rage_hitchance_amount_auto, 0.f);
	OPTION(float, rage_hitchance_amount_scout, 0.f);
	OPTION(float, rage_hitchance_amount_assaultrifle, 0.f);
	OPTION(float, rage_hitchance_amount_mg, 0.f);
	OPTION(float, rage_hitchance_amount_smg, 0.f);
	OPTION(float, rage_hitchance_amount_pistol, 0.f);
	OPTION(float, rage_hitchance_amount_awp, 0.f);
	OPTION(float, rage_hitchance_amount_shotgun, 0.f);
	OPTION(float, rage_hitchance_amount_deagr8, 0.f);

	OPTION(float, rage_mindmg_amount_auto, 0.f);
	OPTION(float, rage_mindmg_amount_scout, 0.f);
	OPTION(float, rage_mindmg_amount_assaultrifle, 0.f);
	OPTION(float, rage_mindmg_amount_mg, 0.f);
	OPTION(float, rage_mindmg_amount_smg, 0.f);
	OPTION(float, rage_mindmg_amount_pistol, 0.f);
	OPTION(float, rage_mindmg_amount_awp, 0.f);
	OPTION(float, rage_mindmg_amount_shotgun, 0.f);
	OPTION(float, rage_mindmg_amount_deagr8, 0.f);

	OPTION(float, rage_pointscale_amount_auto, 0.7f);
	OPTION(float, rage_pointscale_amount_scout, 0.7f);
	OPTION(float, rage_pointscale_amount_assaultrifle, 0.7f);
	OPTION(float, rage_pointscale_amount_mg, 0.7f);
	OPTION(float, rage_pointscale_amount_smg, 0.7f);
	OPTION(float, rage_pointscale_amount_pistol, 0.7f);
	OPTION(float, rage_pointscale_amount_awp, 0.7f);
	OPTION(float, rage_pointscale_amount_shotgun, 0.7f);
	OPTION(float, rage_pointscale_amount_deagr8, 0.7f);

	OPTION(int, iRage_hitbox_auto, 0);
	OPTION(int, iRage_hitbox_scout, 0);
	OPTION(int, iRage_hitbox_assaultrifle, 0);
	OPTION(int, iRage_hitbox_mg, 0);
	OPTION(int, iRage_hitbox_smg, 0);
	OPTION(int, iRage_hitbox_pistol, 0);
	OPTION(int, iRage_hitbox_awp, 0);
	OPTION(int, iRage_hitbox_shotgun, 0);
	OPTION(int, iRage_hitbox_deagr8, 0);

	OPTION(bool, bRage_multipoint_pistol, false);
	OPTION(bool, bRage_multipoint_deagr8, false);
	OPTION(bool, bRage_multipoint_smg, false);
	OPTION(bool, bRage_multipoint_shotgun, false);
	OPTION(bool, bRage_multipoint_mg, false);
	OPTION(bool, bRage_multipoint_assaultrifle, false);
	OPTION(bool, bRage_multipoint_scout, false);
	OPTION(bool, bRage_multipoint_awp, false);
	OPTION(bool, bRage_multipoint_auto, false);

	OPTION(bool, bRage_prioritize_pistol, false);
	OPTION(bool, bRage_prioritize_deagr8, false);
	OPTION(bool, bRage_prioritize_smg, false);
	OPTION(bool, bRage_prioritize_mg, false);
	OPTION(bool, bRage_prioritize_shotgun, false);
	OPTION(bool, bRage_prioritize_assaultrifle, false);
	OPTION(bool, bRage_prioritize_scout, false);
	OPTION(bool, bRage_prioritize_awp, false);
	OPTION(bool, bRage_prioritize_auto, false);

	//OPTION(int, rage_hitbox, 0);
	//OPTION(bool, rage_multipoint, false);
	//OPTION(float, rage_pointscale, 0.7f);


	/*bool rage_multiHitboxes[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};*/

	bool rage_multiHitboxesPistol[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesDeagR8[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesSMG[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesMG [14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesShotgun[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesAssaultRifle[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesScout[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesAWP[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};
	bool rage_multiHitboxesAuto[14] =
	{
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true
	};

	//OPTION(bool, rage_prioritize, false);

	OPTION(bool, esp_backtracked_player_skelet, false);
	OPTION(bool, esp_lagcompensated_hitboxes, false);
	OPTION(int, esp_lagcompensated_hitboxes_type, 0);
	OPTION(int, hvh_antiaim_x, 0);
	OPTION(int, hvh_antiaim_y, 0);
	OPTION(int, hvh_antiaim_y_move, 0);
	OPTION(float, hvh_antiaim_y_move_trigger_speed, 0.1);
	//OPTION(float, hvh_antiaim_y_custom_fake_left, 0.f);
	//OPTION(float, hvh_antiaim_y_custom_fake_right, 0.f);
	OPTION(float, hvh_antiaim_y_custom_left, 0.f);
	OPTION(float, hvh_antiaim_y_custom_right, 0.f);
	OPTION(float, hvh_antiaim_y_custom_back, 0.f);
	OPTION(float, hvh_antiaim_y_desync, 0.f);
	OPTION(float, hvh_antiaim_y_desync_start_left, 0.f);
	OPTION(float, hvh_antiaim_y_desync_start_right, 0.f);
	OPTION(float, hvh_antiaim_y_desync_start_back, 0.f);
	//OPTION(float, hvh_antiaim_y_custom_fakemove_left, 0.f);
	//OPTION(float, hvh_antiaim_y_custom_fakemove_right, 0.f);
	OPTION(float, hvh_antiaim_y_custom_realmove_left, 0.f);
	OPTION(float, hvh_antiaim_y_custom_realmove_right, 0.f);
	OPTION(float, hvh_antiaim_y_custom_realmove_back, 0.f);

	OPTION(int, hvh_antiaim_y_fake, 0);
	//OPTION(int, hvh_antiaim_y_fake_move, 0);
	OPTION(bool, hvh_antiaim_lby_breaker, false);
	OPTION(int, hvh_show_real_angles, false);
	OPTION(bool, hvh_resolver, false);
	OPTION(bool, hvh_resolver_custom, false);
	OPTION(bool, hvh_resolver_override, false);

	//Resolver scheiﬂe
	/* eye lby */
	OPTION(bool, resolver_eyelby_running, false);
	OPTION(bool, resolver_eyelby_notrunning, false);
	OPTION(bool, resolver_eyelby_always, false);
	OPTION(float, resolver_eyelby_running_value, false);
	OPTION(bool, resolver_eyelby, false);
	OPTION(int, resolver_eyelby_missed_shots, 0);
	OPTION(bool, resolver_eyelby_after_x_shots, false);

	OPTION(bool, resolver_autoflip, false);
	OPTION(int, resolver_flip_after_missed_shots, 0);
	OPTION(bool, resolver_flip_after_x_shots, false);

	OPTION(ButtonCode_t, hvh_resolver_override_key, BUTTON_CODE_NONE);
	OPTION(ButtonCode_t, hvh_resolver_disable_key, BUTTON_CODE_NONE);
	OPTION(bool, hvh_resolver_disabled_draw, false);
	OPTION(bool, skinchanger_enabled, false);
	OPTION(bool, debug_window, false);
	OPTION(bool, debug_showposes, false);
	OPTION(bool, debug_showactivities, false);
	OPTION(bool, debug_headbox, false);
	OPTION(bool, debug_fliponkey, false);
	OPTION(ButtonCode_t, debug_flipkey, BUTTON_CODE_NONE);

};

extern const char *opt_EspType[];
extern const char *opt_BoundsType[];
extern const char *opt_WeaponBoxType[];
extern const char *opt_GrenadeESPType[];
extern const char *opt_AimHitboxSpot[];
extern const char *opt_GlowStyles[];
extern const char *opt_NightmodeType[];
extern const char *opt_Chams_Local_Mode[];
extern const char *opt_Chams[];
extern const char *opt_Chams_Mat[];
extern const char *opt_AimSpot[];
extern const char *opt_MultiHitboxes[14];
extern const char *opt_AApitch[];
extern const char *opt_AAyaw[];
extern const char *opt_AAyaw_move[];
extern const char *opt_Skynames[];
extern const char *opt_nosmoketype[];
//extern const char *opt_weapon_names[];
extern int realAimSpot[];
extern int realHitboxSpot[];
extern const char *opt_LagCompType[];
extern bool input_shouldListen;
extern ButtonCode_t* input_receivedKeyval;
extern bool menuOpen;

extern Options g_Options;
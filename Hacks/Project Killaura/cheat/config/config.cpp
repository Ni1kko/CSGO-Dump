#include "config.hpp"
#include "options.hpp"
#include "../../helpers/math.hpp"
#include "../../helpers/utils.hpp"
#include "../../helpers/input.hpp"
#include "../../helpers/utils.hpp"
#include "../menu/menu.hpp"

void CConfig::SetupValue(int& value, int def, std::string category, std::string name) { 
	value = def; ints.push_back(new ConfigValue< int >(category, name, &value, def)); 
}
void CConfig::SetupValue(char* value, char* def, std::string category, std::string name) { 
	value = def; chars.push_back(new ConfigValue< char >(category, name, value, *def)); 
}
void CConfig::SetupValue(float& value, float def, std::string category, std::string name) { 
	value = def; floats.push_back(new ConfigValue< float >(category, name, &value, def)); 
}
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name) { 
	value = def; bools.push_back(new ConfigValue< bool >(category, name, &value, def)); 
}
void CConfig::SetupColor(float value[4], std::string name) {
	SetupValue(value[0], value[0], ("color"), name + "_r");
	SetupValue(value[1], value[1], ("color"), name + "_g");
	SetupValue(value[2], value[2], ("color"), name + "_b");
	SetupValue(value[3], value[3], ("color"), name + "_a");
}

void CConfig::SetupLegit() {
	for (auto& [key, val] : weapon_names) {
		SetupValue(Cfg.legitbot_items[key].enabled, false, (val), ("legaim_enb"));
		SetupValue(Cfg.legitbot_items[key].deathmatch, false, (val), ("legaim_dtmh"));
		SetupValue(Cfg.legitbot_items[key].autopistol, false, (val), ("legaim_apist"));
		SetupValue(Cfg.legitbot_items[key].smoke_check, false, (val), ("legaim_sm_ch"));
		SetupValue(Cfg.legitbot_items[key].flash_check, false, (val), ("legaim_fl_ch"));
		SetupValue(Cfg.legitbot_items[key].jump_check, false, (val), ("legaim_jm_check"));
		SetupValue(Cfg.legitbot_items[key].autowall, false, (val), ("legaim_at_wl"));
		SetupValue(Cfg.legitbot_items[key].silent, false, (val), ("legaim_sl"));
		SetupValue(Cfg.legitbot_items[key].autofire, false, (val), ("legaim_afre"));
		SetupValue(Cfg.legitbot_items[key].autofire_key, 1, (val), ("legaim_afre_key"));
		SetupValue(Cfg.legitbot_items[key].on_key, true, (val), ("legaim_on_key"));
		SetupValue(Cfg.legitbot_items[key].key, 1, (val), ("legaim_key"));
		SetupValue(Cfg.legitbot_items[key].rcs, false, (val), ("legaim_rcs"));
		SetupValue(Cfg.legitbot_items[key].rcs_fov_enabled, false, (val), ("legaim_fov_rcs_enb"));
		SetupValue(Cfg.legitbot_items[key].rcs_smooth_enabled, false, (val), ("legaim_smth_rcs_enb"));
		SetupValue(Cfg.legitbot_items[key].autostop, false, ( val ), ("legaim_atst"));
		SetupValue(Cfg.legitbot_items[key].backtrack_time, 0.0f, (val), ("legaim_bk_tme"));
		SetupValue(Cfg.legitbot_items[key].only_in_zoom, false, (val), ("legaim_on_zm"));
		SetupValue(Cfg.legitbot_items[key].aim_type, 1, (val), ("legaim_tpe"));
		SetupValue(Cfg.legitbot_items[key].priority, 0, (val), ("legaim_prior"));
		SetupValue(Cfg.legitbot_items[key].rcs_type, 0, (val), ("legaim_tpe_rcs")); 
		SetupValue(Cfg.legitbot_items[key].hitbox, 0, (val), ("legaim_htbx"));
		SetupValue(Cfg.legitbot_items[key].fov, 0.f, (val), ("legaim_fov"));
		SetupValue(Cfg.legitbot_items[key].silent_fov, 0.f, (val), ("legaim_fov_sl"));
		SetupValue(Cfg.legitbot_items[key].rcs_fov, 0.f, (val), ("legaim_fov_rcs"));
		SetupValue(Cfg.legitbot_items[key].smooth, 1, (val), ("legaim_smth"));
		SetupValue(Cfg.legitbot_items[key].rcs_smooth, 1, (val), ("legaim_smth_rcs"));
		SetupValue(Cfg.legitbot_items[key].shot_delay, 0, (val), ("legaim_sh_del"));
		SetupValue(Cfg.legitbot_items[key].kill_delay, 0, (val), ("legaim_kl_del"));
		SetupValue(Cfg.legitbot_items[key].rcs_x, 100, (val), ("legaim_rcs_pitch"));
		SetupValue(Cfg.legitbot_items[key].rcs_y, 100, (val), ("legaim_rcs_yaw"));
		SetupValue(Cfg.legitbot_items[key].rcs_start, 1, (val), ("legaim_start_rcs"));
		SetupValue(Cfg.legitbot_items[key].min_damage, 1, (val), ("legaim_min_dmg"));
	}
}

void CConfig::SetupVisuals() {
	SetupValue(Cfg.esp_enabled, false, ("visualization"), ("vis_enb"));
	SetupValue(Cfg.esp_enemies_only, false, ("visualization"), ("vis_enm"));
	SetupValue(Cfg.esp_visible_only, false, ("visualization"), ("vis_vis_onl"));
	SetupValue(Cfg.esp_player_skeleton, false, ("visualization"), ("vis_skl"));
	SetupValue(Cfg.esp_player_boxes, false, ("visualization"), ("vis_bx"));
	SetupValue(Cfg.esp_player_boxes_type, 0, ("visualization"), ("vis_bx_tpe"));
	SetupValue(Cfg.boxes_rounding, 0, ("visualization"), ("vis_bx_round"));
	SetupValue(Cfg.esp_player_names, false, ("visualization"), ("vis_nme"));
	SetupValue(Cfg.esp_player_health, false, ("visualization"), ("vis_hlth"));
	SetupValue(Cfg.esp_player_armour, false, ("visualization"), ("vis_arm"));
	SetupValue(Cfg.esp_player_weapons, false, ("visualization"), ("vis_wpn"));
	SetupValue(Cfg.esp_dropped_weapons, false, ("visualization"), ("vis_dr_wnp"));
	SetupValue(Cfg.esp_items, false, ("visualization"), ("vis_it"));
	SetupValue(Cfg.esp_grenade_prediction, false, ("visualization"), ("vis_grnd_prd"));
	SetupValue(Cfg.esp_sounds, false, ("visualization"), ("vis_snd"));
	SetupValue(Cfg.esp_sounds_time, 0.5f, ("visualization"), ("vis_snd_t"));
	SetupValue(Cfg.esp_sounds_radius, 15.0f, ("visualization"), ("vis_snd_rad"));
	SetupValue(Cfg.esp_angle_lines, false, ("visualization"), ("vis_aa_ln"));

	SetupValue(Cfg.aspect_ratio, false, ("visualization"), ("vis_aratio"));
	SetupValue(Cfg.aspect_ratio_value, 0, ("visualization"), ("vis_aratio_val"));

	SetupValue(Cfg.esp_crosshair, false, ("visualization"), ("vis_crs"));
	SetupValue(Cfg.esp_snipercrosshair, false, ("visualization"), ("vis_snp_crs"));
	SetupValue(Cfg.esp_recoilcrosshair, false, ("visualization"), ("vis_rec_crs"));

	SetupValue(Cfg.esp_flags, false, ("visualization"), ("vis_flags"));
	for (int i = 0; i < 7; i++) 
		SetupValue(Cfg.esp_player_flags_selected[i], false, ("visualization"), ("vis_flags_") + std::to_string(i));
	
	SetupValue(Cfg.other_drawfov, false, ("visualization"), ("vis_leg_fov"));
	SetupValue(Cfg.other_no_hands, false, ("visualization"), ("vis_no_hn"));
	SetupValue(Cfg.other_no_smoke, false, ("visualization"), ("vis_no_sm"));
	SetupValue(Cfg.other_no_flash, false, ("visualization"), ("vis_no_fl"));

	SetupValue(Cfg.misc_thirdperson, false, ("visualization"), ("vis_th"));
	SetupValue(Cfg.misc_thirdperson_bind, 0, ("visualization"), ("vis_th_bn"));
	SetupValue(Cfg.misc_thirdperson_dist, 50.f, ("visualization"), ("vis_th_dst"));

	SetupValue(Cfg.misc_nightmode, false, ("visualization"), ("vis_ngt"));

	SetupValue(Cfg.glow_enabled, false, ("visualization"), ("vis_glw_enb"));
	SetupValue(Cfg.glow_enemies_only, false, ("visualization"), ("vis_glw_en"));
	SetupValue(Cfg.glow_visible_only, false, ("visualization"), ("vis_glw_vis"));
	SetupValue(Cfg.glow_style, 0, ("visualization"), ("vis_glw_st"));

	SetupValue(Cfg.chams_player_enabled, false, ("visualization"), ("vis_ch_enb"));
	SetupValue(Cfg.chams_player_enemies_only, false, ("visualization"), ("vis_ch_en"));
	SetupValue(Cfg.chams_player_wireframe, false, ("visualization"), ("vis_ch_wi"));
	SetupValue(Cfg.chams_player_ignorez, false, ("visualization"), ("vis_ch_ig"));

	SetupValue(Cfg.chams_arms_enabled, false, ("visualization"), ("vis_ch_arm_enb"));
	SetupValue(Cfg.chams_arms_rainbow, false, ("visualization"), ("vis_ch_arm_rnb"));

	SetupValue(Cfg.chams_backtrack, 0, ("visualization"), ("vis_ch_bk"));
	SetupValue(Cfg.chams_backtrack_wireframe, false, ("visualization"), ("vis_ch_bk_wf"));
}

void CConfig::SetupMisc() {
	SetupValue(Cfg.misc_bhop, false, ("miscellaneous"), ("msc_bh"));
	SetupValue(Cfg.misc_autostrafe, false, ("miscellaneous"), ("msc_bh_stf"));
	SetupValue(Cfg.misc_edgejump, false, ("miscellaneous"), ("msc_edg"));
	SetupValue(Cfg.misc_edgejump_key, 0, ("miscellaneous"), ("msc_edg_bn"));
	SetupValue(Cfg.misc_spectatorlist, false, ("miscellaneous"), ("msc_sp_ls"));
	SetupValue(Cfg.misc_radar, false, ("miscellaneous"), ("msc_rdr"));
	SetupValue(Cfg.misc_engine_radar, false, ("miscellaneous"), ("msc_eng_rdr"));
	SetupValue(Cfg.misc_showranks, false, ("miscellaneous"), ("msc_rnk"));
	SetupValue(Cfg.misc_autoaccept, false, ("miscellaneous"), ("msc_aacc"));
	SetupValue(Cfg.misc_clantag, false, ("miscellaneous"), ("msc_ctg"));
	SetupValue(Cfg.misc_chat_spam, false, ("miscellaneous"), ("msc_ch_sp"));
	SetupValue(Cfg.misc_radio_spam, false, ("miscellaneous"), ("msc_rd_sp"));
	SetupValue(Cfg.misc_watermark, true, ("miscellaneous"), ("msc_wtr"));
	SetupValue(Cfg.esp_hitmarker, false, ("miscellaneous"), ("msc_htmk"));
	SetupValue(Cfg.misc_desync, 0, ("miscellaneous"), ("msc_dsk_tp"));
	SetupValue(Cfg.misc_desync_bind, 0, ("miscellaneous"), ("msc_dsk_bn"));
	SetupValue(Cfg.misc_override_fov, 90, ("miscellaneous"), ("msc_ovfov"));
	SetupValue(Cfg.misc_viewmodel_fov, 68, ("miscellaneous"), ("msc_vfov"));
	SetupValue(Cfg.misc_leftknife, false, ("miscellaneous"), ("msc_lkn"));
	SetupValue(Cfg.misc_model, false, ("miscellaneous"), ("agnt_ch"));
	SetupValue(Cfg.misc_model_type_ct, 0, ("miscellaneous"), ("agnt_ch_ct"));
	SetupValue(Cfg.misc_model_type_tt, 0, ("miscellaneous"), ("agnt_ch_tt"));
	SetupValue(Cfg.backdrop, false, ("miscellaneous"), ("bck_drp"));

	for (int i = 0; i < 4; i++) 
		SetupValue(Cfg.fps_boost_selected[i], false, ("miscellaneous"), ("fps_boost_") + std::to_string(i));	
}

void CConfig::SetupSkins() {
	for (auto& key : k_weapon_names) {
		SetupValue(Cfg.skins.m_items[key.definition_index].name, "Default", (key.name), ("wpn_skin_nme"));
		SetupValue(Cfg.skins.m_items[key.definition_index].definition_vector_index, 0, (key.name), ("wpn_skin_def_vec_dx"));
		SetupValue(Cfg.skins.m_items[key.definition_index].definition_index, 1, (key.name), ("wpn_skin_def_dx"));
		SetupValue(Cfg.skins.m_items[key.definition_index].paint_kit_vector_index, 0, (key.name), ("wpn_skin_kit_vec_dx"));
		SetupValue(Cfg.skins.m_items[key.definition_index].paint_kit_index, 0, (key.name), ("wpn_skin_kit_dx"));
		SetupValue(Cfg.skins.m_items[key.definition_index].definition_override_vector_index, 0, (key.name), ("wpn_skin_vec_tpe"));
		SetupValue(Cfg.skins.m_items[key.definition_index].definition_override_index, 0, (key.name), ("wpn_skin_tpe"));
		SetupValue(Cfg.skins.m_items[key.definition_index].seed, 0, (key.name), ("wpn_skin_seed"));
		SetupValue(Cfg.skins.m_items[key.definition_index].stat_trak, 0, (key.name), ("wnp_skin_stat_trak"));
		SetupValue(Cfg.skins.m_items[key.definition_index].wear, 0.0f, (key.name), ("wpn_skin_wear"));
	}
}

void CConfig::SetupColors() {
	SetupColor(Cfg.color_esp_ally_visible, "color_teammate_visible");
	SetupColor(Cfg.color_esp_ally_occluded, "color_teammate_invisible");
	SetupColor(Cfg.color_esp_enemy_visible, "color_enemy_visible");
	SetupColor(Cfg.color_esp_enemy_occluded, "color_enemy_invisible");
	SetupColor(Cfg.color_esp_weapons, "color_esp_wpn");
	SetupColor(Cfg.color_esp_item, "color_esp_it");
	SetupColor(Cfg.color_glow_ally, "color_glow_teammate");
	SetupColor(Cfg.color_glow_enemy, "color_glow_enemy");
	SetupColor(Cfg.color_chams_player_ally_visible, "color_chms_teammate_visible");
	SetupColor(Cfg.color_chams_player_ally_occluded, "color_chms_teammate_invisible");
	SetupColor(Cfg.color_chams_player_enemy_visible, "color_chms_enemy_visible");
	SetupColor(Cfg.color_chams_player_enemy_occluded, "color_chms_enemy_invisible");

	SetupColor(Cfg.color_chams_arms, "color_chms_arms");
	SetupColor(Cfg.color_backtrack, "color_backtrack");
	SetupColor(Cfg.color_grenade_prediction, "color_grnade_pred");
	SetupColor(Cfg.color_esp_sounds, "color_snd");
	SetupColor(Cfg.color_esp_crosshair, "color_crs");
	SetupColor(Cfg.color_menu, "color_menu");
}

void CConfig::Setup() {
	CreateDirectoryA(u8"C:\\killaura", NULL); // 100p cheat location not detected (rename this crap its cringe)
	CConfig::SetupLegit();
	CConfig::SetupVisuals();
	CConfig::SetupMisc();
	CConfig::SetupSkins();
	CConfig::SetupColors();
}

void CConfig::Save(const std::string& name) {
	if (name.empty()) return;

	std::string file = u8"C:\\killaura" + name;

	for (auto value : ints) 
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	
	for (auto value : floats) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	for (auto value : bools) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(const std::string& name) {
	if (name.empty()) return;

	g_ClientState->ForceFullUpdate();

	std::string file = u8"C:\\killaura" + name;

	char value_l[32] = { '\0' };
	for (auto value : ints) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0", value_l, 32, file.c_str()); *value->value = atoi(value_l);
	}

	for (auto value : floats) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0.0f", value_l, 32, file.c_str()); *value->value = atof(value_l);
	}

	for (auto value : bools) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "false", value_l, 32, file.c_str()); *value->value = !strcmp(value_l, "true");
	}
}

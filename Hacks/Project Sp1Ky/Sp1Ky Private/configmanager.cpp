#include "configmanager.h"
#include "options.hpp"
#include "valve_sdk\misc\Color.hpp"
void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}
void CConfig::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(category, name, &value));
}
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(category, name, &value));
}
void CConfig::SetupValueC(Color &value, Color def, std::string category, std::string name)
{
}
void CConfig::Setup()
{
	SetupValue(g_Options.legit_enabled_esp, false, "ESP", "legit_enabled_esp");
	SetupValue(g_Options.legit_only_enemy, false, "ESP", "legit_only_enemy");
	SetupValue(g_Options.legit_box_toggle, false, "ESP", "legit_box_toggle");
	SetupValue(g_Options.legit_playa_name, false, "ESP", "legit_playa_name");
	SetupValue(g_Options.legit_playa_heal, false, "ESP", "legit_playa_heal");
	SetupValue(g_Options.legit_playa_arm, false, "ESP", "legit_playa_arm");
//	SetupValue(g_Options.legit_playa_weap, false, "ESP", "legit_playa_weap");
	SetupValue(g_Options.esp_player_snaplines, false, "ESP", "esp_player_snaplines");
	SetupValue(g_Options.legit_self_cros, false, "ESP", "legit_self_cros");
	SetupValue(g_Options.legit_playa_dro_wep, false, "ESP", "legit_playa_dro_wep");
	SetupValue(g_Options.legit_playa_def, false, "ESP", "legit_playa_def");
	SetupValue(g_Options.legit_dog_bom, false, "ESP", "legit_dog_bom");
	//CHAMS
	SetupValue(g_Options.legit_dog_cham_enb, false, "CHAMS", "legit_dog_cham_enb");
	SetupValue(g_Options.legit_dog_cham_only_en, false, "CHAMS", "legit_dog_cham_only_en");
	SetupValue(g_Options.legit_dog_kostickovany, false, "CHAMS", "legit_dog_kostickovany");
	SetupValue(g_Options.legit_dog_flat, false, "CHAMS", "legit_dog_flat");
	SetupValue(g_Options.legit_dog_ignz, false, "CHAMS", "legit_dog_ignz");
	SetupValue(g_Options.legit_dog_sklicko, false, "CHAMS", "legit_dog_sklicko");
	SetupValue(g_Options.legit_chsm_amr_en, false, "CHAMS", "legit_chsm_amr_en");
	SetupValue(g_Options.legit_chsm_amr_kostickovany, false, "CHAMS", "legit_chsm_amr_kostickovany");
	SetupValue(g_Options.legit_chsm_amr_fl, false, "CHAMS", "legit_chsm_amr_fl");
	SetupValue(g_Options.legit_chsm_amr_igz, false, "CHAMS", "legit_chsm_amr_igz");
	SetupValue(g_Options.legit_chsm_amr_sklicko, false, "CHAMS", "legit_chsm_amr_sklicko");
	//	MISC
	SetupValue(g_Options.more_more_mi_b_hop, false, "MISC", "more_more_mi_b_hop");
	SetupValue(g_Options.misc_no_hands, false, "MISC", "misc_no_hands");
	SetupValue(g_Options.viewmodel_fov, 68, "MISC", "viewmodel_fov");
	SetupValue(g_Options.fake_angle_chams, false, "MISC", "fake_angle_chams");
	SetupValue(g_Options.nightmode, false, "MISC", "nightmode");
	//Colors
	SetupValueC(g_Options.legit_dog_dog_vis_col_tm, Color(0, 128, 255), "COLOR", "legit_dog_dog_vis_col_tm");
	SetupValueC(g_Options.legit_dog_dog_vis_col_en_vis, Color(255, 0, 0), "COLOR", "legit_dog_dog_vis_col_en_vis");
	SetupValueC(g_Options.legit_dog_dog_vis_col_tm_vis_cl, Color(0, 128, 255), "COLOR", "color_esp_ally_accluded");
	SetupValueC(g_Options.legit_dog_dog_vis_col_entm_invs, Color(255, 0, 0), "COLOR", "legit_dog_dog_vis_col_entm_invs");
	SetupValueC(g_Options.legit_dog_dog_dog_cr_col, Color(255, 255, 255), "COLOR", "legit_dog_dog_dog_cr_col");
	SetupValueC(g_Options.legit_pes_dog_wep, Color(128, 0, 255), "COLOR", "legit_pes_dog_wep");
	SetupValueC(g_Options.legit_col_def_kyt, Color(0, 128, 255), "COLOR", "legit_col_def_kyt");
	SetupValueC(g_Options.legit_col_pl_c_bo, Color(255, 255, 0), "COLOR", "legit_col_pl_c_bo");
	SetupValueC(g_Options.legit_dog_cham_tm_vis, Color(0, 128, 255), "COLOR", "legit_dog_cham_tm_vis");
	SetupValueC(g_Options.legit_dog_cham_tm_invs, Color(0, 255, 128), "COLOR", "legit_dog_cham_tm_invs");
	SetupValueC(g_Options.legit_dog_cham_entm_vis, Color(255, 0, 0), "COLOR", "legit_dog_cham_entm_vis");
	SetupValueC(g_Options.legit_dog_cham_entm_invis, Color(255, 128, 0), "COLOR", "color_chams_player_enemy_accluded");
	SetupValueC(g_Options.cl_legit_chsm_amr_visvis, Color(0, 128, 255), "COLOR", "cl_legit_chsm_amr_visvis");
	SetupValueC(g_Options.clcl_legit_chsm_amr_invs, Color(0, 128, 255), "COLOR", "clcl_legit_chsm_amr_invs");
}




























































































































































































































































































































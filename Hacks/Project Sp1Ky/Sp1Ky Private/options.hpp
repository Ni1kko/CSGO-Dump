#pragma once
#include <string>
#include <map>
#include "valve_sdk/Misc/Color.hpp"
#include "valve_sdk/kit_parser.hpp"
#include "valve_sdk/item_definitions.hpp"
#include <limits>
#include <chrono>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <set>
#include "../Sp1Ky Private/IInputSystem.hpp"
#define OPTION(type, var, val) type var = val
struct item_setting
{
	char name[32] = "Default";
	bool enabled = false;
	int definition_vector_index = 0;
	int definition_index = 1;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	float wear = 0.0000000001;
	char custom_name[32] = "Sp1Ky";
};
class Config
{
public:
	int Hitsound = 1;
	OPTION(int, selected_config_index, 0);
    // 
    // ESP
	//
    // 
    OPTION(bool, legit_enabled_esp, false);
    OPTION(bool, legit_only_enemy, false);
    OPTION(bool, legit_box_toggle, false);
    OPTION(bool, legit_playa_name, false);
    OPTION(bool, legit_playa_heal, false);
    OPTION(bool, legit_playa_arm, false);
    //OPTION(bool, legit_playa_weap, false);
    OPTION(bool, esp_player_snaplines, false);
    OPTION(bool, legit_self_cros, false);
    OPTION(bool, legit_playa_dro_wep, false);
    OPTION(bool, legit_playa_def, false);
    OPTION(bool, legit_dog_bom, false);
	OPTION(bool, more_more_more_bektrkr_ske__Lt, false);
	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_players, false);
	float glow_player_color_t[4] = { 1.f, 0.f, 0.3f, 1.f };
	float glow_player_color_ct[4] = { 0.f, 0.2f, 1.f, 1.f };
	float glow_player_color_t_visible[4] = { 1.f, 1.f, 0.0f, 1.f };
	float glow_player_color_ct_visible[4] = { 0.f, 0.7f, 1.f, 0.85f };
	OPTION(int, glow_players_style, 0);
	OPTION(bool, glow_others, false);
	OPTION(int, glow_others_style, 0);
    //
    // CHAMS
    //
    OPTION(bool, legit_dog_cham_enb, false);
    OPTION(bool, legit_dog_cham_only_en, false);
    OPTION(bool, legit_dog_kostickovany, false);
    OPTION(bool, legit_dog_flat, false);
    OPTION(bool, legit_dog_ignz, false);
    OPTION(bool, legit_dog_sklicko, false);
    OPTION(bool, legit_chsm_amr_en, false);
    OPTION(bool, legit_chsm_amr_kostickovany, false);
    OPTION(bool, legit_chsm_amr_fl, false);
    OPTION(bool, legit_chsm_amr_igz, false);
    OPTION(bool, legit_chsm_amr_sklicko, false);
    //
    // MISC
    //
    OPTION(bool, more_more_mi_b_hop, false);
	OPTION(bool, more_more_mi_a_str_ejf, false);
    OPTION(bool, misc_no_hands, false);
	OPTION(bool, misc_thirdperson, false);
	OPTION(bool, more_more_ani_mejtid_klentek, false);
	OPTION(bool, visuals_others_grenade_pred, false);
	OPTION(float, misc_thirdperson_dist, 90.f);
    OPTION(int, viewmodel_fov, 68);
	OPTION(bool, fake_angle_chams, false);
	OPTION(bool, more_more_more_bu_ll_ert_bejmz, false);
	OPTION(bool, more_more_more_bu_ll_ert_rejsr, false);
	OPTION(int, misc_bullettracers_type, 0);
	OPTION(float, esp_bullettrace_length, 1.5f);
	OPTION(float, esp_bulletbeams_length, 1.5f);
	OPTION(bool, nightmode, false);
	OPTION(bool, total_total_l_m_exi__kero, false);
	OPTION(bool, more_more_mi_hejt_merkr, false);
	OPTION(bool, more_more_ara_spemrkk, false);
	OPTION(bool, more_more_spe_tcetrs, false);
	OPTION(bool, watermark, false);

	float sky_color[3] = { 0.15f, 0.15f, 0.15f };
	float nightmode_intensity = 0.05f;
	float misc_bullettracers_color[4] = { 0.65f, 0.11f, 0.02f, 1.f };
    // 
    // COLORS
    // 
    OPTION(Color, legit_dog_dog_vis_col_tm, Color(0, 128, 255));
    OPTION(Color, legit_dog_dog_vis_col_en_vis, Color(255, 0, 0));
    OPTION(Color, legit_dog_dog_vis_col_tm_vis_cl, Color(0, 128, 255));
    OPTION(Color, legit_dog_dog_vis_col_entm_invs, Color(255, 0, 0));
    OPTION(Color, legit_dog_dog_dog_cr_col, Color(255, 255, 255));
    OPTION(Color, legit_pes_dog_wep, Color(128, 0, 128));
    OPTION(Color, legit_col_def_kyt, Color(0, 128, 255));
    OPTION(Color, legit_col_pl_c_bo, Color(255, 255, 0));
    OPTION(Color, legit_dog_cham_tm_vis, Color(0, 128, 255));
    OPTION(Color, legit_dog_cham_tm_invs, Color(0, 255, 128));
    OPTION(Color, legit_dog_cham_entm_vis, Color(255, 0, 0));
    OPTION(Color, legit_dog_cham_entm_invis, Color(255, 128, 0));
    OPTION(Color, cl_legit_chsm_amr_visvis, Color(0, 128, 255));
    OPTION(Color, clcl_legit_chsm_amr_invs, Color(0, 128, 255));
	OPTION(Color, color_esp_bullet_beams, Color(12, 56, 214));
	OPTION(bool, total_total_l_enabled, false);
	OPTION(bool, total_total_l_r____cs, false);
	OPTION(int, total_total_l_topka_hit_bejx, 0);
	OPTION(int, total_total_l_vymrdanejzasobnik, 3);
	OPTION(int, total_total_l_tdolkaa_hit_dolni_bejk, 0);
	OPTION(float, total_total_l_zmensimtitozatebe_sm_fct, 5.f);
	OPTION(float, total_total_l_ejfouvykkk, 2.f);
	struct {
		std::map<int, item_setting> m_items;
		std::unordered_map<std::string, std::string> m_icon_overrides;
		auto get_icon_override(const std::string original) const -> const char*
		{
			return m_icon_overrides.count(original) ? m_icon_overrides.at(original).data() : nullptr;
		}
	} skins;
};
extern bool input_shouldListen;
extern const char *opt_AimSpot[];
extern int realAimSpot[];
extern const char *opt_GlowStyles[];
extern int realHitboxSpot[];
extern const char *opt_AimHitboxSpot[];
extern const char *opt_MultiHitboxes[14];
extern Config g_Options;
extern bool   g_Unload;
extern bool   g_Save;
extern bool   g_Load;
extern bool   g_Clear;
extern bool   g_View;
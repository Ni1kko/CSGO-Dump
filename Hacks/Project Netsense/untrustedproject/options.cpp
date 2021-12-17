#include "options.hpp"
#include "valve_sdk/sdk.hpp"
#include "valve_sdk/csgostructs.hpp"

namespace values {
	const char* box_types[] = {
		"Default",
		"Corners"
	};
	const char* bind_types[] = {
		"Hold",
		"Toggle",
		"On",
		"Off"
	};
	const char* aim_types[] = {
		"Hitbox",
		"Nearest"
	};

	const char* menu_styles[] = {
		"Black",
		"White",
		"Dark"
	};

	const char* smooth_types[] = {
		"Slow near target",
		"Linear"
	};
	const char* hitbox_list[] = {
		"Head",
		"Neck",
		"Pelvis",
		"Stomach",
		"Lower chest",
		"Chest",
		"Upper chest",
	};
	const char* rcs_types[] = {
		"Standalone",
		"Aim"
	};
	const char* priorities[] = {
		"FOV",
		"Health",
		"Damage",
		"Distance"
	};
	const char* fov_types[] = {
		"Static",
		"Dynamic"
	};
	const char* show_spread_types[] = {
		"Color",
		"Rainbow"
	};
	const char* knifebot_filters[] = {
		"All",
		"Team",
		"Enemy"
	};
	const char* knifebot_modes[] = {
		"Auto",
		"Left, Right",
		"Right",
		"Left"
	};
	const char* glow_modes[] = {
		"Default",
		"Pulsing full",
		"Obvodka",
		"Obvodka pulsing"
	};
	const char* legitaa_modes[] = {
		"Balance"

	};
	const char* resolver_modes[] = {
		"Desync"
	};
	const char* soundesp_modes[] = {
		"Render",
		"Beams"
	};

	std::vector< weapon_name_t> WeaponNames =
	{
	{ WEAPON_AK47, "AK-47" },
	{ WEAPON_AUG, "AUG" },
	{ WEAPON_AWP, "AWP" },
	{ WEAPON_CZ75A, "CZ75 Auto" },
	{ WEAPON_DEAGLE, "Desert Eagle" },
	{ WEAPON_ELITE, "Dual Berettas" },
	{ WEAPON_FAMAS, "FAMAS" },
	{ WEAPON_FIVESEVEN, "Five-SeveN" },
	{ WEAPON_G3SG1, "G3SG1" },
	{ WEAPON_GALILAR, "Galil AR" },
	{ WEAPON_GLOCK, "Glock-18" },
	{ WEAPON_M249, "M249" },
	{ WEAPON_M4A1_SILENCER, "M4A1-S" },
	{ WEAPON_M4A1, "M4A4" },
	{ WEAPON_MAC10, "MAC-10" },
	{ WEAPON_MAG7, "MAG-7" },
	{ WEAPON_MP7, "MP7" },
	{ WEAPON_MP5, "MP5" },
	{ WEAPON_MP9, "MP9" },
	{ WEAPON_NEGEV, "Negev" },
	{ WEAPON_NOVA, "Nova" },
	{ WEAPON_HKP2000, "P2000" },
	{ WEAPON_P250, "P250" },
	{ WEAPON_P90, "P90" },
	{ WEAPON_BIZON, "PP-Bizon" },
	{ WEAPON_REVOLVER, "R8 Revolver" },
	{ WEAPON_SAWEDOFF, "Sawed-Off" },
	{ WEAPON_SCAR20, "SCAR-20" },
	{ WEAPON_SSG08, "SSG 08" },
	{ WEAPON_SG556, "SG 553" },
	{ WEAPON_TEC9, "Tec-9" },
	{ WEAPON_UMP45, "UMP-45" },
	{ WEAPON_USP_SILENCER, "USP-S" },
	{ WEAPON_XM1014, "XM1014" },
	};

	std::vector< weapon_name_t> WeaponNamesFull =
	{
	{WEAPON_KNIFE, "Knife"},
	{GLOVE_T_SIDE, "Glove"},
	{ WEAPON_AK47, "AK-47" },
	{ WEAPON_AUG, "AUG" },
	{ WEAPON_AWP, "AWP" },
	{ WEAPON_CZ75A, "CZ75 Auto" },
	{ WEAPON_DEAGLE, "Desert Eagle" },
	{ WEAPON_ELITE, "Dual Berettas" },
	{ WEAPON_FAMAS, "FAMAS" },
	{ WEAPON_FIVESEVEN, "Five-SeveN" },
	{ WEAPON_G3SG1, "G3SG1" },
	{ WEAPON_GALILAR, "Galil AR" },
	{ WEAPON_GLOCK, "Glock-18" },
	{ WEAPON_M249, "M249" },
	{ WEAPON_M4A1_SILENCER, "M4A1-S" },
	{ WEAPON_M4A1, "M4A4" },
	{ WEAPON_MAC10, "MAC-10" },
	{ WEAPON_MAG7, "MAG-7" },
	{ WEAPON_MP7, "MP7" },
	{ WEAPON_MP5, "MP5" },
	{ WEAPON_MP9, "MP9" },
	{ WEAPON_NEGEV, "Negev" },
	{ WEAPON_NOVA, "Nova" },
	{ WEAPON_HKP2000, "P2000" },
	{ WEAPON_P250, "P250" },
	{ WEAPON_P90, "P90" },
	{ WEAPON_BIZON, "PP-Bizon" },
	{ WEAPON_REVOLVER, "R8 Revolver" },
	{ WEAPON_SAWEDOFF, "Sawed-Off" },
	{ WEAPON_SCAR20, "SCAR-20" },
	{ WEAPON_SSG08, "SSG 08" },
	{ WEAPON_SG556, "SG 553" },
	{ WEAPON_TEC9, "Tec-9" },
	{ WEAPON_UMP45, "UMP-45" },
	{ WEAPON_USP_SILENCER, "USP-S" },
	{ WEAPON_XM1014, "XM1014" },
	};

	const std::map<size_t, weapon_info_t> WeaponInfo =
	{
		{WEAPON_KNIFE,{"models/weapons/v_knife_default_ct.mdl", "knife_default_ct"}},
		{WEAPON_KNIFE_T,{"models/weapons/v_knife_default_t.mdl", "knife_t"}},
		{WEAPON_KNIFE_BAYONET, {"models/weapons/v_knife_bayonet.mdl", "bayonet"}},
		{WEAPON_KNIFE_FLIP, {"models/weapons/v_knife_flip.mdl", "knife_flip"}},
		{WEAPON_KNIFE_GUT, {"models/weapons/v_knife_gut.mdl", "knife_gut"}},
		{WEAPON_KNIFE_KARAMBIT, {"models/weapons/v_knife_karam.mdl", "knife_karambit"}},
		{WEAPON_KNIFE_M9_BAYONET, {"models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet"}},
		{WEAPON_KNIFE_TACTICAL, {"models/weapons/v_knife_tactical.mdl", "knife_tactical"}},
		{WEAPON_KNIFE_FALCHION, {"models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion"}},
		{WEAPON_KNIFE_SURVIVAL_BOWIE, {"models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie"}},
		{WEAPON_KNIFE_BUTTERFLY, {"models/weapons/v_knife_butterfly.mdl", "knife_butterfly"}},
		{WEAPON_KNIFE_PUSH, {"models/weapons/v_knife_push.mdl", "knife_push"}},
		{WEAPON_KNIFE_URSUS,{"models/weapons/v_knife_ursus.mdl", "knife_ursus"}},
		{WEAPON_KNIFE_GYPSY_JACKKNIFE,{"models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife"}},
		{WEAPON_KNIFE_STILETTO,{"models/weapons/v_knife_stiletto.mdl", "knife_stiletto"}},
		{WEAPON_KNIFE_WIDOWMAKER,{"models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker"}},
		{WEAPON_KNIFE_CSS,{"models/weapons/v_knife_css.mdl", "knife_css"}},
		{WEAPON_KNIFE_SKELETON,{"models/weapons/v_knife_skeleton.mdl", "knife_skeleton"}},
		{WEAPON_KNIFE_OUTDOOR,{"models/weapons/v_knife_outdoor.mdl", "knife_outdoor"}},
		{WEAPON_KNIFE_CANIS, {"models/weapons/v_knife_canis.mdl", "knife_canis"}},
		{WEAPON_KNIFE_CORD, {"models/weapons/v_knife_cord.mdl", "knife_cord"}},
		{GLOVE_STUDDED_BLOODHOUND,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"}},
		{GLOVE_T_SIDE,{"models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl"}},
		{GLOVE_CT_SIDE,{"models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl"}},
		{GLOVE_SPORTY,{"models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"}},
		{GLOVE_SLICK,{"models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"}},
		{GLOVE_LEATHER_WRAP,{"models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"}},
		{GLOVE_MOTORCYCLE,{"models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"}},
		{GLOVE_SPECIALIST,{"models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"}},
		{GLOVE_HYDRA,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"}}
	};

	std::map<short, std::string> WNames = {
		{ WEAPON_AK47, "AK-47" },
		{ WEAPON_AUG, "AUG" },
		{ WEAPON_AWP, "AWP" },
		{ WEAPON_CZ75A, "CZ75 Auto" },
		{ WEAPON_DEAGLE, "Desert Eagle" },
		{ WEAPON_ELITE, "Dual Berettas" },
		{ WEAPON_FAMAS, "FAMAS" },
		{ WEAPON_FIVESEVEN, "Five-SeveN" },
		{ WEAPON_G3SG1, "G3SG1" },
		{ WEAPON_GALILAR, "Galil AR" },
		{ WEAPON_GLOCK, "Glock-18" },
		{ WEAPON_M249, "M249" },
		{ WEAPON_M4A1_SILENCER, "M4A1-S" },
		{ WEAPON_M4A1, "M4A4" },
		{ WEAPON_MAC10, "MAC-10" },
		{ WEAPON_MAG7, "MAG-7" },
		{ WEAPON_MP7, "MP7" },
		{ WEAPON_MP5, "MP5" },
		{ WEAPON_MP9, "MP9" },
		{ WEAPON_NEGEV, "Negev" },
		{ WEAPON_NOVA, "Nova" },
		{ WEAPON_HKP2000, "P2000" },
		{ WEAPON_P250, "P250" },
		{ WEAPON_P90, "P90" },
		{ WEAPON_BIZON, "PP-Bizon" },
		{ WEAPON_REVOLVER, "R8 Revolver" },
		{ WEAPON_SAWEDOFF, "Sawed-Off" },
		{ WEAPON_SCAR20, "SCAR-20" },
		{ WEAPON_SSG08, "SSG 08" },
		{ WEAPON_SG556, "SG 553" },
		{ WEAPON_TEC9, "Tec-9" },
		{ WEAPON_UMP45, "UMP-45" },
		{ WEAPON_USP_SILENCER, "USP-S" },
		{ WEAPON_XM1014, "XM1014" }
	};

	const std::vector<weapon_name_t> KnifeNames =
	{
		{0, "Default"},
		{WEAPON_KNIFE_BAYONET, "Bayonet"},
		{WEAPON_KNIFE_CSS, "Classic Knife"},
		{WEAPON_KNIFE_CORD, "Cord Knife"},
		{WEAPON_KNIFE_CANIS, "Canis Knife"},
		{WEAPON_KNIFE_OUTDOOR, "Outdoor Knife"},
		{WEAPON_KNIFE_SKELETON, "Skeleton Knife"},
		{WEAPON_KNIFE_FLIP, "Flip Knife"},
		{WEAPON_KNIFE_GUT, "Gut Knife"},
		{WEAPON_KNIFE_KARAMBIT, "Karambit"},
		{WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet"},
		{WEAPON_KNIFE_TACTICAL, "Huntsman Knife"},
		{WEAPON_KNIFE_FALCHION, "Falchion Knife"},
		{WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife"},
		{WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife"},
		{WEAPON_KNIFE_PUSH, "Shadow Daggers"},
		{WEAPON_KNIFE_URSUS, "Ursus Knife"},
		{WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife"},
		{WEAPON_KNIFE_STILETTO, "Stiletto Knife"},
		{WEAPON_KNIFE_WIDOWMAKER, "Talon Knife"}
	};

	const std::vector<weapon_name_t> GloveNames =
	{
		{0, "Default"},
		{GLOVE_STUDDED_BLOODHOUND, "Bloodhound"},
		{GLOVE_T_SIDE, "Default (Terrorists)"},
		{GLOVE_CT_SIDE, "Default (Counter-Terrorists)"},
		{GLOVE_SPORTY, "Sporty"},
		{GLOVE_SLICK, "Slick"},
		{GLOVE_LEATHER_WRAP, "Handwrap"},
		{GLOVE_MOTORCYCLE, "Motorcycle"},
		{GLOVE_SPECIALIST, "Specialist"},
		{GLOVE_HYDRA, "Hydra"}
	};


	const char* GetWeaponNameById(int id)
	{
		switch (id)
		{
		case 1:
			return "deagle";
		case 2:
			return "elite";
		case 3:
			return "fiveseven";
		case 4:
			return "glock";
		case 7:
			return "ak47";
		case 8:
			return "aug";
		case 9:
			return "awp";
		case 10:
			return "famas";
		case 11:
			return "g3sg1";
		case 13:
			return "galilar";
		case 14:
			return "m249";
		case 60:
			return "m4a1_silencer";
		case 16:
			return "m4a1";
		case 17:
			return "mac10";
		case 19:
			return "p90";
		case 23:
			return "mp5sd";
		case 24:
			return "ump45";
		case 25:
			return "xm1014";
		case 26:
			return "bizon";
		case 27:
			return "mag7";
		case 28:
			return "negev";
		case 29:
			return "sawedoff";
		case 30:
			return "tec9";
		case 32:
			return "hkp2000";
		case 33:
			return "mp7";
		case 34:
			return "mp9";
		case 35:
			return "nova";
		case 36:
			return "p250";
		case 38:
			return "scar20";
		case 39:
			return "sg556";
		case 40:
			return "ssg08";
		case 61:
			return "usp_silencer";
		case 63:
			return "cz75a";
		case 64:
			return "revolver";
		case 508:
			return "knife_m9_bayonet";
		case 500:
			return "bayonet";
		case 505:
			return "knife_flip";
		case 506:
			return "knife_gut";
		case 507:
			return "knife_karambit";
		case 509:
			return "knife_tactical";
		case 512:
			return "knife_falchion";
		case 514:
			return "knife_survival_bowie";
		case 515:
			return "knife_butterfly";
		case 516:
			return "knife_push";

		case 519:
			return "knife_ursus";
		case 520:
			return "knife_gypsy_jackknife";
		case 522:
			return "knife_stiletto";
		case 523:
			return "knife_widowmaker";
		
		case WEAPON_KNIFE_CSS:
			return "knife_css";
		
		case WEAPON_KNIFE_CORD:
			return "knife_cord";

		case WEAPON_KNIFE_CANIS:
			return "knife_canis";
		
		case WEAPON_KNIFE_OUTDOOR:
			return "knife_outdoor";

		case WEAPON_KNIFE_SKELETON:
			return "knife_skeleton";

		case 5027:
			return "studded_bloodhound_gloves";
		case 5028:
			return "t_gloves";
		case 5029:
			return "ct_gloves";
		case 5030:
			return "sporty_gloves";
		case 5031:
			return "slick_gloves";
		case 5032:
			return "leather_handwraps";
		case 5033:
			return "motorcycle_gloves";
		case 5034:
			return "specialist_gloves";
		case 5035:
			return "studded_hydra_gloves";

		default:
			return "";
		}
	}

	int GetKnifeDefinitionIndex(int si)
	{
		return KnifeNames.at(si).definition_index;
	}

	int GetGloveDefinitionIndex(int si)
	{
		return GloveNames.at(si).definition_index;
	}
};

ImVec2 g_ChangeMenuSizeValue = ImVec2(800, 450);
ImVec2 g_MenuSize = ImVec2(800, 450);
bool g_ChangeMenuSize = true;

bool g_Unload = false;

bool g_ShowPrompt;
std::string g_PromptTitle;
std::function<void()> g_PromptCallback;
#pragma once
#include "../features/JSON.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../features/Configs.h"
#include <unordered_map>
#include <array>
#include <algorithm>
#include <fstream>

class pstring : public std::string
{
public:
	pstring() : std::string() {}

	template<typename T>
	pstring(const T v) : std::string(v) {}

	template<typename T>
	pstring& operator<<(const T s)
	{
		std::stringstream stream;
		stream << *this;
		stream << s;
		*this = stream.str();
		return *this;
	}

	pstring& operator+(const unsigned int i)
	{
		std::stringstream stream;
		stream << *this;
		stream << i;
		*this = stream.str();
		return *this;
	}
};

namespace Configs
{
	void DumpClassIDs(const char* fileName)
	{
		auto stream = std::ofstream(fileName);

		stream << "--------------------[ Sub's CLASS ID DUMP ]--------------------" << std::endl;
		stream << std::endl;
		stream << "enum ClassId" << std::endl;
		stream << "{" << std::endl;

		for (ClientClass* DumpClass = g_CHLClient->GetAllClasses(); DumpClass; DumpClass = DumpClass->m_pNext)
			stream << "\tClassId_" << DumpClass->m_pNetworkName << " = " << DumpClass->m_ClassID << "," << std::endl;

		stream << "}" << std::endl;
	}

	void GetVal(Json::Value &config, int* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asInt();
	}

	void GetVal(Json::Value &config, bool* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asBool();
	}

	void GetVal(Json::Value &config, float* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asFloat();
	}

	void GetVal(Json::Value &config, Color* setting)
	{
		if (config.isNull())
			return;

		static int R, G, B, A;

		GetVal(config["R"], &R);
		GetVal(config["G"], &G);
		GetVal(config["B"], &B);
		GetVal(config["A"], &A);

		setting->SetColor(R, G, B, A);
	}

	void GetVal(Json::Value &config, char** setting)
	{
		if (config.isNull())
			return;

		*setting = strdup(config.asCString());
	}

	void GetVal(Json::Value &config, char* setting)
	{
		if (config.isNull())
			return;

		strcpy(setting, config.asCString());
	}

	void LoadColor(Json::Value &config, Color* color)
	{
		static int R, G, B, A;

		color->GetColor(R, G, B, A);

		config["R"] = R;
		config["G"] = G;
		config["B"] = B;
		config["A"] = A;
	}

	void total_sheja_sejvit_kenfig(std::string fileName)
	{
		Json::Value settings;

		settings["Legit"]["Enabled"] = g_Options.total_total_l_enabled;
		settings["Legit"]["Recoil Control System"] = g_Options.total_total_l_r____cs;
		settings["Legit"]["Hitbox Primary"] = g_Options.total_total_l_topka_hit_bejx;
		settings["Legit"]["Aftershots"] = g_Options.total_total_l_vymrdanejzasobnik;
		settings["Legit"]["Hitbox Secondary"] = g_Options.total_total_l_tdolkaa_hit_dolni_bejk;
		settings["Legit"]["Smooth"] = g_Options.total_total_l_zmensimtitozatebe_sm_fct;
		settings["Legit"]["Fov"] = g_Options.total_total_l_ejfouvykkk;
		settings["Legit"]["Backtrack"] = g_Options.total_total_l_m_exi__kero;

		settings["Visuals"]["Player ESP"] = g_Options.legit_enabled_esp;
		settings["Visuals"]["Enemies Only ESP"] = g_Options.legit_only_enemy;
		settings["Visuals"]["Player Boxes"] = g_Options.legit_box_toggle;
		settings["Visuals"]["Player Names"] = g_Options.legit_playa_name;
		settings["Visuals"]["Player Health"] = g_Options.legit_playa_heal;
		settings["Visuals"]["Player Armor"] = g_Options.legit_playa_arm;
//		settings["Visuals"]["Player Weapons"] = g_Options.legit_playa_weap;
		settings["Visuals"]["Player Snaplines"] = g_Options.esp_player_snaplines;
		settings["Visuals"]["Crosshair"] = g_Options.legit_self_cros;
		settings["Visuals"]["Dropped Weapons ESP"] = g_Options.legit_playa_dro_wep;
		settings["Visuals"]["Defuse Kit ESP"] = g_Options.legit_playa_def;
		settings["Visuals"]["Planted C4 ESP"] = g_Options.legit_dog_bom;
		settings["Visuals"]["Chams Players"] = g_Options.legit_dog_cham_enb;
		settings["Visuals"]["Chams Enemies Only"] = g_Options.legit_dog_cham_only_en;
		settings["Visuals"]["Chams Player Wireframe"] = g_Options.legit_dog_kostickovany;
		settings["Visuals"]["Chams Arms"] = g_Options.legit_chsm_amr_en;
		settings["Visuals"]["Chams Arms Wireframe"] = g_Options.legit_chsm_amr_kostickovany;
		settings["Visuals"]["No Hands"] = g_Options.misc_no_hands;
		settings["Visuals"]["NightMode"] = g_Options.nightmode;
		settings["Visuals"]["Viewmodel FOV"] = g_Options.viewmodel_fov;

		settings["Misc"]["Show Backtrack"] = g_Options.more_more_more_bektrkr_ske__Lt;
		settings["Misc"]["Bullet Beams"] = g_Options.more_more_more_bu_ll_ert_bejmz;
		settings["Misc"]["Bullet Tracer"] = g_Options.more_more_more_bu_ll_ert_rejsr;
		settings["Misc"]["Hitmarker"] = g_Options.more_more_mi_hejt_merkr;
		settings["Misc"]["Bunny Hop"] = g_Options.more_more_mi_b_hop;
		settings["Misc"]["NightMode"] = g_Options.nightmode;
		settings["Misc"]["Auto Strafe"] = g_Options.more_more_mi_a_str_ejf;
		settings["Skins"]["Skins Changer"] = g_Options.total_total_l_ejfouvykkk;



		LoadColor(settings["Colors"]["Ally Visible Color"], &g_Options.legit_dog_dog_vis_col_tm);
		LoadColor(settings["Colors"]["Ally Occluded Color"], &g_Options.legit_dog_dog_vis_col_tm_vis_cl);
		LoadColor(settings["Colors"]["Enemy Visible Color"], &g_Options.legit_dog_dog_vis_col_en_vis);
		LoadColor(settings["Colors"]["Enemy Occluded Color"], &g_Options.legit_dog_dog_vis_col_entm_invs);
		LoadColor(settings["Colors"]["Weapon ESP Color"], &g_Options.legit_pes_dog_wep);
		LoadColor(settings["Colors"]["Defuse Kit ESP Color"], &g_Options.legit_col_def_kyt);
		LoadColor(settings["Colors"]["C4 ESP Color"], &g_Options.legit_col_pl_c_bo);
		LoadColor(settings["Colors"]["Ally Chams Visible Color"], &g_Options.legit_dog_cham_tm_vis);
		LoadColor(settings["Colors"]["Ally Chams Occluded Color"], &g_Options.legit_dog_cham_tm_invs);
		LoadColor(settings["Colors"]["Enemy Chams Visible Color"], &g_Options.legit_dog_cham_entm_vis);
		LoadColor(settings["Colors"]["Enemy Chams Occluded Color"], &g_Options.legit_dog_cham_entm_invis);
		LoadColor(settings["Colors"]["Arm Chams Visible Color"], &g_Options.cl_legit_chsm_amr_visvis);
		LoadColor(settings["Colors"]["Arm Chams Occluded Color"], &g_Options.clcl_legit_chsm_amr_invs);

		Json::StyledWriter styledWriter;
		std::string strJson = styledWriter.write(settings);
		FILE* file = fopen(fileName.c_str(), "w+");
		if (file)
		{
			fwrite(strJson.c_str(), 1, strJson.length(), file);
			fclose(file);
		}
	}

	void total_sheja_loud_kenfig(std::string fileName)
	{
		FILE* infile = fopen(fileName.c_str(), "r");

		if (!infile)
		{
			total_sheja_sejvit_kenfig(fileName);
			return;
		}

		fseek(infile, 0, SEEK_END);
		long filesize = ftell(infile);
		char* buf = new char[filesize + 1];
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, filesize, infile);
		fclose(infile);

		buf[filesize] = '\0';
		std::stringstream ss;
		ss.str(buf);
		delete[] buf;

		Json::Value settings;
		ss >> settings;

		GetVal(settings["Legit"]["Enabled"], &g_Options.total_total_l_enabled);
		GetVal(settings["Legit"]["Recoil Control System"], &g_Options.total_total_l_r____cs);
		GetVal(settings["Legit"]["Hitbox Primary"], &g_Options.total_total_l_topka_hit_bejx);
		GetVal(settings["Legit"]["Aftershots"], &g_Options.total_total_l_vymrdanejzasobnik);
		GetVal(settings["Legit"]["Hitbox Secondary"], &g_Options.total_total_l_tdolkaa_hit_dolni_bejk);
		GetVal(settings["Legit"]["Smooth"], &g_Options.total_total_l_zmensimtitozatebe_sm_fct);
		GetVal(settings["Legit"]["Fov"], &g_Options.total_total_l_ejfouvykkk);
		GetVal(settings["Legit"]["Backtrack"], &g_Options.total_total_l_m_exi__kero);

		GetVal(settings["Visuals"]["Player ESP"], &g_Options.legit_enabled_esp);
		GetVal(settings["Visuals"]["Enemies Only ESP"], &g_Options.legit_only_enemy);
		GetVal(settings["Visuals"]["Player Boxes"], &g_Options.legit_box_toggle);
		GetVal(settings["Visuals"]["Player Names"], &g_Options.legit_playa_name);
		GetVal(settings["Visuals"]["Player Health"], &g_Options.legit_playa_heal);
		GetVal(settings["Visuals"]["Player Armor"], &g_Options.legit_playa_arm);
//		GetVal(settings["Visuals"]["Player Weapons"], &g_Options.legit_playa_weap);
		GetVal(settings["Visuals"]["Player Snaplines"], &g_Options.esp_player_snaplines);
		GetVal(settings["Visuals"]["Crosshair"], &g_Options.legit_self_cros);
		GetVal(settings["Visuals"]["Dropped Weapons ESP"], &g_Options.legit_playa_dro_wep);
		GetVal(settings["Visuals"]["Defuse Kit ESP"], &g_Options.legit_playa_def);
		GetVal(settings["Visuals"]["Planted C4 ESP"], &g_Options.legit_dog_bom);
		GetVal(settings["Visuals"]["Chams Players"], &g_Options.legit_dog_cham_enb);
		GetVal(settings["Visuals"]["Chams Enemies Only"], &g_Options.legit_dog_cham_only_en);
		GetVal(settings["Visuals"]["Chams Player Wireframe"], &g_Options.legit_dog_kostickovany);
		GetVal(settings["Visuals"]["Chams Arms"], &g_Options.legit_chsm_amr_en);
		GetVal(settings["Visuals"]["Chams Arms Wireframe"], &g_Options.legit_chsm_amr_kostickovany);
		GetVal(settings["Visuals"]["No Hands"], &g_Options.misc_no_hands);
		GetVal(settings["Visuals"]["NightMode"], &g_Options.nightmode);
		GetVal(settings["Visuals"]["Viewmodel FOV"], &g_Options.viewmodel_fov);

		GetVal(settings["Misc"]["Show Backtrack"], &g_Options.more_more_more_bektrkr_ske__Lt);
		GetVal(settings["Misc"]["Bullet Beams"], &g_Options.more_more_more_bu_ll_ert_bejmz);
		GetVal(settings["Misc"]["Bullet Tracer"], &g_Options.more_more_more_bu_ll_ert_rejsr);
		GetVal(settings["Misc"]["Hitmarker"], &g_Options.more_more_mi_hejt_merkr);
		GetVal(settings["Misc"]["Bunny Hop"], &g_Options.more_more_mi_b_hop);
		GetVal(settings["Misc"]["NightMode"], &g_Options.nightmode);
		GetVal(settings["Misc"]["Auto Strafe"], &g_Options.more_more_mi_a_str_ejf);
		GetVal(settings["Skins"]["Skins Changer"], &g_Options.total_total_l_ejfouvykkk);

		GetVal(settings["Colors"]["Ally Visible Color"], &g_Options.legit_dog_dog_vis_col_tm);
		GetVal(settings["Colors"]["Ally Occluded Color"], &g_Options.legit_dog_dog_vis_col_tm_vis_cl);
		GetVal(settings["Colors"]["Enemy Visible Color"], &g_Options.legit_dog_dog_vis_col_en_vis);
		GetVal(settings["Colors"]["Enemy Occluded Color"], &g_Options.legit_dog_dog_vis_col_entm_invs);
		GetVal(settings["Colors"]["Weapon ESP Color"], &g_Options.legit_pes_dog_wep);
		GetVal(settings["Colors"]["Defuse Kit ESP Color"], &g_Options.legit_col_def_kyt);
		GetVal(settings["Colors"]["C4 ESP Color"], &g_Options.legit_col_pl_c_bo);
		GetVal(settings["Colors"]["Ally Chams Visible Color"], &g_Options.legit_dog_cham_tm_vis);
		GetVal(settings["Colors"]["Ally Chams Occluded Color"], &g_Options.legit_dog_cham_tm_invs);
		GetVal(settings["Colors"]["Enemy Chams Visible Color"], &g_Options.legit_dog_cham_entm_vis);
		GetVal(settings["Colors"]["Enemy Chams Occluded Color"], &g_Options.legit_dog_cham_entm_invis);
		GetVal(settings["Colors"]["Arm Chams Visible Color"], &g_Options.cl_legit_chsm_amr_visvis);
		GetVal(settings["Colors"]["Arm Chams Occluded Color"], &g_Options.clcl_legit_chsm_amr_invs);
		g_ClientState->ForceFullUpdate();
	}
}


























































































































































































































































































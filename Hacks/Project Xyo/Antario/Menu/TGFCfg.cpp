#include "TGFCfg.h"
#include "config.h"
#include <Windows.h>
#include <Psapi.h>
#include <lmcons.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <shlobj.h>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <shlwapi.h>
#include <iomanip>
#include <ctime>

/*
Credits: A5
Side Notes: Baby you're cute for letting me use your config shit <3
*/

void CConfig::Setup()
{

	SetupValue(c_config::get().aimbot_enabled, false, "Aimbot", "aimbot_enabled");
	SetupValue(c_config::get().legit_aimbot_backtrack, false, "Aimbot", "legit_aimbot_backtrack");
	SetupValue(c_config::get().legit_aimbot_fov, 0, "Aimbot", "legit_aimbot_fov");
	SetupValue(c_config::get().linear_progression_threshold, 0, "Aimbot", "linear_progression_threshold");
	SetupValue(c_config::get().rcs, false, "Aimbot", "rcs");
	SetupValue(c_config::get().rcs_x, 0, "Aimbot", "rcs_x");
	SetupValue(c_config::get().rcs_y, 0, "Aimbot", "rcs_y");

	SetupValue(c_config::get().aimbot_enabled, false, "Aimbot", "aimbot_enabled");
	SetupValue(c_config::get().aimbot_norecoil, false, "Aimbot", "aimbot_norecoil");
	SetupValue(c_config::get().autostop, false, "Aimbot", "aimbot_autostop");
	SetupValue(c_config::get().autostop_mode, false, "Aimbot", "autostop_mode");

	SetupValue(c_config::get().stop_inbetween_shots, false, "Aimbot", "stop_inbetween_shots");

	
	SetupValue(c_config::get().aimbot_headpointscale, 0, "Aimbot", "aimbot_headpointscale");
	SetupValue(c_config::get().aimbot_bodypointscale, 0, "Aimbot", "aimbot_bodypointscale");
	SetupValue(c_config::get().aimbot_silentaim, false, "Aimbot", "aimbot_silentaim");
	SetupValue(c_config::get().aimbot_resolver, false, "Aimbot", "aimbot_resolver");

	SetupValue(c_config::get().auto_hitchance, false, "Aimbot", "auto_hitchance");
	SetupValue(c_config::get().auto_mindamage, false, "Aimbot", "auto_mindamage");

	SetupValue(c_config::get().scout_hitchance, false, "Aimbot", "scout_hitchance");
	SetupValue(c_config::get().scout_mindamage, false, "Aimbot", "scout_mindamage");

	SetupValue(c_config::get().awp_hitchance, false, "Aimbot", "awp_hitchance");
	SetupValue(c_config::get().awp_mindamage, false, "Aimbot", "awp_mindamage");

	SetupValue(c_config::get().heavy_pistol_hitchance, false, "Aimbot", "heavy_pistol_hitchance");
	SetupValue(c_config::get().heavy_pistol_mindamage, false, "Aimbot", "heavy_pistol_mindamage");

	SetupValue(c_config::get().other_hitchance, false, "Aimbot", "other_hitchance");
	SetupValue(c_config::get().other_mindamage, false, "Aimbot", "other_mindamage");

	SetupValue(c_config::get().antiaim_enabled, false, "Aimbot", "antiaim_enabled");

	SetupValue(c_config::get().pitch_standing, 0, "Aimbot", "pitch_standing");
	SetupValue(c_config::get().yaw_standing, 0, "Aimbot", "yaw_standing");

	SetupValue(c_config::get().pitch_moving, 0, "Aimbot", "pitch_moving");
	SetupValue(c_config::get().yaw_moving, 0, "Aimbot", "yaw_moving");

	SetupValue(c_config::get().pitch_air, 0, "Aimbot", "pitch_air");
	SetupValue(c_config::get().yaw_air, 0, "Aimbot", "yaw_air");


	SetupValue(c_config::get().fakelag, 0, "Aimbot", "fakelag");
	SetupValue(c_config::get().antiaim_freestanding, false, "Aimbot", "antiaim_freestanding");
	SetupValue(c_config::get().antiaim_freestanding_mode, 0, "Aimbot", "antiaim_freestanding_mode");
	SetupValue(c_config::get().extrapolated, false, "Aimbot", "extrapolated");
	SetupValue(c_config::get().desync, 0, "Aimbot", "desync");
	SetupValue(c_config::get().accuracy_boost, false, "Aimbot", "accuracy_boost");
	SetupValue(c_config::get().auto_scope, false, "Aimbot", "auto_scope");
	SetupValue(c_config::get().autorevolver, false, "Aimbot", "auto_revolver");
	SetupValue(c_config::get().fakelag_prediction, false, "Aimbot", "fakelag_prediction");
	SetupValue(c_config::get().prefer_bodyaim, false, 5, "Aimbot", "prefer_bodyaim");
	SetupValue(c_config::get().bodyaim_mode, 0, "Aimbot", "bodyaim_mode");
	SetupValue(c_config::get().on_shot_aa, false, "Aimbot", "on_shot_aa");
	SetupValue(c_config::get().on_shot_aa_type, 0, "Aimbot", "on_shot_aa_type");
	SetupValue(c_config::get().aa_jitter, 0, "Aimbot", "aa_jitter");

	SetupValue(c_config::get().visuals_enabled, false, "Visuals", "Enabled");
	SetupValue(c_config::get().fov, 0, "Visuals", "Fov");
	SetupValue(c_config::get().nightmode, false, "Visuals", "Nightmode");
	SetupValue(c_config::get().remove_scope, false, "Visuals", "Remove_scope"); 
	SetupValue(c_config::get().hitmarker, false, "Visuals", "hitmarker");
	SetupValue(c_config::get().hitmarker_sound, 0, "Visuals", "hitmarker_sound");
	SetupValue(c_config::get().remove_visual_recoil, false, "Visuals", "remove_visual_recoil");
	SetupValue(c_config::get().viewmodel_fov, 0, "Visuals", "viewmodel_fov");
	SetupValue(c_config::get().no_smoke, false, "Visuals", "no_smoke");
	SetupValue(c_config::get().transparent_props, false, "Visuals", "transparent_props");
	SetupValue(c_config::get().wireframe_arms, false, "Visuals", "wireframe_arms");
	SetupValue(c_config::get().thirdperson_distance, 0, "Visuals", "thirdperson_distance");
	SetupValue(c_config::get().event_logger, false, 3, "Visuals", "event_logger");
	SetupValue(c_config::get().dynamic_scope, false, "Visuals", "dynamic_scope");
	SetupValue(c_config::get().force_crosshair, false, "Visuals", "force_crosshair");
	SetupValue(c_config::get().visual_preserve_killfeed, false, "Visuals", "visual_preserve_killfeed");
	//SetupValue(c_config::get().visual_spectator_list, false, "Visuals", "visual_preserve_killfeed");
	SetupValue(c_config::get().remove_flash, false, "Visuals", "remove_flash");
	/*
	Enemies
	*/
	SetupValue(c_config::get().name_enemies, false, "Visuals", "name_enemies");
	SetupValue(c_config::get().box_enemies, false, "Visuals", "box_enemies");
	SetupValue(c_config::get().weapon_enemies, false, "Visuals", "weapon_enemies");
	SetupValue(c_config::get().show_icon_when_possible_enemies, 0, "Visuals", "show_icon_when_possible_enemies");
	SetupValue(c_config::get().health_enemies, 0, "Visuals", "health_enemies");
	SetupValue(c_config::get().glow_enemies, false, "Visuals", "glow_enemies");
	SetupValue(c_config::get().chams_enemies, false, "Visuals", "chams_enemies");
	SetupValue(c_config::get().chams_xqz_enemies, false, "Visuals", "chams_xqz_enemies");
	SetupValue(c_config::get().fov_arrows_enemy, false, "Visuals", "fov_arrows_enemy");
	SetupValue(c_config::get().fov_arrows_enemy_distance, 255, "Visuals", "fov_arrows_enemy_distance");
	SetupValue(c_config::get().fov_arrows_enemy_size, 255, "Visuals", "fov_arrows_enemy_size");
	SetupValue(c_config::get().flags_enemy, false, "Visuals", "flags_enemy");
	SetupValue(c_config::get().ammo_enemy, false, "Visuals", "ammo_enemy");
	SetupValue(c_config::get().fov_while_zoomed, false, "Visuals", "fov_while_zoomed");
	SetupValue(c_config::get().fov_arrows_always_indicate, false, "Visuals", "fov_arrows_always_indicate");
	SetupValue(c_config::get().local_chams, false, "Visuals", "local_chams");
	SetupValue(c_config::get().local_glow, false, "Visuals", "local_glow");
	SetupValue(c_config::get().hand_chams, false, "Visuals", "hand_chams");
	SetupValue(c_config::get().penetration_crosshair, false, "Visuals", "penetration_crosshair");
	SetupValue(c_config::get().visuals_radar, false, "Visuals", "visuals_radar");
	SetupValue(c_config::get().grenade_prediction, false, "Visuals", "grenade_prediction");
	

	SetupValue(c_config::get().name_esp_color_r, 255, "Visuals", "name_esp_color_r");
	SetupValue(c_config::get().name_esp_color_g, 255, "Visuals", "name_esp_color_g");
	SetupValue(c_config::get().name_esp_color_b, 255, "Visuals", "name_esp_color_b");
	SetupValue(c_config::get().name_esp_color_a, 255, "Visuals", "name_esp_color_a");

	SetupValue(c_config::get().box_esp_color_r, 255, "Visuals", "box_esp_color_r");
	SetupValue(c_config::get().box_esp_color_g, 255, "Visuals", "box_esp_color_g");
	SetupValue(c_config::get().box_esp_color_b, 255, "Visuals", "box_esp_color_b");
	SetupValue(c_config::get().box_esp_color_a, 255, "Visuals", "box_esp_color_a");

	SetupValue(c_config::get().weapon_esp_color_r, 255, "Visuals", "weapon_esp_color_r");
	SetupValue(c_config::get().weapon_esp_color_g, 255, "Visuals", "weapon_esp_color_g");
	SetupValue(c_config::get().weapon_esp_color_b, 255, "Visuals", "weapon_esp_color_b");
	SetupValue(c_config::get().weapon_esp_color_a, 255, "Visuals", "weapon_esp_color_a");

	SetupValue(c_config::get().glow_esp_color_r, 255, "Visuals", "glow_esp_color_r");
	SetupValue(c_config::get().glow_esp_color_g, 255, "Visuals", "glow_esp_color_g");
	SetupValue(c_config::get().glow_esp_color_b, 255, "Visuals", "glow_esp_color_b");
	SetupValue(c_config::get().glow_esp_color_a, 255, "Visuals", "glow_esp_color_a");

	SetupValue(c_config::get().chams_esp_color_r, 255, "Visuals", "chams_esp_color_r");
	SetupValue(c_config::get().chams_esp_color_g, 255, "Visuals", "chams_esp_color_g");
	SetupValue(c_config::get().chams_esp_color_b, 255, "Visuals", "chams_esp_color_b");
	SetupValue(c_config::get().chams_esp_color_a, 255, "Visuals", "chams_esp_color_a");

	SetupValue(c_config::get().chams_xqz_esp_color_r, 255, "Visuals", "chams_xqz_esp_color_r");
	SetupValue(c_config::get().chams_xqz_esp_color_g, 255, "Visuals", "chams_xqz_esp_color_g");
	SetupValue(c_config::get().chams_xqz_esp_color_b, 255, "Visuals", "chams_xqz_esp_color_b");
	SetupValue(c_config::get().chams_xqz_esp_color_a, 255, "Visuals", "chams_xqz_esp_color_a");

	SetupValue(c_config::get().fov_arrows_esp_color_r, 255, "Visuals", "fov_arrows_esp_color_r");
	SetupValue(c_config::get().fov_arrows_esp_color_g, 255, "Visuals", "fov_arrows_esp_color_g");
	SetupValue(c_config::get().fov_arrows_esp_color_b, 255, "Visuals", "fov_arrows_esp_color_b");
	SetupValue(c_config::get().fov_arrows_esp_color_a, 255, "Visuals", "fov_arrows_esp_color_a");

	SetupValue(c_config::get().local_chams_color_r, 255, "Visuals", "local_chams_color_r");
	SetupValue(c_config::get().local_chams_color_g, 255, "Visuals", "local_chams_color_g");
	SetupValue(c_config::get().local_chams_color_b, 255, "Visuals", "local_chams_color_b");
	SetupValue(c_config::get().local_chams_color_a, 255, "Visuals", "local_chams_color_a");

	SetupValue(c_config::get().hand_chams_color_r, 255, "Visuals", "hand_chams_color_r");
	SetupValue(c_config::get().hand_chams_color_g, 255, "Visuals", "hand_chams_color_g");
	SetupValue(c_config::get().hand_chams_color_b, 255, "Visuals", "hand_chams_color_b");
	SetupValue(c_config::get().hand_chams_color_a, 255, "Visuals", "hand_chams_color_a");

	SetupValue(c_config::get().local_glow_color_r, 255, "Visuals", "local_glow_color_r");
	SetupValue(c_config::get().local_glow_color_g, 255, "Visuals", "local_glow_color_g");
	SetupValue(c_config::get().local_glow_color_b, 255, "Visuals", "local_glow_color_b");
	SetupValue(c_config::get().local_glow_color_a, 255, "Visuals", "local_glow_color_a");
	
	SetupValue(c_config::get().misc_bhop, false, "Misc", "Bunny_hop");
	SetupValue(c_config::get().misc_autostrafe, false, "Misc", "Auto_strafe");
	SetupValue(c_config::get().misc_fastcrouch, false, "Misc", "misc_fastcrouch");

	SetupValue(c_config::get().thirdperson_bind, -1, "Misc", "thirdperson_bind");
	SetupValue(c_config::get().misc_clantag, false, "Misc", "misc_clantag");
	SetupValue(c_config::get().slowwalk_bind, -1, "Misc", "slowwalk_bind");
	SetupValue(c_config::get().slowwalk_speed, 0, "Misc", "slowwalk_speed");
	SetupValue(c_config::get().misc_antiuntrusted, false, "Misc", "misc_antiuntrusted");
	SetupValue(c_config::get().manual_swap_bind, -1, "Misc", "manual_swap_bind");

	SetupValue(c_config::get().menu_color_r, 255, "Misc", "menu_color_r");
	SetupValue(c_config::get().menu_color_g, 255, "Misc", "menu_color_g");
	SetupValue(c_config::get().menu_color_b, 255, "Misc", "menu_color_b");
	SetupValue(c_config::get().menu_color_a, 255, "Misc", "menu_color_a");

	SetupValue(c_config::get().knife_model, 0, "Skins", "knife_model");
	SetupValue(c_config::get().knife_skin, 0, "Skins", "knife_skin");
	SetupValue(c_config::get().scar20_skin, 0, "Skins", "scar20_skin");
	SetupValue(c_config::get().g3sg1_skin, 0, "Skins", "g3sg1_skin");
	SetupValue(c_config::get().ssg08_skin, 0, "Skins", "ssg08_skin");
}

void CConfig::export_to_clipboard(std::string file) {

	static TCHAR path[MAX_PATH];
	std::string folder, file_directory;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\xyo\\";
		file_directory = std::string(path) + "\\xyo\\" + file + ".ini";
	}

	Save(file);

	std::ifstream input_file = std::ifstream(file_directory);
	std::string str((std::istreambuf_iterator< char >(input_file)), std::istreambuf_iterator< char >());

	const char* output = str.c_str();
	const size_t len = strlen(output) + 1;

	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(mem), output, len);

	GlobalUnlock(mem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, mem);
	CloseClipboard();
}


bool CConfig::import_from_clipboard(std::string file) {
	const auto get_clipboard_data = []() -> std::string {
		OpenClipboard(0);
		HANDLE hData = GetClipboardData(CF_TEXT);
		char *data = static_cast<char*>(GlobalLock(hData));
		std::string text(data);
		GlobalUnlock(hData);
		CloseClipboard();

		return text;
	};

	static TCHAR path[MAX_PATH];
	std::string folder, file_directory;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\xyo\\";
		file_directory = std::string(path) + "\\xyo\\" + file + ".ini";
	}

	std::string clipboard = get_clipboard_data();

	std::ofstream output = std::ofstream(file_directory);
	if (!output.good())
		return false;

	// clear our config
	output.clear();

	// write clipboard data to the file
	output << clipboard;


	// close the file after writing
	output.close();

	// re-load the imported config
	Load(file);

	return true;
}

void CConfig::SetupValue(int& value, int def, std::string category, std::string name)
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

void CConfig::SetupValue(bool* value, bool def, int size, std::string category, std::string name) // for multiboxes
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(category, name, &value[c]));
	}
}

void CConfig::Save(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\xyo\\";
		file = std::string(path) + "\\xyo\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\xyo\\";
		file = std::string(path) + "\\xyo\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config2 = new CConfig();

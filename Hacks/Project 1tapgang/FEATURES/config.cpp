#include "config.h"
#include "../includes.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>


void c_config::setup()
{
	//aimbot tab
	SetupValue(SETTINGS::settings.aim_bool, false, ("aimbot"), ("enable aimbot"));
	SetupValue(SETTINGS::settings.acc_type, 2, ("aimbot"), ("hitscan"));
	SetupValue(SETTINGS::settings.chance_val, 6, ("aimbot"), ("hitchance"));
	SetupValue(SETTINGS::settings.damage_val, 4, ("aimbot"), ("min damage"));
	SetupValue(SETTINGS::settings.point_val, 0, ("aimbot"), ("head scale"));
	SetupValue(SETTINGS::settings.body_val, 0, ("aimbot"), ("body scale"));
	SetupValue(SETTINGS::settings.mmoving, false, ("aimbot"), ("extrapolation"));
	SetupValue(SETTINGS::settings.fakefix_bool, false, ("aimbot"), ("fake-lag prediction"));
	SetupValue(SETTINGS::settings.delay_shot, false, ("aimbot"), ("delay_shot"));
	SetupValue(SETTINGS::settings.slow_walk, false, ("aimbot"), ("slow_walk"));
	SetupValue(SETTINGS::settings.slow_walk_key, 0, ("aimbot"), ("slow_walk key"));
	SetupValue(SETTINGS::settings.remove_duck_cooldown, false, ("aimbot"), ("fast-duck"));
	SetupValue(SETTINGS::settings.autozeus, false, ("aimbot"), ("auto-zeus"));
	SetupValue(SETTINGS::settings.auto_scope, false, ("aimbot"), ("auto-scope"));
	SetupValue(SETTINGS::settings.auto_stop, false, ("aimbot"), ("auto-stop"));
	SetupValue(SETTINGS::settings.fake_crouch, false, ("aimbot"), ("fake-crouch"));
	SetupValue(SETTINGS::settings.crouch_peak, false, ("aimbot"), ("crouch-peak"));
	SetupValue(SETTINGS::settings.auto_revolver, false, ("aimbot"), ("auto-revolver"));
	SetupValue(SETTINGS::settings.resolve_bool, false, ("aimbot"), ("resolver"));
	SetupValue(SETTINGS::settings.override, false, ("aimbot"), ("override"));
	SetupValue(SETTINGS::settings.overridekey, 0, ("aimbot"), ("override key"));

	//hitscan tab
	SetupValue(SETTINGS::settings.autopoint, false, ("aimbot"), ("auto_point"));
	SetupValue(SETTINGS::settings.pointlimit, 0, ("aimbot"), ("point_limit"));
	SetupValue(SETTINGS::settings.bhead, false, ("aimbot"), ("head_hitbox"));
	SetupValue(SETTINGS::settings.bneck, false, ("aimbot"), ("neck_hitbox"));
	SetupValue(SETTINGS::settings.bchest, false, ("aimbot"), ("chest_hitbox"));
	SetupValue(SETTINGS::settings.bstomach, false, ("aimbot"), ("stomach_hitbox"));
	SetupValue(SETTINGS::settings.bpelvis, false, ("aimbot"), ("pelvis_hitbox"));
	SetupValue(SETTINGS::settings.barms, false, ("aimbot"), ("arms_hitbox"));
	SetupValue(SETTINGS::settings.blegs, false, ("aimbot"), ("legs_hitbox"));

	SetupValue(SETTINGS::settings.head, 0, ("aimbot"), ("head_scale"));
	SetupValue(SETTINGS::settings.neck, 0, ("aimbot"), ("neck_scale"));
	SetupValue(SETTINGS::settings.chest, 0, ("aimbot"), ("chest_scale"));
	SetupValue(SETTINGS::settings.stomach, 0, ("aimbot"), ("stomach_scale"));
	SetupValue(SETTINGS::settings.pelvis, 0, ("aimbot"), ("pelvis_scale"));
	SetupValue(SETTINGS::settings.arms, 0, ("aimbot"), ("arms_scale"));
	SetupValue(SETTINGS::settings.legs, 0, ("aimbot"), ("legs_scale"));

	//anti-aim tab
	SetupValue(SETTINGS::settings.aa_bool, false, ("anti-aim"), ("enable anti-aim"));
	SetupValue(SETTINGS::settings.aa_pitch_type, 0, ("anti-aim"), ("pitch"));
	SetupValue(SETTINGS::settings.aa_real_type, 0, ("anti-aim"), ("yaw"));
	SetupValue(SETTINGS::settings.aa_left, 0, ("anti-aim"), ("left key"));
	SetupValue(SETTINGS::settings.aa_right, 0, ("anti-aim"), ("right key"));
	SetupValue(SETTINGS::settings.aa_back, 0, ("anti-aim"), ("back key"));
	SetupValue(SETTINGS::settings.desync_aa, false, ("anti-aim"), ("desync"));
	SetupValue(SETTINGS::settings.aa_auto_bool, false, ("anti-aim"), ("freestanding desync"));
	SetupValue(SETTINGS::settings.disable_tp_on_nade, false, ("anti-aim"), ("disable thirdperson on nade"));
	SetupValue(SETTINGS::settings.aa_jitter, false, ("anti-aim"), ("left jitter"));
	SetupValue(SETTINGS::settings.aa_jitter1, false, ("anti-aim"), ("right jitter"));
	SetupValue(SETTINGS::settings.aa_jitter2, false, ("anti-aim"), ("back jitter"));
	SetupValue(SETTINGS::settings.jitter_range, false, ("anti-aim"), ("left jitter range"));
	SetupValue(SETTINGS::settings.jitter_range1, false, ("anti-aim"), ("right jitter range"));
	SetupValue(SETTINGS::settings.jitter_range2, false, ("anti-aim"), ("back jitter range"));

	//visuals tab
	SetupValue(SETTINGS::settings.esp_bool, false, ("visuals"), ("enable visuals"));
	SetupValue(SETTINGS::settings.box_bool, false, ("visuals"), ("draw box"));
	SetupValue(SETTINGS::settings.name_bool, false, ("visuals"), ("draw player name"));
	SetupValue(SETTINGS::settings.weap_bool, false, ("visuals"), ("draw player weapon"));
	SetupValue(SETTINGS::settings.health_bool, false, ("visuals"), ("draw health"));
	SetupValue(SETTINGS::settings.info_bool, false, ("visuals"), ("player event logs"));
	SetupValue(SETTINGS::settings.ammo_bool, false, ("visuals"), ("draw ammo"));
	SetupValue(SETTINGS::settings.fov_bool, false, ("visuals"), ("draw fov arrows"));
	SetupValue(SETTINGS::settings.bullet_impact, false, ("visuals"), ("draw bullet impact"));
	SetupValue(SETTINGS::settings.radar, false, ("visuals"), ("draw radar"));
	SetupValue(SETTINGS::settings.lag_comp_hitbox, false, ("visuals"), ("lag comp hitbox"));
	SetupValue(SETTINGS::settings.asus_bool, false, ("visuals"), ("asus walls"));
	SetupValue(SETTINGS::settings.night_bool, false, ("visuals"), ("nightmode"));
	SetupValue(SETTINGS::settings.chams_type, 0, ("visuals"), ("chams"));
	SetupValue(SETTINGS::settings.xqz, false, ("visuals"), ("xqz chams"));
	SetupValue(SETTINGS::settings.glowenable, false, ("visuals"), ("enemy glow"));
	SetupValue(SETTINGS::settings.beam_bool, false, ("visuals"), ("bullet tracers"));
	SetupValue(SETTINGS::settings.tp_bool, false, ("visuals"), ("thirdperson"));
	SetupValue(SETTINGS::settings.thirdperson_int, 0, ("visuals"), ("thirdperson key"));
	SetupValue(SETTINGS::settings.smoke_bool, false, ("visuals"), ("remove smoke"));
	SetupValue(SETTINGS::settings.no_flash, false, ("visuals"), ("remove flash"));
	SetupValue(SETTINGS::settings.scope_bool, false, ("visuals"), ("remove scope"));
	SetupValue(SETTINGS::settings.removescoping, false, ("visuals"), ("remove zoom"));
	SetupValue(SETTINGS::settings.static_crosshair, false, ("visuals"), ("static crosshair"));
	SetupValue(SETTINGS::settings.fixscopesens, false, ("visuals"), ("fix zoom sensivity"));
	SetupValue(SETTINGS::settings.force_ragdoll, false, ("visuals"), ("ragdoll"));
	SetupValue(SETTINGS::settings.fov_val, false, ("visuals"), ("render fov"));
	SetupValue(SETTINGS::settings.viewfov_val, false, ("visuals"), ("viewmodel fov"));

	//misc tab
	SetupValue(SETTINGS::settings.misc_bool, false, ("misc"), ("enable misc"));
	SetupValue(SETTINGS::settings.bhop_bool, false, ("misc"), ("bunnyhop"));
	SetupValue(SETTINGS::settings.strafe_bool, false, ("misc"), ("autostrafe"));
	SetupValue(SETTINGS::settings.misc_chat_spam, false, ("misc"), ("chat-spam"));
	SetupValue(SETTINGS::settings.hit_sound, 0, ("misc"), ("hit-sound"));
	SetupValue(SETTINGS::settings.misc_clantag, false, ("misc"), ("clan-tag"));
	SetupValue(SETTINGS::settings.lag_bool, false, ("misc"), ("enable fake-lag"));
	SetupValue(SETTINGS::settings.lag_type, 1, ("misc"), ("fake-lag type"));
	SetupValue(SETTINGS::settings.fake_lag_choke, 1, ("misc"), ("fake lag"));

	//colors
	SetupValue(SETTINGS::settings.vmodel_col[0], 1.f, ("visuals"), ("chams_color_r"));
	SetupValue(SETTINGS::settings.vmodel_col[1], 1.f, ("visuals"), ("chams_color_g"));
	SetupValue(SETTINGS::settings.vmodel_col[2], 1.f, ("visuals"), ("chams_color_b"));

	SetupValue(SETTINGS::settings.invisible_col[0], 1.f, ("visuals"), ("invis_chams_color_r"));
	SetupValue(SETTINGS::settings.invisible_col[1], 1.f, ("visuals"), ("invis_chams_color_g"));
	SetupValue(SETTINGS::settings.invisible_col[2], 1.f, ("visuals"), ("invis_chams_color_b"));

	SetupValue(SETTINGS::settings.glow_col[0], 1.f, ("visuals"), ("glow_color_r"));
	SetupValue(SETTINGS::settings.glow_col[1], 1.f, ("visuals"), ("glow_color_g"));
	SetupValue(SETTINGS::settings.glow_col[2], 1.f, ("visuals"), ("glow_color_b"));

	SetupValue(SETTINGS::settings.bulletlocal_col[0], 1.f, ("visuals"), ("beam_color_r"));
	SetupValue(SETTINGS::settings.bulletlocal_col[1], 1.f, ("visuals"), ("beam_color_g"));
	SetupValue(SETTINGS::settings.bulletlocal_col[2], 1.f, ("visuals"), ("beam_color_b"));

	SetupValue(SETTINGS::settings.arrows_col[0], 1, "visuals", "arrows_color_r");
	SetupValue(SETTINGS::settings.arrows_col[1], 1, "visuals", "arrows_color_g");
	SetupValue(SETTINGS::settings.arrows_col[2], 1, "visuals", "arrows_color_b");
}

void c_config::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void c_config::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void c_config::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}


void c_config::Save()
{
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		char szCmd[256];
		sprintf(szCmd, "\\nicetap\\%s.cfg", SETTINGS::settings.config_name);

		folder = std::string(path) + "\\nicetap\\";
		file = std::string(path) + szCmd;
	}

	CreateDirectoryA(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void c_config::Load()
{
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		char szCmd[256];
		sprintf(szCmd, "\\nicetap\\%s.cfg", SETTINGS::settings.config_name);

		folder = std::string(path) + "\\nicetap\\";
		file = std::string(path) + szCmd;
	}

	CreateDirectoryA(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

c_config* Config = new c_config();
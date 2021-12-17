#pragma once
#include <vector>
#include <string>
#include <minwindef.h>
using namespace std;

template <typename T>
class ConfigItem
{
	std::string category, name;
	T* value;
public:
	ConfigItem(std::string category, std::string name, T* value)
	{
		this->category = category;
		this->name = name;
		this->value = value;
	}
};

template <typename T>
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
	std::vector<ConfigValue<float*>*> colors;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);
	void SetupValue(float*&, float*, std::string, std::string);
	string GetModuleFilePath(HMODULE hModule);
	string GetModuleBaseDir(HMODULE hModule);

public:
	vector<string> ConfigList;
	void ReadConfigs(LPCTSTR lpszFileName);
	CConfig()
	{
		Setup();
	}

	void Setup()
	{
		SetupValue(Vars.Ragebot.Enable, TRUE, "RAGEBOT", "ENABLE");
		SetupValue(Vars.Ragebot.Silent, TRUE, "RAGEBOT", "SILENTAIM");
		SetupValue(Vars.Ragebot.AutoFire, TRUE, "RAGEBOT", "AUTOFIRE");
		SetupValue(Vars.Ragebot.AutoScope, FALSE, "RAGEBOT", "AUTOSCOPE");
		SetupValue(Vars.Ragebot.Accuracy.RemoveRecoil, TRUE, "RAGEBOT", "ACCURACY_NORECOIL");
		SetupValue(Vars.Misc.Knifebot, TRUE, "RAGEBOT", "ACCURACY_KNIFEBOT");
		SetupValue(Vars.Ragebot.Accuracy.Hitchance, 0, "RAGEBOT", "ACCURACY_HITCHANCE");
		SetupValue(Vars.Ragebot.Accuracy.Mindamage, 1, "RAGEBOT", "ACCURACY_MINDAMAGE");
		SetupValue(Vars.Ragebot.Hitscan.PriorityHitbox, NULL, "RAGEBOT", "HITSCAN_PRIORITYHITBOX");
		SetupValue(Vars.Ragebot.Hitscan.Multibox, 2, "RAGEBOT", "HITSCAN_MULTIBOX");
		SetupValue(Vars.Ragebot.Hitscan.Multipoint, TRUE, "RAGEBOT", "HITSCAN_MULTIPOINT");

		SetupValue(Vars.Ragebot.Antiaim.Enable, TRUE, "ANTIAIM", "ENABLE");
		SetupValue(Vars.Ragebot.Antiaim.Pitch, 1, "ANTIAIM", "PITCH");
		SetupValue(Vars.Ragebot.Antiaim.Yaw, 1, "ANTIAIM", "YAW");
		SetupValue(Vars.Ragebot.Antiaim.Fakelag.Type, 1, "ANTIAIM", "FAKELAGS");
		SetupValue(Vars.Ragebot.Antiaim.Desync, FALSE, "ANTIAIM", "DESYNC");

		SetupValue(Vars.Misc.AntiUT, TRUE, "MOVEMENT", "ANTI_UNTRUSTED");
		SetupValue(Vars.Misc.Bunnyhop, TRUE, "MOVEMENT", "BUNNYHOP");
		SetupValue(Vars.Misc.Strafers.Enable, TRUE, "MOVEMENT", "AUTOSTRAFE");

		SetupValue(Vars.Visuals.Player.Box, TRUE, "ESP", "BOX");
		SetupValue(Vars.Visuals.Colors.Box[0], 1, "ESP", "BOX_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Box[1], 1, "ESP", "BOX_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Box[2], 1, "ESP", "BOX_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Box[3], 1, "ESP", "BOX_COLOR_B");

		//SetupValue(Vars.Visuals.Chams.Material, 0, "CHAMS", "MATERIAL");
		//SetupValue(Vars.Visuals.Chams.Enemy, FALSE, "CHAMS", "CHAMS_ENEMY");
		//SetupValue(Vars.Visuals.Chams.Team, FALSE, "CHAMS", "CHAMS_TEAM");
		//SetupValue(Vars.Visuals.Chams.Local, FALSE, "CHAMS", "CHAMS_LOCAL");

		SetupValue(Vars.Visuals.Colors.Chams.Visible[0], 1, "CHAMS", "CHAMS_ENEMY_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.Visible[1], 1, "CHAMS", "CHAMS_ENEMY_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.Visible[2], 1, "CHAMS", "CHAMS_ENEMY_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.Visible[3], 1, "CHAMS", "CHAMS_ENEMY_COLOR_A");

		SetupValue(Vars.Visuals.Colors.Chams.XQZ[0], 1, "CHAMS", "CHAMS_ENEMY_XQZ_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.XQZ[1], 1, "CHAMS", "CHAMS_ENEMY_XQZ_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.XQZ[2], 1, "CHAMS", "CHAMS_ENEMY_XQZ_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.XQZ[3], 1, "CHAMS", "CHAMS_ENEMY_XQZ_COLOR_A");

		SetupValue(Vars.Visuals.Colors.Chams.Team[0], 1, "CHAMS", "CHAMS_TEAM_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.Team[1], 1, "CHAMS", "CHAMS_TEAM_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.Team[2], 1, "CHAMS", "CHAMS_TEAM_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.Team[3], 1, "CHAMS", "CHAMS_TEAM_COLOR_A");

		SetupValue(Vars.Visuals.Colors.Chams.TeamXQZ[0], 1, "CHAMS", "CHAMS_TEAM_XQZ_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.TeamXQZ[1], 1, "CHAMS", "CHAMS_TEAM_XQZ_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.TeamXQZ[2], 1, "CHAMS", "CHAMS_TEAM_XQZ_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.TeamXQZ[3], 1, "CHAMS", "CHAMS_TEAM_XQZ_COLOR_A");

		SetupValue(Vars.Visuals.Colors.Chams.Local[0], 1, "CHAMS", "CHAMS_LOCAL_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.Local[1], 1, "CHAMS", "CHAMS_LOCAL_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.Local[2], 1, "CHAMS", "CHAMS_LOCAL_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.Local[3], 1, "CHAMS", "CHAMS_LOCAL_COLOR_A");

		SetupValue(Vars.Visuals.Colors.Chams.LocalXQZ[0], 1, "CHAMS", "CHAMS_LOCAL_XQZ_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Chams.LocalXQZ[1], 1, "CHAMS", "CHAMS_LOCAL_XQZ_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Chams.LocalXQZ[2], 1, "CHAMS", "CHAMS_LOCAL_XQZ_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Chams.LocalXQZ[3], 1, "CHAMS", "CHAMS_LOCAL_XQZ_COLOR_A");

		SetupValue(Vars.Visuals.Player.Glow, TRUE, "ESP", "GLOW_ENTITY");
		SetupValue(Vars.Visuals.Colors.Glow[0], 1, "ESP", "GLOW_ENTITY_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Glow[1], 0, "ESP", "GLOW_ENTITY_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Glow[2], 1, "ESP", "GLOW_ENTITY_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Glow[3], 0.7f, "ESP", "GLOW_ENTITY_COLOR_B");

		SetupValue(Vars.Visuals.Player.LocalGlow, FALSE, "ESP", "GLOW_LOCAL");
		SetupValue(Vars.Visuals.Player.PulseLocalGlow, FALSE, "ESP", "GLOW_LOCAL_PULSE");
		SetupValue(Vars.Visuals.Colors.LocalGlow[0], 0, "ESP", "GLOW_LOCAL_COLOR_R");
		SetupValue(Vars.Visuals.Colors.LocalGlow[1], 1, "ESP", "GLOW_LOCAL_COLOR_G");
		SetupValue(Vars.Visuals.Colors.LocalGlow[2], 1, "ESP", "GLOW_LOCAL_COLOR_B");
		SetupValue(Vars.Visuals.Colors.LocalGlow[3], 0.7f, "ESP", "GLOW_LOCAL_COLOR_B");

		SetupValue(Vars.Visuals.Player.SnapLines, FALSE , "ESP", "SNAPLINES");
		SetupValue(Vars.Visuals.Colors.SnapLines[0], 1, "ESP", "SNAPLINES_COLOR_R");
		SetupValue(Vars.Visuals.Colors.SnapLines[1], 1, "ESP", "SNAPLINES_COLOR_G");
		SetupValue(Vars.Visuals.Colors.SnapLines[2], 1, "ESP", "SNAPLINES_COLOR_B");
		SetupValue(Vars.Visuals.Colors.SnapLines[3], 1, "ESP", "SNAPLINES_COLOR_B");

		SetupValue(Vars.Visuals.Player.Skeleton, TRUE, "ESP", "SKELETON");
		SetupValue(Vars.Visuals.Colors.Skeleton[0], 1, "ESP", "SKELETON_COLOR_R");
		SetupValue(Vars.Visuals.Colors.Skeleton[1], 1, "ESP", "SKELETON_COLOR_G");
		SetupValue(Vars.Visuals.Colors.Skeleton[2], 1, "ESP", "SKELETON_COLOR_B");
		SetupValue(Vars.Visuals.Colors.Skeleton[3], 1, "ESP", "SKELETON_COLOR_B");

		SetupValue(Vars.Visuals.Other.BulletTracers, TRUE, "ESP", "BULLETTRACER");
		SetupValue(Vars.Visuals.Colors.BulletTracers[0], 1, "ESP", "BULLETTRACER_COLOR_R");
		SetupValue(Vars.Visuals.Colors.BulletTracers[1], 0, "ESP", "BULLETTRACER_COLOR_G");
		SetupValue(Vars.Visuals.Colors.BulletTracers[2], 1, "ESP", "BULLETTRACER_COLOR_B");
		SetupValue(Vars.Visuals.Colors.BulletTracers[3], 1, "ESP", "BULLETTRACER_COLOR_B");

		SetupValue(Vars.Visuals.Other.FovArrows_Enable, TRUE, "ESP", "FOVARROWS");
		SetupValue(Vars.Visuals.Colors.FovArrows[0], 1, "ESP", "FOVARROWS_COLOR_R");
		SetupValue(Vars.Visuals.Colors.FovArrows[1], 0, "ESP", "FOVARROWS_COLOR_G");
		SetupValue(Vars.Visuals.Colors.FovArrows[2], 0, "ESP", "FOVARROWS_COLOR_B");
		SetupValue(Vars.Visuals.Colors.FovArrows[3], 1, "ESP", "FOVARROWS_COLOR_B");

		SetupValue(Vars.Visuals.Player.Health, TRUE, "ESP", "HEALTH");
		SetupValue(Vars.Visuals.Player.Name, TRUE, "ESP", "NAME");
		SetupValue(Vars.Visuals.Player.Weapon, TRUE, "ESP", "WEAPON");
		SetupValue(Vars.Visuals.Player.Armor, TRUE, "ESP", "ARMOR");
		SetupValue(Vars.Visuals.Player.LastHittedhitbox, TRUE, "ESP", "HITBOX");

		SetupValue(Vars.Visuals.Other.ThirdPersonKey, VK_XBUTTON1, "ESP", "THIRDPERSON");
		SetupValue(Vars.Visuals.Other.Nightmode, TRUE, "ESP", "NIGHTMODE");
		SetupValue(Vars.Visuals.Other.Radar, TRUE, "ESP", "RADAR");
		SetupValue(Vars.Visuals.C4, TRUE, "ESP", "BOMB");
		SetupValue(Vars.Visuals.Other.Hitmarker, TRUE, "ESP", "HITMARKER");
		SetupValue(Vars.Misc.FOV, 0, "ESP", "FOV");
		SetupValue(Vars.Visuals.DroppedWeapons, TRUE, "ESP", "DROPPEDWEAPONS");

		SetupValue(Vars.Visuals.Effects.NoVisRecoil, TRUE, "ESP", "NOVISRECOIL");
		SetupValue(Vars.Visuals.Effects.NoSmoke, TRUE, "ESP", "NOSMOKE");
		SetupValue(Vars.Visuals.Effects.NoScope, TRUE, "ESP", "NOSCOPE");
		SetupValue(Vars.Visuals.Effects.NoZoom, FALSE, "ESP", "NOZOOM");
		SetupValue(Vars.Visuals.Effects.NoPostProcessing, FALSE, "ESP", "NOPOSTPROCESS");
		SetupValue(Vars.Visuals.Effects.NoFlash, TRUE, "ESP", "NOFLASH");
	}

	void Save(string cfg_name);
	void Load(string cfg_name);
};

extern CConfig Config;
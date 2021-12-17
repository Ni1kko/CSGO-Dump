#include "Configuration.hpp"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
void CConfig::Setup()
{


    SetupValue(g_Options.Ragebot.MainSwitch, false, ("Ragebot"), ("RageToggle"));
    SetupValue(g_Options.Ragebot.Enabled, false, ("Ragebot"), ("Enabled"));
    SetupValue(g_Options.Ragebot.AutoFire, false, ("Ragebot"), ("AutoFire"));
    SetupValue(g_Options.Ragebot.FOV, 0.f, ("Ragebot"), ("FOV"));
    SetupValue(g_Options.Ragebot.Silent, false, ("Ragebot"), ("Silent"));
    SetupValue(g_Options.Ragebot.AutoPistol, false, ("Ragebot"), ("AutoPistol"));
    SetupValue(g_Options.Ragebot.KeyPress, 0, ("Ragebot"), ("Key"));


    SetupValue(g_Options.Ragebot.EnabledAntiAim, false, ("Ragebot"), ("AntiaimEnabled"));
    SetupValue(g_Options.Ragebot.Pitch, 0, ("Ragebot"), ("AntiaimPitch"));
    SetupValue(g_Options.Ragebot.YawTrue, 0, ("Ragebot"), ("AntiaimYaw-true"));
    SetupValue(g_Options.Ragebot.YawFake, 0, ("Ragebot"), ("AntiaimYaw-fake"));
    SetupValue(g_Options.Ragebot.AtTarget, false, ("Ragebot"), ("attargets"));
    SetupValue(g_Options.Ragebot.Edge, false, ("Ragebot"), ("edge"));
    SetupValue(g_Options.Ragebot.KnifeAA, false, ("Ragebot"), ("KnifeAA"));

    SetupValue(g_Options.Ragebot.FriendlyFire, false, ("Ragebot"), ("FriendlyFire"));
    SetupValue(g_Options.Ragebot.Hitbox, 0, ("Ragebot"), ("Hitbox"));
    SetupValue(g_Options.Ragebot.Hitscan, 0, ("Ragebot"), ("Hitscan"));

    SetupValue(g_Options.Ragebot.AntiRecoil, false, ("Ragebot"), ("AntiRecoil"));
    SetupValue(g_Options.Ragebot.AutoStop, false, ("Ragebot"), ("AutoStop"));
    SetupValue(g_Options.Ragebot.AutoCrouch, false, ("Ragebot"), ("AutoCrouch"));
    SetupValue(g_Options.Ragebot.AutoScope, false, ("Ragebot"), ("AutoScope"));
    SetupValue(g_Options.Ragebot.MinimumDamage, 0.f, ("Ragebot"), ("AutoWallDamage"));
    SetupValue(g_Options.Ragebot.Hitchance, false, ("Ragebot"), ("HitChance"));
    SetupValue(g_Options.Ragebot.HitchanceAmount, 0.f, ("Ragebot"), ("HitChanceAmount"));
    SetupValue(g_Options.Ragebot.Resolver, false, ("Ragebot"), ("Resolver"));
    SetupValue(g_Options.Ragebot.FakeLagFix, false, ("Ragebot"), ("Backtrack-lby"));
    SetupValue(g_Options.Ragebot.BAIMkey, 0, ("Ragebot"), ("BAIMKey"));




	SetupValue(g_Options.Legitbot.MainSwitch, false, ("Legitbot"), ("LegitToggle"));
	SetupValue(g_Options.Legitbot.Aimbot.Enabled, false, ("Legitbot"), ("Enabled"));
    SetupValue(g_Options.Legitbot.backtrack, false, ("Legitbot"), ("Backtrack"));


	SetupValue(g_Options.Legitbot.MainKey, 1, ("Legitbot"), ("Key"));
	SetupValue(g_Options.Legitbot.Mainfov, 0.f, ("Legitbot"), ("FOV"));
	SetupValue(g_Options.Legitbot.MainSmooth, 1.f, ("Legitbot"), ("Speed"));
	SetupValue(g_Options.Legitbot.main_recoil_min, 0, ("Legitbot"), ("RCS-min"));
    SetupValue(g_Options.Legitbot.main_recoil_max, 0, ("Legitbot"), ("RCS-max"));

	SetupValue(g_Options.Legitbot.PistolKey, 6, ("Legitbot"), ("Key-Pistol"));
	SetupValue(g_Options.Legitbot.Pistolfov, 0.f, ("Legitbot"), ("FOV-Pistol"));
	SetupValue(g_Options.Legitbot.PistolSmooth, 1.f, ("Legitbot"), ("Speed-Pistol"));
    SetupValue(g_Options.Legitbot.pistol_recoil_min, 0, ("Legitbot"), ("RCS-min-pistol"));
    SetupValue(g_Options.Legitbot.pistol_recoil_max, 0, ("Legitbot"), ("RCS-max-pistol"));

	SetupValue(g_Options.Legitbot.SniperKey, 6, ("Legitbot"), ("Key-Sniper"));
	SetupValue(g_Options.Legitbot.Sniperfov, 0.f, ("Legitbot"), ("FOV-Sniper"));
	SetupValue(g_Options.Legitbot.SniperSmooth, 0.f, ("Legitbot"), ("Speed-Sniper"));
    SetupValue(g_Options.Legitbot.sniper_recoil_min, 0, ("Legitbot"), ("RCS-min-sniper"));
    SetupValue(g_Options.Legitbot.sniper_recoil_max, 0, ("Legitbot"), ("RCS-max-sniper"));

	SetupValue(g_Options.Legitbot.Triggerbot.Enabled, false, ("Triggerbot"), ("Enabled"));
	SetupValue(g_Options.Legitbot.Triggerbot.Key, 6, ("Triggerbot"), ("Key"));
	SetupValue(g_Options.Legitbot.Triggerbot.Delay, 0.f, ("Triggerbot"), ("Delay"));
	SetupValue(g_Options.Legitbot.Triggerbot.hitchance, 0.f, ("Triggerbot"), ("Hitchance"));

	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Head, false, ("Triggerbot"), ("FilterHead"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Chest, false, ("Triggerbot"), ("FilterChest"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Stomach, false, ("Triggerbot"), ("FilterStomach"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Arms, false, ("Triggerbot"), ("FilterArms"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Legs, false, ("Triggerbot"), ("FilterLegs"));





	SetupValue(g_Options.Visuals.Enabled, false, ("Visuals"), ("VisualsEnabled"));
	SetupValue(g_Options.Visuals.Box, false, ("Visuals"), ("Box"));
	SetupValue(g_Options.Visuals.Name, false, ("Visuals"), ("Name"));
	SetupValue(g_Options.Visuals.HP, false, ("Visuals"), ("HP"));
	SetupValue(g_Options.Visuals.Weapon, false, ("Visuals"), ("Weapon"));
	SetupValue(g_Options.Visuals.Chams, false, ("Visuals"), ("Chams"));
	SetupValue(g_Options.Visuals.DLight, false, ("Visuals"), ("DLight"));
	SetupValue(g_Options.Visuals.SpreadCrosshair, false, ("Visuals"), ("SpreadCrosshair"));
	SetupValue(g_Options.Visuals.GrenadeESP, false, ("Visuals"), ("GrenadeESP"));
    SetupValue(g_Options.Visuals.Glow, false, ("Visuals"), ("Glow"));

	SetupValue(g_Options.Visuals.NoVisualRecoil, false, ("Visuals"), ("NoVisualRecoil"));
	SetupValue(g_Options.Visuals.Hands, 0, ("Visuals"), ("Hands"));
    SetupValue(g_Options.Visuals.FOVChanger, 0.f, ("Visuals"), ("fovchanger"));
	SetupValue(g_Options.Visuals.viewmodelChanger, 68.f, ("Visuals"), ("viewmodel_fov"));
	SetupValue(g_Options.Visuals.NoFlash, false, ("Visuals"), ("NoFlash"));
	SetupValue(g_Options.Visuals.NoSmoke, false, ("Visuals"), ("NoSmoke"));
	SetupValue(g_Options.Visuals.Time, false, ("Visuals"), ("Time"));
	SetupValue(g_Options.Visuals.noscopeborder, false, ("Visuals"), ("noscopeborder"));
	SetupValue(g_Options.Visuals.C4, false, ("Visuals"), ("C4"));
    SetupValue(g_Options.Visuals.GrenadePrediction, false, ("Visuals"), ("GrenadePrediction"));
    SetupValue(g_Options.Visuals.ThirdPerson, false, ("Visuals"), ("thirdperson"));

	SetupValue(g_Options.Visuals.Filter.Players, false, ("Visuals"), ("Players"));
	SetupValue(g_Options.Visuals.Filter.EnemyOnly, false, ("Visuals"), ("EnemyOnly"));
	SetupValue(g_Options.Visuals.WeaponsWorld, false, ("Visuals"), ("WeaponsWorld"));
	SetupValue(g_Options.Visuals.C4World, false, ("Visuals"), ("C4World"));





	SetupValue(g_Options.Misc.Bhop, false, ("Misc"), ("Bhop"));
    SetupValue(g_Options.Misc.AutoStrafe, false, ("Misc"), ("Autostrafe"));
    SetupValue(g_Options.Misc.SpecList, false, ("Misc"), ("speclist"));
    SetupValue(g_Options.Misc.ragequit, 0, ("Misc"), ("ragequit-key"));


    SetupValue(g_Options.Skinchanger.Enabled, false, ("Skinchanger"), ("Enabled"));
    SetupValue(g_Options.Skinchanger.Knife, 0, ("SkinChanger"), ("Knife"));
    SetupValue(g_Options.Skinchanger.KnifeSkin, 0, ("SkinChanger"), ("KnifeSkin"));
    SetupValue(g_Options.Skinchanger.gloves, 0, ("SkinChanger"), ("gloves"));

    SetupValue(g_Options.Skinchanger.AK47Skin, 0, ("SkinChanger"), ("AK47Skin"));
    SetupValue(g_Options.Skinchanger.M4A1SSkin, 0, ("SkinChanger"), ("M4A1SSkin"));
    SetupValue(g_Options.Skinchanger.M4A4Skin, 0, ("SkinChanger"), ("M4A4Skin"));
    SetupValue(g_Options.Skinchanger.AUGSkin, 0, ("SkinChanger"), ("AUGSkin"));
    SetupValue(g_Options.Skinchanger.FAMASSkin, 0, ("SkinChanger"), ("FAMASSkin"));

    SetupValue(g_Options.Skinchanger.AWPSkin, 0, ("SkinChanger"), ("AWPSkin"));
    SetupValue(g_Options.Skinchanger.SSG08Skin, 0, ("SkinChanger"), ("SSG08Skin"));
    SetupValue(g_Options.Skinchanger.SCAR20Skin, 0, ("SkinChanger"), ("SCAR20Skin"));

    SetupValue(g_Options.Skinchanger.P90Skin, 0, ("SkinChanger"), ("P90Skin"));
    SetupValue(g_Options.Skinchanger.UMP45Skin, 0, ("SkinChanger"), ("UMP45Skin"));

    SetupValue(g_Options.Skinchanger.GlockSkin, 0, ("SkinChanger"), ("GlockSkin"));
    SetupValue(g_Options.Skinchanger.USPSkin, 0, ("SkinChanger"), ("USPSkin"));
    SetupValue(g_Options.Skinchanger.DeagleSkin, 0, ("SkinChanger"), ("DeagleSkin"));

    SetupValue(g_Options.Skinchanger.tec9Skin, 0, ("SkinChanger"), ("tec9Skin"));
    SetupValue(g_Options.Skinchanger.P2000Skin, 0, ("SkinChanger"), ("P2000Skin"));
    SetupValue(g_Options.Skinchanger.P250Skin, 0, ("SkinChanger"), ("P250Skin"));
}

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{


		folder = std::string(path) + ("\\autismhack\\");
		switch (g_Options.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\autismhack\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\autismhack\\rage.ini");
			break;
		}

	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\autismhack\\");
		switch (g_Options.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\autismhack\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\autismhack\\rage.ini");
			break;
		}
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
		*value->value = (float)atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();
Variables g_Options;
#include "Settings.h"
#include <stdio.h>
#include <cstring>

//[enc_string_enable /]
//[junk_enable /]

using namespace Client;
namespace Settings
{
	void LoadSettings(string szIniFile)
	{
		CSX::Cvar::InitPath(szIniFile.c_str());

		Aimbot::aim_Active = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_ACTIVE_AIMBOT, Aimbot::aim_Active);
		Aimbot::aim_DrawBacktrack = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWBACKTRACK, Aimbot::aim_DrawBacktrack);
		//Aimbot::aim_Backtracktickrate = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACKTICKRATE, Aimbot::aim_Backtracktickrate);

		Aimbot::aim_Deathmatch = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DEATHMATCH, Aimbot::aim_Deathmatch);
		Aimbot::aim_WallAttack = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::aim_WallAttack);
		Aimbot::aim_CheckSmoke = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::aim_CheckSmoke);
		Aimbot::aim_CheckFlash = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKFLASH, Aimbot::aim_CheckFlash);
		Aimbot::aim_AntiJump = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::aim_AntiJump);
		//Aimbot::aim_RcsType = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_RCSTYPE, Aimbot::aim_RcsType);
		Aimbot::aim_DrawFov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::aim_DrawFov);
		Aimbot::aim_DrawSpot = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::aim_DrawSpot);
	
		

		string Color_Fov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_FOVCOLOR, "255,0,0");
		ScanColorFromCvar(Color_Fov.c_str(), Aimbot::aim_FovColor);
		//smooth
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Smooth[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Smooth[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Smooth[i]);
				
		}
		//fov
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Fov[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Fov[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Fov[i]);

		}
		//aim_FovType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_FovType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_FovType[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_FovType[i]);

		}

		//int aim_backtrack[6] = { 1,1,1,1,1,1 };
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_backtrack[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_backtrack[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_backtrack[i]);

		}
		//int aim_backtrack[6] = { 1,1,1,1,1,1 };
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_backtrack[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_backtrack[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_backtrack[i]);

		}
		//aim_RCSType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RCSType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_RCSType[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_RCSType[i]);
			

		}
		//aim_Spot	
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Spot[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Spot[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Spot[i]);
			//CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Spot[i]);

		}
		//aim_Rcs
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Rcs[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Rcs[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Rcs[i]);
			//CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Rcs[i]);

		}
		//aim_RcsType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_RcsType[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_RcsType[i]);
			
		}
		//AimBot_RcsAfterXShot
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "AimBot_RcsAfterXShot[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::AimBot_RcsAfterXShot[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::AimBot_RcsAfterXShot[i]);
			//CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::AimBot_RcsAfterXShot[i]);

		}
		//aim_Rcsx
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			
			string szStrTmp = "aim_Rcsx[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Rcsx[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Rcsx[i]);
			

		}
		//aim_Rcsy
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Rcsy[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_Rcsy[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_Rcsy[i]);
			

		}
		//aim_RcsSmooth
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsSmooth[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_RcsSmooth[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_RcsSmooth[i]);
			//CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RcsSmooth[i]);

		}
		//aim_RcsClampType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsClampType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			Aimbot::aim_RcsClampType[i] = CSX::Cvar::LoadCvar("Aimbot", p, Aimbot::aim_RcsClampType[i]);
			//CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RcsClampType[i]);

		}
		CSX::Cvar::SaveCvar("Aimbot", "aim_AutoPistol[3]", Settings::Aimbot::aim_AutoPistol[3]);

		Esp::esp_Style = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_STYLE, Esp::esp_Style);
		Esp::esp_WeaponIcon = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_WPICON, Esp::esp_WeaponIcon);
		Esp::esp_Size = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_SIZE, Esp::esp_Size);
		Esp::esp_Line = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_LINE, Esp::esp_Line) != false;
		Esp::esp_Outline = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_OUTLINE, Esp::esp_Outline) != false;

		Esp::esp_Name = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_NAME, Esp::esp_Name) != false;
		Esp::helper = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_GRANADEHELPER, Esp::helper) != false;
		Esp::esp_Infoz = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_INFO, Esp::esp_Infoz) != false;
		Esp::esp_Defusing = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_DEFUSING, Esp::esp_Defusing) != false;
		Esp::esp_Scoped = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_SCOPED, Esp::esp_Scoped) != false;
		Esp::esp_Flashed = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_FLASHED, Esp::esp_Flashed) != false;
		Esp::esp_Rank = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_RANK, Esp::esp_Rank) != false;
		Esp::esp_Health = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_HEALTH, Esp::esp_Health);
		Esp::esp_Armor = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_ARMOR, Esp::esp_Armor);
		Esp::esp_Ammo = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_AMMO, Esp::esp_Ammo) != false;
		Esp::esp_Distance = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_DISTANCE, Esp::esp_Distance) != false;

		Esp::esp_Skeleton = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_SKELETON, Esp::esp_Skeleton) != false;
		Esp::esp_BulletTrace = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BULLETTRACE, Esp::esp_BulletTrace);
		Esp::esp_Team = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_TEAM, Esp::esp_Team) != false;
		Esp::esp_Enemy = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_ENEMY, Esp::esp_Enemy) != false;
		Esp::esp_Visible = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_VISIBLE, Esp::esp_Visible);
		Esp::esp_ChamsVisible = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_CHAMSVISIBLE, Esp::esp_ChamsVisible);

		Esp::esp_Chams = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_CHAMS, Esp::esp_Chams);
		Esp::esp_Bomb = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BOMB, Esp::esp_Bomb) != false;
		Esp::esp_BombPlanted = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BOMBPLANTED, Esp::esp_BombPlanted) != false;
		Esp::esp_BombTimer = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BOMBTIMER, Esp::esp_BombTimer);
		Esp::esp_WorldWeapons = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_WORLDWEAPONS, Esp::esp_WorldWeapons) != false;
		Esp::esp_WorldGrenade = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_WORLDGRENADE, Esp::esp_WorldGrenade) != false;
		Esp::esp_Watermark = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_WATER, Esp::esp_Watermark) != false;
		Esp::esp_Cheatbuild = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_CHEATBUILD, Esp::esp_Cheatbuild) != false;
		Esp::esp_hitevent = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_HITLOG, Esp::esp_hitevent) != false;
		Esp::esp_Animals = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_ANIMALS, Esp::esp_Animals) != false;

		Esp::esp_Dlightz = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_DLIGHT, Esp::esp_Dlightz) != false;
		string esp_Dlight = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_DLIGHT, "255,88,0");

		string esp_Color_CT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_CT, "255,255,255");
		string esp_Color_TT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_TT, "255,255,255");
		string esp_Color_VCT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_VCT, "255,255,255");
		string esp_Color_VTT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_VTT, "255,255,255");

		string chams_Color_CT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_CT, "0,164,255");
		string chams_Color_TT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_TT, "255,64,64");
		string chams_Color_VCT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_VCT, "124,252,0");
		string chams_Color_VTT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_VTT, "124,252,0");
		string GrenadeHelper = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_GrenadeHelper, "255,0,0");

		string WorldWeapon_Color = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_WORLDWEAPON, "255,255,255");
		//string Visuals_Line_Color = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_LINE, "0,0,0");
		//string Name_Color = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_NAME, "255,255,0");
		ScanColorFromCvar(WorldWeapon_Color.c_str(), Settings::Esp::WorldWeapon_Color);
		//ScanColorFromCvar(Visuals_Line_Color.c_str(), Settings::Esp::Visuals_Line_Color);
		//ScanColorFromCvar(Name_Color.c_str(), Settings::Esp::Name_Color);
		ScanColorFromCvar(esp_Dlight.c_str(), Settings::Esp::esp_Dlight);

		ScanColorFromCvar(esp_Color_CT.c_str(), Settings::Esp::esp_Color_CT);
		ScanColorFromCvar(esp_Color_TT.c_str(), Settings::Esp::esp_Color_TT);
		ScanColorFromCvar(esp_Color_VCT.c_str(), Settings::Esp::esp_Color_VCT);
		ScanColorFromCvar(esp_Color_VTT.c_str(), Settings::Esp::esp_Color_VTT);

		ScanColorFromCvar(chams_Color_CT.c_str(), Settings::Esp::chams_Color_CT);
		ScanColorFromCvar(chams_Color_TT.c_str(), Settings::Esp::chams_Color_TT);
		ScanColorFromCvar(chams_Color_VCT.c_str(), Settings::Esp::chams_Color_VCT);
		ScanColorFromCvar(chams_Color_VTT.c_str(), Settings::Esp::chams_Color_VTT);
		ScanColorFromCvar(GrenadeHelper.c_str(), Settings::Esp::GrenadeHelper);

		Radar::Radar_Enable = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_ENABLE, Radar::Radar_Enable) != false;
		Radar::rad_Active = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_ACTIVE, Radar::rad_Active) != false;
		Radar::rad_Team = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_TEAM, Radar::rad_Team) != false;
		Radar::rad_Enemy = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_ENEMY, Radar::rad_Enemy) != false;
		Radar::rad_InGame = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_INGAME, Radar::rad_InGame) != false;
		Radar::rad_InGameKey = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_INGAMEKEY, Radar::rad_InGameKey);
		Radar::rad_Range = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_RANGE, Radar::rad_Range);
		Radar::rad_Alpha = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_ALPHA, Radar::rad_Alpha);

		string Color_CT = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_COLOR_CT, "0,164,255");
		string Color_TT = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_COLOR_TT, "255,64,64");
		string Color_VCT = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_COLOR_VCT, "0,255,0");
		string Color_VTT = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_COLOR_VTT, "0,255,0");

		ScanColorFromCvar(Color_CT.c_str(), Radar::rad_Color_CT);
		ScanColorFromCvar(Color_TT.c_str(), Radar::rad_Color_TT);
		ScanColorFromCvar(Color_VCT.c_str(), Radar::rad_Color_VCT);
		ScanColorFromCvar(Color_VTT.c_str(), Radar::rad_Color_VTT);

		Skin::knf_ct_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		Skin::knf_ct_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		Skin::knf_tt_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		Skin::knf_tt_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		Skin::gloves_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(SKIN_TEXT, pWeaponData[i], 0);
		}

		g_SkinChangerCfg[WEAPON_KNIFE].nFallbackSeed = CSX::Cvar::LoadCvar(SKIN_TEXT_SEED, CVAR_SKIN_CT_MODEL, g_SkinChangerCfg[WEAPON_KNIFE].nFallbackSeed);
		g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackSeed = CSX::Cvar::LoadCvar(SKIN_TEXT_SEED, CVAR_SKIN_TT_MODEL, g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackSeed);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackSeed = CSX::Cvar::LoadCvar(SKIN_TEXT_SEED, pWeaponData[i], 0);
		}

		if (Interfaces::Engine()->IsInGame())
			ForceFullUpdate();

		Misc::misc_Bhop = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BHOP, Misc::misc_Bhop) != false;
		Misc::SkeetBar = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_SKEETBAR, Misc::SkeetBar) != false;
		Misc::Blackout = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BLACKOUT, Misc::Blackout) != false;
		Misc::misc_antiafk = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_ANTIAFK, Misc::misc_antiafk) != false;
		Misc::misc_Punch = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_PUNCH, Misc::misc_Punch) != false;
		Misc::misc_Punch2 = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_PUNCH2, Misc::misc_Punch2) != false;
		Misc::misc_AwpAim = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AWPAIM, Misc::misc_AwpAim) != false;
		Misc::misc_NoFlash = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_NOFLASH, Misc::misc_NoFlash) != false;
		Misc::misc_NoSmoke = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_NOSMOKE, Misc::misc_NoSmoke) != false;
		Misc::misc_AutoStrafe = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AUTOSTRAFE, Misc::misc_AutoStrafe) != false;
		Misc::misc_AutoAccept = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AUTOACCEPT, Misc::misc_AutoAccept) != false;
		Misc::misc_Spectators = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_SPECTATORS, Misc::misc_Spectators) != false;
		Misc::misc_FovChanger = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_CHANGER, Misc::misc_FovChanger) != false;
		Misc::misc_FovView = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_VIEW, Misc::misc_FovView);
		Misc::misc_FovModelView = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_MDL_VIEW, Misc::misc_FovModelView);
		Misc::misc_HitMarker = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_HITMARKER, Misc::misc_HitMarker) != false;
		Misc::misc_HitMarkerSound = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_HITMARKERSOUND, Misc::misc_HitMarkerSound);
		Misc::misc_LeftHandKnife = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_LEFTHANDKNIFE, Misc::misc_LeftHandKnife);
		Misc::misc_RightHandKnife = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_RIGHTHANDKNIFE, Misc::misc_RightHandKnife);
		Misc::misc_Duck = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FASTDUCK, Misc::misc_Duck);

		string Color_AWPAIM = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AWPAIM_COLOR, "255,0,0");
		ScanColorFromCvar(Color_AWPAIM.c_str(), Misc::misc_AwpAimColor);
		string Color_THEMECOLOR = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_THEMECOLOR, "255,0,0");
		ScanColorFromCvar(Color_THEMECOLOR.c_str(), Misc::misc_themecolor);

		string Color_HITMARKER = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_HITMARKER_COLOR, "255,0,0");
		ScanColorFromCvar(Color_HITMARKER.c_str(), Misc::misc_HitMarkerColor);
	}

	void SaveSettings(string szIniFile)
	{
		CSX::Cvar::InitPath(szIniFile.c_str());
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_ACTIVE_AIMBOT, Aimbot::aim_Active);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWBACKTRACK, Aimbot::aim_DrawBacktrack);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DEATHMATCH, Aimbot::aim_Deathmatch);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::aim_WallAttack);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::aim_CheckSmoke);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKFLASH, Aimbot::aim_CheckFlash);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::aim_AntiJump);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::aim_DrawFov);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::aim_DrawSpot);
		

		string Color_AIM_FOV =
			to_string(int(Aimbot::aim_FovColor[0] * 255.f)) + "," +
			to_string(int(Aimbot::aim_FovColor[1] * 255.f)) + "," +
			to_string(int(Aimbot::aim_FovColor[2] * 255.f));
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_FOVCOLOR, Color_AIM_FOV);

		//smooth
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Smooth[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Smooth[i]);

		}
		//fov
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Fov[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Fov[i]);
		}
		//aim_FovType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_FovType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			
			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_FovType[i]);

		}

		//int aim_backtrack[6] = { 1,1,1,1,1,1 };
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_backtrack[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);
			
			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_backtrack[i]);

		}
		//aim_RCSType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RCSType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RCSType[i]);

		}
		//aim_Spot	
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Spot[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Spot[i]);

		}
		//aim_Rcs
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Rcs[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Rcs[i]);

		}
		//aim_RcsType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RcsType[i]);

		}
		//AimBot_RcsAfterXShot
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "AimBot_RcsAfterXShot[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::AimBot_RcsAfterXShot[i]);

		}
		//aim_Rcsx
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Rcsx[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Rcsx[i]);

		}
		//aim_Rcsy
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_Rcsy[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_Rcsy[i]);

		}
		//aim_RcsSmooth
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsSmooth[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RcsSmooth[i]);

		}
		//aim_RcsClampType
		for (DWORD i = 0; i < 6; i++) // son 5
		{
			string szStrTmp = "aim_RcsClampType[";
			szStrTmp += to_string(i) + "]";
			char* p = new char[szStrTmp.length() + 1];
			memcpy(p, szStrTmp.c_str(), szStrTmp.length() + 1);

			CSX::Cvar::SaveCvar("Aimbot", p, Aimbot::aim_RcsClampType[i]);

		}
		CSX::Cvar::SaveCvar("Aimbot", "aim_AutoPistol[3]", Settings::Aimbot::aim_AutoPistol[3]);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_STYLE, Settings::Esp::esp_Style);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WPICON, Settings::Esp::esp_WeaponIcon);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_SIZE, Settings::Esp::esp_Size);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_LINE, Settings::Esp::esp_Line);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_OUTLINE, Settings::Esp::esp_Outline);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_NAME, Settings::Esp::esp_Name);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_GRANADEHELPER, Settings::Esp::helper);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_INFO, Settings::Esp::esp_Infoz);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_DEFUSING, Settings::Esp::esp_Defusing);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_SCOPED, Settings::Esp::esp_Scoped);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_FLASHED, Settings::Esp::esp_Flashed);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_RANK, Settings::Esp::esp_Rank);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_HEALTH, Settings::Esp::esp_Health);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_ARMOR, Settings::Esp::esp_Armor);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_AMMO, Settings::Esp::esp_Ammo);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_DISTANCE, Settings::Esp::esp_Distance);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_SKELETON, Settings::Esp::esp_Skeleton);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BULLETTRACE, Settings::Esp::esp_BulletTrace);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_TEAM, Settings::Esp::esp_Team);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_ENEMY, Settings::Esp::esp_Enemy);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_VISIBLE, Settings::Esp::esp_Visible);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_CHAMSVISIBLE, Settings::Esp::esp_ChamsVisible);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_CHAMS, Settings::Esp::esp_Chams);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BOMB, Settings::Esp::esp_Bomb);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BOMBPLANTED, Settings::Esp::esp_BombPlanted);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BOMBTIMER, Settings::Esp::esp_BombTimer);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WORLDWEAPONS, Settings::Esp::esp_WorldWeapons);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WORLDGRENADE, Settings::Esp::esp_WorldGrenade);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WATER, Settings::Esp::esp_Watermark);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_CHEATBUILD, Settings::Esp::esp_Cheatbuild);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_HITLOG, Settings::Esp::esp_hitevent);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_ANIMALS, Settings::Esp::esp_Animals);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_DLIGHT, Settings::Esp::esp_Dlightz);
		string esp_Dlight =
			to_string(int(Settings::Esp::esp_Dlight[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Dlight[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Dlight[2] * 255.f));

		string esp_Color_CT =
			to_string(int(Settings::Esp::esp_Color_CT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_CT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_CT[2] * 255.f));

		string esp_Color_TT =
			to_string(int(Settings::Esp::esp_Color_TT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_TT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_TT[2] * 255.f));

		string esp_Color_VCT =
			to_string(int(Settings::Esp::esp_Color_VCT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_VCT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_VCT[2] * 255.f));

		string esp_Color_VTT =
			to_string(int(Settings::Esp::esp_Color_VTT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_VTT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_VTT[2] * 255.f));

		string GrenadeHelper =
			to_string(int(Settings::Esp::GrenadeHelper[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::GrenadeHelper[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::GrenadeHelper[2] * 255.f));

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_CT, esp_Color_CT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_TT, esp_Color_TT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_VCT, esp_Color_VCT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_VTT, esp_Color_VTT);

		string chams_Color_CT =
			to_string(int(Settings::Esp::chams_Color_CT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_CT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_CT[2] * 255.f));

		string chams_Color_TT =
			to_string(int(Settings::Esp::chams_Color_TT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_TT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_TT[2] * 255.f));

		string chams_Color_VCT =
			to_string(int(Settings::Esp::chams_Color_VCT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_VCT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_VCT[2] * 255.f));

		string chams_Color_VTT =
			to_string(int(Settings::Esp::chams_Color_VTT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_VTT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_VTT[2] * 255.f));

		string WorldWeapon_Color =
			to_string(int(Settings::Esp::WorldWeapon_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::WorldWeapon_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::WorldWeapon_Color[2] * 255.f));

		string Visuals_Line_Color =
			to_string(int(Settings::Esp::Visuals_Line_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::Visuals_Line_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::Visuals_Line_Color[2] * 255.f));

		string Name_Color =
			to_string(int(Settings::Esp::Name_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::Name_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::Name_Color[2] * 255.f));

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_CT, chams_Color_CT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_TT, chams_Color_TT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_VCT, chams_Color_VCT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_VTT, chams_Color_VTT);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_WORLDWEAPON, WorldWeapon_Color);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_LINE, Visuals_Line_Color);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_NAME, Name_Color);

		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_ENABLE, Radar::Radar_Enable);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_ACTIVE, Radar::rad_Active);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_TEAM, Radar::rad_Team);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_ENEMY, Radar::rad_Enemy);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_INGAME, Radar::rad_InGame);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_INGAMEKEY, Radar::rad_InGameKey);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_RANGE, Radar::rad_Range);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_ALPHA, Radar::rad_Alpha);

		string Color_CT =
			to_string(int(Radar::rad_Color_CT[0] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_CT[1] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_CT[2] * 255.f));

		string Color_TT =
			to_string(int(Radar::rad_Color_TT[0] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_TT[1] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_TT[2] * 255.f));

		string Color_VCT =
			to_string(int(Radar::rad_Color_VCT[0] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_VCT[1] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_VCT[2] * 255.f));

		string Color_VTT =
			to_string(int(Radar::rad_Color_VTT[0] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_VTT[1] * 255.f)) + "," +
			to_string(int(Radar::rad_Color_VTT[2] * 255.f));

		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_COLOR_CT, Color_CT);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_COLOR_TT, Color_TT);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_COLOR_VCT, Color_VCT);
		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_COLOR_VTT, Color_VTT);

		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			CSX::Cvar::SaveCvar(SKIN_TEXT, pWeaponData[i], g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit);
		}

		CSX::Cvar::SaveCvar(SKIN_TEXT_SEED, CVAR_SKIN_CT_MODEL, g_SkinChangerCfg[WEAPON_KNIFE].nFallbackSeed);
		CSX::Cvar::SaveCvar(SKIN_TEXT_SEED, CVAR_SKIN_TT_MODEL, g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackSeed);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			CSX::Cvar::SaveCvar(SKIN_TEXT_SEED, pWeaponData[i], g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackSeed);
		}

		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BHOP, Misc::misc_Bhop);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_SKEETBAR, Misc::SkeetBar);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BLACKOUT, Misc::Blackout);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_ANTIAFK, Misc::misc_antiafk);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_PUNCH, Misc::misc_Punch);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_PUNCH2, Misc::misc_Punch2);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_NOFLASH, Misc::misc_NoFlash);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_NOSMOKE, Misc::misc_NoSmoke);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AWPAIM, Misc::misc_AwpAim);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AUTOSTRAFE, Misc::misc_AutoStrafe);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AUTOACCEPT, Misc::misc_AutoAccept);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_SPECTATORS, Misc::misc_Spectators);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_CHANGER, Misc::misc_FovChanger);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_VIEW, Misc::misc_FovView);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_MDL_VIEW, Misc::misc_FovModelView);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_HITMARKER, Settings::Misc::misc_HitMarker);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_HITMARKERSOUND, Settings::Misc::misc_HitMarkerSound);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_LEFTHANDKNIFE, Settings::Misc::misc_LeftHandKnife);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_RIGHTHANDKNIFE, Settings::Misc::misc_RightHandKnife);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FASTDUCK, Settings::Misc::misc_Duck);


		string Color_AWP_AIM =
			to_string(int(Misc::misc_AwpAimColor[0] * 255.f)) + "," +
			to_string(int(Misc::misc_AwpAimColor[1] * 255.f)) + "," +
			to_string(int(Misc::misc_AwpAimColor[2] * 255.f));
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AWPAIM_COLOR, Color_AWP_AIM);

		string Color_THEMECOLOR =
			to_string(int(Misc::misc_themecolor[0] * 255.f)) + "," +
			to_string(int(Misc::misc_themecolor[1] * 255.f)) + "," +
			to_string(int(Misc::misc_themecolor[2] * 255.f));
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_THEMECOLOR, Color_THEMECOLOR);

		string Color_HITMARKER =
			to_string(int(Misc::misc_HitMarkerColor[0] * 255.f)) + "," +
			to_string(int(Misc::misc_HitMarkerColor[1] * 255.f)) + "," +
			to_string(int(Misc::misc_HitMarkerColor[2] * 255.f));
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_HITMARKER_COLOR, Color_HITMARKER);
	}

	float hitmarkerAlpha;

	namespace Aimbot
	{
		string WeaponStr = "pistola";
		bool aim_Active = true;
		bool aim_Backtrack = true;
		bool aim_DrawBacktrack = false;
		int aim_Backtracktickrate = 1;
		bool aim_Deathmatch = false;
		bool aim_WallAttack = false;
		bool aim_CheckSmoke = false;
		bool aim_CheckFlash = false;
		bool aim_AntiJump = false;
		
		bool aim_DrawFov = false;
		bool aim_DrawSpot = false;
		bool aim_KillDelay = false;
		int aim_KillDelayTime = 0;
		bool aim_OnKey = false;
		int aim_Key = 1;
		float aim_FovColor[3] = { 0.f,0.0f,0.f };
		weapon_aim_s weapon_aim_settings[34] = { 0 };

		int aim_Smooth[6] = { 300,300,300,300,300,300 };
		int aim_Fov[6] = { 0,0,0,0,0,0 };
		int aim_FovType[6] = { 0,0,0,0,0,0 };
		int aim_RCSType[6] = { 0,0,0,0,0,0 };
		int AimBot_RcsAfterXShot[6] = { 0,0,0,0,0,0 };
		int aim_BestHit[6]= { 0,0,0,0,0,0 };
		int aim_Spot[6] = { 0,0,0,0,0,0 };
		int aim_Delay[6];
		int aim_Rcs[6]= { 0,0,0,0,0,0 };
		int aim_RcsAfterXShot[6]= { 0,0,0,0,0,0 };
		int aim_Rcsx[6]= { 0,0,0,0,0,0 };
		int aim_Rcsy[6]= { 0,0,0,0,0,0 };
		int aim_RcsFov[6]= { 0,0,0,0,0,0 };
		int aim_RcsSmooth[6] = { 300,300,300,300,300,300 };
		int aim_RcsClampType[6] = { 0,0,0,0,0,0 };
		int aim_backtrack[6] = {1,1,1,1,1,1};
		int aim_Globalbacktrack = 1;
		bool aim_AutoPistol[6] = { 0,0,0,0,0,0 };
		int aim_RcsType[6] = { 0,0,0,0,0,0 };


	}

	namespace Esp
	{
		int esp_Style = 0; // 0 - Box 1 - CoalBox
		int esp_Size = 1;
		bool esp_Line = false;
		bool esp_Outline = false; // Box ( 0 - Box 1 - OutlineBox ) ,
								  // CoalBox ( 0 - CoalBox 1 - OutlineCoalBox )	
		int esp_rendergrenadecheck = 0;
		float Hands[3] = { 0.f,0.0f,0.f };
		bool esp_Name = false;
		bool esp_Rank = false;
		int esp_Health = 0;
		int esp_Armor = 0;
		int esp_Chams_Light = 0;
		bool esp_Glow = false;
		int esp_WeaponIcon = 0;
		bool esp_Ammo = false;
		bool esp_Distance = false;
		bool esp_HitMarker = false;
		bool esp_Dlightz = false;
		bool esp_AsusWalls = false;
		int esp_WallsOpacity = 100;
		bool esp_NightMode = false;
		bool esp_Infoz = true;
		bool esp_Defusing = false;
		bool esp_Scoped = false;
		bool esp_Flashed = false;

		float esp_Ambient[3];
		float esp_Dlight[3];
		bool esp_Animals= false;

		bool esp_Skeleton = 0;
		int esp_BulletTrace = 0;
		bool esp_Team = 0;
		bool esp_Enemy = 0;
		bool esp_Visible = 0;
		bool esp_ChamsVisible = 0;

		int esp_Chams = 0;
		bool esp_Bomb = false;
		bool esp_BombPlanted = false;
		int esp_BombTimer = 40;
		bool esp_WorldWeapons = false;
		bool esp_WorldGrenade = false;

		float WorldWeapon_Color[3] = { 0.f,0.0f,0.f };
		float Visuals_Line_Color[3] = { 0.f,0.0f,0.f };
		float Name_Color[3] = { 0.f,0.0f,0.f };

		float esp_Color_CT[3] = { 0.f,0.0f,0.f };
		float esp_Color_TT[3] = { 0.f,0.0f,0.f };
		float esp_Color_VCT[3] = { 0.f,0.0f,0.f };
		float esp_Color_VTT[3] = { 0.f,0.0f,0.f };

		bool helper = false;
		float GrenadeHelper[3] = { 0.f,0.f,0.f };

		float chams_Color_CT[3] = { 0.f,0.0f,0.f };
		float chams_Color_TT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VCT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VTT[3] = { 0.f,0.0f,0.f };
		float flTracer_Beam[3] = { 200.f,0.0f,255.f };
		bool esp_beams_tracer = false;
		float flTracersDuration;
		float flTracersWidth;
		bool esp_Watermark = true;
		bool esp_Cheatbuild = true;
		bool esp_hitevent = false;

	}

	namespace Radar
	{
		bool Radar_Enable = false;
		bool rad_Active = false;
		bool rad_Team = false;
		bool rad_Enemy = false;
		bool rad_InGame = false;
		int rad_InGameKey = 0;
		int rad_Range = 3500;
		int rad_Alpha = 255;
		float rad_Color_CT[3] = { 0.f,0.0f,0.f };
		float rad_Color_TT[3] = { 0.f,0.0f,0.f };
		float rad_Color_VCT[3] = { 0.f,0.f,0.f };
		float rad_Color_VTT[3] = { 0.f,0.f,0.f };
	}

	namespace Skin
	{
		int knf_ct_model = 0;
		int knf_ct_skin = 0;
		int knf_tt_model = 0;
		int knf_tt_skin = 0;
		int gloves_skin = 0;
	}

	namespace Misc
	{
		QAngle qLastTickAngle;
		const char* misc_SkyName;

		bool SkeetBar = true;
		bool Autoaccept = false;
		bool Blackout = true;
		int misc_Clan = 0;
		bool misc_antiafk = false;
		int misc_CurrentSky;
		bool misc_NoSky = false;
		bool Ragdoll = false;
		bool misc_Snow = false;
		bool misc_Duck = false;
		bool misc_ChamsHands = false;
		bool misc_ChamsWeapon = false;
		bool misc_EPostprocess = false;
		bool ShowSpread = false;
		bool misc_Postprocess = false;
		bool misc_Bhop = false;
		bool misc_EdgeJump = false;
		bool misc_LeftHandKnife = false;
		bool misc_RightHandKnife = false;
		bool misc_Punch = false;
		bool misc_Punch2 = false;
		bool misc_NoFlash = false;
		bool misc_NoSmoke = false;
		bool misc_AwpAim = false;
		bool misc_AutoStrafe = false;
		bool misc_AutoAccept = false;

		bool misc_Spectators = false;
		bool misc_killspam = false;
		bool misc_FovChanger = false; 
		int misc_FovView = 90;
		int misc_FovModelView = 60;
		float misc_AwpAimColor[3] = { 0.f,0.f,0.f };
		float misc_themecolor[3] = { 0.f,0.f,0.f };
		bool misc_HitMarker = false;
		float hitmarkerAlpha;
		int misc_HitMarkerSound = 0;
		float misc_HitMarkerColor[3] = { 0.f, 0.f, 0.f };
		int KillDelayVar = 0;
		bool KillDelayBool2 = false;
	}
}










































































































































































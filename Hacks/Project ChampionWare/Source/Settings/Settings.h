#pragma once

#include "../Main/Main.h"

#define FIX_MIN_FOV_HEAD	30
#define FIX_MIN_FOV_CHEST	50
#define FOV_BASE_DISTANCE	200

namespace Settings
{
	void LoadSettings( string szIniFile );
	void SaveSettings( string szIniFile );
	extern float hitmarkerAlpha;

	namespace Aimbot
	{
		struct weapon_aim_s
		{
			//[swap_lines]
			bool aim_Active;
			int aim_Smooth;
			int aim_Fov;
			int aim_FovType;
			int aim_RCSType;
			int AimBot_RcsAfterXShot;
			int aim_BestHit;
			int aim_Spot;
			int aim_Delay;
			int aim_Rcs;
			int aim_RcsAfterXShot;
			int aim_Rcsx;
			int aim_Rcsy;
			int aim_RcsFov;
			int aim_RcsSmooth;
			int aim_RcsClampType;
			int aim_backtrack;
			bool aim_AutoPistol;
			//[/swap_lines]
		};
		///Modificacion para usar armas segun tipo
		//

		extern int aim_Smooth[6];
		extern int aim_Fov[6];
		extern int aim_FovType[6];
		extern int aim_RCSType[6];
		extern int AimBot_RcsAfterXShot[6];
		extern int aim_BestHit[6];
		extern int aim_Spot[6];
		extern int aim_Delay[6];
		extern int aim_Rcs[6];
		extern int aim_RcsAfterXShot[6];
		extern int aim_Rcsx[6];
		extern int aim_Rcsy[6];
		extern int aim_RcsFov[6];
		extern int aim_RcsSmooth[6];
		extern int aim_RcsClampType[6];
		extern int aim_backtrack[6];
		extern int aim_Globalbacktrack;
		extern bool aim_AutoPistol[6];
		extern int aim_RcsType[6];
		

		//
		////////////////////
		extern bool aim_Active;
		extern string WeaponStr;
		extern bool aim_Backtrack;
		extern bool aim_DrawBacktrack;
		extern int aim_Backtracktickrate;
		extern bool aim_Deathmatch;
		extern bool aim_WallAttack;
		extern bool aim_CheckSmoke;
		extern bool aim_CheckFlash;
		extern bool aim_AntiJump;
		extern bool aim_DrawFov;
		extern bool aim_DrawSpot;
		extern bool aim_KillDelay;
		extern int aim_KillDelayTime;
		extern bool aim_OnKey;
		extern int aim_Key;
		extern float aim_FovColor[3];
		
		extern weapon_aim_s weapon_aim_settings[34];
		//[/swap_lines]
	}

	namespace Esp
	{
		//[swap_lines]
		extern int esp_Style; // 0 - Box 1 - CoalBox
		extern int esp_Size;
		extern bool esp_Line;
		extern bool esp_Glow;
		extern bool esp_HitMarker;
		extern bool esp_Outline; // Box ( 0 - Box 1 - OutlineBox ) ,
		extern int esp_rendergrenadecheck;
		extern bool esp_AsusWalls;
		extern int esp_WallsOpacity;
		extern float Hands[3];
		extern bool esp_HealthChams;
		extern bool esp_Name;
		extern bool esp_Rank;
		extern int esp_Health;
		extern int esp_Armor;
		extern int esp_Chams_Light;
		extern int esp_WeaponIcon;
		extern bool esp_Ammo;
		extern bool esp_Distance;
		extern bool esp_Dlightz;

		extern float esp_Ambient[3];
		extern float esp_Dlight[3];
		extern bool esp_NightMode;
		extern bool esp_Infoz;
		extern bool esp_Defusing;
		extern bool esp_Scoped;
		extern bool esp_Flashed;

		extern bool esp_Skeleton;
		extern int esp_BulletTrace;
		extern bool esp_Team;
		extern bool esp_Enemy;
		//extern int esp_Visible;
		extern bool esp_Visible;
		//extern int esp_ChamsVisible;
		extern bool esp_ChamsVisible;

		extern int esp_Chams;
		extern bool esp_Bomb;
		extern bool esp_BombPlanted;
		extern int esp_BombTimer;
		extern bool esp_WorldWeapons;
		extern bool esp_WorldGrenade;
		
		extern bool Visuals_WorldWeapons;

		extern float WorldWeapon_Color[3];
		extern float Visuals_Line_Color[3];
		extern float Name_Color[3];
		
		extern bool esp_Animals;

		extern float esp_Color_CT[3];
		extern float esp_Color_TT[3];
		extern float esp_Color_VCT[3];
		extern float esp_Color_VTT[3];

		extern bool helper;
		extern float GrenadeHelper[3];

		extern float chams_Color_CT[3];
		extern float chams_Color_TT[3];
		extern float chams_Color_VCT[3];
		extern float chams_Color_VTT[3];
		extern float flTracer_Beam[3];
		extern bool esp_beams_tracer;
		extern float flTracersDuration;
		extern float flTracersWidth;
		extern bool esp_Watermark;
		extern bool esp_Cheatbuild;
		extern bool esp_hitevent;
		//[/swap_lines]
	}

	namespace Radar
	{
		//[swap_lines]
		extern bool Radar_Enable;
		extern bool rad_Active;
		extern bool rad_Team;
		extern bool rad_Enemy;
		extern bool rad_InGame;
		extern int rad_InGameKey;
		extern int rad_Range;
		extern int rad_Alpha;
		extern float rad_Color_CT[3];
		extern float rad_Color_TT[3];
		extern float rad_Color_VCT[3];
		extern float rad_Color_VTT[3];
		//[/swap_lines]
	}

	namespace Skin
	{
		//[swap_lines]
		extern int knf_ct_model;
		extern int knf_ct_skin;
		extern int knf_tt_model;
		extern int knf_tt_skin;
		extern int gloves_skin;
		//[/swap_lines]
	}

	namespace Misc
	{
		//[swap_lines]
		extern QAngle qLastTickAngle;
		extern const char* misc_SkyName;
		extern bool SkeetBar;
		extern bool Autoaccept;
		extern bool Blackout;
		extern bool misc_antiafk;
		extern bool ShowSpread;
		extern int misc_Clan;
		extern int misc_CurrentSky;
		extern bool misc_NoSky;
		extern bool Ragdoll;
		extern bool misc_killspam;
		extern bool misc_Snow;
		extern bool misc_Duck;
		extern bool misc_EPostprocess;
		extern bool misc_Postprocess;
		extern bool misc_Bhop;
		extern bool misc_ChamsHands;
		extern bool misc_ChamsWeapon;
		extern bool misc_EdgeJump;
		extern bool misc_LeftHandKnife;
		extern bool misc_RightHandKnife;
		extern bool misc_Punch;
		extern bool misc_Punch2;
		extern bool misc_AwpAim;
		extern bool misc_NoFlash;
		extern bool misc_NoSmoke;
		extern bool misc_AutoStrafe;
		extern bool misc_AutoAccept;
		extern bool misc_Spectators;
		extern bool misc_FovChanger;
		extern int misc_FovView;
		extern int misc_FovModelView;
		extern float misc_AwpAimColor[3];
		extern float misc_themecolor[3];
		extern float hitmarkerAlpha;
		extern bool misc_HitMarker;
		extern int misc_HitMarkerSound;
		extern float misc_HitMarkerColor[3];
		extern int KillDelayVar;
		extern bool KillDelayBool2;
		//[/swap_lines]
	}
}

//[enc_string_enable /]

#define HACK_NAME "www.Aimware.cc"

#define AIMBOT_TEXT "Aimbot"
#define CONFIG_TEXT "#"
#define VISUAL_TEXT "%"
#define RADAR_TEXT "Radar"
#define SKIN_TEXT "?"
#define SKIN_TEXT_SEED "Seed"
#define MISC_TEXT "&"

#define CVAR_AIMBOT_ACTIVE_AIMBOT "aim_Active_aimbot"
#define CVAR_AIMBOT_BACKTRACK "aim_Backtrake"
#define CVAR_AIMBOT_DRAWBACKTRACK "aim_DrawBacktrack"
#define CVAR_AIMBOT_BACKTRACKTICKRATE "aim_BacktrackTickrate"

#define CVAR_AIMBOT_DEATHMATCH "aim_FriendlyFire"
#define CVAR_AIMBOT_WALLATTACK "aim_WallAttack"
#define CVAR_AIMBOT_CHECKSMOKE "aim_CheckSmoke"
#define CVAR_AIMBOT_CHECKFLASH "aim_CheckFlash"
#define CVAR_AIMBOT_ANTIJUMP	"aim_IgnoreJump"
#define CVAR_AIMBOT_RCSTYPE "aim_RcsType"
#define CVAR_AIMBOT_DRAWFOV "aim_DrawFov"
#define CVAR_AIMBOT_FOVCOLOR "aim_FovColor"
#define CVAR_AIMBOT_DRAWSPOT "aim_DrawSpot"
#define CVAR_AIMBOT_KILLDELAY "aim_KillDelay"
#define CVAR_AIMBOT_KILLDELAYTIME "aim_KillDelayTime"
#define CVAR_AIMBOT_ONKEY "aim_OnKey"
#define CVAR_AIMBOT_KEY "aim_Key"

#define CVAR_AIMBOT_ACTIVE "aim_Active"
#define CVAR_AIMBOT_SMOOTH "aim_Smooth"
#define CVAR_AIMBOT_FOV "aim_Fov"
#define CVAR_AIMBOT_BACKTRACK "aim_backtrack"
#define CVAR_AIMBOT_FOVTYPE "aim_FovType"
#define CVAR_AIMBOT_BESTHIT "aim_BestHit"
#define CVAR_AIMBOT_SPOT "aim_Spot"
#define CVAR_AIMBOT_DELAY "aim_Delay"

#define CVAR_AIMBOT_RCSTYPEXY "aim_RCSType"
#define CVAR_AIMBOT_RCS "aim_Rcs"
#define CVAR_AIMBOT_RCSX "aim_RcsX"
#define CVAR_AIMBOT_RCSY "aim_RcsY"
#define CVAR_AIMBOT_RCSFOV "aim_RcsFov"
#define CVAR_AIMBOT_RCSSMOOTH "aim_RcsSmooth"
#define CVAR_AIMBOT_RCSCLAMPTYPE "aim_RcsClampType"
#define CVAR_AIMBOT_AUTOPISTOL "aim_AutoPistol"

#define CVAR_KEY_MOUSE3 "MOUSE3"
#define CVAR_KEY_MOUSE4 "MOUSE4"
#define CVAR_KEY_MOUSE5 "MOUSE5"

#define CVAR_ESP_STYLE "esp_Style"
#define CVAR_ESP_WPICON "esp_WeaponIcon"
#define CVAR_ESP_SIZE "esp_Size"
#define CVAR_ESP_LINE "esp_Line"
#define CVAR_ESP_OUTLINE "esp_Outline"
#define CVAR_ESP_NAME "esp_Name"
#define CVAR_ESP_GRANADEHELPER "helper"
#define CVAR_ESP_RANK "esp_Rank"
#define CVAR_ESP_INFO "esp_Infoz"
#define CVAR_ESP_DEFUSING "esp_Defusing"
#define CVAR_ESP_SCOPED "esp_Scoped"
#define CVAR_ESP_FLASHED "esp_Flashed"

#define CVAR_ESP_HEALTH "esp_Health"
#define CVAR_ESP_ARMOR "esp_Armor"
#define CVAR_ESP_WEAPON "esp_Weapon"
#define CVAR_ESP_AMMO "esp_Ammo"
#define CVAR_ESP_DISTANCE "esp_Distance"
#define CVAR_ESP_DLIGHT "esp_Dlightz"
#define CVAR_ESP_COLOR_DLIGHT "esp_Dlight"

#define CVAR_ESP_SKELETON "esp_Skeleton"
#define CVAR_ESP_BULLETTRACE "esp_BulletTrace"
#define CVAR_ESP_TEAM "esp_Team"
#define CVAR_ESP_ENEMY "esp_Enemy"
#define CVAR_ESP_VISIBLE "esp_Visible"
#define CVAR_ESP_CHAMSVISIBLE "esp_ChamsVisible"

#define CVAR_ESP_CHAMS "esp_Chams"
#define CVAR_ESP_BOMB "esp_Bomb"
#define CVAR_ESP_BOMBPLANTED "esp_BombPlanted"
#define CVAR_ESP_BOMBTIMER "esp_BombTimer"
#define CVAR_ESP_WORLDWEAPONS "esp_WorldWeapons"
#define CVAR_ESP_WORLDGRENADE "esp_WorldGrenade"

#define CVAR_ESP_WATER "esp_Watermark"
#define CVAR_ESP_CHEATBUILD "esp_Cheatbuild"
#define CVAR_ESP_HITLOG "esp_Hitlog"
#define CVAR_ESP_ANIMALS "esp_Animals"

#define CVAR_ESP_COLOR_WORLDWEAPON "WorldWeapon_Color"
#define CVAR_ESP_COLOR_LINE "Visuals_Line_Color"
#define CVAR_ESP_COLOR_NAME "Name_Color"

#define CVAR_ESP_COLOR_CT "esp_Color_CT"
#define CVAR_ESP_COLOR_TT "esp_Color_TT"
#define CVAR_ESP_COLOR_VCT "esp_Color_VCT"
#define CVAR_ESP_COLOR_VTT "esp_Color_VTT"

#define CVAR_CHAMS_COLOR_CT "chams_Color_CT"
#define CVAR_CHAMS_COLOR_TT "chams_Color_TT"
#define CVAR_CHAMS_COLOR_VCT "chams_Color_VCT"
#define CVAR_CHAMS_COLOR_VTT "chams_Color_VTT"

#define CVAR_ESP_COLOR_GrenadeHelper "GrenadeHelper"

#define C4_TIMER_STRING "C4 Timer: %0.2f"

#define CVAR_RAD_ENABLE "Radar_Enable"
#define CVAR_RAD_ACTIVE "rad_Active"
#define CVAR_RAD_TEAM "rad_Team"
#define CVAR_RAD_ENEMY "rad_Enemy"
#define CVAR_RAD_INGAME "rad_InGame"
#define CVAR_RAD_INGAMEKEY "rad_InGameKey"
#define CVAR_RAD_RANGE "rad_Range"
#define CVAR_RAD_ALPHA "rad_Alpha"

#define CVAR_RAD_COLOR_CT "rad_Color_CT"
#define CVAR_RAD_COLOR_TT "rad_Color_TT"
#define CVAR_RAD_COLOR_VCT "rad_Color_VCT"
#define CVAR_RAD_COLOR_VTT "rad_Color_VTT"

#define CVAR_SKIN_CT_MODEL "knf_ct_model"
#define CVAR_SKIN_CT_SKIN "knf_ct_skin"
#define CVAR_SKIN_TT_MODEL "knf_tt_model"
#define CVAR_SKIN_TT_SKIN "knf_tt_skin"
#define CVAR_SKIN_GLOVES "gloves_skin"

#define CVAR_MISC_BHOP "misc_Bhop"
#define CVAR_MISC_SKEETBAR "SkeetBar"
#define CVAR_MISC_BLACKOUT "Blackout"
#define CVAR_MISC_ANTIAFK "misc_antiafk"
#define CVAR_MISC_PUNCH "misc_Punch"
#define CVAR_MISC_PUNCH2 "misc_Punch2"
#define CVAR_MISC_AWPAIM "misc_AwpAim"
#define CVAR_MISC_NOFLASH "misc_NoFlash"
#define CVAR_MISC_NOSMOKE "misc_NoSmoke"
#define CVAR_MISC_AUTOSTRAFE "misc_AutoStrafe"
#define CVAR_MISC_AUTOACCEPT "misc_AutoAccept"
#define CVAR_MISC_SPECTATORS "misc_Spectators"
#define CVAR_MISC_FOV_CHANGER "misc_FovChanger"
#define CVAR_MISC_FOV_VIEW "misc_FovView"
#define CVAR_MISC_FOV_MDL_VIEW "misc_FovModelView"
#define CVAR_MISC_AWPAIM_COLOR "misc_AwpAimColor"
#define CVAR_MISC_THEMECOLOR "misc_themecolor"
#define CVAR_MISC_HITMARKER "misc_HitMarker"
#define CVAR_MISC_HITMARKER_COLOR "misc_HitMarkerColor"
#define CVAR_MISC_HITMARKERSOUND "misc_HitMarkerSound"
#define CVAR_MISC_LEFTHANDKNIFE "misc_LeftHandKnife"
#define CVAR_MISC_RIGHTHANDKNIFE "misc_RightHandKnife"
#define CVAR_MISC_FASTDUCK "misc_Duck"
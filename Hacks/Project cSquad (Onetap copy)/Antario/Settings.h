#pragma once
#include "GUI\GUI.h"
#include <filesystem>
using namespace ui; 
namespace fs = std::filesystem;

class Settings
{
public:
    void Initialize(MenuMain* pMenuObj);

    void SaveSettings(const std::string& strFileName, MenuMain* pMenuObj);
    void LoadSettings(const std::string& strFileName, MenuMain* pMenuObj);

private:
    void UpdateDirectoryContent(const fs::path& fsPath);
    inline fs::path GetFolderPath();

    fs::path                 fsPath{};
    std::vector<std::string> vecFileNames{};

public:
    /* All our settings variables will be here  *
    * The best would be if they'd get          *
    * initialized in the class itself.         */

	float chamsvisible[4] = { 60 / 255.f, 120 / 255.f, 180 / 255.f, 150 / 255.f };
	float chamshidden[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 255 / 255.f };

	float glowcolorlocal[4] = {255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f };
	float glowcolorother[4] = {180 / 255.f, 60 / 255.f, 120 / 255.f, 170 / 255.f };
	

	float boxcolor[4] = {1, 1, 1, 1 };
	float namecolor[4] = {1, 1, 1, 0.8f };
	float skeletoncolor[4] = {255 / 255.f, 255 / 255.f, 170 / 255.f, 220 / 255.f };
	float ammocolor[4] = {80 / 255.f, 140 / 255.f, 200 / 255.f, 220 / 255.f };
	float projectilecolor[4] = {230 / 255.f, 230 / 255.f, 230 / 255.f, 220 / 255.f };
	float projectiledroppedcolor[4] = {230 / 255.f, 230 / 255.f, 230 / 255.f, 220 / 255.f };
	float bullettracer[4] = {150 / 255.f, 130 / 255.f, 255 / 255.f, 255 / 255.f };
	float impact[4] = {255 / 255.f, 127 / 255.f, 4 / 255.f, 100 / 255.f };
	float droppedweaponname[4] = {230 / 255.f, 230 / 255.f, 230 / 255.f, 220 / 255.f };
	//int weapon_skin[skin_changer.NUM_WEAPONS];
	//int weapon_quality[skin_changer.NUM_WEAPONS];
//	float weapon_wear[skin_changer.NUM_WEAPONS];
//	float weapon_seed[skin_changer.NUM_WEAPONS];
	//bool weapon_stat_trak_enabled[skin_changer.NUM_WEAPONS];
//	float weapon_stat_trak_kills[skin_changer.NUM_WEAPONS];
	//char weapon_custom_name[skin_changer.NUM_WEAPONS][PPGUI::PPGUI_MAX_STRING_LENGTH];

    bool  bCheatActive = true;
    bool  bMenuOpened  = true;
    bool  bShowBoxes   = true;
    bool  bShowNames   = true;
	bool  bShowHealth = true;
	bool  bShowChams   = true;
	bool  bShowChamsXQZ = true;
	bool  bShowLocalChams = false;
	bool  bShowLocalChamsXQZ = false;
	bool  bShowGlow    = true;
	bool  bShowAmmo = true;
	bool  bShowSkeleton = false;
	bool  bShowTeammates = false;
	bool  bShowWeaponName = true;
	bool  bShowWeaponIcon = true;
	bool  bShowProjectiles = true;
	bool  bShowDormant = false;
	int   iHitSound = 1;

	bool bHitboxSelection[7] = {
		true,
		false,
		false,
		true,
		false,
		false,
		true
	};
	bool bMultiPointSelection[7]= {
		true,
		false,
		false,
		true,
		false,
		false,
		true
	};
	bool bFakeLagFlags[4] = {false, true, true, false};
	bool bDroppedItems[2];
	bool bItemGlow[2];
	bool bBulletTracers = false;
	bool bBulletImpacts = false;
	bool bKnifeLeft = false;
	bool bFixLegMovement = false;
	bool bSlideWalk = true;
	bool bFreezeWalk = true;
	bool bSlowWalk = true;
	int iSlowWalkKey = VK_LSHIFT;
	bool bSpreadScope = false;
	bool bThirdPersonAlive = true;
	bool bSilentAim = true;
	bool bAutoFire  = true;
	bool bFriendlyFire = false;
	bool bAimBotEnabled = true;
	bool bForceUpdate;
	bool bPreferBodyAim = false;
	bool bAutoWallEnabled = false;
	bool bRemoveRecoil = true;
	bool bBhopEnabled = true;
	bool bAutomaticWeapons = true;
	bool bAutomaticRevolver = true;
	bool bResolver = true;
	bool bLagComp = true;
	bool bAutoWallCrosshair = false;
	float cWatermark[4] = { 0.8, 0, 0, 0.6};
	float cMenuCol[4] = {0.8, 0, 0, 1 };
	bool bNoFlash = true;
	bool bNoPostProcess = true;
	bool bEngineCrosshair = true;
	bool bNoSmoke = true;
	bool bAirStrafer = true;
	bool bAntiAim = true;
	bool AntiUT = true;
	bool bThirdPersonDead = true;
	bool bPreserveKillFeed = false;
	float flTransparentWorld = 100.f;
	float flTransparentProps = 100.f;
	float flWorldFov = 0.f;
	float flViewModelFov = 0.f;
	bool bKillSay = false;
	bool bClantag = false;
	char chClantagName[128];
	bool bDamageLogs = FALSE;
	bool bSkinChangerWindow = false;
	bool bScopeNoZoom = true;
	bool bNoZoom = true;
	bool bNoVisRecoil = true;
	bool bHitmaker = true;
	bool bUselessInfo = false;
	int  iMinSpeedAmmount = 0;
	int  iBodyKey = 0;
	bool bFakeLagShooting = false;
	bool bFakeLagBreak = FALSE;
	bool bAutoStop = true;
	bool bAutoScope = true;
	bool bAutoRevolver = true;
	int iDelayShot = 0;
	int iBodyAimIfxDamage = 0;
	int iChamsMode = 1;
	int iImpactTime = 4;
	int iChamsModeTeam = 0;	
	int iLocalChamsMode = 0;
	int iGlowStyle = 1;
	int iLocalGlowStyle = 0;
	bool bNightMode = false;
	bool bNoSky = false;
	int iMinDamage = 30;
	int iMinAutoWallDamage = 0;
	int iMaxFov = 0;
	int iResolver = 1;
	int iHitChance = 60;
	int iHitScanMode = 1;
	int iMultiPointAmount = 0;
	int iBodyAimOptions = 0;
	int iHitChanceType = 1;
	int iFakeDuckKey = VK_CAPITAL;
	int iThirdPersonKey = VK_XBUTTON1;
	int iThirdPersonDistance = 100;
	int iScopedBlend = 50;
	int iEntBlend = 100;
	int iFakeLagType = 1;
	int iFakeLagChoke = 14;
	int DesyncType = 2;
	int iPitch = 1;
	int iYaw = 2;
	bool bYawAtTargets = true;
	bool bDesync = true;

	int iPointHeadScale;
	int iPointBodyScale;

	int iHeadScale = 50;
	int iNeckScale = 50;
	int iChestScale = 50;
	int iStomachScale = 50;
	int iPelvisScale = 50;
	int iArmsScale = 50;
	int iLegsScale = 50;
	float fontsize = true;

	
	bool bLegitBotMaster = true;
	bool bLegitNoRecoil = false;
	bool bLegitAutoFire = false;
	bool bLegitAimAccBoost = false;
	bool bLegitOnShot = false;
	bool bLegitSilentAim = false;
	float flFovLegit = .5f;
	int iLegitHitBox = 0;
	bool bLegitBacktrack = true;
	int iWeaponType;


	int iKnifeModel = 5;
	bool bKnifeChanger = true;
	bool bSkinChanger = true;
	bool Save(std::string file_name);

	bool Load(std::string file_name);
	
};

extern Settings g_Settings;


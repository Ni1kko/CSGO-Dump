#include "Offsets.h"
#include <map>
#include <string>

std::map<std::string, DWORD> offsets;

namespace Offsets
{
	DWORD m_hMyWeapons = 0x2DF8;
	DWORD m_hMyWearables = 0;
	DWORD m_hViewModel = 0;
	DWORD m_nTickBase = 0x342C;
	DWORD m_flFlashMaxAlpha = 0xA3F0;
	DWORD m_iObserverMode = 0x3374;
	DWORD m_hObserverTarget = 0x3388;

	DWORD deadflag = 0;

	DWORD m_lifeState = 0x25F;
	DWORD m_flFlashDuration = 0xA3F4;

	DWORD m_iAccount = 0;

	DWORD m_bHasHelmet = 0xB334;
	DWORD m_bHasDefuser = 0xB350;

	DWORD m_flDefuseCountDown = 0x29AC;
	DWORD m_flC4Blow = 0x2990;
	DWORD m_hBombDefuser = 0;
	DWORD m_bBombDefused = 0;

	DWORD m_bIsScoped = 0x3910;

	DWORD m_iFOVStart = 0x31E8;
	DWORD m_fFlags = 0x104;

	DWORD m_iHealth = 0x100;
	DWORD m_ArmorValue = 0xB340;
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iShotsFired = 0xA380;

	DWORD m_aimPunchAngle = 0x302C;
	DWORD m_viewPunchAngle = 0x3020;


	DWORD m_flSimulationTime = 0x268;


	DWORD m_vecVelocity = 0x114;
	DWORD m_vecViewOffset = 0x108;
	DWORD m_vecPunchAngles = 0; //0x3038 ?

	DWORD m_vecOrigin = 0x138;

	DWORD m_bIsDefusing = 0x3918;

	DWORD m_angEyeAngles = 0; // X -> 0xB344 Y -> 0xB348

	DWORD m_hActiveWeapon = 0x2EF8;

	DWORD m_iClip1 = 0x3244;
	DWORD m_iClip2 = 0;
	DWORD m_flNextPrimaryAttack = 0x3218;
	DWORD m_iWorldModelIndex = 0;
	DWORD m_bCanReload = 0;

	DWORD m_iItemDefinitionIndex = 0x2FAA;
	DWORD m_iItemIDHigh = 0x2FC0;
	DWORD m_iAccountID = 0x2FC8;
	DWORD m_iEntityQuality = 0x2FAC;
	DWORD m_OriginalOwnerXuidLow = 0x31B0;
	DWORD m_OriginalOwnerXuidHigh = 0x31B4;
	DWORD m_nFallbackPaintKit = 0x31B8;
	DWORD m_flFallbackWear = 0x31C0;
	DWORD m_nFallbackSeed = 0x31BC;
	DWORD m_nFallbackStatTrak = 0x31C4;
	DWORD m_szCustomName = 0x303C;
	DWORD m_hWeaponWorldModel = 0;
	DWORD m_bIsAutoaimTarget = 0;

	DWORD m_nModelIndex = 0;
	DWORD m_iViewModelIndex = 0;
	DWORD m_hOwner = 0x29CC;
	DWORD m_hWeapon = 0;
	DWORD m_bSpotted = 0x93D;

	DWORD m_zoomLevel = 0;

	DWORD KeyValues_KeyValues = 0;
	DWORD KeyValues_LoadFromBuffer = 0;

	DWORD m_CollisionGroup = 0x474;
	DWORD m_rgflCoordinateFrame = 0x444;

	DWORD m_vecMins = 0;
	DWORD m_vecMaxs = 0;
}
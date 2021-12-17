#pragma once

#include "main.h"
#include "dt_recv2.h"

class CNetvarManager
{
public:
	void SetColor( int color );
	int CNetvarManager::GetNetvarOld( char *szClassName, char *szVariable );
};
extern CNetvarManager g_pNetvarManager;

typedef void( *NetvarHookCallback )( const CRecvProxyData* pData, void* pStruct, void* pOut );

class CNetVars
{
public:
	CNetVars( void );
	DWORD GetOffset( const char *tableName, const char *propName );
	bool HookProp( const char *tableName, const char *propName, RecvVarProxyFn fun );
	void* Hook( std::string table_to_hook, std::string prop_to_hook, NetvarHookCallback callback );
	void DumpTable( RecvTable *table );
	RecvTable *GetTable( const char *tableName );
private:
	int GetProp( const char *tableName, const char *propName, RecvProp **prop = 0 );
	int GetProp( RecvTable *recvTable, const char *propName, RecvProp **prop = 0 );
	std::vector<RecvTable*>    m_tables;
};

extern CNetVars *g_pNetVars;

namespace Offsets
{
	extern void				Init( void );

	namespace Player
	{
		extern DWORD	m_iHealth;
		extern DWORD	m_ArmorValue;
		extern DWORD	m_iTeamNum;
		extern DWORD	m_fFlags;
		extern DWORD    m_iAccount;
		extern DWORD    m_szLastPlaceName;
		extern DWORD	m_lifeState;
		extern DWORD	m_bIsDefusing;
		extern DWORD	m_Local;
		extern DWORD    m_bIsPlayerGhost;
		extern DWORD    m_flC4Blow;
		extern DWORD	m_aimPunchAngle;
		extern DWORD    m_aimPunchAngleVel;
		extern DWORD    m_ViewPunchAngle;
		extern DWORD	m_hOwner;
		extern DWORD    m_fEffects;
		extern DWORD    m_angRotation;
		extern DWORD	m_iPlayerC4;
		extern DWORD	m_iCompetitiveRanking;
		extern DWORD	m_flDefuseLength;
		extern DWORD	m_bSpotted;
		extern DWORD	m_bHasDefuser;
		extern DWORD	m_BombTimer;
		extern DWORD	m_MyWeapons;
		extern DWORD	m_bIsScoped;
		extern DWORD	m_iKills;
		extern DWORD	m_vecOrigin;
		extern DWORD	m_flFlashDuration;
		extern DWORD	m_flFlashMaxAlpha;
		extern DWORD	m_vecViewOffset;
		extern DWORD	m_bGunGameImmunity;
		extern DWORD	m_dwWeaponTableIndex;
		extern DWORD	m_nTickBase;
		extern DWORD    m_dwLoadFromBuffer;
		extern DWORD    m_hObserverTarget;
		extern DWORD    m_totalHitsOnServer;
		extern DWORD    m_hConstraintEntity;
		extern DWORD    m_flSimulationTime;
		extern DWORD    m_ZoomLevel;
		extern DWORD    m_pCurrentCommand;
		extern DWORD    m_pPlayerCommand;
		extern DWORD    dwGlowObjectManager;
		extern DWORD    dwSmokeCount;
		extern DWORD    m_flLowerBodyYawTarget;
		extern DWORD	m_hMyWeapons;
		extern DWORD    m_flMaxSpeed;
		extern DWORD    m_flDuckAmount;
		extern DWORD    m_dwSetupBonesRTADD;
		extern DWORD    m_vecBaseVelocity;
		extern DWORD    m_flCycle;
		extern DWORD    m_flPoseParameter;
		extern DWORD    m_nSequence;
		extern DWORD    m_nForceBone;
		extern DWORD	m_hActiveWeapon;
		extern DWORD	m_iShotsFired;
		extern DWORD	m_vecVelocity;
		extern DWORD	m_angEyeAngles;
		extern DWORD    m_flFlashBangTime;
		extern DWORD    m_angEyeAnglesY;
		extern DWORD	m_vecEyeAngles;
		extern DWORD	m_bHasHelmet;
		extern DWORD	m_nHitboxSet;
		extern DWORD	m_clrRender;
		extern DWORD	m_vecAngles;
		extern DWORD	m_iCompetitiveWins;
		extern DWORD    m_iObserverMode;
		extern DWORD    deathflag;
		extern DWORD    m_flFallVelocity;
		extern DWORD    m_vecBaseVelocity;
		extern DWORD    m_flStepSize;
		extern DWORD    m_flMaxspeed;
		extern DWORD    m_bClientSideAnimation;
		extern DWORD    m_bDucking;
		extern DWORD    m_hViewModel;
		extern DWORD    m_hWeapon;
		extern DWORD    m_hWeaponWorldModel;
	}

	namespace Attribute
	{
		extern DWORD m_AttributeManager;
		extern DWORD m_Item;
		extern DWORD m_iItemDefinitionIndex;
		extern DWORD m_iItemIDHigh;
		extern DWORD m_iItemIDLow;
		extern DWORD m_OriginalOwnerXuidLow;
		extern DWORD m_OriginalOwnerXuidHigh;
		extern DWORD m_nFallbackPaintKit;
		extern DWORD m_nFallbackSeed;
		extern DWORD m_flFallbackWear;
		extern DWORD m_nFallbackStatTrak;
		extern DWORD m_iEntityQuality;
		extern DWORD m_szCustomName;
		extern DWORD m_iAccountID;
		extern DWORD m_iViewModelIndex;
		extern DWORD m_iWorldModelIndex;
		extern DWORD m_iWorldDroppedModelIndex;
	}

	namespace Weapon
	{
		extern DWORD	m_iClip1;
		extern DWORD	m_flNextPrimaryAttack;
		extern DWORD    m_flPostponeFireReadyTime;
		extern DWORD	m_iState;
		extern DWORD 	m_iViewModelIndex;
		extern DWORD    m_nModelIndex;
		extern DWORD    m_bIsBroken;
		extern DWORD    m_flNextAttack;
		extern DWORD    m_bPinPulled;
		extern DWORD    m_fThrowTime;
		extern DWORD    m_flRecoilIndex;
		extern DWORD    m_HandlebulletPenetration;
		extern DWORD    m_iPrimaryReserveAmmoCount;
		extern DWORD    m_iSecondaryReserveAmmoCount;
		extern DWORD    m_hWeaponWorldModel;
	}

	namespace Entity
	{
		extern DWORD m_CollisionGroup;
		extern DWORD m_Collision;
		extern DWORD m_vecMins;
		extern DWORD m_vecMaxs;
		extern DWORD m_rgflCoordinateFrame;
		extern DWORD m_fEffects;
	}
}
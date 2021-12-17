#pragma once

#include "main.h"

struct sRageBot
{
	CBaseEntity* pEntity = nullptr;
	Vector vAim = Vector();
	bool bValid = false;
};

class CRage
{
public:
	bool CanTarget( CBaseEntity * pLocal, CUserCmd * pCmd, CBaseCombatWeapon * pWeapon );
	Vector CalculatePoint( CBaseEntity * pLocal, CBaseEntity * pEnemy, CBaseCombatWeapon * pWeapon, int iHitbox, float flMinDamage, matrix3x4_t Matrix[], float &flDamage, int iMultipoints );
	sRageBot GetBestTarget( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd, int iWpnID );
	void GetBestPoint( CBaseEntity * pLocal, CBaseEntity * pEnemy, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd, int iWpnID, Vector & vFinal );
	void RunRage( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd );
	void AutoStop( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd );
	void RunZeus( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd );
	bool bInRage, bInZeus, bShouldStop, bShotTick;
	bool bCanShoot = false;
	bool bWalling;
	int iTarget = NULL;
	int iShotsFired[ 33 ];
	Vector vAimAngle;
	matrix3x4_t* OriginalMatrixPtr[ 33 ];
};

extern CRage g_Rage;
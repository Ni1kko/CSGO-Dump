#pragma once

#include "main.h"

struct sPunchRecord
{
	int m_iTickBase = 0;
	Vector m_vStoredPunch = { };
	Vector m_vStoredPunchVel = { };
	Vector m_vStoredViewOffset = { };
};

class CMisc
{
public:

	bool CanShoot( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon );

	bool IsHitboxVisible( CBaseEntity * pLocal, CBaseEntity * pEntity, Vector vHitSpot, int iHitBox );

	bool IsVisible( CBaseEntity * pLocal, CBaseEntity * pEntity );
	bool IsVisible( CBaseEntity * pLocal, CBaseEntity * pEntity, Vector vEnd );
	bool HittedEntity( CBaseEntity * pLocal, CUserCmd * pCmd );
	bool bInSameTeam(CBaseEntity * pLocal, CBaseEntity * pEnemy);
	float MaxAbsYawDelta(CBaseEntity * pLocal);
	bool InSight( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon );
	void FakeLag( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, bool & bSendPackets, CUserCmd * pCmd );
	void AutoRevolver( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd );
	bool HitChance( CBaseEntity * pLocal, Vector vSpot, CBaseCombatWeapon * pWeapon, CBaseEntity * pEnemy, float flPercentage );


	void RemoveFlash();

	void RemoveSmoke();

	void StorePunch( CBaseEntity * pEntity );

	void FixUpPunch( CBaseEntity * pLocal );

	void PreserverFeed( CBaseEntity * pLocal );

	void HandleBuybot(CBaseEntity* pLocal);

	void DoRagdoll();

	void UpdateLocal(CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd);

	void UpdateLocalAnimations( CBaseEntity * pLocal );

	void FixPVS( CBaseEntity * pLocal );

	void StrafeBot( CUserCmd * pUserCmd, CBaseEntity * pLocal );

	void Movement( CBaseEntity * pLocal, CUserCmd * pCmd );

	void FixTickbase( CUserCmd * pCmd, CBaseEntity * pLocal );

	void ClampWalk(CUserCmd * pCmd, float flSpeed);

	void DoCounterStrafe( CBaseEntity * pLocal, CUserCmd * pCmd );

	void DoThirdperson( CBaseEntity * pLocal );

	void ThirdPersonAngle( CBaseEntity * pLocal );

	void DoKnifeBot( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd );

	void ClanTag(CBaseEntity * pLocal);

	void NameChange();

	void FlameEnemy( int iEnemy, int iLocal, int iUser, bool bHitHead );

	int GetFPS();

	bool SetupBones(matrix3x4_t * matrix, CBaseEntity * pEntity );

	bool HandleBoneSetup( CBaseEntity * pEnemy, int iBoneMask, matrix3x4_t * MatrixOut );

	void SimulateMovement( Vector & velocity, Vector & origin, CBaseEntity * player, int & flags, bool was_in_air );

	std::string GetWeaponNameByID( int iIndex );

	bool bKeyPressed[ 256 ];

	bool IsKeyPressed( int iKey )
	{
		if( GetAsyncKeyState( iKey ) )
		{
			if( !bKeyPressed[ iKey ] )
			{
				bKeyPressed[ iKey ] = true;
				return true;
			}
		}
		else
			bKeyPressed[ iKey ] = false;

		return false;
	}

	bool bInRange( float flValue, float flValue2, float flTolerance )
	{
		return fmodf(fabsf(flValue - flValue2), 360.f) < flTolerance;
	}

	bool bBrokeLC, bInLag, bInKnife, bShouldShotRevolver, bUpdateLocalAnim;
	sPunchRecord m_sPunchRecord[ 128 ] = { };
};

extern CMisc g_Misc;
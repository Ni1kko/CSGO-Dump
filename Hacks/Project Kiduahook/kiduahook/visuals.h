#pragma once

#include "main.h"

class CVisuals
{
public:
	void InitMaterials();

	void Run( CBaseEntity* pLocal );
	void LocalVisuals( CBaseEntity * pLocal );
	void GlowMain( CBaseEntity * pLocal );
	void PlayerVisuals( CBaseEntity * pLocal, int iScreenWidth, int iScreenHeight );
	void EntityVisuals( CBaseEntity * pLocal, int iScreenWidth, int iScreenHeight );
	void Crosshair( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, int iScreenWidth, int iScreenHeight );
	void BoundingBox( CBaseEntity * pEntity, float * flSides );

	void InitRecoil( CUserCmd * pCmd, CBaseEntity * pLocal );

	bool HasVisualRecoil( CBaseEntity * pLocal );

	void SpecList( CBaseEntity * pLocal, int iScreenWidth, int iScreenHeight );

	void RunMaterials();

	void NadeVisuals( CBaseEntity * pEntity );

	void BombVisuals( CBaseEntity * pLocal, CBaseEntity * pEntity, int iScreenWidth, int iScreenHeight );

	void InitIndicators( CBaseEntity * pLocal, int iScreenWidth, int iScreenHeight );

	void DrawAimbot( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon );

	void DrawHitbox( int iIndex );
	void DrawHitbox(int iIndex, matrix3x4_t* mat);

	void DrawLocalFake(CBaseEntity * pLocal, matrix3x4_t * mMatrix);

	enum eBBSides
	{
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_BOT,
		SIDE_TOP
	};

	bool bNewMap;
	bool bVisualRecoil;
	Vector* vAimPunch;
	Vector* vViewPunch;
	Vector vOldAimPunch;
	Vector vOldViewPunch;

};

extern CVisuals g_Visuals;
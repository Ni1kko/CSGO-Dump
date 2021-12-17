#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>


#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )


class Aimbot
{
public:
	matrix3x4_t Matrix[65][128];
	float FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity * pEntity, int aHitBox);
	void OnCreateMove();
	float bestEntDmg;
	void Autostop();
	int targetID = 0;
	bool CanShootWithCurrentHitchanceAndValidTarget;
private:
	float LerpTime();
	Vector Hitscan(C_BaseEntity* pEnt);
	bool HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance);
	float SynchronizeHitchance();
	float SynchronizeMinDamage();

	bool Backtrack[65];
	bool ShotBacktrack[65];

	template<class T, class U>
	T clamp(T in, U low, U high);
};
extern Aimbot g_Aimbot;
#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\ConVar.h"
#include "..\Settings.h"
#include "..\SDK\CTrace.h"
#include "..\SDK\ISurfaceData.h"

struct FireBulletData
{
	FireBulletData(const Vector &eye_pos)
		: src(eye_pos)
	{
	}

	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

struct CAutoWall
{
public:
	void UTIL_TraceLine(Vector & vecAbsStart, Vector & vecAbsEnd, unsigned int mask, C_BaseEntity * ign, CGameTrace * tr);
	bool SimulateFireBullet(C_BaseCombatWeapon * weap, FireBulletData & data, C_BaseEntity * player, int hitbox);
	bool SimulateFireBullet2(C_BaseEntity * pEntity, C_BaseEntity * local, C_BaseCombatWeapon * weapon, FireBulletData & data);
	bool SimulateFireBullet2(C_BaseEntity * local, C_BaseCombatWeapon * weapon, FireBulletData & data);
	//void UTIL_TraceLine(const Vector & vecAbsStart, const Vector & vecAbsEnd, unsigned int mask, const C_BaseEntity * ignore, int collisionGroup, trace_t * ptr);
	bool CanHit(const Vector & point, float * damage_given, C_BaseEntity * player, int hitbox);
	bool CanWallbang(float& dmg);
	bool CanHit2(C_BaseEntity * pEntity, const Vector & point, float * damage_given);
	bool CanHit2(const Vector & point, float * damage_given);
};

extern CAutoWall* autowall;
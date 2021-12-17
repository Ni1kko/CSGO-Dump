#pragma once
#include "../../utilities/includes.h"


struct FireBulletData
{
	
	FireBulletData(const Vector3 &eyePos) : src(eyePos)
	{
	}

	Vector3           src;
	CGameTrace          enter_trace;
	Vector3           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;

};

class Autowall
{
public:
	bool HandleBulletPenetration2( CSWeaponInfo * wpn_data, FireBulletData & data, bool extracheck );
	float CanHit(Vector3 &point);
	bool FireBullet(CBaseCombatWeapon* pWeapon, Vector3& direction, float& currentDamage);
	bool HandleBulletPenetration(CSWeaponInfo* weaponData, CGameTrace& enterTrace, Vector3& eyePosition, Vector3 direction, int& possibleHitsRemaining, float& currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration);
	bool TraceToExit(CGameTrace& enterTrace, CGameTrace& exitTrace, Vector3 startPosition, Vector3 direction);
	void ScaleDamage(CGameTrace &enterTrace, CSWeaponInfo *weaponData, float& currentDamage);
	bool IsBreakableEntity(CBaseEntity* entity);
	void GetBulletTypeParameters(float& maxRange, float& maxDistance, char* bulletType, bool sv_penetration_type);
	bool TraceToExit2( Vector3 & vecEnd, CGameTrace * pEnterTrace, Vector3 vecStart, Vector3 vecDir, CGameTrace * pExitTrace );
	void ClipTraceToPlayers(Vector3& absStart, Vector3 absEnd, unsigned int mask, CTraceFilter* filter, CGameTrace* tr, float minDistanceToRay);

	bool CanHitFloatingPoint(const Vector3 &point, const Vector3 &source);

	bool CanWallbang( float & dmg );

	bool DidHitNonWorldEntity(CBaseEntity * ent);
	bool DidHitWorld(CBaseEntity * ent);

private:
};
extern Autowall g_Autowall;
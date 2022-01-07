#pragma once
#include "Interfaces.h"
#include "Singleton.hpp"
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

class autowall_2
{
public:

	
	bool handle_penetration;

	float damage_reduction;

	void TraceLine(Vector & absStart, Vector & absEnd, unsigned int mask, IClientEntity * ignore, CGameTrace * ptr);

	void ClipTraceToPlayers(const Vector & absStart, const Vector absEnd, unsigned int mask, ITraceFilter * filter, CGameTrace * tr);

	void GetBulletTypeParameters(float & maxRange, float & maxDistance, char * bulletType, bool sv_penetration_type);

	bool BreakableEntity(IClientEntity * entity);

	void ScaleDamage(trace_t & enterTrace, CSWeaponInfo * weaponData, float & currentDamage);

	bool TraceToExit(trace_t & enterTrace, trace_t & exitTrace, Vector startPosition, Vector direction);

	bool HandleBulletPenetration(CSWeaponInfo * weaponData, trace_t & enterTrace, Vector & eyePosition, Vector direction, int & possibleHitsRemaining, float & currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration);

	bool FireBullet(C_BaseCombatWeapon * pWeapon, Vector & direction, float & currentDamage);

	float CanHit(Vector & point);

	bool PenetrateWall(IClientEntity * pBaseEntity, Vector & vecPoint);

};
extern autowall_2 * new_autowall;


class backup_autowall
{
public:

	bool can_hit(const Vector &point, float *damage_given);
//	bool trace_autowallable_2(float& dmg);
	bool SimulateFireBullet_2(IClientEntity *local, C_BaseCombatWeapon *weapon, FireBulletData &data);

	void traceIt(Vector & vecAbsStart, Vector & vecAbsEnd, unsigned int mask, IClientEntity * ign, CGameTrace * tr);

	bool TraceToExit_main(Vector & vecEnd, trace_t * pEnterTrace, Vector vecStart, Vector vecDir, trace_t * pExitTrace);

	bool BreakableEntity(IClientEntity * entity);

	float GetHitgroupDamageMult_2(int iHitGroup);

	void ScaleDamage_2(int hitgroup, IClientEntity * enemy, float weapon_armor_ratio, float & current_damage);

	bool HandleBulletPenetration_2(CSWeaponInfo * wpn_data, FireBulletData & data);

};
extern backup_autowall * backup_awall;

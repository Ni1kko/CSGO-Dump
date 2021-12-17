#pragma once
#include "Interfaces.h"
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

};
extern autowall_2 * new_autowall;


class backup_autowall
{
public:

	bool CanHit_2(const Vector &point, float *damage_given);
//	bool trace_autowallable_2(float& dmg);
	bool SimulateFireBullet_2(IClientEntity *local, C_BaseCombatWeapon *weapon, FireBulletData &data);

	bool trace_to_exit(Vector & end, Vector start, Vector dir, trace_t * enter_trace, trace_t * exit_trace);

	float GetHitgroupDamageMult_2(int iHitGroup);

	void ScaleDamage_2(int hitgroup, IClientEntity * enemy, float weapon_armor_ratio, float & current_damage);

	bool HandleBulletPenetration_2(CSWeaponInfo * wpn_data, FireBulletData & data);

};
extern backup_autowall * backup_awall;

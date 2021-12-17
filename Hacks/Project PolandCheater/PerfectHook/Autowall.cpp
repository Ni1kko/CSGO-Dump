#include "AutoWall.h"
//#include "R.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

inline bool CGameTrace::DidHitWorld() const
{
	return m_pEnt->GetIndex() == 0;
}

inline bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data);
float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 1.f;
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_CHEST:
		return 1.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 1.f;
	case HITGROUP_RIGHTARM:
		return 1.f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.f;
	default:
		break;
	}

	return 1.f;
}
bool IsArmored(C_BaseEntity* Entity, int ArmorValue, int Hitgroup)
{
    bool result = false;

    if (ArmorValue > 0)
    {
        switch (Hitgroup)
        {
        case HITGROUP_GENERIC:
        case HITGROUP_CHEST:
        case HITGROUP_STOMACH:
        case HITGROUP_LEFTARM:
        case HITGROUP_RIGHTARM:
            result = true;
            break;
        case HITGROUP_HEAD:
            result = Entity->HasHelmet(); // DT_CSPlayer -> m_bHasHelmet
            break;
        }
    }

    return result;
}

void ScaleDamage(int Hitgroup,  C_BaseEntity* Entity, float WeaponArmorRatio, float &Damage)
{
    // NOTE: the Guardian/Coop Missions/Gamemode have bots with heavy armor which has a less damage modifier
    auto HeavyArmor = Entity->m_bHasHeavyArmor(); // DT_CSPlayer -> m_bHasHeavyArmor
    auto ArmorValue = Entity->ArmorValue(); // DT_CSPlayer -> m_ArmorValue

    switch (Hitgroup)
    {
    case HITGROUP_HEAD:
        if (HeavyArmor)
            Damage = (Damage * 4.f) * 0.5f;
        else
            Damage *= 4.f;
        break;
    case HITGROUP_STOMACH:
        Damage *= 1.25f;
        break;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        Damage *= 0.75f;
        break;
    }

    if (IsArmored(Entity, ArmorValue, Hitgroup))
    {
        float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = WeaponArmorRatio * 0.5f;

        if (HeavyArmor)
        {
            ArmorBonusRatio = 0.33f;
            ArmorRatio = (WeaponArmorRatio * 0.5f) * 0.5f;
            v47 = 0.33f;
        }

        auto NewDamage = Damage * ArmorRatio;

        if (HeavyArmor)
            NewDamage *= 0.85f;

        if (((Damage - (Damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > ArmorValue)
            NewDamage = Damage - (ArmorValue / ArmorBonusRatio);

        Damage = NewDamage;
    }
}

/*void ScaleDamage(int hitgroup, C_BaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->ArmorValue() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
			{
				current_damage *= (weapon_armor_ratio * 0.25f);
			}
		}
		else
		{
			current_damage *= (weapon_armor_ratio * 0.5f);
		}
	}

}*/

bool SimulateFireBullet(C_BaseEntity* entity, C_BaseEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data)
{
	//Utils::ToLog("SimulateFireBullet");
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = weapon->GetCSWpnData();

	data.current_damage = static_cast<float>(wpn_data->m_iDamage);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_fRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		UTIL_TraceLine(data.src, end, 0x4600400B, local, 0, &data.enter_trace);
		UTIL_ClipTraceToPlayers(entity, data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7)
			&& (data.enter_trace.hitgroup > 0)
			&& (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()
				|| g_Options.Ragebot.FriendlyFire))
		{
			data.trace_length += (float)(data.enter_trace.fraction * data.trace_length_remaining);
			data.current_damage *= (float)(pow(wpn_data->m_fRangeModifier, data.trace_length * 0.002));
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->m_fArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = g_PhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(wpn_data->m_fRangeModifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.direction, data.enter_trace.endpos, data.enter_trace, trace_exit))
		return false;

	surfacedata_t *exit_surface_data = g_PhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_fPenetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;


	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}


/*
*    CanHit() - example of how to use the code
*     @in  point: target hitbox vector
*     @out damage_given: amount of damage the shot would do
*/
bool CanHit(C_BaseEntity* entity,const Vector &point, float *damage_given)
{
	//Utils::ToLog("CANHIT");
	auto *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	auto data = FireBulletData(local->GetOrigin() + local->GetViewOffset());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
//	CalcAngle(data.src, point, angles);
	VectorAngles(point - data.src, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(entity, local, reinterpret_cast<CBaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(static_cast<HANDLE>(local->GetActiveWeaponHandle()))), data))
	{
		*damage_given = data.current_damage;
		//Utils::ToLog("CANHIT END");
		return true;
	}

	return false;
}
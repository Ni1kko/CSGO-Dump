#pragma once
#include "../../stdafx.h"
#include "../Hacks/Misc.h"

class Autowall
{
public:
	static float DamageMultiplyer(int iHitGroup)
	{
		switch (iHitGroup)
		{
		case HITGROUP_HEAD:
		{
			return 4.0f;
		}
		case HITGROUP_CHEST:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
		{
			return 1.0f;
		}
		case HITGROUP_STOMACH:
		{
			return 1.25f;
		}
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
		{
			return 0.75f;
		}
		}
		return 1.0f;
	}

	static void ScaleDamage(int hitgroup, CBaseEntity* enemy, float weapon_armor_ratio, float& current_damage)
	{
		int armor = enemy->GetArmor();
		float ratio;

		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			current_damage *= 4.f;
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		}

		if (armor > 0)
		{
			switch (hitgroup)
			{
			case HITGROUP_HEAD:
				if (enemy->HasHelmet())
				{
					ratio = (weapon_armor_ratio * 0.5) * current_damage;
					if (((current_damage - ratio) * 0.5) > armor)
						ratio = current_damage - (armor * 2.0);
					current_damage = ratio;
				}
				break;
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
					ratio = current_damage - (armor * 2.0);
				current_damage = ratio;
				break;
			}
		}
	}

	static bool HandleBulletPen(CCSWeaponInfo* wpn_data, FireBulletData& data)
	{
		surfacedata_t* enter_surface_data = Interfaces.pPhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow(wpn_data->range_modifier, (data.trace_length * 0.002));

		if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
			data.penetrate_count = 0;
		if (data.penetrate_count <= 0)
			return false;

		Vector dummy;
		trace_t trace_exit;
		if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
			return false;
		surfacedata_t* exit_surface_data = Interfaces.pPhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);
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
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
		float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;
		float lost_damage = fmaxf(0.0f, v35 + thickness);
		if (lost_damage > data.current_damage)
			return false;
		if (lost_damage >= 0.0f)
			data.current_damage -= lost_damage;
		if (data.current_damage < 10)//ClientVars.Ragebot.MinDmg )
			return false;
		data.src = trace_exit.endpos;
		data.penetrate_count--;

		return true;
	}

	static bool FireSimulatedBullet(FireBulletData& data, CBaseEntity* Local, CBaseCombatWeapon* weapon)
	{
		data.penetrate_count = 4; // Max Amount Of Penitration
		data.trace_length = 0.0f; // wow what a meme
		CCSWeaponInfo* Weapon_Info = weapon->GetCSWpnData(); // Get Weapon Info
		data.current_damage = (float)Weapon_Info->damage;// Set Damage Memes
		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = Weapon_Info->range - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;
			UTIL_TraceLine(data.src, End_Point, MASK_SHOT, Local, 0, &data.enter_trace);
			UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, MASK_SHOT, &data.filter, &data.enter_trace);
			if (data.enter_trace.fraction == 1.0f)
				break;
			if (!(Local->GetTeam() != data.enter_trace.m_pEnt->GetTeam()) || data.enter_trace.m_pEnt->IsHostage() || data.enter_trace.m_pEnt->IsChicken())
				break;
			if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow(Weapon_Info->range_modifier, data.trace_length * 0.002);
				ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, Weapon_Info->armor_ratio, data.current_damage);
				return true;
			}
			if (!HandleBulletPen(Weapon_Info, data))
				break;
		}
		return false;
	}

	static float GetDamage(Vector Target)
	{
		FireBulletData Bullet_Data = FireBulletData(Hacks.LocalPlayer->GetEyePosition());
		Bullet_Data.filter.pSkip = Hacks.LocalPlayer;
		Vector Angle = Vector(0, 0, 0);
		Misc::CalcAngle(Bullet_Data.src, Target, Angle);
		g_Math.angleVectors(Angle, Bullet_Data.direction);

		if (FireSimulatedBullet(Bullet_Data, Hacks.LocalPlayer, Hacks.LocalWeapon))
			return Bullet_Data.current_damage;
		return 0;
	}
};

#include "autowall.hpp"
#include "..//helpers/math.hpp"

#define HITGROUP_GENERIC   0
#define HITGROUP_HEAD      1
#define HITGROUP_CHEST     2
#define HITGROUP_STOMACH   3
#define HITGROUP_LEFTARM   4
#define HITGROUP_RIGHTARM  5
#define HITGROUP_LEFTLEG   6
#define HITGROUP_RIGHTLEG  7
#define HITGROUP_GEAR      10
#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1
#define DAMAGE_YES		2
#define DAMAGE_AIM		3
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E'
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z'


returninfo c_autowall::autowall(Vector start, Vector end, C_BasePlayer* from_ent, C_BasePlayer* to_ent, int hitgroup) {
	returninfo rt;

	fbdata bullet;
	bullet.start = start;
	bullet.end = end;
	bullet.pos = start;
	bullet.thickness = 0.f;
	bullet.walls = 4;

	Math_AngleVectors(Math_CalcAngle(start, end), bullet.dir);

	auto flt_player = CTraceFilterOneEntity();
	flt_player.pEntity = to_ent;

	auto flt_self = CTraceFilter();
	flt_self.pSkip = from_ent;

	if (to_ent)
		bullet.filter = &flt_player;
	else
		bullet.filter = &flt_self;

	auto wep = from_ent->m_hActiveWeapon();
	if (!wep)
		return rt;

	auto inf = wep->GetCSWeaponData();
	if (!inf)
		return rt;

	end = start + bullet.dir * (wep->IsKnife() ? 45.f : inf->flRange);
	bullet.damage = inf->iDamage;

	while (bullet.damage > 0 && bullet.walls > 0) {
		rt.walls = bullet.walls;

		Ray_t ray;
		ray.Init(bullet.pos, end);

		CTraceFilter filter;
		filter.pSkip = from_ent;

		g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);
		this->clip_trace_to_player(bullet.pos, bullet.pos + bullet.dir * 40.f, to_ent, MASK_SHOT | CONTENTS_GRATE, bullet.filter, &bullet.trace);

		bullet.damage *= powf(inf->flRangeModifier, (bullet.trace.endpos - start).Length() / 500.f);

		if (bullet.trace.fraction == 1.f) {
			if (to_ent && hitgroup != -1) {
				this->scale_damage(to_ent, inf, hitgroup, bullet.damage);

				rt.damage = bullet.damage;
				rt.hitgroup = hitgroup;
				rt.end = bullet.trace.endpos;
				rt.ent = to_ent;
			}
			else {
				rt.damage = bullet.damage;
				rt.hitgroup = -1;
				rt.end = bullet.trace.endpos;
				rt.ent = nullptr;
			}
		}

		if (bullet.trace.hitgroup > 0 && bullet.trace.hitgroup <= 7) {
			if (to_ent && bullet.trace.hit_entity != to_ent) {
				rt.damage = -1;

				return rt;
			}
			C_BasePlayer* player = (C_BasePlayer*)bullet.trace.hit_entity;
			if (hitgroup != -1)
				scale_damage(player, inf, hitgroup, bullet.damage);
			else
				scale_damage(player, inf, bullet.trace.hitgroup, bullet.damage);

			rt.damage = bullet.damage;
			rt.hitgroup = bullet.trace.hitgroup;
			rt.end = bullet.trace.endpos;
			rt.ent = bullet.trace.hit_entity;

			break;
		}

		if (!this->handle_bullet_penetration(inf, bullet))
			break;

		rt.did_penetrate_wall = true;
	}

	rt.walls = bullet.walls;
	return rt;
}

void c_autowall::clip_trace_to_player(Vector& start, Vector& end, C_BasePlayer* ent, unsigned int mask, ITraceFilter* filter, trace_t* trace) {
	if (!ent)
		return;

	Vector mins = ent->GetCollideable()->OBBMins();
	Vector maxs = ent->GetCollideable()->OBBMaxs();

	Vector dir(end - start);
	dir.Normalized();

	Vector
		center = (maxs + mins) / 2,
		pos(center + ent->m_vecOrigin());

	Vector to = pos - start;
	float range_along = dir.Dot(to);

	float range;
	if (range_along < 0.f)
		range = -to.Length();
	else if (range_along > dir.Length())
		range = -(pos - end).Length();
	else {
		auto ray(pos - (dir * range_along + start));
		range = ray.Length();
	}

	if (range <= 60.f) {
		trace_t newtrace;

		Ray_t ray;
		ray.Init(start, end);

		g_EngineTrace->ClipRayToEntity(ray, mask, ent, &newtrace);

		if (trace->fraction > newtrace.fraction)
			*trace = newtrace;
	}
}

void c_autowall::scale_damage(C_BasePlayer* e, CCSWeaponInfo* weapon_info, int& hitgroup, float& current_damage) {
	bool has_heavy_armor = false;
	int armor_value = e->m_ArmorValue();

	auto is_armored = [&e, &hitgroup]()-> bool {
		C_BasePlayer* target_entity = e;
		switch (hitgroup) {
		case HITGROUP_HEAD:
			return target_entity->m_bHasHelmet();
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch (hitgroup) {
	case HITGROUP_HEAD:
		current_damage *= has_heavy_armor ? 2.f : 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armor_value > 0 && is_armored()) {
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_info->flArmorRatio / 2.f;

		if (has_heavy_armor) {
			armor_bonus_ratio = 0.33f;
			armor_ratio *= 0.5f;
			bonus_value = 0.33f;
		}

		auto new_damage = current_damage * armor_ratio;

		if (has_heavy_armor)
			new_damage *= 0.85f;

		if (((current_damage - (current_damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor_value)
			new_damage = current_damage - (armor_value / armor_bonus_ratio);

		current_damage = new_damage;
	}
}

bool c_autowall::handle_bullet_penetration(CCSWeaponInfo* inf, fbdata& bullet) {
	trace_t trace;

	static auto dbp = g_CVar->FindVar("ff_damage_bullet_penetration");

	auto sdata = g_PhysSurface->GetSurfaceData(bullet.trace.surface.surfaceProps);
	auto mat = sdata->game.material;

	auto sp_mod = *(float*)((uintptr_t)sdata + 88);
	auto dmg_mod = 0.16f;
	auto pnt_mod = 0.f;

	auto solid_surf = (bullet.trace.contents >> 3)& CONTENTS_SOLID;
	auto light_surf = (bullet.trace.surface.flags >> 7)& SURF_LIGHT;

	if (bullet.walls <= 0
		|| !bullet.walls && !light_surf && !solid_surf && mat != CHAR_TEX_GLASS && mat != CHAR_TEX_GRATE
		|| inf->flPenetration <= 0.f
		|| !this->trace_to_exit(&bullet.trace, bullet.trace.endpos, bullet.dir, &trace)
		&& !g_EngineTrace->GetPointContents(bullet.trace.endpos, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX))
		return false;

	auto e_sdata = g_PhysSurface->GetSurfaceData(trace.surface.surfaceProps);
	auto e_mat = e_sdata->game.material;
	auto e_sp_mod = *(float*)((DWORD)e_sdata + 88);

	if (mat == CHAR_TEX_GRATE || mat == CHAR_TEX_GLASS)
	{
		pnt_mod = 3.f;
		dmg_mod = 0.05f;
	}
	else if (light_surf || solid_surf)
	{
		pnt_mod = 1.f;
		dmg_mod = 0.16f;
	}
	else if (mat == CHAR_TEX_FLESH)
	{
		pnt_mod = dbp->GetFloat();
		dmg_mod = 0.16f;
	}
	else
	{
		pnt_mod = (sp_mod + e_sp_mod) / 2.f;
		dmg_mod = 0.16f;
	}

	if (mat == e_mat)
	{
		if (e_mat == CHAR_TEX_CARDBOARD || e_mat == CHAR_TEX_WOOD)
			pnt_mod = 3.f;
		else if (e_mat == CHAR_TEX_PLASTIC)
			pnt_mod = 2.f;
	}

	auto thickness = (trace.endpos - bullet.trace.endpos).LengthSqr();
	auto modifier = fmaxf(0.f, 1.f / pnt_mod);

	auto lost_damage = fmaxf(
		((modifier * thickness) / 24.f)
		+ ((bullet.damage * dmg_mod)
			+ (fmaxf(3.75 / inf->flPenetration, 0.f) * 3.f * modifier)), 0.f);

	if (lost_damage > bullet.damage)
		return false;

	if (lost_damage > 0.f)
		bullet.damage -= lost_damage;

	if (bullet.damage < 1.f)
		return false;

	bullet.pos = trace.endpos;
	bullet.walls--;

	return true;
}

bool c_autowall::trace_to_exit(trace_t* enter_trace, Vector& start, Vector& dir, trace_t* exit_trace) {
	auto end = Vector();
	auto distance = 0.f;
	auto distance_check = 23;
	auto first_contents = 0;

	do {
		distance += 4.f;
		end = start + dir * distance;

		if (!first_contents)
			first_contents = g_EngineTrace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		int point_contents = g_EngineTrace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) {
			Vector new_end = end - (dir * 4.f);

			Ray_t ray;
			ray.Init(end, new_end);

			g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
				Ray_t ray1;
				ray1.Init(start, end);

				CTraceFilter filter;
				filter.pSkip = exit_trace->hit_entity;

				g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startsolid)
					return true;

				continue;
			}
			C_BasePlayer* player = (C_BasePlayer*)enter_trace->hit_entity;
			if (exit_trace->DidHit() && !exit_trace->startsolid) {
				if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
					if (exit_trace->plane.normal.Dot(dir) <= 1.f)
						return true;

					continue;
				}

				if (this->is_breakable(player) && this->is_breakable(player))
					return true;

				continue;
			}

			if (exit_trace->surface.flags & SURF_NODRAW) {
				if (this->is_breakable(player) && this->is_breakable(player))
					return true;
				else if (!(enter_trace->surface.flags & SURF_NODRAW))
					continue;
			}

			if ((!enter_trace->hit_entity || enter_trace->hit_entity->EntIndex() == 0) && this->is_breakable(player)) {
				exit_trace = enter_trace;
				exit_trace->endpos = start + dir;
				return true;
			}

			continue;
		}

		distance_check--;
	} while (distance_check);

	return false;
}

bool c_autowall::is_breakable(C_BasePlayer* e) {
	using func = bool(__fastcall*)(C_BasePlayer*);
	static auto fn = reinterpret_cast<func>(Utils::PatternScan("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));

	if (!e || !e->EntIndex())
		return false;

	auto take_damage{ (char*)((uintptr_t)e + *(size_t*)((uintptr_t)fn + 38)) };
	auto take_damage_backup{ *take_damage };

	auto* cclass = g_CHLClient->GetAllClasses();

	if ((cclass->m_pNetworkName[1]) != 'F'
		|| (cclass->m_pNetworkName[4]) != 'c'
		|| (cclass->m_pNetworkName[5]) != 'B'
		|| (cclass->m_pNetworkName[9]) != 'h')
		*take_damage = 2;

	bool breakable = fn(e);
	*take_damage = take_damage_backup;

	return breakable;
}

float c_autowall::get_estimated_point_damage(Vector point) {
	if (!g_LocalPlayer)
		return -1.f;

	fbdata bullet;
	auto filter = CTraceFilter();
	filter.pSkip = g_LocalPlayer;

	bullet.filter = &filter;
	bullet.start = g_LocalPlayer->GetEyePos();
	bullet.end = point;
	bullet.pos = g_LocalPlayer->GetEyePos();
	Math_AngleVectors(Math_CalcAngle(bullet.start, point), bullet.dir);
	bullet.trace.startpos = bullet.start;
	bullet.trace.endpos = point;

	auto wep = g_LocalPlayer->m_hActiveWeapon();
	if (!wep)
		return -2.f;

	bullet.walls = 1;
	bullet.thickness = 0.f;

	auto inf = wep->GetCSWeaponData();
	if (!inf)
		return -3.f;

	bullet.damage = inf->iDamage;

	Ray_t ray;
	ray.Init(bullet.start, bullet.end);

	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);

	if (bullet.trace.fraction == 1.f)
		return -4.f;

	if (this->handle_bullet_penetration(inf, bullet))
		return bullet.damage;

	return -5.f;
}
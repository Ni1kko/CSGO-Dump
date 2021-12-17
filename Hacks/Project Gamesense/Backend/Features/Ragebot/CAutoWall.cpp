#include "CAutoWall.h"

returninfo c_autowall::autowall(Vector start, Vector end, CBaseEntity* from_ent, CBaseEntity* to_ent, int hitgroup) {
	returninfo rt;

	fbdata bullet;
	bullet.start = start;
	bullet.end = end;
	bullet.pos = start;
	bullet.thickness = 0.f;
	bullet.walls = 4;

	Utilities->Math_AngleVectors(Utilities->Math_CalcAngle(start, end), bullet.dir);

	auto flt_player = CTraceFilterOneEntity();
	flt_player.pEntity = to_ent;

	auto flt_self = CTraceFilter();
	flt_self.pSkip1 = from_ent;

	if (to_ent)
		bullet.filter = &flt_player;
	else
		bullet.filter = &flt_self;

	auto wep = from_ent->GetActiveWeapon();
	if (!wep)
		return rt;

	auto inf = wep->get_full_info();
	if (!inf)
		return rt;

	end = start + bullet.dir * (wep->is_knife() ? 45.f : inf->range);
	bullet.damage = inf->damage;

	while (bullet.damage > 0 && bullet.walls > 0) {
		rt.walls = bullet.walls;

		Ray_t ray;
		ray.Init(bullet.pos, end);

		CTraceFilter filter;
		filter.pSkip1 = from_ent;

		Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);
		this->clip_trace_to_player(bullet.pos, bullet.pos + bullet.dir * 40.f, to_ent, MASK_SHOT | CONTENTS_GRATE, bullet.filter, &bullet.trace);

		bullet.damage *= powf(inf->range_modifier, (bullet.trace.end - start).Length() / 500.f);

		if (bullet.trace.fraction == 1.f) {
			if (to_ent && hitgroup != -1) {
				this->scale_damage(to_ent, inf, hitgroup, bullet.damage);

				rt.damage = bullet.damage;
				rt.hitgroup = hitgroup;
				rt.end = bullet.trace.end;
				rt.ent = to_ent;
			}
			else {
				rt.damage = bullet.damage;
				rt.hitgroup = -1;
				rt.end = bullet.trace.end;
				rt.ent = nullptr;
			}
		}

		if (bullet.trace.hitGroup > 0 && bullet.trace.hitGroup <= 7) {
			if (to_ent && bullet.trace.m_pEnt != to_ent ||
				bullet.trace.m_pEnt->GetTeam() == from_ent->GetTeam()) {
				rt.damage = -1;

				return rt;
			}

			if (hitgroup != -1)
				scale_damage(bullet.trace.m_pEnt, inf, hitgroup, bullet.damage);
			else
				scale_damage(bullet.trace.m_pEnt, inf, bullet.trace.hitGroup, bullet.damage);

			rt.damage = bullet.damage;
			rt.hitgroup = bullet.trace.hitGroup;
			rt.end = bullet.trace.end;
			rt.ent = bullet.trace.m_pEnt;

			break;
		}

		if (!this->handle_bullet_penetration(inf, bullet))
			break;

		rt.did_penetrate_wall = true;
	}

	rt.walls = bullet.walls;
	return rt;
}

void c_autowall::clip_trace_to_player(Vector& start, Vector& end, CBaseEntity* ent, unsigned int mask, ITraceFilter* filter, trace_t* trace) {
	if (!ent)
		return;

	Vector mins = ent->GetCollision()->VecMins();
	Vector maxs = ent->GetCollision()->VecMaxs();

	Vector dir(end - start);
	dir.Normalize();

	Vector
		center = (maxs + mins) / 2,
		pos(center + ent->GetOrigin());

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

		Interfaces::Trace->ClipRayToEntity(ray, mask, ent, &newtrace);

		if (trace->fraction > newtrace.fraction)
			*trace = newtrace;
	}
}
bool c_autowall::IsArmored(CBaseEntity *player, int armorVal, int hitgroup)
{
	bool res = false;

	if (armorVal > 0)
	{
		switch (hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:

			res = true;
			break;

		case HITGROUP_HEAD:

			res = player->HasHelmet();
			break;

		}
	}

	return res;
}
void c_autowall::scale_damage(CBaseEntity* ent, CSWeaponInfo * weapon_info, int& hitgroup, float& damage) {
	auto armor_value = ent->GetArmor();
	//bool heavArmor = ent->HasHeavyArmor();
	bool has_heavy_armor = false;

	auto is_armored = [&ent, &hitgroup]()-> bool {
		CBaseEntity * target_entity = ent;
		switch (hitgroup) {
		case HITGROUP_HEAD:
			return target_entity->HasHelmet();
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
		damage *= has_heavy_armor ? 2.f : 4.f;
		break;
	case HITGROUP_STOMACH:
		damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armor_value > 0 && is_armored()) {
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_info->armor_ratio / 2.f;

		if (has_heavy_armor) {
			armor_bonus_ratio = 0.33f;
			armor_ratio *= 0.5f;
			bonus_value = 0.33f;
		}

		auto new_damage = damage * armor_ratio;

		if (has_heavy_armor)
			new_damage *= 0.85f;

		if (((damage - (damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor_value)
			new_damage = damage - (armor_value / armor_bonus_ratio);

		damage = new_damage;
	}
}

bool c_autowall::handle_bullet_penetration(CSWeaponInfo* inf, fbdata& bullet) {
	trace_t trace;

	static auto dbp = Interfaces::Var->FindVar("ff_damage_bullet_penetration");

	auto sdata = Interfaces::PhysSurface->GetSurfaceData(bullet.trace.surface.surfaceProps);
	auto mat = sdata->game.material;

	auto sp_mod = *(float*)((uintptr_t)sdata + 88);
	auto dmg_mod = 0.16f;
	auto pnt_mod = 0.f;

	auto solid_surf = (bullet.trace.contents >> 3) & CONTENTS_SOLID;
	auto light_surf = (bullet.trace.surface.flags >> 7) & SURF_LIGHT;

	if (bullet.walls <= 0
		|| !bullet.walls && !light_surf && !solid_surf && mat != CHAR_TEX_GLASS && mat != CHAR_TEX_GRATE
		|| inf->penetration <= 0.f
		|| !this->trace_to_exit(&bullet.trace, bullet.trace.end, bullet.dir, &trace)
		&& !Interfaces::Trace->GetPointContents(bullet.trace.end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX))
		return false;

	auto e_sdata = Interfaces::PhysSurface->GetSurfaceData(trace.surface.surfaceProps);
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

	auto thickness = (trace.end - bullet.trace.end).LengthSqr();
	auto modifier = fmaxf(0.f, 1.f / pnt_mod);

	auto lost_damage = fmaxf(
		((modifier * thickness) / 24.f)
		+ ((bullet.damage * dmg_mod)
			+ (fmaxf(3.75 / inf->penetration, 0.f) * 3.f * modifier)), 0.f);

	if (lost_damage > bullet.damage)
		return false;

	if (lost_damage > 0.f)
		bullet.damage -= lost_damage;

	if (bullet.damage < 1.f)
		return false;

	bullet.pos = trace.end;
	bullet.walls--;

	return true;
}

bool c_autowall::trace_to_exit(trace_t* enter_trace, Vector& start, Vector& dir, trace_t * exit_trace) {
	auto end = Vector();
	auto distance = 0.f;
	auto distance_check = 23;
	auto first_contents = 0;

	do {
		distance += 4.f;
		end = start + dir * distance;

		if (!first_contents)
			first_contents = Interfaces::Trace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		int point_contents = Interfaces::Trace->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) {
			Vector new_end = end - (dir * 4.f);

			Ray_t ray;
			ray.Init(end, new_end);

			Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

			if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX) {
				Ray_t ray1;
				ray1.Init(start, end);

				CTraceFilter filter;
				filter.pSkip1 = exit_trace->m_pEnt;

				Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startSolid)
					return true;

				continue;
			}

			if (exit_trace->DidHit() && !exit_trace->startSolid) {
				if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
					if (exit_trace->plane.normal.Dot(dir) <= 1.f)
						return true;

					continue;
				}

				if (this->is_breakable(enter_trace->m_pEnt) && this->is_breakable(exit_trace->m_pEnt))
					return true;

				continue;
			}

			if (exit_trace->surface.flags& SURF_NODRAW) {
				if (this->is_breakable(enter_trace->m_pEnt) && this->is_breakable(exit_trace->m_pEnt))
					return true;
				else if (!(enter_trace->surface.flags& SURF_NODRAW))
					continue;
			}

			if ((!enter_trace->m_pEnt || enter_trace->m_pEnt->GetIndex() == 0) && this->is_breakable(enter_trace->m_pEnt)) {
				exit_trace = enter_trace;
				exit_trace->end = start + dir;
				return true;
			}

			continue;
		}

		distance_check--;
	} while (distance_check);

	return false;
}

bool c_autowall::is_breakable(CBaseEntity* e) {
	using func = bool(__fastcall *)(CBaseEntity *);
	static auto fn = reinterpret_cast<func>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));

	if (!e || !e->GetIndex())
		return false;

	auto take_damage{ (char *)((uintptr_t)e + *(size_t*)((uintptr_t)fn + 38)) };
	auto take_damage_backup{ *take_damage };

	auto* cclass = Interfaces::Client->GetAllClasses();

	if ((cclass->m_pNetworkName[1]) != 'F'
		|| (cclass->m_pNetworkName[4]) != 'c'
		|| (cclass->m_pNetworkName[5]) != 'B'
		|| (cclass->m_pNetworkName[9]) != 'h')
		*take_damage = 2;

	bool breakable = fn(e);
	*take_damage = take_damage_backup;

	return breakable;
}

float c_autowall::get_point_damage(Vector point, CBaseEntity* e) {
	return this->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, e).damage;
}

float c_autowall::get_estimated_point_damage(Vector point) {
	if (!G::LocalPlayer)
		return -1.f;

	fbdata bullet;
	auto filter = CTraceFilter();
	filter.pSkip1 = G::LocalPlayer;

	bullet.filter = &filter;
	bullet.start = G::LocalPlayer->GetEyePosition();
	bullet.end = point;
	bullet.pos = G::LocalPlayer->GetEyePosition();
	Utilities->Math_AngleVectors(Utilities->Math_CalcAngle(bullet.start, point), bullet.dir);
	bullet.trace.start = bullet.start;
	bullet.trace.end = point;

	auto wep = G::LocalPlayer->GetActiveWeapon();
	if (!wep)
		return -2.f;

	bullet.walls = 1;
	bullet.thickness = 0.f;

	auto inf = wep->get_full_info();
	if (!inf)
		return -3.f;

	bullet.damage = inf->damage;

	Ray_t ray;
	ray.Init(bullet.start, bullet.end);

	Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);

	if (bullet.trace.fraction == 1.f)
		return -4.f;

	if (this->handle_bullet_penetration(inf, bullet))
		return bullet.damage;

	return -5.f;
}
inline float VectorNormalize(Vector& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		// FIXME:
		// Just copying the existing implemenation; shouldn't res.z == 0?
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}
bool VectortoVectorVisible(Vector src, Vector point, CBaseEntity* pEnt)
{
	Ray_t ray;
	fbdata bullet;
	bullet.trace.m_pEnt = NULL;
	auto filter = CTraceFilter();
	Interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &bullet.trace);

	if (bullet.trace.fraction == 1.0f)
	{
		return true;
	}
	if (pEnt != nullptr && bullet.trace.m_pEnt == pEnt)
		return true;
	return false;
}
bool c_autowall::CanHitFloatingPoint(const Vector &point, const Vector &source, CBaseEntity* pEnt) {
	if (!G::LocalPlayer)
		return false;

	fbdata bullet;
	auto filter = CTraceFilter();
	filter.pSkip1 = G::LocalPlayer;

	/*bullet.filter = &filter;
	bullet.start = G::LocalPlayer->GetEyePosition();
	bullet.end = point;
	bullet.pos = G::LocalPlayer->GetEyePosition();/**/
	Utilities->Math_AngleVectors(Utilities->Math_CalcAngle(bullet.start, point), bullet.dir);
	VectorNormalize(bullet.dir);
	/*bullet.trace.start = bullet.start;
	bullet.trace.end = point;*/

	auto wep = G::LocalPlayer->GetActiveWeapon();
	if (!wep)
		return false;

	bullet.walls = 1;
	bullet.thickness = 0.f;

	auto inf = wep->get_full_info();
	if (!inf)
		return false;

	bullet.damage = inf->damage;

	Ray_t ray;
	ray.Init(bullet.start, bullet.end);

	Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);

	if (VectortoVectorVisible(bullet.start, point, pEnt))
	{
		return true;
	}

	if (handle_bullet_penetration(inf, bullet)) {
		return true;
	}

	return false;
}
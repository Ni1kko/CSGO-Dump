//#include "autowall.h"
//
//autowall::returninfo_t autowall::calculate_return_info(vec3_t start, vec3_t end, IClientEntity * from_entity, IClientEntity * to_entity, int specific_hitgroup) {
//	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
//	returninfo_t return_info = returninfo_t(-1, -1, 4, false, 0.f, nullptr);
//
//	firebulletdata_t fire_bullet_data;
//	fire_bullet_data.m_start = start;
//	fire_bullet_data.m_end = end;
//	fire_bullet_data.m_current_position = start;
//	fire_bullet_data.m_thickness = 0.f;
//	fire_bullet_data.m_penetration_count = 4;
//
//	math::angle_vectors(math::calculate_angle(start, end), fire_bullet_data.m_direction);
//
//	if (!from_entity)
//		from_entity = local_player;
//
//	if (!from_entity)
//		return return_info;
//
//	auto filter_player = CTraceFilterOneEntity();
//	filter_player.pEntity = to_entity;
//
//	auto filter_local = CTraceFilter();
//	filter_local.pSkip = from_entity;
//
//	if (to_entity)
//		fire_bullet_data.m_filter = &filter_player;
//	else
//		fire_bullet_data.m_filter = &filter_player;
//
//	if (from_entity->GetClientClass()->m_ClassID != CCSPlayer)
//		return return_info;
//
//	player_t * e_from = static_cast<player_t *>(from_entity);
//	player_t * e_to = static_cast<player_t *>(to_entity);
//
//	weapon_t * weapon = e_from->m_hActiveWeapon().Get();
//	if (!weapon)
//		return return_info;
//
//	csweaponinfo_t * weapon_info = weapon->get_csweapon_info();
//	if (!weapon_info)
//		return return_info;
//
//	float range = min(weapon_info->range, (start - end).Length());
//	end = start + (fire_bullet_data.m_direction * range);
//	fire_bullet_data.m_current_damage = weapon_info->damage;
//
//	while (fire_bullet_data.m_current_damage > 0 && fire_bullet_data.m_penetration_count > 0) {
//		return_info.m_penetration_count = fire_bullet_data.m_penetration_count;
//
//		trace_line(fire_bullet_data.m_current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &fire_bullet_data.m_enter_trace);
//		clip_trace_to_player(fire_bullet_data.m_current_position, fire_bullet_data.m_current_position + (fire_bullet_data.m_direction * 40.f), e_to, MASK_SHOT | CONTENTS_GRATE, fire_bullet_data.m_filter, &fire_bullet_data.m_enter_trace);
//
//		const float distance_traced = (fire_bullet_data.m_enter_trace.endpos - start).Length();
//		fire_bullet_data.m_current_damage *= pow(weapon_info->range_modifier, (distance_traced / 500.f));
//
//		if (fire_bullet_data.m_enter_trace.fraction == 1.f) {
//			if (to_entity && specific_hitgroup != -1) {
//				scale_damage(e_to, weapon_info, specific_hitgroup, fire_bullet_data.m_current_damage);
//
//				return_info.m_damage = fire_bullet_data.m_current_damage;
//				return_info.m_hitgroup = specific_hitgroup;
//				return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
//				return_info.m_hit_entity = to_entity;
//			}
//			else {
//				return_info.m_damage = fire_bullet_data.m_current_damage;
//				return_info.m_hitgroup = -1;
//				return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
//				return_info.m_hit_entity = nullptr;
//			}
//
//			break;
//		}
//
//		if (fire_bullet_data.m_enter_trace.hitgroup > 0 && fire_bullet_data.m_enter_trace.hitgroup <= 7) {
//			if ((to_entity && fire_bullet_data.m_enter_trace.hit_entity != to_entity) ||
//				(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity)->m_iTeamNum() == e_from->m_iTeamNum())) {
//				return_info.m_damage = -1;
//				return return_info;
//			}
//
//			if (specific_hitgroup != -1)
//				scale_damage(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity), weapon_info, specific_hitgroup, fire_bullet_data.m_current_damage);
//			else
//				scale_damage(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity), weapon_info, fire_bullet_data.m_enter_trace.hitgroup, fire_bullet_data.m_current_damage);
//
//			return_info.m_damage = fire_bullet_data.m_current_damage;
//			return_info.m_hitgroup = fire_bullet_data.m_enter_trace.hitgroup;
//			return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
//			return_info.m_hit_entity = fire_bullet_data.m_enter_trace.hit_entity;
//
//			break;
//		}
//
//		if (!handle_bullet_penetration(weapon_info, fire_bullet_data))
//			break;
//
//		return_info.m_did_penetrate_wall = true;
//	}
//
//	return_info.m_penetration_count = fire_bullet_data.m_penetration_count;
//	return return_info;
//}
//
//void autowall::scale_damage(player_t * e, csweaponinfo_t * weapon_info, int hitgroup, float & current_damage) {
//	bool has_heavy_armor = false;
//	int armor_value = e->m_ArmorValue();
//
//	auto is_armored = [&e, &hitgroup]()-> bool {
//		player_t * target_entity = e;
//		switch (hitgroup) {
//		case HITGROUP_HEAD:
//			return target_entity->m_bHasHelmet();
//		case HITGROUP_GENERIC:
//		case HITGROUP_CHEST:
//		case HITGROUP_STOMACH:
//		case HITGROUP_LEFTARM:
//		case HITGROUP_RIGHTARM:
//			return true;
//		default:
//			return false;
//		}
//	};
//
//	switch (hitgroup) {
//	case HITGROUP_HEAD:
//		current_damage *= has_heavy_armor ? 2.f : 4.f;
//		break;
//	case HITGROUP_STOMACH:
//		current_damage *= 1.25f;
//		break;
//	case HITGROUP_LEFTLEG:
//	case HITGROUP_RIGHTLEG:
//		current_damage *= 0.75f;
//		break;
//	default:
//		break;
//	}
//
//	if (armor_value > 0 && is_armored()) {
//		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_info->armor_ratio / 2.f;
//
//		if (has_heavy_armor) {
//			armor_bonus_ratio = 0.33f;
//			armor_ratio *= 0.5f;
//			bonus_value = 0.33f;
//		}
//
//		auto new_damage = current_damage * armor_ratio;
//
//		if (has_heavy_armor)
//			new_damage *= 0.85f;
//
//		if (((current_damage - (current_damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor_value)
//			new_damage = current_damage - (armor_value / armor_bonus_ratio);
//
//		current_damage = new_damage;
//	}
//}
//
//bool autowall::handle_bullet_penetration(csweaponinfo_t * info, firebulletdata_t & data) {
//	CGameTrace trace_exit;
//	surfacedata_t * enter_surface_data = g_csgo.m_physsurface()->GetSurfaceData(data.m_enter_trace.surface.surfaceProps);
//	int enter_material = enter_surface_data->game.material;
//
//	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
//	float final_damage_modifier = 0.16f;
//	float compined_penetration_modifier = 0.f;
//	bool solid_surf = ((data.m_enter_trace.contents >> 3) & CONTENTS_SOLID);
//	bool light_surf = ((data.m_enter_trace.surface.flags >> 7) & SURF_LIGHT);
//
//	if (data.m_penetration_count <= 0
//		|| (!data.m_penetration_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE)
//		|| info->penetration <= 0.f
//		|| !trace_to_exit(&data.m_enter_trace, data.m_enter_trace.endpos, data.m_direction, &trace_exit)
//		&& !(g_csgo.m_trace()->GetPointContents(data.m_enter_trace.endpos, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX)))
//		return false;
//
//	surfacedata_t *exit_surface_data = g_csgo.m_physsurface()->GetSurfaceData(trace_exit.surface.surfaceProps);
//	int exit_material = exit_surface_data->game.material;
//	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;
//
//	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
//		compined_penetration_modifier = 3.f;
//		final_damage_modifier = 0.05f;
//	}
//
//	else if (light_surf || solid_surf) {
//		compined_penetration_modifier = 1.f;
//		final_damage_modifier = 0.16f;
//	}
//	else {
//		compined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) * 0.5f;
//		final_damage_modifier = 0.16f;
//	}
//
//	if (enter_material == exit_material) {
//		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
//			compined_penetration_modifier = 3.f;
//		else if (exit_material == CHAR_TEX_PLASTIC)
//			compined_penetration_modifier = 2.0f;
//	}
//
//	float thickness = (trace_exit.endpos - data.m_enter_trace.endpos).LengthSqr();
//	float modifier = fmaxf(0.f, 1.f / compined_penetration_modifier);
//
//	float lost_damage = fmaxf(
//		((modifier * thickness) / 24.f) //* 0.041666668
//		+ ((data.m_current_damage * final_damage_modifier)
//			+ (fmaxf(3.75 / info->penetration, 0.f) * 3.f * modifier)), 0.f);
//
//	if (lost_damage > data.m_current_damage)
//		return false;
//
//	if (lost_damage > 0.f)
//		data.m_current_damage -= lost_damage;
//
//	if (data.m_current_damage < 1.f)
//		return false;
//
//	data.m_current_position = trace_exit.endpos;
//	data.m_penetration_count--;
//
//	return true;
//}
//
//bool autowall::trace_to_exit(trace_t * enter_trace, vec3_t start, vec3_t dir, trace_t * exit_trace) {
//	vec3_t end;
//	float distance = 0.f;
//	signed int distance_check = 23;
//	int first_contents = 0;
//
//	do {
//		distance += 4.f;
//		end = start + dir * distance;
//
//		if (!first_contents)
//			first_contents = g_csgo.m_trace()->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);
//
//		int point_contents = g_csgo.m_trace()->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);
//
//		if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) {
//			vec3_t new_end = end - (dir * 4.f);
//
//			Ray_t ray;
//			ray.Init(end, new_end);
//
//			g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);
//
//			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
//				util::trace_line(end, start, MASK_SHOT_HULL | CONTENTS_HITBOX, exit_trace->hit_entity, exit_trace);
//
//				if (exit_trace->DidHit() && !exit_trace->startsolid)
//					return true;
//
//				continue;
//			}
//
//			if (exit_trace->DidHit() && !exit_trace->startsolid) {
//				if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
//					if (exit_trace->plane.normal.Dot(dir) <= 1.f)
//						return true;
//
//					continue;
//				}
//
//				if (is_breakable_entity(enter_trace->hit_entity) && is_breakable_entity(exit_trace->hit_entity))
//					return true;
//
//				continue;
//			}
//
//			if (exit_trace->surface.flags & SURF_NODRAW) {
//				if (is_breakable_entity(enter_trace->hit_entity) && is_breakable_entity(exit_trace->hit_entity))
//					return true;
//				else if (!(enter_trace->surface.flags & SURF_NODRAW))
//					continue;
//			}
//
//			if ((!enter_trace->hit_entity || enter_trace->hit_entity->EntIndex() == 0) && (is_breakable_entity(enter_trace->hit_entity))) {
//				exit_trace = enter_trace;
//				exit_trace->endpos = start + dir;
//				return true;
//			}
//
//			continue;
//		}
//
//		distance_check--;
//	} while (distance_check);
//
//	return false;
//}
//
//void autowall::trace_line(vec3_t & start, vec3_t & end, unsigned int mask, IClientEntity * ignore, trace_t * trace) {
//	Ray_t ray;
//	ray.Init(start, end);
//
//	CTraceFilter filter;
//	filter.pSkip = ignore;
//
//	g_csgo.m_trace()->TraceRay(ray, mask, &filter, trace);
//}
//
//void autowall::clip_trace_to_player(vec3_t & start, vec3_t & end, player_t * e, unsigned int mask, ITraceFilter * filter, trace_t * old_trace) {
//	if (!e)
//		return;
//
//	vec3_t mins = e->GetCollideable()->OBBMins(), maxs = e->GetCollideable()->OBBMaxs();
//
//	vec3_t dir(end - start);
//	dir.Normalize();
//
//	vec3_t
//		center = (maxs + mins) / 2,
//		pos(center + e->m_vecOrigin());
//
//	vec3_t to = pos - start;
//	float range_along = dir.Dot(to);
//
//	float range;
//	if (range_along < 0.f)
//		range = -to.Length();
//
//	else if (range_along > dir.Length())
//		range = -(pos - end).Length();
//
//	else {
//		auto ray(pos - (dir * range_along + start));
//		range = ray.Length();
//	}
//
//	if (range <= 60.f) {
//		trace_t trace;
//
//		Ray_t ray;
//		ray.Init(start, end);
//
//		g_csgo.m_trace()->ClipRayToEntity(ray, mask, e, &trace);
//
//		if (old_trace->fraction > trace.fraction)
//			*old_trace = trace;
//	}
//}
//
//bool autowall::is_breakable_entity(IClientEntity * e) {
//	using Fn = bool(__fastcall *)(IClientEntity *);
//	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));
//
//	if (!e || !e->EntIndex())
//		return false;
//
//	auto take_damage{ (char *)((uintptr_t)e + *(size_t*)((uintptr_t)fn + 38)) };
//	auto take_damage_backup{ *take_damage };
//
//	auto * cclass = g_csgo.m_client()->GetAllClasses();
//
//	if ((cclass->m_pNetworkName[1]) != 'F'
//		|| (cclass->m_pNetworkName[4]) != 'c'
//		|| (cclass->m_pNetworkName[5]) != 'B'
//		|| (cclass->m_pNetworkName[9]) != 'h')
//		*take_damage = DAMAGE_YES;
//
//	bool breakable = fn(e);
//	*take_damage = take_damage_backup;
//
//	return breakable;
//}

#include "autowall.h"

autowall::returninfo_t autowall::calculate_return_info(vec3_t start, vec3_t end, IClientEntity * from_entity, IClientEntity * to_entity, int specific_hitgroup) {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	returninfo_t return_info = returninfo_t(-1, -1, 4, false, 0.f, nullptr);

	firebulletdata_t fire_bullet_data;
	fire_bullet_data.m_start = start;
	fire_bullet_data.m_end = end;
	fire_bullet_data.m_current_position = start;
	fire_bullet_data.m_thickness = 0.f;
	fire_bullet_data.m_penetration_count = 4;

	math::angle_vectors(math::calculate_angle(start, end), fire_bullet_data.m_direction);

	if (!from_entity)
		from_entity = local_player;

	if (!from_entity)
		return return_info;

	auto filter_player = CTraceFilterOneEntity();
	filter_player.pEntity = to_entity;

	auto filter_local = CTraceFilter();
	filter_local.pSkip = from_entity;

	if (to_entity)
		fire_bullet_data.m_filter = &filter_player;
	else
		fire_bullet_data.m_filter = &filter_player;

	if (from_entity->GetClientClass()->m_ClassID != CCSPlayer)
		return return_info;

	player_t * e_from = static_cast<player_t *>(from_entity);
	player_t * e_to = static_cast<player_t *>(to_entity);

	weapon_t * weapon = e_from->m_hActiveWeapon().Get();
	if (!weapon)
		return return_info;

	csweaponinfo_t * weapon_info = weapon->get_csweapon_info();
	if (!weapon_info)
		return return_info;

	float range = min(weapon_info->range, (start - end).Length());
	end = start + (fire_bullet_data.m_direction * range);
	fire_bullet_data.m_current_damage = weapon_info->damage;

	while (fire_bullet_data.m_current_damage > 0 && fire_bullet_data.m_penetration_count > 0) {
		return_info.m_penetration_count = fire_bullet_data.m_penetration_count;

		trace_line(fire_bullet_data.m_current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &fire_bullet_data.m_enter_trace);
		clip_trace_to_player(fire_bullet_data.m_current_position, fire_bullet_data.m_current_position + (fire_bullet_data.m_direction * 40.f), e_to, MASK_SHOT | CONTENTS_GRATE, fire_bullet_data.m_filter, &fire_bullet_data.m_enter_trace);

		const float distance_traced = (fire_bullet_data.m_enter_trace.endpos - start).Length();
		fire_bullet_data.m_current_damage *= pow(weapon_info->range_modifier, (distance_traced / 500.f));

		if (fire_bullet_data.m_enter_trace.fraction == 1.f) {
			if (to_entity && specific_hitgroup != -1) {
				scale_damage(e_to, weapon_info, specific_hitgroup, fire_bullet_data.m_current_damage);

				return_info.m_damage = fire_bullet_data.m_current_damage;
				return_info.m_hitgroup = specific_hitgroup;
				return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
				return_info.m_hit_entity = to_entity;
			}
			else {
				return_info.m_damage = fire_bullet_data.m_current_damage;
				return_info.m_hitgroup = -1;
				return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
				return_info.m_hit_entity = nullptr;
			}

			break;
		}

		if (fire_bullet_data.m_enter_trace.hitgroup > 0 && fire_bullet_data.m_enter_trace.hitgroup <= 7) {
			if ((to_entity && fire_bullet_data.m_enter_trace.hit_entity != to_entity) ||
				(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity)->m_iTeamNum() == e_from->m_iTeamNum())) {
				return_info.m_damage = -1;
				return return_info;
			}

			if (specific_hitgroup != -1)
				scale_damage(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity), weapon_info, specific_hitgroup, fire_bullet_data.m_current_damage);
			else
				scale_damage(reinterpret_cast<player_t *>(fire_bullet_data.m_enter_trace.hit_entity), weapon_info, fire_bullet_data.m_enter_trace.hitgroup, fire_bullet_data.m_current_damage);

			return_info.m_damage = fire_bullet_data.m_current_damage;
			return_info.m_hitgroup = fire_bullet_data.m_enter_trace.hitgroup;
			return_info.m_end = fire_bullet_data.m_enter_trace.endpos;
			return_info.m_hit_entity = fire_bullet_data.m_enter_trace.hit_entity;

			break;
		}

		if (!handle_bullet_penetration(weapon_info, fire_bullet_data))
			break;

		return_info.m_did_penetrate_wall = true;
	}

	return_info.m_penetration_count = fire_bullet_data.m_penetration_count;
	return return_info;
}

void autowall::scale_damage(player_t * e, csweaponinfo_t * weapon_info, int hitgroup, float & current_damage) {
	bool has_heavy_armor = false;
	int armor_value = e->m_ArmorValue();

	auto is_armored = [&e, &hitgroup]()-> bool {
		player_t * target_entity = e;
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
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_info->armor_ratio / 2.f;

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

bool autowall::handle_bullet_penetration(csweaponinfo_t * info, firebulletdata_t & data) {
	CGameTrace trace_exit;
	surfacedata_t * enter_surface_data = g_csgo.m_physsurface()->GetSurfaceData(data.m_enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float compined_penetration_modifier = 0.f;
	bool solid_surf = ((data.m_enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool light_surf = ((data.m_enter_trace.surface.flags >> 7) & SURF_LIGHT);

	if (data.m_penetration_count <= 0
		|| (!data.m_penetration_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE)
		|| info->penetration <= 0.f
		|| !trace_to_exit(&data.m_enter_trace, data.m_enter_trace.endpos, data.m_direction, &trace_exit)
		&& !(g_csgo.m_trace()->GetPointContents(data.m_enter_trace.endpos, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX)))
		return false;

	surfacedata_t *exit_surface_data = g_csgo.m_physsurface()->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;

	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
		compined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}

	else if (light_surf || solid_surf) {
		compined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	else {
		compined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) * 0.5f;
		final_damage_modifier = 0.16f;
	}

	if (enter_material == exit_material) {
		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
			compined_penetration_modifier = 3.f;
		else if (exit_material == CHAR_TEX_PLASTIC)
			compined_penetration_modifier = 2.0f;
	}

	float thickness = (trace_exit.endpos - data.m_enter_trace.endpos).LengthSqr();
	float modifier = fmaxf(0.f, 1.f / compined_penetration_modifier);

	float lost_damage = fmaxf(
		((modifier * thickness) / 24.f) //* 0.041666668
		+ ((data.m_current_damage * final_damage_modifier)
			+ (fmaxf(3.75 / info->penetration, 0.f) * 3.f * modifier)), 0.f);

	if (lost_damage > data.m_current_damage)
		return false;

	if (lost_damage > 0.f)
		data.m_current_damage -= lost_damage;

	if (data.m_current_damage < 1.f)
		return false;

	data.m_current_position = trace_exit.endpos;
	data.m_penetration_count--;

	return true;
}

bool autowall::trace_to_exit(trace_t * enter_trace, vec3_t start, vec3_t dir, trace_t * exit_trace) {
	vec3_t end;
	float distance = 0.f;
	signed int distance_check = 23;
	int first_contents = 0;

	do {
		distance += 4.f;
		end = start + dir * distance;

		if (!first_contents)
			first_contents = g_csgo.m_trace()->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		int point_contents = g_csgo.m_trace()->GetPointContents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

		if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) {
			vec3_t new_end = end - (dir * 4.f);

			Ray_t ray;
			ray.Init(end, new_end);

			g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
				util::trace_line(end, start, MASK_SHOT_HULL | CONTENTS_HITBOX, exit_trace->hit_entity, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startsolid)
					return true;

				continue;
			}

			if (exit_trace->DidHit() && !exit_trace->startsolid) {
				if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
					if (exit_trace->plane.normal.Dot(dir) <= 1.f)
						return true;

					continue;
				}

				if (is_breakable_entity(enter_trace->hit_entity) && is_breakable_entity(exit_trace->hit_entity))
					return true;

				continue;
			}

			if (exit_trace->surface.flags & SURF_NODRAW) {
				if (is_breakable_entity(enter_trace->hit_entity) && is_breakable_entity(exit_trace->hit_entity))
					return true;
				else if (!(enter_trace->surface.flags & SURF_NODRAW))
					continue;
			}

			if ((!enter_trace->hit_entity || enter_trace->hit_entity->EntIndex() == 0) && (is_breakable_entity(enter_trace->hit_entity))) {
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

void autowall::trace_line(vec3_t & start, vec3_t & end, unsigned int mask, IClientEntity * ignore, trace_t * trace) {
	Ray_t ray;
	ray.Init(start, end);

	CTraceFilter filter;
	filter.pSkip = ignore;

	g_csgo.m_trace()->TraceRay(ray, mask, &filter, trace);
}

void autowall::clip_trace_to_player(vec3_t & start, vec3_t & end, player_t * e, unsigned int mask, ITraceFilter * filter, trace_t * old_trace) {
	if (!e)
		return;

	vec3_t mins = e->GetCollideable()->OBBMins(), maxs = e->GetCollideable()->OBBMaxs();

	vec3_t dir(end - start);
	dir.Normalize();

	vec3_t
		center = (maxs + mins) / 2,
		pos(center + e->m_vecOrigin());

	vec3_t to = pos - start;
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
		trace_t trace;

		Ray_t ray;
		ray.Init(start, end);

		g_csgo.m_trace()->ClipRayToEntity(ray, mask, e, &trace);

		if (old_trace->fraction > trace.fraction)
			*old_trace = trace;
	}
}

bool autowall::is_breakable_entity(IClientEntity * e) {
	using Fn = bool(__fastcall *)(IClientEntity *);
	//static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));
	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));

	if (!e || !e->EntIndex())
		return false;

	auto take_damage{ (char *)((uintptr_t)e + *(size_t*)((uintptr_t)fn + 38)) };
	auto take_damage_backup{ *take_damage };

	auto * cclass = g_csgo.m_client()->GetAllClasses();

	if ((cclass->m_pNetworkName[1]) != 'F'
		|| (cclass->m_pNetworkName[4]) != 'c'
		|| (cclass->m_pNetworkName[5]) != 'B'
		|| (cclass->m_pNetworkName[9]) != 'h')
		*take_damage = DAMAGE_YES;

	bool breakable = fn(e);
	*take_damage = take_damage_backup;

	return breakable;
}
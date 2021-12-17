#include "..\..\ragebot.h"
#include "..\autowall\autowall.h"
#include "..\sdk\animation_state.h"
#include "..\..\Resolver_Remake.h"
#include "..\..\Backtracking.h"
#include "..\lagcompensation\lagcompensation.h"
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
void aimbot::create_move()
{
	g_csgo.m_engine()->GetViewAngles(this->engine_angles);

	this->local_weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	g_ctx.get_command()->m_viewangles -= g_ctx.m_local->m_aimPunchAngle() * 2.f;

	if (!local_weapon)
		return;

	if ((g_ctx.m_local->m_flNextAttack() > util::server_time())) return;

	if (local_weapon->is_non_aim() || local_weapon->m_iClip1() < 1) return;

	if (local_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		if (g_cfg.ragebot.autoresolver) g_ctx.get_command()->m_buttons |= IN_ATTACK;
		float flPostponeFireReady = local_weapon->m_flPostponeFireReadyTime();

		if (flPostponeFireReady && flPostponeFireReady < g_csgo.m_globals()->m_curtime + g_csgo.m_globals()->m_interval_per_tick * g_csgo.m_clientstate()->m_nChokedCommands && g_cfg.ragebot.autoshoot) {
			g_ctx.get_command()->m_buttons &= ~IN_ATTACK; return;
		}
	}
	if (!local_weapon->can_fire()) return;

	iterate_players();

	aim();
}
void aimbot::iterate_players()
{
	target_data.reset();

	static weapon_t * old_weapon;
	if (local_weapon != old_weapon) {
		old_weapon = local_weapon;

		g_ctx.get_command()->m_buttons &= ~IN_ATTACK;

		return;
	}

	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++)
	{
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e->valid(true))
			continue;

		matrix3x4_t bones[128];
		if (!e->SetupBones(bones, 128, 0x100, 0.f))
			continue;

		float simulation_time = e->m_flSimulationTime();

		vec3_t location = best_point(e, bones);
		bool backtrack = false;
		tickrecord_t lag_record;

		if (location == vec3_t(0, 0, 0))
		{
			auto & player = lagcompensation::get().players[i];

			if (player.m_e && !player.get_valid_track().empty())
			{
				bt_point_return_t return_info = best_point_backtrack(e, player);

				location = return_info.point;
				lag_record = return_info.record;
				simulation_time = lag_record.m_simulation_time;

				CBacktracking::get().ShotBackTrackAimbotStart(e);
				CBacktracking::get().RestoreTemporaryRecord(e);
				CBacktracking::get().ShotBackTrackedTick(e);

				backtrack = true;
			}
		}

		if (location == vec3_t(0, 0, 0))
			continue;

		float fov = math::get_fov(engine_angles, math::calculate_angle(g_ctx.m_local->get_eye_pos(), location));
		if (fov > g_cfg.ragebot.field_of_view)
			continue;

		get_target(e, fov, location, simulation_time, backtrack, lag_record);
	}
}
void aimbot::zeus_run()
{
	Vector aim_pos;
	auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(target_data.tid));
	if (!g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_TASER)
		return;

	Vector point = e->hitbox_position(2);
	if (util::visible(g_ctx.m_local->get_eye_pos(), point, e, g_ctx.m_local)) {
		if ((g_ctx.m_local->m_vecOrigin() - e->m_vecOrigin()).Length() <= 180) {
			aim_pos = math::calculate_angle(g_ctx.m_local->get_eye_pos(), point);
			aim_pos.Clamp();
			g_ctx.get_command()->m_buttons |= IN_ATTACK;
		}
	}
	if (g_ctx.get_command()->m_buttons & IN_ATTACK)
		g_ctx.get_command()->m_viewangles = aim_pos;
}

bool aimbot::can_zeus(Vector &bestpoint)
{
	auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(target_data.tid));
	Vector forward;
	math::angle_vectors(bestpoint, forward);
	forward *= 183;

	Vector eye_pos = g_ctx.m_local->get_eye_pos();

	Ray_t ray;
	trace_t trace;
	ray.Init(eye_pos, eye_pos + forward);

	CTraceFilter filter;
	filter.pSkip = g_ctx.m_local;

	g_csgo.m_trace()->TraceRay(ray, MASK_SHOT, &filter, &trace);

	return trace.hit_entity == e;
}

float aimbot::GetLerpTime()
{
	static ConVar* cl_interp = g_csgo.m_cvar()->FindVar("cl_interp");
	static ConVar* cl_updaterate = g_csgo.m_cvar()->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");
	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;
	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;
	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;
	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;
	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;
	else
		return v20;
}

void aimbot::aim()
{
	if (target_data.tid == -1)
		return;

	auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(target_data.tid));

	if (e->m_bGunGameImmunity())
		return;

	vec3_t next_angle = math::calculate_angle(g_ctx.m_local->get_eye_pos(), target_data.aimspot);

	bool hitchanced = false;

	if (g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_SCAR20)
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_scar;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_scar;
	}
	else if (g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_G3SG1)
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_scar;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_scar;
	}
	else if (g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_SSG08)
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_scout;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_scout;
	}
	else if (g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_AWP)
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_awp;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_awp;
	}
	else if (local_weapon->is_pistol())
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_pistol;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_pistol;
	}
	else
	{
		g_cfg.ragebot.hit_chance = g_cfg.ragebot.hit_other;
		g_cfg.ragebot.min_dmg = g_cfg.ragebot.min_other;
	}
	if (g_cfg.ragebot.hitchance)
		hitchanced = hitchance(next_angle, e, g_cfg.ragebot.hit_chance);
	else
		hitchanced = true;

	if (hitchanced)
	{
		if (g_cfg.ragebot.zeus_bot)
		{
			if (can_zeus(next_angle))
				zeus_run();
			else
				g_ctx.get_command()->m_viewangles = next_angle;
		}
		else {
			g_ctx.get_command()->m_viewangles = next_angle;
		}

		g_ctx.get_command()->m_viewangles = next_angle;

		if (g_cfg.ragebot.anti_recoil)
		{
			g_ctx.get_command()->m_viewangles -= g_ctx.m_local->m_aimPunchAngle() * 2.f;
			math::normalize_vector(g_ctx.get_command()->m_viewangles);
		}

		if (!g_cfg.ragebot.silent_aim)
			g_csgo.m_engine()->SetViewAngles(next_angle);

		if (g_cfg.ragebot.autoshoot)
		{
			CBacktracking::get().ShotBackTrackAimbotStart(e);
			CBacktracking::get().RestoreTemporaryRecord(e);
			CBacktracking::get().ShotBackTrackedTick(e);

			g_ctx.get_command()->m_buttons |= IN_ATTACK;

			if (g_cfg.misc.anti_untrusted)
				g_ctx.send_packet = false;
		}
		if (g_cfg.ragebot.anti_recoil)
			g_ctx.get_command()->m_viewangles -= g_ctx.m_local->m_aimPunchAngle() * 2.f;

		if (!g_cfg.ragebot.silent_aim)
			g_csgo.m_engine()->SetViewAngles(next_angle);

		if (g_ctx.get_command()->m_buttons & IN_ATTACK)
			g_ctx.get_command()->m_tickcount = TIME_TO_TICKS(target_data.simtime + util::lerp_time());

		g_ctx.m_globals.aimbotting = true;
	}

	if (g_cfg.ragebot.quickstop && g_ctx.m_globals.aimbotting && g_ctx.m_local->m_fFlags() & FL_ONGROUND) quickstop();
	if (g_cfg.ragebot.autostop && g_ctx.m_globals.aimbotting &&g_ctx.m_local->m_fFlags() & FL_ONGROUND) autostop(e, hitchanced);
}
vec3_t aimbot::best_point(player_t * e, matrix3x4_t bones[128]) {
	vec3_t output = vec3_t(0, 0, 0);
	//bool g_cfg.ragebot.hit
	std::vector< int >
		hitboxes = hitboxes_from_vector(e, e->m_fFlags(), e->m_vecVelocity(), g_cfg.ragebot.hitscan),
		multipoints = hitboxes_from_vector(e, e->m_fFlags(), e->m_vecVelocity(), g_cfg.ragebot.multipoint_hitboxes);
	float best_damage = 0.f;
	for (auto current : hitboxes) {
		vec3_t location = e->hitbox_position(current, bones);
		if (g_cfg.ragebot.autowall)
		{
			float damage = autowall::get().calculate_return_info(g_ctx.m_local->get_eye_pos(), location, g_ctx.m_local, e).m_damage;
			if ((damage > best_damage) && (damage >= g_cfg.ragebot.min_dmg))
			{
				best_damage = damage;
				output = location;
			}
		}
		else if (!g_cfg.ragebot.autowall && g_ctx.m_local->point_visible(location)) return output;
	}
	for (auto current : multipoints)
	{
		std::vector< vec3_t > points;  getpoints(e, current, bones, points);
		for (auto point : points)
		{
			if (g_cfg.ragebot.autowall)
			{
				float damage = autowall::get().calculate_return_info(g_ctx.m_local->get_eye_pos(), point, g_ctx.m_local, e).m_damage;
				if ((damage > best_damage) && (damage > g_cfg.ragebot.min_dmg)) {
					best_damage = damage;
					output = point;
				}
			}
			else if (!g_cfg.ragebot.autowall && g_ctx.m_local->point_visible(point)) return point;
		}
	}
	return output;
}
aimbot::bt_point_return_t aimbot::best_point_backtrack(player_t * e, player_record_t & player) {
	auto hitgroup_from_hitbox = [](int hitbox) -> int {
		switch (hitbox) {
		case HITBOX_HEAD:
		case HITBOX_NECK:
			return HITGROUP_HEAD;
			break;
		case HITBOX_PELVIS:
		case HITBOX_STOMACH:
			return HITGROUP_STOMACH;
			break;
		case HITBOX_LOWER_CHEST:
		case HITBOX_CHEST:
		case HITBOX_UPPER_CHEST:
			return HITGROUP_CHEST;
			break;
		case HITBOX_RIGHT_THIGH:
		case HITBOX_RIGHT_CALF:
		case HITBOX_RIGHT_FOOT:
			return HITGROUP_RIGHTLEG;
			break;
		case HITBOX_LEFT_THIGH:
		case HITBOX_LEFT_CALF:
		case HITBOX_LEFT_FOOT:
			return HITGROUP_LEFTLEG;
			break;
		case HITBOX_RIGHT_HAND:
		case HITBOX_RIGHT_UPPER_ARM:
		case HITBOX_RIGHT_FOREARM:
			return HITGROUP_RIGHTARM;
			break;
		case HITBOX_LEFT_UPPER_ARM:
		case HITBOX_LEFT_FOREARM:
		case HITBOX_LEFT_HAND:
			return HITGROUP_LEFTARM;
			break;
		}
	};
	std::deque< tickrecord_t > track = player.get_valid_track();
	tickrecord_t best_record;
	float best_damage = g_cfg.ragebot.min_dmg;
	vec3_t best_position;
	for (int i = 0; i < track.size(); i++)
	{
		tickrecord_t record = track[i];
		std::vector< int > hitboxes = hitboxes_from_vector(e, record.m_flags, record.m_velocity, g_cfg.ragebot.hitscan_history);
		for (auto hitbox : hitboxes) {
			vec3_t location = e->hitbox_position(hitbox, record.m_bone_matrix);
			if (g_cfg.ragebot.autowall) {
				float damage = autowall::get().calculate_return_info(g_ctx.m_local->get_eye_pos(), location, g_ctx.m_local, e, hitgroup_from_hitbox(hitbox)).m_damage;
				if (damage > best_damage) {
					best_position = location;
					best_damage = damage;
					best_record = record;
				}
			}
			else if (!g_cfg.ragebot.autowall && g_ctx.m_local->point_visible(location)) {
				float damage = 50; autowall::get().scale_damage(e, local_weapon->get_csweapon_info(), hitgroup_from_hitbox(hitbox), damage);
				if (damage > best_damage) {
					best_position = location;
					best_damage = damage;
					best_record = record;
				}
			}
		}
	}
	return bt_point_return_t{ best_record, best_position };
}
void aimbot::getpoints(player_t * e, int hitbox_id, matrix3x4_t bones[128], std::vector< vec3_t > & points) {
	studiohdr_t * studio_model = g_csgo.m_modelinfo()->GetStudiomodel(e->GetModel());
	mstudiohitboxset_t * set = studio_model->pHitboxSet(e->m_nHitboxSet());
	auto m_weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	mstudiobbox_t * hitbox = set->pHitbox(hitbox_id);
	if (!hitbox) return;
	vec3_t min, max;
	math::vector_transform(hitbox->bbmin, bones[hitbox->bone], min);
	math::vector_transform(hitbox->bbmax, bones[hitbox->bone], max);
	vec3_t
		center = (min + max) * 0.5f,
		angle = math::calculate_angle(center, g_ctx.m_local->get_eye_pos());
	vec3_t forward;
	math::angle_vectors(angle, forward);
	vec3_t
		right = forward.Cross(vec3_t(0, 0, 1)),
		left = vec3_t(-right.x, -right.y, right.z),
		top = vec3_t(0, 0, 1),
		bot = vec3_t(0, 0, -1);
	if (hitbox_id == HITBOX_HEAD)
	{
		points.push_back(center + (top * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (right * (hitbox->m_flRadius *  (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_RIGHT_UPPER_ARM || hitbox_id == HITBOX_LEFT_UPPER_ARM || hitbox_id == HITBOX_LEFT_FOREARM || hitbox_id == HITBOX_RIGHT_FOREARM) {
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_LEFT_FOOT || hitbox_id == HITBOX_RIGHT_FOOT || hitbox_id == HITBOX_LEFT_CALF || hitbox_id == HITBOX_RIGHT_CALF || hitbox_id == HITBOX_LEFT_THIGH || hitbox_id == HITBOX_RIGHT_THIGH)
	{
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_PELVIS)
	{
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_STOMACH)
	{
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_NECK)
	{
		points.push_back(center + (right * (hitbox->m_flRadius *(g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else if (hitbox_id == HITBOX_CHEST || hitbox_id == HITBOX_LOWER_CHEST || hitbox_id == HITBOX_UPPER_CHEST)
	{
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
	else {
		points.push_back(center + (right * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
		points.push_back(center + (left * (hitbox->m_flRadius * (g_cfg.ragebot.multiscale / 100))));
	}
}
void aimbot::quickstop()
{
	auto weapon = g_ctx.m_local->m_hActiveWeapon();
	if (!weapon) return;
	if (weapon->is_non_aim() || weapon->m_iItemDefinitionIndex() == WEAPON_TASER || weapon->m_iItemDefinitionIndex() == WEAPON_KNIFE) return;
	if (weapon->can_fire())
	{
		g_ctx.get_command()->m_sidemove = 0;
		g_ctx.get_command()->m_forwardmove = g_ctx.m_local->m_vecVelocity().Length2D() > 20.f ? 450.f : 0.f;
		math::rotate_movement(math::calculate_angle(vec3_t(0, 0, 0), g_ctx.m_local->m_vecVelocity()).y + 180.f);
		fridge::ConsolePrint("Quick Stopped! \n");
	}
}
void aimbot::autostop(player_t * target, bool doinAimbot)
{
	if (!g_ctx.m_local) return;
	Vector local_position = g_ctx.m_local->m_vecOrigin() + g_ctx.m_local->m_vecViewOffset();
	if (target->m_bGunGameImmunity()) return;
	if (target->IsDormant()) return;
	auto weapon = g_ctx.m_local->m_hActiveWeapon();
	if (!weapon) return;
	if (weapon->is_non_aim() || weapon->m_iItemDefinitionIndex() == WEAPON_TASER || weapon->m_iItemDefinitionIndex() == WEAPON_KNIFE) return;
	if (!doinAimbot) return;
	//float damage = autowall::get().calculate_return_info(local_position, target->hitbox_position(128), g_ctx.m_local, target).m_damage;
	if (weapon->can_fire())
	{
		math::rotate_movement(math::calculate_angle(Vector(0, 0, 0), g_ctx.m_local->m_vecVelocity()).y + 180.f);
		fridge::ConsolePrint("Aimbot: Stopped movement! \n");
	}
}
void aimbot::owowalk(player_t * target, bool doinAimbot) {
	if (!g_ctx.m_local) return;
	Vector local_position = g_ctx.m_local->m_vecOrigin() + g_ctx.m_local->m_vecViewOffset();
	if (target->m_bGunGameImmunity()) return;
	if (target->IsDormant()) return;
	auto weapon = g_ctx.m_local->m_hActiveWeapon();
	if (!weapon) return;
	if (weapon->is_non_aim() || weapon->m_iItemDefinitionIndex() == WEAPON_TASER || weapon->m_iItemDefinitionIndex() == WEAPON_KNIFE) return;
	if (!doinAimbot) return;
	g_ctx.m_globals.fakewalking = true;
	g_ctx.send_packet = true;
	Vector velocity = g_ctx.unpred_velocity;
	int32_t
		ticks_to_update = TIME_TO_TICKS(antiaim::get().m_flNextBodyUpdate - util::server_time()) - 1.1;
	const int32_t max_ticks = std::min<int32_t>(7, ticks_to_update);
	const int32_t chocked = g_csgo.m_clientstate()->m_nChokedCommands;
	int32_t ticks_left = max_ticks - chocked;
	if (chocked < max_ticks) g_ctx.send_packet = false;
	else g_ctx.send_packet = true;
	if (weapon->can_fire())
	{
		if (!chocked || g_ctx.send_packet) {
			float BAKAA = g_cfg.misc.fakewalkspeed;
			static int choked = 0;
			choked = choked > 14 ? 0 : choked + 1;
			g_ctx.get_command()->m_forwardmove = choked < BAKAA || choked > 14 ? 0 : g_ctx.get_command()->m_forwardmove;
			g_ctx.get_command()->m_sidemove = choked < BAKAA || choked > 14 ? 0 : g_ctx.get_command()->m_sidemove;
		}
	}
}
bool aimbot::hitchance(vec3_t angles, player_t * e, float chance) {
	angles -= g_ctx.m_local->m_aimPunchAngle() * recoil_scale;

	vec3_t
		forward,
		right,
		up,
		src = g_ctx.m_local->get_eye_pos();

	math::angle_vectors(angles, &forward, &right, &up);

	int
		hits = 0,
		needed_hits = static_cast<int>(256.f * (chance / 100.f));

	float
		weap_spread = local_weapon->get_spread(),
		weap_inaccuracy = local_weapon->get_innacuracy();

	for (int i = 0; i < 256; i++) {
		float
			a = math::random_float(0.f, 1.f),
			b = math::random_float(0.f, 2.f * DirectX::XM_PI),
			c = math::random_float(0.f, 1.f),
			d = math::random_float(0.f, 2.f * DirectX::XM_PI),
			inaccuracy = a * weap_inaccuracy,
			spread = c * weap_spread;

		if (local_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER) {
			if (g_ctx.get_command()->m_buttons & IN_ATTACK2) {
				a = 1.f - a * a;
				c = 1.f - c * c;
			}
		}

		vec3_t
			spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0),
			direction;

		direction.x = forward.x + (spread_view.x * right.x) + (spread_view.y * up.x);
		direction.y = forward.y + (spread_view.x * right.y) + (spread_view.y * up.y);
		direction.z = forward.z + (spread_view.x * right.z) + (spread_view.y * up.z);
		direction.Normalized();

		vec3_t
			viewangles_spread,
			view_forward;

		math::vector_angles(direction, up, viewangles_spread);
		math::normalize_angles(viewangles_spread);

		math::angle_vectors(viewangles_spread, view_forward);
		view_forward.NormalizeInPlace();

		view_forward = src + (view_forward * local_weapon->get_csweapon_info()->range);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, view_forward);
		g_csgo.m_trace()->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, e, &tr);

		if (tr.hit_entity == e)
			hits++;

		if (static_cast<int>((static_cast<float>(hits) / 256.f) * 100.f) >= chance)
			return true;

		if ((256 - i + hits) < needed_hits)
			return false;
	}

	return false;
}
void aimbot::get_target(player_t * e, float fov, vec3_t location, float simulation_time, bool backtrack, tickrecord_t lag_record) {
	switch (g_cfg.ragebot.selection_type) {
	case 0:
		if (target_data.best_distance < fov) return;
		target_data.best_distance = fov;
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;
		target_data.backtrack = backtrack;
		target_data.best_record = lag_record;
		break;
	case 1:
		if (target_data.best_distance < e->m_iHealth()) return;
		target_data.best_distance = e->m_iHealth();
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;
		target_data.backtrack = backtrack;
		target_data.best_record = lag_record;
		break;
	case 2:
		float distance = sqrt(
			pow(double(location.x - g_ctx.m_local->m_vecOrigin().x), 2) +
			pow(double(location.y - g_ctx.m_local->m_vecOrigin().y), 2) +
			pow(double(location.z - g_ctx.m_local->m_vecOrigin().z), 2));
		if (target_data.best_distance < distance) return;
		target_data.best_distance = distance;
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;
		target_data.backtrack = backtrack;
		target_data.best_record = lag_record;
		break;
	}
}
bool is_slow_walking_aimbot(player_t* entity) {
	float velocity_2D[64], old_velocity_2D[64];
	if (entity->m_vecVelocity().Length2D() != velocity_2D[entity->EntIndex()] && entity->m_vecVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->EntIndex()] = velocity_2D[entity->EntIndex()];
		velocity_2D[entity->EntIndex()] = entity->m_vecVelocity().Length2D();
	}
	if (velocity_2D[entity->EntIndex()] > 1) {
		int tick_counter[64];
		if (velocity_2D[entity->EntIndex()] == old_velocity_2D[entity->EntIndex()]) tick_counter[entity->EntIndex()] += 1;
		else tick_counter[entity->EntIndex()] = 0;
		while (tick_counter[entity->EntIndex()] > (1 / g_csgo.m_globals()->m_interval_per_tick) * fabsf(0.1f)) return true;
	}
	if (entity->m_vecVelocity().Length2D() > 65 && entity->m_vecVelocity().Length2D() < 80)
	{
		return true;
	}
	Vector velocity = entity->m_vecVelocity();
	Vector direction = entity->m_angEyeAngles();
	float speed = velocity.Length();
	direction.y = entity->m_angEyeAngles().y - direction.y;
	if (speed > 129.f) {
		int divider = 127.f / speed;
		Vector slowedDirection = direction * divider * speed;
		if (entity->m_vecVelocity() == slowedDirection) return true;
	}
	return false;
}
bool aimbot::head_check(player_t * e, int m_fFlags, vec3_t m_vecVelocity)
{
	bool use = true;
	//const int resolve_type = ResolverOvermake::resolver.GetPlayerResolveInfo(e).resolve_type;
	//Vector local_position = g_ctx.m_local->m_vecOrigin() + g_ctx.m_local->m_vecViewOffset();

	//if (g_cfg.ragebot.baim_settings[BAIM_INAIR].enabled && !(m_fFlags & FL_ONGROUND))
	//	use = false;

	//if (g_cfg.ragebot.resolver_experimental && g_cfg.ragebot.baim_settings[BAIM_NOTRESOLVED].enabled)
	//{
	//	if (!ResolverOvermake::resolver.IsResolved(resolve_type))
	//		use = false;
	//}
	//if (g_cfg.ragebot.baim_settings[BAIM_BREAKLBY].enabled && ResolverOvermake::resolver.GetPlayerResolveInfo(e).is_balance_adjust_triggered)
	//{
	//	use = false;
	//}
	//if (ResolverOvermake::resolver.IsResolved(resolve_type)) use = true;
	//if (g_cfg.ragebot.baim_settings[BAIM_CANKILL].enabled)
	//{
	//	float damage = g_ctx.m_local->m_hActiveWeapon()->get_csweapon_info()->damage / 1.2;//just to be safe
	//	if (damage > e->m_iHealth())
	//		use = false;
	//}
	//if (g_ctx.m_globals.isFakeing) use = false;
	//if (g_cfg.ragebot.baim_settings[BAIM_SLOWWALK].enabled && ResolverOvermake::resolver.is_slow_walking(e))
	//{
	//	use = false;
	//}
	//if (g_cfg.ragebot.baim_settings[BAIM_ALWAYS].enabled)
	//{
	//	use = false;
	//}
	//if (g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == (short)ItemDefinitionIndex::WEAPON_TASER || g_ctx.m_local->m_hActiveWeapon()->m_iItemDefinitionIndex() == WEAPON_KNIFE) use = false;
	return use;
}
std::vector< int > aimbot::hitboxes_from_vector(player_t * e, int m_fFlags, vec3_t m_vecVelocity, std::vector< MultiDropdownItem_t > arr)
{
	//std::vector< int > hitboxes;
	std::vector< int> hitboxes;
	bool head_valid = head_check(e, m_fFlags, m_vecVelocity);
	if (!g_cfg.ragebot.hsonly)
	{
		if (arr[0].enabled)
		{
			hitboxes.push_back(HITBOX_HEAD);
			hitboxes.push_back(HITBOX_NECK);
		}
		if (arr[1].enabled) {
			hitboxes.push_back(HITBOX_UPPER_CHEST);
			hitboxes.push_back(HITBOX_CHEST);
		}
		if (arr[2].enabled) {
			hitboxes.push_back(HITBOX_PELVIS);
			hitboxes.push_back(HITBOX_STOMACH);
		}
		if (arr[3].enabled) {
			hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
			hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
			hitboxes.push_back(HITBOX_RIGHT_FOREARM);
			hitboxes.push_back(HITBOX_LEFT_FOREARM);
			hitboxes.push_back(HITBOX_RIGHT_HAND);
			hitboxes.push_back(HITBOX_LEFT_HAND);
		}
		if (arr[4].enabled) {
			hitboxes.push_back(HITBOX_RIGHT_THIGH);
			hitboxes.push_back(HITBOX_LEFT_THIGH);
			hitboxes.push_back(HITBOX_RIGHT_CALF);
			hitboxes.push_back(HITBOX_LEFT_CALF);
			hitboxes.push_back(HITBOX_RIGHT_FOOT);
			hitboxes.push_back(HITBOX_LEFT_FOOT);
		}
	}
	else
	{
		hitboxes.push_back(HITBOX_HEAD);
	}
	return hitboxes;
}
void aimbot::update_config()
{
	/*auto id = local_weapon->m_iItemDefinitionIndex();
	int element = -1;
	if (local_weapon->is_pistol() && id != WEAPON_DEAGLE && id != WEAPON_REVOLVER) element = 0;
	if (local_weapon->m_iItemDefinitionIndex() == WEAPON_DEAGLE || local_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER) element = 1;
	if (local_weapon->is_smg()) element = 3;
	if (id == WEAPON_SSG08) element = 4;
	if (id == WEAPON_SCAR20 || id == WEAPON_G3SG1) element = 5;
	if (id == WEAPON_AWP || id == WEAPON_TASER || id == WEAPON_KNIFE) element = 6;
	if (element == -1) element = 2;
	config.autoshoot = g_cfg.ragebot.autoshoot[element];
	config.autowall = g_cfg.ragebot.autowall[element];
	config.minimum_damage = g_cfg.ragebot.minimum_damage[element];
	config.hitscan = g_cfg.ragebot.hitscan[element];
	config.hitscan_history = g_cfg.ragebot.hitscan_history[element];
	config.multipoint = g_cfg.ragebot.multipoint_hitboxes[element];
	config.pointscale = g_cfg.ragebot.pointscale[element];
	config.pointscaleneck = g_cfg.ragebot.pointscaleneck[element];
	config.pointscalehead = g_cfg.ragebot.pointscalehead[element];
	config.pointscalepelvis = g_cfg.ragebot.pointscalepelvis[element];
	config.pointscalestomach = g_cfg.ragebot.pointscalestomach[element];
	config.pontscalearms = g_cfg.ragebot.pointscalearms[element];
	config.pointscalelegs = g_cfg.ragebot.pointscalelegs[element];
	config.pointscalechest = g_cfg.ragebot.pointscalechest[element];
	g_cfg.ragebot.hitchance = g_cfg.ragebot.hitchance[element];
	g_cfg.ragebot.hit_chance = g_cfg.ragebot.hitchance_amount[element];
	config.baim = g_cfg.ragebot.baim_settings[element];
	config.quickstop = g_cfg.ragebot.quickstop[element];
	config.autostop = g_cfg.ragebot.autostopmovement[element];
	config.autofakewalk = g_cfg.ragebot.autofakewalk[element];
	config.baimdmg = g_cfg.ragebot.baimifhp[element];*/
}
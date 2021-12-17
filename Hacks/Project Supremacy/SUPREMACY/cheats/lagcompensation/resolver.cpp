#include "lagcompensation.h"

#include "..\autowall\autowall.h"

void c_player_resolver::run() {
	if (!g_cfg.ragebot.antiaim_correction || !g_ctx.m_local->is_alive())
		return;

	player_info_t info;
	g_csgo.m_engine()->GetPlayerInfo(m_e->EntIndex(), &info);

	if (info.fakeplayer) {
		m_mode = resolver_modes::none;
		return;
	}

	update();
	resolve();
	pitch_resolve();

	m_previous = m_current;
	m_has_previous = true;
}
bool predict_lbyflick(player_t* player, float oldlby[64], float lby, float speed)
{
	static bool nextflick[64];

	static float add_time[64];

	const auto sim = player->m_flSimulationTime();

	if (!g_cfg.ragebot.predictlbyupdate)
		return false;

	for (auto i = 0; i < g_csgo.m_globals()->m_maxclients; ++i)
	{
		if (oldlby[i] != lby && speed <= 0.1f)
		{
			add_time[i] = g_csgo.m_globals()->m_interval_per_tick + 1.1f;
		}

		if (speed >= 0.1f)
		{
			add_time[i] = 0.22f;
			nextflick[i] = sim + add_time[i];
		}

		if (sim >= nextflick[i] && speed <= 0.1f)
		{
			add_time[i] = 1.1f;
			nextflick[i] = sim + add_time[i];
			return true;
		}
	}
	return false;
}
void c_player_resolver::create_move(vec3_t latency_based_eye_pos) {
	if (!g_cfg.ragebot.antiaim_correction)
		return;

	player_t * e = m_e;

	const float height = 64;

	Vector direction_1, direction_2;
	math::angle_vectors(vec3_t(0.f, math::calculate_angle(g_ctx.m_local->m_vecOrigin(), e->m_vecOrigin()).y - 90.f, 0.f), direction_1);
	math::angle_vectors(vec3_t(0.f, math::calculate_angle(g_ctx.m_local->m_vecOrigin(), e->m_vecOrigin()).y + 90.f, 0.f), direction_2);

	const auto left_eye_pos = e->m_vecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
	const auto right_eye_pos = e->m_vecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);

	m_antifreestand.left_damage = autowall::get().calculate_return_info(latency_based_eye_pos, left_eye_pos, g_ctx.m_local, e, 1).m_damage;
	m_antifreestand.right_damage = autowall::get().calculate_return_info(latency_based_eye_pos, right_eye_pos, g_ctx.m_local, e, 1).m_damage;

	Ray_t ray;
	trace_t trace;
	CTraceFilterWorldOnly filter;

	ray.Init(left_eye_pos, latency_based_eye_pos);
	g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
	m_antifreestand.left_fraction = trace.fraction;

	ray.Init(right_eye_pos, latency_based_eye_pos);
	g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
	m_antifreestand.right_fraction = trace.fraction;
}
#define M_PI 3.14159265358979323846
void VectorAnglesBrute(const Vector& forward, Vector &angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0) pitch = 270; else pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0) yaw += 360; tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]); pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0) pitch += 360;
	} angles[0] = pitch; angles[1] = yaw; angles[2] = 0;
}
void c_player_resolver::lby_prediction() {
	static float next_lby_update[65];

	resolver_info_t
		& current = m_current,
		&previous = m_previous;
	player_t * e = m_e;
	if (e->IsDormant()) {
		current.m_predicted_flick = false;
		current.m_flick = false;
		g_ctx.m_globals.add_time[e->EntIndex()] = 0.f;
		g_ctx.m_globals.next_update[e->EntIndex()] = 0.f;
		next_lby_update[e->EntIndex()] = 0.f;
	}

	if (e->get_animtime() >= next_lby_update[e->EntIndex()] && !e->IsDormant())
	{
		current.m_predicted_flick = true;
		g_ctx.m_globals.add_time[e->EntIndex()] = 1.1f;
		next_lby_update[e->EntIndex()] = e->get_animtime() + g_ctx.m_globals.add_time[e->EntIndex()];
		g_ctx.m_globals.next_update[e->EntIndex()] = next_lby_update[e->EntIndex()];
	}
	else
		current.m_predicted_flick = false;

	if (current.m_lowerbody != e->m_flLowerBodyYawTarget() && !e->IsDormant())
	{
		current.m_flick = true;
		g_ctx.m_globals.add_time[e->EntIndex()] = g_csgo.m_globals()->m_interval_per_tick + 1.1f;
		next_lby_update[e->EntIndex()] = e->get_animtime() + g_ctx.m_globals.add_time[e->EntIndex()];
		g_ctx.m_globals.next_update[e->EntIndex()] = next_lby_update[e->EntIndex()];
	}
	else
		current.m_flick = false;

	if (current.m_velocity.Length2D() > 0.1f && !current.m_fakewalking && !e->IsDormant()) {
		g_ctx.m_globals.add_time[e->EntIndex()] = 0.22f;
		next_lby_update[e->EntIndex()] = e->get_animtime() + g_ctx.m_globals.add_time[e->EntIndex()];
		g_ctx.m_globals.next_update[e->EntIndex()] = next_lby_update[e->EntIndex()];
	}
}

void c_player_resolver::update() {
	resolver_info_t
		& current = m_current,
		&previous = m_previous;
	player_t * e = m_e;

	// update basic data
	current.m_velocity = e->m_vecVelocity();
	current.m_origin = e->m_vecOrigin();
	current.m_lowerbody = e->m_flLowerBodyYawTarget();
	current.m_flags = e->m_fFlags();

	current.back = vec3_t(e->m_angEyeAngles().x, math::calculate_angle(e->m_vecOrigin(), g_ctx.m_local->m_vecOrigin()).y + 180.f, 0.f);
	current.right = vec3_t(e->m_angEyeAngles().x, math::calculate_angle(e->m_vecOrigin(), g_ctx.m_local->m_vecOrigin()).y + 70.f + ((rand() % 40) - (40 * 0.5f)), 0.f);
	current.left = vec3_t(e->m_angEyeAngles().x, math::calculate_angle(e->m_vecOrigin(), g_ctx.m_local->m_vecOrigin()).y - 70.f + ((rand() % 40) - (40 * 0.5f)), 0.f);

	current.m_at_target = math::calculate_angle(g_ctx.m_local->m_vecOrigin(), current.m_origin).y;

	// check fakewalk
	update_fakewalk_data(current, e);

	const bool moving_on_ground = current.m_velocity.Length2D() > 0.1 && current.m_flags & FL_ONGROUND;

	// override
	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.ragebot.override_key))
		m_mode = resolver_modes::override;

	// air check
	if (!(current.m_flags & FL_ONGROUND && (!moving_on_ground && current.m_fakewalking)))
		m_mode = resolver_modes::brute;

	// anti freestand
	if (m_antifreestand.left_damage < 20 && m_antifreestand.right_damage < 20 && (!moving_on_ground && !current.m_fakewalking))
		m_mode = resolver_modes::anti_freestand;

	// lby checks
	if (current.m_lowerbody != previous.m_lowerbody) {
		m_mode = resolver_modes::lby_update;

		if ((e->m_flSimulationTime() - current.m_last_move_time) > .22f)
			current.m_predicted_yaw_brute += current.m_lowerbody - previous.m_lowerbody;
	}

	if (moving_on_ground && !current.m_fakewalking && predict_lbyflick(e, &previous.m_lowerbody, current.m_lowerbody, current.m_velocity.Length2D())) {
		m_mode = resolver_modes::lby;

		// last moving lby
		current.m_last_move_lby = current.m_lowerbody;
		current.m_last_move_lby_valid = true;
		current.m_last_move_time = e->m_flSimulationTime();

		//reset brute data
		current.m_predicted_yaw_brute = 0.f;
	}

	// pitch fix
	if (math::normalize_pitch(current.m_pitch) > 5.f)
		current.last_pitchdown = g_csgo.m_globals()->m_curtime;
}

void c_player_resolver::update_fakewalk_data(resolver_info_t & current, player_t * e) {
	resolver_info_t previous = m_previous;

	AnimationLayer anim_layers[15];
	bool s_1 = false,
		s_2 = false,
		s_3 = false;

	for (int i = 0; i < e->animlayer_count(); i++)
	{
		anim_layers[i] = e->get_animlayers()[i];
		if (anim_layers[i].m_nSequence == 26 && anim_layers[i].m_flWeight < 0.4f)
			s_1 = true;
		if (anim_layers[i].m_nSequence == 7 && anim_layers[i].m_flWeight > 0.001f)
			s_2 = true;
		if (anim_layers[i].m_nSequence == 2 && anim_layers[i].m_flWeight == 0)
			s_3 = true;
	}

	if (s_1 && s_2)
		if (s_3 || (e->m_fFlags() & FL_DUCKING))
			current.m_fakewalking = true;
		else
			current.m_fakewalking = false;
	else
		current.m_fakewalking = false;
}

void c_player_resolver::resolve() {
	resolver_info_t current = m_current, previous = m_previous;
	player_t * e = m_e;
	Vector velocityAngle; VectorAnglesBrute(e->m_vecVelocity(), velocityAngle); float primaryBaseAngle = e->m_flLowerBodyYawTarget(); float secondaryBaseAngle = velocityAngle.y;
	int fired = g_ctx.m_globals.shots[e->EntIndex()];
	int missed = g_ctx.m_globals.missed_shots[e->EntIndex()];
	float base_angle = current.m_last_move_lby_valid ? current.m_last_move_lby : current.m_lowerbody;

	// in case we haven't initialize our previous record use this
	if (e->m_vecVelocity().Length2D() > 0.1 && e->m_fFlags() & FL_ONGROUND)
		e->m_angEyeAngles().y = current.m_lowerbody;
	else
		e->m_angEyeAngles().y = current.m_lowerbody + 180;

	// resolve the player
	if (m_mode == resolver_modes::lby)
	{
		e->m_angEyeAngles().y = current.m_lowerbody;
		if (missed > 2)
		{
			if (m_antifreestand.left_damage <= 0 && m_antifreestand.right_damage <= 0) {
				if (m_antifreestand.right_fraction < m_antifreestand.left_fraction)
					e->m_angEyeAngles().y = current.m_at_target + 125.f;
				else
					e->m_angEyeAngles().y = current.m_at_target - 73.f;
			}
			else {
				if (m_antifreestand.left_damage > m_antifreestand.right_damage)
					e->m_angEyeAngles().y = current.m_at_target + 130.f;
				else
					e->m_angEyeAngles().y = current.m_at_target - 55.f;
			}
		}
	}

	if (m_mode == resolver_modes::override) {
		vec3_t screen;
		if (math::world_to_screen(e->m_vecOrigin(), screen)) {
			POINT mouse; GetCursorPos(&mouse);
			float delta = mouse.x - screen.x;

			if (delta > 0)
				e->m_angEyeAngles().y = current.m_at_target - 90;
			else if (delta < 0)
				e->m_angEyeAngles().y = current.m_at_target + 90;
		}
	}

	else if (m_mode == resolver_modes::lby_update)
		e->m_angEyeAngles().y = current.m_lowerbody;

	else if (m_mode == resolver_modes::anti_freestand) {
		if (m_antifreestand.left_damage <= 0 && m_antifreestand.right_damage <= 0) {
			if (m_antifreestand.right_fraction < m_antifreestand.left_fraction)
				e->m_angEyeAngles().y = current.m_at_target + 125.f;
			else
				e->m_angEyeAngles().y = current.m_at_target - 73.f;
		}
		else {
			if (m_antifreestand.left_damage > m_antifreestand.right_damage)
				e->m_angEyeAngles().y = current.m_at_target + 130.f;
			else
				e->m_angEyeAngles().y = current.m_at_target - 55.f;
		}
	}
	else {
		auto best = [](float primary, float secondary, float defined, bool accurate) -> float
		{
			if (accurate)
			{
				if (math::angle_distance(math::normalize_yaw(primary), math::normalize_yaw(defined)) <= 50)
					return math::normalize_yaw(primary);
				else if (math::angle_distance(math::normalize_yaw(secondary), math::normalize_yaw(defined)) <= 50)
					return math::normalize_yaw(secondary);
				else
					return math::normalize_yaw(defined);
			}
			else
			{
				if (math::angle_distance(math::normalize_yaw(primary), math::normalize_yaw(defined)) <= 80)
					return math::normalize_yaw(primary);
				else if (math::angle_distance(math::normalize_yaw(secondary), math::normalize_yaw(defined)) <= 80)
					return math::normalize_yaw(secondary);
				else
					return math::normalize_yaw(defined);
			}
		};
		e->m_angEyeAngles().y = base_angle + (current.m_predicted_yaw_brute * fired);
	}
}

void c_player_resolver::pitch_resolve() {
	resolver_info_t current = m_current;
	player_t * e = m_e;

	// to stop the pitch going to 0 (when using fakewalk, max fakelag, etc)

	if (fabs(g_csgo.m_globals()->m_curtime - current.last_pitchdown) < 0.5f)
		e->m_angEyeAngles().x = 70.f;
	else
		e->m_angEyeAngles().x = e->m_angEyeAngles().x;
}
/*
int __cdecl PitchResolve1(int viewangles, unsigned __int8 swap)
 {
	int result; // eax@1

	result = swap;
	if (swap)
		*(_DWORD *)(viewangles + 12) = -89;
	else
		*(_DWORD *)(viewangles + 12) = 89;
	return result;
}*/
#include "legit_backtrack.h"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / g_csgo.m_globals( )->m_interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( g_csgo.m_globals( )->m_interval_per_tick *( t ) )

struct backtrack_data {
	float simtime;
	vec3_t hitbox_position;
};

backtrack_data hitbox_positions[64][12];

void legitbacktrack::create_move() {
	static auto distance_point_line = [](Vector point, Vector line_origin, Vector direction) {
		auto point_direction = point - line_origin;

		auto temp_offset = point_direction.Dot(direction) / (direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
		if (temp_offset < 0.000001f)
			return FLT_MAX;

		auto perp_point = line_origin + (direction * temp_offset);

		return (point - perp_point).Length();
	};

	if (!g_ctx.m_local || !g_ctx.m_local->is_alive14())
		return;

	int target_index = -1;
	float max_fov = FLT_MAX;

	for (int i = 1; i < g_csgo.m_globals()->m_maxclients; i++) {
		auto m_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (m_player->valid(true)) {
			vec3_t head_pos = m_player->hitbox_position(HITBOX_HEAD);
			float simulation_time = m_player->m_flSimulationTime();

			hitbox_positions[i][g_ctx.get_command()->m_command_number % 13] = backtrack_data{ simulation_time, head_pos };

			vec3_t angles = g_ctx.get_command()->m_viewangles + (g_ctx.m_local->m_aimPunchAngle() * 2.0f);
			vec3_t forward; math::angle_vectors(angles, forward);

			float distance = distance_point_line(head_pos, g_ctx.m_local->get_eye_pos(), forward);

			if (max_fov > distance) {
				max_fov = distance;
				target_index = i;
			}
		}
	}

	if (target_index == -1)
		return;

	float simulation_time = -1;

	max_fov = FLT_MAX;

	vec3_t angles = g_ctx.get_command()->m_viewangles + (g_ctx.m_local->m_aimPunchAngle() * 2.0f);
	vec3_t forward; math::angle_vectors(angles, forward);

	for (int i = 0; i < 12; i++) {
		float distance = distance_point_line(hitbox_positions[target_index][i].hitbox_position, g_ctx.m_local->get_eye_pos(), forward);

		if (hitbox_positions[target_index][i].simtime <= g_ctx.m_local->m_flSimulationTime() - 1)
			continue;

		if (max_fov > distance) {
			max_fov = distance;
			simulation_time = hitbox_positions[target_index][i].simtime;
		}
	}

	if (!simulation_time || simulation_time <= g_ctx.m_local->m_flSimulationTime() - 1)
		return;

	if (g_ctx.get_command()->m_buttons & IN_ATTACK)
		g_ctx.get_command()->m_tickcount = TIME_TO_TICKS(simulation_time);
}

void legitbacktrack::paint_traverse() {
	if (!g_ctx.m_local || !g_ctx.m_local->is_alive15())
		return;

	if (!g_cfg.legit.backtracking)
		return;

	for (int i = 1; i < g_csgo.m_globals()->m_maxclients; i++) {
		auto m_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!m_player->valid(true))
			continue;

		for (int j = 0; j < 12; j++) {
			if (hitbox_positions[i][j].simtime <= g_ctx.m_local->m_flSimulationTime() - 1)
				continue;

			if (!g_ctx.m_local->point_visible(hitbox_positions[i][j].hitbox_position))
				continue;

			vec3_t screen;
			if (math::world_to_screen(hitbox_positions[i][j].hitbox_position, screen))
				render::get().rect_filled(screen.x, screen.y, 1, 1, g_cfg.legit.backtracking_visualize_color);
		}
	}
}
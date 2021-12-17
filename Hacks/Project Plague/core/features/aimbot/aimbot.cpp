#include "../features.hpp"
#include "../../../security/cl_junkcode.hpp"
aimbot_ctx_t aimbot::ctx;

void aimbot::weapon_cfg(weapon_t* weapon) {
	junkcode::call();
	ctx.hitbox_pos = vec3_t{ 0.0f, 0.0f, 0.0f };
	if (weapon->is_pistol()) {
		ctx.enabled = variables.aimbot.pistols.enabled;
		ctx.mode = variables.aimbot.pistols.mode;
		ctx.hitpoint = variables.aimbot.pistols.hitpoint;
		ctx.fov = variables.aimbot.pistols.fov;
		ctx.smooth = variables.aimbot.pistols.smooth;
		ctx.rcs = true;
	}
	else if (weapon->is_sniper()) {
		ctx.enabled = variables.aimbot.snipers.enabled;
		if (!csgo::local_player->is_scoped())
			ctx.enabled = false;
		ctx.mode = variables.aimbot.snipers.mode;
		ctx.hitpoint = variables.aimbot.snipers.hitpoint;
		ctx.fov = variables.aimbot.snipers.fov;
		ctx.smooth = variables.aimbot.snipers.smooth;
		ctx.rcs = true;
	}
	else {
		ctx.enabled = variables.aimbot.rifles.enabled;
		ctx.mode = variables.aimbot.rifles.mode;
		ctx.hitpoint = variables.aimbot.rifles.hitpoint;
		ctx.fov = variables.aimbot.rifles.fov;
		ctx.smooth = variables.aimbot.rifles.smooth;
		ctx.rcs = true;
	}
}
int aimbot::get_hitbox(int hitpoint)
{
	junkcode::call();
	if (hitpoint != hitbox_head && hitpoint != hitbox_neck)
		return hitbox_chest;

	return hitpoint;
}
int aimbot::get_hitbox(player_t* player, matrix_t* matrix, vec3_t eye_pos) {
	junkcode::call();
	vec3_t view_angle;
	interfaces::engine->get_view_angles(view_angle);
	float max_fov = ctx.fov;
	int closest_hitbox = hitbox_chest;
	for (int i = 0; i <= 6; i++)
	{
		float fov_distance = math::get_fov(view_angle + (csgo::local_player->aim_punch_angle() * interfaces::console->get_convar("weapon_recoil_scale")->get_float()), math::calculate_angle(eye_pos, player->get_hitbox_position(i, matrix)));

		if (max_fov > fov_distance)
		{
			max_fov = fov_distance;
			closest_hitbox = i;
		}
	}
	return closest_hitbox;
}

void aimbot::run(c_usercmd* cmd)
{
	junkcode::call();
	if (!csgo::local_player ||
		!csgo::local_player->is_alive() ||
		!variables.aimbot.ignore_flash && csgo::local_player->flash_alpha())
		return;

	auto weapon = csgo::local_player->active_weapon();

	if(!weapon ||
		weapon->is_knife() ||
		weapon->is_nade() ||
		weapon->is_c4() ||
		weapon->is_taser() ||
		!(cmd->buttons & in_attack))
		return;

	weapon_cfg(weapon);

	if (!ctx.enabled)
		return;

	float max_player_fov = ctx.fov;
	vec3_t aim_angle;
	vec3_t view_angle;
	interfaces::engine->get_view_angles(view_angle);
	vec3_t eye_pos = csgo::local_player->get_eye_pos();
	int closest_player = -1;

	for (int i = 1; i <= 64; i++) {
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player ||
			player->dormant() ||
			!player->is_player() ||
			!player->is_alive() ||
			player->team() == csgo::local_player->team())
			continue;

		vec3_t current_hitbox;

		if (variables.backtracking.enabled && 
			variables.backtracking.aimatbt &&
			backtracking::records[i].size() && 
			backtracking::closest_tick != -1 &&
			backtracking::closest_tick >= 0 &&
			backtracking::closest_tick < backtracking::records[i].size() &&
			backtracking::records[i].at(backtracking::closest_tick).bone_matrix_built &&
			backtracking::records[i].at(backtracking::closest_tick).bone_matrix) {
			vec3_t temp_hitbox = player->get_hitbox_position((ctx.hitpoint == 3) ?
				get_hitbox(player, backtracking::records[i].at(backtracking::closest_tick).bone_matrix, eye_pos)
				: get_hitbox(ctx.hitpoint), backtracking::records[i].at(backtracking::closest_tick).bone_matrix);
			if (variables.aimbot.ignore_smoke || !utilities::goes_through_smoke(eye_pos, temp_hitbox))
				current_hitbox = temp_hitbox;
		}
		else {
			matrix_t bone_matrix[128];
			if (player->setup_bones(bone_matrix, 128, BONE_USED_BY_HITBOX, interfaces::globals->cur_time) && bone_matrix) {
				vec3_t temp_hitbox = player->get_hitbox_position((ctx.hitpoint == 3) ?
					get_hitbox(player, bone_matrix, eye_pos)
					: get_hitbox(ctx.hitpoint), bone_matrix);
				if (variables.aimbot.ignore_smoke || !utilities::goes_through_smoke(eye_pos, temp_hitbox))
					current_hitbox = temp_hitbox;
			}
		}
		if (current_hitbox.is_valid()) {
			float fov_distance = math::get_fov(view_angle + (csgo::local_player->aim_punch_angle() * interfaces::console->get_convar("weapon_recoil_scale")->get_float()), math::calculate_angle(eye_pos, current_hitbox));
			if (max_player_fov > fov_distance)
			{
				if (csgo::local_player->can_see_player_pos(player, current_hitbox))
				{
					max_player_fov = fov_distance;
					closest_player = i;
					ctx.hitbox_pos = current_hitbox;
				}
			}
		}
	}
	if (closest_player != -1 && ctx.hitbox_pos.is_valid())
	{
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(closest_player));
		if (!player) return;
		aim_angle = math::calculate_angle(eye_pos, ctx.hitbox_pos);
		math::sanitize_angle(aim_angle);
		vec3_t recoil_angle = csgo::local_player->aim_punch_angle() * interfaces::console->get_convar("weapon_recoil_scale")->get_float();
		math::sanitize_angle(recoil_angle);
		aim_angle -= recoil_angle;
		math::sanitize_angle(view_angle);
		vec3_t delta_angle = view_angle - aim_angle;
		math::sanitize_angle(delta_angle);
		float smoothing = (ctx.mode == 1 && ctx.smooth > 1.f) ? ctx.smooth : 1.f;
		vec3_t final_angle = view_angle - delta_angle / smoothing;
		if (!math::sanitize_angle(final_angle))
			return;
		cmd->viewangles = final_angle;
		if (ctx.mode != 2)
			interfaces::engine->set_view_angles(cmd->viewangles);
	}
}



#include "../features.hpp"
#include "../../../security/cl_junkcode.hpp"
float server_curtime() {
	junkcode::call();
	const auto v1 = static_cast<i_net_channel_info*> (interfaces::engine->get_net_channel_info());

	const auto v3 = v1->get_latency(1); //local player
	const auto v4 = v1->get_latency(0); //generic

	return v3 + v4 + ticks_to_time(1) + ticks_to_time(csgo::cmd->tick_count);
}
bool lby_update() {
	junkcode::call();
	static auto next_lby_update_time = -1.f;

	auto spawn_time = csgo::local_player->spawn_time();
	auto anim_state = csgo::local_player->get_anim_state();

	const auto current_time = server_curtime();
	{
		if (spawn_time != csgo::local_player->spawn_time())
		{
			spawn_time = csgo::local_player->spawn_time();
			next_lby_update_time = -1.f;
		}

		if (anim_state->speed_2d > 0.1 || !(csgo::local_player->flags() & fl_onground))
			next_lby_update_time = current_time + 0.22f;
		else if (next_lby_update_time == -1.f || current_time >= next_lby_update_time)
		{
			next_lby_update_time = current_time + 1.1f;
		}
	}

	if (anim_state->speed_2d > 0.1)
		return false;

	if (!(csgo::local_player->flags() & fl_onground))
		return false;

	return next_lby_update_time - current_time <= interfaces::globals->interval_per_tick;
}
void antiaim::run(c_usercmd* cmd, float& old_sidemove) {
	junkcode::call();
	if (!variables.misc.antiaim) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;
	weapon_t* weapon = csgo::local_player->active_weapon();
	if (!weapon) return;
	if (csgo::local_player->move_type() == movetype_ladder) return;
	if (cmd->buttons & in_use) return;
	if (cmd->buttons & in_attack) return;
	if (cmd->buttons & in_attack2 && weapon->is_knife()) return;
	if (cmd->buttons & in_attack2 && weapon->item_definition_index() == WEAPON_REVOLVER) return;
	if (weapon->is_nade()) return;
	//if (csgo::local_player->velocity().Length2D() > 10.f) return;
	csgo::aa_running = true;
	csgo::send_packet = interfaces::engine->get_net_channel()->choked_packets >= 2;

	if (variables.misc.antiaim_invert)
		csgo::aa_side = GetKeyState(variables.misc.antiaim_invert);
	else 
		csgo::aa_side = false;

	if (lby_update()) {
		if (csgo::aa_side)
			cmd->viewangles.y += 120;
		else
			cmd->viewangles.y -= 120;
		csgo::send_packet = false;
	}
	else if (!csgo::send_packet) {
		if (csgo::aa_side)
			cmd->viewangles.y -= 120;
		else
			cmd->viewangles.y += 120;
	}
	/*
	const float new_forwardmove = cmd->buttons & in_duck ? 3.0f : 1.1f;
	if (cmd->forwardmove == 0.f) {
		cmd->forwardmove = (cmd->tick_count % 2) ? new_forwardmove : -new_forwardmove;
		old_sidemove = cmd->forwardmove;
	}*/

	cmd->buttons &= ~(in_forward | in_back | in_moveright | in_moveleft);
}
void antiaim::animation_fix(int stage) {
	junkcode::call();
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && stage == FRAME_RENDER_START && csgo::cmd && csgo::local_player != nullptr && csgo::local_player->is_alive()) {
		if (interfaces::input->m_fCameraInThirdPerson) {
			auto state = csgo::local_player->get_anim_state();
			auto layers = csgo::local_player->anim_overlays();
			if (state && layers) {
				auto fresh_tick = []() -> bool {
					static int old_tick = interfaces::globals->tick_count;
					if (old_tick != interfaces::globals->tick_count) {
						old_tick = interfaces::globals->tick_count;
						return true;
					}
					return false;
				};

				static float proper_abs = state->m_flGoalFeetYaw;
				static std::array<float, 24> sent_pose_params = csgo::local_player->pose_param();
				static animation_layer backup_layers[15];
				if (!backup_layers)
					std::memcpy(backup_layers, layers, (sizeof(animation_layer) * 15));
				if (fresh_tick())
				{
					std::memcpy(backup_layers, layers, (sizeof(animation_layer) * 15));
					csgo::local_player->animations(true);
					csgo::local_player->update_state(state, csgo::cmd->viewangles);
					if (state)
						state->m_iLastClientSideAnimationUpdateFramecount = interfaces::globals->frame_count - 1;
					if (csgo::send_packet)
					{
						proper_abs = state->m_flGoalFeetYaw;
						csgo::real_abs = proper_abs;
						sent_pose_params = csgo::local_player->pose_param();
					}
				}
				csgo::local_player->animations(false);
				csgo::local_player->set_abs_angles(vec3_t(0, proper_abs, 0));
				state->m_flUnknownFraction = 0.f; // Lol.
				std::memcpy(layers, backup_layers, (sizeof(animation_layer) * 15));
				csgo::local_player->pose_param() = sent_pose_params;
				csgo::local_player->setup_bones(nullptr, 128, BONE_USED_BY_ANYTHING, interfaces::globals->cur_time);
			}
		}
		else if (interfaces::clientstate->delta_tick != -1)
			csgo::local_player->animations(true);
	}
}
fake_state_t fake_state;
void antiaim::fake_animations() {
	junkcode::call();
	if (!csgo::local_player || !csgo::local_player->is_alive() || !csgo::cmd)
		return;

	if (fake_state.m_fake_spawntime != csgo::local_player->spawn_time() || fake_state.m_should_update_fake)
	{
		fake_state.init_fake_anim = false;
		fake_state.m_fake_spawntime = csgo::local_player->spawn_time();
		fake_state.m_should_update_fake = false;
	}

	if (!fake_state.init_fake_anim)
	{
		fake_state.m_fake_state = static_cast<anim_state*> (interfaces::mem_alloc->alloc(sizeof(anim_state)));

		if (fake_state.m_fake_state != nullptr)
			csgo::local_player->create_state(fake_state.m_fake_state);

		fake_state.init_fake_anim = true;
	}

	if (csgo::send_packet)
	{
		junkcode::call();
		std::array<animation_layer, 15> networked_layers;
		std::memcpy(&networked_layers, csgo::local_player->anim_overlays(), sizeof(animation_layer) * 15);
		auto backup_poses = csgo::local_player->pose_param();
		auto backup_abs_angles = csgo::local_player->abs_angles();

		csgo::local_player->update_state(fake_state.m_fake_state, csgo::cmd->viewangles);
		fake_state.m_fake_rotation = reinterpret_cast<anim_state*> (fake_state.m_fake_state)->m_flGoalFeetYaw;
		csgo::local_player->set_abs_angles(vec3_t(0, fake_state.m_fake_rotation, 0));
		csgo::local_player->invalidate_bone_cache();
		fake_state.m_got_fake_matrix = csgo::local_player->setup_bones(fake_state.m_fake_matrix, 128, BONE_USED_BY_ANYTHING & ~BONE_USED_BY_ATTACHMENT, interfaces::globals->cur_time);
		const auto org_tmp = csgo::local_player->renderable()->get_render_origin();
		if (fake_state.m_got_fake_matrix)
		{
			for (auto& i : fake_state.m_fake_matrix)
			{
				i[0][3] -= org_tmp.x;
				i[1][3] -= org_tmp.y;
				i[2][3] -= org_tmp.z;
			}
		}
		csgo::local_player->set_abs_angles(vec3_t(0.f, backup_abs_angles.y, 0.f));
		std::memcpy(csgo::local_player->anim_overlays(), &networked_layers, sizeof(animation_layer) * 15);
		csgo::local_player->pose_param() = backup_poses;
	}
}
#define NOMINMAX
#include "../features.hpp"
#include "../../../security/cl_junkcode.hpp"

float lerp_time() {
	junkcode::call();
	int ud_rate = interfaces::console->get_convar("cl_updaterate")->get_int();
	convar* min_ud_rate = interfaces::console->get_convar("sv_minupdaterate");
	convar* max_ud_rate = interfaces::console->get_convar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->get_int();

	float ratio = interfaces::console->get_convar("cl_interp_ratio")->get_float();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::console->get_convar("cl_interp")->get_float();
	convar* c_min_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
	convar* c_max_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->get_float() != 1)
		ratio = std::clamp(ratio, c_min_ratio->get_float(), c_max_ratio->get_float());

	return std::max(lerp, ratio / ud_rate);
}

bool backtracking::is_tick_valid(float simulation_time, float backtrack_time) {
	junkcode::call();
	i_net_channel_info* nci = interfaces::engine->get_net_channel_info();
	if (!nci) return true;
	float correct = 0;
	correct += nci->get_latency(0);
	correct += nci->get_latency(1);
	//correct += lerp_time();

	std::clamp(correct, 0.f, interfaces::console->get_convar("sv_maxunlag")->get_float());

	float delta_time = correct - (interfaces::globals->cur_time - simulation_time);

	float time_limit = std::clamp(backtrack_time, 0.001f, 0.2f);

	if (fabsf(delta_time) > time_limit)
		return false;

	return true;
}
std::vector<tick_info> backtracking::records[65];
int backtracking::closest_player = -1;
int backtracking::closest_tick = -1;
void backtracking::store(c_usercmd* cmd) {
	junkcode::call();
	if (!csgo::local_player || !variables.backtracking.enabled || !variables.backtracking.range)
		return;

	if (!csgo::local_player->is_alive())
		return;

	vec3_t eye_pos = csgo::local_player->get_eye_pos();

	closest_player = -1;
	float max_player_fov = FLT_MAX;
	for (int i = 1; i <= 64; i++) {
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player ||
			player->dormant() ||
			!player->is_player() ||
			!player->is_alive() ||
			player->team() == csgo::local_player->team()) {
			if (records[i].size() > 0)
				records[i].clear();
			continue;
		}

		records[i].insert(records[i].begin(), tick_info(player));

		for (auto tick : records[i])
			if (!is_tick_valid(tick.simulation_time, float(variables.backtracking.range) / 1000.f))
				records[i].pop_back();
	}
}
void backtracking::run(c_usercmd* cmd) {
	junkcode::call();
	if (!csgo::local_player || !variables.backtracking.enabled || !variables.backtracking.range)
		return;

	if (!csgo::local_player->is_alive())
		return;

	vec3_t eye_pos = csgo::local_player->get_eye_pos();
	float max_player_fov = FLT_MAX;
	for (int i = 1; i <= 64; i++) {
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player ||
			player->dormant() ||
			!player->is_player() ||
			!player->is_alive() ||
			player->team() == csgo::local_player->team()) {
			if (records[i].size() > 0)
				records[i].clear();
			continue;
		}

		vec3_t view_angles{};
		math::angle_vectors((cmd->viewangles + (csgo::local_player->aim_punch_angle() * interfaces::console->get_convar("weapon_recoil_scale")->get_float())), view_angles);
		float fov_distance = math::distance_point_to_line(records[i].front().head_pos, eye_pos, view_angles);

		if (max_player_fov > fov_distance) {
			max_player_fov = fov_distance;
			closest_player = i;
		}
	}
	closest_tick = -1;
	float max_tick_fov = FLT_MAX;
	if (closest_player != -1) {
		for (int t = 0; t < records[closest_player].size(); t++) {
			vec3_t view_angles2{};
			math::angle_vectors((cmd->viewangles + (csgo::local_player->aim_punch_angle() * interfaces::console->get_convar("weapon_recoil_scale")->get_float())), view_angles2);
			float fov_distance2 = math::distance_point_to_line(records[closest_player].at(t).head_pos, eye_pos, view_angles2);

			if (max_tick_fov > fov_distance2) {
				max_tick_fov = fov_distance2;
				closest_tick = t;
			}
		}
		if (closest_tick != -1 && records[closest_player].at(closest_tick).simulation_time != -1.f && (cmd->buttons & in_attack))
			cmd->tick_count = time_to_ticks(records[closest_player].at(closest_tick).simulation_time);
	}
}

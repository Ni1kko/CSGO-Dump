#include "aimbot.h"
#include "../../menu/fgui_menu.hpp"
#include "../../menu/fgui_input.hpp"

void aimbot::smooth(vec3_t& viewangles, vec3_t& angles, int amount)
{
	angles = (viewangles + (angles - viewangles).clamped() / amount).clamped();
}


void aimbot::weapon_settings(weapon_t* weapon) {
	/*if (!weapon)
		return;

	if (!csgo::local_player)
		return;

	if (is_pistol(weapon)) {
		fov2 = vars::slider["#pistol_fov"]->get_value();
		smoothness = vars::slider["#pistol_smooth"]->get_value();
		rcs = vars::slider["#pistol_rcs"]->get_value();
	}
	if (is_heavy_pistol(weapon)) {
		fov2 = vars::slider["#heavy_pistol_fov"]->get_value();
		smoothness = vars::slider["#heavy_pistol_smooth"]->get_value();
		rcs = vars::slider["#heavy_pistol_rcs"]->get_value();
	}
	if (is_smg(weapon)) {
		fov2 = vars::slider["#smg_fov"]->get_value();
		smoothness = vars::slider["#smg_smooth"]->get_value();
		rcs = vars::slider["#smg_rcs"]->get_value();
	}
	if (is_rifle(weapon)) {
		fov2 = vars::slider["#rifle_fov"]->get_value();
		smoothness = vars::slider["#rifle_smooth"]->get_value();
		rcs = vars::slider["#rifle_rcs"]->get_value();
	}
	if (is_sniper(weapon)) {
		fov2 = vars::slider["#sniper_fov"]->get_value();
		smoothness = vars::slider["#sniper_smooth"]->get_value();
		rcs = vars::slider["#sniper_rcs"]->get_value();
	}
	if (is_scout(weapon)) {
		fov2 = vars::slider["#scout_fov"]->get_value();
		smoothness = vars::slider["#scout_smooth"]->get_value();
		rcs = vars::slider["#scout_rcs"]->get_value();
	}
	if (is_heavy(weapon)) {
		fov2 = vars::slider["#heavy_fov"]->get_value();
		smoothness = vars::slider["#heavy_smooth"]->get_value();
		rcs = vars::slider["#heavy_rcs"]->get_value();
	}*/

}

bool aimbot::aim(c_usercmd* cmd)
{
	if (!vars::checkbox["#enable_aim"]->get_bool())
		return false;

	if (vars::checkbox["#aim_on_key"] && !fgui_input::get_key_state(vars::keybinder["#aim_key"]->get_key()))
		return false;
	float fov2{};
	float smoothness{};
	float rcs{};
	bool silent = vars::checkbox["#silent_aim"]->get_bool();

	auto weapon = csgo::local_player->active_weapon();

	if (is_pistol(weapon)) {
		fov2 = vars::slider["#pistol_fov"]->get_value();
		smoothness = vars::slider["#pistol_smooth"]->get_value();
		rcs = vars::slider["#pistol_rcs"]->get_value();
	}
	if (is_heavy_pistol(weapon)) {
		fov2 = vars::slider["#heavy_pistol_fov"]->get_value();
		smoothness = vars::slider["#heavy_pistol_smooth"]->get_value();
		rcs = vars::slider["#heavy_pistol_rcs"]->get_value();
	}
	if (is_smg(weapon)) {
		fov2 = vars::slider["#smg_fov"]->get_value();
		smoothness = vars::slider["#smg_smooth"]->get_value();
		rcs = vars::slider["#smg_rcs"]->get_value();
	}
	if (is_rifle(weapon)) {
		fov2 = vars::slider["#rifle_fov"]->get_value();
		smoothness = vars::slider["#rifle_smooth"]->get_value();
		rcs = vars::slider["#rifle_rcs"]->get_value();
	}
	if (is_sniper(weapon)) {
		fov2 = vars::slider["#sniper_fov"]->get_value();
		smoothness = vars::slider["#sniper_smooth"]->get_value();
		rcs = vars::slider["#sniper_rcs"]->get_value();
	}
	if (is_scout(weapon)) {
		fov2 = vars::slider["#scout_fov"]->get_value();
		smoothness = vars::slider["#scout_smooth"]->get_value();
		rcs = vars::slider["#scout_rcs"]->get_value();
	}
	if (is_heavy(weapon)) {
		fov2 = vars::slider["#heavy_fov"]->get_value();
		smoothness = vars::slider["#heavy_smooth"]->get_value();
		rcs = vars::slider["#heavy_rcs"]->get_value();
	}

	auto player = csgo::local_player;

	if (!player || !player->is_alive() || player->is_defusing())
		return false;

	struct data {
		float fov = 180;
		int target = -1;
	} data;

	vec3_t final_angles, viewangles;
	interfaces::engine->get_view_angles(viewangles);

	for (int i = 0; i < interfaces::globals->max_clients; i++)
	{
		auto ent = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!ent || !ent->is_alive() || ent->dormant() || ent->has_gun_game_immunity())
			continue;

		const bool is_enemy = ent->team() != csgo::local_player->team();

		if (!is_enemy)
			continue;

		const vec3_t eye_pos = csgo::local_player->get_eye_pos();
		vec3_t eye_pos_nonconst = csgo::local_player->get_eye_pos();

		for (int x = 0; x < 5; x++)
		{
			const vec3_t hitbox_pos = ent->get_hitbox_position(ent, x);
			vec3_t hitbox_pos_nonconst = ent->get_hitbox_position(ent, x);

			const vec3_t punch = player->aim_punch_angle();
			
			vec3_t angles = math::calculate_angle(eye_pos_nonconst, hitbox_pos_nonconst) - punch * (rcs / 50);
			const vec3_t fov_angles = math::calculate_angle2(eye_pos, hitbox_pos, viewangles);

			if (!player->can_see_player_pos(ent, hitbox_pos))
				continue;

			angles.normalize_aimbot();

			const float fov = fov_angles.Length2D();

			if (data.fov > fov && fov <	fov2)
			{
				data.fov = fov;
				data.target = i;
				final_angles = angles;
			}
		}
	}

	if (data.target == -1)
		return false;

	smooth(viewangles, final_angles, smoothness);

	if (!silent)
		interfaces::engine->set_view_angles(final_angles);

	cmd->viewangles = final_angles;

	return true;
}
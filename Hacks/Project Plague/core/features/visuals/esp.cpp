#include "../features.hpp"

esp_ctx_t esp::ctx;

bbox_t get_bbox(player_t* player) {
	bbox_t bbox = { 0,0,0,0 };
	auto collideable = player->collideable();

	if (!collideable)
		return bbox;

	auto min = collideable->mins();
	auto max = collideable->maxs();

	matrix_t& trans = player->coordinate_frame();

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t points_transformed[8];
	for (int i = 0; i < 8; i++) {
		math::transform_vector(points[i], trans, points_transformed[i]);
	}

	vec3_t screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!math::world_to_screen(points_transformed[i], screen_points[i]))
			return bbox;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	bbox.x = left;
	bbox.y = bottom;
	bbox.w = right - left;
	bbox.h = top - bottom;
	return bbox;
}

void esp::render() {
	interfaces::engine->get_screen_size(ctx.screen_w, ctx.screen_h);
	if (variables.misc.antiaim && variables.misc.antiaim_arrows && csgo::local_player->is_alive() && csgo::aa_running && !interfaces::input->m_fCameraInThirdPerson)
		aa_arrows();
	if (variables.visuals.bomb_timer)
		bomb_timer();
	if (variables.misc.speclist) 
		spectator_list();
	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); ++i) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!player)
			continue;

		if (i <= 64 && player->is_player() && player->is_alive() && player->team() != csgo::local_player->team()) {
			ctx.pl = player;
			if (ctx.pl) {
				vec3_t pl_head = ctx.pl->get_hitbox_position(hitbox_head);
				vec3_t pl_origin = ctx.pl->origin();
				if (math::world_to_screen(ctx.pl->get_hitbox_position(hitbox_head), pl_head) || 
					math::world_to_screen(ctx.pl->origin(), pl_origin)) {
					ctx.bbox = get_bbox(ctx.pl);

					constexpr float frequency = 1.f / 0.5f;
					float step = frequency * interfaces::globals->frame_time;
					ctx.pl->dormant() ? ctx.f_temp_alpha[ctx.pl->index()] -= step : ctx.f_temp_alpha[ctx.pl->index()] += step;
					ctx.f_temp_alpha[ctx.pl->index()] = std::clamp(ctx.f_temp_alpha[ctx.pl->index()], 0.f, 1.f);
					ctx.i_temp_alpha[ctx.pl->index()] = 0xff * ctx.f_temp_alpha[ctx.pl->index()];

					ctx.main_alpha[ctx.pl->index()] = std::clamp(ctx.i_temp_alpha[ctx.pl->index()], 0, 220);
					ctx.outline_alpha[ctx.pl->index()] = std::clamp(ctx.i_temp_alpha[ctx.pl->index()], 0, 180);

					if (ctx.i_temp_alpha[ctx.pl->index()] <= 0)
						continue;

					if (!variables.pl_esp.dead_only || variables.pl_esp.dead_only && !csgo::local_player->is_alive()) {
						if (variables.pl_esp.key_bind == 0 || (GetKeyState(variables.pl_esp.key_bind) && !variables.pl_esp.dead_only)) {
							ctx.weapon_offset = 0;
							if (variables.pl_esp.box) 
								box();
							if (variables.pl_esp.name) 
								name();
							if (variables.pl_esp.ammo) 
								ammo();
							if (variables.pl_esp.health) 
								health();
							if (variables.pl_esp.weapon) 
								weapon();
							if (variables.pl_esp.skeleton) 
								skeleton();
						}
					}
					if (variables.misc.radar) 
						ctx.pl->spotted() = true;
				}
			}
		}
	}
}

void esp::box() {
	render::draw_outline(ctx.bbox.x - 1, ctx.bbox.y - 1, ctx.bbox.w + 2, ctx.bbox.h + 2, color::black(ctx.outline_alpha[ctx.pl->index()]));
	render::draw_outline(ctx.bbox.x + 1, ctx.bbox.y + 1, ctx.bbox.w - 2, ctx.bbox.h - 2, color::black(ctx.outline_alpha[ctx.pl->index()]));
	render::draw_outline(ctx.bbox.x, ctx.bbox.y, ctx.bbox.w, ctx.bbox.h, color::from_float(variables.pl_esp.box_clr, ctx.main_alpha[ctx.pl->index()]));
}

void esp::name() {
	player_info_t info;
	interfaces::engine->get_player_info(ctx.pl->index(), &info);
	std::string name = info.name;
	if (info.fakeplayer) name += " (BOT)";
	render::draw_text_string(ctx.bbox.x + ctx.bbox.w / 2, ctx.bbox.y - 13, render::fonts::esp_font, name, true, color::from_float(variables.pl_esp.name_clr, ctx.main_alpha[ctx.pl->index()]));
}

void esp::weapon() {
	auto weapon = ctx.pl->active_weapon();
	if (weapon) {
		auto weapon_data = weapon->get_weapon_data();
		if (weapon_data) {
			std::string weapon_name = weapon->get_weapon_data()->m_szWeaponName + 7;
			if (variables.pl_esp.ammo)
				weapon_name += " [" + std::to_string(weapon->clip1_count()) + "]";
			std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::toupper);
			render::draw_text_string(ctx.bbox.x + ctx.bbox.w / 2, ctx.bbox.y + ctx.bbox.h + 1 + ctx.weapon_offset, render::fonts::esp_font_small, weapon_name, true, color::from_float(variables.pl_esp.weapon_clr, ctx.main_alpha[ctx.pl->index()]));
		}
	}
}

void esp::health() {
	int health = ctx.pl->health();
	int color_additive = (100 - health) * 2;
	color bar_color = color(50 + color_additive, 200 - color_additive, 50, ctx.main_alpha[ctx.pl->index()]);
	int bar_height = ((ctx.bbox.h - 2) * health) / 100;
	render::draw_filled_rect(ctx.bbox.x - 6, ctx.bbox.y, 4, ctx.bbox.h, color::black(ctx.outline_alpha[ctx.pl->index()]));
	render::draw_filled_rect(ctx.bbox.x - 5, ctx.bbox.y + 1, 2, bar_height, bar_color);
	int text_height = bar_height - 3;
	int hp_offset = (health < 10) ? 12 : 16;
	if (health < 100)
		render::draw_text_string(ctx.bbox.x - hp_offset, ctx.bbox.y + text_height, render::fonts::esp_font_small, std::to_string(health), false, color::white(ctx.main_alpha[ctx.pl->index()]));
}

void esp::ammo() {
	ctx.weapon_offset += 5;
	auto weapon = ctx.pl->active_weapon();
	if (weapon) {
		int ammo = weapon->clip1_count();
		int bar_width = ((ctx.bbox.w - 2) * ammo) / weapon->get_weapon_data()->m_iMaxClip;
		render::draw_filled_rect(ctx.bbox.x, ctx.bbox.y + ctx.bbox.h + 2, ctx.bbox.w, 4, color::black(ctx.outline_alpha[ctx.pl->index()]));
		render::draw_filled_rect(ctx.bbox.x + 1, ctx.bbox.y + ctx.bbox.h + 3, bar_width, 2, color::from_float(variables.pl_esp.ammo_clr, ctx.main_alpha[ctx.pl->index()]));
	}
}

void esp::skeleton() {
	studio_hdr_t* studio_model = interfaces::model_info->get_studio_model(ctx.pl->model());
	if (studio_model)
	{
		for (int j = 0; j < studio_model->bones_count; j++)
		{
			studio_bone_t* bone = studio_model->bone(j);

			if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1))
			{
				vec3_t v_child = ctx.pl->get_bone_position(j);
				vec3_t v_parent = ctx.pl->get_bone_position(bone->parent);

				vec3_t s_parent = vec3_t{}, s_child = vec3_t{};
				if (!math::world_to_screen(v_parent, s_parent) ||
					!math::world_to_screen(v_child, s_child))
					continue;

				render::draw_line(s_parent.x, s_parent.y, s_child.x, s_child.y, color::from_float(variables.pl_esp.skeleton_clr, ctx.main_alpha[ctx.pl->index()]));
			}
		}
	}
}

void esp::aa_arrows() {
	render::draw_text_string(ctx.screen_w / 2 - 40 - 6, ctx.screen_h / 2 - 12, render::fonts::arrow_font, "<", false, !csgo::aa_side ? color::white(255) : color::from_float(variables.misc.desyncclr1, 255));
	render::draw_text_string(ctx.screen_w / 2 + 40 - 6, ctx.screen_h / 2 - 12, render::fonts::arrow_font, ">", false, csgo::aa_side ? color::white(255) : color::from_float(variables.misc.desyncclr1, 255));
	//render::draw_text_string(screen_w / 2, screen_h / 2 - 30, render::fonts::esp_font, std::to_string(math::angle_diff(csgo::real_abs, fake_state.m_fake_rotation)), true, color::white(255));
}

void esp::bomb_timer() {
	int bar_size = ctx.screen_w / 4;
	for (int i = 1; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity ||
			entity->client_class()->class_id != class_ids::cplantedc4)
			continue;

		float c4blow = entity->c4_blow();//the time when the bomb will detonate
		float explode_time_remaining = c4blow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);//subtract current time to get time remaining

		float defuse_countdown = entity->defuse_countdown();//time bomb is expected to defuse. if defuse is cancelled and started again this will be changed to the new value
		float defuse_time_remaining = defuse_countdown - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);//subtract current time to get time remaining

		char time_to_explode[64]; sprintf_s(time_to_explode, "Explode in: %.1f", explode_time_remaining);//Text we gonna display for explosion
		char time_to_defuse[64]; sprintf_s(time_to_defuse, "Defuse in: %.1f", defuse_time_remaining);//Text we gonna display for defuse

		if (explode_time_remaining > 0 && !entity->bomb_defused()) {							
			float fraction = explode_time_remaining / entity->timer_length();//the proportion of time remaining, use fltimerlength cos bomb detonation time can vary by gamemode
			int onscreenwidth = fraction * bar_size;//the width of the bomb timer bar. proportion of time remaining multiplied by width of screen

			float red = 255 - (fraction * 255);//make our bar fade from complete green to complete red
			float green = fraction * 255;

			render::draw_text_string(ctx.screen_w / 2, ctx.screen_h - ctx.screen_h / 4 - 16, render::fonts::esp_font, time_to_explode, true, color::white(220));
			render::draw_filled_rect(ctx.screen_w / 2 - bar_size / 2 - 1, ctx.screen_h - ctx.screen_h / 4 - 1, bar_size + 2, 12, color::black(180));
			render::draw_filled_rect(ctx.screen_w / 2 - bar_size / 2, ctx.screen_h - ctx.screen_h / 4, onscreenwidth, 10, color(red, green, 0, 255));
		}

		auto defuser= reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->bomb_defuser()));

		if (defuser) {
			float fraction = defuse_time_remaining / entity->timer_length();
			int onscreenwidth = fraction * bar_size;

			render::draw_text_string(ctx.screen_w / 2, ctx.screen_h - ctx.screen_h / 4 + 13, render::fonts::esp_font, time_to_defuse, true, color::white(220));
			render::draw_filled_rect(ctx.screen_w / 2 - bar_size / 2, ctx.screen_h - ctx.screen_h / 4, onscreenwidth, 10, color(80, 130, 200, 255));
		}
	}
}

void esp::spectator_list() {
	int kapi = 0;
	for (int i = 1; i <= 64; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (player) {
			auto handle = player->observer_target();
			if (handle != 0xFFFFFFFF)
			{
				player_t* target = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(handle));
				if (target && target == csgo::local_player)
				{
					player_info_t spectator_info;
					interfaces::engine->get_player_info(player->index(), &spectator_info);
					std::string spectator_name = spectator_info.name;
					if (spectator_info.fakeplayer) spectator_name += " (BOT)";

					render::draw_text_string(4, ctx.screen_h / 2 - 80 + 15 * kapi, render::fonts::esp_font, spectator_name, false, color::white(220));
					kapi++;
				}
			}
		}
	}
	render::draw_text_string(4, ctx.screen_h / 2 - 80 - 15, render::fonts::esp_font, "Spectators:", false, color::white(220));
}
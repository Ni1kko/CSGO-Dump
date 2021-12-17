#include "esp.h"
#include "../../menu/fgui_menu.hpp"
#include "../../menu/fgui_input.hpp"


void visuals::run() {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (interfaces::engine->is_taking_screenshot()) // hehe
		return;

	if (!local_player)
		return;

	if (!vars::checkbox["#enable_esp"]->get_bool())
		return;

	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	

	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));


		if (!entity)
			continue;

		const int fade = (int)((6.66666666667f * interfaces::globals->frame_time) * 255);

		auto new_alpha = alpha[i];
		new_alpha += entity->dormant() ? -fade : fade;

		if (new_alpha > (entity->has_gun_game_immunity() ? 130 : 210))
			new_alpha = (entity->has_gun_game_immunity() ? 130 : 210);

		alpha[i] = new_alpha;

		if (entity->team() == local_player->team() && !vars::checkbox["#team_check"]->get_bool())
			continue;


		if (entity == csgo::local_player)
			continue;

		if (entity->health() <= 0)
			continue;


		visuals::player_esp(entity);

		if (vars::checkbox["#radar_esp"]->get_bool())
			entity->spotted() = true;

	}

}



void visuals::force_crosshair() {

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!interfaces::engine->is_connected())
		return;

	if (!local_player)
		return;

	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");

	if (local_player && local_player->health() > 0) {
		weapon_debug_spread_show->set_value(local_player->is_scoped() || !vars::checkbox["#force_crosshair"]->get_bool() ? 0 : 3);
	}
}



void visuals::player_esp(player_t* entity)
{
	if (entity->dormant())
		return;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	box b_box;
	if (!get_playerbox(entity, b_box))
		return;
	

	// name esp
	if (vars::checkbox["#name_esp"]->get_bool())
	{
		std::string print(info.fakeplayer ? std::string("\n ").append(info.name).c_str() : info.name);
		std::transform(print.begin(), print.end(), print.begin(), ::tolower);

		auto colorguy = (vars::colorpicker["#name_color"]->get_color());
		auto colornormal = (vars::colorpicker["#normal_box_color"]->get_color());

		int w = 0, h = 0;

		interfaces::surface->get_text_size(render::fonts::esp_font, (const wchar_t*)print.c_str(), w, h);
		
		if (w >= 120)
			w = 120;

		render::draw_filled_rect(b_box.x + (b_box.w / 2) - (w / 2) - 3, b_box.y - 22, w, 2, color(colornormal.m_red, colornormal.m_green, colornormal.m_blue, colornormal.m_alpha));
		render::draw_filled_rect(b_box.x + (b_box.w / 2) - (w / 2) - 3, b_box.y - 20, w, 14, color(25, 25, 25, 125));
		render::draw_text_string_fgui(b_box.x + (b_box.w / 2) - 3, b_box.y - 20, render::fonts::esp_font, print, true, fgui::color(colorguy.m_red, colorguy.m_green, colorguy.m_blue, colorguy.m_alpha));
		
	}

	if (vars::checkbox["#name_esp"]->get_bool() || vars::checkbox["#box_esp"]->get_bool())
	{
		std::vector<std::pair<std::string, color>> flags;

		if (vars::checkbox["#name_esp"]->get_bool() && entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("HK", color(255, 255, 255, 255)));
		else if (vars::checkbox["#name_esp"]->get_bool() && !entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("K", color(255, 255, 255, 255)));

		if (vars::checkbox["#name_esp"]->get_bool() && entity->money())
			flags.push_back(std::pair<std::string, color>(std::string("[$]").append(std::to_string(entity->money())), color(120, 180, 10, 255)));

		if (vars::checkbox["#name_esp"]->get_bool() && entity->is_scoped())
			flags.push_back(std::pair<std::string, color>(std::string("ZOOM"), color(80, 160, 200, 255)));

		if (vars::checkbox["#name_esp"]->get_bool() && entity->has_c4())
			flags.push_back(std::pair<std::string, color>(std::string("C4"), color(255, 255, 255, 255)));

		if (vars::checkbox["#name_esp"]->get_bool() && entity->is_flashed())
			flags.push_back(std::pair<std::string, color>(std::string("BLIND"), color(255, 255, 255, 255)));

		if (vars::checkbox["#name_esp"]->get_bool() && info.fakeplayer)
			flags.push_back(std::pair<std::string, color>(std::string("BOT"), color(205, 205, 255, 255)));

		auto position = 0;
		for (auto text : flags) {
			render::draw_text_string(b_box.x + b_box.w + 3, b_box.y + position - 2, render::fonts::weapon_font, text.first, false, text.second);
			position += 10;
		}
	}

	if (vars::checkbox["#box_esp"]->get_bool())
	{
		auto colornormal = (vars::colorpicker["#normal_box_color"]->get_color());

		render::draw_outline_fgui(b_box.x - 1, b_box.y - 1, b_box.w + 2, b_box.h + 2, fgui::color(0, 0, 0, 255));
		render::draw_rect(b_box.x, b_box.y, b_box.w, b_box.h, color(colornormal.m_red, colornormal.m_green, colornormal.m_blue, colornormal.m_alpha));
		render::draw_outline_fgui(b_box.x + 1, b_box.y + 1, b_box.w - 2, b_box.h - 2, fgui::color(0, 0, 0, 255));
	}
	const int healtha = math::clamp(entity->health(), 0, 100);
	auto health_colora = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, 255);

	box temp(b_box.x - 5, b_box.y + (b_box.h - b_box.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100.f) / 100.f)), 1, b_box.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
	box temp_bg(b_box.x - 5, b_box.y, 1, b_box.h);


	auto health_color = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, alpha[entity->index()]);

	if (entity->health() > 100)
		health_color = color(0, 255, 0);

	if (vars::checkbox["#health_esp"]->get_bool())
	{
		switch (vars::combobox["#health_esp_style"]->get_value()) {
		case 0:

			render::draw_filled_rect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color(0, 0, 0, 175));
			render::draw_filled_rect(temp.x, temp.y, temp.w, temp.h, color(health_color));
			render::draw_text_string(temp.x - 12, temp.y, render::fonts::weapon_font, std::to_string(healtha), true, color(255, 255, 255, 225));
			break;
		case 1:

			render::draw_filled_rect(b_box.x, b_box.y + b_box.h - 11, b_box.w - 5, 4, color(25, 25, 25, 175));
			render::draw_filled_rect(b_box.x + 1, b_box.y + b_box.h - 10, (b_box.w - 7) * healtha / 100, 2, health_color);
			render::draw_text_string(b_box.x - 3 + (b_box.w - 7) * healtha / 100, b_box.y + b_box.h - 20, render::fonts::hp_font, std::to_string(healtha), true, color(255, 255, 255, 225));
			break;
		}
	}

	if (vars::checkbox["#weapon_esp"]->get_bool())
	{
		auto weapon = entity->active_weapon();

		if (!weapon)
			return;

		std::string names;
		names = clean_item_name(weapon->get_weapon_data()->m_szWeaponName);

		std::transform(names.begin(), names.end(), names.begin(), ::toupper);

		render::draw_text_string(b_box.x + (b_box.w / 2) - 3, b_box.h + b_box.y + 2, render::fonts::weapon_font, names, true, color(255, 255, 255, 255));
	}

	if (vars::checkbox["#skeleton_esp"]->get_bool()) {

		auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());

		if (!p_studio_hdr)
			return;

		auto col = vars::colorpicker["#skeleton_color"]->get_color();

		vec3_t v_parent, v_child, s_parent, s_child;

		for (int i = 0; i < p_studio_hdr->bones_count; i++) {
			studio_bone_t* bone = p_studio_hdr->bone(i);

			if (!bone)
				return;

			if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
				v_child = entity->get_bone_position(i);
				v_parent = entity->get_bone_position(bone->parent);

				if (math::world_to_screen(v_parent, s_parent) && math::world_to_screen(v_child, s_child))
					render::draw_line_fgui(s_parent[0], s_parent[1], s_child[0], s_child[1], fgui::color(col.m_red, col.m_green, col.m_blue, col.m_alpha));
			}
		}
	}
}


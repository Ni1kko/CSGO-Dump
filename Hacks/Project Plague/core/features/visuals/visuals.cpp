#include "../features.hpp"

void visuals::nightmode() {
	static convar* draw_specific_static_prop = interfaces::console->get_convar("r_drawspecificstaticprop");
	if (draw_specific_static_prop->get_int() != 0)
		draw_specific_static_prop->set_value(0);

	for (MaterialHandle_t i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material_handle(); i = interfaces::material_system->next_material(i))
	{
		i_material* material = interfaces::material_system->get_material(i);
		if (!material)
			continue;

		if (strstr(material->get_texture_group_name(), "World") || strstr(material->get_texture_group_name(), "StaticProp"))
		{
			float nightmode_value = (100.f - float(variables.visuals.nightmode)) / 100.f;
			float props_value = std::clamp(nightmode_value + 0.3f, 0.00f, 1.00f);

			if (variables.visuals.nightmode > 0) {
				if (strstr(material->get_texture_group_name(), "StaticProp"))
					material->color_modulate(props_value, props_value, props_value);
				else
					material->color_modulate(nightmode_value, nightmode_value, nightmode_value);
			}
			else
				material->color_modulate(1.0f, 1.0f, 1.0f);
		}
	}
}

void visuals::sniper_crosshair() {
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player) {
		static convar* weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");
		if (variables.visuals.sniper_crosshair && csgo::local_player->is_alive() && !csgo::local_player->is_scoped()) {
			if (weapon_debug_spread_show->get_int() != 69)
				weapon_debug_spread_show->set_value(69);
		}
		else {
			if (weapon_debug_spread_show->get_int() != 0)
				weapon_debug_spread_show->set_value(0);
		}
	}
}


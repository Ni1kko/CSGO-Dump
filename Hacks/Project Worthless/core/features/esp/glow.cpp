#include "glow.h"

void visuals::glow::run() {

	if (!vars::checkbox["#glow"]->get_bool())
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	std::vector<float> colors = get_glow_colors();

	for (int i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow_obj = interfaces::glow_manager->objects[i];
		auto ent = reinterpret_cast<player_t*>(glow_obj.entity);

		if (glow_obj.unused())
			continue;

		if (!ent || ent->dormant() || ent->team() == csgo::local_player->team())
			continue;

		auto ent_class = ent->client_class();

		if (ent_class->class_id == class_ids::ccsplayer) {

			if (!ent->is_alive())
				continue;

			glow_obj.set(colors[0], colors[1], colors[2], 1);
			glow_obj.bloom_amount = colors[3];
		}

	}
}
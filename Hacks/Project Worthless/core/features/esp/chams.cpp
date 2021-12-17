#include "chams.h"

void visuals::chams::create_materials() {
	std::ofstream("csgo\\materials\\chams_regular.vmt") << R"#("VertexLitGeneric"
	{
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "0"
			"$envmap"       ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\chams_glow.vmt") << R"#("VertexLitGeneric"
	{
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.8"
	}
	)#";
}

void visuals::chams::remove_materials() {
	std::remove("csgo\\materials\\chams_regular.vmt");
	std::remove("csgo\\materials\\chams_glow.vmt");
}



void visuals::chams::run() {

	if (!vars::checkbox["#chams_enable"]->get_bool()) 
		return;

	// grab our wish material
	static i_material* material = interfaces::material_system->find_material("chams_regular", TEXTURE_GROUP_MODEL, true, nullptr);
	static i_material* ignorez = interfaces::material_system->find_material("chams_regular", TEXTURE_GROUP_MODEL, true, nullptr);
	static i_material* glow = interfaces::material_system->find_material("chams_glow", TEXTURE_GROUP_MODEL, true, nullptr);

	// grab local
	player_t* local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	// make sure local and material are both valid
	if (local && material) {

		// loop thru entities
		for (int i = 1; i <= interfaces::globals->max_clients; i++) {
			// grab entity
			player_t* entity = (player_t*)interfaces::entity_list->get_client_entity(i);

			// perform sanity checks
			if (i == interfaces::engine->get_local_player())
				continue;

			if (!entity)
				continue;

			if (!entity->is_player())
				continue;

			if (entity->health() <= 0)
				continue;

			if (entity->team() == local->team())
				continue;

			auto col = get_chams_colors();
			auto col_invis = get_invisible_chams_colors();
			auto col_glow = get_glow_chams_colors();

			const float col_fl[3]{
				col[0],
				col[1],
				col[2]
			};

			const float col_invis_fl[3]{
				col_invis[0],
				col_invis[1],
				col_invis[2]
			};

			const float col_glow_fl[3]{
				col_glow[0],
				col_glow[1],
				col_glow[2]
			};

			interfaces::model_render->override_material(ignorez);
			interfaces::render_view->modulate_color(col_invis_fl);
			interfaces::render_view->set_blend(col_invis[3]);
			ignorez->set_material_var_flag(material_var_ignorez, true);

			entity->draw_model(0x1, 255);

			interfaces::model_render->override_material(glow);
			interfaces::render_view->modulate_color(col_glow_fl);
			interfaces::render_view->set_blend(col_glow[3]);
			glow->set_material_var_flag(material_var_ignorez, true);

			entity->draw_model(0x1, 255);

			interfaces::model_render->override_material(material);
			interfaces::render_view->modulate_color(col_fl);
			interfaces::render_view->set_blend(col[3]);
			material->set_material_var_flag(material_var_ignorez, false);

			entity->draw_model(0x1, 255);

			interfaces::model_render->override_material(glow);
			interfaces::render_view->modulate_color(col_glow_fl);
			interfaces::render_view->set_blend(col_glow[3]);

			glow->set_material_var_flag(material_var_ignorez, false);

			entity->draw_model(0x1, 255);

			interfaces::model_render->override_material(nullptr);
		}
	}
}
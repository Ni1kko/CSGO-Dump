#include "../features.hpp"
void chams::init() {
	std::ofstream("csgo/materials/glow_overlay.vmt") << R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";
}
void chams::shutdown() {
	std::remove("csgo\\materials\\glow_overlay.vmt");
}
void modulate_shit_clr(i_material* mat, float* clr) {
	bool b_found = false;
	auto p_var = mat->find_var("$envmaptint", &b_found);
	if (b_found)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)p_var + 44))((uintptr_t)p_var, clr[0], clr[1], clr[2]);
}
void modulate_shit_alpha(i_material* mat, float alpha) {
	bool b_found = false;
	auto p_var = mat->find_var("$alpha", &b_found);
	if (b_found)
		(*(void(__thiscall**)(int, float))(*(DWORD*)p_var + 44))((uintptr_t)p_var, alpha);
}
void set_tint(i_material* mat) {
	bool b_found; 
	auto p_var = mat->find_var("$envmapfresnelminmaxexp", &b_found);
	if (b_found && p_var) {
		p_var->set_vector(0.f, 1.f, 5.f);
	}
}
void chams::run(hooks::draw_model_execute::fn ofunc, IMatRenderContext* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	static i_material* mat_textured = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
	static i_material* mat_flat = interfaces::material_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
	static i_material* mat_eso_glow = interfaces::material_system->find_material("dev/glow_armsrace", TEXTURE_GROUP_MODEL);
	static i_material* mat_glow = interfaces::material_system->find_material("dev/glow_armsrace", TEXTURE_GROUP_MODEL);

	i_material* material = variables.pl_mdl.chams.flat ? mat_flat : mat_textured;

	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player) {
		if (info.model) {
			std::string model_name = interfaces::model_info->get_model_name(info.model);

			if (model_name.find("models/player/") != std::string::npos && variables.pl_mdl.chams.enabled) {
				auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
				if (csgo::local_player && player && player->is_player() && !player->dormant() && player->is_alive()) {
					if (player->team() != csgo::local_player->team()) {
						if (variables.backtracking.enabled &&
							variables.backtracking.range &&
							variables.pl_mdl.chams.backtrack &&
							backtracking::records[info.entity_index].size() > 0 &&
							backtracking::records[info.entity_index].back().bone_matrix_built &&
							backtracking::records[info.entity_index].back().bone_matrix &&
							csgo::local_player->is_alive())
						{
							interfaces::render_view->modulate_color(variables.pl_mdl.chams.clr_backtrack);
							interfaces::render_view->set_blend(float(100 - variables.pl_mdl.chams.transparency_backtrack) / 100.f);
							material->set_material_var_flag(material_var_flags_t::material_var_ignorez, variables.pl_mdl.chams.ignore_z);
							interfaces::model_render->override_material(material);
							ofunc(interfaces::model_render, ctx, state, info, backtracking::records[info.entity_index].back().bone_matrix);
						}
						if (variables.pl_mdl.chams.ignore_z) {
							interfaces::render_view->modulate_color(variables.pl_mdl.chams.clr_z);
							interfaces::render_view->set_blend(float(100 - variables.pl_mdl.chams.transparency) / 100.f);
							material->set_material_var_flag(material_var_flags_t::material_var_ignorez, true);
							interfaces::model_render->override_material(material);
							ofunc(interfaces::model_render, ctx, state, info, matrix);
						}
						interfaces::render_view->modulate_color(variables.pl_mdl.chams.clr);
						interfaces::render_view->set_blend(float(100 - variables.pl_mdl.chams.transparency) / 100.f);
						material->set_material_var_flag(material_var_flags_t::material_var_ignorez, false);
						interfaces::model_render->override_material(material);
						if (variables.pl_mdl.chams.glow_overlay) {
							ofunc(interfaces::model_render, ctx, state, info, matrix);
							interfaces::render_view->modulate_color(variables.pl_mdl.chams.glow_clr);
							modulate_shit_clr(mat_eso_glow, variables.pl_mdl.chams.glow_clr);
							interfaces::render_view->set_blend(float(100 - variables.pl_mdl.chams.transparency_glow) / 100.f);
							modulate_shit_alpha(mat_eso_glow, float(100 - variables.pl_mdl.chams.transparency_glow) / 100.f);
							mat_eso_glow->set_material_var_flag(material_var_flags_t::material_var_ignorez, variables.pl_mdl.chams.ignore_z);
							interfaces::model_render->override_material(mat_eso_glow);
						}
					}
					else if (player == csgo::local_player) {
						if (variables.misc.antiaim && variables.misc.antiaim_arrows)
						{
							if (fake_state.init_fake_anim && fake_state.m_got_fake_matrix)
							{
								for (auto& i : fake_state.m_fake_matrix)
								{
									i[0][3] += info.origin.x;
									i[1][3] += info.origin.y;
									i[2][3] += info.origin.z;
								}

								interfaces::render_view->modulate_color(variables.misc.desyncclr2);
								interfaces::render_view->set_blend(1.f);
								interfaces::model_render->override_material(mat_flat);
								ofunc(interfaces::model_render, ctx, state, info, fake_state.m_fake_matrix);

								interfaces::render_view->modulate_color(variables.misc.desyncclr1);
								modulate_shit_clr(mat_eso_glow, variables.misc.desyncclr1);
								set_tint(mat_eso_glow);
								interfaces::render_view->set_blend(1.f);
								modulate_shit_alpha(mat_eso_glow, 1.f);
								mat_eso_glow->set_material_var_flag(material_var_flags_t::material_var_ignorez, false);
								interfaces::model_render->override_material(mat_eso_glow);
								ofunc(interfaces::model_render, ctx, state, info, fake_state.m_fake_matrix);

								for (auto& i : fake_state.m_fake_matrix)
								{
									i[0][3] -= info.origin.x;
									i[1][3] -= info.origin.y;
									i[2][3] -= info.origin.z;
								}
							}
							static const float white[3] = { 1.f,1.f,1.f };
							interfaces::render_view->modulate_color(white);
							interfaces::render_view->set_blend(1.f);
							interfaces::model_render->override_material(nullptr);
							//ofunc(interfaces::model_render, ctx, state, info, matrix);
						}
					}
				}
			}
		}
	}
	ofunc(interfaces::model_render, ctx, state, info, matrix);
	interfaces::model_render->override_material(nullptr);
}
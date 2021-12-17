#include "../features.hpp"
#include "../../../security/xor.h"

static auto get_wearable_create_fn() -> create_client_class_fn {
	auto client_class = interfaces::client->get_client_classes();

	for (client_class = interfaces::client->get_client_classes();
		client_class; client_class = client_class->next_ptr) {

		if (client_class->class_id == (int)class_ids::ceconwearable) {
			return client_class->create_fn;
		}
	}
}

static auto make_glove(int entry, int serial) -> attributable_item_t* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = utilities::pattern_scan(GetModuleHandleA(xorstr_("client.dll")), xorstr_("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool glovechanger::apply_glove_model(attributable_item_t* glove, const char* model)  {
	player_info_t info;
	interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &info);
	glove->acc_id() = info.xuidlow;
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;

	return true;
}

bool glovechanger::apply_glove_skin(attributable_item_t* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear)  {
	glove->item_definition_index() = item_definition_index;
	glove->fallback_paint_kit() = paint_kit;
	glove->set_model_index(model_index);
	glove->entity_quality() = entity_quality;
	glove->fallback_wear() = fallback_wear;

	return true;
}
int get_glove_skin(int idx, int skin) {
	switch (idx) {
	case GLOVE_STUDDED_BLOODHOUND:
		switch (skin) {
		case 0:
			return 10007;
			break;
		case 1:
			return 10039;
			break;
		case 2:
			return 10006;
			break;
		case 3:
			return 10008;
			break;
		}
		break;
	case GLOVE_SPORTY:
		switch (skin) {
		case 0:
			return 10048;
			break;
		case 1:
			return 10045;
			break;
		case 2:
			return 10047;
			break;
		case 3:
			return 10046;
			break;
		case 4:
			return 10037;
			break;
		case 5:
			return 10018;
			break;
		case 6:
			return 10038;
			break;
		case 7:
			return 10019;
			break;
		}
		break;
	case GLOVE_SLICK:
		switch (skin) {
		case 0:
			return 10041;
			break;
		case 1:
			return 10042;
			break;
		case 2:
			return 10043;
			break;
		case 3:
			return 10044;
			break;
		case 4:
			return 10016;
			break;
		case 5:
			return 10040;
			break;
		case 6:
			return 10013;
			break;
		case 7:
			return 10015;
			break;
		}
		break;
	case GLOVE_LEATHER_WRAP:
		switch (skin) {
		case 0:
			return 10053;
			break;
		case 1:
			return 10054;
			break;
		case 2:
			return 10056;
			break;
		case 3:
			return 10055;
			break;
		case 4:
			return 10021;
			break;
		case 5:
			return 10036;
			break;
		case 6:
			return 10009;
			break;
		case 7:
			return 10010;
			break;
		}
		break;
	case GLOVE_MOTORCYCLE:
		switch (skin) {
		case 0:
			return 10049;
			break;
		case 1:
			return 10050;
			break;
		case 2:
			return 10052;
			break;
		case 3:
			return 10051;
			break;
		case 4:
			return 10026;
			break;
		case 5:
			return 10027;
			break;
		case 6:
			return 10028;
			break;
		case 7:
			return 10024;
			break;
		}
		break;
	case GLOVE_SPECIALIST:
		switch (skin) {
		case 0:
			return 10063;
			break;
		case 1:
			return 10061;
			break;
		case 2:
			return 10064;
			break;
		case 3:
			return 10062;
			break;
		case 4:
			return 10033;
			break;
		case 5:
			return 10034;
			break;
		case 6:
			return 10035;
			break;
		case 7:
			return 10030;
			break;
		}
		break;
	case GLOVE_HYDRA:
		switch (skin) {
		case 0:
			return 10057;
			break;
		case 1:
			return 10058;
			break;
		case 2:
			return 10060;
			break;
		case 3:
			return 10059;
			break;
		}
		break;
	}
	return 0;
}
void glovechanger::run()  {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_ent = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_ent)
		return;

	if (variables.skins.glove_model == 0)
		return;

	// gloveschanger
	if (variables.skins.glove_model > 0) {

		static const auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
		static const auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
		static const auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
		static const auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
		static const auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
		static const auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
		static const auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";

		static const auto index_blood = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
		static const auto index_sport = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
		static const auto index_slick = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
		static const auto index_leath = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
		static const auto index_moto = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
		static const auto index_speci = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
		static const auto index_hydra = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

		//credit to namazso for nskinz

		uintptr_t* const wearables = local_ent->get_wearables();
		if (!wearables)
			return;

		static uintptr_t glove_handle = uintptr_t(0);

		auto glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(wearables[0]));

		if (!glove) // There is no glove
		{
			const auto our_glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(glove_handle));

			if (our_glove) // Try to get our last created glove
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}
		if (!local_ent || // We are dead but we have a glove, destroy it
			!local_ent->is_alive() ||
			!interfaces::engine->is_connected() ||
			!interfaces::engine->is_in_game()
			) {
			if (glove) {
				glove->net_set_destroyed_on_recreate_entities();
				glove->net_release();
			}
			return;
		}
		if (!glove) // We don't have a glove, but we should
		{
			const auto entry = interfaces::entity_list->get_highest_index() + 1;
			const auto serial = rand() % 0x1000;
			glove = make_glove(entry, serial);   // He he
			wearables[0] = entry | serial << 16;
			glove_handle = wearables[0]; // Let's store it in case we somehow lose it.
		}
		if (glove)
		{
			//glove conditions
			static const float g_wear = 0.0000001f;
			

			//apply glove model
			switch (variables.skins.glove_model) {
			case 0:
				break;
			case 1:
				apply_glove_model(glove, model_blood);
				break;
			case 2:
				apply_glove_model(glove, model_sport);
				break;
			case 3:
				apply_glove_model(glove, model_slick);
				break;
			case 4:
				apply_glove_model(glove, model_leath);
				break;
			case 5:
				apply_glove_model(glove, model_moto);
				break;
			case 6:
				apply_glove_model(glove, model_speci);
				break;
			case 7:
				apply_glove_model(glove, model_hydra);
				break;
			}

			//apply glove skins

			glove->item_id_high() = 1;
			glove->fallback_wear() = 0.0000001f;
			glove->fallback_seed() = 0;
			switch (variables.skins.glove_model) {
			case 0:
				break;
			case 1:
				apply_glove_skin(glove, GLOVE_STUDDED_BLOODHOUND, get_glove_skin(GLOVE_STUDDED_BLOODHOUND, variables.skins.bloodhound_skin), index_blood, 3, g_wear);
				break;
			case 2:
				apply_glove_skin(glove, GLOVE_SPORTY, get_glove_skin(GLOVE_SPORTY, variables.skins.sport_skin), index_sport, 3, g_wear);
				break;
			case 3:
				apply_glove_skin(glove, GLOVE_SLICK, get_glove_skin(GLOVE_SLICK, variables.skins.driver_skin), index_slick, 3, g_wear);
				break;
			case 4:
				apply_glove_skin(glove, GLOVE_LEATHER_WRAP, get_glove_skin(GLOVE_LEATHER_WRAP, variables.skins.handwrap_skin), index_leath, 3, g_wear);
				break;
			case 5:
				apply_glove_skin(glove, GLOVE_MOTORCYCLE, get_glove_skin(GLOVE_MOTORCYCLE, variables.skins.moto_skin), index_moto, 3, g_wear);
				break;
			case 6:
				apply_glove_skin(glove, GLOVE_SPECIALIST, get_glove_skin(GLOVE_SPECIALIST, variables.skins.specialist_skin), index_speci, 3, g_wear);
				break;
			case 7:
				apply_glove_skin(glove, GLOVE_HYDRA, get_glove_skin(GLOVE_HYDRA, variables.skins.hydra_skin), index_hydra, 3, g_wear);
				break;
			}

			glove->net_pre_data_update(DATA_UPDATE_CREATED);
		}
	}
}

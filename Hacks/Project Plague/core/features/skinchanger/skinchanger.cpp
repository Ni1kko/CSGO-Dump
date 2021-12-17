#include "../features.hpp"

bool apply_knife_model(attributable_item_t* weapon, const char* model, player_t* local) noexcept {
	auto viewmodel = reinterpret_cast<base_view_model*>(interfaces::entity_list->get_client_entity_handle(local->view_model()));
	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->m_hweapon();
	if (!h_view_model_weapon)
		return false;

	auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(h_view_model_weapon));
	if (view_model_weapon != weapon)
		return false;

	viewmodel->model_index() = interfaces::model_info->get_model_index(model);

	return true;
}

bool apply_knife_skin(attributable_item_t* weapon, int item_definition_index, int model_index) noexcept {
	weapon->model_index() = model_index;
	weapon->item_definition_index() = item_definition_index;

	return true;
}

void skinchanger::run() {
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;
	auto local_ent = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_ent || !local_ent->is_alive())
		return;

	static const auto model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	static const auto model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	static const auto model_karambit = "models/weapons/v_knife_karam.mdl";
	static const auto model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	static const auto model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	static const auto model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	static const auto model_flip = "models/weapons/v_knife_flip.mdl";
	static const auto model_gut = "models/weapons/v_knife_gut.mdl";
	static const auto model_huntsman = "models/weapons/v_knife_tactical.mdl";
	static const auto model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	static const auto model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	static const auto model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	static const auto model_talon = "models/weapons/v_knife_widowmaker.mdl";
	static const auto model_ursus = "models/weapons/v_knife_ursus.mdl";
	static const auto model_nomad = "models/weapons/v_knife_outdoor.mdl";
	static const auto model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	static const auto model_survival = "models/weapons/v_knife_canis.mdl";
	static const auto model_paracord = "models/weapons/v_knife_cord.mdl";
	static const auto model_css = "models/weapons/v_knife_css.mdl";

	static const auto index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	static const auto index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	static const auto index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	static const auto index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	static const auto index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	static const auto index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	static const auto index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	static const auto index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	static const auto index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	static const auto index_shadow_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	static const auto index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	static const auto index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	static const auto index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	static const auto index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	static const auto index_nomad = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	static const auto index_skeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	static const auto index_survival = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	static const auto index_paracord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	static const auto index_css = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");

	auto active_weapon = local_ent->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_ent->get_weapons();
	for (size_t i = 0; my_weapons[i] != 0xFFFFFFFF; i++) {
		auto weapon_atr = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
		auto weapon = reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));

		if (!weapon_atr || !weapon || weapon->is_c4() || weapon->is_nade() || weapon->is_taser())
			return;

		if (weapon->is_knife()) {
			switch (variables.skins.knife_model) {
			case 0:
				break;
			case 1:
				apply_knife_model(weapon_atr, model_bayonet, local_ent);
				break;
			case 2:
				apply_knife_model(weapon_atr, model_flip, local_ent);
				break;
			case 3:
				apply_knife_model(weapon_atr, model_gut, local_ent);
				break;
			case 4:
				apply_knife_model(weapon_atr, model_karambit, local_ent);
				break;
			case 5:
				apply_knife_model(weapon_atr, model_m9, local_ent);
				break;
			case 6:
				apply_knife_model(weapon_atr, model_huntsman, local_ent);
				break;
			case 7:
				apply_knife_model(weapon_atr, model_falchion, local_ent);
				break;
			case 8:
				apply_knife_model(weapon_atr, model_bowie, local_ent);
				break;
			case 9:
				apply_knife_model(weapon_atr, model_butterfly, local_ent);
				break;
			case 10:
				apply_knife_model(weapon_atr, model_shadow_daggers, local_ent);
				break;
			case 11:
				apply_knife_model(weapon_atr, model_ursus, local_ent);
				break;
			case 12:
				apply_knife_model(weapon_atr, model_navaja, local_ent);
				break;
			case 13:
				apply_knife_model(weapon_atr, model_stiletto, local_ent);
				break;
			case 14:
				apply_knife_model(weapon_atr, model_talon, local_ent);
				break;
			case 15:
				apply_knife_model(weapon_atr, model_nomad, local_ent);
				break;
			case 16:
				apply_knife_model(weapon_atr, model_skeleton, local_ent);
				break;
			case 17:
				apply_knife_model(weapon_atr, model_survival, local_ent);
				break;
			case 18:
				apply_knife_model(weapon_atr, model_paracord, local_ent);
				break;
			case 19:
				apply_knife_model(weapon_atr, model_css, local_ent);
				break;
			}

			switch (variables.skins.knife_model) {
			case 0:
				break;
			case 1:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_BAYONET, index_bayonet);
				break;
			case 2:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_FLIP, index_flip);
				break;
			case 3:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_GUT, index_gut);
				break;
			case 4:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_KARAMBIT, index_karambit);
				break;
			case 5:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_M9_BAYONET, index_m9);
				break;
			case 6:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_TACTICAL, index_huntsman);
				break;
			case 7:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_FALCHION, index_falchion);
				break;
			case 8:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_SURVIVAL_BOWIE, index_bowie);
				break;
			case 9:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_BUTTERFLY, index_butterfly);
				break;
			case 10:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_PUSH, index_shadow_daggers);
				break;
			case 11:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_URSUS, index_ursus);
				break;
			case 12:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_GYPSY_JACKKNIFE, index_navaja);
				break;
			case 13:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_STILETTO, index_stiletto);
				break;
			case 14:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_WIDOWMAKER, index_talon);
				break;
			case 15:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_OUTDOOR, index_nomad);
				break;
			case 16:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_SKELETON, index_skeleton);
				break;
			case 17:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_CANIS, index_survival);
				break;
			case 18:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_CORD, index_paracord);
				break;
			case 19:
				apply_knife_skin(weapon_atr, WEAPON_KNIFE_CSS, index_css);
				break;
			}
		}
		weapon_atr->item_id_high() = 1;
		weapon_atr->fallback_wear() = 0.0000001f;
		weapon_atr->fallback_seed() = 420;
		switch (weapon->item_definition_index()) {
		case WEAPON_KNIFE_BAYONET:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 558; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 568; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 573; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 563; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 580; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 578; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 19:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 20:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 21:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 22:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 23:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 24:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_FLIP:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 559; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 568; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 574; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 564; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 581; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 579; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 19:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 20:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 21:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 22:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 23:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 24:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_GUT:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 560; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 568; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 575; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 565; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 580; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 578; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 19:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 20:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 21:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 22:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 23:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 24:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_KARAMBIT:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 561; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 568; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 576; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 566; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 580; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 578; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 19:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 20:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 21:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 22:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 23:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 24:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_M9_BAYONET:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 562; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 568; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 577; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 567; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 580; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 578; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 19:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 20:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 21:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 22:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 23:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 24:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_TACTICAL:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_FALCHION:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_BUTTERFLY:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_PUSH:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_URSUS:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_STILETTO:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_WIDOWMAKER:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_OUTDOOR:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_SKELETON:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_CANIS:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_CORD:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		case WEAPON_KNIFE_CSS:
			switch (variables.skins.knife_skin)
			{
			case 0:
				weapon_atr->fallback_paint_kit() = 0; break;
			case 1:
				weapon_atr->fallback_paint_kit() = 856; break;
			case 2:
				weapon_atr->fallback_paint_kit() = 409; break;
			case 3:
				weapon_atr->fallback_paint_kit() = 415; break;
			case 4:
				weapon_atr->fallback_paint_kit() = 98; break;
			case 5:
				weapon_atr->fallback_paint_kit() = 857; break;
			case 6:
				weapon_atr->fallback_paint_kit() = 203; break;
			case 7:
				weapon_atr->fallback_paint_kit() = 38; break;
			case 8:
				weapon_atr->fallback_paint_kit() = 59; break;
			case 9:
				weapon_atr->fallback_paint_kit() = 44; break;
			case 10:
				weapon_atr->fallback_paint_kit() = 12; break;
			case 11:
				weapon_atr->fallback_paint_kit() = 42; break;
			case 12:
				weapon_atr->fallback_paint_kit() = 40; break;
			case 13:
				weapon_atr->fallback_paint_kit() = 72; break;
			case 14:
				weapon_atr->fallback_paint_kit() = 5; break;
			case 15:
				weapon_atr->fallback_paint_kit() = 43; break;
			case 16:
				weapon_atr->fallback_paint_kit() = 175; break;
			case 17:
				weapon_atr->fallback_paint_kit() = 77; break;
			case 18:
				weapon_atr->fallback_paint_kit() = 143; break;
			}
			break;
		}

		switch (weapon->item_definition_index()) {
		case WEAPON_USP_SILENCER:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_usp;//() = wear;
			break;
		case WEAPON_HKP2000:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_p2000;//() = wear;
			break;
		case WEAPON_GLOCK:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_glock;//() = wear;
			break;
		case WEAPON_P250:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_p250;//() = wear;
			break;
		case WEAPON_FIVESEVEN:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_fiveseven;//() = wear;
			break;
		case WEAPON_TEC9:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_tec;//() = wear;
			break;
		case WEAPON_CZ75A:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_cz;//() = wear;
			break;
		case WEAPON_ELITE:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_duals;//() = wear;
			break;
		case WEAPON_DEAGLE:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_deagle;//() = wear;
			break;
		case WEAPON_REVOLVER:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_revolver;//() = wear;
			break;
		case WEAPON_FAMAS:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_famas;//() = wear;
			break;
		case WEAPON_GALILAR:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_galil;//() = wear;
			break;
		case WEAPON_M4A1:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_m4a4;//() = wear;
			break;
		case WEAPON_M4A1_SILENCER:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_m4a1;//() = wear;
			break;
		case WEAPON_AK47:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_ak47;//() = wear;
			break;
		case WEAPON_SG556:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_sg553;//() = wear;
			break;
		case WEAPON_AUG:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_aug;//() = wear;
			break;
		case WEAPON_SSG08:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_ssg08;//() = wear;
			break;
		case WEAPON_AWP:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_awp;//() = wear;
			break;
		case WEAPON_SCAR20:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_scar;//() = wear;
			break;
		case WEAPON_G3SG1:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_g3sg1;//() = wear;
			break;
		case WEAPON_SAWEDOFF:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_sawoff;//() = wear;
			break;
		case WEAPON_M249:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_m249;//() = wear;
			break;
		case WEAPON_NEGEV:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_negev;//() = wear;
			break;
		case WEAPON_MAG7:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_mag7;//() = wear;
			break;
		case WEAPON_XM1014:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_xm1014;//() = wear;
			break;
		case WEAPON_NOVA:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_nova;//() = wear;
			break;
		case WEAPON_BIZON:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_bizon;//() = wear;
			break;
		case WEAPON_MP5_SD:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_mp5sd;//() = wear;
			break;
		case WEAPON_MP7:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_mp7;//() = wear;
			break;
		case WEAPON_MP9:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_mp9;//() = wear;
			break;
		case WEAPON_MAC10:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_mac10;//() = wear;
			break;
		case WEAPON_P90:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_p90;//() = wear;
			break;
		case WEAPON_UMP45:
			weapon_atr->fallback_paint_kit() = variables.skins.paint_kit_index_ump45;//() = wear;
			break;
		}
	}	
}

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

typedef void(*RecvVarProxyFn)(const c_recv_proxy_data* pData, void* pStruct, void* pOut);
RecvVarProxyFn fnSequenceProxyFn = nullptr;
RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(c_recv_proxy_data* pData, void* pStruct, void* pOut)
{
	static int default_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	static int default_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	static int iBayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	static int iButterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	static int iFlip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	static int iGut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	static int iKarambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	static int iM9Bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	static int iHuntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	static int iFalchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	static int iDagger = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	static int iBowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	static int iUrsus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	static int iNavaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	static int iStiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	static int iTalon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	static int iNomad = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	static int iSkeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	static int iSurvival = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	static int iParacord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	static int iClassic = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");

	if (csgo::local_player)
	{
		if (csgo::local_player->is_alive() && (pData->value.m_int == default_ct
			|| pData->value.m_int == default_t
			|| pData->value.m_int == iGut
			|| pData->value.m_int == iKarambit
			|| pData->value.m_int == iM9Bayonet
			|| pData->value.m_int == iHuntsman
			|| pData->value.m_int == iFalchion
			|| pData->value.m_int == iDagger
			|| pData->value.m_int == iBowie
			|| pData->value.m_int == iButterfly
			|| pData->value.m_int == iFlip
			|| pData->value.m_int == iBayonet
			|| pData->value.m_int == iUrsus
			|| pData->value.m_int == iNavaja
			|| pData->value.m_int == iStiletto
			|| pData->value.m_int == iTalon
			|| pData->value.m_int == iNomad
			|| pData->value.m_int == iSkeleton
			|| pData->value.m_int == iSurvival
			|| pData->value.m_int == iParacord
			|| pData->value.m_int == iClassic))
		{
			if (variables.skins.knife_model == 1)
				pData->value.m_int = iBayonet;
			else if (variables.skins.knife_model == 2)
				pData->value.m_int = iFlip;
			else if (variables.skins.knife_model == 3)
				pData->value.m_int = iGut;
			else if (variables.skins.knife_model == 4)
				pData->value.m_int = iKarambit;
			else if (variables.skins.knife_model == 5)
				pData->value.m_int = iM9Bayonet;
			else if (variables.skins.knife_model == 6)
				pData->value.m_int = iHuntsman;
			else if (variables.skins.knife_model == 7)
				pData->value.m_int = iFalchion;
			else if (variables.skins.knife_model == 8)
				pData->value.m_int = iBowie;
			else if (variables.skins.knife_model == 9)
				pData->value.m_int = iButterfly;
			else if (variables.skins.knife_model == 10)
				pData->value.m_int = iDagger;
			else if (variables.skins.knife_model == 11)
				pData->value.m_int = iUrsus;
			else if (variables.skins.knife_model == 12)
				pData->value.m_int = iNavaja;
			else if (variables.skins.knife_model == 13)
				pData->value.m_int = iStiletto;
			else if (variables.skins.knife_model == 14)
				pData->value.m_int = iTalon;
			else if (variables.skins.knife_model == 15)
				pData->value.m_int = iNomad;
			else if (variables.skins.knife_model == 16)
				pData->value.m_int = iSkeleton;
			else if (variables.skins.knife_model == 17)
				pData->value.m_int = iSurvival;
			else if (variables.skins.knife_model == 18)
				pData->value.m_int = iParacord;
			else if (variables.skins.knife_model == 19)
				pData->value.m_int = iClassic;


		}
	}


	oRecvnModelIndex(pData, pStruct, pOut);
}


void SetViewModelSequence2(const c_recv_proxy_data* pDataConst, void* pStruct, void* pOut)
{
	c_recv_proxy_data* pData = const_cast<c_recv_proxy_data*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	base_view_model* pViewModel = (base_view_model*)pStruct;

	if (pViewModel) {
		auto pOwner = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(pViewModel->m_howner()));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->index() == interfaces::engine->get_local_player()) {
			// Get the filename of the current view model.
			const model_t* pModel = interfaces::model_info->get_model(pViewModel->model_index());
			const char* szModel = interfaces::model_info->get_model_name(pModel);

			// Store the current sequence.
			int m_nSequence = pData->value.m_int;

			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_ursus.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_stiletto.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_widowmaker.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_css.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_cord.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_canis.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_outdoor.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_skeleton.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
				}
			}
			// Set the fixed sequence.
			pData->value.m_int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	fnSequenceProxyFn(pData, pStruct, pOut);

}

void skinchanger::animfix_hook()
{
	for (c_client_class* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->proxy_fn);

				// Replace the proxy function with our sequence changer.
				pProp->proxy_fn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}

			break;
		}
	}
}

void skinchanger::animfix_unhook()
{
	for (c_client_class* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;

				// Replace the proxy function with our sequence changer.
				pProp->proxy_fn = fnSequenceProxyFn;

				break;
			}

			break;
		}
	}
}
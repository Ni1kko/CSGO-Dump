#include "skinchanger.h"

//static auto erase_override_if_exists_by_index(const int definition_index) -> void {
//if (k_weapon_info.count(definition_index)) {
//	auto& icon_override_map = skins.m_icon_overrides;
//		const auto& original_item = k_weapon_info.at(definition_index);
//		if (original_item.icon && icon_override_map.count(original_item.icon))
//			icon_override_map.erase(icon_override_map.at(original_item.icon)); // Remove the leftover override
//	}
//}
//static auto apply_config_on_attributable_item(CBaseAttributableItem* item, const item_setting* config,
//	const unsigned xuid_low) -> void {
//	if (!config->enabled) {
//		return;
//	}
//	*item->GetItemIDHigh() = -1;
//	*item->GetAccountID() = xuid_low;
//	//if( config->custom_name[0] )
//	//	strcpy_s( item->m_Item( ).m_iCustomName( ), config->custom_name );
//	//if (config->paint_kit_index)
//	//	*item->GetFallbackPaintKit() = config->paint_kit_index;
//	//if (config->seed)
//	//	*item->GetFallbackSeed() = config->seed;
//	//if (config->stat_trak) {
//	//	item->GetFallbackStatTrak() = config->stat_trak;
//		*item->GetEntityQuality() = 9;
//	//}
//	//else {
//		*item->GetEntityQuality() = is_knife(config->definition_index) ? 3 : 0;
//	//}
//	//*item->GetFallbackWear() = config->wear;
//	auto& definition_index = *item->GetItemDefinitionIndex();
//	auto& icon_override_map = skins.m_icon_overrides;
//	if (config->definition_override_index && config->definition_override_index != definition_index && k_weapon_info.count(config->definition_override_index)) {
//		const auto old_definition_index = definition_index;
//		definition_index = config->definition_override_index;
//		const auto& replacement_item = k_weapon_info.at(config->definition_override_index);
//		*item->m_nModelIndex() = Interfaces::ModelInfo->GetModelIndex(replacement_item.model);
//		item->SetModelIndex(Interfaces::ModelInfo->GetModelIndex(replacement_item.model));
//		//item->GetClientNetworkable()->PreDataUpdate(0);
//		if (old_definition_index && k_weapon_info.count(old_definition_index)) {
//			const auto& original_item = k_weapon_info.at(old_definition_index);
//			if (original_item.icon && replacement_item.icon) {
//				icon_override_map[original_item.icon] = replacement_item.icon;
//				icon_override_map["knife"] = replacement_item.icon;
//			}
//		}
//	}
//	else {
//		erase_override_if_exists_by_index(definition_index);
//	}
//}
//static auto get_wearable_create_fn() -> CreateClientClassFn {
//	auto clazz = Interfaces::Client->GetAllClasses();
//	// Please, if you gonna paste it into a cheat use classids here. I use names because they
//	// won't change in the foreseeable future and i dont need high speed, but chances are
//	// you already have classids, so use them instead, they are faster.
//	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
//		clazz = clazz->m_pNext;
//	return (CreateClientClassFn)clazz->m_pCreateFn;
//}
//void Skins::OnFrameStageNotify(bool frame_end) {
//	const auto local = G::LocalPlayer;
//	if (!local)
//		return;
//
//	player_info_t player_info;
//	if (!G::LocalPlayer->GetPlayerInfo(&player_info))
//		return;
//
//	if (frame_end) {
//		const auto wearables = local->GetWearables();
//		const auto glove_config = &skins.m_items[GLOVE_T_SIDE];
//
//		static auto glove_handle = 0;
//		auto glove = reinterpret_cast<CBaseAttributableItem*>(Interfaces::EntityList->GetClientEntityFromHandle((ULONG)wearables[0]));
//		if (!glove) {
//			const auto our_glove = reinterpret_cast<CBaseAttributableItem*>(Interfaces::EntityList->GetClientEntityFromHandle((ULONG)glove_handle));
//			if (our_glove) // Our glove still exists
//			{
//				wearables[0] = glove_handle;
//				glove = our_glove;
//			}
//		}
//
//		if (!local->IsAlive()) {
//			if (glove) {
//				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
//				glove->GetClientNetworkable()->Release();
//			}
//			return;
//		}
//
//		if (glove_config && glove_config->definition_override_index) {
//			if (!glove) {
//				static auto create_wearable_fn = get_wearable_create_fn();
//				const auto entry = Interfaces::EntityList->GetHighestEntityIndex() + 1;
//				const auto serial = rand() % 0x1000;
//
//				//glove = static_cast<CBaseAttributableItem*>(create_wearable_fn(entry, serial));
//				create_wearable_fn(entry, serial);
//				glove = reinterpret_cast<CBaseAttributableItem*>(Interfaces::EntityList->GetClientEntity(entry));
//
//				assert(glove);
//				{
//					static auto set_abs_origin_addr = Cheat::Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1");
//					const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
//					static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
//					set_abs_origin_fn(glove, new_pos);
//				}
//
//				const auto wearable_handle = reinterpret_cast<CBaseHandle*>(&wearables[0]);
//				*wearable_handle = entry | serial << 16;
//				glove_handle = wearables[0];
//			}
//
//			// Thanks, Kiparis
//			glove->SetModelIndex(-1);
//			apply_config_on_attributable_item(glove, glove_config, player_info.xuidlow);
//		}
//	}
//	else {
//		/*auto viewmodel_i = *(PDWORD)((DWORD)G::LocalPlayer + NetVarManager->getOffs("CBasePlayer", "m_hViewModel[0]"));
//		auto viewmodel = (CBaseViewModel*)Interfaces::EntityList->GetClientEntityFromHandle((ULONG)viewmodel_i);
//		if (viewmodel)
//		{
//			int index = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
//			printf("t: %u %u %u %u %u %u\n", viewmodel_i, viewmodel, viewmodel->GetWeapon(), local->GetActiveWeapon(), local->GetActiveWeaponIndex(), local->GetActiveWeaponIndex2());
//			if (viewmodel->GetWeapon() == local->GetActiveWeaponIndex2() && *viewmodel->m_nModelIndex() != index)
//				viewmodel->SetModelIndex(index);
//		}*/
//
//		//auto weapons = local->GetWeapons();
//		//  for (int i = 0; weapons[i] != INVALID_EHANDLE_INDEX; ++i) {
//		//	CBaseAttributableItem* weapon = (CBaseAttributableItem*)Interfaces::EntityList->GetClientEntityFromHandle((ULONG)weapons[i]);
//		//	printf("t: %d %u %u\n", i, weapons[i], weapon);
//		for (auto _weapon : Features::Visuals->weaponData)
//		{
//			auto weapon = (CBaseAttributableItem*)_weapon;
//			if (!weapon)
//				continue;
//			auto& definition_index = *weapon->GetItemDefinitionIndex();
//			int m_item_key = is_knife(definition_index) ? WEAPON_KNIFE : definition_index;
//			if (skins.m_items.count(m_item_key) == 0)
//				continue;
//			const auto active_conf = &skins.m_items[m_item_key];
//			apply_config_on_attributable_item(weapon, active_conf, player_info.xuidlow);
//		}
//
//		const auto view_model_handle = local->m_hViewModel();
//		if (!view_model_handle)
//			return;
//
//		const auto view_model = (CBaseViewModel*)Interfaces::EntityList->GetClientEntityFromHandle((ULONG)view_model_handle);
//		if (!view_model)
//			return;
//
//		const auto view_model_weapon_handle = view_model->GetWeapon();
//		if (!view_model_weapon_handle)
//			return;
//
//		const auto view_model_weapon = (CBaseWeapon*)Interfaces::EntityList->GetClientEntityFromHandle((ULONG)view_model_weapon_handle);
//		if (!view_model_weapon)
//			return;
//
//		if (k_weapon_info.count(view_model_weapon->GetItemDefenitionIndex())) {
//			const auto override_model = k_weapon_info.at(view_model_weapon->GetItemDefenitionIndex()).model;
//			auto override_model_index = Interfaces::ModelInfo->GetModelIndex(override_model);
//			*view_model->m_nModelIndex() = override_model_index;
//			auto world_model_handle = view_model_weapon->m_hWeaponWorldModel();
//			if (!world_model_handle)
//				return;
//			const auto world_model = (CBaseWeaponWorldModel*)Interfaces::EntityList->GetClientEntityFromHandle((ULONG)world_model_handle);
//			if (!world_model)
//				return;
//			*world_model->m_nModelIndex() = override_model_index + 1;
//		}
//	}
//}
//
//s_skins skins;
//#pragma once
//#include "../../Configuration/Config.h"
//#include "../../Interfaces/Interfaces.h"
//#include "../../SDK/player_info_t.h"
//#include "../../SDK/CBaseEntity.h"
//#include "../../Features/Skinchanger/items.h"
//#include "../../Security/fnv.hpp"
//
//using CreateClientClassFn = IClientNetworkable * (*)(int, int);
//
//static auto erase_override_if_exists_by_index(const int definition_index) -> void
//{
//	// We have info about the item not needed to be overridden
//	if (const auto original_item = game_data::get_weapon_info(definition_index))
//	{
//		auto& icon_override_map = g_config.get_icon_override_map();
//
//		if (!original_item->icon)
//			return;
//
//		const auto override_entry = icon_override_map.find(original_item->icon);
//
//		// We are overriding its icon when not needed
//		if (override_entry != end(icon_override_map))
//			icon_override_map.erase(override_entry); // Remove the leftover override
//	}
//}
//
//static auto apply_config_on_attributable_item(C_BaseAttributableItem* item, const item_setting* config,
//	const unsigned xuid_low) -> void
//{
//	*item->GetItemIDHigh() = -1;
//	*item->GetAccountID() = xuid_low;
//
//	if (config->entity_quality_index)
//		*item->GetEntityQuality() = config->entity_quality_index;
//
//	if (config->custom_name[0])
//		strcpy(item->GetCustomName(), config->custom_name);
//
//	if (config->paint_kit_index)
//		*item->GetFallbackPaintKit() = config->paint_kit_index;
//
//	if (config->seed)
//		*item->GetFallbackSeed() = config->seed;
//
//	if (config->stat_trak)
//		*item->GetFallbackStatTrak() = config->stat_trak;
//
//	*item->GetFallbackWear() = config->wear;
//
//	auto& definition_index = *item->GetItemDefinitionIndex();
//
//	auto& icon_override_map = g_config.get_icon_override_map();
//
//	if (config->definition_override_index
//		&& config->definition_override_index != definition_index)
//	{
//		if (const auto replacement_item = game_data::get_weapon_info(config->definition_override_index))
//		{
//			const auto old_definition_index = definition_index;
//
//			definition_index = config->definition_override_index;
//
//			item->SetModelIndex(g_model_info->GetModelIndex(replacement_item->model));
//			item->GetClientNetworkable()->PreDataUpdate(0);
//
//			// We didn't override 0, but some actual weapon, that we have data for
//			if (old_definition_index)
//			{
//				if (const auto original_item = game_data::get_weapon_info(old_definition_index))
//				{
//					if (original_item->icon && replacement_item->icon)
//						icon_override_map[original_item->icon] = replacement_item->icon;
//				}
//			}
//		}
//	}
//	else
//	{
//		erase_override_if_exists_by_index(definition_index);
//	}
//
//	apply_sticker_changer(item);
//}
//
//static auto get_wearable_create_fn() -> CreateClientClassFn
//{
//	auto clazz = Interfaces::Client->GetAllClasses();
//
//	while (fnv::hash_runtime(clazz->m_pNetworkName) != FNV("CEconWearable"))
//		clazz = clazz->m_pNext;
//
//	return (CreateClientClassFn)clazz->m_pCreateFn;
//}
//
//static auto make_glove(int entry, int serial) -> C_BaseAttributableItem*
//{
//	static auto create_wearable_fn = get_wearable_create_fn();
//
//	create_wearable_fn(entry, serial);
//
//	const auto glove = static_cast<C_BaseAttributableItem*>(Interfaces::EntityList->GetClientEntity(entry));
//	assert(glove);
//
//	// He he
//	{
//		static auto set_abs_origin_addr = platform::find_pattern(get_client_name(), "\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1", "xxxxxxxxxxxx");
//
//		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const sdk::Vector&)>(set_abs_origin_addr);
//
//		static constexpr Vector new_pos = { 10000.f, 10000.f, 10000.f };
//
//		set_abs_origin_fn(glove, new_pos);
//	}
//
//	return glove;
//}
//
//namespace Cheat {
//	namespace Hooked {
//		void __fastcall PostDataUpdateHook(IClientNetworkable* pThis, void*, int iUpdateType) {
//			const auto local = (C_BaseCombatCharacter*)pThis;
//			const auto local_index = local->GetIndex();
//
//			player_info_t player_info;
//			if (!Interfaces::Engine->GetPlayerInfo(local_index, &player_info))
//				return;
//
//			// Handle glove config
//			{
//				const auto wearables = local->GetWearables();
//
//				const auto glove_config = g_config.get_by_definition_index(GLOVE_T_SIDE);
//				static auto glove_handle = CBaseHandle(0);
//
//				auto glove = (C_BaseAttributableItem*)Interfaces::EntityList->GetClientEntityFromHandle(wearables[0]);
//
//				if (!glove)
//				{
//					const auto our_glove = (C_BaseAttributableItem*)Interfaces::EntityList->GetClientEntityFromHandle(glove_handle);
//
//					if (our_glove)
//					{
//						wearables[0] = glove_handle;
//						glove = our_glove;
//					}
//				}
//
//				if (!local->IsAlive())
//				{
//					if (glove)
//					{
//						glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
//						glove->GetClientNetworkable()->Release();
//					}
//
//					return;
//				}
//
//				if (glove_config && glove_config->definition_override_index)
//				{
//					if (!glove)
//					{
//						const auto entry = Interfaces::EntityList->GetHighestEntityIndex() + 1;
//						const auto serial = rand() % 0x1000;
//
//						glove = make_glove(entry, serial);
//
//						wearables[0] = entry | serial << 16;
//
//						// Let's store it in case we somehow lose it.
//						glove_handle = wearables[0];
//					}
//
//					*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;
//
//					apply_config_on_attributable_item(glove, glove_config, player_info.xuid_low);
//				}
//			}
//
//			{
//				auto& weapons = local->GetWeapons();
//
//				for (auto weapon_handle : weapons)
//				{
//					if (weapon_handle == 0xFFFFFFFF)
//						break;
//
//					auto weapon = get_entity_from_handle<sdk::C_BaseAttributableItem>(weapon_handle);
//
//					if (!weapon)
//						continue;
//
//					auto& definition_index = weapon->GetItemDefinitionIndex();
//
//					// All knives are terrorist knives.
//					if (const auto active_conf = g_config.get_by_definition_index(is_knife(definition_index) ? WEAPON_KNIFE : definition_index))
//						apply_config_on_attributable_item(weapon, active_conf, player_info.xuid_low);
//					else
//						erase_override_if_exists_by_index(definition_index);
//				}
//			}
//
//			const auto view_model = get_entity_from_handle<C_BaseViewModel>(local->GetViewModel());
//
//			if (!view_model)
//				return;
//
//			const auto view_model_weapon = get_entity_from_handle<C_BaseAttributableItem>(view_model->GetWeapon());
//
//			if (!view_model_weapon)
//				return;
//
//			const auto override_info = game_data::get_weapon_info(view_model_weapon->GetItemDefinitionIndex());
//
//			if (!override_info)
//				return;
//
//			const auto override_model_index = Interfaces::ModelInfo->GetModelIndex(override_info->model);
//			view_model->GetModelIndex() = override_model_index;
//
//			const auto world_model = get_entity_from_handle<CBaseWeaponWorldModel>(view_model_weapon->GetWeaponWorldModel());
//
//			if (!world_model)
//				return;
//
//			world_model->GetModelIndex() = override_model_index + 1;
//		}
//	}
//}
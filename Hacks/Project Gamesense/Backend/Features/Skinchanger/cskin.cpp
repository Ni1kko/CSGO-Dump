//#include "../../Include/GlobalIncludes.h"
//#include "../../SDK/CBaseEntity.h"
//#include "../../SDK/CViewSetup.h"
//#include "cskin.h"
//#include <fstream>
//#include <experimental/filesystem> // hack
//
//#pragma warning (disable: 4715) // condition expression is constant
//
//const std::map<size_t, Item_t> k_weapon_info =
//{
//	{ WEAPON_KNIFE,{ "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
//	{ WEAPON_KNIFE_T,{ "models/weapons/v_knife_default_t.mdl", "knife_t" } },
//	{ WEAPON_KNIFE_BAYONET,{ "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
//	{ WEAPON_KNIFE_CSS, {"models/weapons/v_knife_css.mdl", "knife_css"}},
//	{ WEAPON_KNIFE_FLIP,{ "models/weapons/v_knife_flip.mdl", "knife_flip" } },
//	{ WEAPON_KNIFE_GUT,{ "models/weapons/v_knife_gut.mdl", "knife_gut" } },
//	{ WEAPON_KNIFE_KARAMBIT,{ "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
//	{ WEAPON_KNIFE_M9_BAYONET,{ "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
//	{ WEAPON_KNIFE_CSS,{"models/weapons/v_knife_css.mdl", "knife_css"}},
//	{ WEAPON_KNIFE_TACTICAL,{ "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
//	{ WEAPON_KNIFE_FALCHION,{ "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
//	{ WEAPON_KNIFE_SURVIVAL_BOWIE,{ "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
//	{ WEAPON_KNIFE_BUTTERFLY,{ "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
//	{ WEAPON_KNIFE_PUSH,{ "models/weapons/v_knife_push.mdl", "knife_push" } },
//	{ WEAPON_KNIFE_CORD,{ "models/weapons/v_knife_cord.mdl", "knife_cord" } },
//	{ WEAPON_KNIFE_CANIS,{ "models/weapons/v_knife_canis.mdl", "knife_push" } },
//	{ WEAPON_KNIFE_URSUS,{ "models/weapons/v_knife_ursus.mdl", "knife_ursus" } },
//	{ WEAPON_KNIFE_GYPSY_JACKKNIFE,{ "models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife" } },
//	{ WEAPON_KNIFE_OUTDOOR,{ "models/weapons/v_knife_outdoor.mdl", "knife_stiletto" } },
//	{ WEAPON_KNIFE_STILETTO,{ "models/weapons/v_knife_stiletto.mdl", "knife_stiletto" } },
//	{ WEAPON_KNIFE_WIDOWMAKER,{ "models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker" } },
//	{ WEAPON_KNIFE_SKELETON,{ "models/weapons/v_knife_skeleton.mdl", "knife_skeleton" } },
//
//};
//
//const std::vector<WeaponName_t> k_knife_names =
//{
//	{ 0, "Default" },
//	{ WEAPON_KNIFE_BAYONET, "Bayonet" },
//	{ WEAPON_KNIFE_CSS, "Classic Knife"},
//	{ WEAPON_KNIFE_FLIP, "Flip Knife" },
//	{ WEAPON_KNIFE_GUT, "Gut Knife" },
//	{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
//	{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
//	{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
//	{ WEAPON_KNIFE_FALCHION, "Falchion Knife" },
//	{ WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife" },
//	{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
//	{ WEAPON_KNIFE_PUSH, "Shadow Daggers" },
//	{WEAPON_KNIFE_CORD, "Paracord Knife"},
//	{ WEAPON_KNIFE_CANIS, "Survival Knife" },
//	{ WEAPON_KNIFE_URSUS, "Ursus Knife"},
//	{ WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife" },
//	{ WEAPON_KNIFE_OUTDOOR, "Nomad Knife" },
//	{ WEAPON_KNIFE_STILETTO, "Stiletto Knife" },
//	{ WEAPON_KNIFE_WIDOWMAKER, "Talon Knife" },
//	{ WEAPON_KNIFE_SKELETON, "Skeleton Knife" }
//};
//
//const std::vector<WeaponName_t> k_weapon_names =
//{
//	{ WEAPON_KNIFE, "Knife" },
//	{ 7, "AK-47" },
//	{ 8, "AUG" },
//	{ 9, "AWP" },
//	{ 63, "CZ-75" },
//	{ 1, "Desert Eagle" },
//	{ 2, "Dual Berettas" },
//	{ 10, "FAMAS" },
//	{ 3, "Five-SeveN" },
//	{ 11, "G3SG1" },
//	{ 13, "Galil AR" },
//	{ 4, "Glock-18" },
//	{ 14, "M249" },
//	{ 60, "M4A1-S" },
//	{ 16, "M4A4" },
//	{ 17, "MAC-10" },
//	{ 27, "MAG-7" },
//	{ 33, "MP7" },
//	{ 23, "MP5SD"},
//	{ 34, "MP9" },
//	{ 28, "Negev" },
//	{ 35, "Nova" },
//	{ 32, "P2000" },
//	{ 36, "P250" },
//	{ 19, "P90" },
//	{ 26, "PP-Bizon" },
//	{ 64, "R8 Revolver" },
//	{ 29, "Sawed-Off" },
//	{ 38, "SCAR-20" },
//	{ 40, "SSG 08" },
//	{ 39, "SG-553" },
//	{ 30, "Tec-9" },
//	{ 24, "UMP-45" },
//	{ 61, "USP-S" },
//	{ 25, "XM1014" },
//};
//
//enum ESequence
//{
//	SEQUENCE_CSS_LOOKAT01 = 14,
//	SEQUENCE_CSS_LOOKAT02 = 15,
//};
//
//inline int RandomSequence(int low, int high)
//{
//	return rand() % (high - low + 1) + low;
//}
//
//// Map of animation fixes
//// unfortunately can't be constexpr
//const static std::unordered_map<std::string, int(*)(int)> animation_fix_map
//{
//	{ "models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
//		default:
//			return sequence + 1;
//		}
//	} },
//	{ "models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_FALCHION_IDLE1;
//		case SEQUENCE_DEFAULT_HEAVY_MISS1:
//			return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		default:
//			return sequence - 1;
//		}
//
//		{ "models/weapons/v_knife_css.mdl", [](int sequence) -> int
//		{
//			switch (sequence)
//			{
//			case SEQUENCE_DEFAULT_LOOKAT01:
//				return RandomSequence(SEQUENCE_CSS_LOOKAT01, SEQUENCE_CSS_LOOKAT02);
//			default:
//				return sequence;
//			}
//		}; }
//	} },
//	{ "models/weapons/v_knife_push.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_DAGGERS_IDLE1;
//		case SEQUENCE_DEFAULT_LIGHT_MISS1:
//		case SEQUENCE_DEFAULT_LIGHT_MISS2:
//			return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
//		case SEQUENCE_DEFAULT_HEAVY_MISS1:
//			return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
//		case SEQUENCE_DEFAULT_HEAVY_HIT1:
//		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return sequence + 3;
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		default:
//			return sequence + 2;
//		}
//	} },
//	{ "models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//		case SEQUENCE_DEFAULT_IDLE1:
//			return sequence;
//		case SEQUENCE_DEFAULT_IDLE2:
//			return SEQUENCE_BOWIE_IDLE1;
//		default:
//			return sequence - 1;
//		}
//	} },
//
//	{ "models/weapons/v_knife_cord.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//		default:
//			return sequence + 1;
//		}
//	} },
//
//	{ "models/weapons/v_knife_canis.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//		default:
//			return sequence + 1;
//		}
//	} },
//
//	{ "models/weapons/v_knife_outdoor.mdl", [](int sequence) -> int
//		{
//			switch (sequence)
//			{
//			case SEQUENCE_DEFAULT_DRAW:
//				return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//			case SEQUENCE_DEFAULT_LOOKAT01:
//				return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//			default:
//				return sequence + 1;
//			}
//		} },
//
//	{ "models/weapons/v_knife_skeleton.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//		default:
//			return sequence + 1;
//		}
//	} },
//
//	{ "models/weapons/v_knife_ursus.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_DRAW:
//			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
//		default:
//			return sequence + 1;
//		}
//	} },
//	{ "models/weapons/v_knife_stiletto.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(12, 13);
//		}
//	} },
//	{ "models/weapons/v_knife_widowmaker.mdl", [](int sequence) -> int
//	{
//		switch (sequence)
//		{
//		case SEQUENCE_DEFAULT_LOOKAT01:
//			return RandomSequence(14, 15);
//		}
//	} }
//
//};
//
//void Proxies::nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
//{
//	auto local = G::LocalPlayer;
//
//	if (!local || !local->IsAlive())
//		return o_nSequence(pData, pStruct, pOut);
//
//	CRecvProxyData* proxy_data = const_cast<CRecvProxyData*>(pData);
//	CBaseViewModel* view_model = static_cast<CBaseViewModel*>(pStruct);
//
//	if (view_model && view_model->GetOwner())
//	{
//		auto owner = view_model->GetOwner();
//
//		if (owner == local->GetIndex())
//		{
//			// Get the filename of the current view model.
//			auto knife_model = Interfaces::ModelInfo->GetModel(view_model->GetModelIndex());
//			auto model_name = Interfaces::ModelInfo->GetModelName(knife_model);
//
//			if (animation_fix_map.count(model_name))
//				proxy_data->m_Value.m_Int = animation_fix_map.at(model_name)(proxy_data->m_Value.m_Int);
//		}
//	}
//
//	o_nSequence(proxy_data, pStruct, pOut);
//}
//
//bool IsKnife(int i)
//{
//	return (i >= WEAPON_KNIFE_BAYONET && i < 5027) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
//}
//
//void ForceItemUpdate(CBaseWeapon* item)
//{
//	if (!item)
//		return;
//
//	CEconItemView& view = item->m_AttributeManager.m_Item;
//
//	item->m_bCustomMaterialInitialized = (reinterpret_cast<CBaseAttributableItem*>(item)->GetFallbackPaintKit() <= 0);
//
//	item->m_CustomMaterials.VectorClear(item->m_CustomMaterials); // clear vector, but don't unreference items
//	view.m_CustomMaterials.VectorClear(view.m_CustomMaterials);
//}
//
//static void EraseOverrideIfExistsByIndex(int definition_index)
//{
//	// We have info about the item not needed to be overridden
//	if (k_weapon_info.count(definition_index))
//	{
//		auto& icon_override_map = Skinchanger::get()->GetIconOverrideMap();
//
//		const auto& original_item = k_weapon_info.at(definition_index);
//
//		// We are overriding its icon when not needed
//		if (original_item.icon && icon_override_map.count(original_item.icon))
//			icon_override_map.erase(icon_override_map.at(original_item.icon)); // Remove the leftover override
//	}
//}
//
//static void ApplyConfigOnAttributableItem(CBaseAttributableItem* item, const EconomyItem_t* config, unsigned xuid_low)
//{
//	if (config->definition_index != 5028 && config->definition_override_index != 5028)
//	{
//		auto world_model_handle = item->m_hWeaponWorldModel_c();
//
//		auto view_model_weapon = (CBaseEntity*)world_model_handle;
//
//		if (!view_model_weapon)
//			return;
//	}
//
//	// Force fallback values to be used.
//	*item->GetItemIDHigh() = -1;
//
//	// Set the owner of the weapon to our lower XUID. (fixes StatTrak)
//	*item->GetAccountID() = xuid_low;
//
//	if (config->entity_quality_index)
//		*item->GetEntityQuality() = config->entity_quality_index;
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
//	if (config->definition_override_index // We need to override defindex
//		&& config->definition_override_index != definition_index // It is not yet overridden
//		&& k_weapon_info.count(config->definition_override_index)) // We have info about what we gonna override it to
//	{
//		unsigned old_definition_index = definition_index;
//
//		definition_index = config->definition_override_index;
//
//		const auto& replacement_item = k_weapon_info.at(config->definition_override_index);
//
//		// Set the weapon model index -- required for paint kits to work on replacement items after the 29/11/2016 update.
//		item->GetClientNetworkable()->PreDataUpdate(0);
//	}
//	else
//	{
//		EraseOverrideIfExistsByIndex(definition_index);
//	}
//}
//
//void Skinchanger::Work()
//{
//	auto local_index = G::LocalPlayer->GetIndex());
//	auto local = G::LocalPlayer;
//	if (!local)
//		return;
//
//	player_info_t* pinfo;
//
//	if (!Interfaces::Engine->GetPlayerInfo(local_index, pinfo))
//		return;
//
//	// Handle weapon configs
//	{
//		auto weapons = local->GetWeapons();
//
//		for (size_t i = 0; weapons[i].IsValid(); i++)
//		{
//			auto weapon = weapons[i].Get();
//
//			if (!weapon)
//				continue;
//
//			auto& definition_index = weapon->m_iItemDefinitionIndex();
//
//			// All knives are terrorist knives.
//			if (auto active_conf = this->GetByDefinitionIndex(IsKnife(definition_index) ? WEAPON_KNIFE : definition_index))
//				ApplyConfigOnAttributableItem(weapon, active_conf, player_info.xuid_low);
//			else
//				EraseOverrideIfExistsByIndex(definition_index);
//		}
//	}
//
//	auto view_model_handle = local->m_hViewModel();
//
//	if (!view_model_handle.IsValid())
//		return;
//
//	auto view_model = view_model_handle.Get();
//
//	if (!view_model)
//		return;
//
//	auto view_model_weapon_handle = view_model->m_hWeapon();
//
//	if (!view_model_weapon_handle.IsValid())
//		return;
//
//	auto view_model_weapon = view_model_weapon_handle.Get();
//
//	if (!view_model_weapon)
//		return;
//
//	if (k_weapon_info.count(view_model_weapon->m_iItemDefinitionIndex()))
//	{
//		auto& override_model = k_weapon_info.at(view_model_weapon->m_iItemDefinitionIndex()).model;
//		view_model->m_nModelIndex() = g_MdlInfo->GetModelIndex(override_model);
//	}
//
//	if (bForceFullUpdate)
//	{
//		ForceItemUpdates();
//		bForceFullUpdate = false;
//	}
//}
//
//void Skinchanger::ForceItemUpdates()
//{
//	g_ClientState->ForceFullUpdate();
//}
//
//using json = nlohmann::json;
//
//void to_json(json& j, const EconomyItem_t& item)
//{
//	j = json
//	{
//		{ "name", item.name },
//		{ "enabled", item.enabled },
//		{ "definition_index", item.definition_index },
//		{ "entity_quality_index", item.entity_quality_index },
//		{ "paint_kit_index", item.paint_kit_index },
//		{ "definition_override_index", item.definition_override_index },
//		{ "seed", item.seed },
//		{ "stat_trak", item.stat_trak },
//		{ "wear", item.wear },
//		{ "custom_name", item.custom_name },
//	};
//}
//
//void from_json(const json& j, EconomyItem_t& item)
//{
//	strcpy_s(item.name, j.at("name").get<std::string>().c_str());
//	item.enabled = j.at("enabled").get<bool>();
//	item.definition_index = j.at("definition_index").get<int>();
//	item.entity_quality_index = j.at("entity_quality_index").get<int>();
//	item.paint_kit_index = j.at("paint_kit_index").get<int>();
//	item.definition_override_index = j.at("definition_override_index").get<int>();
//	item.seed = j.at("seed").get<int>();
//	item.stat_trak = j.at("stat_trak").get<int>();
//	item.wear = j.at("wear").get<float>();
//	strcpy_s(item.custom_name, j.at("custom_name").get<std::string>().c_str());
//	item.UpdateIds();
//}
//
//void Skinchanger::SaveSkins()
//{
//	std::string fPath = std::string(Global::my_documents_folder) + "\\Nasa\\" + "skinchanger.json";
//
//	std::ofstream(fPath) << json(m_items);
//}
//
//void Skinchanger::LoadSkins()
//{
//	auto compareFunction = [](const EconomyItem_t& a, const EconomyItem_t& b) { return std::string(a.name) < std::string(b.name); };
//
//	std::string fPath = std::string(Global::my_documents_folder) + "\\Nasa\\" + "skinchanger.json";
//
//	if (!Config::Get().FileExists(fPath))
//		return;
//
//	try
//	{
//		m_items = json::parse(std::ifstream(fPath)).get<std::vector<EconomyItem_t>>();
//		std::sort(m_items.begin(), m_items.end(), compareFunction);
//		ForceItemUpdates();
//	}
//	catch (const std::exception&) {}
//}
//
//EconomyItem_t* Skinchanger::GetByDefinitionIndex(int definition_index)
//{
//	for (auto& x : m_items)
//		if (x.enabled && x.definition_index == definition_index)
//			return &x;
//
//	return nullptr;
//}
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "skinchanger2.h"

Skins_t G::skins;

item_setting* get_by_definition_index(const int definition_index)
{
	if (G::skins.skinChanger.count(definition_index))
		return &G::skins.skinChanger[definition_index];
	return nullptr;
}

static const char* player_model_index_t[] =
{
	"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
	"models/player/custom_player/legacy/tm_phoenix_varianth.mdl",
	"models/player/custom_player/legacy/tm_leet_variantg.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
	"models/player/custom_player/legacy/tm_leet_varianth.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
	"models/player/custom_player/legacy/tm_leet_varianti.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
	"models/player/custom_player/legacy/tm_leet_variantf.mdl"
};

static const char* player_model_index_ct[] =
{
	"models/player/custom_player/legacy/ctm_st6_variante.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
	"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantb.mdl"
};

Memory memory;

std::unordered_map <std::string, int> SkinChanger::model_indexes;
std::unordered_map <std::string, int> SkinChanger::player_model_indexes;

std::vector <SkinChanger::PaintKit> SkinChanger::skinKits;
std::vector <SkinChanger::PaintKit> SkinChanger::gloveKits;
std::vector <SkinChanger::PaintKit> SkinChanger::displayKits;

bool SkinChanger::updating_skins = false;

std::unordered_map <std::string_view, const char*> SkinChanger::iconOverrides;

static void erase_override_if_exists_by_index(const int definition_index) noexcept
{
    if (auto original_item = game_data::get_weapon_info(definition_index)) 
	{
        if (!original_item->icon)
            return;

        if (const auto override_entry = SkinChanger::iconOverrides.find(original_item->icon); override_entry != end(SkinChanger::iconOverrides))
			SkinChanger::iconOverrides.erase(override_entry);
    }
}

static void apply_config_on_attributable_item(CBaseAttributableItem* item, const item_setting* config, const unsigned xuid_low) noexcept
{
    auto& definition_index = *item->GetItemDefinitionIndex();

    if (config->definition_override_index && config->definition_override_index != definition_index)
    {
        if (auto replacement_item = game_data::get_weapon_info(config->definition_override_index)) 
		{
            auto old_definition_index = definition_index;
            definition_index = config->definition_override_index;

			if (SkinChanger::model_indexes.find(replacement_item->model) == SkinChanger::model_indexes.end())
				SkinChanger::model_indexes.emplace(replacement_item->model, Interfaces::ModelInfo->GetModelIndex(replacement_item->model));

            item->SetModelIndex(SkinChanger::model_indexes.at(replacement_item->model));

			if (config->quality)
				*item->GetEntityQuality() = config->quality;

            if (old_definition_index) 
                if (auto original_item = game_data::get_weapon_info(old_definition_index); original_item && original_item->icon && replacement_item->icon)
                    SkinChanger::iconOverrides[original_item->icon] = replacement_item->icon;
        }
    } 
	else
        erase_override_if_exists_by_index(definition_index);

	*item->GetItemIDHigh() = -1; //-V522
	*item->GetAccountID() = xuid_low;

	/* if (config->custom_name[0])
	 strcpy_s(item->m_szCustomName(), sizeof(config->custom_name), config->custom_name);*/

	*item->GetFallbackWear() = config->wear;

	if (config->paintKit)
		*item->GetFallbackPaintKit() = config->paintKit;

	if (config->seed)
		*item->GetFallbackSeed() = config->seed;

	if (config->stat_trak)
		*item->GetFallbackStatTrak() = config->stat_trak;

	item->PreDataUpdate(0);
}

static auto get_wearable_create_fn() -> CreateClientClassFn
{
	auto classes = Interfaces::Client->GetAllClasses();

	while (classes->m_ClassID != (int)ClassID::CEconWearable)
		classes = classes->m_pNext;

	return (CreateClientClassFn)classes->m_pCreateFn;
}

static CBaseAttributableItem* make_glove(int entry, int serial) noexcept
{
	get_wearable_create_fn()(entry, serial);
	auto glove = (CBaseAttributableItem*)Interfaces::EntityList->GetClientEntity(entry);

	if (!glove)
		return nullptr;

	static auto Fn = Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("55 8B EC 83 E4 F8 51 53 56 57 8B F1"));
	static auto set_abs_origin = reinterpret_cast <void(__thiscall*)(void*, const Vector&)> (Fn);

	set_abs_origin(glove, Vector(16384.0f, 16384.0f, 16384.0f));
	return glove;
}

static float last_skins_update = 0.0f;

static void post_data_update_start(CBaseEntity* local) noexcept
{
    player_info_t player_info;

    if (!local->GetPlayerInfo(&player_info))
        return;

	static UINT glove_handle = 0;

	auto wearables = local->GetWearables();
	auto glove_config = get_by_definition_index(ItemDefinitionIndex::GLOVE_T_SIDE);
	auto glove = reinterpret_cast <CBaseAttributableItem*> (Interfaces::EntityList->GetClientEntityFromHandle(wearables[0]));

	if (!glove)
	{
		auto our_glove = reinterpret_cast <CBaseAttributableItem*> (Interfaces::EntityList->GetClientEntityFromHandle(glove_handle));

		if (our_glove)
		{
			wearables[0] = glove_handle;
			glove = our_glove;
		}
	}

	if (!local->IsAlive())
	{
		if (glove)
		{
			glove->SetDestroyedOnRecreateEntities();
			glove->Release();
		}

		return;
	}

	if (glove_config && glove_config->definition_override_index)
	{
		if (!glove)
		{
			auto entry = Interfaces::EntityList->GetHighestEntityIndex() + 1;
			auto serial = rand() % 0x1000;

			glove = make_glove(entry, serial);
			wearables[0] = entry | serial << 16;
			glove_handle = wearables[0];
		}

		*reinterpret_cast <int*> (uintptr_t(glove) + 0x64) = -1;
		apply_config_on_attributable_item(glove, glove_config, player_info.xuidlow);
	}

	auto weapons = local->GetWeapons2();

	for (auto weapon_handle = 0; weapons[weapon_handle] != INVALID_EHANDLE_INDEX; ++weapon_handle)
	{
		auto weapon = (CBaseWeapon*)Interfaces::EntityList->GetClientEntityFromHandle(weapons[weapon_handle]);

		if (!weapon)
			continue;

		auto definition_index = weapon->GetItemDefenitionIndex();

		if (auto active_conf = get_by_definition_index(is_knife(definition_index) ? WEAPON_KNIFE : definition_index))
			apply_config_on_attributable_item((CBaseAttributableItem*)weapon, active_conf, player_info.xuidlow);
		else
			erase_override_if_exists_by_index(definition_index);
	}

	auto view_model = (CBaseViewModel*)Interfaces::EntityList->GetClientEntityFromHandle(local->m_hViewModel());

    if (!view_model)
        return;

    auto view_model_weapon = (CBaseWeapon*)Interfaces::EntityList->GetClientEntityFromHandle(view_model->GetWeapon());

    if (!view_model_weapon)
        return;

    auto override_info = game_data::get_weapon_info(view_model_weapon->GetItemDefenitionIndex());

    if (!override_info)
        return;

	auto world_model = (CBaseWeapon*)Interfaces::EntityList->GetClientEntityFromHandle(*(PDWORD)view_model_weapon->m_hWeaponWorldModel());

	if (!world_model)
		return;

	if (SkinChanger::model_indexes.find(override_info->model) == SkinChanger::model_indexes.end())
		SkinChanger::model_indexes.emplace(override_info->model, Interfaces::ModelInfo->GetModelIndex(override_info->model));

	*((CBaseEntity*)view_model)->m_nModelIndex()  = SkinChanger::model_indexes.at(override_info->model);
    *((CBaseEntity*)world_model)->m_nModelIndex() = SkinChanger::model_indexes.at(override_info->model) + 1;
}

static bool UpdateRequired = false;
static bool hudUpdateRequired = false;

static constexpr void updateHud() noexcept
{
    if (auto hud_weapons = memory.findHudElement(memory.hud, xorstr_("CCSGO_HudWeaponSelection")) - 0x28)
        for (auto i = 0; i < *(hud_weapons + 0x20); i++)
            i = memory.clearHudWeapon(hud_weapons, i);
    
    hudUpdateRequired = false;
}

void knife_config_update()
{
	static int old_knife = -1;
	int new_knife = c_config::get()->b["skin_changer_override_knife"] ? c_config::get()->i["skin_changer_model_knife"] : 0;

	if (old_knife != new_knife)
	{
		old_knife = new_knife;

		auto weapon_knife = &G::skins.skinChanger[ItemDefinitionIndex::WEAPON_KNIFE];
		ZeroMemory(weapon_knife, sizeof(item_setting));
		weapon_knife->itemId = ItemDefinitionIndex::WEAPON_KNIFE;

		static std::array<int, 15> list_knifes
		{
			0,
			ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET,
			ItemDefinitionIndex::WEAPON_KNIFE_BAYONET,
			ItemDefinitionIndex::WEAPON_KNIFE_FLIP,
			ItemDefinitionIndex::WEAPON_KNIFE_GUT,
			ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT,
			ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL,
			ItemDefinitionIndex::WEAPON_KNIFE_FALCHION,
			ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE,
			ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY,
			ItemDefinitionIndex::WEAPON_KNIFE_PUSH,
			ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE,
			ItemDefinitionIndex::WEAPON_KNIFE_STILETTO,
			ItemDefinitionIndex::WEAPON_KNIFE_URSUS,
			ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER
		};

		if (new_knife >= 0 && new_knife < 15)
		{
			weapon_knife->definition_override_index = list_knifes.at(new_knife);
			if (new_knife >= 2)
			{
				weapon_knife->quality = 3;
				weapon_knife->wear = 0.01f;
				// weapon_knife->paintKit = 419;
			}
		}
	}
}

void glove_config_update()
{
	static int old_model_gloves = -1;
	int new_model_gloves = c_config::get()->b["skin_changer_override_gloves"] ? (c_config::get()->i["skin_changer_model_gloves"] + 1) : 0;

	static int old_paintkit_gloves = -1;
	int new_paintkit_gloves = c_config::get()->i["skin_changer_paintkit_gloves"];

	if ((old_model_gloves != new_model_gloves) || (old_paintkit_gloves != new_paintkit_gloves))
	{
		old_model_gloves = new_model_gloves;
		old_paintkit_gloves = new_paintkit_gloves;

		auto gloves = &G::skins.skinChanger[ItemDefinitionIndex::GLOVE_T_SIDE];
		ZeroMemory(gloves, sizeof(item_setting));
		gloves->itemId = ItemDefinitionIndex::GLOVE_T_SIDE;

		static std::array<int, 8> list_gloves
		{
			0,
			ItemDefinitionIndex::GLOVE_STUDDED_BLOODHOUND,
			ItemDefinitionIndex::GLOVE_LEATHER_WRAP,
			ItemDefinitionIndex::GLOVE_SLICK,
			ItemDefinitionIndex::GLOVE_SPORTY,
			ItemDefinitionIndex::GLOVE_SPECIALIST,
			ItemDefinitionIndex::GLOVE_MOTORCYCLE,
			ItemDefinitionIndex::GLOVE_HYDRA
		};

		if (new_model_gloves >= 0 && new_model_gloves < 8)
			gloves->definition_override_index = list_gloves.at(new_model_gloves);


		if (c_config::get()->b["skin_changer_override_gloves"])
		{
			static std::vector<std::vector<int>> skin_changer_list_paintkit_gloves
			{
				{ GLOVE_PAINTKIT_BRONZED, GLOVE_PAINTKIT_CHARRED, GLOVE_PAINTKIT_GUERRILLA, GLOVE_PAINTKIT_SNAKEBITE },

				{ GLOVE_PAINTKIT_ARBOREAL, GLOVE_PAINTKIT_BADLANDS, GLOVE_PAINTKIT_COBALT_SKULLS, GLOVE_PAINTKIT_DUCT_TAPE, GLOVE_PAINTKIT_LEATHER, GLOVE_PAINTKIT_OVERPRINT, GLOVE_PAINTKIT_SLAUGHTER, GLOVE_PAINTKIT_SPRUCE_DDPAT },

				{ GLOVE_PAINTKIT_CONVOY, GLOVE_PAINTKIT_CRIMSON_WEAVE, GLOVE_PAINTKIT_DIAMONDBACK, GLOVE_PAINTKIT_IMPERIAL_PLAID, GLOVE_PAINTKIT_KING_SNAKE, GLOVE_PAINTKIT_LUNAR_WEAVE, GLOVE_PAINTKIT_OVERTAKE, GLOVE_PAINTKIT_RACING_GREEN },

				{ GLOVE_PAINTKIT_AMPHIBIOUS, GLOVE_PAINTKIT_ARID, GLOVE_PAINTKIT_BONZE_MORPH, GLOVE_PAINTKIT_HEDGE_MAZE, GLOVE_PAINTKIT_OMEGA, GLOVE_PAINTKIT_PANDORAS_BOX, GLOVE_PAINTKIT_SUPERCONDUCTOR, GLOVE_PAINTKIT_VICE },

				{ GLOVE_PAINTKIT_BUCKSHOT, GLOVE_PAINTKIT_CRIMSON_KIMONO, GLOVE_PAINTKIT_CRIMSON_WEB, GLOVE_PAINTKIT_EMERALD_WEB, GLOVE_PAINTKIT_FADE, GLOVE_PAINTKIT_FOREST_DDPAT, GLOVE_PAINTKIT_FOUNDATION, GLOVE_PAINTKIT_MOGUL },

				{ GLOVE_PAINTKIT_BOOM, GLOVE_PAINTKIT_COOL_MINT, GLOVE_PAINTKIT_ECLIPSE, GLOVE_PAINTKIT_POW, GLOVE_PAINTKIT_POLYGON, GLOVE_PAINTKIT_SPEARMINT, GLOVE_PAINTKIT_TRANSPORT, GLOVE_PAINTKIT_TURTLE },

				{ GLOVE_PAINTKIT_CASE_HARDENED, GLOVE_PAINTKIT_EMERALD, GLOVE_PAINTKIT_MANGROVE, GLOVE_PAINTKIT_RATTLER },
			};

			auto& skin_changer_sublist_paintkit_gloves = skin_changer_list_paintkit_gloves[c_config::get()->i["skin_changer_model_gloves"]];
			if (new_paintkit_gloves >= skin_changer_sublist_paintkit_gloves.size())
				new_paintkit_gloves = 0;
			gloves->paintKit = skin_changer_sublist_paintkit_gloves[new_paintkit_gloves];
		}
	}
}

void SkinChanger::run(ClientFrameStage_t stage) noexcept
{
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!G::LocalPlayer)
		return;

	knife_config_update();
	glove_config_update();

	post_data_update_start(G::LocalPlayer);

	if (!G::LocalPlayer->IsAlive()) //-V807
	{
		UpdateRequired = false;
		hudUpdateRequired = false;
		return;
	}

	//static auto backup_model_index = -1;

	//if (g_cfg.player.enable)
	//{
	//	const char** player_model_index = nullptr;
	//	auto player_model = 0;

	//	switch (G::LocalPlayer->GetTeam())
	//	{
	//	case 2:
	//		player_model_index = player_model_index_t;
	//		player_model = g_cfg.player.player_model_t;
	//		break;
	//	case 3:
	//		player_model_index = player_model_index_ct;
	//		player_model = g_cfg.player.player_model_ct;
	//		break;
	//	}

	//	if (player_model)
	//	{
	//		if (!g_ctx.globals.backup_model)
	//		{
	//			auto model = G::LocalPlayer->GetModel();

	//			if (model)
	//			{
	//				auto studio_model = Interfaces::ModelInfo->GetStudioModel(model);

	//				if (studio_model)
	//				{
	//					auto name = xorstr_("models/") + (std::string)studio_model->name;
	//					backup_model_index = Interfaces::ModelInfo->GetModelIndex(name.c_str());
	//				}
	//			}
	//		}

	//		if (SkinChanger::player_model_indexes.find(player_model_index[player_model - 1]) == SkinChanger::player_model_indexes.end()) //-V522
	//			SkinChanger::player_model_indexes.emplace(player_model_index[player_model - 1], Interfaces::ModelInfo->GetModelIndex(player_model_index[player_model - 1]));

	//		G::LocalPlayer->set_model_index(SkinChanger::player_model_indexes[player_model_index[player_model - 1]]);
	//		g_ctx.globals.backup_model = true;
	//	}
	//	else if (g_ctx.globals.backup_model)
	//	{
	//		G::LocalPlayer->set_model_index(backup_model_index);
	//		g_ctx.globals.backup_model = false;
	//	}
	//}

	if (UpdateRequired)
	{
		UpdateRequired = false;
		hudUpdateRequired = true;

		Interfaces::ClientState->iDeltaTick = -1;
		SkinChanger::updating_skins = true;
	}
	else if (hudUpdateRequired && !SkinChanger::updating_skins)
	{
		hudUpdateRequired = false;
		updateHud();
	}
}

void SkinChanger::scheduleHudUpdate() noexcept
{
	if (!G::LocalPlayer->IsAlive())
		return;

	if (Interfaces::Globals->realtime - last_skins_update < 1.0f)
		return;

	UpdateRequired = true;
	last_skins_update = Interfaces::Globals->realtime;
}

void SkinChanger::overrideHudIcon(IGameEvent* event) noexcept
{
	if (auto iconOverride = iconOverrides[event->GetString(xorstr_("weapon"))])
		event->SetString(xorstr_("weapon"), iconOverride);
}

void SkinChanger::setup_skins()
{
	auto items = std::ifstream(xorstr_("csgo/scripts/items/items_game_cdn.txt"));
	auto gameItems = std::string(std::istreambuf_iterator <char> { items }, std::istreambuf_iterator <char> { });

	if (!items.is_open())
		return;

	items.close();
	memory.initialize();

	for (auto i = 0; i <= memory.itemSchema()->paintKits.lastElement; i++)
	{
		auto paintKit = memory.itemSchema()->paintKits.memory[i].value;

		if (paintKit->id == 9001)
			continue;

		auto itemName = Interfaces::Localize->FindSafe(paintKit->itemName.buffer + 1);
		auto itemNameLength = WideCharToMultiByte(CP_UTF8, 0, itemName, -1, nullptr, 0, nullptr, nullptr);

		if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, itemName, -1, &name[0], itemNameLength, nullptr, nullptr))
		{
			if (paintKit->id < 10000)
			{
				if (auto pos = gameItems.find('_' + std::string{ paintKit->name.buffer } + '='); pos != std::string::npos && gameItems.substr(pos + paintKit->name.length).find('_' + std::string{ paintKit->name.buffer } + '=') == std::string::npos)
				{
					if (auto weaponName = gameItems.rfind(xorstr_("weapon_"), pos); weaponName != std::string::npos)
					{
						name.back() = ' ';
						name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
					}
				}
				SkinChanger::skinKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
			else
			{
				std::string_view gloveName{ paintKit->name.buffer };
				name.back() = ' ';
				name += '(' + std::string{ gloveName.substr(0, gloveName.find('_')) } + ')';
				SkinChanger::gloveKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
		}
	}

	std::sort(SkinChanger::skinKits.begin(), SkinChanger::skinKits.end());
	std::sort(SkinChanger::gloveKits.begin(), SkinChanger::gloveKits.end());

	static auto new_changer = [](int itemId) {
		G::skins.skinChanger[itemId] = {};
		auto item = &G::skins.skinChanger[itemId];
		item->itemId = itemId;
	};

	new_changer(ItemDefinitionIndex::WEAPON_KNIFE);
	new_changer(ItemDefinitionIndex::GLOVE_T_SIDE);
}
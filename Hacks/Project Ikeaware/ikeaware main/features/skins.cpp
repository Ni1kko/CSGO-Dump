#include <Windows.h>
#include <WinInet.h>

#include "skins.hpp"

#include "..//helpers/items.hpp"
#include "..//helpers/parser.hpp"
#include "..//options.hpp"
#include <filesystem>

struct skin_info
{
	int seed = -1;
	int paintkit;
	int rarity = 0;
	std::string tag_name;
	std::string cdn_name;
};

#pragma comment(lib, "Wininet")

CreateClientClassFn GetWearableCreateFn()
{
	auto clazz = g_CHLClient->GetAllClasses();
	// Please, if you gonna paste it into a cheat use classids here. I use names because they
	// won't change in the foreseeable future and i dont need high speed, but chances are
	// you already have classids, so use them instead, they are faster.
	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;
	return clazz->m_pCreateFn;
}

const weapon_info* GetWeaponInfo(const short& defindex)
{
	const auto entry = k_weapon_info.find(defindex);

	return entry == k_weapon_info.end() ? nullptr : &entry->second;
}

void Skins::EraseOverrideIfExistsByIndex(const short definition_index)
{
	if (const auto original_item = GetWeaponInfo(definition_index))
	{
		if (!original_item->icon)
			return;

		const auto override_entry = m_icon_overrides.find(original_item->icon);
		if (override_entry != std::end(m_icon_overrides))
			m_icon_overrides.erase(override_entry);
	}
}

const char* Skins::GetIconOverride(const std::string& original)
{
	return m_icon_overrides.count(original) > 0 ? m_icon_overrides.at(original).data() : nullptr;
}

int GetWeaponRarity(std::string rarity)
{
	if (rarity == "default")
		return 0;
	else if (rarity == "common")
		return 1;
	else if (rarity == "uncommon")
		return 2;
	else if (rarity == "rare")
		return 3;
	else if (rarity == "mythical")
		return 4;
	else if (rarity == "legendary")
		return 5;
	else if (rarity == "ancient")
		return 6;
	else if (rarity == "immortal")
		return 7;
	else if (rarity == "unusual")
		return 99;

	return 0;
}

void Skins::ParseSkins() const
{
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skin_info> skinMap;
	std::unordered_map<std::string, std::string> skinNames;

	valve_parser::document doc;
	auto r = doc.load(R"(.\csgo\scripts\items\items_game.txt)", valve_parser::encoding::utf8);
	if (!r)
		return;

	valve_parser::document english;
	r = english.load(R"(.\csgo\resource\csgo_english.txt)", valve_parser::encoding::utf16_le);
	if (!r)
		return;

	auto weapon_skin_combo = doc.breadth_first_search("weapon_icons");
	if (!weapon_skin_combo || !weapon_skin_combo->to_object())
		return;

	auto paint_kits_rarity = doc.breadth_first_search_multiple("paint_kits_rarity");
	if (paint_kits_rarity.empty())
		return;

	auto skin_data_vec = doc.breadth_first_search_multiple("paint_kits");
	if (skin_data_vec.empty())
		return;

	auto paint_kit_names = english.breadth_first_search("Tokens");
	if (!paint_kit_names || !paint_kit_names->to_object())
		return;

	std::array weaponNames = {
		std::string("deagle"),
		std::string("elite"),
		std::string("fiveseven"),
		std::string("glock"),
		std::string("ak47"),
		std::string("aug"),
		std::string("awp"),
		std::string("famas"),
		std::string("g3sg1"),
		std::string("galilar"),
		std::string("m249"),
		std::string("m4a1_silencer"),
		std::string("m4a1"),
		std::string("mac10"),
		std::string("p90"),
		std::string("ump45"),
		std::string("xm1014"),
		std::string("bizon"),
		std::string("mag7"),
		std::string("negev"),
		std::string("sawedoff"),
		std::string("tec9"),
		std::string("hkp2000"),
		std::string("mp5sd"),
		std::string("mp7"),
		std::string("mp9"),
		std::string("nova"),
		std::string("p250"),
		std::string("scar20"),
		std::string("sg556"),
		std::string("ssg08"),
		std::string("usp_silencer"),
		std::string("cz75a"),
		std::string("revolver"),
		std::string("knife_m9_bayonet"),
		std::string("bayonet"),
		std::string("knife_flip"),
		std::string("knife_gut"),
		std::string("knife_karambit"),
		std::string("knife_tactical"),
		std::string("knife_falchion"),
		std::string("knife_survival_bowie"),
		std::string("knife_butterfly"),
		std::string("knife_push"),
		std::string("knife_ursus"),
		std::string("knife_gypsy_jackknife"),
		std::string("knife_stiletto"),
		std::string("knife_widowmaker"),
		std::string("studded_bloodhound_gloves"),
		std::string("sporty_gloves"),
		std::string("slick_gloves"),
		std::string("leather_handwraps"),
		std::string("motorcycle_gloves"),
		std::string("specialist_gloves"),
		std::string("studded_hydra_gloves")
	};

	for (const auto& child : weapon_skin_combo->children)
	{
		if (child->to_object())
		{
			for (const auto& weapon : weaponNames)
			{
				auto skin_name = child->to_object()->get_key_by_name("icon_path")->to_key_value()->value.to_string();
				const auto pos = skin_name.find(weapon);
				if (pos != std::string::npos)
				{
					const auto pos2 = skin_name.find_last_of('_');
					weaponSkins[weapon].insert(
						skin_name.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	for (const auto& skin_data : skin_data_vec)
	{
		if (skin_data->to_object())
		{
			for (const auto& skin : skin_data->children)
			{
				if (skin->to_object())
				{
					skin_info si;
					si.paintkit = skin->to_object()->name.to_int();

					if (si.paintkit == 0)
						continue;

					auto skin_name = skin->to_object()->get_key_by_name("name")->to_key_value()->value.to_string();
					si.cdn_name = skin_name;
					auto tag_node = skin->to_object()->get_key_by_name("description_tag");
					if (tag_node)
					{
						auto tag = tag_node->to_key_value()->value.to_string();
						tag = tag.substr(1, std::string::npos);
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tag_name = tag;
					}



					auto key_val = skin->to_object()->get_key_by_name("seed");
					if (key_val != nullptr)
						si.seed = key_val->to_key_value()->value.to_int();

					skinMap[skin_name] = si;
				}
			}
		}
	}

	for (const auto& child : paint_kit_names->children)
	{
		if (child->to_key_value())
		{
			auto key = child->to_key_value()->key.to_string();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				skinNames[key] = child->to_key_value()->value.to_string();
			}
		}
	}

	for (const auto& rarity : paint_kits_rarity)
	{
		if (rarity->to_object())
		{
			for (const auto& child : rarity->children)
			{
				if (child->to_key_value())
				{
					std::string paint_kit_name = child->to_key_value()->key.to_string();
					std::string paint_kit_rarity = child->to_key_value()->value.to_string();

					auto skinInfo = &skinMap[paint_kit_name];

					skinInfo->rarity = GetWeaponRarity(paint_kit_rarity);
				}
			}
		}
	}

	for (auto weapon : weaponNames)
	{
		for (auto skin : weaponSkins[weapon])
		{
			skinInfo* info = &g_Options.inventory.skinInfo[skinMap[skin].paintkit];
			info->weaponName.push_back(weapon);
			info->cdnName = skin;
			info->name = skinNames[skinMap[skin].tag_name].c_str();
			info->rarity = skinMap[skin].rarity;
			g_Options.inventory.skinInfo.insert({ skinMap[skin].paintkit, *info });
		}
	}
}

bool Skins::ApplyCustomSkin(C_BasePlayer* localPlayer, C_BaseAttributableItem* pWeapon, short nWeaponIndex)
{
	switch (nWeaponIndex)
	{
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE:
		nWeaponIndex = WEAPON_KNIFE;
	}

	skins_s::sides skin_changer_cfg;
	if (localPlayer->m_iTeamNum() == 2)
		skin_changer_cfg = g_Options.weapons[nWeaponIndex].skins.t;
	else
		skin_changer_cfg = g_Options.weapons[nWeaponIndex].skins.ct;

	if (!skin_changer_cfg.enabled && nWeaponIndex != WEAPON_KNIFE && nWeaponIndex != WEAPON_KNIFE_T)
		return false;

	pWeapon->m_nFallbackPaintKit() = skin_changer_cfg.paintKit;
	pWeapon->m_Item().m_iEntityQuality() = skin_changer_cfg.quality;
	pWeapon->m_nFallbackSeed() = skin_changer_cfg.seed;
	pWeapon->m_nFallbackStatTrak() = skin_changer_cfg.stattrak;
	pWeapon->m_flFallbackWear() = max(skin_changer_cfg.wear, 0.00001f);

	pWeapon->m_Item().m_iItemIDHigh() = -1;

	if (skin_changer_cfg.definitionIndex != WEAPON_NONE) {
		static auto old_definition_index = pWeapon->m_Item().m_iItemDefinitionIndex();

		pWeapon->m_Item().m_iItemDefinitionIndex() = skin_changer_cfg.definitionIndex;
		pWeapon->m_Item().m_iEntityQuality() = 3;
		const auto& replacement_item = k_weapon_info.at(pWeapon->m_Item().m_iItemDefinitionIndex());

		pWeapon->SetModelIndex(g_MdlInfo->GetModelIndex(replacement_item.model));
		pWeapon->GetClientNetworkable()->PreDataUpdate(0);

		const auto original_item = GetWeaponInfo(old_definition_index);
		if (original_item && original_item->icon && replacement_item.icon)
			m_icon_overrides[original_item->icon] = replacement_item.icon;
	}
	return true;
}

void Skins::FrameStageNotify(bool frame_end)
{
	const auto localPlayerID = g_EngineClient->GetLocalPlayer();

	C_BasePlayer* local = C_BasePlayer::GetPlayerByIndex(localPlayerID);

	if (!local)
		return;

	player_info_t info;
	g_EngineClient->GetPlayerInfo(localPlayerID, &info);

	const auto wearables = local->m_hMyWearables();

	if (frame_end && !g_EntityList->GetClientEntityFromHandle(wearables[0]))
	{
		skins_s::sides gloveConfig;
		if (local->m_iTeamNum() == 2)
			gloveConfig = g_Options.weapons[GLOVE_T_SIDE].skins.t;
		else
			gloveConfig = g_Options.weapons[GLOVE_T_SIDE].skins.ct;

		static auto gloveHandle = CBaseHandle(0);
		auto glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(wearables[0]));

		if (!glove)
		{
			const auto ourGlove = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(gloveHandle);
			if (ourGlove)
			{
				wearables[0] = gloveHandle;
				glove = ourGlove;
			}
		}

		if (!local->IsAlive())
		{
			if (glove)
			{
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}
			return;
		}

		if (gloveConfig.definitionIndex)
		{
			if (!glove)
			{
				static auto create_wearable_fn = GetWearableCreateFn();
				const auto entry = g_EntityList->GetHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				create_wearable_fn(entry, serial);
				glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntity(entry));
				glove->SetAbsOrigin({ 10000.f, 10000.f, 10000.f });
				const auto wearable_handle = reinterpret_cast<CBaseHandle*>(&wearables[0]);
				*wearable_handle = entry | serial << 16;
				gloveHandle = wearables[0];

			}

			if (glove)
			{
				glove->m_Item().m_iItemDefinitionIndex() = gloveConfig.definitionIndex;
				glove->m_Item().m_iItemIDHigh() = -1;
				glove->m_Item().m_iEntityQuality() = 4;
				glove->m_Item().m_iAccountID() = info.xuid_low;
				glove->m_nFallbackSeed() = gloveConfig.seed;
				glove->m_nFallbackStatTrak() = -1;
				glove->m_flFallbackWear() = gloveConfig.wear;
				glove->m_nFallbackPaintKit() = gloveConfig.paintKit;
				glove->SetModelIndex(-1);
				glove->PreDataUpdate(0);
			}
		}
	}
	else
	{
		const auto weapons = local->m_hMyWeapons();

		if (!weapons)
			return;

		for (auto i = 0; weapons[i].IsValid(); i++)
		{
			auto weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(weapons[i]));

			if (!weapon)
				continue;

			const auto weaponIndex = weapon->m_Item().m_iItemDefinitionIndex();

			if (info.xuid_low != weapon->m_OriginalOwnerXuidLow())
				continue;

			if (info.xuid_high != weapon->m_OriginalOwnerXuidHigh())
				continue;

			ApplyCustomSkin(local, weapon, weaponIndex);

			weapon->m_Item().m_iAccountID() = info.xuid_low;
		}

		const auto view_model_handle = local->m_hViewModel();
		if (!view_model_handle.IsValid())
			return;

		const auto view_model = static_cast<C_BaseViewModel*>(g_EntityList->GetClientEntityFromHandle(view_model_handle));
		if (!view_model)
			return;

		const auto view_model_weapon_handle = view_model->m_hWeapon();
		if (!view_model_weapon_handle.IsValid())
			return;

		const auto view_model_weapon = static_cast<C_BaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
		if (!view_model_weapon)
			return;

		if (k_weapon_info.count(view_model_weapon->m_Item().m_iItemDefinitionIndex()))
		{
			const auto override_model = k_weapon_info.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
			const auto override_model_index = g_MdlInfo->GetModelIndex(override_model);
			view_model->m_nModelIndex() = override_model_index;
			const auto world_model_handle = view_model_weapon->m_hWeaponWorldModel();

			if (!world_model_handle.IsValid())
				return;

			const auto world_model = static_cast<C_BaseWeaponWorldModel*>(g_EntityList->GetClientEntityFromHandle(world_model_handle));

			if (!world_model)
				return;

			world_model->m_nModelIndex() = override_model_index + 1;
		}
	}
}

namespace Preview
{
	std::string FindUrl(std::string paintName, std::string weapon)
	{
		std::ifstream inFile;
		inFile.open(".\\csgo\\scripts\\items\\items_game_cdn.txt");
		std::string line;
		std::string search = paintName.append("=");
		while (std::getline(inFile, line))
		{
			if (line.find(search, 0) != std::string::npos)
			{
				if (line.find(weapon, 0) != std::string::npos)
				{
					return line.substr(line.find(search)).erase(0, search.length());
				}
			}
		}
		return "nf";
	}

	std::string DownloadBytes(const char* const szUrl)
	{
		HINTERNET hOpen = NULL;
		HINTERNET hFile = NULL;
		char* lpBuffer = NULL;
		DWORD dwBytesRead = 0;
		char* data = 0;
		DWORD dataSize = 0;

		hOpen = InternetOpenA("Sanktum", NULL, NULL, NULL, NULL);
		if (!hOpen) return "";

		hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD, NULL);

		if (!hFile) {
			InternetCloseHandle(hOpen);
			return "";
		}

		std::string output;
		do {
			char buffer[2000];
			InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
			output.append(buffer, dwBytesRead);
		} while (dwBytesRead);

		InternetCloseHandle(hFile);
		InternetCloseHandle(hOpen);

		return output;
	}
}
#pragma once

#include "../../options.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include <set>
#include <string>
#include <map>
#include <d3dx9.h>

class Skins {
private:
	static auto is_knife(const int i) -> bool {
		return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
	}

	template <typename T>
	void PickCurrentWeapon(int* idx, int* vec_idx, std::vector<T> arr) {
		if (!g_LocalPlayer) return;
		if (!g_EngineClient->IsInGame()) return;
		auto weapon = g_LocalPlayer->m_hActiveWeapon();
		if (!weapon) return;
		short wpn_idx = weapon->m_Item().m_iItemDefinitionIndex();
		if (is_knife(wpn_idx)) {
			*idx = values::WeaponNamesFull.at(0).definition_index;
			*vec_idx = 0;
			return;
		}
		auto wpn_it = std::find_if(arr.begin(), arr.end(), [wpn_idx](const T& a) {
			return a.definition_index == wpn_idx;
			});
		if (wpn_it != arr.end()) {
			*idx = wpn_idx;
			*vec_idx = std::abs(std::distance(arr.begin(), wpn_it));
		}
	}
public:
	struct skinInfo {
		int seed = -1;
		int paintkit;
		int rarity;
		std::string tagName;
		std::string     shortname; // shortname
		std::string     name;      // full skin name
	};
public:
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skinInfo> skinMap;
	std::unordered_map<std::string, std::string> skinNames;
private:
	int weapon_index_skins = 7;
	int weapon_vector_index_skins = 0;
private:
	struct item_setting {
		char name[32] = "";
		int definition_vector_index = 0;
		int definition_index = 1;
		int paint_kit_vector_index = 0;
		int paint_kit_index = 0;
		int definition_override_vector_index = 0;
		int definition_override_index = 0;
		int seed = 0;
		bool enable_stat_track = false;
		int stat_trak = 0;
		float wear = 0.0f;
	};
	struct options_t {
		std::map<int, item_setting> Items;
		std::unordered_map<std::string, std::string> IconOverrides;
	};
	options_t options;
private:
	auto get_icon_override(const std::string original) const -> const char* {
		return options.IconOverrides.count(original) ? options.IconOverrides.at(original).data() : nullptr;
	};
private:
	void erase_override_if_exists_by_index(const int definition_index);
	void apply_config_on_attributable_item(C_BaseAttributableItem* item, const item_setting* config, const unsigned xuid_low);
public:
	void OnFrameStageNotify(bool frame_end);
public:
	void Menu();
	void SetupValues();
};

extern Skins* g_SkinChanger;
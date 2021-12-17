#pragma once
#include "../features.hpp"

bool is_pistol(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_GLOCK, WEAPON_ELITE, WEAPON_P250, WEAPON_TEC9, WEAPON_CZ75A, WEAPON_USP_SILENCER, WEAPON_HKP2000, WEAPON_FIVESEVEN };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_heavy_pistol(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_DEAGLE, WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_sniper(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_AWP, WEAPON_G3SG1, WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_scout(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_SSG08 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_smg(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_MAC10, WEAPON_MP7, WEAPON_UMP45, WEAPON_P90, WEAPON_BIZON, WEAPON_MP9, WEAPON_MP5SD };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_heavy(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_NOVA, WEAPON_XM1014, WEAPON_SAWEDOFF, WEAPON_M249, WEAPON_NEGEV, WEAPON_MAG7 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_knife(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_KNIFE, WEAPON_KNIFEGG, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GHOST, WEAPON_KNIFE_GUT, WEAPON_KNIFE_GYPSY_JACKKNIFE, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_STILETTO, WEAPON_KNIFE_T, WEAPON_KNIFE_TACTICAL, WEAPON_KNIFE_URSUS, WEAPON_KNIFE_WIDOWMAKER, WEAPON_BAYONET };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_grenade(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_FRAG_GRENADE, WEAPON_HEGRENADE, WEAPON_INCGRENADE, WEAPON_SMOKEGRENADE, WEAPON_TAGRENADE, WEAPON_MOLOTOV, WEAPON_FLASHBANG };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_bomb(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_C4 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_rifle(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,WEAPON_M249,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}
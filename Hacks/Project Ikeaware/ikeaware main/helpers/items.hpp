#pragma once

#include <map>

struct weapon_info
{
	constexpr weapon_info(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}
	const char* model;
	const char* icon;
};

extern const std::map<short, weapon_info> k_weapon_info;
extern const std::map<short, const char*> k_glove_names;
extern const std::map<short, const char*> k_knife_names;
extern const std::map<short, const char*> k_item_names;
extern const std::map<short, const char*> k_inventory_names;
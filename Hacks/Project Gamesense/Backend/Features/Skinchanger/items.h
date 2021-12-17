#pragma once
#include <map>
#include <vector>

namespace game_data
{
	struct weapon_info
	{
		constexpr weapon_info(const char* model, const char* icon = nullptr) :
			model(model),
			icon(icon)
		{}

		const char* model;
		const char* icon;
	};

	struct weapon_name
	{
		constexpr weapon_name(const int definition_index, const char* name) :
			definition_index(definition_index),
			name(name)
		{}

		int definition_index = 0;
		const char* name = nullptr;
	};

	struct quality_name
	{
		constexpr quality_name(const int index, const char* name) :
			index(index),
			name(name)
		{}

		int index = 0;
		const char* name = nullptr;
	};

	const weapon_info* get_weapon_info(int defindex);
	extern const std::vector<weapon_name> knife_names;
	extern const std::vector<weapon_name> glove_names;
	extern const std::vector<weapon_name> weapon_names;
	extern const std::vector<quality_name> quality_names;
}
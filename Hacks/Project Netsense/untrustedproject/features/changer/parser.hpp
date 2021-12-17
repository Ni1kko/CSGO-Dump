#pragma once
#include <vector>
#include <string>

namespace game_data
{
	struct paint_kit
	{
		int id;
		std::string name;

		auto operator < (const paint_kit& other) const -> bool
		{
			return name < other.name;
		}
	};

	extern std::vector<paint_kit> skin_kits;
	extern std::vector<paint_kit> glove_kits;
	extern std::vector<paint_kit> sticker_kits;

	extern auto initialize_kits() -> void;
	extern int ParseSkins();
}
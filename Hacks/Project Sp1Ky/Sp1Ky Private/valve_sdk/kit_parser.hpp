#pragma once
#include "../Helpers/Utils.hpp"
#include <vector>
struct paint_kit
{
	int Skinchanger;
	std::string name;
	auto operator < (const paint_kit& other) const -> bool
	{
		return name < other.name;
	}
};
extern std::vector<paint_kit> k_skins;
extern std::vector<paint_kit> k_gloves;
extern auto initialize_kits() -> void;

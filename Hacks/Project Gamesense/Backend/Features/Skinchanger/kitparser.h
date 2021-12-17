#pragma once
#include <vector>
#include "../../singleton.h"

struct paint_kit_t {
	int id;
	std::string name;

	auto operator < (const paint_kit_t& other) const -> bool {
		return name < other.name;
	}
};

class c_kitparser : public singleton<c_kitparser> {
	std::vector<paint_kit_t> skin_kits;
	std::vector<paint_kit_t> glove_kits;
	std::vector<paint_kit_t> sticker_kits;

	auto initialize_kits() -> void;
};
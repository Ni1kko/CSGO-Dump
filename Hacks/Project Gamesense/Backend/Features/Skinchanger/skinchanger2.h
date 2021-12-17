#pragma once

#include "../../Include/GlobalIncludes.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/handle.h"
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../Visuals/Visuals.h"
#include "../../SDK/player_info_t.h"
#include "item_definitions.hpp"
#include "Memory.h"
#include <fstream>

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class GameEvent;
extern Memory memory;

namespace SkinChanger
{
	void run(ClientFrameStage_t stage) noexcept;
	void scheduleHudUpdate() noexcept;
	void overrideHudIcon(IGameEvent* event) noexcept;
	void setup_skins();

	struct PaintKit
	{
		int id;
		std::string name;
		std::string skin_name;

		PaintKit(int id, std::string&& name, std::string&& skin_name) noexcept : id(id), name(name), skin_name(skin_name)
		{

		}

		auto operator<(const PaintKit& other) const noexcept
		{
			return name < other.name;
		}
	};

	extern std::unordered_map <std::string, int> model_indexes;
	extern std::unordered_map <std::string, int> player_model_indexes;

	extern std::vector <PaintKit> skinKits;
	extern std::vector <PaintKit> gloveKits;
	extern std::vector <PaintKit> displayKits;

	extern bool updating_skins;

	extern std::unordered_map <std::string_view, const char*> iconOverrides;
}

struct item_setting
{
	void update()
	{
		itemId = game_data::weapon_names[itemIdIndex].definition_index;
		quality = game_data::quality_names[entity_quality_vector_index].index;

		const std::vector <SkinChanger::PaintKit>* kit_names;
		const game_data::weapon_name* defindex_names;

		if (itemId == GLOVE_T_SIDE)
		{
			kit_names = &SkinChanger::gloveKits;
			defindex_names = game_data::glove_names;
		}
		else
		{
			kit_names = &SkinChanger::skinKits;
			defindex_names = game_data::knife_names;
		}

		paintKit = (*kit_names)[paint_kit_vector_index].id;
		definition_override_index = defindex_names[definition_override_vector_index].definition_index;
		skin_name = (*kit_names)[paint_kit_vector_index].skin_name;
	}

	int itemIdIndex = 0;
	int itemId = 1;
	int entity_quality_vector_index = 0;
	int quality = 0;
	int paint_kit_vector_index = 0;
	int paintKit = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	float wear = 0.0f;
	char custom_name[24] = "\0";
	std::string skin_name;
};

item_setting* get_by_definition_index(const int definition_index);

struct Skins_t
{
	bool rare_animations;
	std::map<int, item_setting> skinChanger;
	std::string custom_name_tag[36];
};

namespace G
{
	extern Skins_t skins;
}
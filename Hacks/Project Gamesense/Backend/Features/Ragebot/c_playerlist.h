#pragma once
#include <vector>

#include "../../singleton.h"
#include "../../Configuration/Config.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/player_info_t.h"

class c_playerlist : public singleton<c_playerlist> {
public:
	bool should_disable_resolver(CBaseEntity* ent);
	bool should_disable_aimbot(CBaseEntity* ent);
	bool should_disable_visuals(CBaseEntity* ent);
	bool should_prefer_body(CBaseEntity* ent);

	void apply_for_player(CBaseEntity* ent);

	std::vector<player_info_t> get_players();
	std::string get_steam_id(std::vector<player_info_t> players, int userid);
};
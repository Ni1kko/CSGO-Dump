#include "c_playerlist.h"

std::vector<player_info_t> c_playerlist::get_players() {
	std::vector<player_info_t> players;

	if (!Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected())
		return players;

	for (int i = 0; i < 65; i++) {
		auto e = Interfaces::EntityList->GetClientEntity(i);

		if (!e)
			continue;
		if (e == G::LocalPlayer)
			continue;
		if (e->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["plist_everyone"])
			continue;

		player_info_t pinfo;
		Interfaces::Engine->GetPlayerInfo(i, &pinfo);

		if (std::string(pinfo.szSteamID) == "")
			continue;

		players.push_back(pinfo);
	}

	return players;
}

bool c_playerlist::should_disable_resolver(CBaseEntity* ent) {
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &pinfo);

	auto key = "plist_" + std::string(pinfo.fakeplayer ? pinfo.szName : pinfo.szSteamID) + "_";
	auto cfg = c_config::get();

	return cfg->b[key + "noresolver"];
}

bool c_playerlist::should_disable_aimbot(CBaseEntity* ent) {
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &pinfo);

	auto key = "plist_" + std::string(pinfo.fakeplayer ? pinfo.szName : pinfo.szSteamID) + "_";
	auto cfg = c_config::get();

	return cfg->b[key + "friend"];
}

bool c_playerlist::should_disable_visuals(CBaseEntity* ent) {
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &pinfo);
	auto key = "plist_" + std::string(pinfo.fakeplayer ? pinfo.szName : pinfo.szSteamID) + "_";
	auto cfg = c_config::get();

	return cfg->b[key + "novis"];
}

bool c_playerlist::should_prefer_body(CBaseEntity* ent) {
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &pinfo);

	auto key = "plist_" + std::string(pinfo.fakeplayer ? pinfo.szName : pinfo.szSteamID) + "_";
	auto cfg = c_config::get();

	return cfg->b[key + "baim"];
}

void c_playerlist::apply_for_player(CBaseEntity* ent) {
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &pinfo);

	if (pinfo.szSteamID == std::string(""))
		return;

	auto key = "plist_" + std::string(pinfo.fakeplayer ? pinfo.szName : pinfo.szSteamID) + "_";
	auto cfg = c_config::get();

	if (cfg->b[key + "pitch"])
		ent->GetEyeAnglesPtr()->x = cfg->i[key + "pitchval"];

	if (cfg->b[key + "yaw"]) {
		switch (cfg->i[key + "yawtype"]) {
		case 0:
			//ent->GetEyeAnglesPtr()->y += cfg->i[key + "yawval"]; // add
			break;
		case 1:
			ent->GetEyeAnglesPtr()->y = cfg->i[key + "yawval"]; // set
			break;
		case 2:
			//ent->GetEyeAnglesPtr()->y -= cfg->i[key + "yawval"]; // subtract
			break;
		}
	}
}

std::string c_playerlist::get_steam_id(std::vector<player_info_t> players, int userid) {
	for (auto p : players) {
		if (p.userId == userid) {
			if (p.fakeplayer)
				return p.szName;

			return p.szSteamID;
		}
	}

	return "UNKNOWN";
}
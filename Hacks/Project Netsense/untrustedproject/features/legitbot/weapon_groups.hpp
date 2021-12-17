#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../helpers/json.hpp"
#include "../../options.hpp"
#include <map>


enum DNDropTarget_ {
	DNDropTarget_Delete = 0,
	DNDropTarget_New,
	DNDropTarget_CreateGroup,
	DNDropTarget_MoveToAnotherGroup
};


struct weapon_setts_t {
	std::vector<short> def_indexes;
	legitbot_settings settings;
	bool lock; // if it's true you can't edit weapon group while it's true
};

class CCustomWeaponGroups {
private:
	// we will store each group in this map 
	//         name         settings(like a def_indexes for weapons which will be applied and settings for this set of weapons)
	std::map<std::string, weapon_setts_t> setts;
public:
	void RenderSelector(std::string& selected, float customSpasing = 0.f, std::function<void(std::string nd)> callback = [&](std::string nd) -> void {});
	void Menu(std::string& selected);
public:
	void LoadSettings(nlohmann::json& data);
	void SaveSettings(nlohmann::json& data);
public:
	legitbot_settings* GetSettings(short idx);
	legitbot_settings* GetSettings(std::string name);
	weapon_setts_t GetGroupSettings(short idx);
	weapon_setts_t GetGroupSettings(std::string name);
	std::string GetGroupName(short idx);
public:
	void FillDefaults();
	CCustomWeaponGroups();
};


extern CCustomWeaponGroups* g_CustomWeaponGroups;
#pragma once

#include <d3dx9.h>

#include "..//valve_sdk/sdk.hpp"
#include "..//valve_sdk/csgostructs.hpp"

class Skins
{
public:
	std::unordered_map<std::string, std::string> m_icon_overrides = { };

	Skins() {}

	void ParseSkins() const;
	void FrameStageNotify(bool frame_end);
	const char* GetIconOverride(const std::string& original);
	void EraseOverrideIfExistsByIndex(short definition_index);
	bool ApplyCustomSkin(C_BasePlayer* localPlayer, C_BaseAttributableItem* pWeapon, short nWeaponIndex);
};

inline Skins* g_Skins;

namespace Preview
{
	std::string FindUrl(std::string paintName, std::string weapon);
	std::string DownloadBytes(const char* const szUrl);
}
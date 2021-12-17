#pragma once
#include "../../utilities/includes.h"

class CVisuals : public CFeature
{
public:
	void onMove(float sampleTime, CUserCmd* userCmd) override;
	void onFrameStage(clientFrameStage_t frameStage) override;
	void onPaint(unsigned int panel) override;
	void onEvent(IGameEvent* pEvent) override;

	void glow();
	void historyHitbox(int entity);
	void inaccuracyOverlay();

	//void drawAntiaimIndicator( bool hotkey );

private:
	bool bNightModulationComplete = false;
	bool bNightModulationRemoveComplete = false;

	bool bFullBrightComplete = false;
	bool bFullBrightRemoveComplete = false;

	struct
	{
		//int left, right, top, bottom;
		int x, y, w, h;
	} Box;

	bool boundingBox(CBaseEntity* pEntity, Vector3 offset = Vector3(0, 0, 0));

	void playerBox(CBaseEntity* pEntity);
	void playerHealth(CBaseEntity* pEntity);
	void playerName(CBaseEntity* pEntity, int index);
	void playerWeapon(CBaseEntity* pEntity);
	void playerAmmo(CBaseEntity* pEntity);
	void playerFlags(CBaseEntity* pEntity);

	void playerBones(CBaseEntity* pEntity);
	void playerHistoryBones(CBaseEntity* pEntity);

	void worldWeapon(CBaseEntity* pEntity);

	void localViewModelSpoof();
	void localViewModelCM();
	void noScopeLines();

	void nightModulation();
	void removeNight();
	void applyFullBright();
	void removeFullBright();
	void outOfPovArrows(CBaseEntity* pEntity);

	static unsigned int getFont(unsigned int defaultFont) {
		return config->get_bool("espType") == 0 ? defaultFont : g_Fonts->visualsAlternate;
	}

	std::unordered_map< int, const char* > weaponIcon = {
{ weapon_deagle, "F" },
{ weapon_duals, "S" },
{ weapon_five7, "U" },
{ weapon_glock, "C" },
{ weapon_ak47, "B" },
{ weapon_aug, "E" },
{ weapon_awp, "R" },
{ weapon_famas, "T" },
{ weapon_g3sg1, "I" },
{ weapon_galil, "V" },
{ weapon_m249, "Z" },
{ weapon_m4a1, "W" },
{ weapon_mac10, "L" },
{ weapon_p90, "M" },
{ weapon_ump45, "Q" },
{ weapon_xm1014, "]" },
{ weapon_bizon, "D" },
{ weapon_mag7, "K" },
{ weapon_negev, "Z" },
{ weapon_sawedoff, "K" },
{ weapon_tec9, "C" },
{ weapon_taser, "Y" },
{ weapon_p2000, "Y" },
{ weapon_mp7, "X" },
{ weapon_mp9, "D" },
{ weapon_nova, "K" },
{ weapon_p250, "Y" },
{ weapon_scar20, "I" },
{ weapon_sg553, "[" },
{ weapon_scout, "N" },
{ weapon_knife_ct, "J" },
{ weapon_flash, "G" },
{ weapon_he, "H" },
{ weapon_smoke, "P" },
{ weapon_molotov, "P" },
{ weapon_decoy, "G" },
{ weapon_inc, "P" },
{ weapon_c4, "\\" },
{ weapon_knife_t, "J" },
{ weapon_m4a1s, "W" },
{ weapon_usps, "Y" },
{ weapon_knife_bayonet, "J" },
{ weapon_knife_flip, "J" },
{ weapon_knife_gut, "J" },
{ weapon_knife_karambit, "J" },
{ weapon_knife_m9bayonet, "J" },
{ weapon_knife_huntsman, "J" },
{ weapon_knife_falchion, "J" },
{ weapon_knife_bowie, "J" },
{ weapon_knife_butterfly, "J" },
{ weapon_knife_dagger, "J" }
	};

	std::string weaponToIcon(const int id)
	{
		auto search = weaponIcon.find(id);
		if (search != weaponIcon.end())
			return std::string(search->second, 1);

		return "";
	}

}; extern CVisuals g_Visuals;
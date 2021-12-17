#include <ShlObj.h>
#include <filesystem>
#include "options.hpp"
#include "valve_sdk/misc/Enums.hpp"
#include "valve_sdk/sdk.hpp"
#include "features/inventory.hpp"

const std::map<int, const char*> config_names = {
	{WEAPON_CZ75A, "CZ75 Auto"},
	{WEAPON_DEAGLE, "Desert Eagle"},
	{WEAPON_ELITE, "Dual Berettas"},
	{WEAPON_FIVESEVEN, "Five-SeveN"},
	{WEAPON_HKP2000, "P2000"},
	{WEAPON_P250, "P250"},
	{WEAPON_USP_SILENCER, "USP-S"},
	{WEAPON_TEC9, "Tec-9"},
	{WEAPON_REVOLVER, "R8 Revolver"},
	{WEAPON_GLOCK, "Glock-18"},

	{WEAPON_MAG7, "MAG-7"},
	{WEAPON_NOVA, "Nova"},
	{WEAPON_SAWEDOFF, "Sawed-Off"},
	{WEAPON_XM1014, "XM1014"},

	{WEAPON_UMP45, "UMP-45"},
	{WEAPON_P90, "P90"},
	{WEAPON_BIZON, "PP-Bizon"},
	{WEAPON_MAC10, "MAC-10"},
	{WEAPON_MP7, "MP7"},
	{WEAPON_MP9, "MP9"},
	{WEAPON_MP5, "MP5-SD"},

	{WEAPON_M249, "M249"},
	{WEAPON_NEGEV, "Negev"},

	{WEAPON_AK47, "AK-47"},
	{WEAPON_AUG, "AUG"},
	{WEAPON_GALILAR, "Galil AR"},
	{WEAPON_M4A1_SILENCER, "M4A1-S"},
	{WEAPON_M4A1, "M4A4"},
	{WEAPON_SG556, "SG 553"},
	{WEAPON_FAMAS, "FAMAS"},

	{WEAPON_AWP, "AWP"},
	{WEAPON_G3SG1, "G3SG1"},
	{WEAPON_SCAR20, "SCAR-20"},
	{WEAPON_SSG08, "SSG 08"},

	{WEAPON_KNIFE, "Knife"},
	{WEAPON_KNIFE_T, "TKnife"},

	{GLOVE_T_SIDE, "Glove"},
};

void Options::SetupValue(int& value, std::string category, std::string name)
{
	ints.push_back(new ConfigValue<int>(std::move(category), std::move(name), &value));
}

void Options::SetupValue(bool& value, std::string category, std::string name)
{
	bools.push_back(new ConfigValue<bool>(std::move(category), std::move(name), &value));
}

void Options::SetupValue(float& value, std::string category, std::string name)
{
	floats.push_back(new ConfigValue<float>(std::move(category), std::move(name), &value));
}

void Options::SetupColor(Color& value, const std::string& name)
{
	SetupValue(value._CColor[0], "Colors", name + "_r");
	SetupValue(value._CColor[1], "Colors", name + "_g");
	SetupValue(value._CColor[2], "Colors", name + "_b");
	SetupValue(value._CColor[3], "Colors", name + "_a");
}

void Options::SetupRagebot()
{
	SetupValue(g_Options.ragebotconfig.bEnabled, "Ragebot", "Enabled");
	SetupValue(g_Options.ragebotconfig.iTargetSelection, "Ragebot", "Target selection");
	SetupValue(g_Options.ragebotconfig.iMaxFOV, "Ragebot", "Maximum FOV");
	SetupValue(g_Options.ragebotconfig.bFriendlyFire, "Ragebot", "Friendly fire");
	SetupValue(g_Options.ragebotconfig.bAutoFire, "Ragebot", "Autofire");
	SetupValue(g_Options.ragebotconfig.bPreferBaim, "Ragebot", "Baim when enemy in air");
	SetupValue(g_Options.ragebotconfig.bAutoWall, "Ragebot", "Automatic penetrate");
	SetupValue(g_Options.ragebotconfig.iAimbotMode, "Ragebot", "Aimbot type");
	SetupValue(g_Options.ragebotconfig.bHitchance, "Ragebot", "Hitchance");
	SetupValue(g_Options.ragebotconfig.flHitchanceAmt, "Ragebot", "Hitchance value");
	SetupValue(g_Options.ragebotconfig.flMinDmg, "Ragebot", "Minimum damage value");
	SetupValue(g_Options.ragebotconfig.bAutoScope, "Ragebot", "Autoscope");
	SetupValue(g_Options.ragebotconfig.flBaimOnX, "Ragebot", "Baim when enemy xp < x");
	SetupValue(g_Options.ragebotconfig.bBaimIfDeadly, "Ragebot", "Baim if lethal");
	SetupValue(g_Options.ragebotconfig.bAutostop, "Ragebot", "Autostop");
	SetupValue(g_Options.ragebotconfig.bRemoveRecoil, "Ragebot", "Norecoil");
	SetupValue(g_Options.ragebotconfig.head, "Ragebot", "Scan head");
	SetupValue(g_Options.ragebotconfig.chest, "Ragebot", "Scan chest");
	SetupValue(g_Options.ragebotconfig.body, "Ragebot", "Scan body");
	SetupValue(g_Options.ragebotconfig.hands, "Ragebot", "Scan hands");
	SetupValue(g_Options.ragebotconfig.legs, "Ragebot", "Scan legs");
	SetupValue(g_Options.ragebotconfig.headmp, "Ragebot", "Pointscale head");
	SetupValue(g_Options.ragebotconfig.chestmp, "Ragebot", "Pointscale chest");
	SetupValue(g_Options.ragebotconfig.bodymp, "Ragebot", "Pointscale body");
	SetupValue(g_Options.ragebotconfig.handsmp, "Ragebot", "Pointscale hands");
	SetupValue(g_Options.ragebotconfig.legsmp, "Ragebot", "Pointscale legs");
	SetupValue(g_Options.ragebotconfig.pitch, "Ragebot", "Pitch");
	SetupValue(g_Options.ragebotconfig.yaw, "Ragebot", "Yaw");
	SetupValue(g_Options.ragebotconfig.jittermin, "Ragebot", "Jitter min");
	SetupValue(g_Options.ragebotconfig.jittermax, "Ragebot", "Jitter max");
	SetupValue(g_Options.ragebotconfig.desync, "Ragebot", "Desync Animations");
	SetupValue(g_Options.ragebotconfig.desynctype, "Ragebot", "Desync Animations Type");
	SetupValue(g_Options.ragebotconfig.bodylean, "Ragebot", "Body Lean");
	SetupValue(g_Options.ragebotconfig.invbodylean, "Ragebot", "Inverted Body Lean");
	SetupValue(g_Options.ragebotconfig.switchdesyncside, "Ragebot", "Desync Switch Hotkey");
	SetupValue(g_Options.ragebotconfig.slowwalk, "Ragebot", "Slow Walk");
	SetupValue(g_Options.ragebotconfig.slowwalkspeed, "Ragebot", "Slow Walk Speed");
	SetupValue(g_Options.ragebotconfig.slowwalkkey, "Ragebot", "Slow Walk Hotkey");
}


void Options::SetupWeapons()
{
	for (auto& [val, key] : config_names)
	{
		auto aimbot = &g_Options.weapons[val].legit;
		auto skins = &g_Options.weapons[val].skins;

		if (val != WEAPON_KNIFE && val != WEAPON_KNIFE_T && val != GLOVE_T_SIDE)
		{
			SetupValue(aimbot->enabled, key, "Enabled");
			SetupValue(aimbot->deathmatch, key, "Deathmatch");
			SetupValue(aimbot->silent, key, "pSilent");
			SetupValue(aimbot->flash_check, key, "Flash Check");
			SetupValue(aimbot->smoke_check, key, "Smoke Check");
			SetupValue(aimbot->autopistol, key, "AutoPistol");

			SetupValue(aimbot->fov, key, "Fov");
			SetupValue(aimbot->silent_fov, key, "pSilent Fov");
			SetupValue(aimbot->smooth, key, "Smooth");

			SetupValue(aimbot->kill_delay, key, "Kill Delay");
			SetupValue(aimbot->shot_delay, key, "Shot Delay");

			SetupValue(aimbot->hitboxes.head, key, "Head Hitbox");
			SetupValue(aimbot->hitboxes.chest, key, "Chest Hitbox");
			SetupValue(aimbot->hitboxes.hands, key, "Hands Hitbox");
			SetupValue(aimbot->hitboxes.legs, key, "Legs Hitbox");

			SetupValue(aimbot->rcs.enabled, key, "RCS Enabled");
			SetupValue(aimbot->rcs.start, key, "RCS Start");
			SetupValue(aimbot->rcs.type, key, "RCS Type");
			SetupValue(aimbot->rcs.x, key, "RCS X");
			SetupValue(aimbot->rcs.y, key, "RCS Y");

			SetupValue(aimbot->autowall.enabled, key, "AutoWall Enabled");
			SetupValue(aimbot->autowall.min_damage, key, "AutoWall MinDamage");

			SetupValue(aimbot->autofire.enabled, key, "AutoFire Enabled");
			SetupValue(aimbot->autofire.hotkey, key, "AutoFire Hotkey");

			SetupValue(skins->t.enabled, key, "SC Enabled");
		}

		SetupValue(skins->t.definitionIndex, key, "SC DefinitionIndex");
		SetupValue(skins->t.paintKit, key, "SC PaintKit");
		SetupValue(skins->t.seed, key, "SC Seed");
		SetupValue(skins->t.wear, key, "SC Wear");
		SetupValue(skins->t.quality, key, "SC Quality");
	}
}

void Options::SetupVisuals()
{
	SetupValue(g_Options.esp.player_enemies, "ESP", "Enemy");
	SetupValue(g_Options.esp.player_teammates, "ESP", "Ally");
	SetupValue(g_Options.esp.player_local, "ESP", "Local");
	SetupValue(g_Options.esp.player_enemies_occluded, "ESP", "Enemy Occluded");
	SetupValue(g_Options.esp.player_teammates_occluded, "ESP", "Ally Occluded");
	SetupValue(g_Options.esp.player_local_occluded, "ESP", "Local Occluded");

	SetupValue(g_Options.esp.player_boxes_enemies, "ESP", "Boxes Enemy");
	SetupValue(g_Options.esp.player_boxes_teammates, "ESP", "Boxes Ally");
	SetupValue(g_Options.esp.player_boxes_local, "ESP", "Boxes Local");

	SetupValue(g_Options.esp.player_names_enemies, "ESP", "Names Enemy");
	SetupValue(g_Options.esp.player_names_teammates, "ESP", "Names Ally");
	SetupValue(g_Options.esp.player_names_local, "ESP", "Names Local");

	SetupValue(g_Options.esp.player_health_enemies, "ESP", "Health Enemy");
	SetupValue(g_Options.esp.player_health_teammates, "ESP", "Health Ally");
	SetupValue(g_Options.esp.player_health_local, "ESP", "Health Local");

	SetupValue(g_Options.esp.player_armour_enemies, "ESP", "Armour Enemy");
	SetupValue(g_Options.esp.player_armour_teammates, "ESP", "Armour Ally");
	SetupValue(g_Options.esp.player_armour_local, "ESP", "Armour Local");

	SetupValue(g_Options.esp.player_weapons_enemies, "ESP", "Weapons Enemy");
	SetupValue(g_Options.esp.player_weapons_teammates, "ESP", "Weapons Ally");
	SetupValue(g_Options.esp.player_weapons_local, "ESP", "Weapons Local");

	SetupValue(g_Options.esp.sound_enemies, "ESP", "Sounds Enemy");
	SetupValue(g_Options.esp.sound_teammates, "ESP", "Sounds Ally");
	SetupValue(g_Options.esp.sound_local, "ESP", "Sounds Local");





	SetupValue(g_Options.esp.dropped_weapons, "ESP", "Dropped Weapons");
	SetupValue(g_Options.esp.defuse_kit, "ESP", "Defuse Kits");
	SetupValue(g_Options.esp.planted_c4, "ESP", "Planted C4");
	SetupValue(g_Options.esp.items, "ESP", "Items");

	SetupValue(g_Options.glow.enemies, "Glow", "Enemy");
	SetupValue(g_Options.glow.enemies_type, "Glow", "Enemy Type");
	SetupValue(g_Options.glow.teammates, "Glow", "Ally");
	SetupValue(g_Options.glow.teammates_type, "Glow", "Ally Type");
	SetupValue(g_Options.glow.local, "Glow", "Local");
	SetupValue(g_Options.glow.local_type, "Glow", "Local Type");

	SetupValue(g_Options.glow.chickens, "Glow", "Chickens");
	SetupValue(g_Options.glow.chickens_type, "Glow", "Chickens Type");
	SetupValue(g_Options.glow.planted_c4, "Glow", "Planted C4");
	SetupValue(g_Options.glow.planted_c4_type, "Glow", "Planted C4 Type");
	SetupValue(g_Options.glow.weapons, "Glow", "Weapons");
	SetupValue(g_Options.glow.weapons_type, "Glow", "Weapons Type");

	SetupValue(g_Options.chams.player_enemies, "Chams", "Player Enemy");
	SetupValue(g_Options.chams.player_enemies_shine, "Chams", "Player Enemy Shine");
	SetupValue(g_Options.chams.player_enemies_type, "Chams", "Player Enemy Type");

	SetupValue(g_Options.chams.player_enemies_occluded, "Chams", "Player Enemy XQZ");
	SetupValue(g_Options.chams.player_enemies_occluded_shine, "Chams", "Player Enemy XQZ Shine");
	SetupValue(g_Options.chams.player_enemies_occluded_type, "Chams", "Player Enemy XQZ Type");

	SetupValue(g_Options.chams.player_teammates, "Chams", "Player Ally");
	SetupValue(g_Options.chams.player_teammates_shine, "Chams", "Player Ally Shine");
	SetupValue(g_Options.chams.player_teammates_type, "Chams", "Player Ally Type");

	SetupValue(g_Options.chams.player_teammates_occluded, "Chams", "Player Ally XQZ");
	SetupValue(g_Options.chams.player_teammates_occluded_shine, "Chams", "Player Ally XQZ Shine");
	SetupValue(g_Options.chams.player_teammates_occluded_type, "Chams", "Player Ally XQZ Type");

	SetupValue(g_Options.chams.player_local, "Chams", "Player Local");
	SetupValue(g_Options.chams.player_local_shine, "Chams", "Player Local Shine");
	SetupValue(g_Options.chams.player_local_type, "Chams", "Player Local Type");

	SetupValue(g_Options.chams.player_local_occluded, "Chams", "Player Local XQZ");
	SetupValue(g_Options.chams.player_local_occluded_shine, "Chams", "Player Local XQZ Shine");
	SetupValue(g_Options.chams.player_local_occluded_type, "Chams", "Player Local XQZ Type");

	SetupValue(g_Options.chams.arms, "Chams", "Arms");
	SetupValue(g_Options.chams.arms_shine, "Chams", "Arms Shine");
	SetupValue(g_Options.chams.arms_type, "Chams", "Arms Type");

	SetupValue(g_Options.chams.arms_occluded, "Chams", "Arms XQZ");
	SetupValue(g_Options.chams.arms_occluded_shine, "Chams", "Arms XQZ Shine");
	SetupValue(g_Options.chams.arms_occluded_type, "Chams", "Arms XQZ Type");

	SetupValue(g_Options.chams.weapon, "Chams", "Weapon");
	SetupValue(g_Options.chams.weapon_shine, "Chams", "Weapon Shine");
	SetupValue(g_Options.chams.weapon_type, "Chams", "Weapon Type");

	SetupValue(g_Options.chams.weapon_occluded, "Chams", "Weapon XQZ");
	SetupValue(g_Options.chams.weapon_occluded_shine, "Chams", "Weapon XQZ Shine");
	SetupValue(g_Options.chams.weapon_occluded_type, "Chams", "Weapon XQZ Type");
}

void Options::SetupMisc()
{
	SetupValue(g_Options.profile.enabled, "Profile", "Enabled");
	SetupValue(g_Options.profile.rank_id, "Profile", "Rank");
	SetupValue(g_Options.profile.wins, "Profile", "Wins");
	SetupValue(g_Options.profile.player_level, "Profile", "Level");
	SetupValue(g_Options.profile.player_xp, "Profile", "XP");
	SetupValue(g_Options.profile.teaching, "Profile", "Teaching");
	SetupValue(g_Options.profile.friendly, "Profile", "Friendly");
	SetupValue(g_Options.profile.leader, "Profile", "Leader");

	SetupValue(g_Options.misc.removals.no_hands, "Misc", "No Hands");
	SetupValue(g_Options.misc.removals.no_sleeves, "Misc", "No Sleeves");
	SetupValue(g_Options.misc.removals.no_weapon, "Misc", "No Weapon");
	SetupValue(g_Options.misc.removals.no_smoke, "Misc", "No Smoke");
	SetupValue(g_Options.misc.removals.no_scope, "Misc", "No Scope");
	SetupValue(g_Options.misc.removals.no_zoom, "Misc", "No Zoom");
	SetupValue(g_Options.misc.removals.no_flash, "Misc", "No Flash");
	SetupValue(g_Options.misc.removals.no_blur, "Misc", "No Blur");
	SetupValue(g_Options.misc.removals.no_postprocess, "Misc", "No Postprocess");
	SetupValue(g_Options.misc.removals.no_fog, "Misc", "No Fog");
	SetupValue(g_Options.misc.removals.no_visual_recoil, "Misc", "No Visual Recoil");
	
	SetupValue(g_Options.misc.bhop, "Misc", "Bunny Hop");
	SetupValue(g_Options.misc.autostrafe, "Misc", "AutoStrafe");
	SetupValue(g_Options.misc.rank_reveal, "Misc", "Show Ranks");
	SetupValue(g_Options.misc.watermark, "Misc", "Watermark");
	SetupValue(g_Options.misc.fakefps, "Misc", "Fake fps");
	SetupValue(g_Options.misc.autoaccept, "Misc", "AutoAccept");
	SetupValue(g_Options.misc.nightmode, "Misc", "NightMode");
	SetupValue(g_Options.misc.sniper_xhair, "Misc", "Sniper Crosshair");
	SetupValue(g_Options.misc.buyinfo, "Misc", "Buy Info");
	SetupValue(g_Options.misc.spectator_list, "Misc", "Spectator List");

	SetupValue(g_Options.misc.thirdperson.hotkey, "Misc", "ThirdPerson Hotkey");

	SetupValue(g_Options.misc.fakelag.enabled, "Misc", "Fakelags Enabled");
	SetupValue(g_Options.misc.fakelag.onkey, "Misc", "Fakelags On Key");
	SetupValue(g_Options.misc.fakelag.hotkey, "Misc", "Fakelags Hotkey");
	SetupValue(g_Options.misc.fakelag.ticks, "Misc", "Fakelags Ticks");

	SetupValue(g_Options.misc.edgejump.enabled, "Misc", "Edgejump Enabled");
	SetupValue(g_Options.misc.edgejump.onkey, "Misc", "Edgejump On Key");
	SetupValue(g_Options.misc.edgejump.hotkey, "Misc", "Edgejump Hotkey");

	SetupValue(g_Options.misc.fov.override, "Misc", "World Fov");
	SetupValue(g_Options.misc.fov.viewmodel, "Misc", "ViewModel Fov");
	SetupValue(g_Options.misc.fov.viewmodel_offset_x, "Misc", "ViewModel Offset X");
	SetupValue(g_Options.misc.fov.viewmodel_offset_y, "Misc", "ViewModel Offset Y");
	SetupValue(g_Options.misc.fov.viewmodel_offset_z, "Misc", "ViewModel Offset Z");
}

void Options::SetupColors()
{
	SetupColor(g_Options.colors.esp.sounds_enemies, "ESP Sounds Enemy");
	SetupColor(g_Options.colors.esp.sounds_teammates, "ESP Sounds Ally");
	SetupColor(g_Options.colors.esp.sounds_local, "ESP Sounds Local");
	SetupColor(g_Options.colors.esp.weapons, "ESP Weapons");
	SetupColor(g_Options.colors.esp.defuse, "ESP Defuse Kits");
	SetupColor(g_Options.colors.esp.c4, "ESP C4");
	SetupColor(g_Options.colors.esp.item, "ESP Items");

	SetupColor(g_Options.colors.glow.ally, "Glow Ally");
	SetupColor(g_Options.colors.glow.enemy, "Glow Enemy");
	SetupColor(g_Options.colors.glow.local, "Glow Local");
	SetupColor(g_Options.colors.glow.chickens, "Glow Chickens");
	SetupColor(g_Options.colors.glow.planted_c4, "Glow Planted C4");
	SetupColor(g_Options.colors.glow.weapons, "Glow Weapons");

	SetupColor(g_Options.colors.chams.player_ally_visible, "Chams Ally Visible");
	SetupColor(g_Options.colors.chams.player_ally_visible_shine, "Chams Ally Visible Shine");
	SetupColor(g_Options.colors.chams.player_ally_occluded, "Chams Ally Occluded");
	SetupColor(g_Options.colors.chams.player_ally_occluded_shine, "Chams Ally Occluded Shine");

	SetupColor(g_Options.colors.chams.player_enemy_visible, "Chams Enemy Visible");
	SetupColor(g_Options.colors.chams.player_enemy_visible_shine, "Chams Enemy Visible Shine");
	SetupColor(g_Options.colors.chams.player_enemy_occluded, "Chams Enemy Occluded");
	SetupColor(g_Options.colors.chams.player_enemy_occluded_shine, "Chams Enemy Occluded Shine");

	SetupColor(g_Options.colors.chams.player_local_visible, "Chams Local Visible");
	SetupColor(g_Options.colors.chams.player_local_visible_shine, "Chams Local Visible Shine");
	SetupColor(g_Options.colors.chams.player_local_occluded, "Chams Local Occluded");
	SetupColor(g_Options.colors.chams.player_local_occluded_shine, "Chams Local Occluded Shine");

	SetupColor(g_Options.colors.chams.arms_visible, "Chams Arms Visible");
	SetupColor(g_Options.colors.chams.arms_visible_shine, "Chams Arms Visible Shine");
	SetupColor(g_Options.colors.chams.arms_occluded, "Chams Arms Occluded");
	SetupColor(g_Options.colors.chams.arms_occluded_shine, "Chams Arms Occluded Shine");

	SetupColor(g_Options.colors.chams.weapon_visible, "Chams Weapon Visible");
	SetupColor(g_Options.colors.chams.weapon_visible_shine, "Chams Weapon Visible Shine");
	SetupColor(g_Options.colors.chams.weapon_occluded, "Chams Weapon Occluded");
	SetupColor(g_Options.colors.chams.weapon_occluded_shine, "Chams Weapon Occluded Shine");
}

void Options::Initialize()
{
	CHAR my_documents[MAX_PATH];
	SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, my_documents);
	folder = my_documents + std::string("\\ikeaware\\");
	CreateDirectoryA(folder.c_str(), nullptr);
	SetupRagebot();
	SetupWeapons();
	SetupVisuals();
	SetupMisc();
	SetupColors();
}

void Options::SaveSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	for(auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	WritePrivateProfileStringA("InventoryChanger", "Count", std::to_string(g_Options.inventory.itemCount).c_str(), file.c_str());

	int i = 0;
	for(auto weapon : g_InventorySkins)
	{
		if (!weapon.second.wId || !weapon.second.paintKit)
			continue;

		std::string itemid = "inventory" + std::to_string(i) + "_id";
		std::string paintkitstr = "inventory" + std::to_string(i) + "_paintkit";
		std::string wearkitstr = "inventory" + std::to_string(i) + "_wear";
		std::string seedkitstr = "inventory" + std::to_string(i) + "_seed";
		std::string inusetstr = "inventory" + std::to_string(i) + "_inuset";
		std::string inusectstr = "inventory" + std::to_string(i) + "_inusect";

		WritePrivateProfileStringA("InventoryChanger", itemid.c_str(), std::to_string(weapon.second.wId).c_str(), file.c_str());
		WritePrivateProfileStringA("InventoryChanger", paintkitstr.c_str(), std::to_string(weapon.second.paintKit).c_str(), file.c_str());
		WritePrivateProfileStringA("InventoryChanger", wearkitstr.c_str(), std::to_string(weapon.second.wear).c_str(), file.c_str());
		WritePrivateProfileStringA("InventoryChanger", seedkitstr.c_str(), std::to_string(weapon.second.seed).c_str(), file.c_str());
		WritePrivateProfileStringA("InventoryChanger", inusetstr.c_str(), weapon.second.in_use_t ? "true" : "false", file.c_str());
		WritePrivateProfileStringA("InventoryChanger", inusectstr.c_str(), weapon.second.in_use_ct ? "true" : "false", file.c_str());
		i++;
	}
}

void Options::LoadSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	if (!std::filesystem::exists(file))
		return;

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = float(atof(value_l));
	}

	g_InventorySkins.clear();

	GetPrivateProfileStringA("InventoryChanger", "Count", "", value_l, 32, file.c_str());
	g_Options.inventory.itemCount = atoi(value_l);

	for (int i = 0; i < g_Options.inventory.itemCount; i++)
	{
		std::string itemid = "inventory" + std::to_string(i) + "_id";
		std::string paintkitstr = "inventory" + std::to_string(i) + "_paintkit";
		std::string wearkitstr = "inventory" + std::to_string(i) + "_wear";
		std::string seedkitstr = "inventory" + std::to_string(i) + "_seed";
		std::string inusetstr = "inventory" + std::to_string(i) + "_inuset";
		std::string inusectstr = "inventory" + std::to_string(i) + "_inusect";

		wskin skinInfo;

		GetPrivateProfileStringA("InventoryChanger", itemid.c_str(), "", value_l, 32, file.c_str());
		skinInfo.wId = atoi(value_l);
		GetPrivateProfileStringA("InventoryChanger", paintkitstr.c_str(), "", value_l, 32, file.c_str());
		skinInfo.paintKit = atoi(value_l);
		GetPrivateProfileStringA("InventoryChanger", wearkitstr.c_str(), "", value_l, 32, file.c_str());
		skinInfo.wear = atof(value_l);
		GetPrivateProfileStringA("InventoryChanger", seedkitstr.c_str(), "", value_l, 32, file.c_str());
		skinInfo.seed = atoi(value_l);
		GetPrivateProfileStringA("InventoryChanger", inusetstr.c_str(), "", value_l, 32, file.c_str());
		skinInfo.in_use_t = !strcmp(value_l, "true");
		GetPrivateProfileStringA("InventoryChanger", inusectstr.c_str(), "", value_l, 32, file.c_str());
		skinInfo.in_use_ct = !strcmp(value_l, "true");

		g_InventorySkins.insert({ RandomInt(20000, 200000), skinInfo });
	}

	Protobuf::SendClientHello();
	Protobuf::SendMatchmakingClient2GCHello();

	g_ClientState->ForceFullUpdate();
}

void Options::DeleteSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	if (!std::filesystem::exists(file))
		return;

	remove(file.c_str());
}
#include "setup.hpp"

#include "../menu.hpp"
#include "../controls/tab.hpp"
#include "../controls/group.hpp"
#include "../controls/checkbox.hpp"
#include "../controls/slider.hpp"
#include "../controls/combo.hpp"
#include "../controls/multicombo.hpp"
#include "../controls/hotkey.hpp"
#include "../controls/colorpicker.hpp"
#include "../controls/button.hpp"

void menu_setup::setup() {

	config->set("clr_menu", Color(225, 0, 255));
	config->set("colorGlow", Color(200, 80, 140)); // pink
	config->set("colorChamsEnemy", Color(130, 220, 90)); // green
	config->set("colorChamsEnemyXQZ", Color(35, 100, 160)); // aqua / light blue
	config->set("menu_bind", 45); // 45 = INSERT
	config->set("espThirdpersonKey", 84); // 84 = T
	config->set("miscFoV", 90);
	config->set("miscAspectRatioW", 1920);
	config->set("miscAspectRatioH", 1080);

	config->set("miscAntiUT", true);
	config->set("espWatermark", true);

	menu->set_position(Vector2(100, 100));
	menu->set_size(Vector2(500, 513));
	/*
	Size(x,y): if Menu size is 500 x 513 then a full group-box should be 223 x 436 | half group-box should be 223 x 221

	pCalculations: 500 - 223 = 277 | 513 - 436 = 77
	Size X - 277 for half menu (right to left)
	Size Y - 77 for tabs' spacing (bottom to top)

	if Menu size is 478 x 491 then a full group-box should be 212 x 414

	pCalculations: 478 - 212 = 266 | 491 - 414 = 77
	Size X - 266 for half menu (right to left)
	Size Y - 77 for tabs' spacing (bottom to top)

	Size X Alternative: 414 - 199 = 215, 436 - 215 = 221

	Tabs: Dont forget to change tabs' width and height at menu.cpp
	*/

	c_tab* legit_tab = new c_tab("Legit-bot");
	{
		menu->add_tab(legit_tab);

		c_group* main_group = new c_group("Main", Vector2(223, 436));
		{

		}
		legit_tab->add_child(main_group);
	}

	c_tab *rage_tab = new c_tab("Rage-bot");
	{
		menu->add_tab(rage_tab);

		c_group *general_group = new c_group("Aimbot", Vector2(223, 436));
		{
			general_group->add_child(new c_checkbox("Enable rage-bot", "rageEnable"));
			general_group->add_child(new c_checkbox("Automatic fire", "rageAutomaticFire"));
			general_group->add_child(new c_multicombo("Hitboxes", "rageHitboxes", { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" }));
			general_group->add_child(new c_checkbox("Avoid limbs if moving", "rageAvoidLimbsMoving"));
			general_group->add_child(new c_checkbox("Avoid head if jumping", "rageAvoidHeadJumping"));
			general_group->add_child(new c_multicombo("Multi-point", "rageMultipoints", { "Head", "Body" }));

			general_group->add_child(new c_slider("Head scale", "rageHeadScale", 1, 100, true, "%.0f%%"));
			general_group->add_child(new c_slider("Point scale", "ragePointScale", 1, 100, true, L"%.0f%%"));
			general_group->add_child(new c_checkbox("Hit chance", "rageHitchance"));
			general_group->add_child(new c_slider("Amount", "rageHitchanceValue", 0, 100, true, "%.0f%%"));
			general_group->add_child(new c_slider("Minimum damage", "rageMinDamageValue", 1, 100.f, true, "%.0fHP"));
		}
		rage_tab->add_child(general_group);

		c_group *other_group = new c_group("Other", Vector2(223, 210));
		{
			other_group->add_child(new c_hotkey("Body-aim key", "rageBodyAimKey", true));
			other_group->add_child(new c_checkbox("Force body-aim", "rageForceBodyAim"));

			other_group->add_child(new c_checkbox("Backtrack", "rageBacktrack"));
			other_group->add_child(new c_combo("Accuracy boost type", "rageBacktrackType", { "Last", "All" }));

			//other_group->add_child(new c_checkbox("Delay shot", "rageDelayShot"));
			other_group->add_child(new c_checkbox("Body-aim if lethal", "rageBodyAimLethal"));
			other_group->add_child(new c_checkbox("Body-aim if in air", "rageBodyAimAir"));
			other_group->add_child(new c_checkbox("Body-aim if missed shots", "rageBodyAimShots"));
			other_group->add_child(new c_slider("X missed shots", "rageBodyAimShotsAmount", 1, 10, true, L"%.0f%%"));
			other_group->add_child(new c_checkbox("Anti-aim correction", "rageAntiAimCorrection"));
			other_group->add_child(new c_checkbox("Automatic scope", "rageAutomaticScope"));
			other_group->add_child(new c_checkbox("Automatic stop", "rageAutomaticStop"));
			other_group->add_child(new c_combo("Type", "rageAutomaticStopType", { "Minimum speed", "Full stop" }));
		}
		rage_tab->add_child(other_group);

		c_group* fakelag_group = new c_group("Fake-lag", Vector2(223, 210));
		{
			fakelag_group->add_child(new c_checkbox("Enable fake-lag", "rageChokeEnable"));
			fakelag_group->add_child(new c_multicombo("Activation", "rageChokeActivation", { "Standing", "Moving", "In air", "Shooting" }));
			fakelag_group->add_child(new c_combo("Type", "rageChokeType", { "Factor", "Switch", "Adaptive", "Random" }));
			fakelag_group->add_child(new c_slider("Ticks", "rageChokeAmount", 1, 14, true, "%.0f"));

			fakelag_group->add_child(new c_hotkey("Fake duck key", "rageFakeDuckKey", true));
			fakelag_group->add_child(new c_checkbox("Fake duck", "rageFakeDuck"));
		}
		rage_tab->add_child(fakelag_group);

	}
	c_tab* antiaim_tab = new c_tab("Anti-aim");
	{
		menu->add_tab(antiaim_tab);

		c_group* antiaim_settings_group = new c_group("Settings", Vector2(223, 210));
		{
			antiaim_settings_group->add_child(new c_checkbox("Enable anti-aim", "aaEnable"));

			//antiaim_settings_group->add_child(new c_combo("Desync", "aaDesync", { "None", "test" }));
			antiaim_settings_group->add_child(new c_checkbox("Desync", "aaDesync"));

			antiaim_settings_group->add_child(new c_hotkey("Slow motion key", "aaSlowMotionKey", true));
			antiaim_settings_group->add_child(new c_checkbox("Slow motion", "aaSlowMotion"));

			antiaim_settings_group->add_child(new c_hotkey("Flip key", "aaFlipKey", false));
			//antiaim_settings_group->add_child(new c_checkbox("Flip", "aaFlip"));
		}
		antiaim_tab->add_child(antiaim_settings_group);

		c_group* standing_aa_group = new c_group("Standing", Vector2(223, 210));
		{
			standing_aa_group->add_child(new c_combo("Pitch", "aaStandingPitch", { "None", "Down", "Up", "Zero", "Jitter" }));
			standing_aa_group->add_child(new c_combo("Yaw", "aaStandingYaw", { "None", "Backwards", "Sideways" }));
			standing_aa_group->add_child(new c_combo("Jitter type", "aaStandingJitterType", { "None", "Offset", "Random" }));
			standing_aa_group->add_child(new c_slider("Jitter range", "aaStandingJitterRange", 1, 180, true, L"%.0f"));

			standing_aa_group->add_child(new c_checkbox("Auto direction", "aaStandingAutoDir"));
		}
		antiaim_tab->add_child(standing_aa_group);

		c_group* moving_aa_group = new c_group("Moving", Vector2(223, 210));
		{
			moving_aa_group->add_child(new c_combo("Pitch", "aaMovingPitch", { "None", "Down", "Up", "Zero", "Jitter" }));
			moving_aa_group->add_child(new c_combo("Yaw", "aaMovingYaw", { "None", "Backwards", "Sideways" }));
			moving_aa_group->add_child(new c_combo("Jitter type", "aaMovingJitterType", { "None", "Offset", "Random" }));
			moving_aa_group->add_child(new c_slider("Jitter range", "aaMovingJitterRange", 1, 180, true, L"%.0f"));

			moving_aa_group->add_child(new c_checkbox("Auto direction", "aaMovingAutoDir"));
		}
		antiaim_tab->add_child(moving_aa_group);

		c_group* inair_aa_group = new c_group("In-air", Vector2(223, 210));
		{
			inair_aa_group->add_child(new c_combo("Pitch", "aaInAirPitch", { "None", "Down", "Up", "Zero", "Jitter" }));
			inair_aa_group->add_child(new c_combo("Yaw", "aaInAirYaw", { "None", "Backwards", "Sideways" }));
			inair_aa_group->add_child(new c_combo("Jitter type", "aaInAirJitterType", { "None", "Offset", "Random" }));
			inair_aa_group->add_child(new c_slider("Jitter range", "aaInAirJitterRange", 1, 180, true, L"%.0f"));

			inair_aa_group->add_child(new c_checkbox("Auto direction", "aaInAirAutoDir"));
		}
		antiaim_tab->add_child(inair_aa_group);
	}
	c_tab *visuals_tab = new c_tab("Visuals");
	{
		menu->add_tab(visuals_tab);

		c_group *player_group = new c_group("Player ESP", Vector2(223, 230));
		{
			visuals_tab->add_child(player_group);
			player_group->add_child(new c_combo("Visuals type", "espType", { "Default", "Alternate" }));

			player_group->add_child(new c_checkbox("Enable visuals", "espEnable"));
			player_group->add_child(new c_checkbox("Teammates", "espTeammates"));
			player_group->add_child(new c_checkbox("Dormant", "espDormant"));

			player_group->add_child(new c_colorpicker("Box color", "colorBox", true, true));
			player_group->add_child(new c_checkbox("Bounding box", "espBox"));

			player_group->add_child(new c_checkbox("Health bar", "espHealth"));
			player_group->add_child(new c_checkbox("Health text", "espHealthText"));

			player_group->add_child(new c_colorpicker("nameclr", "colorName", true, true));
			player_group->add_child(new c_checkbox("Name", "espName"));

			player_group->add_child(new c_checkbox("Flags", "espFlagsEnable"));
			player_group->add_child(new c_multicombo("Flags type", "espFlags", { "Money", "Has armour", "Is scoped", "Is defusing", "Has hostage", "Is reloading", "Is fake-duck" }));

			player_group->add_child(new c_checkbox("Weapon text", "espWeapon"));
			player_group->add_child(new c_colorpicker("wpnclr", "colorWeapon", true, true));
			player_group->add_child(new c_checkbox("Weapon icon", "espWeaponIcon"));

			player_group->add_child(new c_colorpicker("ammoclr", "colorAmmoBar", true, true));
			player_group->add_child(new c_checkbox("Ammo", "espWeaponAmmoBar"));

			player_group->add_child(new c_checkbox("Distance", "espDistance"));

			player_group->add_child(new c_colorpicker("glowclr", "colorGlow", true, true));
			player_group->add_child(new c_multicombo("Glow", "espGlow", { "Enemy", "Team", "Local" }));

			//player_group->add_child(new c_colorpicker("glowclr", "colorGlow", true, true));
			//player_group->add_child(new c_checkbox("Glow", "espGlow"));

			//player_group->add_child(new c_colorpicker("glowclr", "colorGlowLocal", true, true));
			//player_group->add_child(new c_checkbox("Local glow", "espGlowLocal"));

			player_group->add_child(new c_colorpicker("skeletonclr", "colorSkeleton", true, false));
			player_group->add_child(new c_checkbox("Skeleton", "espSkeleton"));

			player_group->add_child(new c_colorpicker("skeletonbtclr", "colorSkeletonBT", true, false));
			player_group->add_child(new c_checkbox("History skeleton", "espSkeletonBT"));

			player_group->add_child(new c_colorpicker("Out of fov color", "colorOffscreenEsp", true, true));
			player_group->add_child(new c_checkbox("Out of fov arrow", "espOffscreen"));

			player_group->add_child(new c_slider("Arrow size", "espOffscreenSize", 5.f, 25.f, "%.0fpx"));
			player_group->add_child(new c_slider("Arrow radius", "espOffscreenRadius", 10.f, 100.f, "%.0f%%"));

			player_group->add_child(new c_checkbox("Hit marker", "espHitmarker"));
			player_group->add_child(new c_combo("Hit marker sound", "espHitmarkerSound", { "Off", "Arena switch press 02", "Bubble", "Cod" }));
		}

		c_group *models_group = new c_group("Coloured models", Vector2(223, 190));
		{
			models_group->add_child(new c_combo("Model material", "chamsMaterial", { "Off", "Default", "Solid", "Shaded", "Metallic", "Glow", "Bubble" }));

			models_group->add_child(new c_colorpicker("colorEnemy", "colorChamsEnemy", true, true));
			models_group->add_child(new c_checkbox("Player", "chamsEnemy"));
			
			models_group->add_child(new c_colorpicker("xqzchamsclr", "colorChamsEnemyXQZ", true, true));
			models_group->add_child(new c_checkbox("Player (behind wall)", "chamsEnemyXQZ"));

			models_group->add_child(new c_colorpicker("teammateschamsclr", "colorChamsTeammates", true, true));
			models_group->add_child(new c_checkbox("Show teammates", "chamsTeammates"));

			models_group->add_child(new c_colorpicker("teammateschamsclrxqz", "colorChamsTeammatesXQZ", true, true));
			models_group->add_child(new c_checkbox("Show teammates (behind wall)", "chamsTeammatesXQZ"));

			models_group->add_child(new c_colorpicker("handschamsclr", "colorChamsHands", true, true));
			models_group->add_child(new c_checkbox("Hands", "chamsHands"));

			models_group->add_child(new c_colorpicker("weaponchamsclr", "colorChamsWeapon", true, true));
			models_group->add_child(new c_checkbox("Weapon", "chamsWeapon"));

			models_group->add_child(new c_colorpicker("shadowchamsclr", "colorChamsShadow", true, true));
			models_group->add_child(new c_checkbox("Shadow", "chamsShadow"));

			models_group->add_child(new c_colorpicker("local", "colorChamsLocal", true, true));
			models_group->add_child(new c_checkbox("Local", "chamsLocal"));

			models_group->add_child(new c_colorpicker("clrDesync", "colorDesyncModel", true, true));
			models_group->add_child(new c_checkbox("Local shadow", "chamsLocalDesync"));
		}
		visuals_tab->add_child(models_group);


		c_group *other_group = new c_group("Other ESP", Vector2(223, 210));
		{
			other_group->add_child(new c_colorpicker("droppedWeaponCol", "colorWorldWeapon", true, true));
			other_group->add_child(new c_multicombo("Dropped weapons", "worldWeapon", { "Name", "Ammo", "Chams" }));

			other_group->add_child(new c_checkbox("Spectator list", "miscSpectators"));
			other_group->add_child(new c_checkbox("Force crosshair", "miscCrosshair"));

			other_group->add_child(new c_colorpicker("clrOutside", "colorInnacuracyOutside", true, true));
			other_group->add_child(new c_checkbox("Inaccuracy overlay", "miscInaccuracyOverlay"));
			other_group->add_child(new c_slider("Overlay size", "miscInaccuracySize", 0, 100, true, "%.0f%%"));

			other_group->add_child(new c_colorpicker("Grenade grenadenfsjhgbb", "colorGrenadePred", true, true));
			other_group->add_child(new c_checkbox("Grenade trajectory", "espGrenadePred"));

			other_group->add_child(new c_checkbox("Damage indicator", "espDamageIndicator"));
			other_group->add_child(new c_checkbox("Autowall indicator", "espAutowallIndicator"));

			other_group->add_child(new c_checkbox("Watermark", "espWatermark"));
		}
		visuals_tab->add_child(other_group);

		c_group *effects_group = new c_group("Effects", Vector2(223, 210));
		{
			effects_group->add_child(new c_checkbox("Remove flashbang effects", "espRemovalsFlash"));
			effects_group->add_child(new c_checkbox("Remove smoke grenades", "espRemovalsSmoke"));
			effects_group->add_child(new c_checkbox("Remove scope overlay", "espRemovalsScope"));
			effects_group->add_child(new c_checkbox("Remove zoom", "espRemovalsZoom"));
			effects_group->add_child(new c_checkbox("Remove visual recoil", "espRemovalsRecoil"));

			effects_group->add_child(new c_multicombo("World modulation", "worldModulation", { "Night", "Fullbright" }));
			effects_group->add_child(new c_slider("Brightness", "nightModeDarkness", 0, 100, true, "%.0f%%"));

			effects_group->add_child(new c_hotkey("Thirdperson key", "espThirdpersonKey", true));
			effects_group->add_child(new c_checkbox("Thirdperson", "espThirdperson"));
		}
		visuals_tab->add_child(effects_group);

	}
	c_tab *misc_tab = new c_tab("Miscellaneous");
	{
		menu->add_tab(misc_tab);

		c_group *misc_group = new c_group("Miscellaneous", Vector2(223, 436));
		{
			misc_group->add_child(new c_slider("Override fov", "miscFoV", 30, 120, true, L"%.0f"));
			misc_group->add_child(new c_checkbox("Bunnyhop", "miscBunnyhop"));

			misc_group->add_child(new c_checkbox("Air strafe", "miscStrafe"));
			misc_group->add_child(new c_checkbox("Movement key strafer", "miscKeyStrafe"));
			misc_group->add_child(new c_slider("Strafer speed", "miscStrafeSpeed", 0, 15, true, "%.0f%%"));

			misc_group->add_child(new c_checkbox("Air duck", "miscDuckInAir"));

			misc_group->add_child(new c_hotkey("Edge jump key", "miscEdgeJumpKey", true));
			misc_group->add_child(new c_checkbox("Edge jump", "miscEdgeJump"));

			misc_group->add_child(new c_checkbox("Clan tag spammer", "miscClanTag"));

			misc_group->add_child(new c_checkbox("Log weapon purchases", "miscPurchasesLog"));
			misc_group->add_child(new c_checkbox("Log damage dealt", "miscDamageLog"));
			misc_group->add_child(new c_checkbox("Remove duck delay", "miscUnlimitedDuck"));

			misc_group->add_child(new c_slider("Aspect ratio W", "miscAspectRatioW", 1, 3840, true, L"%.0fpx"));
			misc_group->add_child(new c_slider("Aspect ratio H", "miscAspectRatioH", 1, 2160, true, L"%.0fpx"));

			misc_group->add_child(new c_colorpicker("TracerColor", "colorBulletTracer", true, true));
			misc_group->add_child(new c_checkbox("Bullet tracer", "miscBulletTracer"));

			misc_group->add_child(new c_colorpicker("ImpactColor", "colorBulletImpact", true, true));
			misc_group->add_child(new c_checkbox("Bullet impact", "miscBulletImpact"));
		}
		misc_tab->add_child(misc_group);

		c_group *settings_group = new c_group("Settings", Vector2(223, 155));
		{
			settings_group->add_child(new c_hotkey("Menu key", "menu_bind", false));
			settings_group->add_child(new c_colorpicker("Menu color", "clr_menu", false, true));

			settings_group->add_child(new c_checkbox("Anti-untrusted", "miscAntiUT"));
		}
		misc_tab->add_child(settings_group);

		c_group* other_group = new c_group("Other", Vector2(223, 265));
		{
			other_group->add_child(new c_combo("Presets", "cfgPreset", { "Legit", "Rage", "HvH", "Secret", "Headshot", "Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot", "Golf", "Hotel", "India", "Juliet", "Kilo" }));

			other_group->add_child(new c_button("Load config", []() {
				std::string load_name = std::string("ConfigId").append(std::to_string(config->get_int("cfgPreset") + 1)).append(".cfg");

				config->load(load_name);
				}));

			other_group->add_child(new c_button("Save config", []() {
				std::string save_name = std::string("ConfigId").append(std::to_string(config->get_int("cfgPreset") + 1)).append(".cfg");

				config->save(save_name);
				}));

			other_group->add_child(new c_button("Reset config", []() {
				config->load("reset.cfg");
				}));

			other_group->add_child(new c_button("Unload", []() {
				config->set("unload", true);
				}));
		}
		misc_tab->add_child(other_group);
	}
	c_tab* skinschanger_tab = new c_tab("Skin changer");
	{
		menu->add_tab(skinschanger_tab);

		c_group* knifeoptions_group = new c_group("Knife options", Vector2(223, 125));
		{
			knifeoptions_group->add_child(new c_checkbox("Override knife", "skinsOverrideKnife"));

			//if (config->get_bool("skinsOverrideKnife"))
			knifeoptions_group->add_child(new c_combo("", "skinsKnifeModel", { "Bayonet", "Flip", "Gut", "Karambit", "M9 Bayonet", "Tactical", "Butterfly", "Falchion", "Shadow Daggers", "Survival Bowie", "Ursus", "Navaja", "Stiletto", "Talon" }));
		}
		skinschanger_tab->add_child(knifeoptions_group);

		c_group* gloveoptions_group = new c_group("Glove options", Vector2(223, 295));
		{

		}
		skinschanger_tab->add_child(gloveoptions_group);

		c_group* weaponskin_group = new c_group("Weapon skin", Vector2(223, 436));
		{

		}
		skinschanger_tab->add_child(weaponskin_group);
	}


	menu->set_active_tab(rage_tab); // active tab
}
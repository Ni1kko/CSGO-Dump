#include "GUI\GUI.h"
#include "Settings.h"
#include "SDK\ConVar.h"




std::vector<std::string> KeyStrings = {
		"",
		"Mouse 1",
		"Mouse 2",
		"Cancel",
		"Middle Mouse",
		"Mouse 4",
		"Mouse 5",
		"",
		"Backspace",
		"Tab",
		"",
		"",
		"Clear",
		"Enter",
		"",
		"",
		"Shift",
		"Control",
		"Alt",
		"Pause",
		"Caps",
		"",
		"",
		"",
		"",
		"",
		"",
		"Escape",
		"",
		"",
		"",
		"",
		"Space",
		"Page Up",
		"Page Down",
		"End",
		"Home",
		"Left",
		"Up",
		"Right",
		"Down",
		"",
		"",
		"",
		"Print",
		"Insert",
		"Delete",
		"",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"",
		"",
		"",
		"",
		"",
		"Numpad 0",
		"Numpad 1",
		"Numpad 2",
		"Numpad 3",
		"Numpad 4",
		"Numpad 5",
		"Numpad 6",
		"Numpad 7",
		"Numpad 8",
		"Numpad 9",
		"Multiply",
		"Add",
		"",
		"Subtract",
		"Decimal",
		"Divide",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",

};

void MenuMain::Initialize()
{
	/* Create our main window (Could have multiple if you'd create vec. for it) */
	auto mainWindow = std::make_shared<Window>(" Sesamhook ", SSize(600, 611), g_Fonts.pFontTahoma8, g_Fonts.pFontTahoma8);
	{

		auto tab1 = std::make_shared<Tab>("rage", 2, mainWindow);
		{
			/* Create sections for it */
			auto sectMain = tab1->AddSection("RageMainSect", 1.5f);
			{
				/* Add controls within section */
				sectMain->AddCheckBox("enabled", &g_Settings.bAimBotEnabled);
				sectMain->AddCheckBox("automatic fire", &g_Settings.bAutoFire);
				sectMain->AddCheckBox("silent aimbot", &g_Settings.bSilentAim);

				sectMain->AddSlider("hitchance", &g_Settings.iHitChance, 0, 100);
				sectMain->AddSlider("minimum damage", &g_Settings.iMinDamage, 1, 100);
				sectMain->AddText("hitbox selection");
				sectMain->AddMulti("hitbox selection", g_Settings.bHitboxSelection, std::vector<std::string>
				{
					"head",
						"neck",
						"chest",
						"stomach",
						"pelvis",
						"arms",
						"legs"
				});
				sectMain->AddText("multipoint selection");
				sectMain->AddMulti("multipoint selection", g_Settings.bMultiPointSelection, std::vector<std::string>
				{
					"head",
						"neck",
						"chest",
						"stomach",
						"pelvis",
						"arms",
						"legs"
				});
				sectMain->AddSlider("head scale", &g_Settings.iHeadScale, 0, 100);
				sectMain->AddSlider("neck scale", &g_Settings.iNeckScale, 0, 100);
				sectMain->AddSlider("chest scale", &g_Settings.iChestScale, 0, 100);
				sectMain->AddSlider("stomach scale", &g_Settings.iStomachScale, 0, 100);
				sectMain->AddSlider("pelvis scale", &g_Settings.iPelvisScale, 0, 100);
				sectMain->AddSlider("arms scale", &g_Settings.iArmsScale, 0, 100);
				sectMain->AddSlider("legs scale", &g_Settings.iLegsScale, 0, 100);
			}

			auto sectMain2 = tab1->AddSection("Rage2Sect", 1.f);
			{
				//sectMain2->AddSlider("body aim after x shots", &g_Settings.iBodyAimIfxDamage, 0, 10);
				sectMain2->AddCheckBox("auto pistol", &g_Settings.bAutomaticWeapons);
				sectMain2->AddCheckBox("auto revolver", &g_Settings.bAutomaticRevolver);
				sectMain2->AddCheckBox("auto stop", &g_Settings.bAutoStop);
				sectMain2->AddCheckBox("auto scope", &g_Settings.bAutoScope);
				sectMain2->AddCheckBox("resolver", &g_Settings.bResolver);
				sectMain2->AddCombo("resolver type", &g_Settings.iResolver, std::vector<std::string>{ "off", "default", "animstate" });
				//sectMain2->AddCheckBox("lag compensation", &g_Settings.bLagComp);
			}

		} mainWindow->AddChild(tab1);

		auto tab2 = std::make_shared<Tab>("anti aim", 2, mainWindow);
		{
			/* Create sections for it */
			auto sectMain = tab2->AddSection("AntiHitMainSect", 1.5f);
			{
				/* Add controls within section */
				sectMain->AddCheckBox("enabled", &g_Settings.bAntiAim);
				//sectMain->AddCombo("pitch", &g_Settings.iPitch, std::vector<std::string>{ "off", "default", "down", "up", "zero", "random" });
				sectMain->AddCombo("yaw", &g_Settings.iYaw, std::vector<std::string>{ "off", "backwards", "freestanding", "manual (arrows)" });
				sectMain->AddCheckBox("at targets", &g_Settings.bYawAtTargets);
				sectMain->AddCheckBox("desync", &g_Settings.bDesync);
				sectMain->AddCombo("desync type", &g_Settings.DesyncType, std::vector<std::string>{ "off", "static", "jitter" });
			}
			 
			auto sectMain2 = tab2->AddSection("AntiHit2Sect", 1.f);
			{
				sectMain2->AddCombo("fakelag", &g_Settings.iFakeLagType, std::vector<std::string>{ "off", "static", "dynamic", "switch" });
				sectMain2->AddSlider("fakelag choke", &g_Settings.iFakeLagChoke, 1, 14);
				sectMain2->AddText("flags");
				sectMain2->AddMulti("flags", g_Settings.bFakeLagFlags, std::vector<std::string>{
						"while standing",
						"on accelerate",
						"on high speed",
						"while moving"
				});
				sectMain2->AddCheckBox("while shooting", &g_Settings.bFakeLagShooting);
				sectMain2->AddCheckBoxUntrusted("fix leg movement", &g_Settings.bFixLegMovement);
				sectMain2->AddCheckBoxUntrusted("slide walk", &g_Settings.bSlideWalk);
				sectMain2->AddCheckBoxUntrusted("fake duck (caps)", &g_Settings.bFreezeWalk);
				sectMain2->AddCheckBoxUntrusted("slow walk (shift)", &g_Settings.bSlowWalk);
				//sectMain->AddCombo("fake duck", &g_Settings.iFakeDuckKey, KeyStrings);
			}

		} mainWindow->AddChild(tab2);

		/*auto tab3 = std::make_shared<Tab>("visuals", 2, mainWindow);
		{
			auto sectMain = tab3->AddSection("VisualsMainSect", 1.f);
			{
				
				sectMain->AddCheckBox("teammates", &g_Settings.bShowTeammates);
			//	sectMain->AddCheckBox("dormant", &g_Settings.bShowDormant);
				sectMain->AddColor("box color", &g_Settings.boxcolor);
				sectMain->AddCheckBox("bounding box", &g_Settings.bShowBoxes);
				sectMain->AddColor("skeleton color", &g_Settings.skeletoncolor);
				sectMain->AddCheckBox("skeleton", &g_Settings.bShowSkeleton);
				sectMain->AddColor("name color", &g_Settings.namecolor);
				sectMain->AddCheckBox("names", &g_Settings.bShowNames);
				sectMain->AddCheckBox("health bar", &g_Settings.bShowHealth);
				sectMain->AddColor("ammo color", &g_Settings.ammocolor);
				sectMain->AddCheckBox("weapon ammo", &g_Settings.bShowAmmo);
				sectMain->AddCheckBox("weapon name", &g_Settings.bShowWeaponName);
			//	sectMain->AddCheckBox("weapon icon", &g_Settings.bShowWeaponIcon);
				sectMain->AddColor("dropped name", &g_Settings.droppedweaponname);
				sectMain->AddText("dropped weapons");
				sectMain->AddMulti("dropped weapons", g_Settings.bDroppedItems, std::vector<std::string>{
					"name",
					"ammo"
				});
				sectMain->AddColor("projectile color", &g_Settings.projectilecolor);
				sectMain->AddCheckBox("projectiles", &g_Settings.bShowProjectiles);
				sectMain->AddColor("projectile color", &g_Settings.projectiledroppedcolor);
				sectMain->AddText("item glow");
				sectMain->AddMulti("item glow", g_Settings.bItemGlow, std::vector<std::string>{
						"dropped weapons",
						"projectiles"
				});
				sectMain->AddCheckBox("night mode", &g_Settings.bNightMode);
				sectMain->AddCheckBox("remove skybox", &g_Settings.bNoSky);
				sectMain->AddSlider("world transparency", &g_Settings.flTransparentWorld, 0.f, 100.f);
				sectMain->AddSlider("prop transparency", &g_Settings.flTransparentProps, 0.f, 100.f);
				sectMain->AddColor("chams v", &g_Settings.chamshidden);
				sectMain->AddCheckBox("player model", &g_Settings.bShowChams);
				sectMain->AddColor("chams h", &g_Settings.chamsvisible);
				sectMain->AddCheckBox("player model behind walls", &g_Settings.bShowChamsXQZ);
				sectMain->AddCombo("type", &g_Settings.iChamsMode, std::vector<std::string>{ "off", "normal", "flat", "wireframe" });
			
				sectMain->AddCombo("player glow", &g_Settings.iGlowStyle, std::vector<std::string>{ "off", "outline", "pulse" });
				sectMain->AddColor("glow clr", &g_Settings.glowcolorother);
			}

			auto sectMain2 = tab3->AddSection("Visuals2Sect", 1.f);
			{
				//sectMain2->AddCheckBox("knife left side", &g_Settings.bKnifeLeft);
				sectMain2->AddSlider("scope blend", &g_Settings.iScopedBlend, 1, 100);
			//	sectMain2->AddSlider("entity blend", &g_Settings.iEntBlend, 1, 100);
				sectMain2->AddSlider("world fov", &g_Settings.flWorldFov, 0, 120);
				sectMain2->AddSlider("viewmodel fov", &g_Settings.flViewModelFov, 0, 120);
				sectMain2->AddCheckBox("no scope ", &g_Settings.bScopeNoZoom);
				sectMain2->AddCheckBox("no zoom ", &g_Settings.bNoZoom);
				sectMain2->AddCheckBox("no visual recoil", &g_Settings.bNoVisRecoil);
				sectMain2->AddCheckBox("no flash", &g_Settings.bNoSmoke);
				sectMain2->AddCheckBox("no smoke", &g_Settings.bNoFlash);
				sectMain2->AddCheckBox("no post processing", &g_Settings.bNoPostProcess);
				sectMain2->AddCheckBox("autowall crosshair", &g_Settings.bAutoWallCrosshair);
				sectMain2->AddCheckBox("spread scope lines ", &g_Settings.bSpreadScope);
				sectMain2->AddCheckBox("hitmarker", &g_Settings.bHitmaker);
				sectMain2->AddColor("bullet tracerrrr", &g_Settings.bullettracer);
			
				sectMain2->AddCheckBox("bullet tracer", &g_Settings.bBulletTracers);
				sectMain2->AddColor("bullet impect", &g_Settings.impact);
				sectMain2->AddCheckBox("bullet impact", &g_Settings.bBulletImpacts);
				sectMain2->AddCombo("hitsound", &g_Settings.iHitSound, std::vector<std::string>{ "off", "skeet", "bubble", "bameware", "cod", "porn" });
		
				sectMain2->AddCheckBox("local player model", &g_Settings.bShowLocalChams);
				sectMain2->AddCheckBox("local player model behind walls", &g_Settings.bShowLocalChamsXQZ);
				sectMain2->AddCombo("type", &g_Settings.iLocalChamsMode, std::vector<std::string>{ "off", "normal", "flat", "original" });

				sectMain2->AddCombo("local glow", &g_Settings.iLocalGlowStyle, std::vector<std::string>{ "off", "outline", "pulse" });
				sectMain2->AddColor("glow clr local", &g_Settings.glowcolorlocal);
			}

		} mainWindow->AddChild(tab3);*/

		auto tab4 = std::make_shared<Tab>("skins", 2, mainWindow);
		{
			/* Create sections for it */
			auto sectMain = tab4->AddSection("SkinsMainSect", 1.5f);
			{
				//sectMain->AddCheckBox("enable knifechanger", &g_Settings.bSkinChangerWindow);
			//	sectMain->AddButton("full update", KnifeApplyCallbk);
				sectMain->AddCombo("knife", &g_Settings.iKnifeModel, std::vector<std::string>
				{ "off", "bayonet", "flip", "gut", "karambit",
				  "m9 bayonet", "huntsman", "falchion", "bowie",
				  "butterfly", "shadow daggers", "ursus", "navaja",
				  "stiletto", "talon" });
		
			}

			auto sectMain2 = tab4->AddSection("Skins2Sect", 1.f);
			{

			}

		} mainWindow->AddChild(tab4);

		auto tab5 = std::make_shared<Tab>("misc", 2, mainWindow);
		{
			auto sectMain = tab5->AddSection("MiscMainSect", 1.f);
			{
				sectMain->AddCheckBox("bunnyhop", &g_Settings.bBhopEnabled);
				sectMain->AddCheckBox("airstrafe", &g_Settings.bAirStrafer);
			//	sectMain->AddCombo("thirdperson", &g_Settings.iThirdPersonKey, KeyStrings);
				//sectMain->AddSlider("thirdperson distance", &g_Settings.iThirdPersonDistance, 0, 300);
				sectMain->AddCheckBox("thirdperson (alive) (v)", &g_Settings.bThirdPersonAlive);
				sectMain->AddCheckBox("thirdperson (dead)", &g_Settings.bThirdPersonDead);
			//	sectMain->AddCheckBox("preserve killfeed", &g_Settings.bPreserveKillFeed);
				sectMain->AddCheckBox("log damage", &g_Settings.bDamageLogs);
				sectMain->AddCheckBox("indicators", &g_Settings.bUselessInfo);
				sectMain->AddCheckBox("clantag changer", &g_Settings.bClantag);
				sectMain->AddCheckBox("kill say", &g_Settings.bKillSay);
				sectMain->AddCheckBox("force crosshair", &g_Settings.bEngineCrosshair);
			}

			auto sectMain2 = tab5->AddSection("Misc2Sect", 1.f);
			{
				//sectMain2->AddButton("shutdown", Detach);

				//sectMain2->AddColor("watermark color", &g_Settings.cWatermark);
				sectMain2->AddText("watermark color");

			//	sectMain2->AddColor("menu color", &g_Settings.cMenuCol);
				sectMain2->AddText("menu color");
				sectMain2->AddCheckBoxUntrusted("anti untrusted", &g_Settings.AntiUT);

				//sectMain2->AddButton("save cfg", save);
				//sectMain2->AddButton("load cfg", load);
			}

		} mainWindow->AddChild(tab5);
	}
	this->vecChildren.push_back(mainWindow);


	/*auto skinWindow = std::make_shared<Window>("skinchanger", SSize(280, 370), g_Fonts.pFontTahoma8, g_Fonts.pFontTahoma8);
	{
		auto tab1 = std::make_shared<Tab>("weapons", 1, skinWindow);
		{
			static int selected_weapon = 0;
			auto sectMainWeap = tab1->AddSection("MiscMainSect", 1.f);
			{
				static int selected_weapon = 0;
			//	sectMainWeap->AddCombo("Weapons", &selected_weapon, std::vector<std::string>{ skin_changer.NUM_WEAPONS });
			}
		} skinWindow->AddChild(tab1);

		auto tab2 = std::make_shared<Tab>("knife", 1, skinWindow);
		{

		} skinWindow->AddChild(tab2);

		auto tab3 = std::make_shared<Tab>("gloves", 1, skinWindow);
		{

		} skinWindow->AddChild(tab3);

		auto tab4 = std::make_shared<Tab>("custom", 1, skinWindow);
		{

		} skinWindow->AddChild(tab4);
	}
	this->vecChildren.push_back(skinWindow);*/

	/* Create our mouse cursor (one instance only) */
	mouseCursor = std::make_unique<MouseCursor>();

	/* Do the first init run through all of the objects */
	for (auto& it : vecChildren)
		it->Initialize();
}
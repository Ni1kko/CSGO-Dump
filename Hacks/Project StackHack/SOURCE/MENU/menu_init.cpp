#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../UTILS/render.h"
#include "../SDK/ConVar.h"
#include "Components.h"

#include "menu_framework.h"

int option = 0;

int menutab;

int AutoCalc(int va)
{
	if (va == 1)
		return va * 35;
	else if (va == 2)
		return va * 34;
	else
		return va * 25 + 7.5;
}

namespace MENU
{
	const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y | %d | %X", &tstruct);

		return buf;
	}
	void InitColors()
	{
		using namespace PPGUI_PP_GUI;


		colors[WINDOW_BODY] = CColor(22, 22, 22);
		colors[WINDOW_TITLE_BAR] = CColor(28, 28, 28, 0); //255, 75, 0
		colors[WINDOW_TEXT] = CColor(5, 84, 89, 1);

		colors[GROUPBOX_BODY] = CColor(40, 40, 40, 0);
		colors[GROUPBOX_OUTLINE] = CColor(60, 60, 60);
		colors[GROUPBOX_TEXT] = WHITE;

		colors[SCROLLBAR_BODY] = SETTINGS::settings.menu_col;

		colors[SEPARATOR_TEXT] = WHITE;
		colors[SEPARATOR_LINE] = CColor(60, 60, 60, 255);

		colors[CHECKBOX_CLICKED] = CColor(162, 204, 47);//HOTPINK
		colors[CHECKBOX_NOT_CLICKED] = CColor(60, 60, 60, 255);
		colors[CHECKBOX_TEXT] = WHITE;

		colors[BUTTON_BODY] = CColor(40, 40, 40, 255);
		colors[BUTTON_TEXT] = WHITE;

		colors[COMBOBOX_TEXT] = LIGHTGREY;
		colors[COMBOBOX_SELECTED] = CColor(40, 40, 40, 255);
		colors[COMBOBOX_SELECTED_TEXT] = WHITE;
		colors[COMBOBOX_ITEM] = CColor(20, 20, 20, 255);
		colors[COMBOBOX_ITEM_TEXT] = LIGHTGREY;

		colors[SLIDER_BODY] = CColor(40, 40, 40, 255);
		colors[SLIDER_VALUE] = CColor(137, 180, 48);//HOTPINK
		colors[SLIDER_TEXT] = WHITE;

		colors[TAB_BODY] = CColor(21, 21, 19);
		colors[TAB_TEXT] = CColor(255, 255, 255, 255);
		colors[TAB_BODY_SELECTED] = CColor(40, 40, 40, 150); //HOTPINK
		colors[TAB_TEXT_SELECTED] = CColor(137, 180, 48);

		colors[VERTICAL_TAB_BODY] = CColor(22, 22, 22, 120);
		colors[VERTICAL_TAB_TEXT] = CColor(255, 255, 255, 255);
		colors[VERTICAL_TAB_OUTLINE] = CColor(22, 22, 22, 150); //HOTPINK
		colors[VERTICAL_TAB_BODY_SELECTED] = CColor(22, 22, 22, 255); //HOTPINK
		colors[VERTICAL_TAB_TEXT_SELECTED] = CColor(137, 180, 48);

		colors[COLOR_PICKER_BODY] = CColor(50, 50, 50, 0);
		colors[COLOR_PICKER_TEXT] = WHITE;
		colors[COLOR_PICKER_OUTLINE] = CColor(0, 0, 0, 0);
	}

	void Do()
	{
		int w1, h1;
		INTERFACES::Engine->GetScreenSize(w1, h1);
		RENDER::DrawFilledRect(w1 - 242, 4, 242 + w1 - 247, 22, SETTINGS::settings.refixsign);
		RENDER::DrawF(w1 - 242 + 7, 5, FONTS::menu_window_font, false, false, CColor(255, 255, 255, 255), "deathrow");
		RENDER::DrawF(w1 - 242 + 70, 5, FONTS::menu_window_font, false, false, CColor(255, 255, 255, 255), currentDateTime());

		static bool menu_open = false;

		if (UTILS::INPUT::input_handler.GetKeyState(VK_INSERT) & 1)
		{
			menu_open = !menu_open;
			static const auto cvar = INTERFACES::cvar->FindVar("cl_mouseenable");
			if (menu_open)
				cvar->SetValue(0);
			else
				cvar->SetValue(1);

			INTERFACES::InputSystem->EnableInput(!menu_open);
		}

		if (GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.flip_int)) & 1)
			SETTINGS::settings.flip_bool = !SETTINGS::settings.flip_bool;

		if (GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.quickstopkey)) & 1)
			SETTINGS::settings.stop_flip = !SETTINGS::settings.stop_flip;

		if (GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.overridekey)) & 1)
			SETTINGS::settings.overridething = !SETTINGS::settings.overridething;


		InitColors();
		if (menu_hide)
		{

		}
		else
		{
			if (menu_open)
			{
				using namespace PPGUI_PP_GUI;
				if (!menu_open) return;

				DrawMouse();

				SetFont(FONTS::menu_window_font);
				WindowBegin("OOF", Vector2D(200, 200), Vector2D(700, 500));

				std::vector<std::string> tabs = { "aimbot", "visuals", "misc", "antiaim", "config","colors" };
				std::vector<std::string> aim_mode = { "rage", "legit" };
				std::vector<std::string> acc_mode = { "head", "body aim", "hitscan" };
				std::vector<std::string> chams_mode = { "none", "visible", "invisible" };
				std::vector<std::string> aa_pitch = { "none", "emotion", "fake down", "fake up", "fake zero", "nospread" };
				std::vector<std::string> desync_mode = { "none", "backwards" };
				std::vector<std::string> stand_desync = { "none", "flick" };
				std::vector<std::string> aa_mode = { "none", "backwards", "sideways", "backjitter", "lowerbody", "legit troll", "rotational", "autodirectional", "new freestand", "180treehouse" };
				std::vector<std::string> aa_fake = { "none", "backjitter", "random", "local view", "opposite", "rotational" };
				std::vector<std::string> real3 = { "none", "mini desync", "balance", "stretch", "maxim", "loca" };
				std::vector<std::string> configs = { "default", "legit", "autos", "scouts", "pistols", "awps", "nospread" };
				std::vector<std::string> box_style = { "none", "full", "debug" };
				std::vector<std::string> media_style = { "perfect", "random" };
				std::vector<std::string> delay_shot = { "off", "lag compensation" };
				std::vector<std::string> fakelag_mode = { "factor", "adaptive" };
				std::vector<std::string> key_binds = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12" };
				std::vector<std::string> hitmarker = { "none", "aimware", "gamesense", "bameware", "bubble", "roblox", "bhop", "penguware", "keyboard", "knock", "stapler", "fatality", "anime meme" };
				std::vector<std::string> antiaimmode = { "main", "desync", "misc" };
				std::vector<std::string> glow_styles = { "regular", "pulsing", "outline" };
				std::vector<std::string> local_chams = { "none","sim fakelag: normal", "non-sim fakelag", "sim fakelag: color" };
				std::vector<std::string> chams_type = { "none", "texture", "metallic" };
				std::vector<std::string> chams_type2 = { "wireframe","golden" ,"glass","glass 2","crystal","shitty blue","golden 2","crystal blue", "music meme" };
				std::vector<std::string> chams_type3 = { "wireframe","golden" ,"glass","glass 2","crystal","shitty blue","golden 2","crystal blue", "music meme" };
				std::vector<std::string> team_select = { "enemy" };
				std::vector<std::string> crosshair_select = { "none", "static", "recoil" };
				std::vector<std::string> autostop_method = { "head", "hitscan" };
				std::vector<std::string> override_method = { "set", "key-press" };
				std::vector<std::string> pistol = { "disabled", "dealge | revolver", "elites", "p250" };
				std::vector<std::string> snipers = { "disabled", "scar20 | g3sg1", "ssg08", "awp" };
				std::vector<std::string> armor = { "disabled", "kevlar", "Helmet & kevlar" };
				std::string config;


				SplitWindow(5);
				SetLine(1);
				GroupboxBegin2("", 100, 450, 0);

				switch (VerticalTab("OOF", tabs, OBJECT_FLAGS::FLAG_NONE))
				{
				case 0:
				{
					menutab = 0;
					break;
				}
				case 1:
				{
					menutab = 1;
					break;
				}
				case 2:
				{
					menutab = 2;
					break;
				}
				case 3:
				{
					menutab = 3;
					break;
				}
				case 4:
				{
					menutab = 4;
					break;
				}
				case 5:
				{
					menutab = 5;
					break;
				}
				}
				GroupboxEnd();

				SetLine(2);

				GroupboxBegin2("", 520, 450, 0);
				if (menutab == 0)// RAGE
				{
					Checkbox("enable aimbot", SETTINGS::settings.aim_bool);
					Combobox("aimbot type", aim_mode, SETTINGS::settings.aim_type);
					Combobox("aimbot mode", acc_mode, SETTINGS::settings.acc_type);
					Checkbox("Resolver enable", SETTINGS::settings.resolve_bool);
					Checkbox("Resolver 2 enable", SETTINGS::settings.resolve2_bool);
					Checkbox("Accuracy Standing", SETTINGS::settings.accuracystanding);
					Checkbox("more aimpoints", SETTINGS::settings.multi_bool);
					//Checkbox("velocity-prediction", SETTINGS::settings.vecvelocityprediction);
					Checkbox("fakeLag-fix", SETTINGS::settings.fakefix_bool);
					//Checkbox("prediciton", SETTINGS::settings.prediction);
					//Checkbox("auto zeus", SETTINGS::settings.autozeus_bool);
					//Checkbox("auto revolver", SETTINGS::settings.zeus);
					Checkbox("auto stop", SETTINGS::settings.stop_bool);
					//Checkbox("auto knife", SETTINGS::settings.autoknife_bool);
					//Checkbox("baim when in air", SETTINGS::settings.hsinair);
					Combobox("delay shot", delay_shot, SETTINGS::settings.delay_shot);
					Slider("minimum hit-chance", 0, 100, SETTINGS::settings.chance_val);
					Slider("minimum damage", 1, 100, SETTINGS::settings.damage_val);
					Slider("head scale", 0, 1, SETTINGS::settings.point_val);
					Slider("body scale", 0, 1, SETTINGS::settings.body_val);
				}

				else if (menutab == 1)//VISUALS
				{

					Checkbox("enable visuals", SETTINGS::settings.esp_bool);
					Checkbox("draw enemy box", SETTINGS::settings.box_bool);
					Checkbox("draw enemy name", SETTINGS::settings.Visuals.Visuals_Name);
					Checkbox("draw enemy weapon", SETTINGS::settings.Visuals.Visuals_Weapons);
					Checkbox("draw enemy flags", SETTINGS::settings.info_bool);
					Checkbox("draw enemy health", SETTINGS::settings.Visuals.Visuals_HealthBar);
					Checkbox("draw enemy ammo", SETTINGS::settings.ammo_bool);
					Checkbox("draw enemy fov arrows", SETTINGS::settings.fov_bool);
					Checkbox("no flash", SETTINGS::settings.Visuals.Visuals_NoFlash); // фикс потом крч
					Checkbox("remove smoke", SETTINGS::settings.smoke_bool);
					Checkbox("remove scope", SETTINGS::settings.scope_bool);
					//Checkbox("static scope", SETTINGS::settings.staticscope);
					Checkbox("Remove Zoom", SETTINGS::settings.removescoping);
					//Checkbox("no draw hand", SETTINGS::settings.wirehand_bool);
					//Checkbox("render spread", SETTINGS::settings.spread_bool);
					//Checkbox("remove visual recoil", SETTINGS::settings.norecoil);
					Checkbox("lby indicator", SETTINGS::settings.lbyenable);
					//Checkbox("aw hitmarker", SETTINGS::settings.awhitmarker);
					Checkbox("fix zoom sensitivity", SETTINGS::settings.fixscopesens);
					//Checkbox("gravity ragdoll", SETTINGS::settings.ragdoll);
					Checkbox("bullet tracers", SETTINGS::settings.beam_bool);
					//Checkbox("spectator list", SETTINGS::settings.spectatorlisty);
					//Checkbox("draw lby timer", SETTINGS::settings.lby_timer);
					Checkbox("bullet impacts", SETTINGS::settings.impacts);
					//Combobox("glow team selection", team_select, SETTINGS::settings.glowteamselection);
				//	if (SETTINGS::settings.glowteamselection == 0)
						//Checkbox("enemy glow enable", SETTINGS::settings.glowenable);
					//else if (SETTINGS::settings.glowteamselection == 1)
						//Checkbox("team glow enable", SETTINGS::settings.glowteam);
					//Combobox("glow style", glow_styles, SETTINGS::settings.glowstyle);
					Checkbox("local glow", SETTINGS::settings.glowlocal);
					Combobox("local glow style", glow_styles, SETTINGS::settings.glowstylelocal);
					Checkbox("thirdperson", SETTINGS::settings.tp_bool);
					Checkbox("FL Indicator", SETTINGS::settings.flindicate);
					Checkbox("Draw LBY Timer", SETTINGS::settings.lby_bar);
					if (SETTINGS::settings.lby_bar)
					{
						ColorPicker("Timer Color", SETTINGS::settings.lby_col);
						Checkbox("Draw LBY Update", SETTINGS::settings.lby_bar1);
						Checkbox("Show LBY delta", SETTINGS::settings.showlbydelta);
					}

					//Combobox("thirdperson key", key_binds, SETTINGS::settings.thirdperson_int);
					Combobox("Chams Type", chams_type, SETTINGS::settings.chamstype);
					//Checkbox("Local Chams", SETTINGS::settings.localchams);
					Combobox("Enemy Chams", chams_mode, SETTINGS::settings.chams_type);
					//Checkbox("force crosshair", SETTINGS::settings.forcehair);
					Combobox("crosshair", crosshair_select, SETTINGS::settings.xhair_type);
					//Checkbox("local chams", SETTINGS::settings.localchams);
					//if (!SETTINGS::settings.matpostprocessenable)
						//Checkbox("Enable Postprocessing", SETTINGS::settings.matpostprocessenable);
					//else
						Checkbox("Disable Postprocessing", SETTINGS::settings.matpostprocessenable);
					Combobox("hitmarker sound", hitmarker, SETTINGS::settings.hitmarker_val);
					Checkbox("player hitmarkers", SETTINGS::settings.hitmarker);
					//Checkbox("screen hitmarkers", SETTINGS::settings.hitmarker_screen);
					Slider("viewmodel fov", 0, 179, SETTINGS::settings.viewfov_val, 68);
					Slider("render fov", 0, 179, SETTINGS::settings.fov_val, 90);
					//Checkbox("sky color changer", SETTINGS::settings.sky_enabled);
					//Checkbox("world color changer", SETTINGS::settings.wolrd_enabled);
					Checkbox("night mode", SETTINGS::settings.night_bool);
					Slider("night value", 0, 100, SETTINGS::settings.daytimevalue, 50);

				}

				else if (menutab == 2)//MISC
				{

					Checkbox("enable misc", SETTINGS::settings.misc_bool);
					Checkbox("auto bunnyhop", SETTINGS::settings.bhop_bool);
					Checkbox("auto strafer", SETTINGS::settings.strafe_bool);
					Checkbox("clantag", SETTINGS::settings.misc_clantag);
					Checkbox("slowwalk[shift]", SETTINGS::settings.slowalkbool);
					Checkbox("logs", SETTINGS::settings.Visuals.Visuals_logs);
					Checkbox("say 1 if dmg > 100", SETTINGS::settings.trashtalk);
					Checkbox("trashtalk if hit head", SETTINGS::settings.trashtalk2);
					//Checkbox("buybot", SETTINGS::settings.buybot_enabled);
					//if (SETTINGS::settings.buybot_enabled)
				//{
						////{
						//	Combobox("Pistols", pistol, SETTINGS::settings.buybot_pistol);
							//Combobox("Snipers", snipers, SETTINGS::settings.buybot_rifle);
							//Combobox("Armor", armor, SETTINGS::settings.buybot_armor);
							//Checkbox("Zeus", SETTINGS::settings.buybot_zeus);
							//Checkbox("Grenades", SETTINGS::settings.buybot_grenade);
						//}
				//	}
					Checkbox("enable fakelag", SETTINGS::settings.lag_bool);
					Combobox("fakelag type", fakelag_mode, SETTINGS::settings.lag_type);

				}

				else if (menutab == 3)//ANTIAIM
				{
					Checkbox("enable misc", SETTINGS::settings.aa_bool);
					Combobox("antiaim mode", antiaimmode, SETTINGS::settings.aa_mode);
					
					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
						Combobox("antiaim pitch", aa_pitch, SETTINGS::settings.aa_pitch_type);
						Combobox("antiaim real", aa_mode, SETTINGS::settings.aa_real_type);
						Combobox("selfdesync", real3, SETTINGS::settings.real3);
						
						
						break;
					case 1://move
						Combobox("selfdesync", real3, SETTINGS::settings.real3);
						Checkbox("desync", SETTINGS::settings.desync_aa3);
						Combobox("manual aa key", key_binds, SETTINGS::settings.flip_int);
						break;
					case 2://jump

						break;
					}



					
					//Checkbox("fake angle chams", SETTINGS::settings.aa_fakeangchams_bool);
					//Checkbox("anti-aim arrows", SETTINGS::settings.rifk_arrow);
					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
						//Slider("real additive", -180, 180, SETTINGS::settings.aa_realadditive_val);
					//	Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive_val);
						//Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta_val);
						break;
					case 1:
						//Slider("real additive ", -180, 180, SETTINGS::settings.aa_realadditive1_val);
						//Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive1_val);
						//Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta1_val);
						break;
					case 2:
						//Slider("real additive", -180, 180, SETTINGS::settings.aa_realadditive2_val);
						//Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive2_val);
						//Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta2_val);
						break;
					}



				

					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
					
						break;
					case 1:
						
						break;
					case 2:
					
						break;
					}

				}
				else if (menutab == 4)//CONFIG
				{
					switch (Combobox("config", configs, SETTINGS::settings.config_sel))
					{
					case 0: config = "default"; break;
					case 1: config = "legit"; break;
					case 2: config = "auto_hvh"; break;
					case 3: config = "scout_hvh"; break;
					case 4: config = "pistol_hvh"; break;
					case 5: config = "awp_hvh"; break;
					case 6: config = "nospread_hvh"; break;
					}

					if (Button("Load Config"))
					{
						SETTINGS::settings.Load(config);

						INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[deathrow] ");
						GLOBAL::Msg("Configuration loaded.    \n");
					}

					if (Button("Save Config"))
					{
						SETTINGS::settings.Save(config);

						INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[deathrow] ");
						GLOBAL::Msg("Configuration saved.    \n");
					}

				}
				else if (menutab == 5)// Colors
				{
					Combobox("ESP Colour Selection", team_select, SETTINGS::settings.espteamcolourselection);
					if (SETTINGS::settings.espteamcolourselection == 0)
					{
						ColorPicker("Enemy Box", SETTINGS::settings.box_col);
						ColorPicker("Enemy Name", SETTINGS::settings.name_col);
						ColorPicker("Enemy Weapon", SETTINGS::settings.weapon_col);
						ColorPicker("Enemy Fov Arrows", SETTINGS::settings.fov_col);
						//ColorPicker("Enemy Visible", SETTINGS::settings.vmodel_col);
						//ColorPicker("Enemy Invisible", SETTINGS::settings.imodel_col);
						//ColorPicker("Glow", SETTINGS::settings.glow_col);
					//	ColorPicker("Bullet Tracer", SETTINGS::settings.bulletenemy_col);
					}
					else if (SETTINGS::settings.espteamcolourselection == 1)
					{
						ColorPicker("Team Box", SETTINGS::settings.boxteam_col);
						ColorPicker("Team Name", SETTINGS::settings.nameteam_col);
						ColorPicker("Team Weapon", SETTINGS::settings.weaponteam_col);
						ColorPicker("Team Fov Arrows", SETTINGS::settings.arrowteam_col);
						//ColorPicker("Team Visible", SETTINGS::settings.teamvis_color);
						//ColorPicker("Team Invisible", SETTINGS::settings.teaminvis_color);
						//ColorPicker("Glow", SETTINGS::settings.teamglow_color);
						//ColorPicker("Bullet Tracer", SETTINGS::settings.bulletteam_col);
					}
					//ColorPicker("Chams - Local", SETTINGS::settings.localchams_col);
					//ColorPicker("Glow - Local", SETTINGS::settings.glowlocal_col);
					//ColorPicker("Bullet Tracer - Local", SETTINGS::settings.bulletlocal_col);
					//ColorPicker("Grenade - Prediction", SETTINGS::settings.grenadepredline_col);
					//ColorPicker("Spread Corsshair", SETTINGS::settings.spread_Col);
					//ColorPicker("sky color", SETTINGS::settings.skycolor);
					//ColorPicker("world color", SETTINGS::settings.night_col);
					//ColorPicker("Hitmarkers", SETTINGS::settings.awcolor);
				}
				GroupboxEnd();

				WindowEnd();

				int w, h;
				static int x, y;

				INTERFACES::Engine->GetScreenSize(w, h);
				static bool init = false;
				if (init == false) {
					x = w / 2 - (400 / 2);
					y = h / 2 - (200 / 2);
					init = true;
				}
			}
		}
	}
}
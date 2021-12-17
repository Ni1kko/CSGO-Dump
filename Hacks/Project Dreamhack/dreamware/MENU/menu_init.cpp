/*#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../UTILS/render.h"
#include "../SDK/ConVar.h"
#include "Components.h"
#include "..\ImGui\imgui.h"
#include <d3d9.h>
#include "menu_framework.h"
#include "../HOOKS/hooks.h"
#include <winuser.h>
#include "../xdxdxd.h"
#include "../FEATURES/defines.h"





typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;


struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
void KnifeApplyCallbk()
{

	static auto clear_hud_weapon_icon_fn =
		reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
			UTILS::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	static SDK::ConVar* Meme = INTERFACES::cvar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	INTERFACES::Engine->ClientCmd_Unrestricted("cl_fullupdate");

}
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
	void InitColors()
	{
		
            using namespace PPGUI_PP_GUI;
			colors[WINDOW_BODY] = CColor(15, 15, 15); // fucking black
			colors[WINDOW_TITLE_BAR] = CColor(28, 28, 28); //255, 75, 0
			colors[WINDOW_TEXT] = CColor(137, 180, 48);

			colors[GROUPBOX_BODY] = CColor(22, 22, 22);
			static float rainbow;
			rainbow += 0.003f;
			if (rainbow > 1.f) rainbow = 0.f;
			colors[GROUPBOX_OUTLINE] = CColor(22, 22, 22);
			colors[GROUPBOX_TEXT] = WHITE;

			colors[SCROLLBAR_BODY] = CColor(83, 16, 16);

			colors[SEPARATOR_TEXT] = WHITE;
			colors[SEPARATOR_LINE] = CColor(83, 16, 16);

			colors[CHECKBOX_CLICKED] = (CColor().FromHSB(rainbow, 1.f, 1.f));
			colors[CHECKBOX_NOT_CLICKED] = CColor(60, 60, 60, 255);
			colors[CHECKBOX_TEXT] = WHITE;

			colors[BUTTON_BODY] = CColor(40, 40, 40, 255);
			colors[BUTTON_TEXT] = WHITE;

			colors[COMBOBOX_TEXT] = LIGHTGREY;
			colors[COMBOBOX_SELECTED] = CColor(40, 40, 40, 255);
			colors[COMBOBOX_SELECTED_TEXT] = WHITE;
			colors[COMBOBOX_ITEM] = CColor(20, 20, 20, 255);
			colors[COMBOBOX_ITEM_TEXT] = LIGHTGREY;

			colors[SLIDER_BODY] = CColor(40, 40, 40, 150);
			colors[SLIDER_VALUE] = (CColor().FromHSB(rainbow, 1.f, 1.f));
			colors[SLIDER_TEXT] = WHITE;

			colors[TAB_BODY] = CColor(22, 22, 22);
			colors[TAB_TEXT] = CColor(255, 255, 255, 255);
			colors[TAB_BODY_SELECTED] = CColor(22, 22, 22);
			colors[TAB_TEXT_SELECTED] = WHITE;  //CColor(83, 16, 16);

			colors[VERTICAL_TAB_BODY] = CColor(22, 22, 22);
			colors[VERTICAL_TAB_TEXT] = CColor(83, 16, 16);   //WHITE;
			colors[VERTICAL_TAB_OUTLINE] = CColor(22, 22, 22);
			colors[VERTICAL_TAB_BODY_SELECTED] = CColor(22, 22, 22);

			colors[COLOR_PICKER_BODY] = CColor(50, 50, 50, 0);
			colors[COLOR_PICKER_TEXT] = WHITE;
			colors[COLOR_PICKER_OUTLINE] = CColor(0, 0, 0, 0);
		


	}

	void Do()
	{
		if (UTILS::INPUT::input_handler.GetKeyState(VK_INSERT) & 1)
		{
			menu_open = !menu_open;
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

				//SKIN STUFF BEGIN//
				

				std::vector<std::string> pistol = { "disabled", "dealge | revolver", "elites", "p250" };
				std::vector<std::string> snipers = { "disabled", "scar20 | g3sg1", "ssg08", "awp" };
				std::vector<std::string> armor = { "disabled", "kevlar", "Helmet & kevlar" };


				WindowBegin("OOF", Vector2D(200, 200), Vector2D(596, 494));

				std::vector<std::string> tabs = { "A", "B", "C", "D", "F" };
				std::vector<std::string> aim_mode = { "rage", "legit" };
				std::vector<std::string> acc_mode = { "head", "Baim", "High", "Smart", "Perfect", };
				std::vector<std::string> chams_mode = { "none", "visible", "invisible" };
				std::vector<std::string> aa_pitch = { "none", "emotion", "fake down", "fake up", "fake zero" };
				std::vector<std::string> aa_mode = { "none", "backwards", "sideways", "backjitter", "lowerbody", "legit troll", "rotational", "autodirectional", "180z", "backwardjitter", "jitter", "spin", "180treehouse", "lby breaker" };
				std::vector<std::string> break_lby_mode =
									{ "none",
									"freestanding" };
											
				std::vector<std::string> aa_fake = { "none", "backjitter", "random", "local view", "opposite", "rotational",  "180z", "180treehouse", "lby breaker" };
				std::vector<std::string> configs = { "auto", "scout", "awp", "pistol", "nospread" };
				std::vector<std::string> box_style = { "none", "full", "debug" };
				std::vector<std::string> media_style = { "perfect", "random" };
				std::vector<std::string> delay_shot = { "off", "lag compensation" };
				std::vector<std::string> fakelag_mode = { "factor", "adaptive" };
				std::vector<std::string> key_binds = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12" };
				std::vector<std::string> fucku = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12" };

				std::vector<std::string> hitmarker = { "none", "gamesense", "custom" };
				std::vector<std::string> antiaimmode = { "standing", "moving", "jumping" };
				std::vector<std::string> glow_styles = { "regular", "pulsing", "outline" };
				std::vector<std::string> local_chams = { "none","sim fakelag: normal", "non-sim fakelag", "sim fakelag: color" };
				std::vector<std::string> chams_type = { "metallic", "texture" };
				std::vector<std::string> team_select = { "enemy", "team" };
				std::vector<std::string> crosshair_select = { "none", "static", "recoil" };
				std::vector<std::string> autostop_method = { "head", "hitscan" };
				std::vector<std::string> team_select_2 = { "enemy" };
				std::vector<std::string> chams_type5 = { "metallic", "texture" };
				std::vector<std::string> chams_mode5 = { "none", "visible", "invisible" };
				std::vector<std::string> chams_type1 = { "metallic", "texture" };
				std::vector<std::string> override_method = { "set", "key-press" };
				std::vector<std::string> resolver_method = { "stackhack", "resolvy.us", "custom" };
				std::vector<std::string> Menu_theme = { "Internaty", "Supremacy" };
				std::vector<std::string> hs_when = { "Hs if in air", "Body if in air" };
				std::vector<std::string> weap_icon = { "off", "icons", "text" };
				std::vector<std::string> ResolverOptions = { "None", "lby prediction", "expremiental" };
				std::vector<std::string> backtrack_option = { "chams", "skelet"};
				std::vector<std::string> spreadino = { "Default", "Cyrcle", "Rainbow" };
				std::vector<std::string> bullet_mode = { "off", "physbeam", "purplelaser" };
				std::vector<std::string> resolverino = { "off", "resolver1", "resolver2" };





				SplitWindow(5);
				SetLine(1);
				GroupboxBegin2("", 91, 444, 0);

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
				
				}
				GroupboxEnd();

				SetLine(2);

				GroupboxBegin2("", 449, 444, 0);
				if (menutab == 0)// RAGE
				{
					Checkbox("enable aimbot", SETTINGS::settings.aim_bool);
					Separator();
					Combobox("aimbot type", aim_mode, SETTINGS::settings.aim_type);
					Combobox("aimbot mode", acc_mode, SETTINGS::settings.acc_type);
					Separator("Accuracy");
					Combobox("Resolver", resolverino, SETTINGS::settings.ResolverEnable);
					Checkbox("Multipoints", SETTINGS::settings.multi_bool);
					if (SETTINGS::settings.multi_bool)
					{
						Slider("Head scale", 0, 1, SETTINGS::settings.point_val);
						Slider("Body scale", 0, 1, SETTINGS::settings.body_val);
						Slider("Arms scale", 0, 1, SETTINGS::settings.arms_val);
						Slider("Legs scale", 0, 1, SETTINGS::settings.legs_val);
						Slider("Foot scale", 0, 1, SETTINGS::settings.foot_val);
					}
					Separator("Hit-Chance");
					Checkbox("htitachance", SETTINGS::settings.hitchancevall);
					if (SETTINGS::settings.hitchancevall)
					{
						Slider("auto", 0, 100, SETTINGS::settings.auto_chance_val);
						Slider("scout/awp", 0, 100, SETTINGS::settings.scout_chance_val);

						Slider("pistols", 0, 100, SETTINGS::settings.pistols_chance_val);
						Slider("revolver/deagle", 0, 100, SETTINGS::settings.revolver_chance_val);

						Slider("rifles", 0, 100, SETTINGS::settings.other_chance_val);
					}

					Separator("Minimum Damage");
					Checkbox("mindamage", SETTINGS::settings.mindamagevall);
					if (SETTINGS::settings.mindamagevall)
					{
						Slider("auto damage", 0, 100, SETTINGS::settings.auto_damage_val);
						Slider("scout/awp damage", 0, 100, SETTINGS::settings.scout_damage_val);
						Slider("pistol damages", 0, 100, SETTINGS::settings.pistols_damage_val);
						Slider("revolver/deagle damage", 0, 100, SETTINGS::settings.revolver_damage_val);
						Slider("other damage", 0, 100, SETTINGS::settings.other_damage_val);
					}
					Separator("Other Settings");
					Checkbox("auto revolver", SETTINGS::settings.auto_revolver_enabled);
					Checkbox("awp baim", SETTINGS::settings.bodyawp);
					Checkbox("baim if faking", SETTINGS::settings.bodyfake);
					Checkbox("Backtrack", SETTINGS::settings.backtrack);
					Checkbox("velocity-prediction", SETTINGS::settings.vecvelocityprediction);
					Checkbox("prediciton", SETTINGS::settings.prediction);
					Checkbox("Accuracy Standing", SETTINGS::settings.accuracystanding);
					Checkbox("Control Recoil", SETTINGS::settings.controlrecoil);
					Combobox("if in air", hs_when, SETTINGS::settings.hsinair2);
					Combobox("delay shot", delay_shot, SETTINGS::settings.delay_shot);
				}

				else if (menutab == 1)//ANTIAIM
				{
					Checkbox("enable aa", SETTINGS::settings.aa_bool);
					Combobox("antiaim mode", antiaimmode, SETTINGS::settings.aa_mode);
					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
						Combobox("antiaim pitch - standing", aa_pitch, SETTINGS::settings.aa_pitch_type);
						Combobox("antiaim real - standing", aa_mode, SETTINGS::settings.aa_real_type);
						Combobox("antiaim fake - standing", aa_fake, SETTINGS::settings.aa_fake_type);
						break;
					case 1:
						Combobox("antiaim pitch - moving", aa_pitch, SETTINGS::settings.aa_pitch1_type);
						Combobox("antiaim real - moving", aa_mode, SETTINGS::settings.aa_real1_type);
						Combobox("antiaim fake - moving", aa_fake, SETTINGS::settings.aa_fake1_type);
						break;
					case 2:
						Combobox("antiaim pitch - jumping", aa_pitch, SETTINGS::settings.aa_pitch2_type);
						Combobox("antiaim real - jumping", aa_mode, SETTINGS::settings.aa_real2_type);
						Combobox("antiaim fake - jumping", aa_fake, SETTINGS::settings.aa_fake2_type);
						break;
					}
					Slider("spin speed", -50, 50, SETTINGS::settings.antiaim.spinspeed);
					Slider("jitter range", -180, 180, SETTINGS::settings.antiaim.jitterrange);
					if (Checkbox("break lby", SETTINGS::settings.antiaim.breaklby.enable)) {
					Combobox("type", break_lby_mode, SETTINGS::settings.antiaim.breaklby.type);
					
					Slider("lby delta", -180, 180, SETTINGS::settings.antiaim.breaklby.delta);
					}
					Combobox("flip key", key_binds, SETTINGS::settings.flip_int);
					Checkbox("Anti-Aim Arrows", SETTINGS::settings.rifk_arrow);
					if (SETTINGS::settings.rifk_arrow)
					{
						Slider("Anti-Aim Arrows Height", 0, 30, SETTINGS::settings.aa_arrows_height, 0);
						Slider("Anti-Aim Arrows Width", 0, 15, SETTINGS::settings.aa_arrows_width, 0);
					}	
					Checkbox("lby flick up (lby breaker)", SETTINGS::settings.flipup);
					Checkbox("draw lines", SETTINGS::settings.aa_lines);

					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
						Slider("real additive", -180, 180, SETTINGS::settings.aa_realadditive_val);
						Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive_val);
						Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta_val);
						break;
					case 1:
						Slider("real additive ", -180, 180, SETTINGS::settings.aa_realadditive1_val);
						Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive1_val);
						Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta1_val);
						break;
					case 2:
						Slider("real additive", -180, 180, SETTINGS::settings.aa_realadditive2_val);
						Slider("fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive2_val);
						Slider("lowerbodyyaw delta", -180, 180, SETTINGS::settings.delta2_val);
						break;
					}



					Slider("rotate fake °", 0, 180, SETTINGS::settings.spinanglefake);
					Slider("rotate fake %", 0, 100, SETTINGS::settings.spinspeedfake);

					switch (SETTINGS::settings.aa_mode)
					{
					case 0:
						Slider("rotate standing °", 0, 180, SETTINGS::settings.spinangle);
						Slider("rotate standing %", 0, 100, SETTINGS::settings.spinspeed);
						break;
					case 1:
						Slider("rotate moving °", 0, 180, SETTINGS::settings.spinangle1);
						Slider("rotate moving %", 0, 100, SETTINGS::settings.spinspeed1);
						break;
					case 2:
						Slider("rotate jumping °", 0, 180, SETTINGS::settings.spinangle2);
						Slider("rotate jumping %", 0, 100, SETTINGS::settings.spinspeed2);
						break;
					}

				}

				else if (menutab == 2)//VISUALS
				{

					Checkbox("enable visuals", SETTINGS::settings.esp_bool);
					Checkbox("draw enemy box", SETTINGS::settings.box_bool);
					if (SETTINGS::settings.box_bool)
					{
						ColorPicker("box color", SETTINGS::settings.box_col);
					}
					Checkbox("draw enemy name", SETTINGS::settings.name_bool);
					if (SETTINGS::settings.name_bool)
					{
						ColorPicker("name color", SETTINGS::settings.name_col);
					}
					Combobox("draw enemy weapon", weap_icon, SETTINGS::settings.weap_bool);
					if (SETTINGS::settings.weap_bool)
					{
						ColorPicker("Enemy Weapon", SETTINGS::settings.weapon_col);
					}
				
					Checkbox("draw enemy flags", SETTINGS::settings.info_bool);
					Checkbox("draw enemy health", SETTINGS::settings.health_bool);
					Checkbox("draw lby timer", SETTINGS::settings.lby_timer);
					if (SETTINGS::settings.lby_timer)
					{
						ColorPicker("lby timer color", SETTINGS::settings.lbytimer_color);
					}

					Checkbox("draw enemy ammo", SETTINGS::settings.ammo_bool);
					Checkbox("draw enemy fov arrows", SETTINGS::settings.fov_bool);
					if (SETTINGS::settings.fov_bool)
					{
						ColorPicker("fov arrows color", SETTINGS::settings.fov_col);
					}
					Checkbox("draw spread crosshair", SETTINGS::settings.spread_circle_enabled);
					if (SETTINGS::settings.spread_circle_enabled)
					{
						Combobox(("Type"), spreadino, SETTINGS::settings.type_spread_circle);
						if (SETTINGS::settings.type_spread_circle == 0)
							ColorPicker("default color", SETTINGS::settings.spread_Col);
						if (SETTINGS::settings.type_spread_circle == 1)
							ColorPicker("recoil color", SETTINGS::settings.spread_coll2);
					}
					Separator();
					Checkbox("remove flash", SETTINGS::settings.noflash);
					Checkbox("remove smoke", SETTINGS::settings.smoke_bool);
					Checkbox("remove scope", SETTINGS::settings.scope_bool);
					Checkbox("remove zoom", SETTINGS::settings.removescoping);
					Separator();
					
					Checkbox("remove visual recoil", SETTINGS::settings.norecoil);
					Checkbox("lby indicator", SETTINGS::settings.lbyenable);
					Checkbox("fix zoom sensitivity", SETTINGS::settings.fixscopesens);
					Checkbox("Bullet Tracers", SETTINGS::settings.beam_bool);
					if (SETTINGS::settings.beam_bool)
					{
						Combobox("Tracer Type", bullet_mode, SETTINGS::settings.bullet_type);
						Checkbox("Raibow Color", SETTINGS::settings.gradientbullet);
						ColorPicker("Bullet Tracer", SETTINGS::settings.bulletlocal_col);
						Slider("Duration", 0, 10, SETTINGS::settings.flTracersDuration);
						Slider("Width", 0, 10, SETTINGS::settings.flTracersWidth);
					}
					Checkbox("bullet impacts", SETTINGS::settings.impacts);
					Separator();
					Combobox("glow team selection", team_select, SETTINGS::settings.glowteamselection);
					if (SETTINGS::settings.glowteamselection == 0)
						Checkbox("Enemy Glow", SETTINGS::settings.glowenable);
					if (SETTINGS::settings.glowenable)
					{
						ColorPicker("glow color", SETTINGS::settings.glow_col);
					}
					if (SETTINGS::settings.glowteamselection == 1)
						Checkbox("glow color ", SETTINGS::settings.glowteam);
					if (SETTINGS::settings.glowteam)
					{
						ColorPicker("team glow color", SETTINGS::settings.teamglow_color);
					}
					Combobox("glow style", glow_styles, SETTINGS::settings.glowstyle);

					Checkbox("local glow", SETTINGS::settings.glowlocal);
					{
						Combobox("local glow style", glow_styles, SETTINGS::settings.glowstylelocal);
						ColorPicker("glow - local", SETTINGS::settings.glowlocal_col);
					}
					Separator();

					Combobox("model team selection", team_select_2, SETTINGS::settings.chamsteamselection);
					Combobox("model type", chams_type1, SETTINGS::settings.chamstype);
					Combobox("coloured models", chams_mode5, SETTINGS::settings.chams_type);
					if (SETTINGS::settings.chamsteamselection == 0)
						if (SETTINGS::settings.chams_type > 0)
							ColorPicker("Enemy Visible", SETTINGS::settings.vmodel_col);
					if (SETTINGS::settings.chams_type > 1)
						ColorPicker("Enemy Invisible", SETTINGS::settings.imodel_col);
					Checkbox("force crosshair", SETTINGS::settings.forcehair);
					Combobox("crosshair", crosshair_select, SETTINGS::settings.xhair_type);
					Separator();
					Checkbox("local chams", SETTINGS::settings.localchams);
					if (SETTINGS::settings.localchams)

					{
						ColorPicker("chams - local", SETTINGS::settings.localchams_col);
						Checkbox("Gradient", SETTINGS::settings.gradient);
						Slider("Gradient speed", 0.001, 0.01, SETTINGS::settings.gradientspeed, 3);

					}
					Checkbox("fake lag chams", SETTINGS::settings.localchams_fakelag);
					if (SETTINGS::settings.localchams_fakelag)
					{
						ColorPicker("fake lag color", SETTINGS::settings.localchams_col_lag);
					}


					Separator();
					Slider("transparency when scoping", 0, 100, SETTINGS::settings.transparency_when_scoped, 50);

					if (!SETTINGS::settings.matpostprocessenable)
						Checkbox("enable post", SETTINGS::settings.matpostprocessenable);
					else
						Checkbox("disable post", SETTINGS::settings.matpostprocessenable);
					Combobox("hitmarker sound", hitmarker, SETTINGS::settings.hitmarker_val);
					Checkbox("screen hitmarkers", SETTINGS::settings.hitmarker_screen);
					Checkbox("Damage Indicator", SETTINGS::settings.dmg_bool);
					if (SETTINGS::settings.dmg_bool)
					{
						ColorPicker("Indicator Color", SETTINGS::settings.dmg_color);
					}
					
					Checkbox("Aimware Hitmarkers", SETTINGS::settings.hitmarker);
					if (SETTINGS::settings.hitmarker)
					{
						ColorPicker("hitmarkers color", SETTINGS::settings.awcolor);
					}
					Checkbox("indicator", SETTINGS::settings.autoindicator);
					if (SETTINGS::settings.autoindicator)
					{
						ColorPicker("indicator color", SETTINGS::settings.autocolor);
					}
					Checkbox("sky color changer", SETTINGS::settings.sky_enabled);
					if (SETTINGS::settings.sky_enabled)
					{
						ColorPicker("sky color", SETTINGS::settings.skycolor);
					}
					
					Checkbox("night mode", SETTINGS::settings.night_bool);
					Slider("night value", 0, 100, SETTINGS::settings.daytimevalue, 50);
					Checkbox("fullbright", SETTINGS::settings.fullbright);
					
					
					

					//Checkbox("Ping & Fps indicator", SETTINGS::settings.fpsping);
					//ColorPicker("Ping & Fps indicator color", SETTINGS::settings.fpsping1);

				}
				else if (menutab == 3)// movement
				{
			     	Checkbox("enable misc", SETTINGS::settings.misc_bool);

				    Separator("movements");
				    Checkbox("auto bunnyhop", SETTINGS::settings.bhop_bool);
					Checkbox("auto strafer", SETTINGS::settings.strafe_bool);
					Checkbox("Duck In Air", SETTINGS::settings.duck_bool);
					Combobox("Circle Strafe", key_binds, SETTINGS::settings.circlestrafekey);
					Checkbox("astro walk", SETTINGS::settings.astro);
					Checkbox("fakewalk", SETTINGS::settings.fakewalk);
					Checkbox("enable fakelag", SETTINGS::settings.lag_bool);
					Combobox("fakelag type", fakelag_mode, SETTINGS::settings.lag_type);
					Slider("moving lag", 1, 14, SETTINGS::settings.move_lag);
					Slider("jumping lag", 1, 14, SETTINGS::settings.jump_lag);
					Separator("Other Settings");

					Checkbox("Gravitational Ragdoll", SETTINGS::settings.ragdoll_bool);
					Checkbox("thirdperson", SETTINGS::settings.tp_bool);
					Combobox("thirdperson key", key_binds, SETTINGS::settings.thirdperson_int);
					Slider("viewmodel fov", 0, 179, SETTINGS::settings.viewfov_val, 68);
					Slider("render fov", 0, 179, SETTINGS::settings.fov_val, 90);
					Checkbox("remove sleeve", SETTINGS::settings.misc_no_sleeves);
					Separator("BuyBot");
					Checkbox("buybot", SETTINGS::settings.buybot_enabled);
					if (SETTINGS::settings.buybot_enabled) {
						Combobox("pistols", pistol, SETTINGS::settings.buybot_pistol);
						Combobox("snipers", snipers, SETTINGS::settings.buybot_rifle);
						Combobox("armor", armor, SETTINGS::settings.buybot_armor);
						Checkbox("zeus", SETTINGS::settings.buybot_zeus);
						Checkbox("nades", SETTINGS::settings.buybot_grenade);
					}
					Checkbox("clan tag", SETTINGS::settings.Clantag);
					Checkbox("logs", SETTINGS::settings.logs);
					Checkbox("Resolver Logs", SETTINGS::settings.rlogs);

					std::string config;

					switch (Combobox("config", configs, SETTINGS::settings.config_sel))
					{
					case 0: config = "auto"; break;
					case 1: config = "scout"; break;
					case 2: config = "awp"; break;
					case 3: config = "pistol"; break;
					case 4: config = "nospread"; break;

					}

					if (Button("Load Config"))
					{
						SETTINGS::settings.Load(config);

						GLOBAL::Msg("[dreamhack.us] Configuration loaded.    \n");
					}

					if (Button("Save Config"))
					{
						SETTINGS::settings.Save(config);

						GLOBAL::Msg("[dreamhack.us] Configuration saved.    \n");
					}
					
				}

				else if (menutab == 4)// skins
				{
				   Checkbox("skinchanger", SETTINGS::settings.skinenabled);
				   Checkbox("Glovechanger", SETTINGS::settings.glovesenabled);
				   Combobox(("Knife Model"), KnifeModel, SETTINGS::settings.Knife);
				   Combobox(("Knife Skin"), knifeskins, SETTINGS::settings.KnifeSkin);
				   Combobox(("Glove Model"), GloveModel, SETTINGS::settings.gloves);

				   if (SETTINGS::settings.gloves == 1)
				   {

					   std::vector<std::string> glovesskins =
					   {
						   "charred",
						   "snakebite",
						   "bronzed",
						   "guerilla"
					   };

					   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);
				   }
				   else
					   if (SETTINGS::settings.gloves == 2)
					   {
						   std::vector<std::string> glovesskins =
						   {
							   "hedge maze",
							   "andoras box",
							   "superconductor",
							   "arid",
							   "omega",
							   "amphibious",
							   "bronze morph"
						   };

						   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);
					   }
					   else
						   if (SETTINGS::settings.gloves == 3)
						   {
							   std::vector<std::string> glovesskins =
							   {
								   "lunar weave",
								   "convoy",
								   "crimson weave",
								   "diamondback",
								   "overtake",
								   "racing green",
								   "king snake",
								   "imperial plaid"
							   };

							   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

						   }
						   else
							   if (SETTINGS::settings.gloves == 4)
							   {
								   std::vector<std::string> glovesskins =
								   {
									   "leather",
									   "spruce ddpat",
									   "slaughter",
									   "cobalt skulls",
									   "overprint",
									   "duct tape",
									   "arboreal"
								   };

								   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

							   }
							   else
								   if (SETTINGS::settings.gloves == 5)
								   {
									   std::vector<std::string> glovesskins =
									   {
										   "eclipse",
										   "spearmint",
										   "boom",
										   "cool mint",
										   "turtle",
										   "transport",
										   "polygon",
										   "pow"
									   };

									   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

								   }
								   else
									   if (SETTINGS::settings.gloves == 6)
									   {
										   std::vector<std::string> glovesskins =
										   {
											   "forest ddpat",
											   "crimson kimono",
											   "emerald web",
											   "foundation",
											   "crimson web",
											   "buckshot",
											   "fade",
											   "mogul"
										   };

										   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

									   }
									   else
										   if (SETTINGS::settings.gloves == 7)
										   {
											   std::vector<std::string> glovesskins =
											   {
												   "emerald",
												   "mangrove",
												   "rattler",
												   "case hardened"
											   };

											   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

										   }
										   else
											   if (SETTINGS::settings.gloves == 0)
											   {
												   std::vector<std::string> glovesskins = { "" };

												   Combobox(("glove skin"), glovesskins, SETTINGS::settings.skingloves);

											   }

				   Separator("rifles");
				   Combobox("AK-47", ak47, SETTINGS::settings.AK47Skin);
				   Combobox("M4A1-S", m4a1s, SETTINGS::settings.M4A1SSkin);
				   Combobox("M4A4", m4a4, SETTINGS::settings.M4A4Skin);
				   Combobox("GALIL-AR", galil, SETTINGS::settings.GalilSkin);
				   Combobox("AUG", aug, SETTINGS::settings.AUGSkin);
				   Combobox("FAMAS", famas, SETTINGS::settings.FAMASSkin);
				   Combobox("SG553", sg553, SETTINGS::settings.Sg553Skin);

				   Separator("Snipers");
				   Combobox("SCAR-20", scar20, SETTINGS::settings.SCAR20Skin);
				   Combobox("G3SG1", g3sg1, SETTINGS::settings.G3sg1Skin);
				   Combobox("AWP", awp, SETTINGS::settings.AWPSkin);
				   Combobox("SSG-08", ssg08, SETTINGS::settings.SSG08Skin);

				   Separator("Smg's");
				   Combobox("UMP-45", ump45, SETTINGS::settings.UMP45Skin);
				   Combobox("MAC-10", mac10, SETTINGS::settings.Mac10Skin);
				   Combobox("MP-9", mp9, SETTINGS::settings.Mp9Skin);

				   Separator("Pistoles");
				   Combobox("USP-S", usp, SETTINGS::settings.USPSkin);
				   Combobox("GLOCK-18", glock, SETTINGS::settings.GlockSkin);
				   Combobox("DEAGLE", deagle, SETTINGS::settings.DeagleSkin);
				   Combobox("CZ-75", cz75, SETTINGS::settings.Cz75Skin);
				   Combobox("Duals", dual, SETTINGS::settings.DualSkin);
				   Combobox("FIVE-SEVEN", five, SETTINGS::settings.FiveSkin);
				   Combobox("REVOLVER", revolver, SETTINGS::settings.RevolverSkin);
				   Combobox("P-2000", p2000, SETTINGS::settings.P2000Skin);
				   Combobox("TEC-9", tec9, SETTINGS::settings.tec9Skin);
				   Combobox("P-250", p250, SETTINGS::settings.P250Skin);

				   if (Button(("Force update")))
				   {
					   KnifeApplyCallbk();

				   }
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
}*/
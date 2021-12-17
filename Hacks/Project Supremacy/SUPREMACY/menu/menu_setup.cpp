#include "cmenu.hpp"
#include "..\cheats\misc\logs.h"
#include <cctype>
#include <shlobj.h>
#include <shlwapi.h>
#include <algorithm>
#define NOMINMAX
#include <Windows.h>
#include <chrono>
struct hud_weapons_t
{
	std::int32_t* get_weapon_count()
	{
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(util::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(util::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
void KnifeApplyCallbk()
{
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(util::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));
	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");
	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);

	if (hud_weapons == nullptr)
		return;
	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++) i = clear_hud_weapon_icon_fn(hud_weapons, i);
	typedef void(*ForceUpdate) (void);
}
std::string get_config_dir() {
	std::string folder;
	static TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
		folder = std::string(path) + "\\Supremacy\\";
	}
	CreateDirectory(folder.c_str(), NULL);
	return folder;
}
#define BUILDSTAMP ( __DATE__ )
void load_config() {
	if (!cfg_manager->files.size()) { return; }
	eventlogs::get().add("Loaded " + cfg_manager->files[g_cfg.selected_config], Color::White);
	cfg_manager->load(cfg_manager->files[g_cfg.selected_config]);
}
void save_config() {
	if (!cfg_manager->files.size()) { return; }
	eventlogs::get().add("Saved " + cfg_manager->files[g_cfg.selected_config], Color::White);
	cfg_manager->save(cfg_manager->files[g_cfg.selected_config]);
	cfg_manager->config_files();
}
void remove_config() {
	if (!cfg_manager->files.size()) { return; }
	cfg_manager->remove(cfg_manager->files[g_cfg.selected_config]);
	cfg_manager->config_files();
	if (g_cfg.selected_config > cfg_manager->files.size() - 1) {
		g_cfg.selected_config = cfg_manager->files.size() - 1;
	}
}
void add_config() {
	if (g_cfg.new_config_name.find(".cfg") == -1) g_cfg.new_config_name += ".cfg";
	cfg_manager->save(g_cfg.new_config_name.c_str());
	cfg_manager->config_files();
	g_cfg.new_config_name = "";
	g_cfg.selected_config = cfg_manager->files.size() - 1;
}
void unload_cheat() {
	unload = true;
}
bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "") return true;
	return sentence.find(word) != std::string::npos;
}
std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);
	return str;
}
void setup_main_menu()
{

	static auto set_sub = [](int sub) -> void {
		g_cfg.menu.group_sub = sub;
	};
	static auto set_tabsub = [](int sub) -> void {
		g_cfg.menu.tab_sub = sub;
	};
	/*auto legit_tab = new C_Tab(&menu::get(), 0, "Legit", "Legit");
	{
		auto legit_aimbot = new C_GroupBox(GROUP_RIGHT, 8, "");
		{
			auto aim_assist = new C_CheckBox("Aim Assist", &g_cfg.legit.aimassist);
			auto aim_assist_key = new C_KeyBind("", &g_cfg.legit.aim_assist_bind);
			auto fov = new C_SliderFloat("Field Of View", &g_cfg.legit.fov, 0, 180, "°");
			auto smoothing = new C_SliderFloat("Smoothing", &g_cfg.legit.smoothing, 0, 15, "°");
		}
		auto legit_backtrack = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto backtracking = new C_CheckBox("Backtracking Exploit", &g_cfg.legit.backtracking);
			auto max_time = new C_SliderFloat("Maximum Backtracking Time", &g_cfg.legit.backtracking_max_time, 0.00f, 0.20f, " s");
			auto visualize = new C_CheckBox("Visualize Backtracking", &g_cfg.legit.backtracking_visualize,
				{ new C_ColorSelector(&g_cfg.legit.backtracking_visualize_color) });
		}
	}*/
	/*auto rage_tab = new C_Tab(&menu::get(), 1, "Aimbot", "Aimbot",
		{
		C_Tab::subtab_t("Main", 0, fonts[TABFONT]),
		C_Tab::subtab_t("Pistol", 1, fonts[TABFONT]),
		C_Tab::subtab_t("Deagle", 2, fonts[TABFONT]),
		C_Tab::subtab_t("Rifl", 3, fonts[TABFONT]),
		C_Tab::subtab_t("Smg", 4, fonts[TABFONT]),
		C_Tab::subtab_t("SSG/AWP", 5, fonts[TABFONT]),
		C_Tab::subtab_t("Scar", 6, fonts[TABFONT]),
		C_Tab::subtab_t("Awp", 7, fonts[TABFONT])
		});
	{
		set_tabsub(0);
		auto rage = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto enable = new C_CheckBox(" Aim bot Enable", &g_cfg.ragebot.enable);
			auto fov = new C_SliderInt(" FOV", &g_cfg.ragebot.field_of_view, 0, 360, "°");
			auto selection_type = new C_Dropdown(" Selection priority", &g_cfg.ragebot.selection_type, { "Field Of View (FOV)", "Lowest Health", "Closest Distance" });
			auto silent_aim = new C_CheckBox(" Silent Aim", &g_cfg.ragebot.silent_aim);
			auto hsonly = new C_CheckBox(" NoSpread", &g_cfg.ragebot.hsonly);
			auto anti_recoil = new C_CheckBox(" No Recoil", &g_cfg.ragebot.anti_recoil);
			auto anti_spread = new C_CheckBox(" No Spread", &g_cfg.ragebot.anti_spread);
			auto zeus_bot = new C_CheckBox(" Zeus bot", &g_cfg.ragebot.zeus_bot);
			auto knife_bot = new C_CheckBox(" Knife bot", &g_cfg.ragebot.knife_bot);
			auto auto_revolver = new C_CheckBox(" Auto Revolver", &g_cfg.ragebot.autorevolver);
			auto autoscope = new C_CheckBox(" Auto Scope", &g_cfg.ragebot.autoscope);
			auto extrapolation = new C_CheckBox(" Clever Extrapolation", &g_cfg.ragebot.extrapolation);
		}
		auto rage2 = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			new C_CheckBox(" Resolver", &g_cfg.ragebot.resolver_experimental);
			new C_Dropdown(" Resolver Mode", &g_cfg.ragebot.resolver_experimentalmode, { "Clever Bruteforce", "Desync Resolver" });

			auto aimstep = new C_CheckBox(" Aim Step", &g_cfg.ragebot.aimstep);
		}
		for (int i = 0; i < 7; i++)
		{
			set_tabsub(i + 1);
			auto weapon = new C_GroupBox(GROUP_LEFT, 8, "", 2); {
				for (int s = 0; s < 2; s++)
				{
					set_sub(s);
					if (s == 0)
					{
						auto autoshoot = new C_CheckBox(" Auto Fire", &g_cfg.ragebot.autoshoot[i]);
						auto autowall = new C_CheckBox(" Min DMG", &g_cfg.ragebot.autowall[i]);
						auto minimum_damage = new C_SliderInt("", &g_cfg.ragebot.minimum_damage[i], 1, 100, "");
						auto hitchance = new C_CheckBox(" Hitchance", &g_cfg.ragebot.hitchance[i]);
						auto hitchance_amount = new C_SliderInt("", &g_cfg.ragebot.hitchance_amount[i], 0, 100, "%");
						auto hitscan = new C_MultiDropdown(" Hitboxes to Scam", &g_cfg.ragebot.hitscan[i]);
						auto hitscan_history = new C_MultiDropdown("Point Scam", &g_cfg.ragebot.hitscan_history[i]);
						auto multipoint = new C_MultiDropdown(" Multipoints", &g_cfg.ragebot.multipoint_hitboxes[i]);
					}
					else if (s == 1)
					{
						new C_SliderFloat(" Head", &g_cfg.ragebot.pointscalehead[i], 0, 0.3f, " %");
						new C_SliderFloat(" Neck", &g_cfg.ragebot.pointscaleneck[i], 0, 0.3f, " %");
						new C_SliderFloat(" Chest", &g_cfg.ragebot.pointscalechest[i], 0, 0.3f, " %");
						new C_SliderFloat(" Stomach", &g_cfg.ragebot.pointscalestomach[i], 0, 0.3f, " %");
						new C_SliderFloat(" Pelvis", &g_cfg.ragebot.pointscalepelvis[i], 0, 0.3f, " %");
						new C_SliderFloat(" Arms", &g_cfg.ragebot.pointscalearms[i], 0, 0.3f, " %");
						new C_SliderFloat(" Legs", &g_cfg.ragebot.pointscalelegs[i], 0, 0.3f, " %");
						new C_SliderFloat(" Other", &g_cfg.ragebot.pointscale[i], 0, 0.3f, " %");
					}
				}
			}
			auto weapon2 = new C_GroupBox(GROUP_RIGHT, 8, "");
			{
				new C_MultiDropdown(" Body Aim if", &g_cfg.ragebot.baim_settings[i]);
				new C_CheckBox(" Quick Stop", &g_cfg.ragebot.quickstop[i]);
				new C_CheckBox(" Auto Stop", &g_cfg.ragebot.autostopmovement[i]);
			}
		}
	}*/
	//auto rage_tab = new C_Tab(&menu::get(), 1, "aimbot", "aimbot");
	//{
	auto rage_tab = new C_Tab(&menu::get(), 1, "aimbot", "aimbot", {
		C_Tab::subtab_t("Main", 0),
		C_Tab::subtab_t("Weapon", 1)
		});
	{
		set_tabsub(0);
		auto aim_one = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto enable = new C_CheckBox(" enable", &g_cfg.ragebot.enable);

			auto autoshoot = new C_CheckBox(" enable autoshoot", &g_cfg.ragebot.autoshoot);

			auto silent_aim = new C_CheckBox(" silent aim", &g_cfg.ragebot.silent_aim);

			auto autowall = new C_CheckBox(" autowall", &g_cfg.ragebot.autowall);

			auto fov = new C_SliderInt(" FOV", &g_cfg.ragebot.field_of_view, 0, 360, "°");

			auto selection_type = new C_Dropdown(" select priority", &g_cfg.ragebot.selection_type, { "Field Of View (FOV) ", "Lowest Health ", "Closest Distance "  });

			auto hit_chance_b = new C_CheckBox(" hitchance enable ", &g_cfg.ragebot.hitchance);

			auto mindmg = new C_CheckBox(" minimal damage enable ", &g_cfg.ragebot.mindmg);

			auto angle = new C_CheckBox(" angle limit", &g_cfg.ragebot.aimstep);

			////
			auto hit_chance = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_chance, 0, 100, "  °");

			auto min_dmg = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_dmg, 0, 100, " ° ");

			auto point = new C_SliderInt(" multi points", &g_cfg.ragebot.pointscale, 0, 100, " °");

			auto hetscam_e = new C_CheckBox(" hitscan enable", &g_cfg.ragebot.hitscan_b);

			auto hitscan = new C_MultiDropdown(" hitbox", &g_cfg.ragebot.hitscan);

			auto hitscan_history = new C_MultiDropdown(" history hitbox", &g_cfg.ragebot.hitscan_history);

			auto multipoint = new C_MultiDropdown(" multi-point", &g_cfg.ragebot.multipoint_hitboxes);

			auto autoresolver = new C_CheckBox(" auto revolver", &g_cfg.ragebot.autoresolver);

			auto lag = new C_CheckBox(" lag compensation", &g_cfg.ragebot.lagcomp);

			auto ex = new C_CheckBox(" extrapolition", &g_cfg.ragebot.extrapolation);
		}
		auto aim_two = new C_GroupBox(GROUP_RIGHT, 8, "");
		{

			auto anti_recoil = new C_CheckBox(" compisale recoil", &g_cfg.ragebot.anti_recoil);

			auto anti_spread = new C_CheckBox(" compisale spread", &g_cfg.ragebot.anti_spread);

			new C_CheckBox(" correct anti-aim", &g_cfg.ragebot.resolver_experimental);

			new C_Dropdown(" correct anti-aim mode", &g_cfg.ragebot.resolver_experimentalmode, { "Clever Bruteforce", "Desync Resolver" });

			new C_CheckBox(" quick Stop", &g_cfg.ragebot.quickstop);

			new C_CheckBox(" auto Stop", &g_cfg.ragebot.autostop);

			new C_MultiDropdown(" prefer body aim", &g_cfg.ragebot.baim_settings);


		}
		set_tabsub(1);
		auto weapon_1 = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto _1 = new C_TextSeperator("              scar settings              ", TextSeperatorType::SEPERATOR_BOLD);

			auto hit_chance_1 = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_scar, 0, 100, "  °");

			auto min_dmg_1 = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_scar, 0, 100, "  °");

			auto _2 = new C_TextSeperator("              awp settings              ", TextSeperatorType::SEPERATOR_BOLD);

			auto hit_chance_2 = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_awp, 0, 100, " °");

			auto min_dmg_2 = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_awp, 0, 100, "  °");


			auto _3 = new C_TextSeperator("              other settings              ", TextSeperatorType::SEPERATOR_BOLD);

			auto hit_chance_3 = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_other, 0, 100, "  °");

			auto min_dmg_3 = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_other, 0, 100, "  °");

		}
		auto weapon_2 = new C_GroupBox(GROUP_RIGHT, 8, "");
		{
			auto _1 = new C_TextSeperator("              scout settings              ", TextSeperatorType::SEPERATOR_BOLD);

			auto hit_chance_1 = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_scout, 0, 100, " °");

			auto min_dmg_1 = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_scout, 0, 100, "  °");

			auto _2 = new C_TextSeperator("              pistols settings              ", TextSeperatorType::SEPERATOR_BOLD);

			auto hit_chance_2 = new C_SliderInt(" hitchance", &g_cfg.ragebot.hit_pistol, 0, 100, "  °");

			auto min_dmg_2 = new C_SliderInt(" minimal damage", &g_cfg.ragebot.min_pistol, 0, 100, "  °");
		}
	}

	auto antiaim_tab = new C_Tab(&menu::get(), 2, "anti-aims", "anti-aims");
	{
		auto antiaim = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto base_angle = new C_Dropdown(" base Angle", &g_cfg.antiaim.base_angle, { "Static", "Crosshair", "At Targets" });
			std::vector< std::string > pitchx{ "Off", "Down", "Up", "Minimal" };
			auto pitch = new C_Dropdown(" pitch", &g_cfg.antiaim.pitch, pitchx);
			std::vector< std::string > reals{ "Off", "Sideways", "Backwards", "Rotate", "Jitter", "Local View", "Lowerbody", "Auto Direction", "180Z" , "Clever Yaw" };
			auto yaw = new C_Dropdown(" real angle", &g_cfg.antiaim.yaw, reals);
			//auto yaw_offset = new C_SliderInt("", &g_cfg.antiaim.yaw_offset, -180.f, 180.f, "°");
			//std::vector< std::string > fakes{ "Off", "Static Desync", "Jitter Desync", "Spin Desync", "Shuffle Desync", "fat Desync","s Desync", };
			//auto fakeyaw = new C_Dropdown("desync yaw", &g_cfg.antiaim.fake_yaw, fakes);
			//auto fakeyaw_offset = new C_SliderInt("", &g_cfg.antiaim.fake_yaw_offset, -59.f, 59.f, "°");

			auto pitchchange = new C_CheckBox(" pitch up if hit ground", &g_cfg.antiaim.hittinggroundpitch);
			auto animation_fix = new C_CheckBox(" anim Fix", &g_cfg.antiaim.animfix);
		}
		auto anti_aim1 = new C_GroupBox(GROUP_RIGHT, 8, "");
		{
			auto fakelag_enablers = new C_MultiDropdown(" Fakelag", &g_cfg.antiaim.fakelag_enablers);

			auto fakelag_mode = new C_Dropdown("", &g_cfg.antiaim.fakelag_mode, { "off", "Factor", "Dynamic", "Switch", "Slow DANK", "Memez" });

			auto fakelag_amount = new C_SliderInt("", &g_cfg.antiaim.fakelag_amount, 1, 14, "");

			auto lcp = new C_CheckBox(" Break Lag Compinsation", &g_cfg.antiaim.breaklagcomp);

			auto fakelagkey = new C_KeyBind(" Fakelag if Key pressed", &g_cfg.antiaim.fakelagonkey);

			auto desync = new C_CheckBox(" Desync Enabled", &g_cfg.antiaim.desync);

			auto max = new C_SliderInt(" Desync Angle Max", &g_cfg.antiaim.desync_delta_swap_st, 0, 360.f, " °");

			auto delta = new C_SliderInt(" Desync Angle Delta", &g_cfg.antiaim.desync_delta_swap_mv, 0, 360.f, " °");

			auto des_b = new C_CheckBox(" Desync Binds", &g_cfg.antiaim.desync_melly);

			auto des_r = new C_KeyBind(" Desync Right", &g_cfg.antiaim.desync_r);

			auto des_l = new C_KeyBind(" Desync Left", &g_cfg.antiaim.desync_l);

			auto slow = new C_CheckBox(" Slow Walk", &g_cfg.fastwalk.enabled);
			auto swlo_key = new C_KeyBind(" Slow Walk Key", &g_cfg.fastwalk.key);
			auto slowamt = new C_SliderInt(" Slow Walk Speed", &g_cfg.fastwalk.speed, 10, 100, " °");
		}
	}
	auto esp_tab = new C_Tab(&menu::get(), 3, "visuals", "visuals", {
		C_Tab::subtab_t("Player", 0),
		});
	{
		set_tabsub(0);
		auto player = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			auto enable = new C_CheckBox("enable visuals", &g_cfg.player.enable);

			auto esp_enem = new C_TextSeperator("enemy esp", TextSeperatorType::SEPERATOR_BOLD);

			auto alpha = new C_SliderInt("alpha", &g_cfg.player.alpha, 0, 255, "");

			auto box = new C_CheckBox("bounding box", &g_cfg.player.box,
				{ new C_ColorSelector(&g_cfg.player.box_color) });

			auto health = new C_CheckBox("health", &g_cfg.player.health);

			auto name = new C_CheckBox("name", &g_cfg.player.name,
				{ new C_ColorSelector(&g_cfg.player.name_color) });

			auto weapon = new C_CheckBox("weapon", &g_cfg.player.weapon,
				{ new C_ColorSelector(&g_cfg.player.weapon_color) });

			auto ammo = new C_Dropdown("ammunition", &g_cfg.player.ammo, { "Off", "Bar" });

			auto ammobar_color = new C_TextSeperator("color bar", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.ammobar_color) });

			auto flags = new C_MultiDropdown("flags", &g_cfg.player.flags);

			auto skeleton = new C_MultiDropdown("skeleton", &g_cfg.player.skeleton);

			auto glow = new C_MultiDropdown("glow", &g_cfg.player.glow);

			auto glow_color_enemy = new C_TextSeperator("enemy color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_enemy) });


			auto glow_opacity = new C_SliderInt("", &g_cfg.player.glowopacity, 1, 100, " %");



			auto glow_bloom = new C_SliderInt("", &g_cfg.player.glowbloom, 1, 100, " %");

			auto glow_type = new C_Dropdown("", &g_cfg.player.glow_type, { "Outline Outer", "Cover", "Outline Inner" });

			auto chams = new C_MultiDropdown("chams", &g_cfg.player.chams);

			auto chams_color_visible = new C_TextSeperator("visible color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.chams_color) });

			auto chams_color_invisible = new C_TextSeperator("invisible color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.xqz_color) });

			auto chams_opacity = new C_SliderInt("", &g_cfg.player.chams_opacity, 0, 100, " %");

			auto chams_type = new C_Dropdown("", &g_cfg.player.chams_type, { "Regular", "Flat", "Rubber", "Metallic" });

			auto loca = new C_CheckBox(" enable local chams", &g_cfg.player.local);

			auto backtrack_chams_opacity = new C_SliderInt("", &g_cfg.player.backtrack_chams_opacity, 0, 100, " %");

			auto esp_tl = new C_TextSeperator("local & team esp", TextSeperatorType::SEPERATOR_BOLD);

			auto esp_glow = new C_TextSeperator("glow", TextSeperatorType::SEPERATOR_NORMAL);

			auto glow_color_teammate = new C_TextSeperator("teammate color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_teammate) });
			auto glow_color_local = new C_TextSeperator("local color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_local) });
		}
		auto player_2 = new C_GroupBox(GROUP_RIGHT, 8, "");
		{
			auto fov = new C_SliderInt("override fov", &g_cfg.esp.fov, 0, 90, "°");
			auto override_fov_scoped = new C_CheckBox("override fov when scoped", &g_cfg.esp.override_fov_scoped);
			auto transparency_scoped = new C_CheckBox("transparency when scoped", &g_cfg.esp.transparency_scoped);
			auto transparency_scoped_alpha = new C_SliderInt("", &g_cfg.esp.transparency_scoped_alpha, 0, 100, "%%");
			//auto show_spread = new C_CheckBox("draw spread", &g_cfg.esp.show_spread,
			//	{ new C_ColorSelector(&g_cfg.esp.show_spread_color) });
			//auto show_spread_type = new C_Dropdown("", &g_cfg.esp.show_spread_type, { "Regular", "Rainbow", "Seed" });
			auto penetration_reticle = new C_CheckBox("penetration reticle", &g_cfg.esp.penetration_reticle);
			auto ghost_chams = new C_CheckBox("desync chams", &g_cfg.esp.ghost_chams,
				{ new C_ColorSelector(&g_cfg.esp.ghost_chams_color) });
			//auto local_chams = new C_CheckBox("Local chams", &g_cfg.esp.local_chams,
			//	{ new C_ColorSelector(&g_cfg.esp.local_chams_color) });
			auto bullet_tracer = new C_CheckBox("bullet tracers", &g_cfg.esp.bullet_tracer,
				{ new C_ColorSelector(&g_cfg.esp.bullet_tracer_color) });
			auto bullet_tracer_enemy = new C_CheckBox("enemy bullet tracers", &g_cfg.esp.bullet_tracer_enemy,
				{ new C_ColorSelector(&g_cfg.esp.bullet_tracer_color_enemy) });
			auto bullet_tracerlifespan = new C_SliderInt("tracer time", &g_cfg.esp.bullet_tracer_duration, 1, 10, "");
			auto bullet_tracersize = new C_SliderInt("tracer size", &g_cfg.esp.bullet_tracer_size, 1, 10, "");
			auto removescope = new C_CheckBox("remove scope", &g_cfg.esp.removescope, { new C_ColorSelector(&g_cfg.menu.removescopecol[0]) });
			auto removescopesense = new C_CheckBox("remove scope sensitivity", &g_cfg.esp.removescopesensitivity);
			auto removezoom = new C_CheckBox("remove zoom effect", &g_cfg.esp.removezoom);
			auto removesmoke = new C_CheckBox("remove smokes", &g_cfg.esp.removesmoke);
			auto removeflash = new C_CheckBox("remove flashbang effect", &g_cfg.esp.removeflash);
			auto removerecoil = new C_CheckBox("remove visual recoil", &g_cfg.esp.removerecoil);

			auto dropped_weapon = new C_CheckBox("dropped weapons", &g_cfg.esp.dropped_weapons,
				{ new C_ColorSelector(&g_cfg.esp.dropped_weapons_color) });
			auto projectiles = new C_CheckBox("grenades", &g_cfg.esp.projectiles,
				{ new C_ColorSelector(&g_cfg.esp.projectiles_color) });
			auto dont_render_teammates = new C_CheckBox("invisible teammates", &g_cfg.esp.dont_render_teammates);
			auto nightmode = new C_CheckBox("nightmode", &g_cfg.esp.nightmode);
			auto pizdes = new C_Dropdown("sky box", &g_cfg.esp.skybox,
				{ "Tibet",
				"Baggage",
				"Monastery",
				"Italy/OldInferno",
				"Aztec",
				"Vertigo",
				"Daylight",
				"Daylight (2)",
				"Clouds",
				"Clouds (2)",
				"Gray",
				"Clear",
				"Canals",
				"Cobblestone",
				"Assault",
				"Clouds (Dark)",
				"Night",
				"Night (2)",
				"Night (Flat)",
				"Dusty",
				"Rainy",
				"Custom: Sunrise",
				"Custom: Galaxy",
				"Custom: Galaxy (2)",
				"Custom: Galaxy (3)",
				"Custom: Clouds (Night)",
				"Custom: Ocean",
				"Custom: Grimm Night",
				"Custom: Heaven",
				"Custom: Heaven (2)",
				"Custom: Clouds",
				"Custom: Night (Blue)" });

		//	auto hitmarker = new C_MultiDropdown("hitmarker", &g_cfg.esp.hitmarker);
			//auto hitsound_1 = new C_CheckBox("OneTap.su Hitmarker", &g_cfg.esp.onetap_hit);
			auto hitsound = new C_CheckBox("hitsound", &g_cfg.esp.hitsound);
			auto thirdperson = new C_KeyBind("thirdperson key", &g_cfg.misc.thirdperson_toggle);
			auto events_to_log = new C_MultiDropdown("event Log", &g_cfg.misc.events_to_log);
			auto crosshair = new C_CheckBox("crosshair", &g_cfg.esp.crosshair);
			auto sniper_crosshair = new C_CheckBox("sniper crosshair", &g_cfg.misc.sniper_crosshair);
			new C_CheckBox("Full bright", &g_cfg.esp.fullbright);
		}
	}
	/*auto misc_tab = new C_Tab(&menu::get(), 4, "misc", "misc");
	{
		auto miscellaneous = new C_GroupBox(GROUP_LEFT, 8, ""); {
			auto bunnyhop = new C_CheckBox("auto bunnyHop", &g_cfg.misc.bunnyhop);
			auto airstrafe = new C_CheckBox("auto strafe", &g_cfg.misc.airstrafe);
			auto circle_stafer = new C_KeyBind("circle strafe", &g_cfg.misc.circle_strafe);
			auto clantag = new C_TextSeperator("clanTag", TextSeperatorType::SEPERATOR_NORMAL);
			auto clantag_spammer = new C_CheckBox("enable clanTag", &g_cfg.misc.clantag_spammer);
			new C_Dropdown("clanTag modes", &g_cfg.misc.clantag_modes, { "supremacy"});
			
		}
		auto miscellaneous_2 = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto exaggerated_ragdoll = new C_CheckBox("exaggerated ragdolls", &g_cfg.misc.exaggerated_ragdolls);
			new C_SliderInt("raggol", &g_cfg.misc.exaggerated_ragdolls_multiplier, 2, 100, "x");
			auto ping_spike = new C_CheckBox("fake latency", &g_cfg.misc.ping_spike);
			auto ping_spike_value = new C_SliderInt("", &g_cfg.misc.ping_spike_value, 10, 999, "ms");

		}
	}*/
	auto configs_tab = new C_Tab(&menu::get(), 4, "misc", "misc");
	{
		auto config = new C_GroupBox(GROUP_LEFT, 8, "");
		{
			new C_TextSeperator("Menu Color", SEPERATOR_NORMAL, { new C_ColorSelector(&g_cfg.menu.menu_theme[0]) });
			new C_TextSeperator("Tab Text Color", SEPERATOR_NORMAL, { new C_ColorSelector(&g_cfg.menu.color_text_tab) });
			new C_TextSeperator("CheckBox Color", SEPERATOR_NORMAL, { new C_ColorSelector(&g_cfg.menu.check_box_color) });
			new C_KeyBind("menu key", &g_cfg.menu.menu_bind);
			new C_CheckBox("anti unstrusted", &g_cfg.misc.anti_untrusted);
			new C_CheckBox("custom viewmodel", &g_cfg.misc.customviewmodel);
			new C_SliderFloat("> x", &g_cfg.misc.CustomViewModelX, -10, 10, "");
			new C_SliderFloat("> y", &g_cfg.misc.CustomViewModelY, -10, 10, "");
			new C_SliderFloat("> z", &g_cfg.misc.CustomViewModelZ, -10, 10, "");
			auto buybot = new C_CheckBox("buy bot", &g_cfg.BuyBot.enable);
			auto buybot_primary = new C_MultiDropdown("primary", &g_cfg.BuyBot.primary);
			auto buybot_secondary = new C_MultiDropdown("secondary", &g_cfg.BuyBot.secondary);
			auto buybot_nades = new C_MultiDropdown("grenades", &g_cfg.BuyBot.nades);
			auto buybot_extras = new C_MultiDropdown("extras", &g_cfg.BuyBot.extras);

			auto bunnyhop = new C_CheckBox("auto bunnyHop", &g_cfg.misc.bunnyhop);
			auto airstrafe = new C_CheckBox("auto strafe", &g_cfg.misc.airstrafe);
			auto circle_stafer = new C_KeyBind("circle strafe", &g_cfg.misc.circle_strafe);
			auto clantag = new C_TextSeperator("clanTag", TextSeperatorType::SEPERATOR_NORMAL);
			auto clantag_spammer = new C_CheckBox("enable clanTag", &g_cfg.misc.clantag_spammer);
			new C_Dropdown("clanTag modes", &g_cfg.misc.clantag_modes, { "supremacy" });
		}
		auto config_2 = new C_GroupBox(GROUP_RIGHT, 8, "configs");
		{

			auto config_dropdown = new C_Dropdown("Presets", &g_cfg.selected_config, &cfg_manager->files);
			auto load = new C_Button("load configuration", []() { load_config(); });
			auto save = new C_Button("save configuration", []() { save_config(); });
			auto remove = new C_Button("delete configuration", []() { remove_config(); });
			new C_TextField("configuration name", &g_cfg.new_config_name);
			auto add = new C_Button("add new configuration", []() { add_config(); });
			new C_Button("unload cheat", []() { unload_cheat(); });
		}
	}
	auto Model_tab = new C_Tab(&menu::get(), 5, "models", "models",
		{
		C_Tab::subtab_t("models", 0),
		});
	{
		set_tabsub(0);
		auto modelchanger = new C_GroupBox(GROUP_LEFT, 8, "model changer");
		{
			new C_CheckBox("model changer enable", &g_cfg.models.Enabled);
			auto knifemodel = new C_Dropdown("knife model", &g_cfg.models.KnifeModel,
			{   
				"crystal fade",
				"minecraft pickaxe",
				"dildo"
			});
		}
		auto modelchanger2 = new C_GroupBox(GROUP_RIGHT, 8, "arms");
		{
			auto armmodel = new C_Dropdown("Arm Model", &g_cfg.models.ArmModel,
				{ "Ring Arms"});
		}
	}
	//auto skins_tab = new C_Tab(&menu::get(), 6, "Skins", "Skins",
	//	{
	//	C_Tab::subtab_t("Weapon", 0),
	//	C_Tab::subtab_t("Knifes & Glowes", 1)
	//	}); 
	//{
	//	set_tabsub(0);
	//	auto knifechanger = new C_GroupBox(GROUP_LEFT, 8, "Skin changer");
	//	{
	//		auto skinson = new C_CheckBox("Enable", &g_cfg.skins.skinenabled);


	//		auto awpns = new C_Dropdown("Awp Skins", &g_cfg.skins.AWPSkin,
	//			{ "none",
	//				"Rainbow",
	//				"Dragon Lore",
	//				"Fever Dream",
	//				"Medusa",
	//				"HyperBeast",
	//				"Boom",
	//				"Lightning Strike",
	//				"Pink DDpat",
	//				"Corticera",
	//				"Redline",
	//				"Manowar",
	//				"Graphite",
	//				"Electric Hive",
	//				"Sun in Leo",
	//				"Hyper Beast",
	//				"Pit viper",
	//				"Phobos",
	//				"Elite Build",
	//				"Worm God",
	//				"Oni Taiji",
	//				"Fever Dream" });
	//		auto scoutns = new C_Dropdown("SSG08 skins", &g_cfg.skins.SSG08Skin,
	//			{ "none",
	//				"Lichen Dashed",
	//				"Dark Water",
	//				"Blue Spruce",
	//				"Sand Dune",
	//				"Palm",
	//				"Mayan Dreams",
	//				"Blood in the Water",
	//				"Tropical Storm",
	//				"Acid Fade",
	//				"Slashed",
	//				"Detour",
	//				"Abyss",
	//				"Big Iron",
	//				"Necropos",
	//				"Ghost Crusader",
	//				"Dragonfire" });
	//		auto scarns = new C_Dropdown("Scar skins", &g_cfg.skins.SCAR20Skin,
	//			{ "none",
	//				"Splash Jam",
	//				"Storm",
	//				"Contractor",
	//				"Carbon Fiber",
	//				"Sand Mesh",
	//				"Palm",
	//				"Crimson Web",
	//				"Cardiac",
	//				"Army Sheen",
	//				"Cyrex",
	//				"Grotto",
	//				"Bloodsport" });
	//		auto glockns = new C_Dropdown("Glock skins", &g_cfg.skins.GlockSkin,
	//			{ "none",
	//				"Fade",
	//				"Dragon Tattoo",
	//				"Twilight Galaxy",
	//				"Wasteland Rebel",
	//				"Water Elemental",
	//				"Off World",
	//				"Weasel",
	//				"Royal Legion",
	//				"Grinder",
	//				"Steel Disruption",
	//				"Brass",
	//				"Ironwork",
	//				"Bunsen Burner",
	//				"Reactor" });
	//		auto G3sg1ns = new C_Dropdown("G3sg1 skins", &g_cfg.skins.G3sg1Skin,
	//			{ "none",
	//				"Hunter",
	//				"The Executioner",
	//				"Terrace",
	//			"Neon Kimono",
	//			"Orange Kimono",
	//			"Predator",
	//			"Chronos"
	//			});

	//		auto dualsns = new C_Dropdown("Duals skins", &g_cfg.skins.DualSkin,
	//			{ "none",
	//				"Panther",
	//				"Dualing Dragons",
	//				"Cobra Strike",
	//				"Royal Consorts",
	//				"Duelist" });

	//		auto deaglens = new C_Dropdown("Deagle skins", &g_cfg.skins.DeagleSkin,
	//			{ "none",
	//				"Blaze",
	//				"Kumicho Dragon",
	//				"Oxide Blaze",
	//				"Golden Koi",
	//				"Cobalt Disruption",
	//				"Directive" });



	//		auto uspns = new C_Dropdown("USP skins", &g_cfg.skins.USPSkin,
	//			{ "none",
	//				"Neo-Noir",
	//				"Cyrex",
	//				"Orion",
	//				"Kill Confirmed",
	//				"Overgrowth",
	//				"Caiman",
	//				"Serum",
	//				"Guardian",
	//				"Road Rash" });

	//	}
	//	auto knifechanger2222222 = new C_GroupBox(GROUP_RIGHT, 8, "");
	//	{
	//		auto ak = new C_Dropdown("AK47 skins", &g_cfg.skins.AK47Skin,
	//			{ "none",
	//				  "Fire serpent",
	//				"Fuel Injector",
	//			  "Bloodsport",
	//				  "Vulcan",
	//				 "Case hardened",
	//				  "Hydroponic",
	//				 "Aquamarine Revenge",
	//				 "Frontside Misty",
	//				 "Point Disarray",
	//				"Neon Revolution",
	//				"Red laminate",
	//			"Redline",
	//			 "Jaguar",
	//				 "Jetset",
	//				 "Wasteland rebel",
	//			 "The Empress",
	//				 "Elite Build" });
	//		auto P2000 = new C_Dropdown("P2000 skins", &g_cfg.skins.P2000Skin,
	//			{ "none",
	//			"Handgun",
	//			"Fade",
	//			"Corticera",
	//			"Ocean Foam",
	//				"Fire Elemental",
	//				"Asterion",
	//				"Pathfinder",
	//			"Imperial",
	//			"Oceanic",
	//				"Imperial Dragon",
	//			});
	//		auto uspns = new C_Dropdown("Revolver skins", &g_cfg.skins.RevolverSkin,
	//			{ "none",
	//				"Llama Cannon",
	//				"Fade",
	//				"Crimson Web",
	//				"Reboot",
	//				"Nitro",
	//				"Bone Mask"
	//			});
	//		auto forceupdate = new C_Button("Full Update", []() { KnifeApplyCallbk(); });
	//		set_tabsub(1);

	//		auto knifechanger2 = new C_GroupBox(GROUP_LEFT, 12, "KnifeChanger");
	//		{
	//			auto knifechanger = new C_CheckBox("Enable Knife Changer", &g_cfg.skins.knifes);
	//			auto knif = new C_Dropdown("Knife Model", &g_cfg.skins.Knife, { "Default",
	//				"Bayonet",
	//				"Flip Knife",
	//				"Gut Knife",
	//				"Karambit",
	//				"M9 Bayonet",
	//				"Huntsman Knife",
	//				"Butterfly Knife",
	//				"Falchion Knife",
	//				"Shadow Daggers",
	//				"Bowie Knife",
	//				"Navaja Knife",
	//				"Stiletto Knife",
	//				"Ursus Knife",
	//				"Talon Knife" });

	//			auto knifskin = new C_Dropdown("Knife Skin", &g_cfg.skins.KnifeSkin, { "none",
	//				"Crimson Web",
	//				"Bone Mask",
	//				"Fade",
	//				"Night",
	//				"Blue Steel",
	//				"Stained",
	//				"Case Hardened",
	//				"Slaughter",
	//				"Safari Mesh",
	//				"Boreal Forest",
	//				"Ultraviolet",
	//				"Urban Masked",
	//				"Scorched",
	//				"Rust Coat",
	//				"Tiger Tooth",
	//				"Damascus Steel",
	//				"Damascus Steel",
	//				"Marble Fade",
	//				"Rust Coat",
	//				"Doppler Ruby",
	//				"Doppler Sapphire",
	//				"Doppler Blackpearl",
	//				"Doppler Phase 1",
	//				"Doppler Phase 2",
	//				"Doppler Phase 3",
	//				"Doppler Phase 4",
	//				"Gamma Doppler Phase 1",
	//				"Gamma Doppler Phase 2",
	//				"Gamma Doppler Phase 3",
	//				"Gamma Doppler Phase 4",
	//				"Gamma Doppler Emerald",
	//				"Lore",
	//				"Black Laminate",
	//				"Autotronic",
	//				"Freehand" });

	//			auto forceupdate = new C_Button("Full Update", []() { KnifeApplyCallbk(); });
	//		}
	//		//auto knifechanger3 = new C_GroupBox(GROUP_RIGHT, 12, "GloveChanger(sooon)");
	//		//{
	//		//	//	auto glove = new C_Dropdown("Glove Model", &g_cfg.skins.glove, { "Default",
	//		//		//	"test"
	//		//			//	});
	//		//		//	auto gloves = new C_Dropdown("Glove Skin", &g_cfg.skins.gloves, { "Default",
	//		//			//	"test"
	//		//			//	});
	//		//		//	auto forceupdate = new C_Button("Full Update", []() { KnifeApplyCallbk(); });	
	//		//}
	//	}
	//}
}
#include "cmenu.h"
#include "bgtexture.h"
#include "bindsfont.h"

#include "../../Backend/Configuration/Config.h"
#include "../../Backend/Interfaces/Interfaces.h"
#include "../../Backend/lazyimports.h"
#include "../../Backend/Lua/CLua.h"
#include "../../Backend/Features/Ragebot/c_playerlist.h"

IDirect3DTexture9* imgxd = nullptr;
ImFont* tabs;
ImFont* tabsbig;
ImFont* tabsbig2;
ImFont* tabsbig3;
ImFont* tabssmall;
void c_menu::initialize(IDirect3DDevice9* pDevice) {
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	auto io = ui::GetIO();
	auto style = &ui::GetStyle();

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 6.f;
	style->WindowPadding = ImVec2(0, 0);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f);

	this->m_bIsOpened = true;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters(&param);
	pDevice->GetSwapChain(0, &pChain);

	if (pChain)
		pChain->GetPresentParameters(&pp);

	ImGui_ImplWin32_Init(param.hFocusWindow);
	ImGui_ImplDX9_Init(pDevice);

	D3DXCreateTextureFromFileInMemoryEx(
		pDevice, texture, sizeof(texture), 4096, 4096, D3DX_DEFAULT, NULL,
		pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->m_pTexture);

	ImFontConfig cfg;
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdana.ttf", 12.f, &cfg);
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/tahomabd.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabs = io.Fonts->AddFontFromMemoryTTF(skeet, 19248, 54.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabsbig = io.Fonts->AddFontFromMemoryTTF(skeet, 19248, 72.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabsbig2 = io.Fonts->AddFontFromMemoryTTF(skeet, 19248, 76.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabsbig3 = io.Fonts->AddFontFromMemoryTTF(skeet, 19248, 68.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	tabssmall = io.Fonts->AddFontFromMemoryTTF(skeet, 19248, 56.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdana.ttf", 12.f, &cfg);
	io.Fonts->AddFontDefault();

	ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00);

	this->m_bInitialized = true;
}

void c_menu::draw_begin() {
	if (!this->m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ui::NewFrame();
}

void dmt(std::string key) {
	if (c_config::get()->b["lua_devmode"] && ui::IsItemHovered()) {
		ui::BeginTooltip();
		ui::Text(key.c_str());
		ui::EndTooltip();
	}
}

void draw_lua_items(std::string tab, std::string container) {
	auto cfg = c_config::get();
	for (auto i : c_lua::get()->menu_items[tab][container]) {
		if (!i.is_visible)
			continue;

		auto type = i.type;
		switch (type)
		{
		case MENUITEM_CHECKBOX:
			if (ui::Checkbox(i.label.c_str(), &cfg->b[i.key])) {
				if (i.callback != sol::nil)
					i.callback(cfg->b[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_SLIDERINT:
			if (ui::SliderInt(i.label.c_str(), &cfg->i[i.key], i.i_min, i.i_max, i.format.c_str())) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_SLIDERFLOAT:
			if (ui::SliderFloat(i.label.c_str(), &cfg->f[i.key], i.f_min, i.f_max, i.format.c_str())) {
				if (i.callback != sol::nil)
					i.callback(cfg->f[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_KEYBIND:
			if (ui::Keybind(i.label.c_str(), &cfg->i[i.key], i.allow_style_change ? &cfg->i[i.key + "style"] : NULL)) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key], cfg->i[i.key + "style"]);
			}

			dmt(i.key + (i.allow_style_change ? " | " + i.key + "style" : ""));
			break;
		case MENUITEM_TEXT:
			ui::Text(i.label.c_str());
			break;
		case MENUITEM_SINGLESELECT:
			if (ui::SingleSelect(i.label.c_str(), &cfg->i[i.key], i.items)) {
				if (i.callback != sol::nil)
					i.callback(cfg->i[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_MULTISELECT:
			if (ui::MultiSelect(i.label.c_str(), &cfg->m[i.key], i.items)) {
				if (i.callback != sol::nil)
					i.callback(cfg->m[i.key]);
			}

			dmt(i.key);
			break;
		case MENUITEM_COLORPICKER:
			if (ui::ColorEdit4(i.label.c_str(), cfg->c[i.key])) {
				if (i.callback != sol::nil)
					i.callback(cfg->c[i.key][0] * 255, cfg->c[i.key][1] * 255, cfg->c[i.key][2] * 255, cfg->c[i.key][3] * 255);
			}

			dmt(i.key);
			break;
		case MENUITEM_BUTTON:
			if (ui::Button(i.label.c_str())) {
				if (i.callback != sol::nil)
					i.callback();
			}
			break;
		default:
			break;
		}
	}
}
/*int RoundUp1(int number) {
	number -= number % 20;
	return number + 20;
}*/

bool str_search_eq(const char* c_text, const char* c_search)
{
	auto text = std::string(c_text);
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	auto search = std::string(c_search);
	std::transform(search.begin(), search.end(), search.begin(), ::tolower);
	if (search != "" && text.find(search) == std::string::npos)
		return false;
	return true;
}

void c_menu::draw() {
	static int unload_start_time = INT_MAX;
	if ((int)GetTickCount() - unload_start_time >= 1000)
	{
		unload_start_time = INT_MAX - 1;
		Cheat::Settings->UnloadCheat = true;
	}
	if (unload_start_time != INT_MAX || Cheat::Settings->UnloadCheat)
		return;

	auto cfg = c_config::get();

	static int oldWeaponType = -1;
	if (!this->m_bIsOpened)
	{
		auto pWeapon = G::LocalPlayer->GetActiveWeapon();
		if (pWeapon != nullptr)
		{
			int weaponType = pWeapon->Type();
			if (weaponType >= 1 && weaponType <= 6)
			{
				if (oldWeaponType != weaponType)
				{
					oldWeaponType = weaponType;
					std::string legit_weapon_preset = std::to_string(weaponType - 1);
					cfg_preset_load(cfg->b, "legit_aimbot_enabled", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_aimbot_key", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_aimbot_keystyle", legit_weapon_preset);
					cfg_preset_load(cfg->f, "legit_aimbot_speed", legit_weapon_preset);
					cfg_preset_load(cfg->f, "legit_aimbot_attack_speed", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_aimbot_speed_scale", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_aimbot_lock_on_time", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_aimbot_delay", legit_weapon_preset);
					cfg_preset_load(cfg->f, "legit_aimbot_fov", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_rcs_pitch", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_rcs_yaw", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_quickstop", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_quickstop_trigger_shooting", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_aimbot_smoke", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_aimbot_blind", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_aimbot_head", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_aimbot_chest", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_aimbot_stomach", legit_weapon_preset);

					cfg_preset_load(cfg->b, "legit_trigger_enabled", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_key", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_keystyle", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_hitchance", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_delay", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_burst_fire", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_burst_fire_time", legit_weapon_preset);
					cfg_preset_load(cfg->i, "legit_trigger_mindamage", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_autowall", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_smoke", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_blind", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_head", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_chest", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_trigger_stomach", legit_weapon_preset);

					cfg_preset_load(cfg->i, "legit_backtrack", legit_weapon_preset);
					cfg_preset_load(cfg->f, "legit_backtrack_range", legit_weapon_preset);
					cfg_preset_load(cfg->b, "legit_other_standalone_rcs", legit_weapon_preset);
				}
			}
		}
	}
	else
		oldWeaponType = -1;

	if (!this->m_bIsOpened && ui::GetStyle().Alpha > 0.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha - fc / 255.f, 0.f, 1.f);
	}

	if (this->m_bIsOpened && ui::GetStyle().Alpha < 1.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha + fc / 255.f, 0.f, 1.f);
	}

	if (!this->m_bIsOpened && ui::GetStyle().Alpha == 0.f)
		return;
	int screensizex;
	int screensizey;
	static bool resizing = false;
	static ImVec2 oldmenupos = ImVec2(0, 0);
	static bool resizing_aim = false;
	Interfaces::Engine->GetScreenSize(screensizex, screensizey);
	ui::SetNextWindowSizeConstraints(ImVec2(660, 560), ImVec2(screensizex, screensizey));
	if (resizing)
		ui::SetNextWindowPos(oldmenupos);

	ui::Begin("gamesense", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	if (ui::GetWindowSize().y > 630) {
		ui::PushFont(tabssmall);
		ui::TabButton("A", &this->m_nCurrentTab, 0, 9);
		ui::PopFont();
		ui::PushFont(tabsbig3);
		ui::TabButton("B", &this->m_nCurrentTab, 1, 9);
		ui::PopFont();
		ui::PushFont(tabsbig);
		ui::TabButton("C", &this->m_nCurrentTab, 2, 9);
		ui::PopFont();
		ui::PushFont(tabsbig2);
		ui::TabButton("D", &this->m_nCurrentTab, 3, 9);
		ui::PopFont();
		ui::PushFont(tabs);
		ui::TabButton("E", &this->m_nCurrentTab, 4, 9);
		ui::TabButton("F", &this->m_nCurrentTab, 5, 9);
		ui::PopFont();
		ui::PushFont(tabssmall);
		ui::TabButton("G", &this->m_nCurrentTab, 6, 9);
		ui::PopFont();
		ui::PushFont(tabs);
		ui::TabButton("H", &this->m_nCurrentTab, 7, 9);
		ui::TabButton("I", &this->m_nCurrentTab, 8, 9);
		ui::PopFont();
	}
	else {
		ui::PushFont(tabssmall);
		ui::TabButton("A", &this->m_nCurrentTab, 0, 8);
		ui::PopFont();
		ui::PushFont(tabsbig3);
		ui::TabButton("B", &this->m_nCurrentTab, 1, 8);
		ui::PopFont();
		ui::PushFont(tabsbig);
		ui::TabButton("C", &this->m_nCurrentTab, 2, 8);
		ui::PopFont();
		ui::PushFont(tabsbig2);
		ui::TabButton("D", &this->m_nCurrentTab, 3, 8);
		ui::PopFont();
		ui::PushFont(tabs);
		ui::TabButton("E", &this->m_nCurrentTab, 4, 8);
		ui::TabButton("F", &this->m_nCurrentTab, 5, 8);
		ui::PopFont();
		ui::PushFont(tabssmall);
		ui::TabButton("G", &this->m_nCurrentTab, 6, 8);
		ui::PopFont();
		ui::PushFont(tabs);
		ui::TabButton("H", &this->m_nCurrentTab, 7, 8);
		ui::PopFont();
	}
	static auto calculateChildWindowPosition = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 120  + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 20);
	};

	static auto calculateChildWindowPositionDouble = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 100 + (ui::GetWindowSize().x / 2 - 70) * num + 20 * num, ui::GetWindowPos().y + 20);
	};

	auto child_size = ImVec2(ui::GetWindowSize().x / 2 - 70, ui::GetWindowSize().y - 40);

	/*if ((p.x >= calculateChildWindowPositionDouble(0).x + child_size.x - 2 && p.x  <= calculateChildWindowPositionDouble(0).x + child_size.x + 2 || resizing_aim) && GetAsyncKeyState(VK_LBUTTON)) {
		resizing = true;
		oldmenupos = ui::GetWindowPos();
		if ((p.y >= calculateChildWindowPositionDouble(0).y + child_size.y - 2 && p.y <= calculateChildWindowPositionDouble(0).y + child_size.y + 2 || resizing_aim)) {
			resizing_aim = true;
			child_size.x = Utilities->Util_Clamp(RoundUp1((int)p.x) - (int)calculateChildWindowPositionDouble(0).x, (int)(calculateChildWindowPositionDouble(0).x / 4), (int)ui::GetWindowSize().x - 120);
			child_size.y = Utilities->Util_Clamp(RoundUp1((int)p.y) - (int)calculateChildWindowPositionDouble(0).y, (int)calculateChildWindowPositionDouble(0).y / 4, (int)ui::GetWindowSize().y - 40);
			//child_size.x = p.x - calculateChildWindowPositionDouble(0).x;
			//child_size.y = p.y - calculateChildWindowPositionDouble(0).y;
		}
		else {
			resizing_aim = false;
		}
	}
	else {
		resizing_aim = false;
		resizing = false;
	}*/

	if (this->m_nCurrentTab == 0) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Aimbot", child_size, resizing_aim);

		ui::Checkbox("Enabled", &cfg->b["rage_aimbot"]); dmt("rage_aimbot");
		ui::Keybind("##aimkey", &cfg->i["rage_aimbot_key"], &cfg->i["rage_aimbot_keystyle"]); dmt("rage_aimbot_key | rage_aimbot_keystyle");

		ui::SingleSelect("Target selection", &cfg->i["rage_selectionmode"], { "Cycle", "Cycle (2x)", "Near crosshair", "Highest damage", "Lowest ping", "Best K/D ratio", "Best hit chance" }); dmt("rage_selectionmode");
		ui::MultiSelect("Target hitbox", &cfg->m["rage_points"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" }); dmt("rage_points");

		ui::MultiSelect("Multi-point", &cfg->m["rage_multipoint"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" }); dmt("rage_multipoint");
		ui::Keybind("##multipointkey", &cfg->i["rage_multipoint_key"], &cfg->i["rage_multipoint_keystyle"]); dmt("rage_multipoint | rage_multipoint_keystyle");
		ui::SingleSelect("##Multipoint", &cfg->i["rage_multipoint_prec"], { "Low", "Medium", "High" }); dmt("rage_multipoint_prec");

		if (cfg->m["rage_multipoint"][0] || cfg->m["rage_multipoint"][1] || cfg->m["rage_multipoint"][2] || cfg->m["rage_multipoint"][3] || cfg->m["rage_multipoint"][4]) {
			ui::SliderInt("Multi-point scale", &cfg->i["rage_headselection_scale"], 0, 100, "%d%%");
		}

		ui::Checkbox("Prefer safe point", &cfg->b["rage_safepoint"]); dmt("rage_safepoint");
		ui::Text("Force safe point");
		ui::Keybind("##forcesafepoint", &cfg->i["rage_safepoint_key"], &cfg->i["rage_safepoint_keystyle"]); dmt("rage_safepoint_key | rage_safepoint_keystyle");

		if (cfg->m["rage_points"][3] || cfg->m["rage_points"][4] || cfg->m["rage_points"][5])
			ui::Checkbox("Force safe point on limbs", &cfg->b["rage_safepoint_limbs"]); dmt("rage_safepoint_limbs");

		//ui::Checkbox("Friendly fire", &cfg->b["rage_friendlyfire"]); dmt("rage_friendlyfire");
		ui::Checkbox("Automatic fire", &cfg->b["rage_autofire"]); dmt("rage_autofire");
		ui::Checkbox("Automatic penetration", &cfg->b["rage_automaticpenetration"]); dmt("rage_automaticpenetration");

		ui::Checkbox("Silent aim", &cfg->b["rage_silentaim"]); dmt("rage_silentaim");
		ui::SliderInt("Minimum hit chance", &cfg->i["rage_hitchance_value1"], 0, 100, cfg->i["rage_hitchance_value1"] == 0 ? "Off" : "%d%%"); dmt("rage_hitchance_value1");
		ui::SliderInt("Minimum damage", &cfg->i["rage_minimaldamage1"], 0, 126, cfg->i["rage_minimaldamage1"] == 0 ? "Auto" : (cfg->i["rage_minimaldamage1"] > 100 ? "HP+%d" : "%d"), cfg->i["rage_minimaldamage1"] > 100 ? 100 : 0); dmt("rage_minimaldamage1");
		ui::Checkbox("Automatic scope", &cfg->b["rage_autoscope"]); dmt("rage_autoscope");
		ui::Checkbox("Reduce aim step", &cfg->b["rage_aimstep"]); dmt("rage_aimstep");
		ui::SliderInt("Maximum FOV", &cfg->i["rage_fov"], 0, 180, u8"%d°"); dmt("rage_fov");
		ui::Checkbox("Log misses due to spread", &cfg->b["rage_logmisses"]); dmt("rage_logmisses");
		ui::MultiSelect("Low FPS mitigations", &cfg->m["rage_lowfps"], { "Force low accuracy boost", "Disable multipoint: feet", "Disable multipoint: arms", "Disable multipoint: legs", "Disable hitbox: feet", "Force low multipoint", "Lower hit-chance precisio", "Limit targets per tick" }); dmt("rage_lowfps");
		//ui::MultiSelect("##targetpoints", &cfg->m["rage_points"], { "Head", "Neck", "Chest", "Stomach", "Arms", "Legs" }); dmt("rage_points");
		
		/*if (cfg->b["rage_multipoint"]) {
			if (cfg->m["rage_points"][0]) {
				ui::SliderInt("Head", &cfg->i["rage_headselection_scale"], 0, 100, "%d%%");
				dmt("rage_headselection_scale");
			}

			if (cfg->m["rage_points"][1]) {
				ui::SliderInt("Neck", &cfg->i["rage_neckselection_scale"], 0, 100, "%d%%");
				dmt("rage_neckselection_scale");
			}

			if (cfg->m["rage_points"][2]) {
				ui::SliderInt("Chest", &cfg->i["rage_chestselection_scale"], 0, 100, "%d%%");
				dmt("rage_chestselection_scale");
			}

			if (cfg->m["rage_points"][3]) {
				ui::SliderInt("Stomach", &cfg->i["rage_pelvisselection_scale"], 0, 100, "%d%%");
				dmt("rage_pelvisselection_scale");
			}

			if (cfg->m["rage_points"][4]) {
				ui::SliderInt("Arms", &cfg->i["rage_armsselection_scale"], 0, 100, "%d%%");
				dmt("rage_armsselection_scale");
			}

			if (cfg->m["rage_points"][5]) {
				ui::SliderInt("Legs", &cfg->i["rage_legsselection_scale"], 0, 100, "%d%%");
				dmt("rage_legsselection_scale");
			}
		}*/

		draw_lua_items("rage", "aimbot");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Other", child_size);
		//ui::CheckboxYellow("Remove spread", &cfg->b["rage_removespread"]); dmt("rage_removespread");
		ui::Checkbox("Remove recoil", &cfg->b["rage_removerecoil"]); dmt("rage_removerecoil");
		ui::SingleSelect("Accuracy boost", &cfg->i["rage_backtrack"], {"Off", "Low", "Medium", "High", "Maximum"}); dmt("rage_backtrack");
		ui::Checkbox("Delay shot", &cfg->b["rage_delayshot"]); dmt("rage_delayshot");
		ui::Checkbox("Quick stop", &cfg->b["rage_quickstop"]); dmt("rage_quickstop");

		if (cfg->b["rage_quickstop"]) {
			ui::MultiSelect("Quick stop options", &cfg->m["rage_quickstop_options"], { "Early", "Slow motion", "Duck", "Fake duck", "Move between shots", "Ignore molotov" }); dmt("rage_quickstop_options");
			ui::Checkbox("Quick peek assist", &cfg->b["rage_quickpeek"]); dmt("rage_quickpeek");
			ui::Keybind("quickpeek", &cfg->i["rage_quickpeek_key"], &cfg->i["rage_quickpeek_keystyle"]); dmt("rage_quickpeek_key | rage_quickpeek_keystyle");

			if (cfg->b["rage_quickpeek"]) {
				ui::Text("Quick peek assist display");
				ui::ColorEdit4("##quickpeek", cfg->c["rage_quickpeek_color"]); dmt("rage_quickpeek_color");
			}
		}
			

		ui::Checkbox("Anti-aim correction", &cfg->b["rage_resolver"]); dmt("rage_resolver");
		if (cfg->b["rage_resolver"]) {
			ui::Text("Anti-aim correction override");
			ui::Keybind("##overrideresolver", &cfg->i["rage_override_key"], &cfg->i["rage_override_keystyle"]); dmt("rage_override_key | rage_override_keystyle");
		}
		/*if (cfg->b["rage_resolver"]) {
			ui::Checkbox("Fake-lag correction", &cfg->b["rage_lagcomp"]); dmt("rage_lagcomp");
		}*/
	 {
			ui::Checkbox("Prefer body aim", &cfg->b["rage_baimpreference"]); dmt("rage_baimpreference");
			if (cfg->b["rage_baimpreference"])
				ui::MultiSelect("Prefer body aim disablers", &cfg->m["rage_baimdisablers"], { "Low inaccuracy", "Target shot fired", "Target resolved", "Safe point headshot", "Low damage" }); dmt("rage_baimdisablers");
			ui::Text("Force body aim");
			ui::Keybind("##forcebaim", &cfg->i["rage_forcebaim_key"], &cfg->i["rage_forcebaim_keystyle"]); dmt("rage_forcebaim_key | rage_forcebaim_keystyle");
			ui::Checkbox("Force body aim on peek", &cfg->b["rage_baimonpeek"]); dmt("rage_baimonpeek");
		}

		ui::Checkbox("Duck peek assist", &cfg->b["rage_fakeduck"]); dmt("rage_fakeduck");
		ui::Keybind("##bottlesitter", &cfg->i["rage_fakeduck_key"], &cfg->i["rage_fakeduck_keystyle"]); dmt("rage_fakeduck_key | rage_fakeduck_keystyle");
		
		ui::CheckboxYellow("Double tap", &cfg->b["rage_doubletap"]); dmt("rage_doubletap");
		ui::Keybind("##exploitbind", &cfg->i["rage_exploit_key"], &cfg->i["rage_exploit_keystyle"]); dmt("rage_exploit_key | rage_exploit_keystyle");
		if (cfg->b["rage_doubletap"]) {
			ui::SingleSelect("Double tap mode", &cfg->i["rage_doubletap_mode"], { "Offensive", "Defensive" }); dmt("rage_doubletap_mode");
			ui::SliderInt("Double tap hit chance", &cfg->i["rage_doubletap_hitchance"], 0, 100, u8"%d%%"); dmt("rage_doubletap_hitchance");
			ui::SliderInt("Double tap fake lag limit", &cfg->i["rage_doubletap_fakelag"], 1, 10, u8"%d"); dmt("rage_doubletap_fakelag");
			ui::SingleSelect("Double tap quick stop", &cfg->i["rage_doubletap_quickstop"], { "Slow motion", "Duck", "Move between shots" }); dmt("rage_doubletap_quickstop");
		}

		draw_lua_items("rage", "other");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 1) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Anti-aimbot angles", child_size);
		ui::Checkbox("Enabled", &cfg->b["aa_enabled"]); dmt("aa_enabled");
		ui::SingleSelect("Pitch", &cfg->i["rage_antiaim_pitch"], { "Off", "Default", "Up", "Down", "Minimal", "Random" }); dmt("rage_antiaim_pitch");
		//ui::SingleSelect("Yaw base", &cfg->i["rage_antiaim_yawbase"], { "Local view", "At targets" }); dmt("rage_antiaim_yawbase");
		ui::SingleSelect("Yaw", &cfg->i["rage_antiaim_yaw"], { "Off", "180", "Spin", "Static", "180 Z", "Crosshair" }); dmt("rage_antiaim_yaw");

		if (cfg->i["rage_antiaim_yaw"] > 0)
			ui::SliderInt("##yawaddy", &cfg->i["rage_antiaim_yaw_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_custom");

		if (cfg->i["rage_antiaim_yaw"] > 0)
		ui::SingleSelect("Yaw jitter", &cfg->i["rage_antiaim_yaw_jitter"], { "Off", "Offset", "Center", "Random"}); dmt("rage_antiaim_yaw_jitter");

		if (cfg->i["rage_antiaim_yaw_jitter"] > 0)
			ui::SliderInt("##yawaddy123", &cfg->i["rage_antiaim_yaw_jitter_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_jitter_custom");

		ui::SingleSelect("Body yaw", &cfg->i["rage_antiaim_desync"], { "Off","Opposite", "Jitter", "Static"}); dmt("rage_antiaim_desync");

		if (cfg->i["rage_antiaim_desync"] > 1)
			ui::SliderInt("##yawaddy321", &cfg->i["rage_antiaim_yaw_desync_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_desync_custom");

		ui::SingleSelect("Lower body yaw target", &cfg->i["rage_antiaim_lby"], { "Off","Sway", "Opposite", "Eye yaw"}); dmt("rage_antiaim_lby");

		/*if (cfg->i["rage_antiaim_desync"] > 0)
			ui::Checkbox("Freestand body yaw", &cfg->b["rage_desyncfreestand"]); dmt("rage_desyncfreestand");*/

		if (cfg->i["rage_antiaim_desync"] > 0)
			ui::SliderInt("Fake yaw limit", &cfg->i["rage_antiaim_yaw_desync_limit"], 0, 60, u8"%d°"); dmt("rage_antiaim_yaw_desync_limit");

		ui::Text("Body yaw inverter");
		ui::Keybind("##inverter", &cfg->i["rage_inverter_key"], &cfg->i["rage_inverter_keystyle"]); dmt("rage_inverter_key | rage_inverter_keystyle");
		//ui::SliderInt("Jitter min", &cfg->i["rage_antiaim_yaw_jitter_min"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_jitter_min");
		//ui::SliderInt("Jitter max", &cfg->i["rage_antiaim_yaw_jitter_max"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_jitter_max");

		//ui::SingleSelect("Yaw move", &cfg->i["rage_antiaim_yaw_move"], { "Off", "180", "Jitter", "Static" }); dmt("rage_antiaim_yaw_move");
		//ui::SliderInt("##yawaddymov", &cfg->i["rage_antiaim_yaw_move_custom"], -180, 180, u8"%d°"); dmt("rage_antiaim_yaw_move_custom");
		//ui::SliderInt("Move jitter min", &cfg->i["rage_antiaim_yaw_move_jitter_min"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_move_jitter_min");
		//ui::SliderInt("Move jitter max", &cfg->i["rage_antiaim_yaw_move_jitter_max"], -90, 90, u8"%d°"); dmt("rage_antiaim_yaw_move_jitter_max");
		
		//ui::Checkbox("Freestanding", &cfg->b["rage_antiaim_freestand"]);dmt("rage_antiaim_freestand");
		//ui::MultiSelect("Freestanding conditions", &cfg->m["rage_antiaim_freestand"], { "Default", "Moving", "Edge" });dmt("rage_antiaim_freestand");

		//ui::Checkbox("flip desync", &cfg->b["rage_flip"]); dmt("rage_flip");
		//ui::Keybind("flip", &cfg->i["rage_flip_key"], &cfg->i["rage_flip_keystyle"]); dmt("rage_flip_key | rage_flip_keystyle");

		
		//ui::Checkbox("Show AA", &cfg->b["rage_aainfo"]); dmt("rage_aainfo");
		/*if (cfg->b["rage_slowmotion"]) {
			ui::SliderInt("##speed", &cfg->i["rage_slowmotion_speed"], 0, 100, "%d%%");
			dmt("rage_slowmotion_speed");
		}*/

		

		draw_lua_items("aa", "anti-aimbot angles");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Fake lag", ImVec2(child_size.x, child_size.y * 0.7 - 15));

		ui::Checkbox("Enabled", &cfg->b["rage_fakelagenable"]); dmt("rage_fakelagenable");
		ui::Checkbox("Customize triggers", &cfg->b["rage_fakelag_customize"]); dmt("rage_fakelag_customize");
		if (cfg->b["rage_fakelag_customize"])
			ui::MultiSelect("Triggers", &cfg->m["rage_fakelag_conds"], { "On high speed", "On jump", "On duck", "On stand", "On accelerate", "On enemy visible", "While enemy visible", "While moving", "While climbing ladder" }); dmt("rage_fakelag_conds");
		ui::SingleSelect("Amount", &cfg->i["rage_fakelag"], { "Maximum", "Dynamic", "Fluctuate" }); dmt("rage_fakelag");
		ui::SliderInt("Variance", &cfg->i["rage_fakelag_variance"], 0, 100, "%d%%"); dmt("rage_fakelag_variance");
		ui::SliderInt("Limit", &cfg->i["rage_fakelag_limit"], 0, 14, "%d"); dmt("rage_fakelag_limit");
		ui::Checkbox("Fake lag while shooting", &cfg->b["rage_fakelagonshot"]); dmt("rage_fakelagonshot");
		ui::Checkbox("Reset on bunny hop", &cfg->b["rage_fakelagreset"]); dmt("rage_fakelagreset");
		
		draw_lua_items("aa", "fake lag");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y + child_size.y * 0.7));
		ui::BeginChild("Other", ImVec2(child_size.x, child_size.y * 0.3));
		ui::Checkbox("Slow motion", &cfg->b["rage_slowmotion"]); dmt("rage_slowmotion");
		ui::Keybind("slowmotion", &cfg->i["rage_slowmotion_key"], &cfg->i["rage_slowmotion_keystyle"]); dmt("rage_slowmotion_key | rage_slowmotion_keystyle");
		ui::SingleSelect("Slow motion type", &cfg->i["rage_tye"], { "Favor high speed","Favor anti-aim" }); dmt("rage_tye");
		ui::SingleSelect("Leg movement", &cfg->i["rage_fixleg"], { "Off","Always slide", "Never slide" }); dmt("rage_fixleg");
		ui::CheckboxYellow("On shot anti-aim", &cfg->b["rage_onshot"]); dmt("rage_onshot");
		ui::CheckboxYellow("Fake peek", &cfg->b["rage_fakepeek"]); dmt("rage_fakepeek");
		draw_lua_items("aa", "other");
		ui::EndChild();
	}
	if (this->m_nCurrentTab == 2) {
		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(0).x, calculateChildWindowPositionDouble(0).y));
		ui::BeginChild("Weapon type", ImVec2(child_size.x * 2 + 20, 80));
		ui::SingleSelect("Weapon selection", &cfg->i["legit_weapon_preset"], { "Pistol","SMG", "Rifle", "Shotgun", "Machine gun", "Sniper" }); dmt("legit_weapon_preset");
		std::string legit_weapon_preset = std::to_string(cfg->i["legit_weapon_preset"]);
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(0).x, calculateChildWindowPositionDouble(0).y + 80 + 15));
		ui::BeginChild("Aimbot", ImVec2(child_size.x, child_size.y - 80 - 15));
		ui::Checkbox("Enabled", &cfg->b["legit_aimbot_enabled" + legit_weapon_preset]); dmt("legit_aimbot_enabled");
		ui::Keybind("##aimbotenabled", &cfg->i["legit_aimbot_key" + legit_weapon_preset], &cfg->i["legit_aimbot_keystyle" + legit_weapon_preset]); dmt("legit_aimbot_key | legit_aimbot_keystyle");
		/*char speedformat[32];
		if (cfg->i["legit_aimbot_speed"] < 9)
			sprintf(speedformat, "0.0%i", cfg->i["legit_aimbot_speed"]);
		else if (cfg->i["legit_aimbot_speed"] < 99)
			sprintf(speedformat, "0.%i", cfg->i["legit_aimbot_speed"]);
		else if (cfg->i["legit_aimbot_speed"] < 199)
			sprintf(speedformat, "1.%i", cfg->i["legit_aimbot_speed"] - 100);
		else if (cfg->i["legit_aimbot_speed"] < 299)
			sprintf(speedformat, "2.%i", cfg->i["legit_aimbot_speed"] - 200);
		else if (cfg->i["legit_aimbot_speed"] < 399)
			sprintf(speedformat, "3.%i", cfg->i["legit_aimbot_speed"] - 300);
		else if (cfg->i["legit_aimbot_speed"] < 499)
			sprintf(speedformat, "4.%i", cfg->i["legit_aimbot_speed"] - 400);
		else
			sprintf(speedformat, "5.00");
		ui::SliderInt("Speed", &cfg->i["legit_aimbot_speed"], 1, 500, speedformat); dmt("legit_aimbot_speed");

		char speedattackformat[32];
		if (cfg->i["legit_aimbot_inattack_speed"] < 9)
			sprintf(speedattackformat, "0.0%i", cfg->i["legit_aimbot_inattack_speed"]);
		else if (cfg->i["legit_aimbot_inattack_speed"] < 99)
			sprintf(speedattackformat, "0.%i", cfg->i["legit_aimbot_inattack_speed"]);
		else if (cfg->i["legit_aimbot_inattack_speed"] < 199)
			sprintf(speedattackformat, "1.%i", cfg->i["legit_aimbot_inattack_speed"] - 100);
		else if (cfg->i["legit_aimbot_inattack_speed"] < 299)
			sprintf(speedattackformat, "2.%i", cfg->i["legit_aimbot_inattack_speed"] - 200);
		else if (cfg->i["legit_aimbot_inattack_speed"] < 399)
			sprintf(speedattackformat, "3.%i", cfg->i["legit_aimbot_inattack_speed"] - 300);
		else if (cfg->i["legit_aimbot_inattack_speed"] < 499)
			sprintf(speedattackformat, "4.%i", cfg->i["legit_aimbot_inattack_speed"] - 400);
		else
			sprintf(speedattackformat, "5.00");
		ui::SliderInt("Speed (in attack)", &cfg->i["legit_aimbot_inattack_speed"], 1, 500, speedattackformat); dmt("legit_aimbot_inattack_speed");*/
		ui::SliderFloat("Speed", &cfg->f["legit_aimbot_speed" + legit_weapon_preset], 0.f, 7.f, "%.2f"); dmt("legit_aimbot_speed");
		ui::SliderFloat("Speed (in attack)", &cfg->f["legit_aimbot_attack_speed" + legit_weapon_preset], 0.f, 7.f, "%.2f"); dmt("legit_aimbot_attack_speed");
		ui::SliderInt("Speed scale - FOV", &cfg->i["legit_aimbot_speed_scale" + legit_weapon_preset], 1, 100, "%d%%"); dmt("legit_aimbot_speed_scale");
		auto lalot = "legit_aimbot_lock_on_time" + legit_weapon_preset;
		ui::SliderInt("Maximum lock-on time", &cfg->i[lalot], 20, 1001, (cfg->i[lalot] > 1000) ? u8"°°" : "%dms"); dmt("legit_aimbot_lock_on_time");
		ui::SliderInt("Reaction time", &cfg->i["legit_aimbot_delay" + legit_weapon_preset], 0, 200, "%dms"); dmt("legit_aimbot_delay");
		ui::SliderFloat("Maximum FOV", &cfg->f["legit_aimbot_fov" + legit_weapon_preset], 0, 90, u8"%.1f°"); dmt("legit_aimbot_fov");
		ui::SliderInt("Recoil compensation (P/Y)", &cfg->i["legit_rcs_pitch" + legit_weapon_preset], 0, 125, "%d%%"); dmt("legit_rcs_pitch");
		ui::SliderInt("##recoilyaw", &cfg->i["legit_rcs_yaw" + legit_weapon_preset], 0, 100, "%d%%"); dmt("legit_rcs_yaw");
		ui::Checkbox("Quick stop", &cfg->b["legit_quickstop" + legit_weapon_preset]); dmt("legit_quickstop");
		// ui::Checkbox("Always quick stop while shooting", &cfg->b["legit_quickstop_trigger_shooting" + legit_weapon_preset]); dmt("legit_quickstop_trigger_shooting");
		ui::Checkbox("Aim through smoke", &cfg->b["legit_aimbot_smoke" + legit_weapon_preset]); dmt("legit_aimbot_smoke");
		ui::Checkbox("Aim while blind", &cfg->b["legit_aimbot_blind" + legit_weapon_preset]); dmt("legit_aimbot_blind");
		ui::Checkbox("Head", &cfg->b["legit_aimbot_head" + legit_weapon_preset]); dmt("legit_aimbot_head");
		ui::Checkbox("Chest", &cfg->b["legit_aimbot_chest" + legit_weapon_preset]); dmt("legit_aimbot_chest");
		ui::Checkbox("Stomach", &cfg->b["legit_aimbot_stomach" + legit_weapon_preset]); dmt("legit_aimbot_stomach");
		draw_lua_items("legit", "aimbot");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y + 80 + 15));
		ui::BeginChild("Triggerbot", ImVec2(child_size.x, child_size.y * 0.7 - 80));
		ui::Checkbox("Enabled", &cfg->b["legit_trigger_enabled" + legit_weapon_preset]); dmt("legit_trigger_enabled");
		ui::Keybind("##triggerenabled", &cfg->i["legit_trigger_key" + legit_weapon_preset], &cfg->i["legit_trigger_keystyle" + legit_weapon_preset]); dmt("legit_trigger_key | legit_trigger_keystyle");
		ui::SliderInt("Minimum hit chance", &cfg->i["legit_trigger_hitchance" + legit_weapon_preset], 0, 100, "%d%%"); dmt("legit_trigger_hitchance");
		ui::SliderInt("Reaction time", &cfg->i["legit_trigger_delay" + legit_weapon_preset], 0, 200, "%dms"); dmt("legit_trigger_delay");
		auto ltbf = "legit_trigger_burst_fire" + legit_weapon_preset;
		bool burst_fire_enabled = ui::Checkbox("Burst fire", &cfg->b[ltbf]); dmt("legit_trigger_burst_fire");
		if (cfg->b[ltbf])
		{
			ui::SliderInt("##legit_trigger_burst_fire_time", &cfg->i["legit_trigger_burst_fire_time" + legit_weapon_preset], 1, 500, "%dms"); dmt("legit_trigger_burst_fire_time");
		}
		ui::SliderInt("Minimum damage", &cfg->i["legit_trigger_mindamage" + legit_weapon_preset], 0, 100, "%d"); dmt("legit_trigger_mindamage");
		if (!cfg->b[ltbf])
		{
			ui::Checkbox("Automatic penetration", &cfg->b["legit_trigger_autowall" + legit_weapon_preset]); dmt("legit_trigger_autowall");
		}
		ui::Checkbox("Shoot through smoke", &cfg->b["legit_trigger_smoke" + legit_weapon_preset]); dmt("legit_trigger_smoke");
		ui::Checkbox("Shoot while blind", &cfg->b["legit_trigger_blind" + legit_weapon_preset]); dmt("legit_trigger_blind");
		ui::Checkbox("Head", &cfg->b["legit_trigger_head" + legit_weapon_preset]); dmt("legit_trigger_head");
		ui::Checkbox("Chest", &cfg->b["legit_trigger_chest" + legit_weapon_preset]); dmt("legit_trigger_chest");
		ui::Checkbox("Stomach", &cfg->b["legit_trigger_stomach" + legit_weapon_preset]); dmt("legit_trigger_stomach");
		draw_lua_items("legit", "triggerbot");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y + child_size.y * 0.7 + 30));
		ui::BeginChild("Other", ImVec2(child_size.x, child_size.y * 0.3 - 30));
		ui::SingleSelect("Accuracy boost", &cfg->i["legit_backtrack" + legit_weapon_preset], { "Off","Low", "High" }); dmt("legit_backtrack");
		ui::SliderFloat("Accuracy boost range", &cfg->f["legit_backtrack_range" + legit_weapon_preset], 0.f, 200.f, "%.1f"); dmt("legit_backtrack_range");

		ui::Checkbox("Standalone recoil compensation", &cfg->b["legit_other_standalone_rcs" + legit_weapon_preset]); dmt("legit_other_standalone_rcs");
		draw_lua_items("legit", "other");
		ui::EndChild();
	}
	// визуалы *-*
	if (this->m_nCurrentTab == 3) {
		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(0).x, calculateChildWindowPositionDouble(0).y));
		ui::BeginChild("Player ESP", ImVec2(child_size.x, child_size.y * 0.75 - 15));
		ui::Text("Activation type");
		ui::Keybind("Activationtype", &cfg->i["vis_active_key"], &cfg->i["vis_active_keystyle"]); dmt("vis_active_key | vis_active_keystyle");
		ui::Checkbox("Teammates", &cfg->b["vis_teammates"]); dmt("vis_teammates");
		//ui::Checkbox("Self", &cfg->b["vis_self"]); dmt("vis_self");
		ui::Checkbox("Dormant", &cfg->b["vis_dormant"]); dmt("vis_dormant");
		//ui::Checkbox("Hide ESP on distance", &cfg->b["vis_hideespdistance"]); dmt("vis_hideespdistance");
		ui::Checkbox("Bounding box", &cfg->b["vis_boundingbox"]); dmt("vis_boundingbox");
		ui::ColorEdit4("##bboxcolor", cfg->c["vis_boundingbox_color"]); dmt("vis_boundingbox_color");
		//ui::SliderInt("ESP draw distance", &cfg->i["vis_maximalespdistance"], 10, 500, cfg->i["vis_maximalespdistance"] == 500 ? "Map" : "%dft"); dmt("vis_maximalespdistance");
		ui::Checkbox("Health bar", &cfg->b["vis_healthbar"]); dmt("vis_healthbar");
		//ui::Checkbox("Armor", &cfg->b["vis_armor"]); dmt("vis_armor");
		//ui::Checkbox("HitBox", &cfg->b["vis_drawhitbox"]); dmt("vis_drawhitbox");
		//ui::Checkbox("Latency", &cfg->b["vis_latency"]); dmt("vis_latency");
		ui::Checkbox("Name", &cfg->b["vis_name"]); dmt("vis_name");
		ui::ColorEdit4("##namecol", cfg->c["vis_name_color"]); dmt("vis_name_color");
		ui::Checkbox("Flags", &cfg->b["vis_flags"]); dmt("vis_flags");
		ui::Checkbox("Weapon text", &cfg->b["vis_weapons"]); dmt("vis_weapons");
		ui::Checkbox("Weapon icon", &cfg->b["vis_weaponsicons"]); dmt("vis_weaponsicons");
		ui::ColorEdit4("##iconcol", cfg->c["vis_weaponicon_color"]); dmt("vis_weaponicon_color");
		ui::Checkbox("Ammo", &cfg->b["vis_ammo"]); dmt("vis_ammo");
		ui::ColorEdit4("##ammocol", cfg->c["vis_ammo_color"]); dmt("vis_ammo_color");
		ui::Checkbox("Distance", &cfg->b["vis_distance"]); dmt("vis_distance");
		ui::Checkbox("Glow", &cfg->b["vis_glow"]); dmt("vis_glow");
		ui::ColorEdit4("##glowcol", cfg->c["vis_glow_color"]); dmt("vis_glow_color");
		ui::Checkbox("Hit marker", &cfg->b["vis_hitmarker"]); dmt("vis_hitmarker");
		ui::Checkbox("Hit marker sound", &cfg->b["vis_hitsound"]); dmt("vis_hitsound");
		ui::Checkbox("Visualize aimbot", &cfg->b["vis_aim_esp"]); dmt("vis_aim_esp");
		ui::ColorEdit4("##ammocol", cfg->c["vis_ammo_color"]); dmt("vis_ammo_color");
		ui::Checkbox("Visualize aimbot (safe point)", &cfg->b["vis_goodesp"]); dmt("vis_goodesp");
		ui::ColorEdit4("##soundespcol", cfg->c["vis_soundesp_color"]); dmt("vis_lineesp_color");
		ui::Checkbox("Visualize sounds", &cfg->b["vis_soundesp"]); dmt("vis_soundesp");
		ui::ColorEdit4("##soundespcol", cfg->c["vis_soundesp_color"]); dmt("vis_soundesp_color");
		ui::Checkbox("Line sight", &cfg->b["vis_lineesp"]); dmt("vis_lineesp");
		ui::ColorEdit4("##soundespcol", cfg->c["vis_soundesp_color"]); dmt("vis_lineesp_color");
		//ui::Checkbox("Foot Trails", &cfg->b["vis_foot_trails"]); dmt("vis_foot_trails");
		ui::Checkbox("Money", &cfg->b["vis_money"]); dmt("vis_money");
		ui::Checkbox("Skeleton", &cfg->b["vis_skeleton"]); dmt("vis_skeleton");
		ui::ColorEdit4("##skelcolor", cfg->c["vis_skeleton_color"]); dmt("vis_skeleton_color");
		ui::Checkbox("Out of FOV arrow", &cfg->b["vis_outoffovarrows"]); dmt("vis_outoffovarrows");
		ui::ColorEdit4("##offscreencol", cfg->c["vis_outoffovarrows_color"]); dmt("vis_outoffovarrows_color");
		if (cfg->b["vis_outoffovarrows"]) {
			ui::SliderInt("##Size", &cfg->i["vis_outoffovarrows_size"], 2, 32, "%dpx"); dmt("vis_outoffovarrows_size");
			ui::SliderInt("##Offset", &cfg->i["vis_outoffovarrows_dist"], 0, 100, "%d%%"); dmt("vis_outoffovarrows_dist");
		}
		draw_lua_items("visuals", "player esp");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(0).x, calculateChildWindowPositionDouble(0).y + child_size.y * 0.75));

		ui::BeginChild("Colored models", ImVec2(child_size.x, child_size.y * 0.25));

		ui::Checkbox("Player", &cfg->b["vis_chamenemy"]); dmt("vis_chamenemy");
		ui::ColorEdit4("##enemiescol", cfg->c["vis_chamenemy_color"]); dmt("vis_chamenemy_color");
		ui::Checkbox("Player behind wall", &cfg->b["vis_chamenemy_xqz"]); dmt("vis_chamenemy_xqz");
		ui::ColorEdit4("##enemies2col", cfg->c["vis_chamenemy_xqz_color"]); dmt("vis_chamenemy_xqz_color");

		if (cfg->b["vis_chamenemy"])
			ui::SingleSelect("##Colored mod123els type", &cfg->i["vis_chams"], { "Default", "Solid", "Shaded", "Metallic", "Glow" }); dmt("vis_chams");

		if (cfg->i["vis_chams"] == 2 || cfg->i["vis_chams"] == 3 || cfg->i["vis_chams"] == 4)
			ui::ColorEdit4("##reflec123tivty", cfg->c["vis_pearlescent_color"]); dmt("vis_pearlescent_color");

		if (cfg->i["vis_chams"] == 2) {
			ui::SliderFloat("Reflectivity", &cfg->f["vis_reflectivity"], 0, 15, ""); dmt("vis_reflectivity");
			ui::SliderFloat("Shine", &cfg->f["vis_shine"], 0, 15, ""); dmt("vis_shine");
			ui::SliderFloat("Rim", &cfg->f["vis_rim"], 0, 15, ""); dmt("vis_rim");
		}

		/*ui::SliderFloat("Reflectivity", &cfg->f["vis_reflectivity"], 0, 1, ""); dmt("vis_reflectivity");
		ui::ColorEdit4("##reflectivty", cfg->c["vis_pearlescent_color"]); dmt("vis_pearlescent_color");
		ui::SliderFloat("Shine", &cfg->f["vis_shine"], 0, 5, ""); dmt("vis_shine");
		ui::SliderFloat("Rim", &cfg->f["vis_rim"], 0, 5, ""); dmt("vis_rim");*/
			ui::Checkbox("Local player", &cfg->b["vis_chamslocal"]); dmt("vis_chamslocal");
			ui::ColorEdit4("##4232col", cfg->c["vis_chamslocal_color"]); dmt("vis_chamslocal_color");

		if (cfg->b["vis_chamslocal"]) {
			ui::SingleSelect("##123123stype", &cfg->i["vis_chamslocaltype"], { "Default", "Solid", "Shaded", "Metallic", "Glow" }); dmt("vis_chamslocaltype");
		}
		
		if (cfg->i["vis_chamslocaltype"] == 2 || cfg->i["vis_chamslocaltype"] == 3 || cfg->i["vis_chamslocaltype"] == 4)
			ui::ColorEdit4("##51239col", cfg->c["vis_pearlescentlocal_color"]); dmt("vis_pearlescentlocal_color");

		/*ui::Checkbox("Local player fake", &cfg->b["vis_fakeangles"]); dmt("vis_fakeangles");
		ui::ColorEdit4("##6123col", cfg->c["vis_fakeangles_color"]); dmt("vis_fakeangles_color");
		if (cfg->b["vis_fakeangles"]) {
			ui::SingleSelect("##213type", &cfg->i["vis_chamsfake"], { "Default", "Solid", "Shaded", "Metallic", "Glow" }); dmt("vis_chamsfake");
		}
		if (cfg->i["vis_chamsfake"] == 2 || cfg->i["vis_chamsfake"] == 3 || cfg->i["vis_chamsfake"] == 4)
			ui::ColorEdit4("##32lor", cfg->c["vis_pearlescentfake_color"]); dmt("vis_pearlescentfake_color");*/

			ui::Checkbox("Teammates", &cfg->b["vis_chamally"]); dmt("vis_chamally");
			ui::ColorEdit4("##123124col", cfg->c["vis_chamally_color"]); dmt("vis_chamally_color");

		ui::Checkbox("Teammates behind wall", &cfg->b["vis_chamally_xqz"]); dmt("vis_chamally_xqz");
		ui::ColorEdit4("##63321col", cfg->c["vis_chamallyxqz_color"]); dmt("vis_chamallyxqz_color");

		if (cfg->b["vis_chamally"]) {
			ui::SingleSelect("##teammatestype", &cfg->i["vis_chamsally"], { "Default", "Solid", "Shaded", "Metallic", "Glow" }); dmt("vis_chamsally");
		}

		if (cfg->i["vis_chamsally"] == 2 || cfg->i["vis_chamsally"] == 3 || cfg->i["vis_chamsally"] == 4)
			ui::ColorEdit4("##322239col", cfg->c["vis_pearlescentally_color"]); dmt("vis_pearlescentally_color");

		ui::Checkbox("Hands", &cfg->b["vis_chamshands"]); dmt("vis_chamshands");
		ui::ColorEdit4("##12312317col", cfg->c["vis_hands_color"]); dmt("vis_hands_color");

		if (cfg->b["vis_chamshands"]) {
			ui::SingleSelect("##333211type", &cfg->i["vis_chamshandstype"], { "Default", "Solid", "Shaded", "Metallic", "Glow" }); dmt("vis_chamsally");
		}

		if (cfg->i["vis_chamshandstype"] == 2 || cfg->i["vis_chamshandstype"] == 3 || cfg->i["vis_chamshandstype"] == 4)
			ui::ColorEdit4("##handscolor", cfg->c["vis_pearlescenthands_color"]); dmt("vis_pearlescenthands_color");

		ui::Checkbox("Ragdolls", &cfg->b["vis_ragdolls"]); dmt("vis_ragdolls");

		ui::Checkbox("Disable model occlusion", &cfg->b["vis_chamsocclusion"]); dmt("vis_chamsocclusion");
		ui::Checkbox("Shadow", &cfg->b["vis_chamshadow"]); dmt("vis_chamshadow");
		ui::ColorEdit4("##enemies3col", cfg->c["vis_chamshadow_color"]); dmt("vis_chamshadow_color");

		draw_lua_items("visuals", "colored models");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y));

		ui::BeginChild("Other ESP", ImVec2(child_size.x, child_size.y * 0.5 - 15));

		ui::Checkbox("Radar", &cfg->b["vis_radar"]); dmt("vis_radar");
		ui::SingleSelect("Dropped weapons", &cfg->i["vis_droppedwpns"], { "Off", "Text", "Glow", "Icon" }); dmt("vis_droppedwpns");
		ui::Checkbox("Dropped weapon ammo", &cfg->b["vis_droppedwpnsammo"]); dmt("vis_droppedwpnsammo");
		ui::ColorEdit4("##droppedcol", cfg->c["vis_droppedwpns_color"]); dmt("vis_droppedwpns_color");
		ui::Checkbox("Grenades", &cfg->b["vis_grenades"]); dmt("vis_grenades");
		ui::ColorEdit4("##nadescol", cfg->c["vis_grenades_color"]); dmt("vis_grenades_color");
		ui::SingleSelect("Inaccuracy overlay", &cfg->i["vis_inaccuracyoverlay"], { "Disabled", "Gradient", "Rainbow" }); dmt("vis_inaccuracyoverlay");
		ui::ColorEdit4("##spreadcol", cfg->c["vis_inaccuracyoverlay_color"]); dmt("vis_inaccuracyoverlay_color");
		ui::Checkbox("Recoil overlay", &cfg->b["vis_recoiloverlay"]); dmt("vis_recoiloverlay");
		ui::Checkbox("Crosshair", &cfg->b["vis_crosshair"]); dmt("vis_crosshair");
		ui::Checkbox("Bomb", &cfg->b["vis_bomb"]); dmt("vis_bomb");
		ui::ColorEdit4("##bombovercol", cfg->c["vis_bomb_color"]); dmt("vis_bomb_color");
		//ui::Checkbox("Zeus Range", &cfg->b["vis_zeus_range"]); dmt("vis_zeus_range");
		ui::Checkbox("Grenade trajectory", &cfg->b["vis_grenadetrajectory"]); dmt("vis_grenadetrajectory");
		ui::ColorEdit4("##grenadecol", cfg->c["vis_grenadetrajectory_color"]); dmt("vis_grenadetrajectory_color");
		ui::Checkbox("Spectators", &cfg->b["vis_spectators"]); dmt("vis_spectators");
		ui::Checkbox("Penetration reticle", &cfg->b["vis_penetrationreticle"]); dmt("vis_penetrationreticle");
		ui::Checkbox("Hostages", &cfg->b["vis_hostage"]); dmt("vis_hostage");
		ui::ColorEdit4("##ammocol", cfg->c["vis_ammo_color"]); dmt("vis_ammo_color");
		ui::Checkbox("Shared ESP", &cfg->b["vis_share"]); dmt("vis_share");
		ui::Keybind("shared", &cfg->i["vis_shared_key"], &cfg->i["vis_shared_keystyle"]); dmt("vis_shared_key | vis_shared_keystyle");
		ui::Checkbox("Upgrade tablet", &cfg->b["vis_upgrade"]); dmt("vis_upgrade");
		ui::Checkbox("Danger Zone items", &cfg->b["vis_zone"]); dmt("vis_zone");
		draw_lua_items("visuals", "other esp");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y + child_size.y * 0.5));

		ui::BeginChild("Effects", ImVec2(child_size.x, child_size.y * 0.5));

		//ui::SliderInt("Alpha in scope", &cfg->i["vis_alphainscope"], 0, 255, "%d"); dmt("vis_alphainscope");
		//ui::Checkbox("draw hit player", &cfg->b["vis_drawhit"]); dmt("vis_drawhit");
		//if (cfg->b["vis_drawhit"]) {
		//	ui::ColorEdit4("##enemies45col", cfg->c["vis_drawhit_color"]); dmt("vis_drawhit_color");
		//	ui::SliderInt("Lagcompensation Duration", &cfg->i["vis_lagcompensation_duration"], 1, 10, "sec"); dmt("vis_lagcompensation_duratione");
		//}
		ui::Checkbox("Remove flashbang effect", &cfg->b["vis_noflash"]); dmt("vis_noflash");
		ui::Checkbox("Remove smoke grenades", &cfg->b["vis_nosmoke"]); dmt("vis_nosmoke");
		//ui::Checkbox("Remove zoom", &cfg->b["vis_nozoom"]); dmt("vis_nozoom");
		ui::Checkbox("Remove fog", &cfg->b["vis_nofog"]); dmt("vis_nofog");
		ui::Checkbox("Remove skybox", &cfg->b["vis_nosky"]); dmt("vis_nosky");
		ui::SingleSelect("Visual recoil adjustment", &cfg->i["vis_novisrecoil"], { "Off", "Remove shake", "Remove all" }); dmt("vis_novisrecoil");
		ui::SliderInt("Transparent walls", &cfg->i["vis_wallstransp"], 0, 100, "%d%%"); dmt("vis_wallstransp");
		ui::SliderInt("Transparent props", &cfg->i["vis_propstransp"], 0, 100, "%d%%"); dmt("vis_propstransp");
		ui::SingleSelect("Brightness adjustment", &cfg->i["vis_nightmode_color"], { "Off", "Fullbright", "Night mode" }); dmt("vis_nightmode_color");
		/*if (cfg->b["vis_nightmode"]) {
			ui::SliderInt("##brightness", &cfg->i["vis_brightness"], 0, 100, "%d%%"); dmt("vis_brightness");
		}*/
		ui::Checkbox("Remove scope overlay", &cfg->b["vis_noscope"]); dmt("vis_noscope");
		ui::Checkbox("Insta scope", &cfg->b["vis_scope"]); dmt("vis_scope");
		ui::Checkbox("Disable post processing", &cfg->b["vis_nopostprocess"]); dmt("vis_nopostprocess");
		ui::Checkbox("Force third person (alive)", &cfg->b["vis_thirdperson"]); dmt("vis_thirdperson");
		ui::Keybind("tpkey", &cfg->i["vis_thirdperson_key"], &cfg->i["vis_thirdperson_keystyle"]); dmt("vis_thirdperson_key | vis_thirdperson_keystyle");
		ui::Checkbox("Force third person (dead)", &cfg->b["vis_thirdperson_dead"]); dmt("vis_thirdperson_dead");
		ui::Checkbox("Disable rendering of teammates", &cfg->b["vis_scope"]); dmt("vis_scope");
		ui::Checkbox("Bullet tracers", &cfg->b["vis_bullettracer"]); dmt("vis_bullettracer");
		ui::ColorEdit4("##buletcol", cfg->c["vis_bullettracer_color"]); dmt("vis_bullettracer_color");
		ui::Checkbox("Bullet impacts", &cfg->b["vis_bulletimpacts"]); dmt("vis_bulletimpacts");
		ui::Checkbox("Override skybox", &cfg->b["vis_nightmode"]); dmt("vis_nightmode");
		ui::ColorEdit4("##droppedcol", cfg->c["vis_droppedwpns_color"]); dmt("vis_droppedwpns_color");
		/*ui::Checkbox("Custom Viewmodel", &cfg->b["vis_viewmodel"]); dmt("vis_viewmodel");
		ui::SliderFloat("> X", &cfg->f["vis_veiwx"], -10, 10, ""); dmt("vis_veiwx");
		ui::SliderFloat("> Y", &cfg->f["vis_veiwy"], -10, 10, ""); dmt("vis_veiwy");
		ui::SliderFloat("> Z", &cfg->f["vis_veiwz"], -10, 10, ""); dmt("vis_veiwz");*/
		draw_lua_items("visuals", "effects");
		ui::EndChild();

	}

	if (this->m_nCurrentTab == 4) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));

		ui::BeginChild("Miscellaneous", child_size);

		ui::SliderInt("Override FOV", &cfg->i["misc_overridefov"], 60, 135); dmt("misc_overridefov");
		ui::SliderInt("Override zoom FOV", &cfg->i["misc_override"], 60, 135); dmt("misc_overridefov");
		//ui::Checkbox("Slide-walk", &cfg->b["misc_slidewalk"]); dmt("misc_slidewalk");
		ui::Checkbox("Knifebot", &cfg->b["misc_knifebot"]); dmt("misc_knifebot");
		ui::Checkbox("Zeusbot", &cfg->b["misc_zeusbot"]); dmt("misc_zeusbot");
		ui::Checkbox("Automatic weapons", &cfg->b["misc_automaticwpns"]); dmt("misc_automaticwpns");
		if (cfg->b["misc_automaticwpns"])
			ui::SliderInt("##autowpnstime", &cfg->i["misc_automaticwpns_time"], 0, 250, "%dms"); dmt("misc_automaticwpns_time");
		ui::Checkbox("Reveal competitive ranks", &cfg->b["misc_revealranks"]); dmt("misc_revealranks");
		ui::Checkbox("Reveal Overwatch players", &cfg->b["misc_revealplayers"]); dmt("misc_revealplayers");
		ui::Checkbox("Auto-accept matchmaking", &cfg->b["misc_autoacceptmm"]); dmt("misc_autoacceptmm");
		ui::Checkbox("Clan tag spammer", &cfg->b["misc_clantagspammer"]); dmt("misc_clantagspammer");
		//ui::Checkbox("Log events", &cfg->b["misc_logevents"]); dmt("misc_logevents");
		ui::Checkbox("Log weapon purchases", &cfg->b["misc_showbuylog"]); ("misc_showbuylog");
		ui::Checkbox("Log damage dealt", &cfg->b["misc_showdamage"]); dmt("misc_showdamage");
		ui::Checkbox("Automatic grenade release", &cfg->b["misc_autogrenade"]); dmt("misc_autogrenade");
		ui::Keybind("grenade", &cfg->i["misc_grenade_key"], &cfg->i["misc_grenade_keystyle"]); dmt("misc_grenade_key | misc_grenade_keystyle");
		ui::Checkbox("Automatic buy", &cfg->b["misc_autobuy"]); dmt("misc_autobuy");
		if (cfg->b["misc_autobuy"]) {
			ui::SingleSelect("Snipers", &cfg->i["misc_sniper_buy"], { "none","G3SG1 / SCAR-20", "AWP", "SSG 08" }); dmt("misc_sniper_buy");
			ui::SingleSelect("Pistols", &cfg->i["misc_pistol_buy"], { "none","Deagle", "Dual Berettas","Revolver" }); dmt("misc_pistol_buy");
			ui::MultiSelect("Others", &cfg->m["misc_other_buy"], { "HE Grenade", "Molotov", "Flashbang", "Smoke", "Taser", "Armor" }); dmt("misc_other_buy");
		}

		ui::Checkbox("Ping spike", &cfg->b["misc_pingspike"]); dmt("misc_pingspike");
		ui::Keybind("pingspike", &cfg->i["misc_pingspike_key"], &cfg->i["misc_pingspike_keystyle"]); dmt("misc_pingspike_key | misc_pingspike_keystyle");

		if (cfg->b["misc_pingspike"])
			ui::SliderInt("##Latency", &cfg->i["misc_pingspike_val"], 0, 200);
		ui::Text("Free look");
		ui::Keybind("free", &cfg->i["misc_free"]); dmt("misc_free");
		ui::Checkbox("Persiant kill feed", &cfg->b["vis_preservekillfeed"]); dmt("vis_preservekillfeed");
		ui::Text("Last second defuse");
		ui::Keybind("defuse", &cfg->i["misc_defuse"]); dmt("misc_defuse");
		ui::Checkbox("Disable sv_pure", &cfg->b["misc_pure"]); dmt("misc_pure");
		if (ui::Button("Steal player name"))
			c_config::get()->save();
		if (ui::Button("Dump MM wins"))
			c_config::get()->save();
		draw_lua_items("misc", "miscellaneous");
		ui::EndChild();
		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y));

		ui::BeginChild("Movement", ImVec2(child_size.x, child_size.y * 0.5 - 15));
		ui::Checkbox("Standalone quick stop", &cfg->b["misc_faststop"]); dmt("misc_faststop");
		ui::CheckboxYellow("Infinite duck", &cfg->b["misc_infiniteduck"]); dmt("misc_infiniteduck");
		ui::Checkbox("Easy strafe", &cfg->b["misc_strafe"]); dmt("misc_strafe");
		ui::Checkbox("Bunny hop", &cfg->b["misc_autojump"]); dmt("misc_autojump");

		if (cfg->b["misc_autojump"]) {
			ui::SingleSelect("Air strafe", &cfg->i["misc_airstrafe"], { "Off", "Movement direction", "View angles" }); dmt("misc_airstrafe");
		}

		////ui::Checkbox("Circle-strafe", &cfg->b["misc_circlestrafe"]); dmt("misc_circlestrafe");
		//ui::Keybind("circlestrafe", &cfg->i["misc_circlestrafe_key"], &cfg->i["misc_circlestrafe_keystyle"]); dmt("misc_circlestrafe_key | misc_circlestrafe_keystyle");
		ui::Checkbox("No fall damage", &cfg->b["misc_damage"]); dmt("misc_damage");
		ui::ColorEdit4("##nofallcolor", cfg->c["fall_color"]); dmt("fall_color");
		ui::Checkbox("Air duck", &cfg->b["misc_airduck"]); dmt("misc_airduck");
		ui::Checkbox("Block bot", &cfg->b["misc_block"]); dmt("misc_block");
		ui::Keybind("block", &cfg->i["misc_block_key"], &cfg->i["misc_block_keystyle"]); dmt("misc_block_key | misc_block_keystyle");
		ui::Checkbox("Jump at edge", &cfg->b["misc_edge"]); dmt("misc_edge");
		ui::Keybind("egde", &cfg->i["misc_edge_key"], &cfg->i["misc_edge_keystyle"]); dmt("misc_edge_key | misc_edge_keystyle");
		ui::Checkbox("Fast walk", &cfg->b["misc_fast"]); dmt("misc_fast");
		draw_lua_items("misc", "movement");
		ui::EndChild();
		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(1).x, calculateChildWindowPositionDouble(1).y + child_size.y * 0.5));
		ui::BeginChild("Settings", ImVec2(child_size.x, child_size.y * 0.5));
		ui::Text("Menu key");
		ui::Keybind("menukey", &cfg->i["misc_menukey"]); dmt("misc_menukey");
		ui::Text("Menu color");
		ui::ColorEdit4("##menucolor", cfg->c["menu_color"]); dmt("menu_color");
		ui::SingleSelect("Dpi scale", &cfg->i["vis_lolol"], { "100%", "125%", "150%", "175%", "200%" }); dmt("vis_lolol");
		ui::Checkbox("Anti-untrusted", &cfg->b["misc_antiuntrusted"]); dmt("misc_antiuntrusted");
		ui::Checkbox("Hide from obs", &cfg->b["misc_hide"]); dmt("misc_hide");
		ui::Checkbox("Low FPS warning", &cfg->b["misc_lowfpswarning"]); dmt("misc_lowfpswarning");
		ui::Checkbox("Lock menu layout", &cfg->b["misc_lock"]); dmt("misc_lock");
		if (ui::Button("Reset menu layout"))
			c_config::get()->load();
		if (ui::Button("Unload") && unload_start_time == INT_MAX)
		{
			this->set_menu_opened(false);
			ui::GetStyle().Alpha = 0.f;
			cfg->load_defaults();
			Interfaces::ClientState->ForceFullUpdate();
			unload_start_time = (int)GetTickCount();
		}
		draw_lua_items("misc", "settings");
		ui::EndChild();

		
	}
	if (this->m_nCurrentTab == 5) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Knife settings", ImVec2(child_size.x, child_size.y * 0.3 - 15));
		if(ui::Checkbox("Override knife", &cfg->b["skin_changer_override_knife"]))
			Interfaces::ClientState->ForceFullUpdate();
		dmt("skin_changer_override_knife");
		if(ui::SingleSelect("Knife", &cfg->i["skin_changer_model_knife"],
		{
			"Default",
			"M9 Bayonet",
			"Bayonet",
			"Flip",
			"Gut",
			"Karambit",
			"Huntsman",
			"Falchion",
			"Bowie",
			"Butterfly",
			"Shadow Daggers",
			"Navaja",
			"Stiletto",
			"Ursus",
			"Talon"
		}))
			Interfaces::ClientState->ForceFullUpdate();
		dmt("skin_changer_model_knife");
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(calculateChildWindowPositionDouble(0).x, calculateChildWindowPositionDouble(0).y + child_size.y * 0.3));
		ui::BeginChild("Glove options", ImVec2(child_size.x, child_size.y * 0.7));
		if(ui::Checkbox("Override gloves", &cfg->b["skin_changer_override_gloves"]))
			Interfaces::ClientState->ForceFullUpdate();
		dmt("skin_changer_override_gloves");
		if(ui::SingleSelect("##skin_changer_model_gloves", &cfg->i["skin_changer_model_gloves"],
		{
			"Bloodhound",
			"Wraps",
			"Driver",
			"Sport",
			"Specialist",
			"Motorcycle",
			"Hydra"
		}))
			Interfaces::ClientState->ForceFullUpdate();
		dmt("skin_changer_model_gloves");
		{
			static std::vector<std::vector<const char*>> skin_changer_list_paintkit_gloves
			{
				{ "Bronzed", "Charred", "Guerrilla", "Snakebite" },
				{ "Arboreal", "Badlands", "Cobalt Skulls", "Duct Tape", "Leather", "Overprint", "Slaughter", "Spruce DDPAT" },
				{ "Convoy", "Crimson Weave", "Diamondback", "Imperial Plaid", "King Snake", "Lunar Weave", "Overtake", "Racing Green" },
				{ "Amphibious", "Arid", "Bonze Morph", "Hedge Maze", "Omega", "Pandora's Box", "Superconductor", "Vice" },
				{ "Buckshot", "Crimson Kimono", "Crimson Web", "Emerald Web", "Fade", "Forest DDPAT", "Foundation", "Mogul" },
				{ "Boom!", "Cool Mint", "Eclipse", "POW!", "Polygon", "Spearmint", "Transport", "Turtle" },
				{ "Case Hardened", "Emerald", "Mangrove", "Rattler" }
			};

			static char search[128];
			ui::InputText("_", search, 128);

			if (ui::ListBoxHeader("##skin_changer_list_paintkit_gloves", ImVec2(child_size.x, child_size.y * 0.7 - 80)))
			{
				auto& skin_changer_sublist_paintkit_gloves = skin_changer_list_paintkit_gloves[cfg->i["skin_changer_model_gloves"]];
				int old_skin_gloves = cfg->i["skin_changer_paintkit_gloves"];
				auto skin_changer_paintkit_gloves = &cfg->i["skin_changer_paintkit_gloves"];
				for (size_t i = 0, count = skin_changer_sublist_paintkit_gloves.size(); i < count; ++i)
				{
					const bool item_selected = (i == *skin_changer_paintkit_gloves);
					auto selectable_label = skin_changer_sublist_paintkit_gloves[i];

					if (!str_search_eq(selectable_label, search))
						continue;

					if (ui::Selectable(selectable_label, item_selected))
						*skin_changer_paintkit_gloves = i;
				}
				if(old_skin_gloves != *skin_changer_paintkit_gloves)
					Interfaces::ClientState->ForceFullUpdate();
				ui::ListBoxFooter();
				dmt("skin_changer_list_paintkit_gloves");
			}
		}
		dmt("ListBoxHeaderX");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Weapon skin", child_size);
		ui::Checkbox("Enable", &cfg->b["skin_changer_override_weapon"]); dmt("skin_changer_override_weapon");
		ui::Checkbox("StatTrak", &cfg->b["skin_changer_stattrak"]); dmt("skin_changer_stattrak");
		ui::SliderInt("Quality", &cfg->i["skin_changer_quality"], 1, 100, "%d%%"); dmt("skin_changer_quality");
		ui::SliderInt("Seed", &cfg->i["skin_changer_seed"], 0, 1000, "%d"); dmt("skin_changer_seed");
		static char search[128];
		ui::InputText("_", search, 128);
		if (ui::ListBoxHeader("##ListBoxHeaderX2", ImVec2(child_size.x, child_size.y * 0.7 - 80)))
		{
			static bool e;
			ui::Selectable("Abyss", &e);
			static bool f;
			ui::Selectable("Acheron", &f);
			static bool g;
			ui::Selectable("Acid Fade", &g);
			static bool h;
			ui::Selectable("Aerial", &h);
			ui::ListBoxFooter();
		}
		{
			//static int i = 0;
			//const char* items[] = { "Abyss", "Acheron", "Acid Fade", "Aerial" };
		   // ui::ListBox("##ListBoxHeaderX2", &i, items, IM_ARRAYSIZE(items), 20);
		}
		dmt("ListBoxHeaderX2");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 6) {
		auto players = c_playerlist::get()->get_players();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Player list", child_size);

		ui::Checkbox("Everyone", &cfg->b["plist_everyone"]); dmt("plist_everyone");

		static char uname[128];
		ui::InputText("Search", uname, 128);

		ui::ListBoxHeader("##players", ImVec2(0, ui::GetWindowSize().y - 80));

		for (auto p : players) {
			if (!str_search_eq(p.szName, uname))
				continue;

			if (ui::Selectable(p.szName, cfg->i["plist_selected"] == p.userId))
				cfg->i["plist_selected"] = p.userId;
		}

		ui::ListBoxFooter();

		draw_lua_items("players", "player list");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Override settings", child_size);

		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
			auto key = "plist_" + c_playerlist::get()->get_steam_id(players, cfg->i["plist_selected"]) + "_";

			ui::Checkbox("Friend", &cfg->b[key + "friend"]); dmt(key + "friend");
			ui::Checkbox("Disable resolver", &cfg->b[key + "noresolver"]); dmt(key + "noresolver");
			ui::Checkbox("Pitch", &cfg->b[key + "pitch"]); dmt(key + "pitch");

			if (cfg->b[key + "pitch"]) {
				ui::SliderInt("##custompitch", &cfg->i[key + "pitchval"], -90, 90, u8"%d°"); dmt(key + "pitchval");
			}

			ui::Checkbox("Yaw", &cfg->b[key + "yaw"]); dmt(key + "yaw");

			if (cfg->b[key + "yaw"]) {
				ui::SliderInt("##customyaw", &cfg->i[key + "yawval"], -180, 180, u8"%d°"); dmt(key + "yawval");
				ui::SingleSelect("Type", &cfg->i[key + "yawtype"], { "Add", "Set", "Subtract" }); dmt(key + "yawtype");
			}

			ui::Checkbox("Prefer body-aim", &cfg->b[key + "baim"]); dmt(key + "baim");
			ui::Checkbox("Disable visuals", &cfg->b[key + "novis"]); dmt(key + "novis");

			if (ui::Button("Reset all")) {
				for (auto p : players) {
					key = "plist_" + c_playerlist::get()->get_steam_id(players, p.userId) + "_";

					cfg->b[key + "friend"] = false;
					cfg->b[key + "noresolver"] = false;
					cfg->b[key + "pitch"] = false;
					cfg->i[key + "pitchval"] = 0;
					cfg->b[key + "yaw"] = false;
					cfg->i[key + "yawval"] = 0;
					cfg->i[key + "yawtype"] = 0;
					cfg->b[key + "baim"] = false;
					cfg->b[key + "novis"] = false;
				}
			}

			if (ui::Button("Apply to all")) {
				for (auto p : players) {
					auto key_temp = "plist_" + c_playerlist::get()->get_steam_id(players, p.userId) + "_";

					cfg->b[key_temp + "friend"] = cfg->b[key + "friend"];
					cfg->b[key_temp + "noresolver"] = cfg->b[key + "noresolver"];
					cfg->b[key_temp + "pitch"] = cfg->b[key + "pitch"];
					cfg->i[key_temp + "pitchval"] = cfg->i[key + "pitchval"];
					cfg->b[key_temp + "yaw"] = cfg->b[key + "yaw"];
					cfg->i[key_temp + "yawval"] = cfg->i[key + "yawval"];
					cfg->i[key_temp + "yawtype"] = cfg->i[key + "yawtype"];
					cfg->b[key_temp + "baim"] = cfg->b[key + "baim"];
					cfg->b[key_temp + "novis"] = cfg->b[key + "novis"];
				}
			}

			draw_lua_items("players", "override settings");
		}
		else
			ui::Text("Not connected.");

		ui::EndChild();
	}
	if (this->m_nCurrentTab == 7) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("Preset", child_size);
		ui::SingleSelect("Preset", &cfg->i["_preset"], c_config::get()->presets); dmt("_preset");
		ui::Keybind("##presetkey", &cfg->i["_preset_" + std::to_string(cfg->i["_preset"])]); dmt("_preset_" + std::to_string(cfg->i["_preset"]));

		if (ui::Button("Load"))
			c_config::get()->load();
		if (ui::Button("Save"))
			c_config::get()->save();
		if (ui::Button("Delete")) {
			//auto res = LI_FN(MessageBoxA).get()(0, "Are you sure?", "Reset warning", MB_APPLMODAL | MB_ICONWARNING | MB_YESNO);

			//if (res == IDYES)
			c_config::get()->load_defaults();
		}
		if (ui::Button("Reset"))
			c_config::get()->load();

		if (ui::Button("Import from clipboard"))
			c_config::get()->load();

		if (ui::Button("Export to clipboard"))
			c_config::get()->load();

		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("Lua", child_size);
		ui::Checkbox("Enabled", &cfg->b["Active"]); dmt("Active");
		ui::Checkbox("Allow unsafe scripts", &cfg->b["Allow"]); dmt("Allow");
		if (ui::Button("Reload active scripts"))
			c_lua::get()->reload_all_scripts();
		if (ui::Button("Refresh scripts"))
			c_lua::get()->refresh_scripts();
		static char search[128];
		ui::InputText("_", search, 128);
		ui::PushItemWidth(196);
		if (ui::ListBoxHeader("##nn123", ImVec2(196, 300))) {
			for (auto s : c_lua::get()->scripts)
			{
				std::string st = s.substr(0, s.size() - 4);
				if (ui::Selectable(st.c_str(), c_lua::get()->loaded.at(c_lua::get()->get_script_id(s)), NULL, ImVec2(0, 0))) {
					auto scriptId = c_lua::get()->get_script_id(s);

					if (c_lua::get()->loaded.at(scriptId))
						c_lua::get()->unload_script(scriptId);
					else
						c_lua::get()->load_script(scriptId);
				}
			}

			ui::ListBoxFooter();
			if (ui::Button("Unload all"))
				c_lua::get()->unload_all_scripts();

			ui::Checkbox("Load on startup", &cfg->b["load"]); dmt("load");
		}
		ui::PopItemWidth();
		
		

		draw_lua_items("config", "lua");
		ui::EndChild();
	}

	if (this->m_nCurrentTab == 8) {
		ui::SetNextWindowPos(calculateChildWindowPositionDouble(0));
		ui::BeginChild("A", child_size);
		draw_lua_items("lua", "a");
		ui::EndChild();

		ui::SetNextWindowPos(calculateChildWindowPositionDouble(1));
		ui::BeginChild("B", child_size);
		draw_lua_items("lua", "b");
		ui::EndChild();
	 }
   
	  ui::End(); 
}
void c_menu::draw_end() {
	if (!this->m_bInitialized)
		return;

	ui::EndFrame();
	ui::Render();
	ImGui_ImplDX9_RenderDrawData(ui::GetDrawData());
}

bool c_menu::is_menu_initialized() {
	return this->m_bInitialized;
}

bool c_menu::is_menu_opened() {
	return this->m_bIsOpened;
}

void c_menu::set_menu_opened(bool v) {

	this->m_bIsOpened = v;
}

IDirect3DTexture9* c_menu::get_texture_data() {
	return this->m_pTexture;
}

ImColor c_menu::get_accent_color() {
	return ImColor(
		c_config::get()->c["menu_color"][0],
		c_config::get()->c["menu_color"][1],
		c_config::get()->c["menu_color"][2],
		ui::GetStyle().Alpha
	);
}
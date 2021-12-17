#include "NewMenu.h"
#include <fstream>
#include "..\Antario\Features\FakeLag.h"
#include <string> 
#include "EventListener.h"
#define UNLEN 256
IDirect3DStateBlock9* state_block;
bool reverse = false;
int offset = 0;
bool show_popup = false;
static bool save_config = false;
static bool load_config = false;
static char buf1[64] = "";
c_menu* newmenu = new c_menu();
using msg_t = void(__cdecl*)(const char*, ...);
msg_t Msg = reinterpret_cast<msg_t>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

#include <execution>
#include <algorithm>
#include <iostream>
namespace ImGui {
	static auto vector_getter = [](void* vec, int idx, const char** out_text) {
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool combo_array(const char* label, int* current_index, std::vector<std::string>& values) {
		if (values.empty()) { return false; }
		return Combo(label, current_index, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	long get_mils() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	void begin_popup(const char* text, int onScreenMils, bool* done) {
		if (!done)
			show_popup = true;

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		int width = io.DisplaySize.x;
		static long old_time = -1;
		ImGui::SetNextWindowPos(ImVec2(width - offset, 100));
		style.WindowMinSize = ImVec2(100.f, 20.f);
		ImGui::Begin("##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 15, p.y - 13), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		long current_time_ms = get_mils();

		ImVec2 text_size = ImGui::CalcTextSize(text);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - text_size.y / 2);
		ImGui::Text(text);

		if (!reverse) {
			if (offset < ImGui::GetWindowWidth())
				offset += (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);

			if (offset >= ImGui::GetWindowWidth() && old_time == -1) {
				old_time = current_time_ms;
			}
		}

		if (current_time_ms - old_time >= onScreenMils && old_time != -1)
			reverse = true;

		if (reverse) {
			if (offset > 0)
				offset -= (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);
			if (offset <= 0) {
				offset = 0;
				reverse = false;
				*done = true;
				old_time = -1;
				show_popup = false;
			}
		}

		ImGui::End();
	}
}



void Detach()
{
	g_Settings.bCheatActive = false;
}




template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD * *> (Utils::FindSignature("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1); //(Utilities::Memory::FindPatternV2("client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

#include <fstream>
#include <memory>
#include <ShlObj.h>

struct hud_weapons_t
{
	std::int32_t* get_weapon_count()
	{
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

bool Settings::Save(std::string file_name)
{
	CreateDirectoryA("C:\\OneWay.sosu", NULL);

	std::string file_path = "C:\\OneWay.sosu\\" + file_name + ".ini";

	std::fstream file(file_path, std::ios::out | std::ios::in | std::ios::trunc);
	file.close();

	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		return false;
	}

	const size_t settings_size = sizeof(Settings);
	for (int i = 0; i < settings_size; i++)
	{
		byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
		for (int x = 0; x < 8; x++)
		{
			file << (int)((current_byte >> x) & 1);
		}
	}

	file.close();

	return true;
}
bool Settings::Load(std::string file_name)
{


	std::string file_path = "C:\\OneWay.sosu\\" + file_name + ".ini";

	std::fstream file;
	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		return false;
	}

	std::string line;
	while (file)
	{
		std::getline(file, line);

		const size_t settings_size = sizeof(Settings);
		if (line.size() > settings_size * 8)
		{
			file.close();
			return false;
		}
		for (int i = 0; i < settings_size; i++)
		{
			byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
			for (int x = 0; x < 8; x++)
			{
				if (line[(i * 8) + x] == '1')
					current_byte |= 1 << x;
				else
					current_byte &= ~(1 << x);
			}
			*reinterpret_cast<byte*>(uintptr_t(this) + i) = current_byte;
		}
	}

	file.close();

	return true;
}

void save()
{

	g_pConVar->ConsoleColorPrintf(Color2(0, 255, 0, 255), "[OneWay] ");
	Msg("Save %s    \n", buf1);

	g_Settings.Save(buf1);
}
void load()
{
	g_pConVar->ConsoleColorPrintf(Color2(0, 255, 0, 255), "[OneWay] ");
	Msg("load %s    \n", buf1);
	g_Settings.Load(buf1);
}







void KnifeApplyCallbk()
{
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(Utils::FindSignature(("client.dll"), ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

	auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	g_pClientState->ForceFullUpdate();
}







void BtnActiveTab()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(52, 57, 68, 255);
	style.Colors[ImGuiCol_Border] = ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(52, 57, 68, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(52, 57, 68, 255);
	style.Colors[ImGuiCol_Text] = ImColor(219, 222, 229, 255);
}
void BtnNormalTab()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_Border] = ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_Text] = ImColor(219, 222, 229, 255);

}
void c_menu::run() {
	ImGui::GetIO().MouseDrawCursor;
	static int page = 0;
	ImVec2 prevwinpos;
	ImGuiWindowFlags window_flags = 0;

	static bool FLAGS = true;

	if (!g_Settings.fontsize)            window_flags |= ImGuiWindowFlags_NoResize ;


	if (g_Settings.bMenuOpened) {

		ImGui::PushFont(Main);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, -10));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(29 / 255.f, 32 / 255.f, 40 / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(41 / 255.f, 44 / 255.f, 52 / 255.f, 1.0f));
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImColor(219, 222, 229);
		if (ImGui::Begin("main", &g_Settings.bMenuOpened, ImVec2(660, 560), ImGui::GetStyle().Alpha, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ))
		{
			int menur, menug, menub;
			menur = g_Settings.cMenuCol[0] * 255;
			menug = g_Settings.cMenuCol[1] * 255;
			menub = g_Settings.cMenuCol[2] * 255;
			ImVec2 p = ImGui::GetWindowPos();// idk why this is the same but btw 
			prevwinpos = ImGui::GetWindowPos();
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 7), ImColor(menur, menug, menub, 255), ImColor(menur, menug, menub, 255), ImColor(menur, menug, menub, 255), ImColor(menur, menug, menub, 255));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7); //
			ImGui::Text("OnePaste.su");

			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + 160, p.y + 22), ImVec2(p.x + 161, p.y + 48), ImColor(48, 54, 64, 255), ImColor(48, 54, 64, 255), ImColor(48, 54, 64, 255), ImColor(48, 54, 64, 255)); // line

			ImGui::SameLine(188);

			ImGui::PushFont(MainCaps);




			if (page == 0) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("ragebot", ImVec2(0, 25)))page = 0;

			ImGui::SameLine();

			if (page == 1) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("anti-hit", ImVec2(0, 25)))page = 1;

			ImGui::SameLine();

			if (page == 2) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("esp", ImVec2(0, 25)))page = 2;




			ImGui::SameLine();

			if (page == 3) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("skins", ImVec2(0, 25)))page = 3;

			ImGui::SameLine();

			if (page == 4) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("misc", ImVec2(0, 25)))page = 4;

			ImGui::SameLine();

			if (page == 5) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button("config", ImVec2(0, 25)))page = 5;




			ImGui::PopFont();

		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopFont();

		ImGui::PushFont(font_main);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.3f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(29 / 255.f, 32 / 255.f, 40 / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(35 / 255.f, 39 / 255.f, 49 / 255.f, 1.0f));
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImColor(219, 222, 229);

		int menur, menug, menub;
		menur = g_Settings.cMenuCol[0] * 255;
		menug = g_Settings.cMenuCol[1] * 255;
		menub = g_Settings.cMenuCol[2] * 255;


		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImColor(menur, menug, menub);
		ImGui::SetNextWindowPos(ImVec2(prevwinpos.x, prevwinpos.y + 76));
		if (ImGui::Begin("tabs", &g_Settings.bMenuOpened, ImVec2(646, 548), ImGui::GetStyle().Alpha, ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{

			if (page == 0) {
				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("AimBot", ImVec2(0, 455), true);
				{


					ImGui::Checkbox("enabled", &g_Settings.bAimBotEnabled);
					ImGui::Checkbox("automatic fire", &g_Settings.bAutoFire);
					ImGui::Checkbox("silent aimbot", &g_Settings.bSilentAim);

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("hitchance", &g_Settings.iHitChance, 0, 100, "%.1");
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("min damage", &g_Settings.iMinDamage, 0, 100, "%.1");
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //


					static const char* hitboxes[] = {
					"head",
						"neck",
						"chest",
						"stomach",
						"pelvis",
						"arms",
						"legs"
					};
					static std::string asd2 = "";
					if (ImGui::BeginCombo("hitbox", "...", ImVec2(0, 126)))
					{
						asd2 = "";
						std::vector<std::string> item;
						for (auto i = 0; i < ARRAYSIZE(hitboxes); ++i) {
							ImGui::Selectable(hitboxes[i], &g_Settings.bHitboxSelection[i], ImGuiSelectableFlags_DontClosePopups);
							if (g_Settings.bHitboxSelection[i])
								item.push_back(hitboxes[i]);
						}

						for (auto i = 0; i < item.size(); ++i) {
							if (item.empty())
								asd2 += "";
							else if (item.size() == 1)
								asd2 += item[i];
							else if (!(i == item.size()))
								asd2 += item[i] + ", ";
							else
								asd2 += item[i];
						}

						ImGui::EndCombo();
					}

					static std::string asd = "";

					if ((ImGui::BeginCombo("multipoints S", "...", ImVec2(0, 126))))
					{
						asd = "";
						std::vector<std::string> item;
						for (auto i = 0; i < ARRAYSIZE(hitboxes); ++i) {
							ImGui::Selectable(hitboxes[i], &g_Settings.bMultiPointSelection[i], ImGuiSelectableFlags_DontClosePopups);
							if (g_Settings.bMultiPointSelection[i])
								item.push_back(hitboxes[i]);
						}

						for (auto i = 0; i < item.size(); ++i) {
							if (item.empty())
								asd += "";
							else if (item.size() == 1)
								asd += item[i];
							else if (!(i == item.size()))
								asd += item[i] + ", ";
							else
								asd += item[i];
						}

						ImGui::EndCombo();
					}
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("head scale", &g_Settings.iHeadScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("neck scale", &g_Settings.iNeckScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("pelvis scale", &g_Settings.iPelvisScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("stom. scale", &g_Settings.iStomachScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("chest scale", &g_Settings.iChestScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
					ImGui::SliderInt("legs scale", &g_Settings.iLegsScale, 0, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //
				}
				ImGui::EndChild();
				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("accuracy", ImVec2(0, 455), true); {
					ImGui::Checkbox("auto pistol", &g_Settings.bAutomaticWeapons);
					ImGui::Checkbox("auto revolver", &g_Settings.bAutomaticRevolver);
					//ImGui::Checkbox("auto stop", &g_Settings.bAutoStop);
					ImGui::Checkbox("auto scope", &g_Settings.bAutoScope);
					ImGui::Checkbox("resolver", &g_Settings.bResolver);
					//	ImGui::Combo("resolver type", &g_Settings.iResolver, resolvers, ARRAYSIZE(resolvers));

					ImGui::Hotkey("body key", &g_Settings.iBodyKey, ImVec2(110, 35));
				}
				ImGui::EndChild();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //

			}
			if (page == 1) {
				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //+
				static const char* yaw[] = { "off", "backwards", "freestanding", "manual (arrows)" };
				static const char* desync[] = { "off", "balance", "flip", "static", "switch" };
				static const char* fakelag[] = { "off", "maximum", "dynamic", "fluctuate" };
				static const char* fakelag_cond[] = { "while standing",
							"on accelerate",
							"on high speed",
							"while moving" };
				ImGui::BeginChild("AimBot", ImVec2(0, 455), true);
				{





					ImGui::Checkbox("enabled", &g_Settings.bAntiAim);
					ImGui::Combo("yaw", &g_Settings.iYaw, yaw, ARRAYSIZE(yaw));
					ImGui::Checkbox("at targets", &g_Settings.bYawAtTargets);
					ImGui::Checkbox("desync", &g_Settings.bDesync);
					ImGui::Combo("desync type", &g_Settings.DesyncType, desync, ARRAYSIZE(desync));





				}
				ImGui::EndChild();

				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("accuracy", ImVec2(0, 455), true); {

					ImGui::Combo("fakelag", &g_Settings.iFakeLagType, fakelag, ARRAYSIZE(fakelag));
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::SliderInt("FL choke", &g_Settings.iFakeLagChoke, 1, 16);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					static std::string asd = "";
					if (ImGui::BeginCombo("flags", "...", ImVec2(0, 86)))
					{
						asd = "";
						std::vector<std::string> item;
						for (auto i = 0; i < ARRAYSIZE(fakelag_cond); ++i) {
							ImGui::Selectable(fakelag_cond[i], &g_Settings.bFakeLagFlags[i], ImGuiSelectableFlags_DontClosePopups);
							if (g_Settings.bFakeLagFlags[i])
								item.push_back(fakelag_cond[i]);
						}

						for (auto i = 0; i < item.size(); ++i) {
							if (item.empty())
								asd += "";
							else if (item.size() == 1)
								asd += item[i];
							else if (!(i == item.size()))
								asd += item[i] + ", ";
							else
								asd += item[i];
						}

						ImGui::EndCombo();
					}

					ImGui::Checkbox("while shooting", &g_Settings.bFakeLagShooting);

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(180 / 255.f, 180 / 255.f, 99 / 255.f, 255 / 255.f));
					ImGui::Checkbox("fix leg movement", &g_Settings.bFixLegMovement);
					//ImGui::PopStyleColor();
					//if (g_Settings.iYaw == 3) {
					//	ImGui::Text("left");
					//	ImGui::Hotkey("key###left", &g_Settings.fffg, ImVec2(110, 35));
					//	ImGui::Text("right");
					//	ImGui::Hotkey("key###right", &g_Settings.Right, ImVec2(110, 35));
					//	ImGui::Text("back");
					//	ImGui::Hotkey("key###back", &g_Settings.fdd, ImVec2(110, 35));


					//}
					ImGui::Checkbox("slide walk", &g_Settings.bSlideWalk);
					ImGui::Checkbox("fake duck", &g_Settings.bFreezeWalk);
					ImGui::Hotkey("key###fduck", &g_Settings.iFakeDuckKey, ImVec2(110, 35));
					ImGui::Checkbox("slow walk", &g_Settings.bSlowWalk);
					ImGui::Hotkey("key###slow", &g_Settings.iSlowWalkKey, ImVec2(110, 35));

				}
				ImGui::EndChild();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
			}
			if (page == 2) {


				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("player", ImVec2(0, 202), true);
				{


					ImGui::Checkbox("names", &g_Settings.bShowNames);
					ImGui::SameLine();
					ImGui::ColorEdit4("##dfhdhdfhfd", g_Settings.namecolor, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("box", &g_Settings.bShowBoxes);
					ImGui::ColorEdit4("Box color", g_Settings.boxcolor, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("health bar", &g_Settings.bShowHealth);

					ImGui::Checkbox("weapon ammo", &g_Settings.bShowAmmo);
					ImGui::SameLine();
					ImGui::ColorEdit4("##ammadaun", g_Settings.ammocolor, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("weapon name", &g_Settings.bShowWeaponName);
					ImGui::Checkbox("skeleton", &g_Settings.bShowSkeleton);


				}
				ImGui::EndChild();




				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //

				
				ImGui::BeginChild("World", ImVec2(0, 262), true); {

					static const char* wpn[] = { "name",
							"ammo" };
					ImGui::BeginGroup();
					static std::string asd = "";
					if (ImGui::BeginCombo("dropped weapons", "...", ImVec2(0, 42)))
					{
						asd = "";
						std::vector<std::string> item;
						for (auto i = 0; i < ARRAYSIZE(wpn); ++i) {
							ImGui::Selectable(wpn[i], &g_Settings.bDroppedItems[i], ImGuiSelectableFlags_DontClosePopups);
							if (g_Settings.bDroppedItems[i])
								item.push_back(wpn[i]);
						}

						for (auto i = 0; i < item.size(); ++i) {
							if (item.empty())
								asd += "";
							else if (item.size() == 1)
								asd += item[i];
							else if (!(i == item.size()))
								asd += item[i] + ", ";
							else
								asd += item[i];
						}

						ImGui::EndCombo();
					}
					ImGui::EndGroup();

					ImGui::SameLine();
					ImGui::ColorEdit4("###weannnnn", g_Settings.droppedweaponname, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("projectiles", &g_Settings.bShowProjectiles);
					ImGui::SameLine();
					ImGui::ColorEdit4("##projectilecolorasdasdasdasd", g_Settings.projectilecolor, ImGuiColorEditFlags_NoInputs);

					static const char* wpnn[] = { "name",
							"ammo" };
					ImGui::BeginGroup();
					static std::string asd2 = "";
					if (ImGui::BeginCombo("item glow", "...", ImVec2(0, 42)))
					{
						asd2 = "";
						std::vector<std::string> item;
						for (auto i = 0; i < ARRAYSIZE(wpnn); ++i) {
							ImGui::Selectable(wpnn[i], &g_Settings.bItemGlow[i], ImGuiSelectableFlags_DontClosePopups);
							if (g_Settings.bItemGlow[i])
								item.push_back(wpn[i]);
						}

						for (auto i = 0; i < item.size(); ++i) {
							if (item.empty())
								asd2 += "";
							else if (item.size() == 1)
								asd2 += item[i];
							else if (!(i == item.size()))
								asd2 += item[i] + ", ";
							else
								asd2 += item[i];
						}

						ImGui::EndCombo();
					}
					ImGui::EndGroup();

					ImGui::Checkbox("night mode", &g_Settings.bNightMode);
					ImGui::Checkbox("remove skybox", &g_Settings.bNoSky);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::SliderFloat("world Trans", &g_Settings.flTransparentWorld, 0.f, 100.f, "%.2f");
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::SliderFloat("prop Trans", &g_Settings.flTransparentProps, 0.f, 100.f, "%.2f");
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //



				}
				ImGui::EndChild();


				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //




				ImGui::BeginChild("chams", ImVec2(0, 252), true); {


					static const char* chams[] = { "off", "normal", "flat", "wireframe" };
					static const char* glow[] = { "off", "outline", "pulse" };
					ImGui::Checkbox("player model", &g_Settings.bShowChams);
					ImGui::SameLine();
					ImGui::ColorEdit4("##chamsv", g_Settings.chamshidden, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("player model behind walls", &g_Settings.bShowChamsXQZ);
					ImGui::SameLine();
					ImGui::ColorEdit4("##bnbvnvn", g_Settings.chamsvisible, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("type", &g_Settings.iChamsMode, chams, ARRAYSIZE(chams));


					ImGui::Combo("player glow", &g_Settings.iGlowStyle, glow, ARRAYSIZE(glow));
					ImGui::SameLine();
					ImGui::ColorEdit4("##54565656456", g_Settings.glowcolorother, ImGuiColorEditFlags_NoInputs);

					//ImGui::Checkbox("local player model", &g_Settings.bShowLocalChams);
					//ImGui::SameLine();
					//ImGui::ColorEdit4("##dasfreety", g_Settings.chamshiddenL, ImGuiColorEditFlags_NoInputs);
					//ImGui::Checkbox("local player model behind walls", &g_Settings.bShowLocalChamsXQZ);
					//ImGui::SameLine();
					//ImGui::ColorEdit4("##6fvbbi", g_Settings.chamsvisibleL, ImGuiColorEditFlags_NoInputs);
					//static const char* lcch[] = { "off", "normal", "flat", "original" };
					//ImGui::Combo("type###localplayer", &g_Settings.iLocalChamsMode, lcch, ARRAYSIZE(lcch));



					static const char* lcgl[] = { "off", "outline", "pulse" };
					ImGui::Combo("local glow", &g_Settings.iLocalGlowStyle, lcgl, ARRAYSIZE(lcgl));
					ImGui::SameLine();
					ImGui::ColorEdit4("##fghfgfgjfgjgf", g_Settings.glowcolorlocal, ImGuiColorEditFlags_NoInputs);

					ImGui::EndChild();
				}


				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //




				ImGui::BeginChild("Other", ImVec2(0, 252), true); {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::SliderInt("scope blend", &g_Settings.iScopedBlend, 1, 100);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					//	sectMain2->AddSlider("entity blend", &g_Settings.iEntBlend, 1, 100);
					ImGui::SliderFloat("world fov", &g_Settings.flWorldFov, 0, 120);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::SliderFloat("viewmodel fov", &g_Settings.flViewModelFov, 0, 120);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50); //

					ImGui::Checkbox("no scope ", &g_Settings.bScopeNoZoom);
					ImGui::Checkbox("no zoom ", &g_Settings.bNoZoom);
					ImGui::Checkbox("no visual recoil", &g_Settings.bNoVisRecoil);
					ImGui::Checkbox("no flash", &g_Settings.bNoSmoke);
					ImGui::Checkbox("no smoke", &g_Settings.bNoFlash);
					ImGui::Checkbox("no post processing", &g_Settings.bNoPostProcess);
					ImGui::Checkbox("autowall crosshair", &g_Settings.bAutoWallCrosshair);
					ImGui::Checkbox("spread scope lines ", &g_Settings.bSpreadScope);
					ImGui::Checkbox("hitmarker", &g_Settings.bHitmaker);
					ImGui::Checkbox("FLAGS", &FLAGS);


					ImGui::Checkbox("bullet tracer", &g_Settings.bBulletTracers);
					ImGui::SameLine();
					ImGui::ColorEdit4("##bhgjgj", g_Settings.bullettracer, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("bullet impact", &g_Settings.bBulletImpacts);
					ImGui::SameLine();
					ImGui::ColorEdit4("##kl;kl", g_Settings.impact, ImGuiColorEditFlags_NoInputs);

					static const char* hitsound[] = { "off", "skeet", "bubble", "bameware", "cod", "porn" };
					ImGui::Combo("hitsound", &g_Settings.iHitSound, hitsound, ARRAYSIZE(hitsound));


					ImGui::EndChild();
				}


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			}

			if (page == 3) {


				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //

				ImGui::BeginChild("knife", ImVec2(0, 543), true); {

					if (ImGui::SmallButton("update"))
						KnifeApplyCallbk();

					static const char* kn[] = { "off", "bayonet", "flip", "gut", "karambit",
						"m9 bayonet", "huntsman", "falchion", "bowie",
						"butterfly", "shadow daggers", "ursus", "navaja",
						"stiletto", "talon" };
					ImGui::Combo("knife", &g_Settings.iKnifeModel, kn, ARRAYSIZE(kn));
				}
				ImGui::EndChild();


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

			}

			if (page == 4) {

				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //

				ImGui::BeginChild("Misc 1", ImVec2(0, 272), true);
				ImGui::Checkbox("anti untrusted", &g_Settings.AntiUT);

				ImGui::Checkbox("bunnyhop", &g_Settings.bBhopEnabled);
				ImGui::Checkbox("airstrafe", &g_Settings.bAirStrafer);

				ImGui::Checkbox("thirdperson (alive)", &g_Settings.bThirdPersonAlive);
				ImGui::Hotkey("key###thirdperson", &g_Settings.iThirdPersonKey, ImVec2(110, 35));

				ImGui::Checkbox("thirdperson (dead)", &g_Settings.bThirdPersonDead);
				ImGui::Checkbox("preserve killfeed", &g_Settings.bPreserveKillFeed);
				ImGui::Checkbox("log damage", &g_Settings.bDamageLogs);
				ImGui::Checkbox("indicators", &g_Settings.bUselessInfo);
				ImGui::Checkbox("clantag changer", &g_Settings.bClantag);
				//ImGui::Checkbox("Freeze Size", &g_Settings.FSIZE);

				ImGui::Checkbox("kill say", &g_Settings.bKillSay);
				ImGui::Checkbox("force crosshair", &g_Settings.bEngineCrosshair);

				ImGui::EndChild();


			}

			if (page == 5) {
				ImGui::Columns(2, NULL, false);
				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("menuColor", ImVec2(0, 202), true);
				if (ImGui::Button("shutdown", ImVec2(245, 25)))
					Detach();



				//ImGui::Checkbox("WaterMark", &g_Settings.WaterMark);


				//ImGui::Text("watermark Color 1");
				//ImGui::SameLine();
				//ImGui::ColorEdit4("watermark Color 1", g_Settings.cWatermark, ImGuiColorEditFlags_NoInputs);

				//ImGui::Text("watermark Color 2");
				//ImGui::SameLine();
				//ImGui::ColorEdit4("watermark Color 2", g_Settings.cWatermarkt, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("Menu Color");
				ImGui::SameLine();
				ImGui::ColorEdit4("Menu colorç", g_Settings.cMenuCol, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();

				ImGui::EndChild();

				ImGui::NextColumn();


				ImGui::PushFont(Menu);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10); //
				ImGui::BeginChild("Config", ImVec2(0, 252), true); {


					ImGui::InputText("config name", buf1, 64);

					if (ImGui::Button("save cfg", ImVec2(245, 25)))
						save();
					if (ImGui::Button("load cfg", ImVec2(245, 25)))
						load();

					ImGui::EndChild();
				}
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

			}
		}

		ImGui::PopFont();

		ImGui::End();
	}

}
void c_menu::run_popup() {
	ImGui::PushFont(font_menu);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

	if (save_config) {
		bool done = false;
		ImGui::begin_popup("config saved.", 2000, &done);
		if (done)
			save_config = false;
	}

	if (load_config) {
		bool done = false;
		ImGui::begin_popup("config loaded.", 2000, &done);
		if (done)
			load_config = false;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}


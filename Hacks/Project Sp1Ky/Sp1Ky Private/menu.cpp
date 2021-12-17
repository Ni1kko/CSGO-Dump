#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"
#include "droid.hpp"
#include "features/Configs.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"
#include "Hitmarker.h"
#include <crtdbg.h>
#include <assert.h>
#include "ui.hpp"
#include "features/XorCompileTime.hpp"
const char* Hitsounds[] =
{
	XorStr("Off"),
	XorStr("Skeet"),
	XorStr("Normal"),
	XorStr("Bubble")
};
static inline char* memdup(const char* s, size_t n)
{
	char* t = (char*)malloc(n);
	memcpy(t, s, n);
	return t;
}
#define _memdup(object) memdup(object, sizeof(object))
static char* sidebar_tabs[] = {
"Legit",
"Visuals",
"Misc",
"Skins",
"Config"
};
ImFont* menufont;
static ConVar* cl_mouseenable = nullptr;
constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }
namespace ImGuiEx
{
    inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
    {
        auto clr = ImVec4{
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };
        if(ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
            v->SetColor(clr.x, clr.y, clr.z, clr.w);
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Color* v)
    {
        return ColorEdit4(label, v, false);
    }
}
template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
	auto& style = ImGui::GetStyle();
	bool values[N] = { false };
	values[activetab] = true;
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.15f, 1.f);
	for (auto i = 0; i < N; ++i) {
		if (activetab == i)
		{
			style.Colors[ImGuiCol_Text] = ImVec4(0.59f, 0.59f, 0.62f, 1.f);
		}
		else
		{
			style.Colors[ImGuiCol_Text] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
		}
		if (ImGui::Button(names[i], ImVec2{ w, h })) {
			activetab = i;
		}
		if (sameline && i < N - 1)
			ImGui::SameLine();
	}
	style.Colors[ImGuiCol_Text] = ImVec4(0.51f, 0.51f, 0.51f, 1.f);
}

static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

ImVec2 get_sidebar_size()
{
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
    constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();
    return ImVec2{ size_w, ImMax(325.0f, size_h) };
}
int get_fps()
{
    using namespace std::chrono;
    static int count = 0;
    static auto last = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    static int fps = 0;
    count++;
    if(duration_cast<milliseconds>(now - last).count() > 1000) {
        fps = count;
        count = 0;
        last = now;
    }
    return fps;
}
void RenderEspTab()
{
    static char* total_l_e_sp_nam_tab[] = { "Esp", "Chams" };
    static int   active_esp_tab = 0;
    bool placeholder_true = true;
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
	style.WindowPadding = ImVec2(0, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    {
		ImGui::BeginChild(XorStr("##Tabs"), ImVec2(0, 26), true);
        render_tabs(total_l_e_sp_nam_tab, active_esp_tab, group_w / _countof(total_l_e_sp_nam_tab), 25.0f, true);
		ImGui::EndChild();
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.15f, 0.15f, 0.16f, 1.f);
		ImGui::BeginChild(XorStr("##Make_Space"), ImVec2(0, 16), false);
		ImGui::EndChild();
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.f);
	}
	style.WindowPadding = ImVec2(16, 16);
    ImGui::PopStyleVar();
    {
		ImGui::Columns(2, NULL, false);
		ImGui::BeginChild(XorStr("##body_contentt"), ImVec2(0, 0), true);
        if(active_esp_tab == 0) {
			ImGui::DrawGroupName(XorStr("Esp"));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
			
            ImGui::Checkbox(XorStr("Enabled"), &g_Options.legit_enabled_esp);
            ImGui::Checkbox(XorStr("Check team"), &g_Options.legit_only_enemy);
            ImGui::Checkbox(XorStr("Box"), &g_Options.legit_box_toggle);
            ImGui::Checkbox(XorStr("Names"), &g_Options.legit_playa_name);
            ImGui::Checkbox(XorStr("Health"), &g_Options.legit_playa_heal);
            ImGui::Checkbox(XorStr("Armor"), &g_Options.legit_playa_arm);
            ImGui::Checkbox(XorStr("Weapon"), &g_Options.legit_playa_weap);
            ImGui::Checkbox(XorStr("Crosshair"), &g_Options.legit_self_cros);
            ImGui::Checkbox(XorStr("Dropped Weapons"), &g_Options.legit_playa_dro_wep);
            ImGui::Checkbox(XorStr("Defuse Kit"), &g_Options.legit_playa_def);
			ImGui::Checkbox(XorStr("Planted C4 "), &g_Options.legit_dog_bom);
            ImGui::PopStyleVar();
        } 
		 else if(active_esp_tab == 1) {
			ImGui::DrawGroupName(XorStr("Chams - Players"));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::BeginGroupBox(XorStr("Players"));
            {
                ImGui::Checkbox(XorStr("Enabled"), &g_Options.legit_dog_cham_enb); ImGui::SameLine();
                ImGui::Checkbox(XorStr("Check team"), &g_Options.legit_dog_cham_only_en);
                ImGui::Checkbox(XorStr("Wireframe"), &g_Options.legit_dog_kostickovany);
                ImGui::Checkbox(XorStr("Flat"), &g_Options.legit_dog_flat);
                ImGui::Checkbox(XorStr("Ignore-Z"), &g_Options.legit_dog_ignz); ImGui::SameLine();
                ImGui::Checkbox(XorStr("Glass"), &g_Options.legit_dog_sklicko);
                ImGui::PushItemWidth(110);
                ImGuiEx::ColorEdit4(XorStr("Team Vis."), &g_Options.legit_dog_cham_tm_vis);
                ImGuiEx::ColorEdit4(XorStr("Team Inv."), &g_Options.legit_dog_cham_tm_invs);
                ImGuiEx::ColorEdit4(XorStr("Enemy Vis."), &g_Options.legit_dog_cham_entm_vis);
                ImGuiEx::ColorEdit4(XorStr("Enemy Inv."), &g_Options.legit_dog_cham_entm_invis);
                ImGui::PopItemWidth();			
            }
            ImGui::EndGroupBox();
            ImGui::PopStyleVar();
        }
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::BeginChild(XorStr("##content_2"), ImVec2(0, 0), true);
		{
			if (active_esp_tab == 0) {
				ImGui::DrawGroupName(XorStr("Esp - Colors"));
				ImGuiEx::ColorEdit3(XorStr("Team Vis."), &g_Options.legit_dog_dog_vis_col_tm);
				ImGuiEx::ColorEdit3(XorStr("Enemy Vis."), &g_Options.legit_dog_dog_vis_col_en_vis);
				ImGuiEx::ColorEdit3(XorStr("Team Inv."), &g_Options.legit_dog_dog_vis_col_tm_vis_cl);
				ImGuiEx::ColorEdit3(XorStr("Enemy Inv."), &g_Options.legit_dog_dog_vis_col_entm_invs);
				ImGuiEx::ColorEdit3(XorStr("Crosshair"), &g_Options.legit_dog_dog_dog_cr_col);
				ImGuiEx::ColorEdit3(XorStr("Weapons"), &g_Options.legit_pes_dog_wep);
				ImGuiEx::ColorEdit3(XorStr("Defuse Kit"), &g_Options.legit_col_def_kyt);
				ImGuiEx::ColorEdit3(XorStr("Planted C4"), &g_Options.legit_col_pl_c_bo);
			}
			else if (active_esp_tab == 1) {
				ImGui::DrawGroupName(XorStr("Chams - Arms"));
				ImGui::Checkbox(XorStr("Enabled"), &g_Options.legit_chsm_amr_en);
				ImGui::Checkbox(XorStr("Wireframe"), &g_Options.legit_chsm_amr_kostickovany);
				ImGui::Checkbox(XorStr("Flat"), &g_Options.legit_chsm_amr_fl);
				ImGui::Checkbox(XorStr("Ignore-Z"), &g_Options.legit_chsm_amr_igz);
				ImGui::Checkbox(XorStr("Glass"), &g_Options.legit_chsm_amr_sklicko);
				ImGuiEx::ColorEdit4(XorStr("Visible"), &g_Options.cl_legit_chsm_amr_visvis);
				ImGuiEx::ColorEdit4(XorStr("Invisible"), &g_Options.clcl_legit_chsm_amr_invs);
			}
		}
		ImGui::EndChild();
    }
}
void RenderMiscTab()
{
    bool placeholder_true = true;
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
	ImGui::Columns(2, NULL, false);
    ImGui::BeginChild(XorStr("##body_content"), ImVec2(0,0), true);
    {
		ImGui::DrawGroupName(XorStr("Misc"));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
        ImGui::Checkbox(XorStr("Bunny Hop"), &g_Options.more_more_mi_b_hop);
		ImGui::Checkbox(XorStr("Auto Strafe"), &g_Options.more_more_mi_a_str_ejf);
		ImGui::Checkbox(XorStr("NightMode"), &g_Options.nightmode);
		ImGui::Checkbox(XorStr("Hitmarker"), &g_Options.more_more_mi_hejt_merkr);
		ImGui::Checkbox(XorStr("Clantag"), &g_Options.more_more_ani_mejtid_klentek);
		ImGui::Checkbox(XorStr("Spectators"), &g_Options.more_more_spe_tcetrs);
    }
    ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild(XorStr("##body_content2"), ImVec2(0,0), true);
	{
		ImGui::DrawGroupName(XorStr("Other"));
		ImGui::SliderInt(XorStr("Viewmodel Fov:"), &g_Options.viewmodel_fov, 68, 120);
		ImGui::Checkbox(XorStr("Bullet Tracer"), &g_Options.more_more_more_bu_ll_ert_rejsr);
		ImGui::Checkbox(XorStr("Bullet Beams"), &g_Options.more_more_more_bu_ll_ert_bejmz);
		ImGui::Checkbox(XorStr("Show Backtrack"), &g_Options.more_more_more_bektrkr_ske__Lt);
		ImGui::Checkbox(XorStr("Chat Spam"), &g_Options.more_more_ara_spemrkk);
	}
	ImGui::EndChild();
	ImGui::Columns(1);
}
static int weapon_index = 7;
static std::map<int, const char*> k_weapon_names =
{
{ 7, _memdup(XorStr("AK-47")) },
{ 8, _memdup(XorStr("AUG")) },
{ 9, _memdup(XorStr("AWP")) },
{ 63, _memdup(XorStr("CZ75 Auto")) },
{ 1, _memdup(XorStr("Desert Eagle")) },
{ 2, _memdup(XorStr("Dual Berettas")) },
{ 10, _memdup(XorStr("FAMAS")) },
{ 3, _memdup(XorStr("Five-SeveN")) },
{ 11, _memdup(XorStr("G3SG1")) },
{ 13, _memdup(XorStr("Galil AR")) },
{ 4, _memdup(XorStr("Glock-18")) },
{ 14, _memdup(XorStr("M249")) },
{ 60, _memdup(XorStr("M4A1-S")) },
{ 16, _memdup(XorStr("M4A4")) },
{ 17, _memdup(XorStr("MAC-10")) },
{ 27, _memdup(XorStr("MAG-7")) },
{ 33, _memdup(XorStr("MP7")) },
{ 34, _memdup(XorStr("MP9")) },
{ 28, _memdup(XorStr("Negev")) },
{ 35, _memdup(XorStr("Nova")) },
{ 32, _memdup(XorStr("P2000")) },
{ 36, _memdup(XorStr("P250")) },
{ 19, _memdup(XorStr("P90")) },
{ 26, _memdup(XorStr("PP-Bizon")) },
{ 64, _memdup(XorStr("R8 Revolver")) },
{ 29, _memdup(XorStr("Sawed-Off")) },
{ 38, _memdup(XorStr("SCAR-20")) },
{ 40, _memdup(XorStr("SSG 08")) },
{ 39, _memdup(XorStr("SG 553")) },
{ 30, _memdup(XorStr("Tec-9")) },
{ 24, _memdup(XorStr("UMP-45")) },
{ 61, _memdup(XorStr("USP-S")) },
{ 25, _memdup(XorStr("XM1014")) },
};
void RenderCurrentWeaponButton()
{
	if (!g_EngineClient->IsConnected() || !g_LocalPlayer || !g_LocalPlayer->IsAlive())
	{
		return;
	}
	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon || !weapon->IsWeapon()) {
		return;
	}
	if (ImGui::Button(XorStr("Current"))) {
		weapon_index = weapon->m_Item().m_iItemDefinitionIndex();
	}
}
void RenderSkinsTab()
{
	if (k_skins.size() == 0) {
		initialize_kits();
	}
	auto& entries = g_Options.skins.m_items;
	static auto definition_vector_index = 0;
	ImGui::Columns(2, nullptr, false);
	{
		ImGui::BeginChild(XorStr("##content_1"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::DrawGroupName(XorStr("Weapon Selection"));
		ImGui::PushItemWidth(-1);
		ImGui::ListBoxHeader(XorStr("##config"));
		{
			for (size_t w = 0; w < k_weapon_names.size(); w++) {
				if (ImGui::Selectable(k_weapon_names_s[w].name, definition_vector_index == w)) {
					definition_vector_index = w;
				}
			}
		}
		ImGui::ListBoxFooter();
		if (ImGui::Button(XorStr("Update")))
			g_ClientState->ForceFullUpdate();
		ImGui::PopItemWidth();
		ImGui::EndChild();
	}
	ImGui::NextColumn();
	{
		
		ImGui::BeginChild(XorStr("##content_2"), ImVec2(0, 0), true);
		ImGui::DrawGroupName(XorStr("Skinchanger"));
		auto& selected_entry = entries[k_weapon_names_s[definition_vector_index].definition_index];
		selected_entry.definition_index = k_weapon_names_s[definition_vector_index].definition_index;
		selected_entry.definition_vector_index = definition_vector_index;
		ImGui::Checkbox(XorStr("Enabled"), &selected_entry.enabled);
		ImGui::InputInt(XorStr("Seed"), &selected_entry.seed);
		ImGui::InputInt(XorStr("StatTrak"), &selected_entry.stat_trak);
		ImGui::SliderFloat(XorStr("Wear"), &selected_entry.wear, FLT_MIN, 1.f, XorStr("%.10f"), 5);
		if (selected_entry.definition_index != GLOVE_T_SIDE)
		{
			ImGui::Combo(XorStr("Paint Kit"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = k_skins[idx].name.c_str();
				return true;
			}, nullptr, k_skins.size(), 10);
			selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].Skinchanger;
		}
		else
		{
			ImGui::Combo(XorStr("Paint Kit"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = k_gloves[idx].name.c_str();
				return true;
			}, nullptr, k_gloves.size(), 10);
			selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].Skinchanger;
		}
		if (selected_entry.definition_index == WEAPON_KNIFE)
		{
			ImGui::Combo(XorStr("Knife"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = k_knife_names.at(idx).name;
				return true;
			}, nullptr, k_knife_names.size(), 5);
			selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
		}
		else if (selected_entry.definition_index == GLOVE_T_SIDE)
		{
			ImGui::Combo(XorStr("Glove"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = k_glove_names.at(idx).name;
				return true;
			}, nullptr, k_glove_names.size(), 5);
			selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
		}
		ImGui::InputText(XorStr("Name Tag"), selected_entry.custom_name, 32);
		ImGui::EndChild();
	}
	ImGui::Columns(1, nullptr, false);
}
void RenderAimbotTab()
{
	ImGui::Columns(2, NULL, false);
	ImGui::BeginChild(XorStr("##aimbot.general"), ImVec2(0, 200), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::DrawGroupName(XorStr("Aimbot"));
		ImGui::PushItemWidth(-1);
		ImGui::Columns(1, NULL, true);
		{
			ImGui::Checkbox(XorStr("Enabled"), &g_Options.total_total_l_enabled);
			//ImGui::Checkbox(XorStr("Recoil Control System"), &g_Options.total_total_l_r____cs);
			ImGui::Checkbox(XorStr("Backtrack"), &g_Options.total_total_l_m_exi__kero);
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild(XorStr("##aimbot.misc"), ImVec2(0, 205), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImGui::PushItemWidth(-1);
		ImGui::Columns(1, NULL, true);
		{
			ImGui::PushItemWidth(-1);
			ImGui::Text(XorStr("Hitbox Primary"));
			ImGui::Combo(XorStr("##PREAIMSPOT"), &g_Options.total_total_l_topka_hit_bejx, opt_AimSpot, 4);
			ImGui::NewLine();
			ImGui::Text(XorStr("Secondary hitbox after missed shots"));
			ImGui::SliderInt(XorStr("##BULLETAFTERAIM"), &g_Options.total_total_l_vymrdanejzasobnik, 3, 15);
			ImGui::NewLine();
			ImGui::Text(XorStr("Hitbox Secondary"));
			ImGui::Combo(XorStr("##AFTERAIMSPOT"), &g_Options.total_total_l_tdolkaa_hit_dolni_bejk, opt_AimSpot, 4);
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild(XorStr("##aimbot.Recoil"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImGui::PushItemWidth(-1);
		ImGui::Columns(1, NULL, true);
		{
			ImGui::PushItemWidth(-1);
			ImGui::Text(XorStr("Fov"));
			ImGui::SliderFloat(XorStr("##FOV"), &g_Options.total_total_l_ejfouvykkk, 1.f, 5.f, XorStr("%.2f"));
			ImGui::NewLine();
			ImGui::Text(XorStr("Smooth"));
			ImGui::SliderFloat(XorStr("##SMOOTH"), &g_Options.total_total_l_zmensimtitozatebe_sm_fct, 1.f, 7.f, XorStr("%.2f"));
			ImGui::PopItemWidth();
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::Columns(1, NULL, false);
}
void RenderConfigTab()
{
	auto& style = ImGui::GetStyle();
	float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
	bool placeholder_true = true;
		ImGui::Columns(2, NULL, false);
		ImGui::BeginChild("##Config_body_content", ImVec2(0,0), true);
		ImGui::DrawGroupName(XorStr("Configs"));
		if (ImGui::Button(XorStr("Load"))) Configs::total_sheja_loud_kenfig(XorStr("legit.ini")), ImVec2{ 150, 25 };
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Save"))) Configs::total_sheja_sejvit_kenfig(XorStr("legit.ini")), ImVec2{ 150, 25 };
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Unload"))) ImVec2{ 150, 25 };
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Clean"))) ImVec2{ 150, 25 };
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::Columns(1);
}
void RenderEmptyTab()
{
	auto& style = ImGui::GetStyle();
	float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
	bool placeholder_true = true;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::ToggleButton(XorStr("CONFIG"), &placeholder_true, ImVec2{ group_w, 25.0f });
	ImGui::PopStyleVar();
	ImGui::BeginGroupBox(XorStr("##body_content"));
	{
		auto message = XorStr("Configs");
		auto pos = ImGui::GetCurrentWindow()->Pos;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		auto wsize = ImGui::GetCurrentWindow()->Size;
		pos = pos + wsize / 2.0f;
		ImGui::RenderText(pos - ImGui::CalcTextSize(message) / 2.0f, message);
	}
	ImGui::EndGroupBox();
}
void Menu::Initialize()
{
    _visible = true;
    cl_mouseenable = g_CVar->FindVar(XorStr("cl_mouseenable"));
	ImGui::CreateContext();
    ImGui_ImplDX9_Init(InputSys::Get().GetMainWindow(), g_D3DDevice9);
    CreateStyle();
	ImGui::GetIO().Fonts->Clear();
	menufont = ImGui::GetIO().Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\impact.ttf"), 12);
	ImGui::GetIO().FontDefault = menufont;
}
void Menu::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
    cl_mouseenable->SetValue(true);
}
void Menu::OnDeviceLost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}
void Menu::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}
void SpecListStyle()
{
	ImVec4* colorss = ImGui::GetStyle().Colors;
	colorss[ImGuiCol_WindowBg].w = ImColor(21, 21, 21, 255);
	colorss[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
	colorss[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
	colorss[ImGuiCol_TitleBgActive] = ImColor(21, 21, 21, 255);
	colorss[ImGuiCol_CloseButton] = ImColor(0, 0, 0, 0);
	colorss[ImGuiCol_CloseButtonHovered] = ImColor(0, 0, 0, 0);
	colorss[ImGuiCol_CloseButtonActive] = ImColor(0, 0, 0, 0);
}
void Menu::Render()
{
    if(!_visible)
        return;	
    ImGui_ImplDX9_NewFrame();
    ImGui::GetIO().MouseDrawCursor = _visible;
    const auto sidebar_size = get_sidebar_size();
    static int active_sidebar_tab = 0;
	auto& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(7, 7);
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 702, 474 }, ImGuiSetCond_Once);
	
	
	style.Colors[ImGuiCol_Border] = ImVec4(0.09f, 0.09f, 0.10f, 1.f);
	if (ImGui::Begin("", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove)) {
		style.WindowPadding = ImVec2(0, 0);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.15f, 0.15f, 0.16f, 1.f);
		ImGui::BeginChild(XorStr("##ALL"), ImVec2(0, 0), true);
		{
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.13f, 0.13f, 0.15f, 1.f);
			ImGui::BeginChild(XorStr("##tabs Window"), ImVec2(0, 51), false);
			{
				ImVec2 windowpos = ImGui::GetWindowPos();
				style.ItemInnerSpacing = ImVec2(0, 0);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				{
					float WindowWidth = ImGui::GetWindowWidth();
					style.Colors[ImGuiCol_CustomColor4] = ImVec4(0.12f, 0.14f, 0.37f, 1.f); //dark left
					style.Colors[ImGuiCol_CustomColor3] = ImVec4(0.32f, 0.39f, 0.69f, 1.f); //dark right 
					style.Colors[ImGuiCol_CustomColor2] = ImVec4(0.44f, 0.54f, 0.94f, 1.f); //right 
					style.Colors[ImGuiCol_CustomColor] = ImVec4(0.22f, 0.29f, 0.94f, 1.f);  //left
					ImGui::DrawLine(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + WindowWidth, windowpos.y + 2));
					style.Colors[ImGuiCol_CustomColor] = ImVec4(0.22f, 0.29f, 0.94f, 1.f); //left
					style.Colors[ImGuiCol_CustomColor2] = ImVec4(0.32f, 0.39f, 0.69f, 1.f); //right 
					style.Colors[ImGuiCol_CustomColor3] = ImVec4(0.44f, 0.54f, 0.94f, 1.f); // dark right 
					style.Colors[ImGuiCol_CustomColor4] = ImVec4(0.21f, 0.29f, 0.68f, 1.f);  //dark left
					ImGui::DrawLine(ImGui::GetWindowPos() + ImVec2(0, 2), ImVec2(ImGui::GetWindowPos().x + WindowWidth, windowpos.y + 4));
					auto message = (XorStr("Sp1Ky | Premium Cheat"));
					ImVec2 textSize = ImGui::CalcTextSize(message);
					ImGui::RenderText(windowpos + ImVec2((ImGui::GetWindowWidth() / 2.f) - (textSize.x / 2.f), 8), message);
					ImGui::NewLine(); ImGui::NewLine();
					float group_w = ImGui::GetCurrentWindow()->Size.x - 2.f;
					float width = (group_w / _countof(sidebar_tabs));
					render_tabs(sidebar_tabs, active_sidebar_tab, width, 26.0f, true);
				}
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
			ImVec2 windowpos = ImGui::GetWindowPos();
			float WindowWidth = ImGui::GetWindowWidth();
			style.Colors[ImGuiCol_CustomColor] = ImVec4(0.f, 0.f, 0.f, 0.00f);
			style.Colors[ImGuiCol_CustomColor2] = ImVec4(0.f, 0.f, 0.f, 0.0f);
			style.Colors[ImGuiCol_CustomColor3] = ImVec4(0.f, 0.f, 0.f, 0.25f);
			style.Colors[ImGuiCol_CustomColor4] = ImVec4(0.f, 0.f, 0.f, 0.25f);
			ImGui::DrawLine(ImGui::GetWindowPos() + ImVec2(0, 51), ImVec2(ImGui::GetWindowPos().x + WindowWidth, windowpos.y + 60));//shadow effect
			auto size = ImVec2{ 0.0f, 0.0f };
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.15f, 0.15f, 0.16f, 1.f);
			style.WindowPadding = ImVec2(16, 16);
			style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.27f, 0.f);
			ImGui::BeginChild(XorStr("selected_content"), size, true, ImGuiWindowFlags_NoScrollbar);
			{
				style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.27f, 1.f);
				style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.f);
				if (active_sidebar_tab == 0) {
					RenderAimbotTab();
				}
				else if (active_sidebar_tab == 1) {
					RenderEspTab();
				}
				else if (active_sidebar_tab == 2) {
					RenderMiscTab();
				}
				else if (active_sidebar_tab == 3) {
					RenderSkinsTab();
				}
				else if (active_sidebar_tab == 4) {
					RenderConfigTab();
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}
	ImGui::End();
    ImGui::Render();
}
void Menu::Show()
{
    _visible = true;
    cl_mouseenable->SetValue(false);
}
void Menu::Hide()
{
    _visible = false;
    cl_mouseenable->SetValue(true);
}
void Menu::Toggle()
{
	cl_mouseenable->SetValue(_visible);
    _visible = !_visible;
}
void Menu::CreateStyle()
{
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 14.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
#ifdef Jaccko
	_style.Alpha                  = 1.0f;                                // Global alpha applies to everything in ImGui
    _style.WindowPadding          = ImVec2(7, 7);                      // Padding within a window
    _style.WindowMinSize          = ImVec2(100, 100);                    // Minimum window size
    _style.WindowRounding         = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    _style.WindowTitleAlign       = ImVec2(0.0f, 0.5f);                  // Alignment for title bar text
    _style.FramePadding           = ImVec2(5, 5);                        // Padding within a framed rectangle (used by most widgets)
    _style.FrameRounding          = 2.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    _style.ItemSpacing            = ImVec2(5, 5);                        // Horizontal and vertical spacing between widgets/lines
    _style.ItemInnerSpacing       = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    _style.TouchExtraPadding      = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    _style.IndentSpacing          = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    _style.ColumnsMinSpacing      = 6.0f;                                // Minimum horizontal spacing between two columns
    _style.ScrollbarSize          = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
    _style.ScrollbarRounding      = 9.0f;                                // Radius of grab corners rounding for scrollbar
    _style.GrabMinSize            = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
    _style.GrabRounding           = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    _style.ButtonTextAlign        = ImVec2(0.5f, 0.5f);                  // Alignment of button text when button is larger than text.
    _style.DisplayWindowPadding   = ImVec2(22, 22);                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
    _style.DisplaySafeAreaPadding = ImVec2(4, 4);                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    _style.AntiAliasedLines       = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    _style.CurveTessellationTol   = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
	
#else
	ImGui::StyleColorsDark();
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
	_style.FrameRounding = 0.f;
	_style.WindowRounding = 0.f;
	_style.ChildRounding = 0.f;
	_style.Colors[ImGuiCol_Button] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
	_style.Colors[ImGuiCol_Header] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
	_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
	//_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.000f, 0.545f, 1.000f, 1.000f);
	//_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.060f, 0.416f, 0.980f, 1.000f);
	_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
	_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.000f, 0.009f, 0.120f, 0.940f);
	_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.076f, 0.143f, 0.209f, 1.000f);
	_style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	_style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	_style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	_style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
	_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	_style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	_style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	_style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	_style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	_style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	_style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	_style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	//_style.Colors[ImGuiCol_TitleText]            = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
	_style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	_style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	_style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	_style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	_style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	//_style.Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	_style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
	_style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	_style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	_style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
	_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
	_style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	_style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	_style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	_style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	_style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	_style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	_style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	_style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//_style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
	//_style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
	//_style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
	_style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	_style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	_style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	_style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	_style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
#endif
	ImGui::GetStyle() = _style;
}







































































































































































































































































































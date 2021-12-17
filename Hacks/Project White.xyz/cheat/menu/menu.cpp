#include <Windows.h>
#include <chrono>
#include "Menu.hpp"
#include "../../sdk/structs.hpp"
#include "../../helpers/input.hpp"
#include "../config/options.hpp"
#include "../config/config.hpp"
#include "../visuals/visuals.hpp"
#include "../other/ui.hpp"
#include "../../fonts/droid.hpp"
#include "../../fonts/cousine.hpp"
#include "../../imgui/imgui_internal.h"
#include "../../imgui/impl/imgui_impl_dx9.h"
#include "../../imgui/impl/imgui_impl_win32.h"
#include "../skins/skins.hpp"

//--------------------------------------------------------------------------------
#include <filesystem>
namespace fs = std::filesystem;
//--------------------------------------------------------------------------------
#pragma region font
ImFont* MainCaps;
ImFont* Main;
ImFont* FMenu;
#pragma endregion
#pragma region hud_update
template<class T>
static T* FindHudElement(const char* name) {
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan(GetModuleHandleA("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = 
		reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), 
			"55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));

	return (T*)find_hud_element(pThis, name);
}
struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
#pragma endregion
#pragma region radar
Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX,
					int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians) {
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

	*viewCheck = y_1 < 0;

	x_1 *= zoom;
	y_1 *= zoom;

	int sizX = sizeX / 2;
	int sizY = sizeY / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;

	x_1 += posX;
	y_1 += posY;

	return Vector(x_1, y_1, 0);
}

void Menu::DrawRadar() {
	auto visible = Color::Red;
	auto Tvisible = Color::White;

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldPadding = style.WindowPadding;
	float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
	style.WindowPadding = ImVec2(0, 0);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Radar", nullptr, ImVec2(150, 170), 0.7F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddLine(ImVec2(pos.x + siz.x * 0.5f, pos.y), 
			ImVec2(pos.x + siz.x * 0.5f, pos.y + siz.y), ImColor(70, 70, 70, 255), 1.f);

		draw_list->AddLine(ImVec2(pos.x, pos.y + siz.y * 0.5f), 
			ImVec2(pos.x + siz.x, pos.y + siz.y * 0.5f), ImColor(70, 70, 70, 255), 1.f);

		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
			auto pLocalEntity = g_LocalPlayer;
			if (pLocalEntity) {
				Vector LocalPos = pLocalEntity->GetEyePos();
				QAngle ang;
				g_EngineClient->GetViewAngles(&ang);
				for (int i = 1; i < g_EngineClient->GetMaxClients(); i++) {
					C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

					if (!pBaseEntity)
						continue;
					if (pBaseEntity->IsDormant() || !pBaseEntity->IsAlive())
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;

					bool bIsEnemy = pLocalEntity->m_iTeamNum() != pBaseEntity->m_iTeamNum();
					if (!bIsEnemy)
						continue;

					bool viewCheck = false;
					Vector EntityPos = RotatePoint(pBaseEntity->m_vecOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.yaw, 2, &viewCheck, 0.F);

					int s = 4;
					draw_list->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, ImColor(254, 24, 110, 255));
				}
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	style.WindowPadding = oldPadding;
	style.Colors[ImGuiCol_WindowBg].w = oldAlpha;
}
#pragma endregion
#pragma region misctab
void misc_tab() {
	for (int i = 0; i < IM_ARRAYSIZE(tabs_misc); i++) {
		if (ImGui::Tab(tabs_misc[i].c_str(), misctab == i, ImVec2(100, 50))) {
			misctab = i;
		}
		if (i < IM_ARRAYSIZE(tabs_misc) - 1)
			ImGui::SameLine();
	}

	switch (misctab) {
	case 0: {
		ImGui::Columns(2, nullptr, false);
		ImGui::BeginChild("##firstchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Bunny hop", &Cfg.misc_bhop);
			if (Cfg.misc_bhop) {
				ImGui::Checkbox("Auto strafe", &Cfg.misc_autostrafe);
			}
			ImGui::Checkbox("Radar", &Cfg.misc_radar);
			ImGui::Checkbox("Engine Radar", &Cfg.misc_engine_radar);
			ImGui::Checkbox("Rank reveal", &Cfg.misc_showranks);
			ImGui::Checkbox("Auto accept", &Cfg.misc_autoaccept);
			ImGui::Combo("Desync", &Cfg.misc_desync, desync, 3);
			if (Cfg.misc_desync)
				ImGui::Hotkey("Flip bind##legitaa", &Cfg.misc_desync_bind);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##secondchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Clantag", &Cfg.misc_clantag);
			ImGui::Checkbox("Name spam", &Cfg.misc_name_spam);
			ImGui::Checkbox("Chat spam", &Cfg.misc_chat_spam);
			ImGui::Checkbox("Radio spam", &Cfg.misc_radio_spam);
			ImGui::Checkbox("Watermark", &Cfg.misc_watermark);
			ImGui::Checkbox("Left knife", &Cfg.misc_leftknife);
			ImGui::Checkbox("Aspect ratio", &Cfg.aspect_ratio);
			if (Cfg.aspect_ratio) {
				ImGui::SliderInt("##aspect_ration", &Cfg.aspect_ratio_value, 0, 200, "Aspect ratio: %.1f");
			}
			ImGui::Checkbox("Enable agent changer", &Cfg.misc_model);
			if (Cfg.misc_model) {
				ImGui::Combo("Type TT", &Cfg.misc_model_type_tt, agent_types, IM_ARRAYSIZE(agent_types));
				ImGui::Combo("Type CT", &Cfg.misc_model_type_ct, agent_types, IM_ARRAYSIZE(agent_types));
			}
			ImGui::Checkbox("BackDrop", &Cfg.backdrop);
			if (ImGui::BeginCombo("Fps Boost", "...")) {
				std::vector<std::string> vec;
				for (size_t i = 0; i < IM_ARRAYSIZE(fps_boost_vec); i++) {
					ImGui::Selectable(fps_boost_vec[i], &Cfg.fps_boost_selected[i], ImGuiSelectableFlags_DontClosePopups);
					if (Cfg.fps_boost_selected[i])
						vec.push_back(fps_boost_vec[i]);
				}
				ImGui::EndCombo();
			}
		}
		ImGui::EndChild();
	} break;
	case 1: {
		ImGui::Columns(2, nullptr, false);

		static std::string current_config;
		static auto is_configs_loaded = false;

		static char config_name[32];

		ImGui::BeginChild("##firstchild", ImVec2(300, 0)); {
			static std::vector<std::string> configs;

			static auto load_configs = []() {
				std::vector<std::string> items = {};

				std::string path("C:\\white.xyz\\config");
				if (!fs::is_directory(path))
					fs::create_directories(path);

				for (auto& p : fs::directory_iterator(path))
					items.push_back(p.path().string().substr(path.length() + 1));

				return items;
			};

			if (!is_configs_loaded) {
				is_configs_loaded = true;
				configs = load_configs();
			}

			ImGui::ListBoxHeader("##configs", ImVec2(-1, -1)); {
				for (auto& config : configs) {
					if (ImGui::Selectable(config.c_str(), config == current_config)) {
						current_config = config;
					}
				}
			}
			ImGui::ListBoxFooter();

		
	
			ImGui::InputText("Config name##config_name", config_name, sizeof(config_name));

			if (ImGui::Button("Create", ImVec2(-1, 20))) {
				current_config = std::string(config_name);

				CConfig::Get().Save(current_config + ".cfg");
				is_configs_loaded = false;
				memset(config_name, 0, 32);
			}

			if (ImGui::Button("Load", ImVec2(-1, 20))) {
				CConfig::Get().Load(current_config);

				static auto clear_hud_weapon_icon_fn =
					reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
						Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

				auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

				if (!element)
					return;

				auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
				if (hud_weapons == nullptr)
					return;

				if (!*hud_weapons->get_weapon_count())
					return;

				for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
					i = clear_hud_weapon_icon_fn(hud_weapons, i);

				typedef void(*ForceUpdate) (void);
				static ForceUpdate FullUpdate = 
					(ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
				FullUpdate();
			}

			if (ImGui::Button("Save", ImVec2(-1, 20)))
				CConfig::Get().Save(current_config);

			if (ImGui::Button("Delete", ImVec2(-1, 20)) && fs::remove("C:\\white.xyz\\configs\\" + current_config)) {
				current_config.clear();
				is_configs_loaded = false;
			}

			if (ImGui::Button("Refresh", ImVec2(-1, 20)))
				is_configs_loaded = false;
		}
		ImGui::EndChild();
		
	} break;
	}
}
#pragma endregion
#pragma region skinstab
static auto definition_vector_index = 0;
static std::string selected_weapon_name = "";
static std::string selected_skin_name = "";
auto& entries = Cfg.skins.m_items;
void skins_tab() {
	//auto& entries = Cfg.skins.m_items;

	ImGui::BeginGroup(); {
		ImGui::BeginChild("##skins_list", ImVec2(200, 0), false); {

			const auto wnd = ImGui::GetCurrentWindowRead();
			float height = (wnd->Pos.y + wnd->Size.y) - 
				wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::ListBoxHeader("##config", ImVec2(-1, 335)); {
				for (size_t w = 0; w < k_weapon_names.size(); w++) {
					if (ImGui::Selectable(k_weapon_names[w].name, definition_vector_index == w)) {
						definition_vector_index = w;
					}
				}
			}
			ImGui::ListBoxFooter();

			if (ImGui::Button("Update Skins", ImVec2(-1, -1))) {
				static auto clear_hud_weapon_icon_fn =
					reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
						Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

				auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

				if (!element) return;

				auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
				if (hud_weapons == nullptr) return;

				if (!*hud_weapons->get_weapon_count()) return;

				for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
					i = clear_hud_weapon_icon_fn(hud_weapons, i);

				typedef void(*ForceUpdate) (void);
				static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
				FullUpdate();
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup(); {
		ImGui::BeginChild("##skins_2", ImVec2(0, 0), false); {
			auto& satatt = Cfg.skins.statrack_items[k_weapon_names[definition_vector_index].definition_index];
			auto& selected_entry = entries[k_weapon_names[definition_vector_index].definition_index];
			selected_entry.definition_index = k_weapon_names[definition_vector_index].definition_index;
			selected_entry.definition_vector_index = definition_vector_index;
			ImGui::InputInt("Seed", &selected_entry.seed);
			ImGui::SliderFloat("¹¹Wear", &selected_entry.wear, FLT_MIN, 1.f, "Wear: %.10f", 5);
			ImGui::Checkbox("Stat Track##2", &selected_entry.stat_trak);
			if (selected_entry.stat_trak) {
				ImGui::InputInt("Value##stattrack", &satatt.statrack_new.counter);
			}

			if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) {
				ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_knife_names.at(idx).name;
					return true;
					}, nullptr, k_knife_names.size(), 10);
				selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
			}
			else if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) {
				ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_glove_names.at(idx).name;
					return true;
					}, nullptr, k_glove_names.size(), 10);
				selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
			}
			else {
				static auto unused_value = 0;
				selected_entry.definition_override_vector_index = 0;
			}

			static char filter_name[32];
			std::string filter = filter_name;

			bool is_glove = selected_entry.definition_index == GLOVE_T_SIDE ||
				selected_entry.definition_index == GLOVE_CT_SIDE;

			bool is_knife = selected_entry.definition_index == WEAPON_KNIFE ||
				selected_entry.definition_index == WEAPON_KNIFE_T;

			int cur_weapidx = 0;
			if (!is_glove && !is_knife) {
				cur_weapidx = k_weapon_names[definition_vector_index].definition_index;
			}
			else {
				if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) {
					cur_weapidx = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
				}
				if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) {
					cur_weapidx = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
				}
			}

			ImGui::InputText("Skin Filter##skins", filter_name, sizeof(filter_name));

			auto weaponName = weaponnames(cur_weapidx);

			ImGui::ListBoxHeader("##skinssdsdadsdadas", ImVec2(-1, -1)); {
				if (selected_entry.definition_index != GLOVE_T_SIDE && selected_entry.definition_index != GLOVE_CT_SIDE) {
					if (ImGui::Selectable("None", selected_entry.paint_kit_index == -1)) {
						selected_entry.paint_kit_vector_index = -1;
						selected_entry.paint_kit_index = -1;
						selected_skin_name = "";
					}

					int lastID = ImGui::GetItemID();
					for (size_t w = 0; w < k_skins.size(); w++) {
						for (auto names : k_skins[w].weaponName) {
							std::string name = k_skins[w].name;

							if (Cfg.skins.show_cur) {
								if (names != weaponName)
									continue;
							}

							if (name.find(filter) != name.npos) {
								ImGui::PushID(lastID++);

								ImGui::PushStyleColor(ImGuiCol_Text, Skins::Get().GetColorRatiry(is_knife ? 6 : k_skins[w].rarity)); {
									if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w)) {
										selected_entry.paint_kit_vector_index = w;
										selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
										selected_skin_name = k_skins[w].name_short;
									}
								}
								ImGui::PopStyleColor();

								ImGui::PopID();
							}
						}
					}
				}
				else {
					int lastID = ImGui::GetItemID();

					if (ImGui::Selectable("None", selected_entry.paint_kit_index == -1)) {
						selected_entry.paint_kit_vector_index = -1;
						selected_entry.paint_kit_index = -1;
						selected_skin_name = "";
					}

					for (size_t w = 0; w < k_gloves.size(); w++) {
						for (auto names : k_gloves[w].weaponName) {
							std::string name = k_gloves[w].name;

							if (Cfg.skins.show_cur) {
								if (names != weaponName)
									continue;
							}

							if (name.find(filter) != name.npos) {
								ImGui::PushID(lastID++);
								ImGui::PushStyleColor(ImGuiCol_Text, Skins::Get().GetColorRatiry(6)); {
									if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w)) {
										selected_entry.paint_kit_vector_index = w;
										selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
										selected_skin_name = k_gloves[selected_entry.paint_kit_vector_index].name_short;
									}
								}
								ImGui::PopStyleColor();

								ImGui::PopID();
							}
						}
					}
				}
				ImGui::ListBoxFooter();
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndGroup();
}
#pragma endregion
#pragma region visualstab
void visuals_tab() {
	for (int i = 0; i < IM_ARRAYSIZE(tabs_esp); i++) {
		if (ImGui::Tab(tabs_esp[i].c_str(), visualtab == i, ImVec2(100, 50))) {
			visualtab = i;
		}
		if (i < (IM_ARRAYSIZE(tabs_esp) - 1))
			ImGui::SameLine();
	}

	switch (visualtab) {
	case 0: {
		ImGui::Columns(2, nullptr, false);
		ImGui::BeginChild("##firstchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Enabled", &Cfg.esp_enabled);
			ImGui::Checkbox("Enemy Only", &Cfg.esp_enemies_only);
			ImGui::Checkbox("Visible Only", &Cfg.esp_visible_only);
			ImGui::Checkbox("Box", &Cfg.esp_player_boxes);
			if (Cfg.esp_player_boxes) {
				ImGui::Combo("Type", &Cfg.esp_player_boxes_type, Box_type, IM_ARRAYSIZE(Box_type), 3);
				if (Cfg.esp_player_boxes_type == 1) 
					ImGui::SliderInt("##Roundingboxes", &Cfg.boxes_rounding, 0, 15, "Rounding: %.1f");		
			}
			ImGui::Checkbox("Name", &Cfg.esp_player_names);
			ImGui::Checkbox("Health", &Cfg.esp_player_health);
			ImGui::Checkbox("Skeleton", &Cfg.esp_player_skeleton);
			ImGui::Checkbox("Weapon", &Cfg.esp_player_weapons);
			ImGui::Checkbox("Flags", &Cfg.esp_flags);
			if (Cfg.esp_flags) {
				if (ImGui::BeginCombo("Flags##combo", "Select")) {
					std::vector<std::string> vec;
					for (size_t i = 0; i < IM_ARRAYSIZE(esp_flags_vec); i++) {
						ImGui::Selectable(esp_flags_vec[i], &Cfg.esp_player_flags_selected[i], ImGuiSelectableFlags_DontClosePopups);
						if (Cfg.esp_player_flags_selected[i])
							vec.push_back(esp_flags_vec[i]);
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Checkbox("Grenade Prediction", &Cfg.esp_grenade_prediction);
			ImGui::Checkbox("Sound ESP", &Cfg.esp_sounds);
			if (Cfg.esp_sounds) {
				ImGui::SliderFloat("##Duration", &Cfg.esp_sounds_time, 0.5f, 3.0f, "Duration: %.3f ms");
				ImGui::SliderFloat("##Radius", &Cfg.esp_sounds_radius, 15.0f, 150.0f, "Radius: %.1f");
			}
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##secondchild", ImVec2(0, 0)); {
			ImGui::SliderInt("##Override FOV", &Cfg.misc_override_fov, 90, 150, "Override FOV: %.1f");
			ImGui::SliderInt("##Viewmodel FOV", &Cfg.misc_viewmodel_fov, 68, 120, "Viewmodel FOV: %.1f");
			ImGui::Checkbox("HitMarker", &Cfg.esp_hitmarker);
			ImGui::Checkbox("Crosshair", &Cfg.esp_crosshair);
			ImGui::Checkbox("Sniper Crosshair", &Cfg.esp_snipercrosshair);
			ImGui::Checkbox("Recoil Crosshair", &Cfg.esp_recoilcrosshair);
			ImGui::Checkbox("Draw FOV", &Cfg.other_drawfov);
			ImGui::Checkbox("Spectator list", &Cfg.misc_spectatorlist);
			ImGui::Checkbox("AA Lines", &Cfg.esp_angle_lines);
			ImGui::Checkbox("Third Person", &Cfg.misc_thirdperson);

			if (Cfg.misc_thirdperson) {
				ImGui::SliderFloat("Dist", &Cfg.misc_thirdperson_dist, 25.f, 250.f);
				ImGui::Hotkey("Bind", &Cfg.misc_thirdperson_bind);
			}

			if (ImGui::Checkbox(("Nightmode"), &Cfg.misc_nightmode)) 
				Misc::Get().NightMode();
			
			ImGui::Checkbox("No Smoke", &Cfg.other_no_smoke);
			ImGui::Checkbox("No Flash", &Cfg.other_no_flash);
			ImGui::Checkbox("No Hands", &Cfg.other_no_hands);
		}
		ImGui::EndChild();
	} break;
	case 1: {
		ImGui::Columns(2, nullptr, false);
		ImGui::BeginChild("##firstchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Enabled", &Cfg.glow_enabled);
			ImGui::Checkbox("Enemy Only", &Cfg.glow_enemies_only);
			ImGui::Checkbox("Visible Only", &Cfg.glow_visible_only);
			ImGui::Combo("Glow Type", &Cfg.glow_style, glow_style, ARRAYSIZE(glow_style));
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##secondchild", ImVec2(0, 0)); {
			//to-do?
		}
		ImGui::EndChild();
	} break;
	case 2: {
		ImGui::Columns(3, nullptr, false);
		ImGui::BeginChild("##firstchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Enabled Chams", &Cfg.chams_player_enabled);
			ImGui::Checkbox("Enemy Only", &Cfg.chams_player_enemies_only);
			ImGui::Combo("Material##player", &Cfg.chams_material, chams_material, IM_ARRAYSIZE(chams_material));
			ImGui::Checkbox("Wireframe", &Cfg.chams_player_wireframe);
			ImGui::Checkbox("Ignore-Z", &Cfg.chams_player_ignorez);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##secondchild", ImVec2(0, 0)); {
			ImGui::Text("Backtrack chams");
			ImGui::Combo("##Tick", &Cfg.chams_backtrack, chams_tick, IM_ARRAYSIZE(chams_tick), 3);
			ImGui::Combo("Material##bt", &Cfg.chams_material_backtrack, chams_material, IM_ARRAYSIZE(chams_material));
			ImGui::Checkbox("Wireframe", &Cfg.chams_backtrack_wireframe);
			ImGui::Checkbox("Rainbow##backtrack", &Cfg.chams_backtrack_rainbow);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##thirdchild", ImVec2(0, 0)); {
			ImGui::Checkbox("Enabled Arms", &Cfg.chams_arms_enabled);
			if (Cfg.chams_arms_enabled) {
				ImGui::Combo("Material##arms", &Cfg.chams_material_arms, chams_material, IM_ARRAYSIZE(chams_material));
				ImGui::Checkbox("Rainbow##arms", &Cfg.chams_arms_rainbow);
			}
			ImGui::Checkbox("Enabled Weapon", &Cfg.chams_weapon_enabled);
			if (Cfg.chams_weapon_enabled) {
				ImGui::Combo("Material##weapon", &Cfg.chams_material_weapon, chams_material, IM_ARRAYSIZE(chams_material));
				ImGui::Checkbox("Rainbow##weapon", &Cfg.chams_weapon_rainbow);
			}
			ImGui::Checkbox("Enabled Sleeve", &Cfg.chams_sleeve_enabled);
			if (Cfg.chams_sleeve_enabled) {
				ImGui::Combo("Material##sleeve", &Cfg.chams_material_sleeve, chams_material, IM_ARRAYSIZE(chams_material));
				ImGui::Checkbox("Rainbow##sleeve", &Cfg.chams_sleeve_rainbow);
			}		
		}
		ImGui::EndChild();
	} break;
	case 3: {
		ImGui::Columns(4, nullptr, false);
		ImGui::BeginChild("##firstchild", ImVec2(0, 0)); {
			ImGui::Text("Esp");
			
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##secondchild", ImVec2(0, 0)); {
			ImGui::Text("Glow");
			ImGui::ColorEdit4("Team##glowteamcolor", Cfg.color_glow_ally, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Enemy##glowenemycolor", Cfg.color_glow_enemy, ImGuiColorEditFlags_NoInputs);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##thirdchild", ImVec2(0, 0)); {
			ImGui::Text("Chams");
			ImGui::ColorEdit4("Team Visible##chamsteam1", Cfg.color_chams_player_ally_visible, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Team Invisible##chamsteam2", Cfg.color_chams_player_ally_occluded, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Enemy Visible##chamsenemy1", Cfg.color_chams_player_enemy_visible, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Enemy Invisible##chamsenemy2", Cfg.color_chams_player_enemy_occluded, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Backtrack", Cfg.color_chams_backtrack, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Arm Chams", Cfg.color_chams_arms, ImGuiColorEditFlags_NoInputs);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##fourchild", ImVec2(0, 0)); {
			ImGui::Text("Misc");
			ImGui::ColorEdit4("HitMarker", Cfg.color_esp_hitmarker, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Crosshair", Cfg.color_esp_crosshair, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Menu color", Cfg.color_menu, ImGuiColorEditFlags_NoInputs);
		}
		ImGui::EndChild();
	} break;
	}
}
#pragma endregion
#pragma region legitstab
static int weapon_index = 7;
static int weapon_vector_index = 0;
struct WeaponName_t {
	constexpr WeaponName_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};

std::vector< WeaponName_t> WeaponNames = {
	{61, "Usp-s"},
	{32, "P2000"},
	{4, "Glock-18"},
	{2, "Dual berettas"},
	{36, "P250"},
	{3, "Five-Seven"},
	{30, "Tec-9"},
	{63, "Cz75a"},
	{64, "R8 revolver"},
	{1, "Deagle"},

	{34, "Mp9"},
	{17, "Mac-10"},
	{23, "Mp5-sd"},
	{33, "Mp7"},
	{24, "Ump-45"},
	{19, "P90"},
	{26, "PP-Bizon"},

	{7, "Ak-47"},
	{60, "M4a1-s"},
	{16, "M4a4"},
	{8, "Aug"},
	{39, "Sg553"},
	{10, "Famas"},
	{13, "Galil"},

	{40, "Ssg08"},
	{38, "Scar-20"},
	{9, "Awp"},
	{11, "G3sg1"},

	{14, "M249"},
	{28, "Negev"},

	{27, "Mag-7"},
	{35, "Nova"},
	{29, "Sawed-off"},
	{25, "Xm1014"},
};

void RenderCurrentWeaponButton() {
	if (!g_EngineClient->IsConnected() || !g_LocalPlayer || !g_LocalPlayer->IsAlive()) return;
	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon) return;
	short wpn_idx = weapon->m_Item().m_iItemDefinitionIndex();
	auto wpn_it = std::find_if(WeaponNames.begin(), WeaponNames.end(), [wpn_idx](const WeaponName_t& a) {
		return a.definition_index == wpn_idx;
		});
	if (wpn_it != WeaponNames.end()) {
		weapon_index = wpn_idx;
		weapon_vector_index = std::abs(std::distance(WeaponNames.begin(), wpn_it));
	}
}

void legit_tab() {
	auto settings = &Cfg.legitbot_items[weapon_index];
	
	ImGui::BeginGroup(); {
		ImGui::BeginChild("##aimbot.general", ImVec2(150, 0)); {
			ImGui::PushItemWidth(-1);
			if (ImGui::Combo("##Weapon", &weapon_vector_index, [](void* data, int32_t idx, const char** out_text) 
				{auto vec = reinterpret_cast<std::vector< WeaponName_t >*>(data);	*out_text = vec->at(idx).name; return true; }, 
				(void*)(&WeaponNames), WeaponNames.size())) {
				weapon_index = WeaponNames[weapon_vector_index].definition_index;
			}
			RenderCurrentWeaponButton();
			ImGui::Separator();
			ImGui::Checkbox("Enabled", &settings->enabled);
			ImGui::Checkbox("On Key", &settings->on_key);
			if (settings->on_key) {
				ImGui::SameLine();
				ImGui::Hotkey("##Key bind", &settings->key);
			}
			
			ImGui::Checkbox("Deathmatch", &settings->deathmatch);
			if (weapon_index == WEAPON_P250 || weapon_index == WEAPON_USPS || weapon_index == WEAPON_GLOCK ||
				weapon_index == WEAPON_FIVESEVEN || weapon_index == WEAPON_TEC9 || weapon_index == WEAPON_DEAGLE ||
				weapon_index == WEAPON_ELITE || weapon_index == WEAPON_P2000) {
				ImGui::Checkbox("Autopistol", &settings->autopistol);
			}

			ImGui::Checkbox("Ignore Wall", &settings->autowall);

			ImGui::Checkbox("Silent", &settings->silent);
			if (weapon_index == WEAPON_AWP || weapon_index == WEAPON_SSG08 ||
				weapon_index == WEAPON_G3SG1 || weapon_index == WEAPON_SCAR20) {
				ImGui::Checkbox("Only In Zoom", &settings->only_in_zoom);
			}

			ImGui::Checkbox("Auto Fire", &settings->autofire);
			if (settings->autofire) {
				ImGui::SameLine();
				ImGui::Hotkey("##Auto Fire bind", &settings->autofire_key);
			}

			if (settings->autofire) 
				ImGui::Checkbox("Auto Stop", &settings->autostop);
			
			ImGui::Separator();
			ImGui::Text("Misc:");
			ImGui::Checkbox("Smoke check", &settings->smoke_check);
			ImGui::Checkbox("Flash check", &settings->flash_check);
			ImGui::Checkbox("Jump check", &settings->jump_check);
			ImGui::SliderFloat("##aimbot_backtrack_time", &settings->backtrack_time, 0.f, 0.2f, "Backtrack: %.3f");
			ImGui::SliderInt("##aimbot.shot_delay", &settings->shot_delay, 0, 100, "Shot Delay: %.0f");
			ImGui::SliderInt("##aimbot.kill_delay", &settings->kill_delay, 0, 1000, "Kill Delay: %.0f");
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup(); {
		ImGui::BeginChild(("##tabs"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar); 
		{
			ImGui::PushItemWidth(-1);

			ImGui::Text("Aim Type:");
			ImGui::Combo("##aimbot.aim_type", &settings->aim_type, aim_types, IM_ARRAYSIZE(aim_types));

			if (settings->aim_type == 0) {
				ImGui::Text("Hitbox:");
				ImGui::Combo("##aimbot.hitbox", &settings->hitbox, hitbox_list, IM_ARRAYSIZE(hitbox_list));
			}

			ImGui::Text("Priority:");
			ImGui::Combo("##aimbot.priority", &settings->priority, priorities, IM_ARRAYSIZE(priorities));

			ImGui::SliderFloat("##aimbot.fov", &settings->fov, 0, 30, "Fov: %.2f");

			if (settings->rcs_fov_enabled) 
				ImGui::SliderFloat("##aimbot.rcs_fov", &settings->rcs_fov, 0, 20, "RCS Fov: %.2f");
			
			if (settings->silent) 
				ImGui::SliderFloat("##aimbot.silent_fov", &settings->silent_fov, 0, 20, "Fov Silent: %.2f");
			
			ImGui::SliderFloat("##aimbot.smooth", &settings->smooth, 1, 20, "Smooth: %.2f");

			if (settings->rcs_smooth_enabled) 
				ImGui::SliderFloat("##aimbot.rcs_smooth", &settings->rcs_smooth, 1, 15, "RCS Smooth: %.2f");
			
			if (settings->autowall) 
				ImGui::SliderInt("##aimbot.min_damage", &settings->min_damage, 1, 100, "Min Damage: %.0f");		

			ImGui::Checkbox("Enabled RCS##aimbot.rcs", &settings->rcs);

			ImGui::Combo("##aimbot.rcs_type", &settings->rcs_type, rcs_types, IM_ARRAYSIZE(rcs_types));

			ImGui::SliderInt("##aimbot.rcs_start", &settings->rcs_start, 1, 30, "RCS After: %.0f shots");
			ImGui::SliderInt("##aimbot.rcs_x", &settings->rcs_x, 0, 100, "RCS X: %.0f");
			ImGui::SliderInt("##aimbot.rcs_y", &settings->rcs_y, 0, 100, "RCS Y: %.0f");

			ImGui::Checkbox("RCS Custom Fov", &settings->rcs_fov_enabled);
			ImGui::Checkbox("RCS Custom Smooth", &settings->rcs_smooth_enabled);
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}
	ImGui::EndGroup();
}
#pragma endregion
//--------------------------------------------------------------------------------
void Menu::Initialize() {
	ImGui::StyleColosClassic();

	MainCaps = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuil.ttf", 24);
	Main = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16);
	FMenu = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 12);

	_opened = false;
}
//--------------------------------------------------------------------------------
void Menu::Shutdown() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
//--------------------------------------------------------------------------------
void Menu::OnDeviceLost() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
}
//--------------------------------------------------------------------------------
void Menu::OnDeviceReset() {
	ImGui_ImplDX9_CreateDeviceObjects();
}
//--------------------------------------------------------------------------------

void Menu::Render() {
	if (Cfg.misc_spectatorlist) Visuals::Get().RenderSpectatorList();

	ImGui::StyleColosClassic();

	ImVec2 p;
	ImDrawList* draw;

	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize({ 545.000000f,470.000000f });

	if (!_opened) return;

	ImGui::Begin("Edited", &_opened, flags); 
	{
		p = ImGui::GetWindowPos();
		draw = ImGui::GetWindowDrawList();

		draw->AddRectFilled(ImVec2(p.x + 0, p.y + 0), ImVec2(p.x + 545, p.y + 12), ImColor(249, 165, 22, 255), 6, 15);
		//ImGui::AddCircleImageFilled(background, ImGui::GetWindowPos() + ImGui::GetWindowSize() - ImVec2(30, 30), 30.f, ImColor(255, 255, 255), 360);
		draw->AddRectFilled(ImVec2(p.x + 0, p.y + 460), ImVec2(p.x + 545, p.y + 470), ImColor(33, 33, 38, 255), 12, 15);
		draw->AddRectFilled(ImVec2(p.x + 0, p.y + 7), ImVec2(p.x + 545, p.y + 464), ImColor(33, 33, 38, 255), 0, 15);
		ImGui::PushFont(MainCaps);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SameLine(0.0f, 30.0f);
		ImGui::Text("WHITE.XYZ", ImVec2(p.x + 140, p.y + 26), ImColor(255, 255, 255, 255));
		ImGui::PopFont();
		draw->AddLine(ImVec2(p.x + 0, p.y + 72), ImVec2(p.x + 545, p.y + 72), ImColor(89, 87, 93, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 0, p.y + 71), ImVec2(p.x + 545, p.y + 71), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 0, p.y + 73), ImVec2(p.x + 545, p.y + 73), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 140, p.y + 72), ImVec2(p.x + 140, p.y + 470), ImColor(79, 77, 82, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 192, p.y + 72), ImVec2(p.x + 192, p.y + 470), ImColor(79, 77, 82, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 191, p.y + 73), ImVec2(p.x + 191, p.y + 470), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 193, p.y + 73), ImVec2(p.x + 193, p.y + 470), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 139, p.y + 73), ImVec2(p.x + 139, p.y + 470), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddLine(ImVec2(p.x + 141, p.y + 73), ImVec2(p.x + 141, p.y + 470), ImColor(22, 21, 26, 255), 1.000000);
		draw->AddRectFilled(ImVec2(p.x + 142, p.y + 73), ImVec2(p.x + 191, p.y + 470), ImColor(22, 21, 26, 255), 0, 15);

		static bool bools[128];
		static int ints[128];
		static float floats[128];
		static int selectedtab = 0;
		static int selectedsubtab = 0;
		static int selectedcategory = 0;

		ImGui::SetCursorPos({ (float)140, (float)26 });
		ImGui::BeginGroup();
		{

			if (ImGui::tab("LegitBot##Tab", "", selectedtab == 0))
				selectedtab = 0;
			ImGui::SameLine();
			if (ImGui::tab("Visuals##Tab", "", selectedtab == 1))
				selectedtab = 1;
			ImGui::SameLine();
			if (ImGui::tab("Misc##Tab", "", selectedtab == 2))
				selectedtab = 2;
			ImGui::SameLine();
			if (ImGui::tab("Skins##Tab", "", selectedtab == 3))
				selectedtab = 3;
		}
		ImGui::EndGroup();

		if (selectedtab == 0)
		{
			auto settings = &Cfg.legitbot_items[weapon_index];

			ImGui::SetCursorPos({ (float)141, (float)73 });
			ImGui::BeginGroup();
			{
				//São as Catergorias do Meio
				if (selectedsubtab == 0) {
					if (ImGui::category(("Aim"), selectedcategory == 0))
						selectedcategory = 0;
					if (ImGui::category(("Misc##aimbot"), selectedcategory == 1))
						selectedcategory = 1;
				}
		
			}
			ImGui::EndGroup();

			//São as SubTabs Ao Lado Esquerdo
			ImGui::SetCursorPos({ (float)27, (float)93 });
			ImGui::BeginGroup();
			{
				if (ImGui::subtab(("General"), selectedsubtab == 0))
					selectedsubtab = 0;
			}
			ImGui::EndGroup();

			//Esse são os itens dentro de cada tab
			ImGui::SetCursorPos({ (float)217, (float)96 });
			ImGui::BeginGroup();
			{
				if (selectedsubtab == 0)
				{
					if (selectedcategory == 0)
					{
						ImGui::BeginChild(("##aimbot_legit"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
						{
							if (ImGui::Combo("##Weapon", &weapon_vector_index, [](void* data, int32_t idx, const char** out_text)
								{auto vec = reinterpret_cast<std::vector< WeaponName_t >*>(data);	*out_text = vec->at(idx).name; return true; },
								(void*)(&WeaponNames), WeaponNames.size())) {
								weapon_index = WeaponNames[weapon_vector_index].definition_index;
							}
							RenderCurrentWeaponButton();
							ImGui::Spacing();
							ImGui::Checkbox("Enabled", &settings->enabled);
							ImGui::Checkbox("On Key", &settings->on_key);
							if (settings->on_key) {
								ImGui::SameLine();
								ImGui::Hotkey("##Key bind", &settings->key);
							}

							ImGui::Checkbox("Deathmatch", &settings->deathmatch);
							if (weapon_index == WEAPON_P250 || weapon_index == WEAPON_USPS || weapon_index == WEAPON_GLOCK ||
								weapon_index == WEAPON_FIVESEVEN || weapon_index == WEAPON_TEC9 || weapon_index == WEAPON_DEAGLE ||
								weapon_index == WEAPON_ELITE || weapon_index == WEAPON_P2000) {
								ImGui::Checkbox("Autopistol", &settings->autopistol);
							}
							ImGui::Checkbox("Ignore Wall", &settings->autowall);
							ImGui::Checkbox("Silent", &settings->silent);
							if (weapon_index == WEAPON_AWP || weapon_index == WEAPON_SSG08 ||
								weapon_index == WEAPON_G3SG1 || weapon_index == WEAPON_SCAR20) {
								ImGui::Checkbox("Only In Zoom", &settings->only_in_zoom);
							}
							ImGui::Checkbox("Auto Fire", &settings->autofire);
							if (settings->autofire) {
								ImGui::SameLine();
								ImGui::Hotkey("##Auto Fire bind", &settings->autofire_key);
							}
							if (settings->autofire)
								ImGui::Checkbox("Auto Stop", &settings->autostop);
							ImGui::Spacing();
							ImGui::Text("Misc:");
							ImGui::Checkbox("Smoke check", &settings->smoke_check);
							ImGui::Checkbox("Flash check", &settings->flash_check);
							ImGui::Checkbox("Jump check", &settings->jump_check);
							ImGui::SliderFloat("##aimbot_backtrack_time", &settings->backtrack_time, 0.f, 0.2f, "Backtrack: %.3f");
							ImGui::SliderInt("##aimbot.shot_delay", &settings->shot_delay, 0, 100, "Shot Delay: %.0f");
							ImGui::SliderInt("##aimbot.kill_delay", &settings->kill_delay, 0, 1000, "Kill Delay: %.0f");
						}
						ImGui::EndChild();
					}
					else if (selectedcategory == 1)
					{
						ImGui::BeginChild(("##aimbot_misc"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
						{
							//ImGui::PushItemWidth(-1);

							ImGui::Text("Aim Type:");
							ImGui::Combo("##aimbot.aim_type", &settings->aim_type, aim_types, IM_ARRAYSIZE(aim_types));

							if (settings->aim_type == 0) {
								ImGui::Text("Hitbox:");
								ImGui::Combo("##aimbot.hitbox", &settings->hitbox, hitbox_list, IM_ARRAYSIZE(hitbox_list));
							}

							ImGui::Text("Priority:");
							ImGui::Combo("##aimbot.priority", &settings->priority, priorities, IM_ARRAYSIZE(priorities));

							ImGui::SliderFloat("##aimbot.fov", &settings->fov, 0, 30, "Fov: %.2f");

							if (settings->rcs_fov_enabled)
								ImGui::SliderFloat("##aimbot.rcs_fov", &settings->rcs_fov, 0, 20, "RCS Fov: %.2f");

							if (settings->silent)
								ImGui::SliderFloat("##aimbot.silent_fov", &settings->silent_fov, 0, 20, "Fov Silent: %.2f");

							ImGui::SliderFloat("##aimbot.smooth", &settings->smooth, 1, 20, "Smooth: %.2f");

							if (settings->rcs_smooth_enabled)
								ImGui::SliderFloat("##aimbot.rcs_smooth", &settings->rcs_smooth, 1, 15, "RCS Smooth: %.2f");

							if (settings->autowall)
								ImGui::SliderInt("##aimbot.min_damage", &settings->min_damage, 1, 100, "Min Damage: %.0f");

							ImGui::Checkbox("Enabled RCS##aimbot.rcs", &settings->rcs);

							ImGui::Combo("##aimbot.rcs_type", &settings->rcs_type, rcs_types, IM_ARRAYSIZE(rcs_types));

							ImGui::SliderInt("##aimbot.rcs_start", &settings->rcs_start, 1, 30, "RCS After: %.0f shots");
							ImGui::SliderInt("##aimbot.rcs_x", &settings->rcs_x, 0, 100, "RCS X: %.0f");
							ImGui::SliderInt("##aimbot.rcs_y", &settings->rcs_y, 0, 100, "RCS Y: %.0f");

							ImGui::Checkbox("RCS Custom Fov", &settings->rcs_fov_enabled);
							ImGui::Checkbox("RCS Custom Smooth", &settings->rcs_smooth_enabled);
							//ImGui::PopItemWidth();
						}
						ImGui::EndChild();
					}
				}
			}
			ImGui::EndGroup();
		}
		else if (selectedtab == 1)
		{
		ImGui::SetCursorPos({ (float)141, (float)73 });
		ImGui::BeginGroup();
		{
			//São as Catergorias do Meio
			if (selectedsubtab == 0) {
				if (ImGui::category(("ESP"), selectedcategory == 0))
					selectedcategory = 0;
				if (ImGui::category(("Misc##esp"), selectedcategory == 1))
					selectedcategory = 1;
				if (ImGui::category(("Glow##esp"), selectedcategory == 2))
					selectedcategory = 2;
			}
			else if (selectedsubtab == 1) {
				if (ImGui::category(("Chams##categoryChams"), selectedcategory == 0))
					selectedcategory = 0;
				if (ImGui::category(("Arms##armscategoryChams"), selectedcategory == 1))
					selectedcategory = 1;
			}
			else if (selectedsubtab == 2) {
				if (ImGui::category(("ESP##um"), selectedcategory == 0))
					selectedcategory = 0;
				if (ImGui::category(("Chams##dois"), selectedcategory == 1))
					selectedcategory = 1;
			}

		}
		ImGui::EndGroup();

		//São as SubTabs Ao Lado Esquerdo
		ImGui::SetCursorPos({ (float)27, (float)93 });
		ImGui::BeginGroup();
		{
			if (ImGui::subtab(("Visuals"), selectedsubtab == 0))
				selectedsubtab = 0;
			if (ImGui::subtab(("Chams"), selectedsubtab == 1))
				selectedsubtab = 1;
			if (ImGui::subtab(("Colors"), selectedsubtab == 2))
				selectedsubtab = 2;
		}
		ImGui::EndGroup();

		//Esse são os itens dentro de cada tab
		ImGui::SetCursorPos({ (float)217, (float)96 });
		ImGui::BeginGroup();
		{
			if (selectedsubtab == 0)
			{
				if (selectedcategory == 0)
				{
					ImGui::BeginChild(("##visualstab"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Checkbox("Enabled", &Cfg.esp_enabled);
						ImGui::Checkbox("Enemy Only", &Cfg.esp_enemies_only);
						ImGui::Checkbox("Visible Only", &Cfg.esp_visible_only);
						ImGui::Checkbox("Box", &Cfg.esp_player_boxes);
						if (Cfg.esp_player_boxes) {
							ImGui::Combo("Type", &Cfg.esp_player_boxes_type, Box_type, IM_ARRAYSIZE(Box_type), 3);
							if (Cfg.esp_player_boxes_type == 1)
								ImGui::SliderInt("##Roundingboxes", &Cfg.boxes_rounding, 0, 15, "Rounding: %.1f");
						}
						ImGui::Checkbox("Name", &Cfg.esp_player_names);
						ImGui::Checkbox("Health", &Cfg.esp_player_health);
						ImGui::Checkbox("Skeleton", &Cfg.esp_player_skeleton);
						ImGui::Checkbox("Weapon", &Cfg.esp_player_weapons);
						ImGui::Checkbox("Flags", &Cfg.esp_flags);
						if (Cfg.esp_flags) {
							if (ImGui::BeginCombo("Flags##combo", "Select")) {
								std::vector<std::string> vec;
								for (size_t i = 0; i < IM_ARRAYSIZE(esp_flags_vec); i++) {
									ImGui::Selectable(esp_flags_vec[i], &Cfg.esp_player_flags_selected[i], ImGuiSelectableFlags_DontClosePopups);
									if (Cfg.esp_player_flags_selected[i])
										vec.push_back(esp_flags_vec[i]);
								}
								ImGui::EndCombo();
							}
						}
						ImGui::Checkbox("Grenade Prediction", &Cfg.esp_grenade_prediction);
						ImGui::Checkbox("Sound ESP", &Cfg.esp_sounds);
						if (Cfg.esp_sounds) {
							ImGui::SliderFloat("##Duration", &Cfg.esp_sounds_time, 0.5f, 3.0f, "Duration: %.3f ms");
							ImGui::SliderFloat("##Radius", &Cfg.esp_sounds_radius, 15.0f, 150.0f, "Radius: %.1f");
						}
					}
					ImGui::EndChild();
				}
				else if (selectedcategory == 1)
				{
					ImGui::BeginChild(("##visualsmisc"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::SliderInt("##Override FOV", &Cfg.misc_override_fov, 90, 150, "Override FOV: %.1f");
						ImGui::SliderInt("##Viewmodel FOV", &Cfg.misc_viewmodel_fov, 68, 120, "Viewmodel FOV: %.1f");
						ImGui::Checkbox("HitMarker", &Cfg.esp_hitmarker);
						ImGui::Checkbox("Crosshair", &Cfg.esp_crosshair);
						ImGui::Checkbox("Sniper Crosshair", &Cfg.esp_snipercrosshair);
						ImGui::Checkbox("Recoil Crosshair", &Cfg.esp_recoilcrosshair);
						ImGui::Checkbox("Draw FOV", &Cfg.other_drawfov);
						ImGui::Checkbox("Spectator list", &Cfg.misc_spectatorlist);
						ImGui::Checkbox("AA Lines", &Cfg.esp_angle_lines);
						ImGui::Checkbox("Third Person", &Cfg.misc_thirdperson);

						if (Cfg.misc_thirdperson) {
							ImGui::SliderFloat("Dist", &Cfg.misc_thirdperson_dist, 25.f, 250.f);
							ImGui::Hotkey("Bind", &Cfg.misc_thirdperson_bind);
						}

						if (ImGui::Checkbox(("Nightmode"), &Cfg.misc_nightmode))
							Misc::Get().NightMode();

						ImGui::Checkbox("No Smoke", &Cfg.other_no_smoke);
						ImGui::Checkbox("No Flash", &Cfg.other_no_flash);
						ImGui::Checkbox("No Hands", &Cfg.other_no_hands);
					}
					ImGui::EndChild();
				}
				else if (selectedcategory == 2)
				{
					ImGui::BeginChild(("Glow##visuals"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Checkbox("Enabled", &Cfg.glow_enabled);
						ImGui::Checkbox("Enemy Only", &Cfg.glow_enemies_only);
						ImGui::Checkbox("Visible Only", &Cfg.glow_visible_only);
						ImGui::Combo("Glow Type", &Cfg.glow_style, glow_style, ARRAYSIZE(glow_style));
					}
					ImGui::EndChild();
				}
			}
			else if (selectedsubtab == 1)
			{
				if (selectedcategory == 0)
				{
					ImGui::BeginChild(("Chams##visuals"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Checkbox("Enabled Chams", &Cfg.chams_player_enabled);
						ImGui::Checkbox("Enemy Only", &Cfg.chams_player_enemies_only);
						ImGui::Combo("Material##player", &Cfg.chams_material, chams_material, IM_ARRAYSIZE(chams_material));
						ImGui::Checkbox("Wireframe", &Cfg.chams_player_wireframe);
						ImGui::Checkbox("Ignore-Z", &Cfg.chams_player_ignorez);
						/*ImGui::Text("Backtrack chams");
						ImGui::Combo("##Tick", &Cfg.chams_backtrack, chams_tick, IM_ARRAYSIZE(chams_tick), 3);
						ImGui::Combo("Material##bt", &Cfg.chams_material_backtrack, chams_material, IM_ARRAYSIZE(chams_material));
						ImGui::Checkbox("Wireframe", &Cfg.chams_backtrack_wireframe);
						ImGui::Checkbox("Rainbow##backtrack", &Cfg.chams_backtrack_rainbow);*/
					}
					ImGui::EndChild();
				}
				else if (selectedcategory == 1)
				{
					ImGui::BeginChild(("Arms##visuals"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Checkbox("Enabled Arms", &Cfg.chams_arms_enabled);
						if (Cfg.chams_arms_enabled) {
							ImGui::Combo("Material##arms", &Cfg.chams_material_arms, chams_material, IM_ARRAYSIZE(chams_material));
							ImGui::Checkbox("Rainbow##arms", &Cfg.chams_arms_rainbow);
						}
						ImGui::Checkbox("Enabled Weapon", &Cfg.chams_weapon_enabled);
						if (Cfg.chams_weapon_enabled) {
							ImGui::Combo("Material##weapon", &Cfg.chams_material_weapon, chams_material, IM_ARRAYSIZE(chams_material));
							ImGui::Checkbox("Rainbow##weapon", &Cfg.chams_weapon_rainbow);
						}
						ImGui::Checkbox("Enabled Sleeve", &Cfg.chams_sleeve_enabled);
						if (Cfg.chams_sleeve_enabled) {
							ImGui::Combo("Material##sleeve", &Cfg.chams_material_sleeve, chams_material, IM_ARRAYSIZE(chams_material));
							ImGui::Checkbox("Rainbow##sleeve", &Cfg.chams_sleeve_rainbow);
						}
					}
					ImGui::EndChild();
				}
			}
			else if (selectedsubtab == 2)
			{
				if (selectedcategory == 0)
				{
					ImGui::BeginChild(("ESP##vls"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Text("ESP COLORS:");
						ImGui::ColorEdit4("Team Visible##espteamcolor1", Cfg.color_esp_ally_visible, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Team Invisible##espteamcolor2", Cfg.color_esp_ally_occluded, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Enemy Visible##espenemycolor1", Cfg.color_esp_enemy_visible, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Enemy Invisible##espenemycolor2", Cfg.color_esp_enemy_occluded, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Skeleton color", Cfg.color_esp_player_skeleton, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Grenade Prediction color", Cfg.color_grenade_prediction, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Sound color", Cfg.color_esp_sounds, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Glow Enemy##espteamcolor1", Cfg.color_glow_enemy, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Glow Enemy Inv##espteamcolor1", Cfg.color_glow_enemy_invis, ImGuiColorEditFlags_NoInputs);
					}
					ImGui::EndChild();
				}
				else if (selectedcategory == 1)
				{
					ImGui::BeginChild(("Chams##dois"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Text("Chams COLORS:");
						ImGui::ColorEdit4("Team Visible##chamsteam1", Cfg.color_chams_player_ally_visible, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Team Invisible##chamsteam2", Cfg.color_chams_player_ally_occluded, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Enemy Visible##chamsenemy1", Cfg.color_chams_player_enemy_visible, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Enemy Invisible##chamsenemy2", Cfg.color_chams_player_enemy_occluded, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Backtrack", Cfg.color_chams_backtrack, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit4("Arm Chams", Cfg.color_chams_arms, ImGuiColorEditFlags_NoInputs);
					}
					ImGui::EndChild();
				}
			}
		} 
		ImGui::EndGroup();
		}
		else if (selectedtab == 2)
		{
		ImGui::SetCursorPos({ (float)141, (float)73 });
		ImGui::BeginGroup();
		{
			//São as Catergorias do Meio
			if (selectedsubtab == 0) {
				if (ImGui::category(("Misc##misctab"), selectedcategory == 0))
					selectedcategory = 0;
				if (ImGui::category(("Config"), selectedcategory == 1))
					selectedcategory = 1;

			}
			else if (selectedsubtab == 1) {
				if (ImGui::category(("AA"), selectedcategory == 0))
					selectedcategory = 0;
			}

		}
		ImGui::EndGroup();

		//São as SubTabs Ao Lado Esquerdo
		ImGui::SetCursorPos({ (float)27, (float)93 });
		ImGui::BeginGroup();
		{
			if (ImGui::subtab(("Misc##categoria"), selectedsubtab == 0))
				selectedsubtab = 0;
			if (ImGui::subtab(("Anti Aim"), selectedsubtab == 1))
				selectedsubtab = 1;
		}
		ImGui::EndGroup();

		//Esse são os itens dentro de cada tab
		ImGui::SetCursorPos({ (float)217, (float)96 });
		ImGui::BeginGroup();
		{
			if (selectedsubtab == 0)
			{
				if (selectedcategory == 0)
				{
					ImGui::BeginChild(("Misc##primario"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Checkbox("Bunny hop", &Cfg.misc_bhop);
						if (Cfg.misc_bhop) {
							ImGui::Checkbox("Auto strafe", &Cfg.misc_autostrafe);
						}
						ImGui::Checkbox("Radar", &Cfg.misc_radar);
						ImGui::Checkbox("Engine Radar", &Cfg.misc_engine_radar);
						ImGui::Checkbox("Rank reveal", &Cfg.misc_showranks);
						ImGui::Checkbox("Auto accept", &Cfg.misc_autoaccept);
						ImGui::Checkbox("Clantag", &Cfg.misc_clantag);
						ImGui::Checkbox("Name spam", &Cfg.misc_name_spam);
						ImGui::Checkbox("Chat spam", &Cfg.misc_chat_spam);
						ImGui::Checkbox("Radio spam", &Cfg.misc_radio_spam);
						ImGui::Checkbox("Watermark", &Cfg.misc_watermark);
						ImGui::Checkbox("Left knife", &Cfg.misc_leftknife);
						ImGui::Checkbox("Aspect ratio", &Cfg.aspect_ratio);
						if (Cfg.aspect_ratio) {
							ImGui::SliderInt("##aspect_ration", &Cfg.aspect_ratio_value, 0, 200, "Aspect ratio: %.1f");
						}
						ImGui::Checkbox("Enable agent changer", &Cfg.misc_model);
							ImGui::Combo("Type TT", &Cfg.misc_model_type_tt, agent_types, IM_ARRAYSIZE(agent_types));
							ImGui::Combo("Type CT", &Cfg.misc_model_type_ct, agent_types, IM_ARRAYSIZE(agent_types));

						ImGui::Checkbox("BackDrop", &Cfg.backdrop);
						if (ImGui::BeginCombo("Fps Boost", "...")) {
							std::vector<std::string> vec;
							for (size_t i = 0; i < IM_ARRAYSIZE(fps_boost_vec); i++) {
								ImGui::Selectable(fps_boost_vec[i], &Cfg.fps_boost_selected[i], ImGuiSelectableFlags_DontClosePopups);
								if (Cfg.fps_boost_selected[i])
									vec.push_back(fps_boost_vec[i]);
							}
							ImGui::EndCombo();
						}
					}
					ImGui::EndChild();
				}
				else if (selectedcategory == 1)
				{
					ImGui::BeginChild(("Config##cfg"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
					static std::string current_config;
					static auto is_configs_loaded = false;

					static char config_name[32];

						static std::vector<std::string> configs;

						static auto load_configs = []() {
							std::vector<std::string> items = {};

							std::string path("C:\\white.xyz\\configs");
							if (!fs::is_directory(path))
								fs::create_directories(path);

							for (auto& p : fs::directory_iterator(path))
								items.push_back(p.path().string().substr(path.length() + 1));

							return items;
						};

						if (!is_configs_loaded) {
							is_configs_loaded = true;
							configs = load_configs();
						}

						ImGui::ListBoxHeader("##configs", ImVec2(-1, 150)); {
							for (auto& config : configs) {
								if (ImGui::Selectable(config.c_str(), config == current_config)) {
									current_config = config;
								}
							}
						}
						ImGui::ListBoxFooter();

						ImGui::InputText("Config name##config_name", config_name, sizeof(config_name));

						if (ImGui::Button("Create", ImVec2(-1, 20))) {
							current_config = std::string(config_name);

							CConfig::Get().Save(current_config + ".cfg");
							is_configs_loaded = false;
							memset(config_name, 0, 32);
						}

						if (ImGui::Button("Load", ImVec2(-1, 20))) {
							CConfig::Get().Load(current_config);

							static auto clear_hud_weapon_icon_fn =
								reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
									Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

							auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

							if (!element)
								return;

							auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
							if (hud_weapons == nullptr)
								return;

							if (!*hud_weapons->get_weapon_count())
								return;

							for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
								i = clear_hud_weapon_icon_fn(hud_weapons, i);

							typedef void(*ForceUpdate) (void);
							static ForceUpdate FullUpdate =
								(ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
							FullUpdate();
						}

						if (ImGui::Button("Save", ImVec2(-1, 20)))
							CConfig::Get().Save(current_config);

						if (ImGui::Button("Delete", ImVec2(-1, 20)) && fs::remove("C:\\white.xyz\\configs\\" + current_config)) {
							current_config.clear();
							is_configs_loaded = false;
						}

						if (ImGui::Button("Refresh", ImVec2(-1, 20)))
							is_configs_loaded = false;
					}
					ImGui::EndChild();
				}
			}
			else if (selectedsubtab == 1)
			{
			ImGui::BeginChild(("AntiAim##primario"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
			{
				ImGui::Combo("Desync", &Cfg.misc_desync, desync, 3);
				if (Cfg.misc_desync)
					ImGui::Hotkey("Flip bind##legitaa", &Cfg.misc_desync_bind);
				ImGui::Spacing();
				ImGui::Checkbox("Legit Resolver", &Cfg.legitresolver);
			}
			ImGui::EndChild();
			}
		}
		ImGui::EndGroup();
		}
		else if (selectedtab == 3)
		{

		ImGui::SetCursorPos({ (float)141, (float)73 });
		ImGui::BeginGroup();
		{
			//São as Catergorias do Meio
			if (selectedsubtab == 0) {
				if (ImGui::category(("Skin##skinschangers"), selectedcategory == 0))
					selectedcategory = 0;

			}
		}
		ImGui::EndGroup();

		//São as SubTabs Ao Lado Esquerdo
		ImGui::SetCursorPos({ (float)27, (float)93 });
		ImGui::BeginGroup();
		{
			if (ImGui::subtab(("Skin Changer##skinchanger"), selectedsubtab == 0))
				selectedsubtab = 0;
		}
		ImGui::EndGroup();

		//Esse são os itens dentro de cada tab
		ImGui::SetCursorPos({ (float)217, (float)96 });
		ImGui::BeginGroup();
		{
			if (selectedsubtab == 0)
			{
				if (selectedcategory == 0)
				{
					ImGui::BeginChild(("changertab##skinchangerchild"), ImVec2(325, 350), ImGuiWindowFlags_NoScrollbar);
					{
						const auto wnd = ImGui::GetCurrentWindowRead();
						float height = (wnd->Pos.y + wnd->Size.y) -
							wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;

						ImGui::ListBoxHeader("##configchanger", ImVec2(-1, 150)); {
							for (size_t w = 0; w < k_weapon_names.size(); w++) {
								if (ImGui::Selectable(k_weapon_names[w].name, definition_vector_index == w)) {
									definition_vector_index = w;
								}
							}
						}
						ImGui::ListBoxFooter();

						if (ImGui::Button("Update Skins", ImVec2(-1, 20))) {
							static auto clear_hud_weapon_icon_fn =
								reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
									Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

							auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

							if (!element) return;

							auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
							if (hud_weapons == nullptr) return;

							if (!*hud_weapons->get_weapon_count()) return;

							for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
								i = clear_hud_weapon_icon_fn(hud_weapons, i);

							typedef void(*ForceUpdate) (void);
							static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
							FullUpdate();
						}

						auto& satatt = Cfg.skins.statrack_items[k_weapon_names[definition_vector_index].definition_index];
						auto& selected_entry = entries[k_weapon_names[definition_vector_index].definition_index];
						selected_entry.definition_index = k_weapon_names[definition_vector_index].definition_index;
						selected_entry.definition_vector_index = definition_vector_index;
						ImGui::InputInt("Seed", &selected_entry.seed);
						ImGui::SliderFloat("¹¹Wear", &selected_entry.wear, FLT_MIN, 1.f, "Wear: %.10f", 5);
						ImGui::Checkbox("Stat Track##2", &selected_entry.stat_trak);
						if (selected_entry.stat_trak) {
							ImGui::InputInt("Value##stattrack", &satatt.statrack_new.counter);
						}

						if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) {
							ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
								*out_text = k_knife_names.at(idx).name;
								return true;
								}, nullptr, k_knife_names.size(), 10);
							selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
						}
						else if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) {
							ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
								*out_text = k_glove_names.at(idx).name;
								return true;
								}, nullptr, k_glove_names.size(), 10);
							selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
						}
						else {
							static auto unused_value = 0;
							selected_entry.definition_override_vector_index = 0;
						}

						static char filter_name[32];
						std::string filter = filter_name;

						bool is_glove = selected_entry.definition_index == GLOVE_T_SIDE ||
							selected_entry.definition_index == GLOVE_CT_SIDE;

						bool is_knife = selected_entry.definition_index == WEAPON_KNIFE ||
							selected_entry.definition_index == WEAPON_KNIFE_T;

						int cur_weapidx = 0;
						if (!is_glove && !is_knife) {
							cur_weapidx = k_weapon_names[definition_vector_index].definition_index;
						}
						else {
							if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) {
								cur_weapidx = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
							}
							if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) {
								cur_weapidx = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
							}
						}

						ImGui::InputText("Skin Filter##skins", filter_name, sizeof(filter_name));

						auto weaponName = weaponnames(cur_weapidx);

						ImGui::ListBoxHeader("##skinssdsdadsdadas", ImVec2(-1, 110)); {
							if (selected_entry.definition_index != GLOVE_T_SIDE && selected_entry.definition_index != GLOVE_CT_SIDE) {
								if (ImGui::Selectable("None", selected_entry.paint_kit_index == -1)) {
									selected_entry.paint_kit_vector_index = -1;
									selected_entry.paint_kit_index = -1;
									selected_skin_name = "";
								}

								int lastID = ImGui::GetItemID();
								for (size_t w = 0; w < k_skins.size(); w++) {
									for (auto names : k_skins[w].weaponName) {
										std::string name = k_skins[w].name;

										if (Cfg.skins.show_cur) {
											if (names != weaponName)
												continue;
										}

										if (name.find(filter) != name.npos) {
											ImGui::PushID(lastID++);

											ImGui::PushStyleColor(ImGuiCol_Text, Skins::Get().GetColorRatiry(is_knife ? 6 : k_skins[w].rarity)); {
												if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w)) {
													selected_entry.paint_kit_vector_index = w;
													selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
													selected_skin_name = k_skins[w].name_short;
												}
											}
											ImGui::PopStyleColor();

											ImGui::PopID();
										}
									}
								}
							}
							else {
								int lastID = ImGui::GetItemID();

								if (ImGui::Selectable("None", selected_entry.paint_kit_index == -1)) {
									selected_entry.paint_kit_vector_index = -1;
									selected_entry.paint_kit_index = -1;
									selected_skin_name = "";
								}

								for (size_t w = 0; w < k_gloves.size(); w++) {
									for (auto names : k_gloves[w].weaponName) {
										std::string name = k_gloves[w].name;

										if (Cfg.skins.show_cur) {
											if (names != weaponName)
												continue;
										}

										if (name.find(filter) != name.npos) {
											ImGui::PushID(lastID++);
											ImGui::PushStyleColor(ImGuiCol_Text, Skins::Get().GetColorRatiry(6)); {
												if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w)) {
													selected_entry.paint_kit_vector_index = w;
													selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
													selected_skin_name = k_gloves[selected_entry.paint_kit_vector_index].name_short;
												}
											}
											ImGui::PopStyleColor();

											ImGui::PopID();
										}
									}
								}
							}
							ImGui::ListBoxFooter();
						}

					}
					ImGui::EndChild();
				}
			}
		}
		ImGui::EndGroup();

		}
	}
	ImGui::End();
	ImGui::Render();
}
//--------------------------------------------------------------------------------
void Menu::Toggle() {
	_opened = !_opened;
}
//--------------------------------------------------------------------------------
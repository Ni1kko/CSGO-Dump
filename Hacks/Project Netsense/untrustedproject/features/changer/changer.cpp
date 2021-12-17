#include "changer.hpp"
#include "../legitbot/aimbot.hpp"
#include "../../addons/ui_addons/ui_addons.hpp"
#include "../../helpers/utils.hpp"
#include "../../config.hpp"

Skins* g_SkinChanger = new Skins();
extern LPDIRECT3DTEXTURE9 m_skin_texture = nullptr;

static auto get_wearable_create_fn() -> CreateClientClassFn {
	auto clazz = g_CHLClient->GetAllClasses();

	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;
	return clazz->m_pCreateFn;
}

void Skins::erase_override_if_exists_by_index(const int definition_index) {
	if (values::WeaponInfo.count(definition_index)) {
		auto& icon_override_map = options.IconOverrides;
		const auto& original_item = values::WeaponInfo.at(definition_index);
		if (original_item.icon && icon_override_map.count(original_item.icon))
			icon_override_map.erase(icon_override_map.at(original_item.icon));
	}
}

void Skins::apply_config_on_attributable_item(C_BaseAttributableItem* item, const item_setting* config, const unsigned xuid_low) {
	item->m_Item().m_iItemIDHigh() = -1;
	item->m_Item().m_iAccountID() = xuid_low;

	if (config->name[0] != '\0')
		strcpy_s(item->m_Item().m_iCustomName(), config->name);

	if (config->paint_kit_index)
		item->m_nFallbackPaintKit() = config->paint_kit_index;

	if (config->seed)
		item->m_nFallbackSeed() = config->seed;

	if (config->stat_trak + 1 && config->enable_stat_track)
		item->m_nFallbackStatTrak() = config->stat_trak;


	item->m_flFallbackWear() = config->wear;
	auto& definition_index = item->m_Item().m_iItemDefinitionIndex();
	auto& icon_override_map = options.IconOverrides;
	if (config->definition_override_index && config->definition_override_index != definition_index && values::WeaponInfo.count(config->definition_override_index)) {
		const auto old_definition_index = definition_index;
		definition_index = config->definition_override_index;
		const auto& replacement_item = values::WeaponInfo.at(config->definition_override_index);
		item->m_nModelIndex() = g_MdlInfo->GetModelIndex(replacement_item.model);
		item->SetModelIndex(g_MdlInfo->GetModelIndex(replacement_item.model));
		item->GetClientNetworkable()->PreDataUpdate(0);
		if (old_definition_index && values::WeaponInfo.count(old_definition_index)) {
			const auto& original_item = values::WeaponInfo.at(old_definition_index);
			if (original_item.icon && replacement_item.icon) {
				icon_override_map[original_item.icon] = replacement_item.icon;
			}
		}
	}
	else {
		erase_override_if_exists_by_index(definition_index);
	}
}

void Skins::OnFrameStageNotify(bool frame_end) {
	const auto local_index = g_EngineClient->GetLocalPlayer();
	const auto local = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(local_index));
	if (!local)
		return;

	player_info_t player_info;
	if (!g_EngineClient->GetPlayerInfo(local_index, &player_info))
		return;

	if (frame_end) {
		const auto wearables = local->m_hMyWearables();
		const auto glove_config = &options.Items[GLOVE_T_SIDE];

		static auto glove_handle = CBaseHandle(0);
		auto glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(wearables[0]));
		if (!glove) {
			const auto our_glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(glove_handle));
			if (our_glove) // Our glove still exists
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}

		if (!local->IsAlive()) {
			if (glove) {
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}
			return;
		}

		if (glove_config && glove_config->definition_override_index) {
			if (!glove) {
				static auto create_wearable_fn = get_wearable_create_fn();
				const auto entry = g_EntityList->GetHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				//glove = static_cast<C_BaseAttributableItem*>(create_wearable_fn(entry, serial));
				create_wearable_fn(entry, serial);
				glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntity(entry));

				assert(glove);
				{
					static auto set_abs_origin_addr = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1");
					const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
					static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
					set_abs_origin_fn(glove, new_pos);
				}

				const auto wearable_handle = reinterpret_cast<CBaseHandle*>(&wearables[0]);
				*wearable_handle = entry | serial << 16;
				glove_handle = wearables[0];
			}

			// Thanks, Beakers
			glove->SetGloveModelIndex(-1);
			apply_config_on_attributable_item(glove, glove_config, player_info.xuid_low);
		}
	}
	else {
		auto weapons = local->m_hMyWeapons();
		for (int i = 0; weapons[i].IsValid(); i++) {
			C_BaseAttributableItem* weapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
			if (!weapon)
				continue;
			auto& definition_index = weapon->m_Item().m_iItemDefinitionIndex();
			const auto active_conf = &options.Items[is_knife(definition_index) ? WEAPON_KNIFE : definition_index];
			apply_config_on_attributable_item(weapon, active_conf, player_info.xuid_low);
		}

		const auto view_model_handle = local->m_hViewModel();
		if (!view_model_handle.IsValid())
			return;

		const auto view_model = static_cast<C_BaseViewModel*>(g_EntityList->GetClientEntityFromHandle(view_model_handle));
		if (!view_model)
			return;

		const auto view_model_weapon_handle = view_model->m_hWeapon();
		if (!view_model_weapon_handle.IsValid())
			return;

		const auto view_model_weapon = static_cast<C_BaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
		if (!view_model_weapon)
			return;

		if (values::WeaponInfo.count(view_model_weapon->m_Item().m_iItemDefinitionIndex())) {
			const auto override_model = values::WeaponInfo.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
			auto override_model_index = g_MdlInfo->GetModelIndex(override_model);
			view_model->m_nModelIndex() = override_model_index;
			auto world_model_handle = view_model_weapon->m_hWeaponWorldModel();
			if (!world_model_handle.IsValid())
				return;
			const auto world_model = static_cast<C_BaseWeaponWorldModel*>(g_EntityList->GetClientEntityFromHandle(world_model_handle));
			if (!world_model)
				return;
			world_model->m_nModelIndex() = override_model_index + 1;
		}

	}
}
template<class T>

static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan(GetModuleHandleA("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

void Skins::Menu() {
	ImGui::Columns(3, nullptr, false);

	static char search_bar[32];
	int menur, menug, menub, menua;
	menur = g_Misc->options.cMenuCol[0] * 255;
	menug = g_Misc->options.cMenuCol[1] * 255;
	menub = g_Misc->options.cMenuCol[2] * 255;
	menua = g_Misc->options.cMenuCol[3] * 255;

	ImGui::BeginChild("##1", ImVec2(-1, 0), true); {
		//ImGui::Text("Selector");
		ImGui::TextColored(ImColor(menur, menug, menub), "Weapon Selector");
		if (ImGui::Button("Current", ImVec2(-1, 19)) || Menu::Get().SkinsFirstTimeRender)
			PickCurrentWeapon(&weapon_index_skins, &weapon_vector_index_skins, values::WeaponNamesFull);

		int iter = 0;
		for (auto& weapon : values::WeaponNamesFull) {
			if (ImGui::ButtonT(weapon.name, ImVec2(-1, 19), weapon_index_skins, weapon.definition_index, false, false)) {
				weapon_index_skins = weapon.definition_index;
				weapon_vector_index_skins = iter;
			}
			iter++;
		}
	};
	ImGui::EndChild();
	ImGui::NextColumn();


	auto& selected_entry = options.Items[values::WeaponNamesFull[weapon_vector_index_skins].definition_index];
	selected_entry.definition_index = weapon_index_skins;
	selected_entry.definition_vector_index = weapon_vector_index_skins;
	bool is_gloves = false;


	//ImGui::Spacing();
	ImGui::BeginChild("##2", ImVec2(-1, ((ImGui::GetCurrentWindow()->Size.y / 2) - 30)), true); {
		//ImGui::Text("Params");
		ImGui::TextColored(ImColor(menur, menug, menub), "Params");
		ImGui::InputInt("Seed", &selected_entry.seed);
		ImGui::InputInt("StatTrak", &selected_entry.stat_trak);
		ImGui::InputText("Name##Skins", selected_entry.name, sizeof(selected_entry.name));
		if (ImGui::SliderFloat("Wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5))
			selected_entry.wear = std::clamp(selected_entry.wear, FLT_MIN, 1.f);
		if (selected_entry.definition_index == WEAPON_KNIFE) {
			ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = values::KnifeNames.at(idx).name;
				return true;
			}, nullptr, values::KnifeNames.size(), 10);
			selected_entry.definition_override_index = values::KnifeNames.at(selected_entry.definition_override_vector_index).definition_index;
		}
		else if (selected_entry.definition_index == GLOVE_T_SIDE) {
			ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
				*out_text = values::GloveNames.at(idx).name;
				return true;
			}, nullptr, values::GloveNames.size(), 10);
			selected_entry.definition_override_index = values::GloveNames.at(selected_entry.definition_override_vector_index).definition_index;
			is_gloves = true;
		}
		else
			selected_entry.definition_override_vector_index = 0;

		static float next_enb_time = 0;
		float time_to_next_up = g_GlobalVars->curtime;

		time_to_next_up = std::clamp(next_enb_time - g_GlobalVars->curtime, 0.f, 1.f);

		std::string name = "Update (";
		name += std::to_string(time_to_next_up);
		name.erase(12, 16);
		name += ")";

		if (ImGui::Button(name.c_str(), ImVec2(-1, 19))) {
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
			static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
			FullUpdate();

		}
	};
	ImGui::EndChild();

	//ImGui::Text("Preview:");
	ImGui::BeginChild("##3", ImVec2(-1, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); {
		if (m_skin_texture) {
			ImGui::Image(m_skin_texture, ImGui::GetCurrentWindow()->Size);
		}
	};
	ImGui::EndChild();
	ImGui::NextColumn();


	ImGui::BeginChild("##4", ImVec2(-1, -1), true); {
		//ImGui::Text("Paintkit");
		ImGui::TextColored(ImColor(menur, menug, menub), "Paintkit");
		static bool showAllWeapons = false;
		ImGui::Checkbox("Enable StatTrack", &selected_entry.enable_stat_track);
		ImGui::Checkbox("All paint kits", &showAllWeapons);
		ImGui::InputText("Filter##search_bar_skins", search_bar, sizeof(search_bar));

		ImGui::ListBoxHeader("##paintkits", ImVec2(-1, -1));

		if (ImGui::Selectable("Default", selected_entry.paint_kit_index == 0)) {
			selected_entry.paint_kit_index = 0;
			//selected_entry.paint_kit_vector_index = 0;
		}

		static auto GetColRar = [&](int rar) -> ImVec4 {
			switch (rar)
			{
			case 1:
				return ImColor(150, 150, 150, 255);
				break;
			case 2:
				return ImColor(100, 100, 255, 255);
				break;
			case 3:
				return ImColor(50, 50, 255, 255);
				break;
			case 4:
				return ImColor(255, 64, 242, 255);
				break;
			case 5:
				return ImColor(255, 50, 50, 255);
				break;
			case 6:
				return ImColor(255, 50, 50, 255);
				break;
			case 7:
				return ImColor(255, 196, 46, 255);
				break;
			default:
				return ImColor(150, 150, 150, 255);
				break;
			}
		};

#pragma region C + P

		//int defindex = (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) ? definition_override_index : ((selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) ? values::GetGloveDefinitionIndex(selected_entry.definition_override_vector_index) : selected_entry.definition_index);
		bool is_knife = selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T;
		int defindex = (is_knife || selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) ? selected_entry.definition_override_index : selected_entry.definition_index;
		std::string skinName = values::GetWeaponNameById(defindex);
		if (skinName.compare("") != 0 || showAllWeapons)
		{
			std::string filter = std::string(search_bar);

			int curItem = -1;
			int s = 0;

			auto set = weaponSkins[skinName];
			std::vector<std::string> list(set.size());
			std::vector<std::string> anti_repeat;

			std::copy(set.begin(), set.end(), list.begin());

			if (showAllWeapons) {
				list = {};
				for (auto [key, value] : weaponSkins) {
					for (auto skin : value) {
						list.push_back(skin);
					}
				}
			}

			auto fnAntiRepeat = [&](std::string tx) -> bool {
				auto ret = std::find(anti_repeat.begin(), anti_repeat.end(), tx) == anti_repeat.end();
				if (ret)
					anti_repeat.push_back(tx);
				return ret;
			};

			for (auto skin : list) {
				int pk = skinMap[skin].paintkit;
				if (pk == selected_entry.paint_kit_index)
					curItem = s;

				bool passed_filter = true;

				if (!filter.empty()) {
					std::string tempName = skinNames[skinMap[skin].tagName];
					std::string tempQuery = filter;
					std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);
					std::transform(tempQuery.begin(), tempQuery.end(), tempQuery.begin(), ::tolower);
					if (tempName.find(tempQuery) == std::string::npos)
						passed_filter = false;
				}

				if (passed_filter && fnAntiRepeat(skinNames[skinMap[skin].tagName])) {
					ImGui::PushStyleColor(ImGuiCol_Text, is_knife ? GetColRar(6) : GetColRar(skinMap[skin].rarity));//tut chvet texta 
					if (ImGui::Selectable((skinNames[skinMap[skin].tagName] + "##" + skinName).c_str(), pk == selected_entry.paint_kit_index)) {
						selected_entry.paint_kit_index = pk;
					}
					ImGui::PopStyleColor();
				}

				s++;
			}

			static int saved_pk_index = -1;
			static int saved_w_index = -1;
			static int saved_wo_index = -1;
			if (saved_pk_index != selected_entry.paint_kit_index || saved_w_index != selected_entry.definition_index || saved_wo_index != selected_entry.definition_override_index || Menu::Get().SkinsFirstTimeRender) {
				saved_pk_index = selected_entry.paint_kit_index;
				saved_w_index = selected_entry.definition_index;
				saved_wo_index = selected_entry.definition_override_index;

				std::string validFname = "";
				if (saved_pk_index != 0)
					for (auto s : weaponSkins[skinName]) {
						auto gg = skinMap[s];
						if (gg.paintkit == selected_entry.paint_kit_index) {
							validFname = s;
							break;
						}
					}

				std::string full_path;

				if (validFname == "") {
					full_path = "resource/flash/econ/weapons/base_weapons/";
					if (!is_gloves)
						full_path += "weapon_";
					full_path += skinName + ".png";
				}
				else {
					full_path = ("resource/flash/econ/default_generated/");
					if (!is_gloves)
						full_path += "weapon_";
					full_path += skinName + "_" + validFname + "_light_large.png";
				}


				const auto handle = g_FileSys->open(full_path.c_str(), "r", "GAME");
				if (handle) {
					int file_len = g_FileSys->size(handle);
					char* image = new char[file_len];

					g_FileSys->read(image, file_len, handle);
					g_FileSys->close(handle);

					D3DXCreateTextureFromFileInMemory(g_D3DDevice9, image, file_len, &m_skin_texture);

					delete[] image;
				}
				else
					m_skin_texture = nullptr;
			}
		}

#pragma endregion

		ImGui::ListBoxFooter();
	};
	ImGui::EndChild();

	Menu::Get().SkinsFirstTimeRender = false;
}


void Skins::SetupValues() {
	for (auto& [key, val] : values::WeaponNamesFull) {
		auto& option = options.Items[key];
		g_Config->PushItem(&option.definition_vector_index, val, "d_vec_index", option.definition_vector_index);
		g_Config->PushItem(&option.definition_index, val, "d_index", option.definition_index);
		g_Config->PushItem(&option.paint_kit_vector_index, val, "pk_vec_index", option.paint_kit_vector_index);
		g_Config->PushItem(&option.paint_kit_index, val, "pk_index", option.paint_kit_index);
		g_Config->PushItem(&option.definition_override_index, val, "do_index", option.definition_override_index);
		g_Config->PushItem(&option.definition_override_vector_index, val, "do_vec_index", option.definition_override_vector_index);
		g_Config->PushItem(&option.seed, val, "seed", option.seed);
		g_Config->PushItem(&option.enable_stat_track, val, "stattrack", option.enable_stat_track);
		g_Config->PushItem(&option.stat_trak, val, "stat_trak_val", option.stat_trak);
		g_Config->PushItem(&option.wear, val, "wear", option.wear);
	}
}
 #include "../render.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../features/features.h"
#include "../../helpers/console.h"

extern void bind_button(const char* eng, const char* rus, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		char name[64];
		char localtag[16];
		bool is_clantag_copied = false;

		void misc_tab()
		{
			child(___("Name", u8"���"), []()
			{
				ImGui::InputText("##misc_name", name, sizeof(name));
				columns(2);
				{
					if (ImGui::Button(___("Apply##name", u8"���������##name"), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						utils::set_player_name(name);

					ImGui::NextColumn();

					if (ImGui::Button(___("Hide", u8"������"), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						utils::set_player_name("\n\xAD\xAD\xAD");
				}
				columns(1);

				separator(___("Clan Tag", u8"�������"));

				if (!is_clantag_copied && std::string(localtag) != globals::clantag::value)
				{
					is_clantag_copied = true;
					strcpy(localtag, globals::clantag::value.c_str());
				}

				ImGui::InputText("##misc_clantag", localtag, sizeof(localtag));
				//if (globals::clantag::animation)
					//ImGui::SliderFloatLeftAligned(___("Delay:", u8"��������:"), &globals::clantag::delay, 0, 1);

				auto player_resource = *interfaces::player_resource;
				if (player_resource)
				{
					if (ImGui::BeginCombo("##clantags.list", ___("Player tags", u8"���� �������")))
					{
						std::vector<std::string> tags;

						for (int i = 1; i < interfaces::engine_client->GetMaxClients(); ++i)
						{
							auto* player = c_base_player::GetPlayerByIndex(i);
							if (!player)
								continue;

							const auto info = player->GetPlayerInfo();
							if (info.fakeplayer)
								continue;

							auto usertag = std::string(player_resource->GetClanTag()[player->GetIndex()]);
							if (usertag.empty() || std::find(tags.begin(), tags.end(), usertag) != tags.end())
								continue;

							tags.push_back(usertag);

							if (ImGui::Selectable(usertag.c_str()))
							{
								strcpy(localtag, usertag.c_str());
								globals::clantag::value = usertag;
								clantag::set(localtag);
							}
						}

						ImGui::EndCombo();
					}
				}

				columns(2);
				{
					if (ImGui::Button(___("Apply##clan", u8"���������##clan"), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
					{
						globals::clantag::value = localtag;
						clantag::set(localtag);

						globals::save();
					}

					ImGui::NextColumn();

					if (ImGui::Button(!globals::clantag::animation ? ___("Animation", u8"����.") : ___("Dont Animate", u8"�� ����."), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						globals::clantag::animation = !globals::clantag::animation;
				}
				columns(1);

				separator(___("Fake Lags", u8"���� ����"));

				ImGui::PushID("fakelags");
				{
					columns(2);
					{
						checkbox("Enabled", u8"��������", &settings::fake_lags::enabled);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						hotkey("##binds.fake_lags", &globals::binds::fake_lag);
						ImGui::PopItemWidth();
					}
					columns(1);

					const char* lag_types[] =
					{
						___("Always", u8"������"),
						___("Only in air", u8"������ � ������"),
						___("When picking", u8"�� ����� ����"),
						___("By button", u8"�� ������"),
						___("Adaptive", u8"�� ������")
					};

					ImGui::Combo("##lag_type", &settings::fake_lags::type, lag_types, IM_ARRAYSIZE(lag_types));
					ImGui::SliderIntLeftAligned(___("Factor:", u8"������:"), &settings::fake_lags::factor, 1, 14, ___("%.0f ticks", u8"%.0f �����"));
				}
				ImGui::PopID();

				static const char* desyncModes[] = {
					"Off",
					"Static",
					"Balance"
				};

				separator("Desync", u8"���������������");
				ImGui::PushID("desync");
				{
					columns(2);
					{
						checkbox("Enabled", u8"��������", &settings::desync::enabled2);

						ImGui::NextColumn();
						ImGui::Combo("Mode", &settings::desync::desync_mode, desyncModes, IM_ARRAYSIZE(desyncModes));
					}
					columns(1);

					bind_button("Left/Right", u8"Left/Right", globals::binds::desync);
				}
				ImGui::PopID();

				separator("FOV");

				ImGui::SliderFloatLeftAligned(___("View Model:", u8"����:"), &settings::misc::viewmodel_fov, 54, 120, "%.0f *");
				ImGui::SliderIntLeftAligned(___("Debug:", u8"�����:"), &settings::misc::debug_fov, 80, 120, "%.0f *");
			});

			ImGui::NextColumn();

			child(___("Extra", u8"������"), []()
			{
				//checkbox("Russian", u8"������� ����", &globals::russian_language);
				//checkbox("Disable Animations (?)", u8"��������� �������� (?)", &globals::no_animations);
				//tooltip("Disables the cheat menu animations.", u8"��������� �������� ���� ����.");

				static const char* skyList[] = { "Baggage", "Tibet", "Embassy", "Italy", "Daylight 1", "Daylight 2", "Daylight 3", "Daylight 4", "Cloudy", "Night 1", "Night 2", "Night Flat", "Day HD", "Day", "Rural", "Vertigo HD", "Vertigo", "Dusty Sky", "Jungle", "Nuke", "Office" };

				checkbox("Engine Prediction", &settings::movement::engine_prediction);

				checkbox("Radar", u8"�����", &settings::misc::radar);
				checkbox("No Flash", u8"������ ��������", &settings::misc::no_flash);
				checkbox("Bunny Hop", u8"���������", &settings::misc::bhop);
				checkbox("Auto Strafe", u8"�������", &settings::misc::auto_strafe);
				checkbox("Knife Bot", u8"������� ���", &settings::misc::knife_bot);
				checkbox("Moon Walk", u8"������ �������", &settings::misc::moon_walk);
				checkbox("Deathmatch", u8"��� ��������", &settings::misc::deathmatch);
				checkbox("Post Processing", u8"��������������", &globals::post_processing);
				checkbox("Resolver", u8"��������������", &settings::desync::resolver);
				checkbox("Humanised Bhop", u8"��������������", &settings::misc::human_bhop);
				checkbox("Noscope Overlay", u8"��������������", &settings::misc::noscope);
				checkbox("-98 Nade (?)", &settings::misc::selfnade);
				tooltip("Look up, Hold mouse 2,When you're fully primed to throw with mouse 2 start holding mouse 1, crouch when released.");
				tooltip("Disabled saving of Bullet Tracer - Causes bug that will make beams not render :(. Default color is purple.");

				separator("Skybox Changer");
				ImGui::Combo("Sky List", &settings::visuals::skychanger_mode, skyList, IM_ARRAYSIZE(skyList));
				if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
				{
					color_modulation::SkyChanger();
				}
			});

			ImGui::NextColumn();

			child(___("Binds", u8"������"), []()
			{
				bind_button("Zeus Bot", u8"Zeus Bot", globals::binds::zeus_bot);
				bind_button("Third Person", "Third Person", globals::binds::thirdperson::key);
				bind_button("Lightning Shot", "Lightning Shot", globals::binds::lightning_shot);

				separator(___("Humanised Bhop Settings", u8"���� ����"));

				ImGui::SliderIntLeftAligned("Bhop Hit Chance", &settings::misc::bhop_hit_chance, 0, 100, "%.0f %%");
				ImGui::SliderIntLeftAligned("Hops Limit", &settings::misc::hops_restricted_limit, 0, 12);
				ImGui::SliderIntLeftAligned("Max Hops Limit", &settings::misc::max_hops_hit, 0, 12);

				separator("Chams - Misc");

				checkbox("Viewmodel Weapons", &settings::chams::wepchams);
				checkbox("Planted C4", &settings::chams::plantedc4_chams);
				checkbox("Dropped Weapons", &settings::chams::wep_droppedchams);
				checkbox("Grenades", &settings::chams::nade_chams);

				separator("Chams Misc - Colors");

				ImGui::ColorEdit4("Viewmodel Weapons", settings::chams::clr_weapon_chams, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Planted C4", settings::chams::clr_plantedc4_chams, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Dropped Weapons", settings::chams::clr_weapon_dropped_chams, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Grenades", settings::chams::clr_nade_chams, ImGuiColorEditFlags_NoInputs);

				if (!interfaces::local_player)
				{
					ImGui::Separator();

					char btn_label[256];
					sprintf_s(btn_label, "%s (%d)", ___("Invite nearby players", u8"���������� ����. �������"), lobby_inviter::max_count);

					if (ImGui::Button(btn_label, ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
						lobby_inviter::inviteAll();
				}
			});
		}
	}
}
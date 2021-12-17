#include "../render.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../helpers/imdraw.h"
#include "../../helpers/console.h"

extern void bind_button(const char* eng, const char* rus, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		void visuals_tab()
		{
			child("ESP", []()
			{
				columns(2);
				{
					checkbox("Enabled", u8"��������", &settings::esp::enabled);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					hotkey("##binds.esp", &globals::binds::esp);
					ImGui::PopItemWidth();
				}
				columns(1);

				checkbox("Visible Only", u8"�������� ���������", &settings::esp::visible_only);

				checkbox("Name", u8"���", &settings::esp::names);
				checkbox("Weapon", u8"������", &settings::esp::weapons);

				columns(2);
				{
					checkbox("Boxes", u8"�����", &settings::esp::boxes);

					ImGui::NextColumn();

					const char* box_types[] = {
						___("Normal", u8"�������"), ___("Corner", u8"�������")
					};

					ImGui::PushItemWidth(-1);
					{
						ImGui::Combo("##esp.box_type", &settings::esp::box_type, box_types, IM_ARRAYSIZE(box_types));
					}
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				const char* positions[] =
				{
					___("Left", u8"�����"),
					___("Right", u8"������"),
					___("Bottom", u8"�����"),
				};

				const char* HealthPositions[] =
				{
					___("Left", u8"�����"),
					___("Right", u8"������"),
					___("Bottom", u8"�����"),
					___("Number", u8"�����"),
				};

				columns(2);
				{
					checkbox("Health", u8"��������", &settings::esp::health);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("##health.position", &settings::esp::health_position, HealthPositions, IM_ARRAYSIZE(HealthPositions));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox("Armor", u8"�����", &settings::esp::armour);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("##armor.position", &settings::esp::armour_position, positions, IM_ARRAYSIZE(positions));
					ImGui::PopItemWidth();
				}
				columns(1);

				static const char* cross_types[] = {
					"Type: Crosshair",
					"Type: Circle"
				};

				//checkbox("Dormant", &Settings::ESP::dormant);
				checkbox("Is Scoped", &settings::esp::is_scoped);
				checkbox("Is Flashed", &settings::esp::is_flashed);
				checkbox("Is Defusing", &settings::esp::is_defusing);
				checkbox("Snap Lines", &settings::esp::snaplines);
				checkbox("Ammo ESP", &settings::esp::ammo);
				checkbox("Money ESP", &settings::esp::money);
				checkbox("Choke ESP", &settings::visuals::choke);
				//checkbox("Beams", u8"���� �����", &settings::esp::beams); //Doesnt work.
				//checkbox("Sound Direction (?)", &settings::esp::sound); //Doesnt work.
				//tooltip("Sound ESP", u8"���������� ��������� ����������� �����, ������ ������ ������.");

				checkbox("Offscreen ESP", u8"����� ����������� (?)", &settings::esp::offscreen);
				separator("RCS Crosshair");
				checkbox("RCS Crosshair", &settings::visuals::rcs_cross);
				ImGui::Combo("RCS Crosshair Type", &settings::visuals::rcs_cross_mode, cross_types, IM_ARRAYSIZE(cross_types));
				if (settings::visuals::rcs_cross_mode == 1)
					ImGui::SliderFloatLeftAligned("Radius", &settings::visuals::radius, 8.f, 18.f, "%.1f");
			});

			ImGui::NextColumn();

			child(___("Chams", u8"������� ������"), []()
			{
				/* //Old chams
					checkbox("Enabled", u8"��������", &settings::chams::enabled);
					checkbox("Visible Only", u8"�������� ���������", &settings::chams::visible_only);
					checkbox("Wireframe", u8"�����", &settings::chams::wireframe);
					checkbox("Flat", &settings::chams::flat);
				*/

				static const char* ChamsTypes[] = { "Visible - Normal", "Visible - Flat", "Visible - Wireframe", "Visible - Glass", "Visible - Metallic",  "XQZ", "Metallic XQZ", "Flat XQZ" };

				static const char* bttype[] = {
				"Off",
				"Last Tick",
				"All Ticks"
				};

				columns(2);
				{
					checkbox("Enemy", &settings::chams::enemynew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("Enemy - Mode", &settings::chams::enemymodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox("Team", &settings::chams::teamnew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("Team - Mode", &settings::chams::teammodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox("Local", &settings::chams::localnew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("Local - Mode", &settings::chams::localmodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				checkbox("Real Angle Chams", &settings::chams::desync);
				separator("BT Chams - Mode");
				ImGui::Combo("BT Chams Mode", &settings::chams::bttype, bttype, IM_ARRAYSIZE(bttype));
				checkbox("BT Chams - Flat", &settings::chams::btflat);
				ColorEdit4("BT Color", &settings::chams::btcolor);

				separator("Colors - Chams");
				ColorEdit4("Enemy Visible", &settings::chams::EnemyColor_vis);
				ImGui::SameLine();
				ColorEdit4("Enemy XQZ", &settings::chams::EnemyColor_XQZ);

				ColorEdit4("Team Visible  ", &settings::chams::TeamColor_vis);
				ImGui::SameLine();
				ColorEdit4("Team XQZ", &settings::chams::TeamColor_XQZ);

				ColorEdit4("Local Visible   ", &settings::chams::LocalColor_vis);
				ImGui::SameLine();
				ColorEdit4("Local XQZ", &settings::chams::LocalColor_XQZ);

				/*separator("Arms", u8"����");

				checkbox("Enabled##arms", u8"��������##arms", &settings::chams::arms::enabled);
				checkbox("Wireframe##arms", u8"�����##arms", &settings::chams::arms::wireframe);

				ImGui::Separator();

				ColorEdit4(___("Visible", u8"�������"), &settings::chams::visible_color);
				ColorEdit4(___("Occluded", u8"�� ���������"), &settings::chams::occluded_color);

				ColorEdit4(___("Arms", u8"����"), &settings::chams::arms::color); */

				separator("Colors - ESP");
				ColorEdit4("ESP Visible", &settings::esp::visible_color);
				ImGui::SameLine();
				ColorEdit4("ESP Invisible", &settings::esp::occluded_color);

				ColorEdit4("RCS Cross ", &settings::visuals::recoilcolor);
				ImGui::SameLine();


			});

			ImGui::NextColumn();

			child(___("Extra", u8"������"), []()
			{
				checkbox("Planted C4", &settings::visuals::planted_c4);
				checkbox("Defuse Kits", u8"������", &settings::visuals::defuse_kit);
				checkbox("World Weapons", u8"��������� ������", &settings::visuals::dropped_weapons);
				checkbox("World Grenades", u8"��������� ������", &settings::visuals::world_grenades);
				checkbox("Sniper Crosshair", u8"����������� ������", &settings::visuals::sniper_crosshair);
				checkbox("Grenade Prediction", u8"������� ������ ������", &settings::visuals::grenade_prediction);
				checkbox("Bomb Damage Esp", &settings::esp::bomb_esp);
				checkbox("Has Kit", &settings::esp::haskit);
				checkbox("Aimbot Fov", &settings::esp::drawFov);
				checkbox("Enemy Armor Status (?)", &settings::esp::kevlarinfo);
				tooltip("Will display HK if enemy has kevlar + helmer or K if enemy has kevlar only.");

				ImGui::Separator();

				const auto old_night_state = settings::visuals::night_mode;
				const auto old_style_state = settings::visuals::newstyle;
				checkbox("Night Mode", u8"������ �����", &settings::visuals::night_mode);

				ColorEdit4(___("Sky", u8"����"), &settings::visuals::sky);
				tooltip("Will not work on Dust 2, Inferno and Nuke", u8"�� ����� �������� �� Dust 2, Inferno � Nuke.");

				ImGui::Checkbox("Night Mode Intensity(?)", &settings::esp::mat_force);
				tooltip("Will update on reconnect.");
				if (settings::esp::mat_force)
				{
					settings::esp::mfts = 0.1f;
					ImGui::SliderFloatLeftAligned(___("Value:", u8"������:"), &settings::esp::mfts, 0.1f, 1.0f, "%.1f %");
				}

				separator("Chance ESP", u8"����");

				checkbox("Chance ESP (?)", &settings::misc::esp_random);
				tooltip("Enables/disables the esp based on chance, that is generated per round.Set chance manually.");

				if (settings::misc::esp_random)
				{
					ImGui::SliderIntLeftAligned("On threshold (?)", &settings::esp::esp_on_chance, 1, 100, "%.0f %%");
					tooltip("Minimal threshold to turn ESP On. Ex: On: 41%, ESP will turn On if chance >= 41");

					ImGui::SliderIntLeftAligned("Off threshold (?)", &settings::esp::esp_off_chance, 1, 100, "%.0f %%");
					tooltip("Minimal threshold to turn ESP Off. Ex: Off: 40%, ESP will turn Off if chance <= 40");
				}

				checkbox("Dark Menu", &settings::visuals::newstyle);
				if (old_style_state != settings::visuals::newstyle) //settings::visuals::night_mode
					imdraw::apply_style(settings::visuals::newstyle);
			});
		}
	}
}
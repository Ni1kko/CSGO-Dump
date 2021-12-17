 #include "../render.h"
#include "../../config.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../helpers/notifies.h"

extern void bind_button(const char* label, int& key);

namespace render
{
	namespace menu
	{
		char filename[32];
		bool is_loaded = true;
		std::string currentName;
		char current_loaded_config[32];

		ImVec2 button_size = ImVec2();

		void initialize()
		{
			if (globals::configs_initialized)
				return;

			globals::configs_initialized = true;
			currentName = globals::settings;
			strcpy(current_loaded_config, globals::settings.c_str());

			button_size = ImVec2(ImGui::GetContentRegionAvailWidth(), 24.f);
		}

		void configs_tab()
		{
			initialize();

			if (!is_loaded)
			{
				config::cache("settings");

				is_loaded = true;
			}

			child(___("New Config", u8"����� ������������"), []()
			{
				ImGui::InputText("##filename", filename, 32);

				if (ImGui::Button(___("Create", u8"�������"), button_size))
				{
					if (strlen(filename) == 0)
					{
						notifies::push(___("Enter config name", u8"������� ��� �������"), notify_state_s::warning_state);
					}
					else
					{
						settings::save(std::string(filename));
						memset(filename, 0, 32);
						is_loaded = false;

						notifies::push(___("Config created", u8"������ ������"));

						globals::settings = currentName;
						globals::save();
					}
				}

				separator(___("Some Actions", u8"����������"));

				if (!currentName.empty())
				{
					ImGui::InputText("##currentname", current_loaded_config, 32);

					if (ImGui::Button(___("Rename", u8"�������������"), button_size))
					{
						if (strlen(current_loaded_config) == 0)
						{
							notifies::push(___("Enter new config name", u8"������� ����� ��� �������"), notify_state_s::warning_state);
						}
						else
						{
							settings::load(currentName);
							config::remove(currentName, "settings");

							currentName = std::string(current_loaded_config);
							settings::save(currentName);
							is_loaded = false;

							notifies::push(___("Config renamed", u8"������ ������������"));

							globals::settings = currentName;
							globals::save();
						}
					}

					if (ImGui::Button(___("Load", u8"���������"), button_size))
					{
						settings::load(currentName);

						globals::settings = currentName;
						globals::save();

						notifies::push(___("Config loaded", u8"������ ��������"));
					}

					if (ImGui::Button(___("Save", u8"���������"), button_size))
					{
						settings::save(currentName);

						globals::settings = currentName;
						globals::save();

						notifies::push(___("Config saved", u8"������ ��������"), notify_state_s::success_state);
					}

					if (ImGui::Button(___("Remove", u8"�������"), button_size))
					{
						if (config::remove(currentName, "settings"))
						{
							currentName.clear();
							is_loaded = false;

							notifies::push(___("Config removed", u8"������ ������"), notify_state_s::success_state);
						}
						else
						{
							notifies::push(___("Something went wrong", u8"�� ������� ������� ������"), notify_state_s::warning_state);
						}
					}
				}
			});

			ImGui::NextColumn();

			child(___("Configs", u8"������������"), []()
			{
				if (!render::fonts::configs_list)
					return;

				ImGui::PushFont(render::fonts::configs_list);
				{
					ImGui::ListBoxHeader("##configs", get_listbox_size(0.f, 26.f));
					{
						for (auto& config : config::cached["settings"])
						{
							if (selectable(config.first.c_str(), config.first == currentName))
							{
								strcpy(current_loaded_config, config.first.c_str());
								currentName = config.first;
							}
						}
					}
					ImGui::ListBoxFooter();
				}
				ImGui::PopFont();

				if (ImGui::Button(___("Refresh", u8"�������� ������"), button_size))
					is_loaded = false;
			});

			ImGui::NextColumn();

			child(___("Binds", u8"�������� �� ������"), []()
			{
				checkbox("Show notify when loaded", u8"���������� ��� ��������", &globals::binds::notify_when_loaded);

				for (auto& config : config::cached["settings"])
					bind_button(config.first.c_str(), globals::binds::configs[config.first]);
			});
		}
	}
}
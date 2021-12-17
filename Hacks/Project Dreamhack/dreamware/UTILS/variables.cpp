#include "../MISC/includes.h"

#include "interfaces.h"

#include "../SDK/ISurface.h"
#include "render.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"

namespace GLOBAL
{
	HWND csgo_hwnd = FindWindow(0, "Counter-Strike: Global Offensive");
	bool should_send_packet;
	bool is_fakewalking;
	int choke_amount;
	float cheat_start_time;
    bool b_IsThirdPerson;
	bool NormalSpeed = true;
	Vector fake_pos;
	Vector real_angles;
	Vector fake_angles;
	Vector strafe_angle;
	bool can_shoot_someone;
	int randomnumber;
	float	flHurtTime;
	bool DisableAA;
	bool disable_fake_lag;
	bool Aimbotting;
	bool do_max_fake_lag;
	bool should_choke_packets;
	using msg_t = void(__cdecl*)(const char*, ...);
	msg_t Msg = reinterpret_cast< msg_t >(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
	std::string old_tag;
	Vector FakePosition;
	int ground_tickz;
	bool CircleStraferActive;
	SDK::CUserCmd originalCMD;
	ImFont* Text;
	Vector angles;

	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skinInfo> skinMap;
	std::unordered_map<std::string, std::string> skinNames;

}
namespace FONTS
{
	unsigned int menu_tab_font;
	unsigned int menu_checkbox_font;
	unsigned int menu_slider_font;
	unsigned int menu_groupbox_font;
	unsigned int menu_combobox_font;
	unsigned int menu_window_font;
	unsigned int numpad_menu_font;
	unsigned int visuals_esp_font;
	unsigned int visuals_esp_font1;
	unsigned int visuals_xhair_font;
	unsigned int visuals_side_font;
	unsigned int visuals_name_font;
	unsigned int visuals_lby_font;
	unsigned int visuals_grenade_pred_font;

	unsigned int visuals_gun_icon;
	unsigned int menulogofont;

	bool ShouldReloadFonts()
	{
		static int old_width, old_height;
		int width, height;
		INTERFACES::Engine->GetScreenSize(width, height);

		if (width != old_width || height != old_height)
		{
			old_width = width;
			old_height = height;
			return true;
		}
		return false;
	}
	void InitFonts()
	{
		menulogofont = RENDER::CreateF("verdana", 39, 550, 0, 10, SDK::FONTFLAG_ANTIALIAS);
		menu_tab_font = RENDER::CreateF("verdana", 18, 550, 0, 0, NULL);
		menu_checkbox_font = RENDER::CreateF("verdana", 18, 550, 0, 0, NULL);
		menu_slider_font = RENDER::CreateF("verdana", 16, 550, 0, 0, NULL);
		menu_groupbox_font = RENDER::CreateF("verdana", 18, 550, 0, 0, NULL);
		menu_combobox_font = RENDER::CreateF("verdana", 16, 550, 0, 0, NULL);
		menu_window_font = RENDER::CreateF("verdana", 18, 550, 0, 0, NULL);
		numpad_menu_font = RENDER::CreateF("Tahoma", 35, 600, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_esp_font1 = RENDER::CreateF("verdana", 14, 300, 0, 0, SDK::FONTFLAG_DROPSHADOW);

		visuals_esp_font = RENDER::CreateF("verdana", 10, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_xhair_font = RENDER::CreateF("Tahoma", 18, 600, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_side_font = RENDER::CreateF("Calibri", 46, 900, 0, 0, NULL);
		visuals_name_font = RENDER::CreateF("Tahoma", 12, 300, 0, 0,  SDK::FONTFLAG_DROPSHADOW);
		visuals_lby_font = RENDER::CreateF("verdana", 30, 650, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		visuals_grenade_pred_font = RENDER::CreateF("undefeated", 18, 400, 0, 0, SDK::FONTFLAG_DROPSHADOW | SDK::FONTFLAG_ANTIALIAS);
		visuals_gun_icon = RENDER::CreateF("Counter-Strike", 22, 450, 0, 0, SDK::FONTFLAG_ANTIALIAS);
	}
}
namespace SETTINGS
{
	CSettings settings;

	bool CSettings::Save(std::string file_name)
	{
		std::string file_path = "C:\\DreamHack\\" + file_name + ".cfg";

		std::fstream file(file_path, std::ios::out | std::ios::in | std::ios::trunc);
		file.close();

		file.open(file_path, std::ios::out | std::ios::in);
		if (!file.is_open())
		{
			file.close();
			return false;
		}

		const size_t settings_size = sizeof(CSettings);
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
	bool CSettings::Load(std::string file_name)
	{
		CreateDirectory("C:\\DreamHack", NULL);

		std::string file_path = "C:\\DreamHack\\" + file_name + ".cfg";

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

			const size_t settings_size = sizeof(CSettings);
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
	void CSettings::CreateConfig(std::string name)
	{
		CreateDirectory("C:\\DreamHack\\", NULL); CreateDirectory("C:\\DreamHack\\", NULL);
		std::ofstream ofs("C:\\DreamHack\\" + name + ".cfg");
	}

	bool CSettings::Remove(std::string file_name)
	{
		CreateDirectory("C:\\DreamHack", NULL);

		std::string file_path = "C:\\DreamHack\\" + file_name + ".cfg";
		remove(file_path.c_str());

		return true;
	}

	std::vector<std::string> CSettings::GetConfigs()
	{
		std::vector<std::string> configs;

		WIN32_FIND_DATA ffd;
		auto directory = "C:\\DreamHack\\*";
		auto hFind = FindFirstFile(directory, &ffd);

		while (FindNextFile(hFind, &ffd))
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string file_name = ffd.cFileName;
				if (file_name.size() < 4) // .cfg
					continue;

				std::string end = file_name;
				end.erase(end.begin(), end.end() - 4); // erase everything but the last 4 letters
				if (end != ".cfg")
					continue;

				file_name.erase(file_name.end() - 4, file_name.end()); // erase the .cfg part
				configs.push_back(file_name);
			}
		}

		return configs;
	}
}
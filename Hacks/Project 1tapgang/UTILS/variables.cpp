#include "../includes.h"

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

	Vector real_angles;
	Vector fake_angles;
	Vector strafe_angle;

	int randomnumber;
	float	flHurtTime;
	 Vector freestanding;
	 Vector freestanding2;
	 Vector freestanding3;
 Vector freestanding4;
	Vector finaleye;
	bool DisableAA;
	bool Aimbotting;
	Vector recoilframe;
	using msg_t = void(__cdecl*)(const char*, ...);
	msg_t Msg = reinterpret_cast< msg_t >(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

	Vector FakePosition;
	int ground_tickz;
	bool CircleStraferActive;
	SDK::CUserCmd originalCMD;
	SDK::CUserCmd* cmd;

	bool disable_fakelag;
}
namespace FONTS
{
	unsigned int font_esp_flags;
	unsigned int font_indicators;
	unsigned int font_watermark;
	unsigned int visuals_esp_font;
	unsigned int font_esp_name;
	unsigned int visuals_lby_font;
	unsigned int font_weapon_icon;
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
		font_esp_flags = RENDER::CreateF("Smallest Pixel-7", 11, 400, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		font_indicators = RENDER::CreateF("Tahoma", 16, 500, 0, 0, SDK::FONTFLAG_ANTIALIAS);
		font_watermark = RENDER::CreateF("Tahoma", 12, 500, 0, 0, SDK::FONTFLAG_DROPSHADOW);
		visuals_esp_font = RENDER::CreateF("Smallest Pixel-7", 10, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		font_esp_name = RENDER::CreateF("Verdana", 12, 400, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		visuals_lby_font = RENDER::CreateF("Verdana", 25, 500, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		font_weapon_icon = RENDER::CreateF("astriumwep", 16, 400, 0, 0, SDK::FONTFLAG_DROPSHADOW | SDK::FONTFLAG_ANTIALIAS);
	}
}
namespace SETTINGS
{
	CSettings settings;

	bool CSettings::Save(std::string file_name)
	{
		std::string file_path = "C:\\nicetap\\" + file_name + ".cfg";

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
		CreateDirectory("C:\\nicetap", NULL);

		std::string file_path = "C:\\nicetap\\" + file_name + ".cfg";

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
		CreateDirectory("C:\\nicetap\\", NULL); CreateDirectory("C:\\nicetap\\", NULL);
		std::ofstream ofs("C:\\nicetap\\" + name + ".cfg");
	}

	bool CSettings::Remove(std::string file_name)
	{
		CreateDirectory("C:\\nicetap", NULL);

		std::string file_path = "C:\\nicetap\\" + file_name + ".cfg";
		remove(file_path.c_str());

		return true;
	}

	std::vector<std::string> CSettings::GetConfigs()
	{
		std::vector<std::string> configs;

		WIN32_FIND_DATA ffd;
		auto directory = "C:\\nicetap\\*";
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
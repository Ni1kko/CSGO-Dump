#include "newgui.h"
#include "stdafx.h"
#include <chrono>
#include "Utils/Utils.h"
#include "cfg.h"

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}
void CConfig::SetupValue(float* &value, float* def, std::string category, std::string name)
{
	value = def;
	colors.push_back(new ConfigValue<float*>(category, name, &value));
}

void CConfig::ReadConfigs(LPCTSTR lpszFileName)
{
	ConfigList.push_back(lpszFileName);
}

string CConfig::GetModuleFilePath(HMODULE hModule)
{
	string ModuleName = "";
	char szFileName[MAX_PATH] = { 0 };

	if (GetModuleFileNameA(hModule, szFileName, MAX_PATH))
		ModuleName = szFileName;

	return ModuleName;
}

string CConfig::GetModuleBaseDir(HMODULE hModule)
{
	string ModulePath = GetModuleFilePath(hModule);
	return ModulePath.substr(0, ModulePath.find_last_of("\\/"));
}

void CConfig::Save(string cfg_name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\OneWay.cc [Closed Beta Test]\\");
		file = std::string(path) + ("\\OneWay.cc [Closed Beta Test]\\" + cfg_name);
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(string cfg_name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\OneWay.cc [Closed Beta Test]\\");
		file = std::string(path) + ("\\OneWay.cc [Closed Beta Test]\\" + cfg_name);
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig Config;

const char* KeyStrings[] = {
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};

vector<string> ConfigList;
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = FALSE)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					SearchFiles(next, lpSearchFunc, TRUE);
				}
			} while (FindNextFile(hSearch, &wfd));
			FindClose(hSearch);
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(wfd.cFileName);
		}
	while (FindNextFile(hSearch, &wfd));
	FindClose(hSearch);
	return TRUE;
}

void ReadConfigs(LPCTSTR lpszFileName)
{
	ConfigList.push_back(lpszFileName);
}

void RefreshConfigs()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		ConfigList.clear();
		string ConfigDir = std::string(path) + "\\OneWay.cc [Closed Beta Test]\\*";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}
}

void drawMenu()
{
	static bool f = true;
	if (!f)
	{
		RefreshConfigs();
		f = false;
	}

	static int tab_count = 0;

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar);

	if (ImGui::SmallButton("HVH"))
		tab_count = 1;
	ImGui::SameLine();
	if (ImGui::SmallButton("ESP"))
		tab_count = 2;
	ImGui::SameLine();
	if (ImGui::SmallButton("MISC"))
		tab_count = 3;
	ImGui::SameLine();
	if (ImGui::SmallButton("CFG"))
		tab_count = 4;

	switch (tab_count)
	{
	case 1:
	{
		ImGui::Checkbox("Enable", &Vars.Ragebot.Enable);
		ImGui::Checkbox("Silent", &Vars.Ragebot.Silent);
		ImGui::Checkbox("Auto fire", &Vars.Ragebot.AutoFire);
		ImGui::Checkbox("No recoil", &Vars.Ragebot.Accuracy.RemoveRecoil);
		ImGui::Checkbox("Knife bot", &Vars.Misc.Knifebot);
		ImGui::Checkbox("Multipoint", &Vars.Ragebot.Hitscan.Multipoint);

		static const char* Priority[] =
		{
			"Head",
			"Neck",
			"Chest",
			"Pelvis",
			"Stomach"
		};
		ImGui::Combo("Priority hitbox", &Vars.Ragebot.Hitscan.PriorityHitbox, Priority, ARRAYSIZE(Priority));

		static const char* Multibox[] =
		{
			"Off",
			"Low",
			"High"
		};
		ImGui::Combo("Hitscan", &Vars.Ragebot.Hitscan.Multibox, Multibox, ARRAYSIZE(Multibox));

		ImGui::SliderFloat("Hitchance", &Vars.Ragebot.Accuracy.Hitchance, 0, 100, "%.1f");
		ImGui::SliderFloat("Minimum damage", &Vars.Ragebot.Accuracy.Mindamage, 1, 100, "%.1f");

		ImGui::Checkbox("Enable", &Vars.Ragebot.Antiaim.Enable);
		static const char* PitchAA[] =
		{
			"Off",
			"Emotion",
			"Fake up",
			"Fake down",
			"Fake zero"
		};
		ImGui::Combo("Pitch", &Vars.Ragebot.Antiaim.Pitch, PitchAA, ARRAYSIZE(PitchAA));

		ImGui::Checkbox("Desync", &Vars.Ragebot.Antiaim.Desync);
		ImGui::Checkbox("Freestand", &Vars.Ragebot.Antiaim.Freestand);
		static const char* YawAA[] =
		{
			"Off",
			"Backward",
			"Backward jitter",
			"180z",
			"Random"
		};
		ImGui::Combo("Yaw", &Vars.Ragebot.Antiaim.Yaw, YawAA, ARRAYSIZE(YawAA));

		static const char* FakelagType[] =
		{
			"Off", "Default", "Adaptive"
		};
		ImGui::Combo("Fakelags", &Vars.Ragebot.Antiaim.Fakelag.Type, FakelagType, ARRAYSIZE(FakelagType));
	}
	break;
	case 2:
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Box", &Vars.Visuals.Player.Box);
		ImGui::SameLine();
		ImGui::ColorEdit4("###boxcolor", Vars.Visuals.Colors.Box);

		ImGui::Checkbox("Snap lines", &Vars.Visuals.Player.SnapLines);
		ImGui::SameLine();
		ImGui::ColorEdit4("###SnapLinescolor", Vars.Visuals.Colors.SnapLines);

		ImGui::Checkbox("Skeleton", &Vars.Visuals.Player.Skeleton);
		ImGui::SameLine();
		ImGui::ColorEdit4("###Skeletoncolor", Vars.Visuals.Colors.Skeleton);

		ImGui::Checkbox("Bullet tracer", &Vars.Visuals.Other.BulletTracers);
		ImGui::SameLine();
		ImGui::ColorEdit4("###BulletTracerscolor", Vars.Visuals.Colors.BulletTracers);

		ImGui::Checkbox("Out of Fov Radar", &Vars.Visuals.Other.FovArrows_Enable);
		ImGui::SameLine();
		ImGui::ColorEdit4("###OutofFovColor", Vars.Visuals.Colors.FovArrows);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Health", &Vars.Visuals.Player.Health);
		ImGui::Checkbox("Name", &Vars.Visuals.Player.Name);
		ImGui::Checkbox("Weapon", &Vars.Visuals.Player.Weapon);
		ImGui::Checkbox("Armor", &Vars.Visuals.Player.Armor);
		ImGui::Checkbox("Engine Radar", &Vars.Visuals.Other.Radar);
		ImGui::Checkbox("Dropped weapons", &Vars.Visuals.DroppedWeapons);
		ImGui::Checkbox("C4", &Vars.Visuals.C4);
		ImGui::Checkbox("Hitmarker", &Vars.Visuals.Other.Hitmarker);
		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::Checkbox("Glow", &Vars.Visuals.Player.Glow);
		ImGui::SameLine();
		ImGui::ColorEdit4("###GlowColor", Vars.Visuals.Colors.Glow);

		ImGui::Checkbox("Glow LocalPlayer", &Vars.Visuals.Player.LocalGlow);
		ImGui::SameLine();
		ImGui::Checkbox("Pulse Glow", &Vars.Visuals.Player.PulseLocalGlow);
		ImGui::SameLine();
		ImGui::ColorEdit4("###localGlowColor", Vars.Visuals.Colors.LocalGlow);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::PushItemWidth(130);
		ImGui::Combo("Thirdperson", &Vars.Visuals.Other.ThirdPersonKey, KeyStrings, ARRAYSIZE(KeyStrings));
		ImGui::Checkbox("Night-mode", &Vars.Visuals.Other.Nightmode);
		ImGui::SliderFloat("World FOV", &Vars.Misc.FOV, -90, 90, "%.1f");
		ImGui::PopItemWidth();
		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::Checkbox("No visual recoil", &Vars.Visuals.Effects.NoVisRecoil);
		ImGui::Checkbox("No flash", &Vars.Visuals.Effects.NoFlash);
		ImGui::Checkbox("No smoke", &Vars.Visuals.Effects.NoSmoke);
		ImGui::Checkbox("No scope", &Vars.Visuals.Effects.NoScope);
		ImGui::Checkbox("No zoom", &Vars.Visuals.Effects.NoZoom);
		ImGui::Checkbox("No post processing", &Vars.Visuals.Effects.NoPostProcessing);
		ImGui::EndGroup();
	}
	break;
	case 3:
		ImGui::Checkbox("Anti untrusted", &Vars.Misc.AntiUT);
		//ImGui::Checkbox("MM Mode", &Vars.Misc.MM_Mode);
		ImGui::Checkbox("Bunnyhop", &Vars.Misc.Bunnyhop);
		ImGui::Checkbox("Auto strafe", &Vars.Misc.Strafers.Enable);
		break;
	case 4: {
		ImGui::Text("Manage config");

		ImGui::PushItemWidth(175.f);
		static int iConfigSelect = 0;
		static char ConfigName[64] = { 0 };
		if (ConfigList.size() > 0)
		{
			ImGui::Text("Configs: %s", ConfigList[iConfigSelect].c_str());
		}

		if (ImGui::Button("Refresh"))
		{
			RefreshConfigs();
		}
		ImGui::InputText("Name", ConfigName, 64);

		if (ImGui::Button("Create"))
		{
			Config.Save(ConfigName);
			RefreshConfigs();
		}

		ImGui::Text("List of configs");

		if (ConfigList.size() > 0)
		{
			for (int g = 0; g < ConfigList.size(); g++)
			{
				if (iConfigSelect == g)
					ImGui::GetStyle().Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]; //main
				else
					ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f); //main
				if (ImGui::Button(ConfigList[g].c_str()))
					iConfigSelect = g;

			}
			ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f); //main
		}

		if (ConfigList.size() > 0)
		{
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				Config.Save(ConfigList[iConfigSelect].c_str());
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				Config.Load(ConfigList[iConfigSelect].c_str());
			}
		}
	}
		break;
	}
}

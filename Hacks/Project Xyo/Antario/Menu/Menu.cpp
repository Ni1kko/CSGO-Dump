#include "Menu.h"
#include "TGFCfg.h"
#include "../SDK/Vector.h"
#include "../SDK/ISurface.h"
#include "../Utils/Color.h"
#include "../Utils/GlobalVars.h"
#include "config.h"
Menu g_Menu;

bool manually_set_key = false;

/*
*
*     CREDITS TO : stickrpg
*
*	BEST MENU FRAMEWORK IN EXISTENCE
*
*/
enum TABS
{
	Aimbot = 0,
	Visuals = 1,
	Misc = 2
};

#define or ||
#define and &&
int TopLeftGroupBox_X = 5;
int TopLeftGroupBox_Y = 52 + (309 / 1.5) + 5;

int GroupBoxSize_Width = 375 / 1.5;
int GroupBoxSize_Height = 309 / 1.5;


void GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void GradientVEnd(int x, int y, int w, int h, Color c1, Color c2)
{
	//g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (float i = 0; i < h / 1.5; i++)
	{
		float fi = i, fh = h / 1.5;
		float a = fi / fh;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x, y + i, w, 2, Color(first, second, third, ia));
	}
}


void DrawESPPlayersSubTab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = (375 / 1.5) - 8;
	static std::string ESP_Player_SubTabs_Names[3] = {"Main", "Main PT 2", "Model"};

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = g_pSurface->GetTextSizeRect(Globals::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 52 + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
		                                                              tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
}

void DrawAimbotTargetSubTab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = (375 / 1.5) - 8;
	static std::string ESP_Player_SubTabs_Names[2] = {"Target", "Body-Aim"};

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = g_pSurface->GetTextSizeRect(Globals::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + TopLeftGroupBox_Y + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
		                                                              tab_area[3]))
			current_players_esp_subtab = i;


		//	g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(23, 23, 25, MenuAlpha_Main));

		if (current_players_esp_subtab == i)
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}

	}
} //

void DrawAimbotAccuracy_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width - 8;
	static std::string ESP_Player_SubTabs_Names[5] = {"Auto", "Scout", "Awp", "H. Pistol", "Other"};

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = g_pSurface->GetTextSizeRect(Globals::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + TopLeftGroupBox_X + GroupBoxSize_Width + 5 + 4 + (i * (in_sizew_esp_player_subtabs / tab_amount)),
			_pos.y + 52 + 5, (in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
		                                                              tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
}

void DrawAntiAim_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width - 8;
	static std::string ESP_Player_SubTabs_Names[4] = {"Standing", "Moving", "Air", "Misc"};

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = g_pSurface->GetTextSizeRect(Globals::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 52 + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
		                                                              tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
} //
#include <tuple>

class vars : public singleton< vars > {
private:

public:
	bool visuals_box = false;
};

void DrawAimbotMain_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width - 8;
	static std::string ESP_Player_SubTabs_Names[2] = {"Main", "Resolver"};

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = g_pSurface->GetTextSizeRect(Globals::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 52 + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
		                                                              tab_area[3]))
			current_players_esp_subtab = i;

		

		if (current_players_esp_subtab == i)
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
			                  tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
			                  Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
			                  ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));
			
			g_pSurface->DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
			                  tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
			                  Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
			                  ESP_Player_SubTabs_Names[i].c_str());
		}

	//	g_pSurface->OutlinedRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(40, 44, 53, MenuAlpha_Main));
	}
} //

std::ptrdiff_t ExtraOffsets::deadflag = NULL;
std::string config_slots[14] = {
	"Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot", "Golf", "Hotel", "India", "Juilett", "Kilo", "Lima",
	"Mike", "November"
};

static int save_slot = 0;
#include "../Features/Visuals/EventLogging.h"
void Save_Config()
{
	Config2->Save(config_slots[save_slot]);

	std::string string_to_add;
	string_to_add += "saved '";
	string_to_add += config_slots[save_slot];
	string_to_add += "'.";

	c_event_logs::get().add(string_to_add, Color(255, 255, 255, 255));
}

void Load_Config()
{
	Config2->Load(config_slots[save_slot]);

	std::string string_to_add;
	string_to_add += "loaded '";
	string_to_add += config_slots[save_slot];
	string_to_add += "'.";

	c_event_logs::get().add(string_to_add, Color(255, 255, 255, 255));
}

void Export_Config()
{
	Config2->export_to_clipboard("export");
}

void Import_Config()
{
	Config2->import_from_clipboard("import");
}

void Save()
{
	std::string ConfigName;

	switch (2)
	{
	case 0: ConfigName = "1";
		break;
	case 1: ConfigName = "2";
		break;
	case 2: ConfigName = "3";
		break;
	case 3: ConfigName = "4";
		break;
	case 4: ConfigName = "5";
		break;
	}
	Config2->Save(ConfigName);
}

void GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x + i, y, 1, h, Color(first, second, third, ia));
	}
}


#include <filesystem>
#include <iostream>

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
#include "../SDK/ClientClass.h"
#include "../SDK/IBaseClientDll.h"


DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
{
	if (!dwAddress || !dwLength || !szPattern)
		return 0;

	const char* pat = szPattern;
	DWORD firstMatch = NULL;

	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else pat += 2;
		}
		else
		{
			pat = szPattern;
			firstMatch = 0;
		}
	}

	return 0;
}
DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern)
{
	HMODULE hModule = GetModuleHandleA(szModuleName);
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

	DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

	return ret;
}

void ForceDaUpdate() {

}

std::string text_uwu;
bool dont_recieve_input = false;

void Menu::Render()
{
	static bool _pressed = true;

	if (!_pressed && GetAsyncKeyState(VK_INSERT))
		_pressed = true;
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		_pressed = false;
		menuOpened = !menuOpened;
	}
	g_pEngine->GetScreenSize(screen_width, screen_width);
	static Vector2D _mouse_pos;
	static int groupbox_bottom;
	static int _drag_x = 300;
	static int _drag_y = 300;
	int _width = 514;
	int _height = 473; //
	static Vector2D oldPos;
	static Vector2D mousePos;
	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;

	g_InputSystem->EnableInput(!menuOpened);
	if (menuOpened)
	{
		MenuAlpha_Main = min(MenuAlpha_Main + 15, 255);
		MenuAlpha_Text = min(MenuAlpha_Text + 5, 255);

		//MenuAlpha_Main = 255;
		//MenuAlpha_Text = 255;

		if (GetAsyncKeyState(VK_LBUTTON))
			_click = true;

		Vector2D _mouse_pos = g_pSurface->GetMousePosition();

		if (_dragging and !_click)
			_dragging = false;

		if (_resizing and !_click)
			_resizing = false;

		if (_dragging and _click)
		{
			_pos.x = _mouse_pos.x - _drag_x;
			_pos.y = _mouse_pos.y - _drag_y;
		}


		//if (_resizing && _click) {
		//	_width = _mouse_pos.x - _pos.x;
		//	_height = _mouse_pos.y - _pos.y;

		//	if (_width < 480) _width = 480;
		//	if (_height < 340) _height = 340;

		//	if (_width > 960) _width = 960;
		//	if (_height > 680) _height = 680;
		//}


		if (g_pSurface->MouseInRegion(_pos.x, _pos.y, _width, _height) && !(g_pSurface->MouseInRegion(
			_pos.x + 10, _pos.y + 20, _width - 10, _height - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - _pos.x;
			_drag_y = _mouse_pos.y - _pos.y;
		}

		//	if (mouse_in_params(_pos.x + 20, _pos.y + 20, _width - 10, _height - 10))
		//	{
		//		_resizing = true;
		//	}
	}
	else
	{
		MenuAlpha_Main = max(MenuAlpha_Main - 15, 0);
		MenuAlpha_Text = max(MenuAlpha_Text - 25, 0);
		//MenuAlpha_Main = 0;
		//MenuAlpha_Text = 0;
	}

	//if (!menuOpened)
	//	return;


	if (menuOpened) {
		GradientH(_pos.x, _pos.y, _width, _height, Color(29, 29, 37, MenuAlpha_Main), Color(49, 49, 57, MenuAlpha_Main));
	}
	else {
		g_pSurface->FilledRect(_pos.x, _pos.y, _width, _height, Color(29, 29, 37, MenuAlpha_Main));
	}

	int sum = 0;
	int x;
	ifstream inFile;

	string  path;
	static string username;
	static string rent;

	static bool GrabName = true;

	if (GrabName) {
		path = getenv("appdata");
		//cout << "appdata=" << path << endl;
		path += "\\xy0_login";

		inFile.open(path);

		inFile >> username >> rent;
		cout << username << rent << endl;

		GrabName = false;
	}

	//cout << name << rent << endl;
	//cout << "path=" << path << endl;

	g_pSurface->FilledRect(_pos.x + 1, _pos.y + 20, _width - 2, _height - 20, Color(23, 23, 30, MenuAlpha_Main));




	for (int i = 0; i < _width / 2; i++) {
		g_pSurface->Line(_pos.x + (i * 2), _pos.y + 22, _pos.x + (i * 2), _pos.y + _height, Color(10, 10, 18, MenuAlpha_Main));
	}

	for (int i = 0; i < _height / 2.1; i++) {
		g_pSurface->Line(_pos.x, _pos.y + 22 + (i * 2), _pos.x + _width, _pos.y + 22 + (i * 2), Color(10, 10, 18, MenuAlpha_Main));
	}

	g_pSurface->FilledRect(_pos.x, _pos.y + 40, _width, 3, Color(39, 39, 47, MenuAlpha_Main));

	g_pSurface->OutlinedRect(_pos.x, _pos.y, _width, _height, Color(65, 63, 79, MenuAlpha_Main));
	g_pSurface->OutlinedRect(_pos.x, _pos.y + 19, _width, 1, Color(65, 63, 79, MenuAlpha_Main));

	RECT TitleText_Size = g_pSurface->GetTextSizeRect(Globals::CourierNew, "Geminisoftware.us");

	g_pSurface->DrawT(_pos.x + 3, _pos.y + (20 / 2) - (TitleText_Size.bottom / 2), Color(143, 143, 143, MenuAlpha_Text), Globals::CourierNew, false, "Geminisoftware.us");

	std::string MenuWatermark;

	MenuWatermark += username;
#if _BETA
	MenuWatermark += " - ";
	MenuWatermark += "Beta";
#endif

	RECT WatermarkMenuText_Size = g_pSurface->GetTextSizeRect(Globals::CourierNew, MenuWatermark.c_str());

	g_pSurface->DrawT(_pos.x + _width - WatermarkMenuText_Size.right - 5, _pos.y + (20 / 2) - (TitleText_Size.bottom / 2), Color(143, 143, 143, MenuAlpha_Text), Globals::CourierNew, false, MenuWatermark.c_str());


	int in_sizew = _width - 1;

	static int tab_selected = 0;
	static const int tab_amount = 6;
	static std::string tab_names[tab_amount] = { "Legit", "Rage", "AA", "Visuals", "Misc", "Skins" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size = g_pSurface->GetTextSizeRect(Globals::TabFont, tab_names[i].c_str());

		int tab_area[4] = { _pos.x + 1 + (i * (in_sizew / tab_amount)), _pos.y + 20, (in_sizew / tab_amount), 20 };

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			tab_selected = i;

		if (tab_selected == i)
		{
			/*

			g_pSurface->FilledRect(tab_area[0], tab_area[1], 1, tab_area[3], Color(40, 44, 53, 255));


			g_pSurface->FilledRect(tab_area[0] + tab_area[2], tab_area[1], 1, tab_area[3], Color(40, 44, 53, 255));

			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], 1, Color(40, 44, 53, 255));


			*/

			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			g_pSurface->FilledRect(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3,  Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main));

			g_pSurface->DrawT(tab_area[0] + (((in_sizew / tab_amount) / 2) - (text_size.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), Color(143, 143, 143, MenuAlpha_Text),
				Globals::TabFont, false, tab_names[i].c_str());
		}
		else
		{
			//g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(33, 33, 35, MenuAlpha_Main));
			g_pSurface->FilledRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			g_pSurface->DrawT(tab_area[0] + (((in_sizew / tab_amount) / 2) - (text_size.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), Color(143, 143, 143, MenuAlpha_Text),
				Globals::TabFont, false, tab_names[i].c_str());
		}


		//g_pSurface->Line(tab_area[0] - 10, tab_area[1] + 5, tab_area[0] - 13, tab_area[1] + tab_area[3] - 10, Color(91, 95, 97, MenuAlpha_Main));
	}
	static int aimbot_main_subtab = 0;
	std::string pitch[5] = { "Disabled", "Down", "Up", "Origin", "Offset" };

	std::string BodyAimOptions[5] = { "In Air", "Slow Walk", "High Inaccuracy", "Vulnerable", "Lethal" };

	std::string AutoStopOptions[4] = { "Minimum Speed", "Full Stop" };

	std::string BodyAimModeOptions[2] = { "Prefer", "Force" };

	if (tab_selected == 0)
	{
		groupbox(5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test");
		checkbox("Enabled", &c_config::get().legit_aimbot_enabled);
		checkbox("Position Adjustment", &c_config::get().legit_aimbot_backtrack);
		slider(180, "FOV", &c_config::get().legit_aimbot_fov, "", 1);
		slider(100, "Linear Progression Threshold", &c_config::get().linear_progression_threshold, "", 1);
		checkbox("RCS", &c_config::get().rcs);
		slider(100, "RCS X", &c_config::get().rcs_x, "", 1);
		slider(100, "RCS Y", &c_config::get().rcs_y, "", 1);
	}
	else if (tab_selected == 1)
	{
		groupbox(5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test", false, true);
		DrawAimbotMain_Subtab(aimbot_main_subtab, 2, _pos, MenuAlpha_Main);

		if (aimbot_main_subtab == 0)
		{
			checkbox("Enabled", &c_config::get().aimbot_enabled);
			checkbox("Silent Aim", &c_config::get().aimbot_silentaim);
			checkbox("Remove Recoil", &c_config::get().aimbot_norecoil);
			checkbox("Fakelag Prediction", &c_config::get().fakelag_prediction);
			checkbox("Auto Stop", &c_config::get().autostop);
			if (c_config::get().autostop) combobox(2, "", AutoStopOptions, &c_config::get().autostop_mode);
			if (c_config::get().autostop) checkbox("Stop Between Shots", &c_config::get().stop_inbetween_shots);
			checkbox("Auto Scope", &c_config::get().auto_scope);
			checkbox("Auto Revolver", &c_config::get().autorevolver);
			checkbox("Accuracy Boost", &c_config::get().accuracy_boost);
		}
		else if (aimbot_main_subtab == 1)
		{
			checkbox("Resolver", &c_config::get().aimbot_resolver);
		}

		groupbox(TopLeftGroupBox_X, TopLeftGroupBox_Y, GroupBoxSize_Width, GroupBoxSize_Height, "test", false, true);
		static int current_target_selection_aimbots = 0;
		DrawAimbotTargetSubTab(current_target_selection_aimbots, 2, _pos, MenuAlpha_Main);

		switch (current_target_selection_aimbots)
		{
		case 0: /*target*/
		{
			slider(100, "Head Scale", &c_config::get().aimbot_headpointscale, "%", 1);
			slider(100, "Body Scale", &c_config::get().aimbot_bodypointscale, "%", 1);
		}
		break;
		case 1: /*hitscan*/
		{			
			MultiComboBox(5, "Body-Aim", BodyAimOptions, c_config::get().prefer_bodyaim);
			combobox(2, "Body-Aim Mode", BodyAimModeOptions, &c_config::get().bodyaim_mode);
		}
		break;
		}

		static int current_subtab_selection_accuracy = 0;
		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, 52, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, "test", false,
			true);
		DrawAimbotAccuracy_Subtab(current_subtab_selection_accuracy, 5, _pos, MenuAlpha_Main);

		switch (current_subtab_selection_accuracy)
		{
		case 0:
		{
			slider(100, "Hitchance", &c_config::get().auto_hitchance, "%", 1);
			slider(100, "Minimum Damage", &c_config::get().auto_mindamage, "HP", 1);
		}
		break;
		case 1:
		{
			slider(100, "Hitchance", &c_config::get().scout_hitchance, "%", 1);
			slider(100, "Minimum Damage", &c_config::get().scout_mindamage, "HP", 1);
		}
		break;
		case 2:
		{
			slider(100, "Hitchance", &c_config::get().awp_hitchance, "%", 1);
			slider(100, "Minimum Damage", &c_config::get().awp_mindamage, "HP", 1);
		}
		break;
		case 3:
		{
			slider(100, "Hitchance", &c_config::get().heavy_pistol_hitchance, "%", 1);
			slider(100, "Minimum Damage", &c_config::get().heavy_pistol_mindamage, "HP", 1);
		}
		break;
		case 4:
		{
			slider(100, "Hitchance", &c_config::get().other_hitchance, "%", 1);
			slider(100, "Minimum Damage", &c_config::get().other_mindamage, "HP", 1);
		}
		break;
		}


	}
	else if (tab_selected == 2) {


		static int current_subtab_antiaim_selection = 0;
		groupbox(5, 52, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5,
			"test", false, true);
		DrawAntiAim_Subtab(current_subtab_antiaim_selection, 4, _pos, MenuAlpha_Main);
		std::string pitch[5] = { "Disabled", "Down", "Up", "Origin", "Offset" };
		std::string yaw[5] = { "Disabled", "Backwards", "Origin", "Spin", "Manual" };
		std::string on_shot[2] = { "Stable", "Experimental" };
		std::string desync[5] = { "Disabled", "Ephemeral", "Cromulent", "Ornery", "Jitter" };
		std::string freestand_mode[3] = { "Threat", "Crosshair", "Distance" };
		switch (current_subtab_antiaim_selection)
		{
		case 0:
		{
			combobox(5, "X", pitch, &c_config::get().pitch_standing);
			combobox(5, "Y", yaw, &c_config::get().yaw_standing);
		}
		break;
		case 1:
		{
			combobox(5, "X", pitch, &c_config::get().pitch_moving);
			combobox(5, "Y", yaw, &c_config::get().yaw_moving);
		}
		break;
		case 2:
		{
			combobox(5, "X", pitch, &c_config::get().pitch_air);
			combobox(5, "Y", yaw, &c_config::get().yaw_air);
		}
		break;
		case 3:
		{
			checkbox("Enabled", &c_config::get().antiaim_enabled);
			checkbox("Auto Direction", &c_config::get().antiaim_freestanding);
			if (c_config::get().antiaim_freestanding) {
				combobox(3, "", freestand_mode, &c_config::get().antiaim_freestanding_mode);
			}
			checkbox("Extrapolated", &c_config::get().extrapolated);
			combobox(5, "Desync", desync, &c_config::get().desync);
			checkbox("On Shot Anti-Aim", &c_config::get().on_shot_aa);
			if (c_config::get().on_shot_aa)
			{
				combobox(2, "On Shot AA Type", on_shot, &c_config::get().on_shot_aa_type);
			}

			slider(180, "Angle Jitter", &c_config::get().aa_jitter, "°", 1);
		}
		break;
		}

		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, 52, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, "test");

		slider(14, "Fakelag", &c_config::get().fakelag, "t", 1);
		
		//slider(100, "Fakelag Randomization", &c_config::get().variance, "%", 1);
		//slider(14, "Fake Duck", &c_config::get().fakeduck_test, "t", 1);
	}
	else if (tab_selected == 3)
	{
		static int current_players_esp_subtab = 0;

		std::string WireFrameHands[3] = { "Disabled", "Invisible", "Chams"};
		std::string EventLogsOptions[5] = { "Weapon Purchase", "Player Hurt", "Player Killed" };
		groupbox(TopLeftGroupBox_X, TopLeftGroupBox_Y, GroupBoxSize_Width, GroupBoxSize_Height, "test");
		MultiComboBox(3, "Event Logger", EventLogsOptions, c_config::get().event_logger);

		groupbox(5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test", false, true);
		DrawESPPlayersSubTab(current_players_esp_subtab, 3, _pos, MenuAlpha_Main);

		std::string Health_Type[3] = { "Disabled", "Normal", "Battery" };
		std::string Hitmarker_Type[3] = { "Disabled", "Arena Switch", "Custom" };
		if (current_players_esp_subtab == 0)
		{
			checkbox("Enabled", &c_config::get().visuals_enabled);
			color_selector("name_col", &c_config::get().name_esp_color_r, &c_config::get().name_esp_color_g,
				&c_config::get().name_esp_color_b, &c_config::get().name_esp_color_a);
			checkbox("Name", &c_config::get().name_enemies);
			color_selector("box_col", &c_config::get().box_esp_color_r, &c_config::get().box_esp_color_g,
				&c_config::get().box_esp_color_b, &c_config::get().box_esp_color_a);
			checkbox("Bounding Box", &c_config::get().box_enemies);
			combobox(3, "Health", Health_Type, &c_config::get().health_enemies);
			color_selector("weapon_col", &c_config::get().weapon_esp_color_r, &c_config::get().weapon_esp_color_g,
				&c_config::get().weapon_esp_color_b, &c_config::get().weapon_esp_color_a);
			checkbox("Weapon", &c_config::get().weapon_enemies);
			if (c_config::get().weapon_enemies)
			{
				checkbox("Show Icon When Possible", &c_config::get().show_icon_when_possible_enemies);
			}
			checkbox("Flags", &c_config::get().flags_enemy);
			checkbox("Ammo", &c_config::get().ammo_enemy);
		}
		else if (current_players_esp_subtab == 1)
		{
			color_selector("player_indicator_col", &c_config::get().fov_arrows_esp_color_r,
				&c_config::get().fov_arrows_esp_color_g, &c_config::get().fov_arrows_esp_color_b,
				&c_config::get().fov_arrows_esp_color_a);
			checkbox("Player Indicator", &c_config::get().fov_arrows_enemy);
			if (c_config::get().fov_arrows_enemy)
			{
				slider(30, "Size", &c_config::get().fov_arrows_enemy_size, "px", 1);
				slider(100, "Distance", &c_config::get().fov_arrows_enemy_distance, "%", 1);
				checkbox("Always indicate player", &c_config::get().fov_arrows_always_indicate);
			}
		}
		else if (current_players_esp_subtab == 2)
		{
			color_selector("glow_col", &c_config::get().glow_esp_color_r, &c_config::get().glow_esp_color_g,
				&c_config::get().glow_esp_color_b, &c_config::get().glow_esp_color_a);
			checkbox("Glow", &c_config::get().glow_enemies);
			color_selector("chams_col", &c_config::get().chams_esp_color_r, &c_config::get().chams_esp_color_g,
				&c_config::get().chams_esp_color_b, &c_config::get().chams_esp_color_a);
			checkbox("Chams", &c_config::get().chams_enemies);
			color_selector("chams_xqz_col", &c_config::get().chams_xqz_esp_color_r,
				&c_config::get().chams_xqz_esp_color_g, &c_config::get().chams_xqz_esp_color_b,
				&c_config::get().chams_xqz_esp_color_a);
			checkbox("Chams XQZ", &c_config::get().chams_xqz_enemies);
			color_selector("hand_cham_col", &c_config::get().hand_chams_color_r, &c_config::get().hand_chams_color_g,
				&c_config::get().hand_chams_color_b, &c_config::get().hand_chams_color_a);
			combobox(3, "Hands", WireFrameHands, &c_config::get().hand_chams);
			color_selector("local_cham_col", &c_config::get().local_chams_color_r, &c_config::get().local_chams_color_g,
				&c_config::get().local_chams_color_b, &c_config::get().local_chams_color_a);
			checkbox("Local Chams", &c_config::get().local_chams);
			color_selector("local_glow_col", &c_config::get().local_glow_color_r, &c_config::get().local_glow_color_g,
				&c_config::get().local_glow_color_b, &c_config::get().local_glow_color_a);
			checkbox("Local Glow", &c_config::get().local_glow);
		}


		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test");

		checkbox("Remove Scope", &c_config::get().remove_scope);
		if (c_config::get().remove_scope)
		{
			checkbox("Display spread with scope", &c_config::get().dynamic_scope);
		}
		checkbox("Hit Marker", &c_config::get().hitmarker);
		combobox(3, "Hit Marker Sound", Hitmarker_Type, &c_config::get().hitmarker_sound);
		
		checkbox("Force Crosshair", &c_config::get().force_crosshair);
		checkbox("Preserve Killfeed", &c_config::get().visual_preserve_killfeed);
		checkbox("Penetration Crosshair", &c_config::get().penetration_crosshair);
		checkbox("Radar", &c_config::get().visuals_radar);
		checkbox("Grenade Prediction", &c_config::get().grenade_prediction);
		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, TopLeftGroupBox_Y, GroupBoxSize_Width, GroupBoxSize_Height,
			"test");
		slider(135, "Field Of View", &c_config::get().fov, "°", 1);
		checkbox("Retain FOV When Scoped", &c_config::get().fov_while_zoomed);
		slider(135, "Viewmodel Field Of View", &c_config::get().viewmodel_fov, "°", 1);

		checkbox("Nightmode", &c_config::get().nightmode);
		checkbox("Transparent Props", &c_config::get().transparent_props);
		slider(135, "Thirdperson Distance", &c_config::get().thirdperson_distance, "°", 1);
		checkbox("Remove Visual Recoil", &c_config::get().remove_visual_recoil);
		std::string nosmoke_type[3] = { "Disabled", "No Draw", "Wireframe" };
		checkbox("No Smoke", &c_config::get().no_smoke);
		checkbox("No Flash", &c_config::get().remove_flash);
	}

	else if (tab_selected == 4)
	{
		groupbox(5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test");
		checkbox("Auto Bunnyhop", &c_config::get().misc_bhop);
		checkbox("Auto Strafe", &c_config::get().misc_autostrafe);
		checkbox("Unlimited Crouch Stamina", &c_config::get().misc_fastcrouch);
		slider(100, "Slow Walk", &c_config::get().slowwalk_speed, "%", 1);
		checkbox("Animated Clantag", &c_config::get().misc_clantag);

		groupbox(TopLeftGroupBox_X, TopLeftGroupBox_Y, GroupBoxSize_Width, GroupBoxSize_Height, "test");
		std::string KeyBindList[3] = { "Thirdperson", "Slow Walk", "Swap Manual" };
		static int CurrentKeybind = 0;


		combobox(3, "Keybind", KeyBindList, &CurrentKeybind);

		static int gay23 = 0;

		switch (CurrentKeybind)
		{
		case 0: keybind(&c_config::get().thirdperson_bind, "a");
			break;
		case 1: keybind(&c_config::get().slowwalk_bind, "b");
			break;
		case 2: keybind(&c_config::get().manual_swap_bind, "c");
			break;
		}


	

		

		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, TopLeftGroupBox_Y, GroupBoxSize_Width, GroupBoxSize_Height,
			"test");
		//combobox(11, "Knife Model", knife_options, &c_config::get().knife_model);
		//combobox(16, "Knife", knife_skins, &c_config::get().knife_skin);
		color_selector("Menu theme", &c_config::get().menu_color_r, &c_config::get().menu_color_g,
			&c_config::get().menu_color_b, &c_config::get().menu_color_a);
		EmptySpace("Menu Theme");
		checkbox("Anti Untrusted", &c_config::get().misc_antiuntrusted);
		combobox(14, "Configuration", config_slots, &save_slot);
		button(Save_Config, "Save");
		button(Load_Config, "Load");
		//button(Import_Config, "Import");
		//button(Export_Config, "Export");

		groupbox(TopLeftGroupBox_X + GroupBoxSize_Width + 5, 52, GroupBoxSize_Width, GroupBoxSize_Height, "test");
	


		//

	}
	else if (tab_selected == 5)
	{
		std::string knife_options[19] = {
		"Default",
		"M9 Bayonet",
		"Bayonet",
		"Flip",
		"Gut",
		"Karambit",
		"Huntsman",
		"Falchion",
		"Bowie",
		"Butterfly",
		"Shadow Daggers",
		"Navaja",
		"Stiletto",
		"Ursus",
		"Talon",
		"Cord",
		"Canis",
		"Outdoor",
		"Skeleton"
		};

		std::string skin_list[17] = {
			"None",
			"Candy Apple",
			"Skulls",
			"Gunsmoke",
			"Bulldozer",
			"Carbon Fiber",
			"Ultraviolet",
			"Death By Kitty",
			"Emerald",
			"Dragon King",
			"Doppler",
			"Monkey Business",
			"Sun In Leo",
			"Gamma Doppler",
			"Aloha",
			"White Out",
			"Cyrex"
		};

		std::string weapon_options[10] = {
			"Knife",
			"Scar20",
			"G3SG1",
			"SSG08",
			"AK47",
			"M4A4",
			"M4A1-S",
			"DEAGLE",
			"REVOLVER",
			"CZ75"
		};
		static int SelectedWeapon;

		if (SelectedWeapon == 0) {
			this->ListBox(5, 50, GroupBoxSize_Width / 1.5, (GroupBoxSize_Height * 2) + 5, 10, "Weapon", weapon_options, &SelectedWeapon);
			this->ListBox(5 + (GroupBoxSize_Width / 1.5) + 5, 50, GroupBoxSize_Width / 1.5 - 4, (GroupBoxSize_Height * 2) + 5, 19, "Knife Model", knife_options, &c_config::get().knife_model);
			this->ListBox(5 + ((GroupBoxSize_Width / 1.5) * 2) + 5, 50, GroupBoxSize_Width / 1.5, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().knife_skin);
		}
		else {
			this->ListBox(5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 10, "Weapon", weapon_options, &SelectedWeapon);

			switch (SelectedWeapon) {
			case 1: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 16, "Skin", skin_list, &c_config::get().scar20_skin); break;
			case 2: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 16, "Skin", skin_list, &c_config::get().g3sg1_skin); break;
			case 3: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().ssg08_skin); break;
			case 4: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().ak47_skin); break;
			case 5: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().m4a4_skin); break;
			case 6: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().m4a1_s_skin); break;
			case 7: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().deagle_skin); break;
			case 8: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().r8_skin); break;
			case 9: this->ListBox(5 + GroupBoxSize_Width + 5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5, 17, "Skin", skin_list, &c_config::get().cz75_skin); break;
			}

		}
	}
	else if (tab_selected == 6)
	{
		DoPlayerList(5, 50, GroupBoxSize_Width, (GroupBoxSize_Height * 2) + 5);
	}
}

void Menu::DoPlayerList(int x, int y, int w, int h) {
	g_pSurface->FilledRect(_pos.x + x, _pos.y + y, w, h, Color(25, 26, 33, MenuAlpha_Main));
	g_pSurface->OutlinedRect(_pos.x + x, _pos.y + y, w, h, Color(40, 44, 53, MenuAlpha_Main));

	g_pSurface->FilledRect(_pos.x + x + 1, _pos.y + y + 1, w - 2, 15, Color(31, 31, 38, MenuAlpha_Main));
	g_pSurface->Line(_pos.x + x + 1, _pos.y + y + 15, _pos.x + x + w - 2, _pos.y + y + 15, Color(40, 44, 53, MenuAlpha_Main));

	g_pSurface->DrawT(_pos.x + x + (w / 2), _pos.y + y + 1, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, true, "Players");

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) {
		for (int i = 0; i < g_pGlobalVars->maxClients; i++) {
			auto Entity = g_pEntityList->GetClientEntity(i);

			if (!Globals::LocalPlayer || !Entity)
				continue;

			if (Entity->GetTeam() == Globals::LocalPlayer->GetTeam())
				continue;

			PlayerInfo_s EntityInfo;

			if (g_pEngine->GetPlayerInfo(i, &EntityInfo)) {
				auto SteamID = EntityInfo.iSteamID;

				PlayerList_Player[i] = SteamID;
				static int selected_player;

				RECT Area = { _pos.x + x, _pos.y + y + 17 + (16 * i), w, 15 };

				if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(Area.left, Area.top, Area.right, Area.bottom) && menuOpened) {
					selected_player = SteamID;
				}

				if (selected_player == SteamID) {
					g_pSurface->FilledRect(Area.left, Area.top, Area.right, Area.bottom, Color(28, 28, 35, MenuAlpha_Main));
				}
				else {
					g_pSurface->FilledRect(Area.left, Area.top, Area.right, Area.bottom, Color(21, 21, 28, MenuAlpha_Main));
				}

				g_pSurface->OutlinedRect(Area.left, Area.top, Area.right, Area.bottom, Color(40, 44, 53, MenuAlpha_Main));

				g_pSurface->DrawT(Area.left + (Area.right / 2), Area.top + 1, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, true, EntityInfo.szName);
				
			}
	
		}
	}

}
void Menu::groupbox(int x, int y, int w, int h, std::string name, bool dont_draw, bool has_subtabs)
{
	if (!dont_draw)
	{
		//	g_pSurface->OutlinedRect(_pos.x + x - 1, _pos.y + y - 1, w + 2, h + 2, Color(0, 0, 0, 185));
		g_pSurface->FilledRect(_pos.x + x, _pos.y + y, w, h, Color(25, 26, 33, MenuAlpha_Main));
		g_pSurface->OutlinedRect(_pos.x + x, _pos.y + y, w, h, Color(40, 44, 53, MenuAlpha_Main));
		//	g_pSurface->DrawT(_pos.x + x + 15, _pos.y + y - (text_size.bottom / 2), Color(181, 181, 181, MenuAlpha_Main), g::GroupboxFont, false, name.c_str());
		auto a = _pos;
	}


	int ItemsToDraw = h / 24;
	Vector2D a = g_pSurface->GetMousePosition();
	// x left 2 right (im braindead)

	groupbox_top = _pos.y + y;

	if (has_subtabs)
	{
		y_offset = (_pos.y + (y + 34));
	}
	else
	{
		y_offset = (_pos.y + (y + 12));
	}


	x_offset = _pos.x + x;
	groupbox_bottom = (_pos.y + (y + h));
	groupbox_width = (_pos.x + (x + w));
}

void Menu::ScrollableGroupBox(int x, int y, int w, int h)
{
	static bool pressed = false;
	static int add;
	if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		add += 16;
	}

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		add -= 16;
	}


	y_offset = y_offset + add;
}

void Menu::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;


	int size = 8;

	static bool pressed = false;

	RECT text_size = g_pSurface->GetTextSizeRect(Globals::SmallText, name.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 6, y_offset, size + 17 + text_size.right,
	                                                               size) && !dont_recieve_input && menuOpened)
	{
		if (pressed)
			*item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 6, y_offset, size + 17 + text_size.right,
	                                                              size) && !pressed)
		pressed = true;


	if (*item == true)
	{
		g_pSurface->FilledRect(x_offset + 6, y_offset, size, size,
		          Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b,
		                MenuAlpha_Main));

		g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
		                  name.c_str());
	}
	else
	{
		g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
		                  name.c_str());

		g_pSurface->FilledRect(x_offset + 6, y_offset, size, size, Color(75, 74, 85, MenuAlpha_Main));
	}


	g_pSurface->OutlinedRect(x_offset + 6, y_offset, size, size, Color(0, 0, 0, MenuAlpha_Main / 1.05));

	//	g_pSurface->DrawT(x_offset + 40, y_offset - 4, Color(0, 0, 0, MenuAlpha_Main), g::SmallText, false, name.c_str());


	y_offset += 16;
}

void Menu::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 25)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 180; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = -3;
	}
	else
	{
		offset = 14;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 20)
			&& !click_rest && !dont_recieve_input)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(
			x_offset + 20, y_offset + offset, ComboSize, 20))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


		g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
		                  name.c_str());

	g_pSurface->FilledRect(x_offset + 20, y_offset + offset, ComboSize, 20, Color(45, 46, 53, MenuAlpha_Main));


	g_pSurface->OutlinedRect(x_offset + 20, y_offset + offset, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));
	//
	if (open)
	{
		g_pSurface->FilledRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (size * 19),
		                       Color(45, 46, 53, MenuAlpha_Main));
		g_pSurface->OutlinedRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (size * 19),
		                         Color(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(
				x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 19))
			{
				*item = i;
			}


			if (i == *item)
				g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19,
				                  Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false, itemname[i].c_str());
			else
				g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19,
				                  Color(123, 123, 123, MenuAlpha_Main), Globals::SmallText, false, itemname[i].c_str());
		}
	}


		g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 3, Color(143, 143, 143, MenuAlpha_Main),
		                  Globals::SmallText, false, itemname[*item].c_str());

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
	{
		if (name == "")
		{
			y_offset += 47 - 26 + (size * 19);
		}
		else
		{
			y_offset += 47 + (size * 19);
		}
	}
	else
	{
		if (name == "")
		{
			y_offset += 47 - 26;
		}
		else
		{
			y_offset += 41;
		}
	}
}

void Menu::slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 30)
		return;

	float SliderSize = 180; // 163

	float _pixel_value = max / SliderSize;
	static bool opened = false;
	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20 - 3, y_offset + 14,
		                                                              SliderSize + (3 * 2), 8) && !dont_recieve_input && menuOpened)
		{
			*item = abs(g_pSurface->GetMousePosition().x - (x_offset + 20)) * _pixel_value;
		}


		static bool pressed_subtract = false;
		static bool pressed_add = false;
	}

	std::string namedisplay;

	namedisplay += name.c_str();

	//namedisplay += " - ";

	//namedisplay += std::to_string(*item).c_str();

	if (*item > max)
	{
		*item = max;
	}

	if (*item < 0)
	{
		*item = 0;
	}

		g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
		                  namedisplay.c_str());

	g_pSurface->FilledRect(x_offset + 20, y_offset + 14, SliderSize, 8, Color(45, 46, 53, MenuAlpha_Main));
	UINT hp = SliderSize - *item / max;

	int Red = 255 - ((SliderSize * (*item / _pixel_value) * 2.55));
	int Green = (SliderSize * (*item / _pixel_value) * 2.55);

	if (*item)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			g_pSurface->FilledRect(x_offset + 20, y_offset + 14, SliderSize, 8,
			          Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b,
			                MenuAlpha_Main));
		else
			g_pSurface->FilledRect(x_offset + 20, y_offset + 14, (*item / _pixel_value), 8,
			          Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b,
			                MenuAlpha_Main));
	}

	g_pSurface->OutlinedRect(x_offset + 20, y_offset + 14, SliderSize, 8, Color(0, 0, 0, MenuAlpha_Main));

	std::string gay2;
	gay2 += std::to_string(*item);
	gay2 += suffix.c_str();

	RECT Text_Size_Meme = g_pSurface->GetTextSizeRect(Globals::SmallText, gay2.c_str());

		g_pSurface->DrawT(x_offset + (SliderSize) - Text_Size_Meme.right + 30, y_offset, Color(143, 143, 143, MenuAlpha_Main),
		                  Globals::SmallText, true, "%s%s", std::to_string(*item).c_str(), suffix.c_str());

	//g_pSurface->DrawT(x_offset + 18, y_offset + 11, Color(100, 100, 100, MenuAlpha_Main), g::SmallText, false, "-");

	//g_pSurface->DrawT(x_offset + 21 + SliderSize+ 20, y_offset + 11, Color(100, 100, 100, MenuAlpha_Main), g::SmallText, false, "+");
	//
	y_offset += 30;
}

void Menu::MultiComboBox(float indexes, std::string name, std::string* itemname, bool* item)
{
	static bool multiPressed = false;
	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;
	std::string itemsSelected = "";
	int lastItem = 0;
	int ComboSize = 180;

	int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 17) && !clickRest
	)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 20))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
	                  name.c_str());

	g_pSurface->FilledRect(x_offset + 20, y_offset + offset - 2, ComboSize, 20, Color(45, 46, 53, MenuAlpha_Main));
	g_pSurface->OutlinedRect(x_offset + 20, y_offset + offset - 2, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		g_pSurface->FilledRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			Color(45, 46, 53, MenuAlpha_Main));
		g_pSurface->OutlinedRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			Color(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < indexes; i++)
		{
			if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(
				x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 20))
			{
				if (multiPressed)
					item[i] = !item[i];
				multiPressed = false;
			}

			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(
				x_offset + 20, y_offset + offset + 16 + (i * 19), ComboSize, 20) && !multiPressed)
				multiPressed = true;

			if (item[i])
				g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, Color(255, 255, 255, MenuAlpha_Main),
				                  Globals::SmallText, false, itemname[i].c_str());
			else
				g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, Color(185, 185, 185, MenuAlpha_Main),
				                  Globals::SmallText, false, itemname[i].c_str());
		}
	}

	bool items = false;

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{
			if (lastItem < i)
				lastItem = i;
		}
	}

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{
			items = true;
			RECT TextSize = g_pSurface->GetTextSizeRect(Globals::SmallText, itemsSelected.c_str());
			RECT TextSizeGonaAdd = g_pSurface->GetTextSizeRect(Globals::SmallText, itemname[i].c_str());
			if (TextSize.right + TextSizeGonaAdd.right < ComboSize)
				itemsSelected += std::to_string(i) + ((lastItem == i) ? "" : ", ");
		}
	}

	if (!items)
		itemsSelected = "Disabled";

	g_pSurface->DrawT(x_offset + 20 + 5, y_offset + offset + 1, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
	                  itemsSelected.c_str());

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
		y_offset += 52 + (indexes * 16);
	else
		y_offset += 41;
}

char* KeyStringsStick[254] = {
	"INVLD", "M1", "M2", "BRK", "M3", "M4", "M5",
	"INVLD", "BSPC", "TAB", "INVLD", "INVLD", "INVLD", "ENTER", "INVLD", "INVLD", "SHI",
	"CTRL", "ALT", "PAU", "CAPS", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"ESC", "INVLD", "INVLD", "INVLD", "INVLD", "SPACE", "PGUP", "PGDOWN", "END", "HOME", "LEFT",
	"UP", "RIGHT", "DOWN", "INVLD", "PRNT", "INVLD", "PRTSCR", "INS", "DEL", "INVLD", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X", "Y", "Z", "LFTWIN", "RGHTWIN", "INVLD", "INVLD", "INVLD", "NUM0", "NUM1",
	"NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
	"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",
	"F21",
	"F22", "F23", "F24", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"NUM LOCK", "SCROLL LOCK", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "LSHFT", "RSHFT", "LCTRL",
	"RCTRL", "LMENU", "RMENU", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "NTRK", "PTRK", "STOP", "PLAY", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", ";", "+", ",", "-", ".", "/?", "~", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "{", "\\|", "}", "'\"", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD"
};

bool keys[256];
bool oldKeys[256];

bool GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	return false;
}

void Menu::keybind(int* item, std::string name)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	int ComboSize = 180; // 163
	static std::string nameSelected;


	static bool IsGettingKey = false;
	static int Key = 0;
	static bool GoodKeyName = false;
	Color text_color = Color(120, 120, 120, 255);
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	char NameBuffer[128];
	char* KeyName = "Unbinded";
	RECT text_size = g_pSurface->GetTextSizeRect(Globals::SmallText, KeyName);
	static bool LetsBindy0 = false;
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset, 157, 20) && menuOpened)
	{
		if (!IsGettingKey)
		{
			IsGettingKey = true;
		}
	}

	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Key = -1;
					*item = Key;
					IsGettingKey = false;
					return;
				}

				Key = i;
				*item = Key;
				IsGettingKey = false;
				return;
			}
		}
	}


	if (IsGettingKey)
	{
		KeyName = "~~~~~";
		text_color = Color(143, 143, 143, 255);
	}
	else
	{
		if (*item >= 0)
		{
			KeyName = KeyStringsStick[*item];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(*item << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "Unbinded";
		}
	}

	if (manually_set_key)
	{
		Key = *item;
		manually_set_key = false;
	}


	g_pSurface->FilledRect(x_offset + 20, y_offset, ComboSize, 20, Color(45, 46, 53, MenuAlpha_Main));
	g_pSurface->OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));
	g_pSurface->DrawT(x_offset + 20 + 5, y_offset + 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
	                  KeyName);

	y_offset += 29;
}

unsigned int rgb(double hue)
{
	int h = int(hue * 256 * 6);
	int x = h % 0x100;

	int r = 0, g = 0, b = 0;
	switch (h / 256)
	{
	case 0: r = 255;
		g = x;
		break;
	case 1: g = 255;
		r = 255 - x;
		break;
	case 2: g = 255;
		b = x;
		break;
	case 3: b = 255;
		g = 255 - x;
		break;
	case 4: b = 255;
		r = x;
		break;
	case 5: r = 255;
		b = 255 - x;
		break;
	}

	return r + (g << 8) + (b << 16);
}

void color_spectrum(int x, int y)
{
	int Width = 168, Height = 179;

	for (int i = 0; i < Width; i++)
	{
		int div = Width / 6;
		int phase = i / div;
		float t = (i % div) / (float)div;
		int r, g, b;

		switch (phase)
		{
		case(0):
			r = 255;
			g = 255 * t;
			b = 0;
			break;
		case(1):
			r = 255 * (1.f - t);
			g = 255;
			b = 0;
			break;
		case(2):
			r = 0;
			g = 255;
			b = 255 * t;
			break;
		case(3):
			r = 0;
			g = 255 * (1.f - t);
			b = 255;
			break;
		case(4):
			r = 255 * t;
			g = 0;
			b = 255;
			break;
		case(5):
			r = 255;
			g = 0;
			b = 255 * (1.f - t);
			break;
		}

		for (int k = 0; k < Height; k++)
		{
			float sat = k / (float)Height;
			int _r = r + sat * (128 - r);
			int _g = g + sat * (128 - g);
			int _b = b + sat * (128 - b);
			g_pSurface->DrawSetColor(_r, _g, _b, 255);
			g_pSurface->DrawFilledRect(10 + i, 10 + k, 1, 1);
		}
	}
}

#include <conio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>

void Menu::EmptySpace(std::string name)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	g_pSurface->DrawT(x_offset + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
	                  name.c_str());

	y_offset += 16;
}

void Menu::color_selector(std::string name, int* red, int* green, int* blue, int* alpha)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "sadasd")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


	static unsigned int r = 255;
	static unsigned int g = 0;
	static unsigned int b = 0;


	static unsigned int selected_r = 0;
	static unsigned int selected_g = 0;
	static unsigned int selected_b = 0;

	static Vector2D MousePos;
	COLORREF color;
	COLORREF selected_color;

	if (open)
	{
		dont_recieve_input = true;
		g_pSurface->FilledRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
		                       Color(21, 23, 27, MenuAlpha_Main));

		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
		                         Color(42, 46, 46, MenuAlpha_Main));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 - 1, y_offset - 1, ComboSize + 2, ComboSizeH + 2,
		                         Color(34, 38, 37, MenuAlpha_Main));

		float _pixel_value = 255 / (ComboSize - 5 - 17);

		//if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20 - 3, y_offset + 14, SliderSize + (3 * 2), 8)) {
		//	*item = abs(g_pSurface->GetMousePosition().x - (x_offset + 20)) * _pixel_value;
		//}

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225 - ComboSize - 5 + 5,
		                                                              y_offset + ComboSizeH - 11, (ComboSize - 5 - 17),
		                                                              7) && menuOpened)
		{
			*alpha = abs(g_pSurface->GetMousePosition().x - (x_offset + 225 - ComboSize - 5 + 5)) * _pixel_value;
		}


		if (*alpha < 3)
		{
			*alpha = 0;
		}

		if (*alpha > 204)
		{
			*alpha = 255;
		}

		g_pSurface->FilledRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
		                       Color(255, 255, 255, *alpha));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
		                         Color(0, 0, 0, MenuAlpha_Main));


		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 233 - ComboSize + 101 + 1,
		                                                              y_offset + 5 + 1, 7 - 2, 107 - 1))
		{
			HDC hdc = GetDC(nullptr);
			color = GetPixel(hdc, g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);

			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
		}


		GradientH(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
		          Color(255, 255, 255), Color(r, g, b));
		GradientVEnd(x_offset + 225 - ComboSize - 5 + 5, y_offset + 40, ComboSize - 5 - 17, ComboSizeH - 20,
		             Color(0, 0, 0), Color(0, 0, 0));


		//g_pSurface->OutlinedRect(MousePos.x - 4, MousePos.y - 4, 8, 8, Color(0, 0, 0, MenuAlpha_Main));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
		                         Color(0, 0, 0, MenuAlpha_Main));

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225 - ComboSize - 5 + 5 + 1,
		                                                              y_offset + 5 + 1, ComboSize - 5 - 17 - 2,
		                                                              ComboSizeH - 20 - 2))
		{
			HDC hdc = GetDC(nullptr);
			selected_color = GetPixel(hdc, g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);

			selected_r = GetRValue(selected_color);
			selected_g = GetGValue(selected_color);
			selected_b = GetBValue(selected_color);

			*red = selected_r;
			*green = selected_g;
			*blue = selected_b;

			MousePos = Vector2D(g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);
		}

		Color rainbow; int hue;
		for (int i = 0; i < 100; i++)
		{
			float hue = (i * .01f);
			rainbow.FromHSV(hue, 1.f, 1.f);
			g_pSurface->FilledRect(x_offset + 233 - ComboSize + 101, y_offset + 5 + i, 7, 8, rainbow);
			g_pSurface->OutlinedRect(x_offset + 233 - ComboSize + 101, y_offset + 5, 7, 107, Color(0, 0, 0, 255));
		}
	}
	else
	{
		dont_recieve_input = false;
	}


	if (*red > 20 && *green > 20 && *blue > 20)
	{
		g_pSurface->FilledRect(x_offset + 225, y_offset, 15, 7, Color(*red, *green, *blue, MenuAlpha_Main));
	}
	else
	{
		g_pSurface->FilledRect(x_offset + 225, y_offset, 15, 7, Color(*red, *green, *blue, MenuAlpha_Main));
	}


	g_pSurface->OutlinedRect(x_offset + 225 - 1, y_offset - 1, 15 + 2, 7 + 2, Color(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		y_offset += ComboSizeH;
	}

	/*
	Color rainbow;

	Color Test2;

	static float test;

	static DWORD color;
	static DWORD store_color;
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225 - 93, y_offset + 1, 108, 8)) {


	}
	else {
		unsigned int r = GetRValue(color);
		unsigned int g = GetGValue(color);
		unsigned int b = GetBValue(color);

		*red = r;
		*green = g;
		*blue = b;

		GradientV(x_offset + 225, y_offset, 16, 9, Color(*red, *green, *blue), Color(*red, *green, *blue));
		g_pSurface->OutlinedRect(x_offset + 225, y_offset, 16, 9, Color(0, 0, 0, MenuAlpha_Main));
	}
	*/
}

char* KeyDigits[254] = {
	nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
	nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
	"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr,
	"SPACEBAR",
	"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen",
	"Insert",
	"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr,
	nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X",
	"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
	"7", "8", "9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11",
	"F12",
	"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause",
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr,
	nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

void Menu::TextBox(std::string name, std::string* text)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}


	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}

	const char* strg = text->c_str();
	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					open = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					*text = text->substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 6 && (i == 0x30 || i == 0x31 || i == 0x32 || i == 0x33 || i == 0x34 || i == 0x35 || i
					== 0x36 || i == 0x37 || i == 0x38 || i == 0x39 || i == VK_NUMPAD0 || i == VK_NUMPAD1 || i ==
					VK_NUMPAD2 || i == VK_NUMPAD3 || i == VK_NUMPAD4 || i == VK_NUMPAD5 || i == VK_NUMPAD6 || i ==
					VK_NUMPAD7 || i == VK_NUMPAD8 || i == VK_NUMPAD9))
				{
					*text = *text + KeyDigits[i];
					return;
				}
			}
		}
	}

	const char* cstr = text->c_str();
	g_pSurface->FilledRect(x_offset + 20, y_offset, ComboSize, 20, Color(45, 46, 53, MenuAlpha_Main));
	g_pSurface->OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));
	g_pSurface->DrawT(x_offset + 20 + 5, y_offset + 3, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, false,
	                  cstr);

	y_offset += 29;
}

void Menu::button(ButtonCallback_t callback, std::string title)
{
	if (groupbox_bottom <= y_offset + 18)
		return;

	int ComboSize = 180;

	static bool pressed = false;

	RECT text_size = g_pSurface->GetTextSizeRect(Globals::SmallText, title.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) && menuOpened)
	{
		if (pressed)
		{
			if (callback)
				callback();
		}
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) && !pressed)
		pressed = true;

	if (g_pSurface->MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) and GetAsyncKeyState(VK_LBUTTON))
	{
		g_pSurface->FilledRect(x_offset + 20, y_offset, ComboSize, 20, Color(35, 36, 43, MenuAlpha_Main));
	}
	else
	{
		g_pSurface->FilledRect(x_offset + 20, y_offset, ComboSize, 20, Color(45, 46, 53, MenuAlpha_Main));
	}

	g_pSurface->OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));

	g_pSurface->DrawT(x_offset + 20 + (ComboSize / 2) - (text_size.right / 2),
	                  y_offset + (20 / 2) - (text_size.bottom / 2) - 1, Color(204, 204, 204, MenuAlpha_Main),
	                  Globals::SmallText, false, title.c_str());
	y_offset += 26;
}


void Menu::ListBox(int x, int y, int w, int h, int size, std::string name, std::string *itemname, int* item) {
	g_pSurface->FilledRect(_pos.x + x, _pos.y + y, w, h, Color(25, 26, 33, MenuAlpha_Main));
	g_pSurface->OutlinedRect(_pos.x + x, _pos.y + y, w, h, Color(40, 44, 53, MenuAlpha_Main));

	g_pSurface->FilledRect(_pos.x + x + 1, _pos.y + y + 1, w - 2, 15, Color(31, 31, 38, MenuAlpha_Main));
	g_pSurface->Line(_pos.x + x + 1, _pos.y + y + 15, _pos.x + x + w - 2, _pos.y + y + 15, Color(40, 44, 53, MenuAlpha_Main));

	g_pSurface->DrawT(_pos.x + x + (w / 2), _pos.y + y + 1, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, true, name.c_str());

	for (int i = 0; i < size; i++) {
		RECT Area = { _pos.x + x, _pos.y + y + 17 + (16 * i), w, 15 };

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(Area.left, Area.top, Area.right, Area.bottom) && menuOpened) {
			*item = i;
		}

		if (*item == i) {
			g_pSurface->FilledRect(Area.left, Area.top, Area.right, Area.bottom, Color(28, 28, 35, MenuAlpha_Main));
		}
		else {
			g_pSurface->FilledRect(Area.left, Area.top, Area.right, Area.bottom, Color(21, 21, 28, MenuAlpha_Main));
		}

		g_pSurface->OutlinedRect(Area.left, Area.top, Area.right, Area.bottom, Color(40, 44, 53, MenuAlpha_Main));

		g_pSurface->DrawT(Area.left + (Area.right / 2), Area.top + 1, Color(143, 143, 143, MenuAlpha_Main), Globals::SmallText, true, itemname[i].c_str());
	}
}
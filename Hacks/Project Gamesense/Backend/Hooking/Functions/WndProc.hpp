#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Configuration/Config.h"
#include "../KeyHandler.h"

#include "../../../Frontend/Menu/cmenu.h"
WNDPROC oWndProc;
bool oldImHandler = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Cheat
{
	namespace Hooked
	{
		static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
		{
			if (msg == WM_DESTROY) {
				Cheat::Settings->SocketConnection.Close();
				PostQuitMessage(0);
			}

			if (msg == WM_KEYDOWN)
			{
				if (wParam == c_config::get()->i["misc_menukey"])
					c_menu::get()->set_menu_opened(!c_menu::get()->is_menu_opened());
				
				if (wParam == c_config::get()->i["_preset_0"])
				{
					c_config::get()->i["_preset"] = 0;
					c_config::get()->load();
				}

				if (wParam == c_config::get()->i["_preset_1"])
				{
					c_config::get()->i["_preset"] = 1;
					c_config::get()->load();
				}

				if (wParam == c_config::get()->i["_preset_2"])
				{
					c_config::get()->i["_preset"] = 2;
					c_config::get()->load();
				}

				if (wParam == c_config::get()->i["_preset_3"])
				{
					c_config::get()->i["_preset"] = 3;
					c_config::get()->load();
				}

				if (wParam == c_config::get()->i["_preset_4"])
				{
					c_config::get()->i["_preset"] = 4;
					c_config::get()->load();
				}

				if (wParam == c_config::get()->i["_preset_5"])
				{
					c_config::get()->i["_preset"] = 5;
					c_config::get()->load();
				}
			}

			c_keyhandler::get()->wnd_handler(msg, wParam);

			if (c_menu::get()->is_menu_opened()) {
				ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);
			
				if (wParam != 'W' && wParam != 'A' && wParam != 'S' && wParam != 'D' && wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_TAB && wParam != VK_SPACE || ui::GetIO().WantTextInput)
					return true;
			}
			
			return CallWindowProcA(oWndProc, window, msg, wParam, lParam);
		}
	}
}
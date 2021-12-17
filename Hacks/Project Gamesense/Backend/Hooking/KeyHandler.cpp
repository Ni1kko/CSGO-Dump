#include "KeyHandler.h"
#include "../Configuration/Config.h"

void c_keyhandler::wnd_handler(UINT msg, WPARAM wParam) {
	
}

bool c_keyhandler::is_key_down(int key) {
	return HIWORD(GetKeyState(key));
}

bool c_keyhandler::is_key_up(int key) {
	return !HIWORD(GetKeyState(key));
}

bool c_keyhandler::is_key_pressed(int key) {
	return false;
}

bool c_keyhandler::auto_check(std::string key) {

	char target_wnd_class[] = "Valve001", active_wnd_class[sizeof(target_wnd_class)];
	HWND active_hwnd = GetActiveWindow();
	GetClassName(active_hwnd, active_wnd_class, sizeof(active_wnd_class));
	bool target_wnd_active = (strcmp(target_wnd_class, active_wnd_class) == 0);

	switch (c_config::get()->i[key + "style"]) {
	case 0:
		return true;
	case 1:
		return target_wnd_active && this->is_key_down(c_config::get()->i[key]);
	case 2:
		return target_wnd_active && LOWORD(GetKeyState(c_config::get()->i[key]));
	case 3:
		return target_wnd_active && this->is_key_up(c_config::get()->i[key]);
	default:
		return true;
	}
}
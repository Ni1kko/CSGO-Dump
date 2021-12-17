#pragma once
#include "../Include/GlobalIncludes.h"
#include "../singleton.h"

class c_keyhandler : public singleton<c_keyhandler> {
public:
	bool is_key_down(int key);
	bool is_key_up(int key);
	bool is_key_pressed(int key);

	bool auto_check(std::string key);

	void wnd_handler(UINT msg, WPARAM wParam);

private:
	bool keys[256] = { false };
	bool pressed[256] = { false };
	bool toggles[256] = { false };
};

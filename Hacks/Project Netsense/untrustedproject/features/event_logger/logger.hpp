#pragma once
#include <string>
#include <deque>
#include "../../render.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"

namespace notify {
	void notify_events(IGameEvent* event);
	void render();

	namespace console {
		void clear();
		void notify(std::string pre, std::string msg, Color clr_pre);
	}
	namespace screen {
		void notify(std::string pre, std::string text, Color color_pre, Color color_text = Color(255, 255, 255, 255), int life_time = 4700);
	}
}
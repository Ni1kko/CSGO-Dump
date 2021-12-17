#pragma once

#include <deque>
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\singleton.h"
#include "..\..\SDK\IGameEvent.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\ICvar.h"

class c_event_logs : public singleton<c_event_logs> {
public:
	void run();
	void event_item_purchase(IGameEvent* event);
	void event_player_hurt(IGameEvent* event);
	void event_player_death(IGameEvent* event);
	void add(std::string text, Color colors);

private:
	struct loginfo_t {
		loginfo_t(const float log_time, std::string message, const Color colors) {
			this->log_time = log_time;
			this->message = message;
			this->colors = colors;

			this->x = 2.f;
			this->y = 0.f;
			this->change_alpha = false;
		}

		float log_time;
		std::string message;
		Color colors;
		float x, y;
		bool change_alpha;
		int alpha;
	};

	std::deque< loginfo_t > logs;
};
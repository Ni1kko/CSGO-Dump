#pragma once

#include "..\..\includes.hpp"
#include <deque>

class eventlogs : public singleton< eventlogs > {
public:
	void paint_traverse();
	void events(IGameEvent * event);
	void add(std::string text, Color color);

private:
	struct loginfo_t
	{
		loginfo_t(const float log_time, std::string message, const Color color) {
			this->log_time = log_time;
			this->message = message;
			this->color = color;

			this->x = 6.f;
			this->y = 0.f;
		}

		float log_time;
		std::string message;
		Color color;
		float x, y;
	};

	std::deque< loginfo_t > logs;
};
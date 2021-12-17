#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class bullettracers : public singleton< bullettracers > {
private:
	class trace_info {
	public:
		trace_info(Vector starts, Vector positions, float times) {
			this->start = starts;
			this->position = positions;
			this->time = times;
		}

		Vector position;
		Vector start;
		float time;
	};

	std::vector<trace_info> logs;
	void draw_beam(vec3_t src, vec3_t end, Color color);
	void draw_beamenemy(vec3_t src, vec3_t end, Color color);
public:
	void events(IGameEvent* event);
};
#pragma once

#include "Hacks.h"
#include "Hooks.h"

namespace global_count
{
	extern int hits[65];
	extern int shots_fired[65];
	extern int missed_shots[64];
	extern bool didhit[64];
	extern bool on_fire;

	extern int missed;
	extern int hit;
}

namespace game_event
{
	void on_hurt(IGameEvent * Event);
}
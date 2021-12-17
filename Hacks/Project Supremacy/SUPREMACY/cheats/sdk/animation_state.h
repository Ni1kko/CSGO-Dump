#pragma once

#include "..\..\includes.hpp"

#include "..\..\ragebot.h"

class animation_state : public singleton< animation_state > {
public:
	void create_move();

	void create_state(c_baseplayeranimationstate * state, player_t * player);
	void update_state(c_baseplayeranimationstate * state, vec3_t angles);
	void reset_state(c_baseplayeranimationstate * state);

	c_baseplayeranimationstate * animstate();
};
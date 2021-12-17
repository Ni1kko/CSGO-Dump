#pragma once

#include "legit_utils.h"
#include "../Ragebot/CAutoWall.h"

class CTriggerBot
{
protected:
	bool  is_delayed        = false;
	bool  shot_delay        = false;
	float shot_delay_time;
	bool can_burst_fire = false;
	bool do_burst_fire = false;
	int burst_fire_start_time = 0;
public:
	bool is_attack = false;
	void OnMove(CUserCmd* usercmd);
};

extern CTriggerBot g_triggerbot;
#pragma once

#include "sdk.h"

class CGlobalVars
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float unknown;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

extern CGlobalVars* g_pGlobalVars;

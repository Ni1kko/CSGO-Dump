#pragma once
#include "Hooks.h"

struct impact_info
{
	float x, y, z;
	float time;
};
struct hitmarker_info
{
	impact_info impact;
	int alpha;
	float time;
};
class CHitMarker
{
public:
	std::vector<impact_info> impacts;
	std::vector<hitmarker_info> hitmarkers;
	void Paint();
	void OnPlayerHurt(IGameEvent* pEvent);
	void OnBulletImpact(IGameEvent* pEvent);
	IBasePlayer* GetPlayer(int ID);
};

extern CHitMarker* g_Hitmarker;
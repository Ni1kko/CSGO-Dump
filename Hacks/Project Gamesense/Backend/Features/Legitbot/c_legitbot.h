#pragma once

#include "legit_utils.h"
#include "c_triggerbot.h"

class CLegitAimbot
{
public:
	void OnMove(CUserCmd* pCmd);
	bool IsEnabled(CUserCmd* pCmd);
	bool IsRcs();
	float GetSmooth();
	void QuickStop();
	CBaseEntity* GetClosestPlayer(CUserCmd* cmd, int* p_bestBone, float* p_bestBoneFov);
	float quick_stop_inaccuracy = -1.0f;
	const float max_smooth = 42.f;
private:
	void RCS(Vector& angle, CBaseEntity* target, bool should_run);
	void Smooth(Vector currentAngle, Vector aimAngle, Vector& angle, float smooth_div);
	std::vector<int> GetHitBoxes(bool hit_head, bool hit_chest, bool hit_stomach);
	CBaseEntity* GetClosestPlayerDelay(CUserCmd* pCmd, int& bestBone, float& bestBoneFov);
	Vector CurrentPunch = { 0, 0, 0 };
	Vector RCSLastPunch = { 0, 0, 0 };
	int kill_delay_time = 0;
	bool kill_delay = false;
	CBaseEntity* target = nullptr;
	bool delayed_shot = false;
	std::map<int, int> lock_on_starts_map = {};
};

extern CLegitAimbot g_legitbot;

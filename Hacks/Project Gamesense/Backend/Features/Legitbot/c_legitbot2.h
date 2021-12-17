//#pragma once
//
//#include "legit_utils.h"
//
//class Aimbot {
//public:
//	void OnMove(CUserCmd* pCmd);
//	bool IsEnabled(CUserCmd* pCmd);
//
//	bool IsRcs();
//	float GetSmooth();
//
//private:
//	void RCS(Vector& angle, CBaseEntity* target, bool should_run);
//	void Smooth(Vector currentAngle, Vector aimAngle, Vector& angle);
//	std::vector<int> GetHitBoxes(bool hit_head, bool hit_chest, bool hit_stomach);
//	CBaseEntity* GetClosestPlayer(CUserCmd* cmd, int* bestBone);
//	CBaseEntity* GetClosestPlayerDelay(CUserCmd* pCmd, int& bestBone);
//	Vector CurrentPunch = { 0,0,0 };
//	Vector RCSLastPunch = { 0,0,0 };
//	int kill_delay_time;
//	bool kill_delay = false;
//	CBaseEntity* target = NULL;
//};
//extern Aimbot g_Aimbot;

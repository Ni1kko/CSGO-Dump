#pragma once
#include "../config/options.hpp"
#include "../../sdk/structs.hpp"
#include "../menu/menu.hpp"

class LegitBot : public Singleton<LegitBot>
{
	friend class Singleton<LegitBot>;
public:
	void OnMove(CUserCmd* pCmd);
	bool IsEnabled(CUserCmd* pCmd);
	float GetFovToPlayer(QAngle viewAngle, QAngle aimAngle);
	legitbot_settings settings;

	bool IsRcs();
	float GetSmooth();
	float GetFov();

private:
	void RCS(QAngle& angle, C_BasePlayer* target, bool should_run);
	bool IsLineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
	void Smooth(QAngle currentAngle, QAngle aimAngle, QAngle& angle);
	bool IsNotSilent(float fov);
	C_BasePlayer* GetClosestPlayer(CUserCmd* cmd, int& bestBone);
	float shot_delay_time;
	bool shot_delay = false;
	bool silent_enabled = false;
	QAngle CurrentPunch = { 0,0,0 };
	QAngle RCSLastPunch = { 0,0,0 };
	bool is_delayed = false;
	int kill_delay_time;
	bool kill_delay = false;
	C_BasePlayer* target = NULL;
};
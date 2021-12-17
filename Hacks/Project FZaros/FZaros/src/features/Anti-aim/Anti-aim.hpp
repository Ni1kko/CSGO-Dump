#pragma once

#include "../../utilities/includes.h"
#include "../Aimbot/Autowall.hpp"

class CAntiAim
{
public:
	void onMove(CUserCmd *userCmd);
private:
	void pitch(CUserCmd*);
	void yaw(CUserCmd*);
	void desync_initialize(CUserCmd* cmd);
	//void desync_polakware(CUserCmd* cmd);

	void breakLby( );

	CBaseEntity* freestandEnt = nullptr;
	float finalAng;
	float nextLbyUpdate;

	void pitch_down(CUserCmd* cmd);
	void pitch_fake_down(CUserCmd* cmd);
	void pitch_up(CUserCmd* cmd);
	void pitch_fake_up(CUserCmd* cmd);
	void pitch_zero(CUserCmd* cmd);
	void pitch_fake_zero(CUserCmd* cmd);
	void pitch_jitter(CUserCmd* cmd);
	void pitch_fake_jitter(CUserCmd* cmd);

	void yaw_backwards(CUserCmd* cmd);
	void yaw_sideways(CUserCmd* cmd);

	void yaw_autoDirection(CUserCmd* cmd);

}; extern CAntiAim g_AntiAim;
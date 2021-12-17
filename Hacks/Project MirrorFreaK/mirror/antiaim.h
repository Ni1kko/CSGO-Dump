#pragma once
#include "Hacks.h"
class anti_aim
{
public:

	void DoPitch(CUserCmd * pCmd);
	float get_feet_yaw();
	void Base_AntiAim(CUserCmd * pCmd, IClientEntity * pLocal, bool & bSendPacket);
	void DoAntiAim(CUserCmd *pCmd, bool&bSendPacket);

	//---- pitches ----//

	void pitchdown(CUserCmd * pcmd);

	void pitchup(CUserCmd * pcmd);

	void zero(CUserCmd * pcmd);

	void pitchjitter(CUserCmd * pcmd);

	void pitchrandom(CUserCmd * pcmd);

	void fakedown(CUserCmd * pcmd);

	void fakeup(CUserCmd * pcmd);

	//---- yaws ----//

	void backwards(CUserCmd * pcmd);

	void backwards_jitter(CUserCmd * pcmd);

	void lowerbody(CUserCmd * pcmd);

	void rand_lowerbody(CUserCmd * pcmd);

	void manual(CUserCmd * pCmd);

	void jitter_crooked(CUserCmd * pcmd);

	void manual_jitter(CUserCmd * pCmd);

	void freestanding_jitter(CUserCmd * pCmd);


};

class antiaim_helper
{
public:

	void anti_lby(CUserCmd * cmd, bool & bSendPacket);

	float fov_ent(Vector ViewOffSet, Vector View, IClientEntity * entity, int hitbox);

	int closest();

private:
	
	int closest_entity = -1;
	float NextPredictedLBYUpdate = 0.f;

};

extern anti_aim * c_antiaim;
extern antiaim_helper * c_helper;
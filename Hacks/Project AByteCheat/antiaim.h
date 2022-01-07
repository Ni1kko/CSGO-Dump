#pragma once
#include "Hacks.h"
class anti_aim
{
public:

	void DoPitch(CUserCmd * pCmd);

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

	float add_jitter_to_yaw();

	void backwards(CUserCmd * pcmd);

	void backwards_jitter(CUserCmd * pcmd, bool moving);

	void lowerbody(CUserCmd * pcmd);

	void rand_lowerbody(CUserCmd * pcmd);

	void jitter_180(CUserCmd * pcmd);

	void manual(CUserCmd * pCmd);

	void twisty(CUserCmd * cmd);

	void jitter_crooked(CUserCmd * pcmd);

	void freestanding_jitter(CUserCmd * pCmd);

	void freestanding_desync(CUserCmd* pCmd, bool SP);

	//---- desync ----//
	float get_feet_yaw();

	void bodyleg_yaw();

	void do_desync(CUserCmd * cmd, bool moving);

	void desync_simple(CUserCmd * cmd, bool moving);

	void desync_default(CUserCmd * cmd, bool moving);

	void desync_stretch(CUserCmd * cmd, bool moving);

	void desync_crooked(CUserCmd * cmd, bool moving);

	void desync_switch(CUserCmd * cmd, bool moving);

	void desync_auto(CUserCmd * cmd, bool moving);

	void desync_jitter(CUserCmd * cmd, bool moving);

	void desync_static(CUserCmd * cmd, bool moving);

	float test(CUserCmd * pCmd);


private:
	bool stand_done = false;
	bool move_done = false;

//	bool swap_sides_stand = options::menu.MiscTab.desync_swapsides_stand.GetState();
//	bool swap_sides_move = options::menu.MiscTab.desync_swapsides_move.GetState();

	float stand_range = options::menu.AntiAimTab.FakeYawOffset.GetValue();
	float move_range = options::menu.AntiAimTab.FakeYawOffset.GetValue();
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
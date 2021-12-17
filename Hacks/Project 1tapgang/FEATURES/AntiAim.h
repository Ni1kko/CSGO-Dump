#pragma once
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CAntiAim
{
public:
	bool next_lby_update2();
	void do_antiaim(SDK::CUserCmd* cmd);
	void fix_movement(SDK::CUserCmd* cmd);
	Vector fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd orignal);
private:
	void backwards(SDK::CUserCmd* cmd);
	void legit(SDK::CUserCmd* cmd);
	void sidespin(SDK::CUserCmd * cmd);

	bool IsTargettingLocal(SDK::CBaseEntity * loca, SDK::CBaseEntity * entity);
	void freestand(SDK::CUserCmd * cmd);
	void sideways(SDK::CUserCmd* cmd);
	void lowerbody(SDK::CUserCmd* cmd);
	void backjitter(SDK::CUserCmd* cmd);
};

extern CAntiAim* antiaim;

float get_max_desync_delta(SDK::CBaseEntity * local);

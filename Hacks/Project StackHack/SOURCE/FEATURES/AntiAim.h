#pragma once
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CAntiAim
{
public:
	void do_antiaim(SDK::CUserCmd* cmd);
	void fix_movement(SDK::CUserCmd* cmd);
	Vector fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd orignal);
	void freestand(SDK::CUserCmd* cmd, float yawangs);
private:
	void backwards(SDK::CUserCmd* cmd);
	void legit(SDK::CUserCmd* cmd);
	void sidespin(SDK::CUserCmd * cmd);	
	void freestand2(SDK::CUserCmd * cmd);
	void sideways(SDK::CUserCmd* cmd);
	void lowerbody(SDK::CUserCmd* cmd);
	void backjitter(SDK::CUserCmd* cmd);
};

extern CAntiAim* antiaim;
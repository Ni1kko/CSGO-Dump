#pragma once
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CAntiAim
{
public:
	void freestand(SDK::CUserCmd* cmd, float yawangs);
	void do_antiaim(SDK::CUserCmd* cmd);
	float TimeUntilNextLBYUpdate()
	{
		return m_next_lby_update_time - UTILS::GetCurtime();
	}
	void fix_movement(SDK::CUserCmd* cmd);
private:
	void backwards(SDK::CUserCmd* cmd);
	void AtTargets(SDK::CUserCmd* pCmd);
	float m_next_lby_update_time = 0.f, m_last_move_time = 0.f, m_last_attempted_lby = 0.f;
	bool m_will_lby_update = false;

	

};

extern CAntiAim* antiaim;
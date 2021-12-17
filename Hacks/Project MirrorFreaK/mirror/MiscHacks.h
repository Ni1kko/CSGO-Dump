#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void namespam();
	void buybot_primary();
	void buybot_secondary();
	void buybot_otr();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void FakeWalk0(CUserCmd * pCmd, bool & bSendPacket);

private:
	
	void AutoPistol(CUserCmd * pCmd);
	void strafe_2(CUserCmd * cmd);
	void PostProcces();

	void MinimalWalk(CUserCmd * pCommand, float fMaxSpeed);
	void fake_crouch(CUserCmd * cmd, bool &packet);
	float get_gun(C_BaseCombatWeapon* weapon);

	void strafer(CUserCmd * cmd);

	void viewmodel_x_y_z();

	void AutoJump(CUserCmd *pCmd);
	void airduck(CUserCmd * pCmd);
	void RageStrafe(CUserCmd *pCmd);

	bool _done = false;
	int CircleFactor = 0;
	static vec_t normalize_yaw(vec_t ang)
	{
		while (ang < -180.0f)
			ang += 360.0f;
		while (ang > 180.0f)
			ang -= 360.0f;
		return ang;
	}
	
}; extern CMiscHacks* g_Misc;


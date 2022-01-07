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
	void animation_fix();

	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void FakeWalk0(CUserCmd * pCmd, bool & bSendPacket);
	float get_gun(C_BaseCombatWeapon* weapon);
	bool _test = false;
	bool oof = false;
	bool _done = false;
	void optimize();
private:
	
	void AutoPistol(CUserCmd * pCmd);
	void strafe_2(CUserCmd * cmd);
	void PostProcces();

	void MinimalWalk(CUserCmd * pCommand, float fMaxSpeed);
	void fake_crouch(CUserCmd * cmd, bool & packet, IClientEntity * local);

	void strafer(CUserCmd * cmd);

	void viewmodel_x_y_z();

	void RankReveal(CUserCmd * cmd);


	void AutoJump(CUserCmd *pCmd);
	void airduck(CUserCmd * pCmd);
	void RageStrafe(CUserCmd * userCMD, IClientEntity * g_LocalPlayer);

	bool paste = false;
	int CircleFactor = 0;
	static vec_t normalize_yaw(vec_t ang)
	{
		while (ang < -180.0f)
			ang += 360.0f;
		while (ang > 180.0f)
			ang -= 360.0f;
		return ang;
	}
	
}; extern CMiscHacks* c_misc;


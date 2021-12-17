#pragma once
#include "Hacks.h"
struct AntiaimData_t
{
	AntiaimData_t(const float& dist, const bool& inair, int player)
	{
		this->flDist = dist;
		this->bInAir = inair;
		this->iPlayer = player;
	}
	float flDist;
	bool bInAir;
	int	iPlayer;
};
class CRageBot : public CHack
{
public:

	void Init();
	void Draw();
	int choked = -1;

	bool lbyupdate;
	bool doing_base_antiaim = false;
	bool should_baim(IClientEntity * pEntity);
	bool hitchance_spread(QAngle angles, IClientEntity * ent, float chance);
	int HitScan(IClientEntity* pEntity);
	void auto_revolver(CUserCmd * m_pcmd);

	bool CanOpenFire();
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void DoNoRecoil(CUserCmd *pCmd);
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	CUserCmd* cmd;
	bool shot_this_tick;
	bool there_is_a_target = false;
	Vector target_point;
	Vector origin_fl;
	int TargetID;
	int GetTargetCrosshair();

private:
	std::vector<AntiaimData_t> Entities;

	bool TargetMeetsRequirements(IClientEntity* pEntity);
	void aimAtPlayer(CUserCmd * pCmd);

	bool AimAtPoint(IClientEntity * pLocal, Vector point, CUserCmd * pCmd, bool & bSendPacket);

	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
//	void Base_AntiAim(CUserCmd * pCmd, IClientEntity * pLocal);
	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;
	IClientEntity * pTarget;
	bool IsLocked;

	int HitBox;
	Vector AimPoint;

};
extern CRageBot * ragebot;
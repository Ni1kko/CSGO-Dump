#pragma once

#include "main.h"

class CAntiAim
{
public:
	void Run( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd, bool & bSendPacket );
	void Pitch( CBaseEntity * pLocal, CUserCmd * pCmd );
	void FakeYaw( CBaseEntity * pLocal, CUserCmd * pCmd );
	float LowerBodyYaw( CBaseEntity * pLocal, float flReal, float flLowerBody );
	void Yaw( CBaseEntity * pLocal, CUserCmd * pCmd );
	float CanEdge( CBaseEntity * pLocal, CBaseEntity * pEnemy, CUserCmd * pCmd );
	void PredictLowerBodyUpdate( CBaseEntity * pLocal );
	bool m_bInAntiAim, m_bInFakeWalk, m_bShouldDrop, m_bInSlowWalk, m_bGroundAdjust;
	float m_flReal, m_flFake;

	bool m_bLeft, m_bRight, m_bBack, m_bFront, m_bLock;

	bool m_bLbyUpdate, m_bUpdated, m_bPreBreak, m_bBrokeThisTick;
	float m_flNextUpdate, m_flFirstUpdate, m_flLastMoveTime, m_flLastTried, m_flOldCurTime, m_flLastLowerBodyAngle;

	std::string m_szFreeStand;
	int m_iDropped, m_iFreeStand;
	CUserCmd* m_pCmd;
};
extern CAntiAim g_AntiAim;
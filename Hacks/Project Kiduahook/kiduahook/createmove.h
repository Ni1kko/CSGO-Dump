#pragma once

#include "main.h"

#define USERCMDOFFSET 0xEC
#define VERIFIEDCMDOFFSET 0xF0
#define MULTIPLAYER_BACKUP 150

struct sLocalData
{
/*	sLocalData::sLocalData(float flAnimTime, float flPitch, float flYaw, float flDuckAmount, int fFlags, Vector vVelocity)
	{
		m_flAnimTime = flAnimTime;
		m_flPitch = flPitch;
		m_flYaw = flYaw;
		m_flDuckAmount = flDuckAmount;
		m_fFlags = fFlags;
		m_vecVelocity = vVelocity;
	}*/

	float  m_flAnimTime;
	float  m_flPitch;
	float  m_flYaw;
	float  m_flDuckAmount;
	int    m_fFlags;
	Vector m_vecVelocity;
};

namespace UserCmd
{
	extern bool bQueuedShot;
	extern bool bInFakeDuck;
	extern bool bBreakingLC;
	extern int iOriginalFlags;
	extern int iTickShift;
	extern float flFixedCurTime;
	extern float flTeleportDistance;
	extern Vector vAntiAimReal;
	extern Vector vFakeAngles;
	extern Vector vOriginalView;
	extern Vector vViewAngles;
	extern Vector vHitPosition;
	extern CUserCmd* pCmd;
	extern std::vector<sLocalData> vLocalRecord;
	extern std::vector<int> sent_cmd_numbers;
}

bool __fastcall HOOKED_CreateMove( void* thisptr, int edx, float frametime, CUserCmd* pCmd );
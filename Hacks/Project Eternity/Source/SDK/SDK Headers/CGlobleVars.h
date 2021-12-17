#pragma once
#include "stdafx.h"

class INetChannel;
class CClientState
{
public:
	void ForceFullUpdate()
	{
		m_nDeltaTick = -1;
	}

	char              pad_0000[148];
	INetChannel*      m_NetChannel;
	char              pad_0098[8];
	uint32_t          m_nChallengeNr;
	char              pad_00A4[100];
	uint32_t          m_nSignonState;
	char              pad_010C[8];
	float             m_flNextCmdTime;
	uint32_t          m_nServerCount;
	uint32_t          m_nCurrentSequence;
	char              pad_0120[84];
	uint32_t          m_nDeltaTick;
	bool              m_bPaused;
	char              pad_0179[7];
	uint32_t          m_nViewEntity;
	uint32_t          m_nPlayerSlot;
	char              m_szLevelName[260];
	char              m_szLevelNameShort[40];
	char              m_szGroupName[40];
	char              pad_02DC[52];
	uint32_t          m_nMaxClients;
	char              pad_0314[18820];
	float             m_flLastServerTickTime;
	bool              insimulation;
	char              pad_4C9D[3];
	uint32_t          oldtickcount;
	float             m_tickRemainder;
	float             m_frameTime;
	uint32_t          lastoutgoingcommand;
	uint32_t          chokedcommands;
	uint32_t          last_command_ack;
	uint32_t          command_ack;
	uint32_t          m_nSoundSequence;
	char              pad_4CC0[80];
	Vector            viewangles;
	char              pad_4D1C[208];
};

class CGlobalVars
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float absoluteframestarttimestddev;
	float curtime;
	float frametime;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int simTicksThisFrame;
	int network_protocol;
	void* pSaveData;
	bool m_bClient;
	int nTimestampNetworkingBase;
	int nTimestampRandomizeWindow;
};

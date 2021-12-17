#pragma once

#include <cstdint>

#include "..\math\Vector.hpp"
#include "..\misc\bf_write.h"

#define GenDefineVFunc(...) ( this, __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return call_virtual< sig >( this, index ) GenDefineVFunc

class CClockDriftMgr {
public:
	float m_ClockOffsets[17];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class INetChannel {
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 m_nOutSequenceNr; //0x0018
	__int32 m_nInSequenceNr; //0x001C
	__int32 m_nOutSequenceNrAck; //0x0020
	__int32 m_nOutReliableState; //0x0024
	__int32 m_nInReliableState; //0x0028
	__int32 m_nChokedPackets; //0x002C

	VFUNC(49, Transmit(bool onlyreliable), bool(__thiscall*)(void*, bool))(onlyreliable)
};//Size=0x4294

class CClientState {
public:
	void ForceFullUpdate() {
		m_nDeltaTick = -1;
	};

	char pad_0000[156];             //0x0000
	INetChannel* m_NetChannel;          //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	uint32_t m_nDeltaTick;          //0x0178
	bool m_bPaused;                 //0x017C
	char pad_017D[3];               //0x017D
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[40];    //0x028C
	char m_szGroupName[40];         //0x02B4
	char pad_02DC[52];              //0x02DC
	uint32_t m_nMaxClients;         //0x0310
	char pad_0314[18820];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	uint32_t m_nLastOutgoingCommand;   //0x4CAC
	uint32_t m_nChokedCommands;        //0x4CB0
	uint32_t m_nLastCommandAck;      //0x4CB4
	uint32_t m_nCommandAck;           //0x4CB8
	uint32_t m_nSoundSequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	Vector viewangles;              //0x4D10
}; //Size: 0x4D1C
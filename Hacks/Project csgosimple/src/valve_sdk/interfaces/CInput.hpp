#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CInput
{
public:
	char pad_0000[172]; //0x0000
	bool N0000004E; //0x00AC
	bool m_fCameraInThirdPerson; //0x00AD
	char pad_00AE[2]; //0x00AE
	QAngle m_vecCameraOffset; //0x00B0
public:
	inline CUserCmd* GetUserCmd(int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
};

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
	return &(*(CUserCmd**)((DWORD)this + 0xF4))[sequence_number % MULTIPLAYER_BACKUP];

	//return &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
	return &(*(CVerifiedUserCmd**)((DWORD)this + 0xF8))[sequence_number % MULTIPLAYER_BACKUP];

	//return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
}


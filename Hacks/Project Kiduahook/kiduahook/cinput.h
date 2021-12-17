#pragma once

#include "sdk.h"

class CInput
{
public:
	char				pad_0x00[0x8];
	bool                m_fTrackIRAvailable;
	bool                m_fMouseInitialized;
	bool                m_fMouseActive;
	bool                m_fJoystickAdvancedInit;
	char                pad_0x08[0x2C];
	void*               m_pKeys;
	char                pad_0x38[0x64];
	int                 pad_0x41;
	int                 pad_0x42;
	bool                m_fCameraInterceptingMouse;
	bool                m_fCameraInThirdPerson;
	bool                m_fCameraMovingWithMouse;
	Vector              m_vecCameraOffset;
	bool                m_fCameraDistanceMove;
	int                 m_nCameraOldX;
	int                 m_nCameraOldY;
	int                 m_nCameraX;
	int                 m_nCameraY;
	bool                m_CameraIsOrthographic;
	Vector              m_angPreviousViewAngles;
	Vector              m_angPreviousViewAnglesTilt;
	float               m_flLastForwardMove;
	int                 m_nClearInputState;
	char                pad_0xE4[0x8];
	CUserCmd*           m_pCommands;
	CVerifiedUserCmd*   m_pVerifiedCommands;

	CUserCmd* GetUserCmd( int slot, int seq )
	{
		typedef CUserCmd* ( __thiscall* OriginalFn )( PVOID, int, int );
		return getvfunc<OriginalFn>( this, 8 )( this, slot, seq );
	}

	CUserCmd* CAM_ToThirdPerson( )
	{
		typedef CUserCmd* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 35 )( this );
	}

	CUserCmd* CAM_ToFirstPerson( )
	{
		typedef CUserCmd* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 36 )( this );
	}
};

extern CInput* g_pInput;
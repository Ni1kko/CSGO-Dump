#include "stdafx.h"

enum
{
	JOYSTICK_MAX_BUTTON_COUNT = 32,
	JOYSTICK_POV_BUTTON_COUNT = 4,
	JOYSTICK_AXIS_BUTTON_COUNT = 12,
};

#define JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_BUTTON + ((_joystick) * JOYSTICK_MAX_BUTTON_COUNT) + (_button) )
#define JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_POV_BUTTON + ((_joystick) * JOYSTICK_POV_BUTTON_COUNT) + (_button) )
#define JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_AXIS_BUTTON + ((_joystick) * JOYSTICK_AXIS_BUTTON_COUNT) + (_button) )

#define JOYSTICK_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) )
#define JOYSTICK_POV_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) )
#define JOYSTICK_AXIS_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) )

//INPUTw	
class CInput
{
public:
	char asdasd[8];

	void*               pvftable;                     //0x00
	bool                m_fTrackIRAvailable;          //0x04
	bool                m_fMouseInitialized;          //0x05
	bool                m_fMouseActive;               //0x06
	bool                m_fJoystickAdvancedInit;      //0x07
	char                pad_0x08[0x2C];               //0x08
	void*               m_pKeys;                      //0x34
	char                pad_0x38[0x64];               //0x38
	int                    pad_0x41;
	int                    pad_0x42;
	bool                m_fCameraInterceptingMouse;   //0x9C
	bool                m_fCameraInThirdPerson;       //0x9D
	bool                m_fCameraMovingWithMouse;     //0x9E
	Vector                m_vecCameraOffset;            //0xA0
	bool                m_fCameraDistanceMove;        //0xAC
	int                 m_nCameraOldX;                //0xB0
	int                 m_nCameraOldY;                //0xB4
	int                 m_nCameraX;                   //0xB8
	int                 m_nCameraY;                   //0xBC
	bool                m_CameraIsOrthographic;       //0xC0
	Vector              m_angPreviousViewAngles;      //0xC4
	Vector              m_angPreviousViewAnglesTilt;  //0xD0
	float               m_flLastForwardMove;          //0xDC
	int                 m_nClearInputState;           //0xE0
	char                pad_0xE4[0x8];                //0xE4

	class CUserCmd
	{
	public:
		CRC32_t GetChecksum( void ) const
		{
			CRC32_t crc;
			CRC32_Init( &crc );
			CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
			CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
			CRC32_ProcessBuffer( &crc, &viewangles, sizeof( viewangles ) );
			CRC32_ProcessBuffer( &crc, &aimdirection, sizeof( aimdirection ) );
			CRC32_ProcessBuffer( &crc, &forwardmove, sizeof( forwardmove ) );
			CRC32_ProcessBuffer( &crc, &sidemove, sizeof( sidemove ) );
			CRC32_ProcessBuffer( &crc, &upmove, sizeof( upmove ) );
			CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
			CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );
			CRC32_ProcessBuffer( &crc, &weaponselect, sizeof( weaponselect ) );
			CRC32_ProcessBuffer( &crc, &weaponsubtype, sizeof( weaponsubtype ) );
			CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
			CRC32_ProcessBuffer( &crc, &mousedx, sizeof( mousedx ) );
			CRC32_ProcessBuffer( &crc, &mousedy, sizeof( mousedy ) );
			CRC32_ProcessBuffer( &crc, &hasbeenpredicted, sizeof(hasbeenpredicted) );
			CRC32_Final( &crc );
			return crc;
		}

		BYTE u1[4];
		int command_number;
		int tick_count;
		Vector viewangles;
		Vector aimdirection;
		float forwardmove;
		float sidemove;
		float upmove;
		int buttons;
		BYTE impulse;
		int weaponselect;
		int weaponsubtype;
		int random_seed;
		short mousedx;
		short mousedy;
		bool hasbeenpredicted;
		Vector headangles;
		Vector headoffset;
	};

	class CVerifiedUserCmd
	{
	public:
		CUserCmd m_cmd;
		unsigned long m_crc;
	};

	CUserCmd* GetUserCmd( int slot, int seq )
	{
		typedef CUserCmd* (__thiscall* OriginalFn)( PVOID, int, int );
		return getvfunc< OriginalFn >( this, 9 )( this, slot, seq );
	}
};

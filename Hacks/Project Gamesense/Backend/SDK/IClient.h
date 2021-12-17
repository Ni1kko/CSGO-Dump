#pragma once
#include "../Include/GlobalIncludes.h"
#include "CGlobalVars.h"
#include "NetChannel.h"
#include "vfunc.hpp"

class ClientClass;

#define MAX_BUTTONSAMPLE			32768

#if !defined( _X360 )
#define INVALID_USER_ID		-1
#else
#define INVALID_USER_ID		XBX_INVALID_USER_ID
#endif

enum
{
	MAX_JOYSTICKS = 1,
	MOUSE_BUTTON_COUNT = 5,
	MAX_NOVINT_DEVICES = 2,
};

#if defined( LINUX )
enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_U,
	JOY_AXIS_R,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};
#else
enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_R,
	JOY_AXIS_U,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};
#endif

enum
{
	MS_WM_XBUTTONDOWN = 0x020B,
	MS_WM_XBUTTONUP = 0x020C,
	MS_WM_XBUTTONDBLCLK = 0x020D,
	MS_MK_BUTTON4 = 0x0020,
	MS_MK_BUTTON5 = 0x0040,
};

enum InputEventType_t
{
	IE_ButtonPressed = 0,	
	IE_ButtonReleased,		
	IE_ButtonDoubleClicked,	
	IE_AnalogValueChanged,	

	IE_FirstSystemEvent = 100,
	IE_Quit = IE_FirstSystemEvent,
	IE_ControllerInserted,	
	IE_ControllerUnplugged,	

	IE_FirstVguiEvent = 1000,
	IE_FirstAppEvent = 2000,
};

struct InputEvent_t
{
	int m_nType;				
	int m_nTick;				
	int m_nData;				
	int m_nData2;				
	int m_nData3;				
};

enum
{
	JOYSTICK_MAX_BUTTON_COUNT = 32,
	JOYSTICK_POV_BUTTON_COUNT = 4,
	JOYSTICK_AXIS_BUTTON_COUNT = MAX_JOYSTICK_AXES * 2,
};

#define JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_BUTTON + ((_joystick) * JOYSTICK_MAX_BUTTON_COUNT) + (_button) )
#define JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_POV_BUTTON + ((_joystick) * JOYSTICK_POV_BUTTON_COUNT) + (_button) )
#define JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_AXIS_BUTTON + ((_joystick) * JOYSTICK_AXIS_BUTTON_COUNT) + (_button) )

#define JOYSTICK_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) )
#define JOYSTICK_POV_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) )
#define JOYSTICK_AXIS_BUTTON( _joystick, _button ) ( (ButtonCode_t)JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) )

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,	
	MOUSE_WHEEL_DOWN,

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,

	JOYSTICK_FIRST = MOUSE_LAST + 1,

	JOYSTICK_FIRST_BUTTON = JOYSTICK_FIRST,
	JOYSTICK_LAST_BUTTON = JOYSTICK_BUTTON_INTERNAL(MAX_JOYSTICKS - 1, JOYSTICK_MAX_BUTTON_COUNT - 1),
	JOYSTICK_FIRST_POV_BUTTON,
	JOYSTICK_LAST_POV_BUTTON = JOYSTICK_POV_BUTTON_INTERNAL(MAX_JOYSTICKS - 1, JOYSTICK_POV_BUTTON_COUNT - 1),
	JOYSTICK_FIRST_AXIS_BUTTON,
	JOYSTICK_LAST_AXIS_BUTTON = JOYSTICK_AXIS_BUTTON_INTERNAL(MAX_JOYSTICKS - 1, JOYSTICK_AXIS_BUTTON_COUNT - 1),

	JOYSTICK_LAST = JOYSTICK_LAST_AXIS_BUTTON,

#if !defined ( _X360 )
	NOVINT_FIRST = JOYSTICK_LAST + 2, 

	NOVINT_LOGO_0 = NOVINT_FIRST,
	NOVINT_TRIANGLE_0,
	NOVINT_BOLT_0,
	NOVINT_PLUS_0,
	NOVINT_LOGO_1,
	NOVINT_TRIANGLE_1,
	NOVINT_BOLT_1,
	NOVINT_PLUS_1,

	NOVINT_LAST = NOVINT_PLUS_1,
#endif

	BUTTON_CODE_LAST,
	BUTTON_CODE_COUNT = BUTTON_CODE_LAST - KEY_FIRST + 1,

	KEY_XBUTTON_UP = JOYSTICK_FIRST_POV_BUTTON,
	KEY_XBUTTON_RIGHT,
	KEY_XBUTTON_DOWN,
	KEY_XBUTTON_LEFT,

	KEY_XBUTTON_A = JOYSTICK_FIRST_BUTTON,
	KEY_XBUTTON_B,
	KEY_XBUTTON_X,
	KEY_XBUTTON_Y,
	KEY_XBUTTON_LEFT_SHOULDER,
	KEY_XBUTTON_RIGHT_SHOULDER,
	KEY_XBUTTON_BACK,
	KEY_XBUTTON_START,
	KEY_XBUTTON_STICK1,
	KEY_XBUTTON_STICK2,

	KEY_XSTICK1_RIGHT = JOYSTICK_FIRST_AXIS_BUTTON,	
	KEY_XSTICK1_LEFT,							
	KEY_XSTICK1_DOWN,							
	KEY_XSTICK1_UP,								
	KEY_XBUTTON_LTRIGGER,						
	KEY_XBUTTON_RTRIGGER,						
	KEY_XSTICK2_RIGHT,							
	KEY_XSTICK2_LEFT,							
	KEY_XSTICK2_DOWN,							
	KEY_XSTICK2_UP,								
};

class IClient
{
	public:
		virtual int              Connect( CreateInterfaceFn appSystemFactory, CGlobalVars* pGlobals ) = 0;
		virtual int              Disconnect( void ) = 0;
		virtual int              Init( CreateInterfaceFn appSystemFactory, CGlobalVars* pGlobals ) = 0;
		virtual void             PostInit() = 0;
		virtual void             Shutdown( void ) = 0;
		virtual void             LevelInitPreEntity( char const* pMapName ) = 0;
		virtual void             LevelInitPostEntity() = 0;
		virtual void             LevelShutdown( void ) = 0;
		virtual ClientClass*     GetAllClasses( void ) = 0;
};

class CClockDriftMgr
{
	public:
		float m_ClockOffsets[17];   //0x0000
		uint32_t m_iCurClockOffset; //0x0044
		uint32_t m_nServerTick;     //0x0048
		uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class CClientState
{
public:
	void ForceFullUpdate()
	{
		iDeltaTick = -1;
	}

	class CClockDriftManager {

	public:
		float clock_offsets[0x10];
		uint32_t cur_clock_offset;
		uint32_t m_server_tick;
		uint32_t m_client_tick;
	};

public:
	char            pad000[0x9C];
	NetChannel* pNetChannel;
	int                m_nChallengeNr;
	char            pad001[0x4];
	double            m_connect_time;
	int                m_retry_number;
	char            pad002[0x54];
	int                m_nSignonState;
	char            pad003[0x4];
	double            flNextCmdTime;
	int                m_nServerCount;
	int                m_current_sequence;
	char            pad004[0x8];

	CClockDriftManager clock_drift_mgr;

	int                iDeltaTick;
	char            pad005[0x4];
	int                m_nViewEntity;
	int                m_nPlayerSlot;
	bool            m_bPaused;
	char            pad006[0x3];
	char            m_szLevelName[0x104];
	char            m_szLevelNameShort[0x28];
	char            pad007[0xD4];
	int                m_nMaxClients;
	char            pad008[0x4994];
	int                oldtickcount;
	float            m_tickRemainder;
	float            m_frameTime;
	int                nLastOutgoingCommand;
	int                iChokedCommands;
	int                nLastCommandAck;
	int                m_last_server_tick;
	int                m_command_ack;
	int                m_nSoundSequence;
	int                m_last_progress_percent;
	bool            m_is_hltv;
	char            pad009[0x4B];
	Vector            m_viewangles;
	char            pad010[0xCC];
	void* pEvents;
};
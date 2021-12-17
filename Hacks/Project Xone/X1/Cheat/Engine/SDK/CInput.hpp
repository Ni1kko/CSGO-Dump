#pragma once

#include "QAngle.hpp"
#include "CRC.hpp"
#include "ButtonCode.h"
//#include "SDK.h"

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)

namespace SDK
{
	class bf_read;
	class bf_write;

//	class INetChannel;
//	class INetMessage
//	{
//	public:
//		virtual	~INetMessage() {};
//
//		// Use these to setup who can hear whose voice.
//		// Pass in client indices (which are their ent indices - 1).
//
//		virtual void	SetNetChannel(INetChannel * netchan) = 0; // netchannel this message is from/for
//		virtual void	SetReliable(bool state) = 0;	// set to true if it's a reliable message
//
//		virtual bool	Process(void) = 0; // calles the recently set handler to process this message
//
//		virtual	bool	ReadFromBuffer(bf_read &buffer) = 0; // returns true if parsing was OK
//		virtual	bool	WriteToBuffer(bf_write &buffer) = 0;	// returns true if writing was OK
//
//		virtual bool	IsReliable(void) const = 0;  // true, if message needs reliable handling
//
//		virtual int				GetType(void) const = 0; // returns module specific header tag eg svc_serverinfo
//		virtual int				GetGroup(void) const = 0;	// returns net message group of this message
//		virtual const char		*GetName(void) const = 0;	// returns network message name, eg "svc_serverinfo"
//		virtual INetChannel		*GetNetChannel(void) const = 0;
//		virtual const char		*ToString(void) const = 0; // returns a human readable string about message content
//	};
//	class	IDemoRecorder;
//	class	INetMessage;
//	class	INetChannelHandler;
//	class	INetChannelInfo;
//	class	INetMessageBinder;
//	class INetChannel
//	{
//	public:
//
//		virtual	~INetChannel(void) {};
//
//		virtual void	SetDataRate(float rate) = 0;
//		virtual bool	RegisterMessage(INetMessageBinder *msg) = 0;
//		virtual bool	UnregisterMessage(INetMessageBinder *msg) = 0;
//		virtual bool	StartStreaming(unsigned int challengeNr) = 0;
//		virtual void	ResetStreaming(void) = 0;
//		virtual void	SetTimeout(float seconds, bool bForceExact = false) = 0;
//		virtual void	SetDemoRecorder(IDemoRecorder *recorder) = 0;
//		virtual void	SetChallengeNr(unsigned int chnr) = 0;
//
//		virtual void	Reset(void) = 0;
//		virtual void	Clear(void) = 0;
//		virtual void	Shutdown(const char *reason) = 0;
//
//		virtual void	ProcessPlayback(void) = 0;
//		virtual bool	ProcessStream(void) = 0;
//		virtual void	ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;
//
//		virtual bool	SendNetMsg(INetMessage &msg, bool bForceReliable = false, bool bVoice = false) = 0;
//#ifdef POSIX
//		FORCEINLINE bool SendNetMsg(INetMessage const &msg, bool bForceReliable = false, bool bVoice = false) { return SendNetMsg(*((INetMessage *)&msg), bForceReliable, bVoice); }
//#endif
//		virtual bool	SendData(bf_write &msg, bool bReliable = true) = 0;
//		virtual bool	SendFile(const char *filename, unsigned int transferID, bool isReplayDemo) = 0;
//		virtual void	DenyFile(const char *filename, unsigned int transferID, bool isReplayDemo) = 0;
//		virtual void	RequestFile_OLD(const char *filename, unsigned int transferID) = 0;	// get rid of this function when we version the 
//		virtual void	SetChoked(void) = 0;
//		virtual int		SendDatagram(bf_write *data) = 0;
//		virtual bool	Transmit(bool onlyReliable = false) = 0;
//		//char pad_0000[20];           //0x0000
//		//bool m_bProcessingMessages;  //0x0014
//		//bool m_bShouldDelete;        //0x0015
//		//char pad_0016[2];            //0x0016
//		//int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
//		//int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
//		//int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
//		//int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
//		//int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
//		//int32_t m_nChokedPackets;    //0x002C number of choked packets
//		//virtual bool SendNetMsg(INetMessage &msg, bool bForceReliable = false, bool bVoice = false) = 0;
//		///*bool SendNetMsg(INetMessage* msg, bool bForceReliable, bool bVoice)
//		//{
//		//	typedef bool(__thiscall* func1)(void*, INetMessage*, bool, bool);
//		//	return GetVFunc<func1>(this, 42)(this, msg, bForceReliable, bVoice);
//		//}*/
//
//		///*void Transmit(bool onlyReliable)
//		//{
//		//	typedef void(__thiscall* func2)(void*, bool);
//
//		//	return GetVFunc<func2>(this, 42)(this, onlyReliable);
//		//}*/
//
//		//virtual bool Transmit(bool onlyReliable = false) = 0;
//		//char pad_0030[1044];         //0x0030
//	}; //Size: 0x0444

	class CUserCmd {
	public:
		virtual ~CUserCmd() {};

		CRC32_t GetChecksum( void ) const
		{
			CRC32_t crc;
			CRC32_Init( &crc );

			CRC32_ProcessBuffer( &crc , &command_number , sizeof( command_number ) );
			CRC32_ProcessBuffer( &crc , &tick_count , sizeof( tick_count ) );
			CRC32_ProcessBuffer( &crc , &viewangles , sizeof( viewangles ) );
			CRC32_ProcessBuffer( &crc , &aimdirection , sizeof( aimdirection ) );
			CRC32_ProcessBuffer( &crc , &forwardmove , sizeof( forwardmove ) );
			CRC32_ProcessBuffer( &crc , &sidemove , sizeof( sidemove ) );
			CRC32_ProcessBuffer( &crc , &upmove , sizeof( upmove ) );
			CRC32_ProcessBuffer( &crc , &buttons , sizeof( buttons ) );
			CRC32_ProcessBuffer( &crc , &impulse , sizeof( impulse ) );
			CRC32_ProcessBuffer( &crc , &weaponselect , sizeof( weaponselect ) );
			CRC32_ProcessBuffer( &crc , &weaponsubtype , sizeof( weaponsubtype ) );
			CRC32_ProcessBuffer( &crc , &random_seed , sizeof( random_seed ) );
			CRC32_ProcessBuffer( &crc , &mousedx , sizeof( mousedx ) );
			CRC32_ProcessBuffer( &crc , &mousedy , sizeof( mousedy ) );

			CRC32_Final( &crc );
			return crc;
		}

		int       command_number;     // 0x04
		int       tick_count;         // 0x08
		QAngle    viewangles;         // 0x0C
		Vector    aimdirection;       // 0x18
		float     forwardmove;        // 0x24
		float     sidemove;           // 0x28
		float     upmove;             // 0x2C
		int       buttons;            // 0x30
		byte      impulse;            // 0x34
		int       weaponselect;       // 0x38
		int       weaponsubtype;      // 0x3C
		int       random_seed;        // 0x40
		short     mousedx;            // 0x44
		short     mousedy;            // 0x46
		bool      hasbeenpredicted;   // 0x48
		char      pad_0x4C[0x18];     // 0x4C
	};

	class CVerifiedUserCmd
	{
	public:
		CUserCmd  m_cmd;
		CRC32_t   m_crc;
	};

	class CClockDriftMgr
	{
	public:
		float m_ClockOffsets[16];   //0x0000
		uint32_t m_iCurClockOffset; //0x0044
		uint32_t m_nServerTick;     //0x0048
		uint32_t m_nClientTick;     //0x004C
	}; //Size: 0x0050

	//class CClientState
	//{
	//public:
	//	void ForceFullUpdate()
	//	{
	//		m_nDeltaTick = -1;
	//	}

	//	char pad_0000[156];             //0x0000
	////	INetChannel * m_NetChannel;			//0x009C
	//	uint32_t m_nChallengeNr;        //0x00A0
	//	char pad_00A4[100];             //0x00A4
	//	uint32_t m_nSignonState;        //0x0108
	//	char pad_010C[8];               //0x010C
	//	float m_flNextCmdTime;          //0x0114
	//	uint32_t m_nServerCount;        //0x0118
	//	uint32_t m_nCurrentSequence;    //0x011C
	//	char pad_0120[8];               //0x0120
	//	CClockDriftMgr m_ClockDriftMgr; //0x0128
	//	uint32_t m_nDeltaTick;          //0x0178
	//	bool m_bPaused;                 //0x017C
	//	char pad_017D[3];               //0x017D
	//	uint32_t m_nViewEntity;         //0x0180
	//	uint32_t m_nPlayerSlot;         //0x0184
	//	char m_szLevelName[260];        //0x0188
	//	char m_szLevelNameShort[40];    //0x028C
	//	char m_szGroupName[40];         //0x02B4
	//	char pad_02DC[56];              //0x02DC
	//	uint32_t m_nMaxClients;         //0x0310
	//	char pad_0314[18940];           //0x0314
	//	Vector viewangles;              //0x4D10
	//}; //Size: 0x4D1C


	class CInput
	{
	public:
		void*               pvftable;                     //0x00
		bool                m_fTrackIRAvailable;          //0x04
		bool                m_fMouseInitialized;          //0x05
		bool                m_fMouseActive;               //0x06
		bool                m_fJoystickAdvancedInit;      //0x07
		char                pad_0x08[0x2C];               //0x08
		void*               m_pKeys;                      //0x34
		char                pad_0x38[0x64];               //0x38
		bool                m_fCameraInterceptingMouse;   //0x9C
		bool                m_fCameraInThirdPerson;       //0x9D
		bool                m_fCameraMovingWithMouse;     //0x9E
		Vector              m_vecCameraOffset;            //0xA0
		bool                m_fCameraDistanceMove;        //0xAC
		int                 m_nCameraOldX;                //0xB0
		int                 m_nCameraOldY;                //0xB4
		int                 m_nCameraX;                   //0xB8
		int                 m_nCameraY;                   //0xBC
		bool                m_CameraIsOrthographic;       //0xC0
		QAngle              m_angPreviousViewAngles;      //0xC4
		QAngle              m_angPreviousViewAnglesTilt;  //0xD0
		float               m_flLastForwardMove;          //0xDC
		int                 m_nClearInputState;           //0xE0
		char                pad_0xE4[0x8];                //0xE4
		CUserCmd*           m_pCommands;                  //0xEC
		CVerifiedUserCmd*   m_pVerifiedCommands;          //0xF0
	};

	class IInputSystem
	{
	public:
		void EnableInput( bool bEnable )
		{
			VirtualFn( void* )( PVOID , bool );
			GetMethod< OriginalFn >( this , 11 )( this , bEnable );
		}

		bool IsButtonDown( ButtonCode_t code )
		{
			VirtualFn( bool )( PVOID , ButtonCode_t );
			return GetMethod< OriginalFn >( this , 15 )( this , code );
		}

		void ResetInputState()
		{
			VirtualFn( void )( PVOID );
			GetMethod< OriginalFn >( this , 39 )( this );
		}

		ButtonCode_t VirtualKeyToButtonCode( int nVirtualKey )
		{
			VirtualFn( ButtonCode_t )( PVOID , int );
			return GetMethod< OriginalFn >( this , 44 )( this , nVirtualKey );
		}

		int ButtonCodeToVirtualKey( ButtonCode_t code )
		{
			VirtualFn( int )( PVOID , ButtonCode_t );
			return GetMethod< OriginalFn >( this , 45 )( this , code );
		}

		void GetCursorPosition( int* m_pX , int* m_pY )
		{
			VirtualFn( void )( PVOID , int* , int* );
			GetMethod< OriginalFn >( this , 56 )( this , m_pX , m_pY );
		}
	};
}
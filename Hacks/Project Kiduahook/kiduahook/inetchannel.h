#pragma once

#include "sdk.h"

class NET_SetConVar
{
public:
	DWORD PatternSearchIDA( std::string moduleName, std::string pattern )
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = ( DWORD )GetModuleHandleA( moduleName.c_str() );
		MODULEINFO miModInfo; GetModuleInformation( GetCurrentProcess(), ( HMODULE )rangeStart, &miModInfo, sizeof( MODULEINFO ) );
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for( DWORD pCur = rangeStart; pCur < rangeEnd; pCur++ )
		{
			if( !*pat )
				return firstMatch;

			if( *( PBYTE )pat == '\?' || *( BYTE* )pCur == getByte( pat ) )
			{
				if( !firstMatch )
					firstMatch = pCur;

				if( !pat[ 2 ] )
					return firstMatch;

				if( *( PWORD )pat == '\?\?' || *( PBYTE )pat != '\?' )
					pat += 3;

				else
					pat += 2;    //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}
	NET_SetConVar( const char* name, const char* value )
	{
		typedef void( __thiscall* oConstructor )( PVOID );
		static oConstructor Constructor = ( oConstructor )( PatternSearchIDA( "engine.dll", "FF CC CC CC CC CC CC CC CC CC CC CC CC CC CC 56 8B F1 C7 06 ? ? ? ? 8D" ) + 15 );

		Constructor( this );

		typedef void( __thiscall* oInit )( PVOID, const char*, const char* );
		static oInit Init = ( oInit )( PatternSearchIDA( "engine.dll", "55 8B EC 56 8B F1 83 4E 14 01 83 7E 0C 00" ) ); // Search for Custom user info value 

		Init( this, name, value );
	}

	~NET_SetConVar()
	{
		typedef void( __thiscall* oDestructor )( PVOID );
		static oDestructor Destructor = ( oDestructor )( PatternSearchIDA( "engine.dll", "00 CC CC CC CC CC CC CC CC CC CC CC CC CC 53 8B D9 56 57" ) + 14 ); // Search for Custom user info value 

		Destructor( this );
	}

private:
	DWORD pad0[ 13 ];
};

#define NET_FRAMES_BACKUP	64		// must be power of 2
#define NET_FRAMES_MASK		(NET_FRAMES_BACKUP-1)

class CNetChannel
{
public:

	uint8_t pad_0x0000[ 0x18 ];
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;

	int SendDatagram(void *data)
	{
		typedef int(__thiscall* fn)(void*, void*);
		return getvfunc<fn>(this, 46)(this, data);
	}

	bool Transmit(bool onlyReliable)
	{
		typedef bool(__thiscall* fn)(void*, bool);
		return getvfunc<fn>(this, 49)(this, onlyReliable);
	}
};

class INetChannelInfo
{
public:
	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYErs,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual void		PlaceHolder1( void ) const = 0;
	virtual const char  *GetName( void ) const = 0;	// get channel name
	virtual const char  *GetAddress( void ) const = 0; // get channel IP address as string
	virtual float		GetTime( void ) const = 0;	// current net time
	virtual float		GetTimeConnected( void ) const = 0;	// get connection time in seconds
	virtual int			GetBuffersize( void ) const = 0;	// netchannel packet history size
	virtual int			GetDataRate( void ) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback( void ) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut( void ) const = 0;	// true if timing out
	virtual bool		IsPlayback( void ) const = 0;	// true if demo playback

	virtual float		GetLatency( int flow ) const = 0;	 // current latency (RTT), more accurate but jittering

	virtual float		GetAvgLatency( int flow ) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss( int flow ) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke( int flow ) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData( int flow ) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets( int flow ) const = 0; // avg packets/sec
	virtual int			GetTotalData( int flow ) const = 0;	 // total flow in/out in bytes
	virtual int			GetTotalPackets( int flow ) const = 0;
	virtual int			GetSequenceNr( int flow ) const = 0;	// last send seq number
	virtual bool		IsValidPacket( int flow, int frame_number ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime( int flow, int frame_number ) const = 0; // time when packet was send
	virtual int			GetPacketBytes( int flow, int frame_number, int group ) const = 0; // group size of this packet
	virtual bool		GetStreamProgress( int flow, int *received, int *total ) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived( void ) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount( int flow, int frame_number ) const = 0;
	virtual void		GetPacketResponseLatency( int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke ) const = 0;
	virtual void		GetRemoteFramerate( float *pflFrameTime, float *pflFrameTimeStdDeviation, float *pflFrameStartTimeStdDeviation ) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;

	bool SendNetMsg( NET_SetConVar &msg, bool bForceReliable = false, bool bVoice = false )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID, NET_SetConVar&, bool, bool );
		return getvfunc<OriginalFn>( this, 42 )( this, msg, bForceReliable, bVoice );
	}
};
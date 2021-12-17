#pragma once

#include "sdk.h"

class VEngineClient
{
public:
	typedef struct player_info_s
	{
		unsigned long long      unknown;
		unsigned long long      xuid;
		char                    name[ 128 ];
		int                     userID;
		char                    guid[ 32 + 1 ];
		unsigned int            friendsID;
		char                    friendsName[ 128 ];
		bool                    fakeplayer;
		bool                    ishltv;
		unsigned long           customFiles[ 4 ];
		unsigned char           filesDownloaded;
	} player_info_t;

	void GetScreenSize( int& width, int& height )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int&, int& );
		return getvfunc<OriginalFn>( this, 5 )( this, width, height );
	}

	void ClientCmd( const char *szCmdString, ... )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const char* );
		getvfunc<OriginalFn>( this, 7 )( this, szCmdString );
	}

	bool GetPlayerInfo( int iIndex, player_info_t *pInfo )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID, int, player_info_t* );
		return getvfunc<OriginalFn>( this, 8 )( this, iIndex, pInfo );
	}

	int GetPlayerForUserID( int iUserId )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 9 )( this, iUserId );
	}

	int GetLocalPlayer( void )
	{
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 12 )( this );
	}

	float GetLastTimeStamp()
	{
		typedef float( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 14 )( this );
	}

	void GetViewAngles( Vector& vAngles )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, Vector& );
		getvfunc<OriginalFn>( this, 18 )( this, vAngles );
	}

	void SetViewAngles( Vector& vAngles )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, Vector& );
		getvfunc<OriginalFn>( this, 19 )( this, vAngles );
	}

	int GetMaxClients( void )
	{
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 20 )( this );
	}

	bool IsInGame( void )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 26 )( this );
	}

	bool IsConnected( void )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 27 )( this );
	}

	matrix3x4_t& WorldToScreenMatrix()
	{
		typedef matrix3x4_t&( __thiscall *OrigFn )( void* );
		return getvfunc<OrigFn>( this, 37 )( this );
	}

	ISpatialQuery* GetBSPTreeQuery(void)
	{
		typedef ISpatialQuery*(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 43)(this);
	}

	bool TakingScreen()
	{
		typedef bool( __thiscall* OriginalFn )( void* );
		return getvfunc<OriginalFn>( this, 92 )( this );
	}

	void ExecuteClientCmd( char const* szCommand )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, char const* szCommand );
		getvfunc<OriginalFn>( this, 108 )( this, szCommand );
	}

	void UnrestrictedClientCmd( char const* szCommand )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, char const* szCommand );
		getvfunc<OriginalFn>( this, 115 )( this, szCommand );
	}

	bool IsVoiceRecording()
	{
		typedef bool(__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 224)(this);
	}

	INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo*( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 78 )( this );
	}

	CNetChannel* GetNetChannel()
	{
		typedef CNetChannel*( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 78 )( this );
	}
};

extern VEngineClient* g_pEngine;
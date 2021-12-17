#pragma once

#include "sdk.h"

class IRecipientFilter
{
public:
	virtual ~IRecipientFilter() {}

	bool IsReliable()
	{
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 1 )( this );
	}
	bool IsInitMessage()
	{
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 2 )( this );
	}
	int GetRecipientCount()
	{
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 3 )( this );
	}
	int GetRecipientIndex( int slot )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, slot );
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sounds
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Sounds
{
	CHAN_REPLACE = -1,
	CHAN_AUTO = 0,
	CHAN_WEAPON = 1,
	CHAN_VOICE = 2,
	CHAN_ITEM = 3,
	CHAN_BODY = 4,
	CHAN_STREAM = 5,        // allocate stream channel from the static or dynamic area
	CHAN_STATIC = 6,        // allocate channel from the static area 
	CHAN_VOICE2 = 7,
	CHAN_VOICE_BASE = 8,        // allocate channel for network voice data
	CHAN_USER_BASE = ( CHAN_VOICE_BASE + 128 )        // Anything >= this number is allocated to game code.
};
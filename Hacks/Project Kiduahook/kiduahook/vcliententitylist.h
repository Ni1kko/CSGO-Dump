#pragma once

#include "sdk.h"

class VClientEntityList
{
public:

	CBaseEntity* GetClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 3 )( this, entnum );
	}

	CBaseEntity* GetClientEntityFromHandle( int hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}

	CBaseEntity* GetClientEntityFromHandle( HANDLE hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, HANDLE );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}

	int GetHighestEntityIndex( void )
	{
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 6 )( this );
	}
};

extern VClientEntityList* g_pEntList;
#pragma once

#include "sdk.h"

class HLCLient
{
public:
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 8 )( this );
	}
	CClientClass* AllClasses( void )
	{
		typedef CClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 8 )( this );
	}
};

extern HLCLient* g_pClient;
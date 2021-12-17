#pragma once

#include "sdk.h"

class IMoveHelper
{
public:
	void SetHost( CBaseEntity *pPlayer )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, CBaseEntity *pPlayer );
		getvfunc<OriginalFn>( this, 1 )( this, pPlayer );
	}
};

extern IMoveHelper* g_pMoveHelper;
#pragma once

#include "sdk.h"

class CBaseClientState
{
public:
	void ForceFullUpdate()
	{
		*reinterpret_cast< int* >( uintptr_t( this ) + 0x174 ) = -1;
	};


	OFFSET( GetNetChannel, CNetChannel*, 0x009C );
	OFFSET(GetDeltaTick, int, 0x174);

	// v30 = *(_DWORD *)(v3 + 0x4D24) + 1 + *(_DWORD *)(v3 + 0x4D28); first outgoing cmd second chokedcmds

	//*(_DWORD *)(clientstate[0] + 0x4D28) = 0; choked
	//(_DWORD *)(clientstate[0] + 0x4D24) = -1; lastoutgoing
	//Broadcast Player: failed to create demo net channel\n
	OFFSET( GetLastOutGoingCmd, int, 0x4D24 );
	OFFSET( GetChokedCmds, int, 0x4D28 );
	OFFSET(GetLastCmdAck, int, 0x4D2C);
};

extern CBaseClientState* g_pClientState;
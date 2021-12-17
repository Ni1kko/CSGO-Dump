#include "debug.h"

CDebug g_pDebug;

bool CDebug::AttachDebugConsole( void )
{
	FILE* g_pConStream;

	if( !AttachConsole( ATTACH_PARENT_PROCESS ) )
	{
		if( !AllocConsole() )
			return false;
	}

	if( !SetConsoleTitle( "Debug" ) )
		return false;

	errno_t err = freopen_s( &g_pConStream, "CONOUT$", "w", stdout );

	if( err != 0 )
		return false;

	return true;
}
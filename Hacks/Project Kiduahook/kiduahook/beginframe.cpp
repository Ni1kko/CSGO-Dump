#include "beginframe.h"
#include <future>

BeginFrameFn oBeginFrame;
void __fastcall HOOKED_BeginFrame( void* thisptr )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr || pLocal->GetHealth() <= 0 || !g_pEngine->IsInGame() )
	{
		g_BulletImpact.clear();
		oBeginFrame( thisptr );
		return;
	}

	g_Hitmarker.InitTrace();

	oBeginFrame( thisptr );
}

IsBoxVisibleFn oIsBoxVisible;

int	__stdcall HOOKED_IsBoxVisible( const Vector& mins, const Vector& maxs )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( !pLocal || pLocal->GetHealth() <= 0 || !g_pEngine->IsInGame() )
		return oIsBoxVisible( mins, maxs );

	if( !memcmp( _ReturnAddress(), "\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C", 10 ) )
		return 1;

	return oIsBoxVisible( mins, maxs );
}
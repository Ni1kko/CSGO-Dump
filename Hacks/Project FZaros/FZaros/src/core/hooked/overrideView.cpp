#include "../../utilities/includes.h"
#include "../../menu/menu.hpp"

void __fastcall CHookManager::overrideView( void* ecx, void* edx, CViewSetup* pSetup )
{
	static auto original = g_Hooks->clientModeHook->getOriginalFunction<decltype( &overrideView )>( 18 );

	if ( !g_Interfaces->gameEngine->connected( ) && !g_Interfaces->gameEngine->inGame( ) )
		return;

	if ( !Globals::localPlayer )
		return;

	if ( !Globals::localPlayer->alive( ) )
		return;

	
	if (config->get_bool("espRemovalsRecoil"))
	{
		Vector3 viewPunch = Globals::localPlayer->viewPunchAngle( );
		Vector3 aimPunch = Globals::localPlayer->aimPunchAngle( );

		pSetup->angles[ 0 ] -= ( viewPunch[ 0 ] + ( aimPunch[ 0 ] * 2 * 0.4499999f ) );
		pSetup->angles[ 1 ] -= ( viewPunch[ 1 ] + ( aimPunch[ 1 ] * 2 * 0.4499999f ) );
		pSetup->angles[ 2 ] -= ( viewPunch[ 2 ] + ( aimPunch[ 2 ] * 2 * 0.4499999f ) );
	}

	if ( !Globals::localPlayer->scoped( ) )
		pSetup->fov = config->get_float("miscFoV");
		//pSetup->fov = config->get_float( "miscFoV", 90.f );
	
	if (config->get_bool("espRemovalsZoom") && Globals::localPlayer->activeWeapon( ) && Globals::localPlayer->activeWeapon( )->zoomLevel( ) )
		pSetup->fov = config->get_float("miscFoV");
		//pSetup->fov = config->get_float( "miscFoV", 90.f );

	original( ecx, edx, pSetup );
}
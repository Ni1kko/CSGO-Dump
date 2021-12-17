#include "../managers/hookManager.hpp"
#include "../../features/featureManager.hpp"
#include "../../features/Misc/Misc.hpp"
#include "../../features/Aimbot/Aimbot.hpp"
#include "../../features/Anti-Aim/Anti-aim.hpp"
#include "../../features/Aimbot/Resolver.hpp"
#include "../../features/Anti-aim/Fakelag.hpp"
#include "../../features/Misc/enginePrediction.hpp"
#include "../../features/Misc/grenadePrediction.hpp"
#include "../../features/Misc/Animations.hpp"

bool __fastcall CHookManager::onMove( void *ecx, void *edx, float sampleTime, CUserCmd *userCmd )
{
	static auto originalFn = g_Hooks->clientModeHook->getOriginalFunction<decltype( &onMove )>( 24 );

	originalFn( ecx, edx, sampleTime, userCmd );

	if ( !userCmd || !userCmd->commandNumber )
		return originalFn( ecx, edx, sampleTime, userCmd );

	uintptr_t* pebp;
	__asm mov pebp, ebp;
	auto sendPacket = ( bool* ) ( *pebp - 0x1C );

	Globals::bSendPacket = true;
	Globals::oCmd = userCmd;
	Globals::localPlayer = g_Interfaces->clientEntity->GetClientEntity( g_Interfaces->gameEngine->getLocalPlayer( ) );

	if ( !Globals::localPlayer )
		return originalFn;

	Globals::originalViewAngle = Globals::oCmd->viewAngles;

	if ( g_Interfaces->gameEngine->inGame( ) )
	{

		//thanks chance... :^)
		if ( Globals::localPlayer->lifeState( ) || Globals::localPlayer->moveType( ) == movetype_ladder )
			Globals::localPlayer->clientSideAnimation( ) = true;
		else
		{
			const float oldCurtime = g_Interfaces->globalVars->curtime;
			const float oldFrametime = g_Interfaces->globalVars->frametime;
			const float intervalPerTick = g_Interfaces->globalVars->intervalPerTick;
			const int tickbase = Globals::localPlayer->tickBase( );

			g_Interfaces->globalVars->frametime = intervalPerTick;
			g_Interfaces->globalVars->curtime = tickbase * intervalPerTick;

			if (config->get_bool("espGrenadePred"))
				g_GrenadePred.think( userCmd );

			if (config->get_bool("rageChokeEnable"))
				g_FakeLag->createMove( );

			if (config->get_bool("miscClanTag"))
				g_Misc.clanTag( ); // TODO: make it so doesnt fuck with fakelag

			g_Features->onMove( sampleTime, userCmd );

			if (config->get_bool("rageAntiAimCorrection"))
				g_Resolver.OnCreateMove();

			g_EnginePrediction.run( userCmd );
			{
				if (config->get_bool("aaEnable"))
					g_AntiAim.onMove( userCmd );

				if (config->get_bool("rageEnable"))
					g_Aimbot.creatMove( );
			}

			Globals::oCmd->buttons &= ~( IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT );

			g_Misc.movementFix( Globals::originalViewAngle );

			if (config->get_bool("miscAntiUT"));
				g_Math.clamp( Globals::oCmd->viewAngles );

			static Vector3 last_choked_angles;

			if ( !Globals::bSendPacket && g_Interfaces->clientState->ChokedCommands == 0 ) // we are choking a command, and we have not already choked a command
				last_choked_angles = userCmd->viewAngles; // save our first choked angles for later use
			else if ( Globals::bSendPacket && g_Interfaces->clientState->ChokedCommands > 0 ) // if we are sending a command, and we have previously choked commands
				Globals::realAngle = last_choked_angles; // apply the first choked angles
			else if ( Globals::bSendPacket && g_Interfaces->clientState->ChokedCommands == 0 ) // if we are sending a command, and have not choked any commands
				Globals::realAngle = userCmd->viewAngles; // apply the sent angles
			else
				Globals::fakeAngle = userCmd->viewAngles;

			if (Globals::bSendPacket)
			{
				Globals::chokedPackets = 0;
				Globals::fakePosition = Globals::localPlayer->GetAbsOrigin();
			}
			else
			{
				++Globals::chokedPackets;
			}

			g_Interfaces->globalVars->frametime = oldFrametime;
			g_Interfaces->globalVars->curtime = oldCurtime;
		}
	}
	*sendPacket = Globals::bSendPacket;

	return false;
}
#include "../managers/hookManager.hpp"
#include "../../features/featureManager.hpp"
#include "../../features/Aimbot/LagCompensation.hpp"
#include "../../features/Misc/Dormancy.hpp"
#include "../../features/Misc/Animations.hpp"

void __fastcall CHookManager::onFrameStage( void *ecx, void *edx, clientFrameStage_t frameStage )
{
	g_LagCompensation.clearRecords( );

	if ( g_Interfaces->gameEngine->inGame( ) )
	{
		g_Features->onFrameStage( frameStage );

		if ( frameStage == FRAME_RENDER_START )
		{
			if ( !g_Interfaces->gameEngine->connected( ) && !g_Interfaces->gameEngine->inGame( ) )
				return;

			if ( !Globals::localPlayer )
				return;

			for ( int i = 1; i < g_Interfaces->globalVars->maxClients; ++i )
			{
				CBaseEntity* pEntity = g_Interfaces->clientEntity->GetClientEntity( i );

				if ( !pEntity || !pEntity->alive( ) || pEntity->IsDormant( ) )
					continue;

				if ( pEntity->team( ) == Globals::localPlayer->team( ) )
					continue;

				if ( pEntity == Globals::localPlayer )
					continue;

				if ( !Globals::localPlayer->alive( ) )
					continue;

				g_LagCompensation.storeRecords( pEntity );
			}
		}

		if ( frameStage == FRAME_NET_UPDATE_POSTDATAUPDATE_END )
		{
			if ( !g_Interfaces->gameEngine->connected( ) && !g_Interfaces->gameEngine->inGame( ) )
				return;

			if ( !Globals::localPlayer )
				return;

			for ( int i = 1; i < g_Interfaces->globalVars->maxClients; ++i )
			{
				CBaseEntity* pEntity = g_Interfaces->clientEntity->GetClientEntity( i );

				if ( !pEntity || !pEntity->alive( ) || pEntity->IsDormant( ) )
					continue;

				if ( pEntity->team( ) == Globals::localPlayer->team( ) )
					continue;

				if ( pEntity == Globals::localPlayer )
					continue;

				if ( !Globals::localPlayer->alive( ) )
					continue;

				VarMapping_t* map = pEntity->varMapping( );

				if ( !map )
					continue;

				for ( int j = 0; j < map->m_nInterpolatedEntries; ++j )
				{

					map->m_Entries[ j ].m_bNeedsToInterpolate = !config->get_bool("rageBacktrack");
				}
			}
		}
	}

	static auto originalFn = g_Hooks->baseClientHook->getOriginalFunction<decltype( &onFrameStage )>( 37 );
	originalFn( ecx, edx, frameStage );
}

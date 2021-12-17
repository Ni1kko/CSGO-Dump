#include "dormancy.hpp"

#include <iostream>
#include <fstream>

#define range( x, a, b )    ( x >= a && x <= b )

void dormancyManager::positionCorrection( int stage ) {
	if ( stage != FRAME_RENDER_START )
		return;

	for ( int i = 1; i <= g_Interfaces->globalVars->maxClients; i++ ) {
		auto entity = g_Interfaces->clientEntity->GetClientEntity( i );

		if ( entity == Globals::localPlayer )
			continue;

		if ( !viable( entity ) )
			continue;

		if ( !entity->IsDormant( ) && !players[ i ].set )
			players[ i ].oldpos = entity->vecOrigin( );

		if ( !entity->IsDormant( ) || players[ i ].set )
			continue;

		entity->setAbsOrigin( players[ i ].oldpos );
	}
}

bool dormancyManager::set( const int index ) {
	if ( !range( index, 1, 64 ) )
		return false;

	return players[ index ].set;
}

bool dormancyManager::viable( CBaseEntity* entity ) {
	if ( !entity )
		return false;

	if ( !Globals::localPlayer )
		return false;

	if ( !entity->alive( ) || entity->flags( ) & ( 1 << 6 ) )
		return false;

	if ( entity->team( ) == Globals::localPlayer->team( ) )
		return false;

	if ( entity->GetClientClass( )->iClassID != 40 )
		return false;

	if ( entity->immunity( ) )
		return false;

	return true;
}

void dormancyManager::soundDormancy( ) {
	soundList.RemoveAll( );
	g_Interfaces->engineSound->getActiveSounds( soundList );

	if ( !soundList.Count( ) )
		return;

	for ( int i = 0; i < soundList.Count( ); i++ ) {
		soundInfo& sound = soundList[ i ];
		if ( !range( sound.sound_source, 1, 64 ) )
			continue;

		if ( sound.sound_source == g_Interfaces->gameEngine->getLocalPlayer( ) )
			continue;

		auto entity = reinterpret_cast< CBaseEntity* >( g_Interfaces->clientEntity->GetClientEntity( sound.sound_source ) );

		if ( !entity || sound.origin->IsZero( ) || !validSound( sound ) )
			continue;

		auto& player = players[ sound.sound_source ];
		player.entity = entity;

	
		Vector3 src_3d, dst_3d;
		CGameTrace tr;
		C_Ray ray;
		CTraceFilter filter;

		filter.pSkip1 = entity;
		src_3d = ( *sound.origin ) + Vector3( 0, 0, 1 );
		dst_3d = src_3d - Vector3( 0, 0, 100 );
		ray.Init( src_3d, dst_3d );

		g_Interfaces->trace->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );

		if ( tr.allSolid ) // if stuck
			player.sounddata.time = -1;

		*sound.origin = ( ( tr.flFraction < 0.97 ) ? tr.end : *sound.origin );
		player.flags = entity->flags( );
		player.flags |= ( tr.flFraction < 0.50f ? ( 1 << 1 ) /*ducking*/ : 0 ) | ( tr.flFraction != 1 ? ( 1 << 0 ) /*on ground*/ : 0 );
		player.flags &= ( tr.flFraction > 0.50f ? ~( 1 << 1 ) /*ducking*/ : 0 ) | ( tr.flFraction == 1 ? ~( 1 << 0 ) /*on ground*/ : 0 );

		player.oldpos = *sound.origin;
		player.sounddata.time = GetTickCount( );

		if ( !entity->IsDormant( ) )
			continue;

		adjustPlayer( sound.sound_source );
	}

	soundBuffer = soundList;
}

void dormancyManager::adjustPlayer( const int index ) {
	if ( !config->get_bool("espDormant") )
		return;

	auto& player = players[ index ];

	static int duration = 1000;
	bool expired = GetTickCount( ) - player.sounddata.time > duration;

	if ( expired )
		player.set = false;

	if ( !expired )
		player.set = true;

	// dormant check
	player.entity->setFlags( player.flags );
	player.entity->vecOrigin( ) = player.oldpos;
	player.entity->setAbsOrigin( player.oldpos );
}

void dormancyManager::restoreSoundDormancy( ) {
	for ( auto& player : players ) {
		if ( player.set )
			player.set = false;
	}
}

bool dormancyManager::validSound( soundInfo& sound ) {
	if ( !sound.from_server )
		return false;

	for ( int i = 0; i < soundBuffer.Count( ); i++ ) {
		soundInfo& cached_sound = soundBuffer[ i ];
		if ( cached_sound.guid == sound.guid )
			return false;
	}

	return true;
}
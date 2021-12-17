#include "player_hurt.hpp"
#include "../Visuals/Visuals.hpp"

#include "hitsounds.hpp"
#pragma comment(lib, "Winmm.lib") // needed for PlaySound(A)

#include "../../utilities/renderer_d3d.hpp"

void player_hurt::fireGameEvent( IGameEvent *event )
{
	if ( !Globals::localPlayer || !event )
		return;

	if ( !Globals::localPlayer->alive( ) ) {
		return;
	}

	int iUser = event->GetInt( "userid" );
	int iAttacker = event->GetInt( "attacker" );
	auto entity = g_Interfaces->clientEntity->GetClientEntity(g_Interfaces->gameEngine->getPlayerIndex(iUser));

	if ( g_Interfaces->gameEngine->getPlayerIndex( event->GetInt( "attacker" ) ) == g_Interfaces->gameEngine->getLocalPlayer( ) &&
		g_Interfaces->gameEngine->getPlayerIndex( event->GetInt( "userid" ) ) != g_Interfaces->gameEngine->getLocalPlayer( ) )
	{	
		if ( config->get_bool( "espHitmarker" ) || config->get_bool( "espDamageIndicator" ) )
			hitMarkerInfo.push_back( { g_Interfaces->globalVars->curtime + 0.8f, event->GetInt( "dmg_health" ) } );
		
		switch ( config->get_bool( "espHitmarkerSound" ) ) {
		case 0:
			break;
		case 1: {
			g_Interfaces->gameEngine->executeCmd( "play buttons\\arena_switch_press_02.wav" );
		}
				break;
		case 2:
			PlaySoundA( ( LPCSTR )bubbleHitsound, NULL, SND_MEMORY | SND_ASYNC );
			break;
		case 3:
			PlaySoundA( ( LPCSTR )codHitsound, NULL, SND_MEMORY | SND_ASYNC );
			break;
		}

	/*	if (entity && !entity->IsDormant( ) && Globals::localPlayer->alive( ) ) {
			Globals::MissedShots[entity->EntIndex( ) ] = 0;

			if (entity && !Globals::localPlayer->activeWeapon( )->isKnifeorNade( ) )
				Globals::shotsHit[entity->EntIndex( ) ] += 1;

			Globals::MissedShots[entity->EntIndex( ) ] = 0;
		}*/

		_flHurtTime = g_Interfaces->globalVars->curtime;
	}

	if ( config->get_bool( "miscDamageLog" ) )
	{
		auto HitgroupToString = [ ] ( int hitgroup ) -> std::string
		{
			switch ( hitgroup )
			{
			case HITGROUP_GENERIC:
				return "generic";
			case HITGROUP_HEAD:
				return "head";
			case HITGROUP_CHEST:
				return "chest";
			case HITGROUP_STOMACH:
				return "stomach";
			case HITGROUP_LEFTARM:
				return "left arm";
			case HITGROUP_RIGHTARM:
				return "right arm";
			case HITGROUP_LEFTLEG:
				return "left leg";
			case HITGROUP_RIGHTLEG:
				return "right leg";
			case HITGROUP_GEAR:
				return "gear";
			}
			return "generic";
		};

		EventInfo info;
		std::stringstream msg;

		auto enemy = event->GetInt( "userid" );
		auto attacker = event->GetInt( "attacker" );
		auto remaining_health = event->GetString( "health" );
		auto dmg_to_health = event->GetString( "dmg_health" );
		auto hitgroup = event->GetInt( "hitgroup" );

		auto enemyIndex = g_Interfaces->gameEngine->getPlayerIndex( enemy );
		auto attackerIndex = g_Interfaces->gameEngine->getPlayerIndex( attacker );
		auto pEnemy = reinterpret_cast< CBaseEntity* >( g_Interfaces->clientEntity->GetClientEntity( enemyIndex ) );;
		auto pAttacker = reinterpret_cast< CBaseEntity* >( g_Interfaces->clientEntity->GetClientEntity( attackerIndex ) );;

		playerinfo_t attacker_info;
		playerinfo_t enemy_info;

		if ( pEnemy && pAttacker && g_Interfaces->gameEngine->getPlayerInfo2( attackerIndex, &attacker_info ) && g_Interfaces->gameEngine->getPlayerInfo2( enemyIndex, &enemy_info ) )
		{
			if ( attackerIndex != g_Interfaces->gameEngine->getLocalPlayer( ) )
				return;

			info.m_flExpTime = g_Interfaces->globalVars->curtime + 4.f;
			std::string szHitgroup = HitgroupToString( hitgroup );
			msg << "hit " << enemy_info.name << " in the " << szHitgroup << " for " << dmg_to_health << " (" << remaining_health << " remaining)";
			info.m_szMessage = msg.str( );

			Globals::eventInfo.emplace_back( info );

			//Globals::Message( "hit %s in the %s for %s (%s remaining)", enemy_info.name, szHitgroup.c_str( ), dmg_to_health, remaining_health );

			g_Interfaces->cvar->ConsoleColorPrintf(Color::MenuMain, "[dopamine]" );
			g_Interfaces->cvar->ConsoleDPrintf( " ""hit"" " );
			g_Interfaces->cvar->ConsoleColorPrintf( Color( 255, 255, 255 ), "%s", enemy_info.name ); //look into this TODO:
			g_Interfaces->cvar->ConsoleColorPrintf( Color( 255, 255, 255 ), " in the %s", szHitgroup.c_str( ) );
			g_Interfaces->cvar->ConsoleDPrintf( " ""for"" " );
			g_Interfaces->cvar->ConsoleColorPrintf( Color( 255, 255, 255 ), "%s", dmg_to_health );
			g_Interfaces->cvar->ConsoleColorPrintf( Color( 255, 255, 255 ), " (%s remaining)\n", remaining_health );
		}
	}
	else
		if ( Globals::eventInfo.size( ) > 0 )
			Globals::eventInfo.clear( );

	if ( g_Interfaces->gameEngine->getPlayerIndex( iUser ) == g_Interfaces->gameEngine->getLocalPlayer( ) )
		return;

	if ( g_Interfaces->gameEngine->getPlayerIndex( iAttacker ) != g_Interfaces->gameEngine->getLocalPlayer( ) )
		return;

//	if ( Globals::aimbotTarget && !Globals::localPlayer->activeWeapon( )->isKnifeorNade( ) )
//		Globals::bPlayerHurt[entity->EntIndex( ) ] = true;
}

int player_hurt::getEventDebugID( void )
{
	return EVENT_DEBUG_ID_INIT;
}

void player_hurt::registerSelf( )
{
	g_Interfaces->gameEvent->AddListener( this, "player_hurt", false );
}

void player_hurt::unRegisterSelf( )
{
	g_Interfaces->gameEvent->RemoveListener( this );
}

void player_hurt::paint( void )
{
	static int width = 0, height = 0;
	if ( width == 0 || height == 0 )
		g_Interfaces->gameEngine->getScreenSize( width, height );

	if ( Globals::eventInfo.size( ) > 15 )
		Globals::eventInfo.erase( Globals::eventInfo.begin( ) + 1 );

	float alpha = 0.f;

	if ( config->get_bool( "espHitmarker" ) || config->get_bool( "espDamageIndicator" ) )
	{
		for ( size_t i = 0; i < hitMarkerInfo.size( ); i++ )
		{
			float diff = hitMarkerInfo.at( i ).m_flExpTime - g_Interfaces->globalVars->curtime;

			if ( diff < 0.f )
			{
				hitMarkerInfo.erase( hitMarkerInfo.begin( ) + i );
				continue;
			}

			int dist = 24;

			float ratio = 1.f - ( diff / 0.8f );
			alpha = 0.8f - diff / 0.8f;

			if ( config->get_bool( "espDamageIndicator" ) )
				g_renderer_d3d->draw_text( width / 2 + 6 + ratio * dist / 2, height / 2 + 6 + ratio * dist, g_renderer_d3d->menu_bold, std::to_string( hitMarkerInfo.at( i ).m_iDmg ).c_str( ), Color::MenuMain, 4 );

		}

		if ( hitMarkerInfo.size( ) > 0 && config->get_bool( "espHitmarker" ) )
		{
			int screenCenterX, screenCenterY;
			g_Interfaces->gameEngine->getScreenSize( screenCenterX, screenCenterY );
			screenCenterX /= 2;
			screenCenterY /= 2;

			if ( _flHurtTime + 0.30f >= g_Interfaces->globalVars->curtime ) {
				int hitmarkerAlpha = static_cast< int >( static_cast< float >( ( ( _flHurtTime + 0.30f ) - g_Interfaces->globalVars->curtime )
					/ 1.2 ) * 1000.f );
				
				if ( config->get_bool( "espType" ) == 0 ) {
					int lineSize = 8;
					g_renderer_d3d->line( screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - ( lineSize / 4 ), screenCenterY - ( lineSize / 4 ), Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
					g_renderer_d3d->line( screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - ( lineSize / 4 ), screenCenterY + ( lineSize / 4 ), Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
					g_renderer_d3d->line( screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + ( lineSize / 4 ), screenCenterY + ( lineSize / 4 ), Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
					g_renderer_d3d->line( screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + ( lineSize / 4 ), screenCenterY - ( lineSize / 4 ), Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
				}
				else {
					int lineSize = 12;
					g_renderer_d3d->line( width / 2 - lineSize / 2, height / 2 - lineSize / 2, width / 2 + lineSize / 2 + 1, height / 2 + lineSize / 2 + 1, Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
					g_renderer_d3d->line( width / 2 + lineSize / 2, height / 2 - lineSize / 2, width / 2 - lineSize / 2, height / 2 + lineSize / 2, Color( 255, 255, 255, ( int )hitmarkerAlpha ) );
				}
			}
		}
	}

	if ( config->get_bool( "miscDamageLog" ) || config->get_bool( "miscPurchasesLog" ) )
	{
		for ( size_t i = 0; i < Globals::eventInfo.size( ); i++ )
		{
			float diff = Globals::eventInfo[ i ].m_flExpTime - g_Interfaces->globalVars->curtime;
			if ( Globals::eventInfo[ i ].m_flExpTime < g_Interfaces->globalVars->curtime )
			{
				Globals::eventInfo.erase( Globals::eventInfo.begin( ) + i );
				continue;
			}

			alpha = diff / 4;

			std::transform( Globals::eventInfo[ i ].m_szMessage.begin( ), Globals::eventInfo[ i ].m_szMessage.end( ), Globals::eventInfo[ i ].m_szMessage.begin( ), tolower );


			g_renderer_d3d->draw_text( 5, 28 + ( 15 * i ), g_renderer_d3d->menu, Globals::eventInfo[ i ].m_szMessage.c_str( ), Color( 255, 255, 255, ( int )( alpha * 255.f ) ), 4 );
		}
	}
}
#include "gameevents.h"
#include <sstream>

CEvents::CEvents()
{
	iDebug = 42;
	bHitted = false;
	vImpact = Vector( 0.f, 0.f, 0.f );
}

std::string GetHitgroup( int group )
{
	switch( group )
	{
	case 1:
		return "Head";
		break;
	case 2:
	case 3:
		return "Body";
		break;
	case 4:
	case 5:
		return "Arm";
		break;
	case 6:
	case 7:
		return "Leg";
		break;
	}
	return "Body";
}

void ResolveEvents( IGameEvent* pEvent, CBaseEntity* pLocal )
{
	if( !pLocal )
		return;

	if( pLocal->GetHealth() <= 0 )
		return;
	
	if( g_Resolver.shot_snapshots.size() <= 0 )
		return;

	auto& snapshot = g_Resolver.shot_snapshots.front();

	if( !pEvent->GetName().compare( "player_hurt" ) )
	{
		if( g_pEngine->GetPlayerForUserID( pEvent->GetInt( "attacker" ) ) != g_pEngine->GetLocalPlayer() )
			return;

		if( !snapshot.was_shot_processed )
		{
			snapshot.was_shot_processed = true;
			snapshot.first_processed_time = g_pGlobalVars->curtime;
		}

		snapshot.hitgroup_hit = pEvent->GetInt( "hitgroup" );
	}

	if( !pEvent->GetName().compare( "bullet_impact" ) )
	{
		if( g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) != g_pEngine->GetLocalPlayer() )
			return;

		if( !snapshot.was_shot_processed )
		{
			snapshot.was_shot_processed = true;
			snapshot.first_processed_time = g_pGlobalVars->curtime;
		}

		snapshot.impact = Vector( pEvent->GetFloat( "x" ), pEvent->GetFloat( "y" ), pEvent->GetFloat( "z" ) );
		snapshot.position = pLocal->GetEyePosition();
	}

	if( !pEvent->GetName().compare( "weapon_fire" ) )
	{
		if( g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) != g_pEngine->GetLocalPlayer() )
			return;

		if( !snapshot.was_shot_processed )
		{
			snapshot.was_shot_processed = true;
			snapshot.first_processed_time = g_pGlobalVars->curtime;
		}
	}
}

void CEvents::FireGameEvent( IGameEvent* pEvent )
{
	int r = g_Var.Color.Menu[ 0 ], g = g_Var.Color.Menu[ 1 ], b = g_Var.Color.Menu[ 2 ];

	if( !pEvent )
		return;

	if( pEvent->GetName().empty() )
		return;

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );
	CBaseEntity* pHurt = ( CBaseEntity* ) g_pEntList->GetClientEntity( g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) );
	CBaseEntity* pAttacker = ( CBaseEntity* ) g_pEntList->GetClientEntity( g_pEngine->GetPlayerForUserID( pEvent->GetInt( "attacker" ) ) );
	bool bHasKit = pEvent->GetBool( "haskit" );
	bool bHeadshot = pEvent->GetBool( "headshot" );

	if( g_Var.AntiAim.Resolve > 0 )
		ResolveEvents( pEvent, pLocal );

	if( !pEvent->GetName().compare( "player_hurt" ) )
	{
		if( pHurt != pLocal && pAttacker == pLocal && pLocal )
		{
			g_ResolveInfo[ pHurt->GetIndex() ].iShotsMissed--;

			bHitted = true;
			flHitTime = g_pGlobalVars->curtime + 1.f;
			flHurtTime = g_pGlobalVars->curtime;

			if( g_Var.Visuals.World.BulletTracer[ 2 ] )
			{
				sBulletImpact impact;

				impact.vEnemy = pHurt->GetOrigin();

				g_BulletHit.push_back( impact );
			}

			if( g_Var.Visuals.World.BulletTracer[ 3 ] )
				g_Visuals.DrawHitbox( pHurt->GetIndex() );

			if( g_Var.Misc.EventLog[ 0 ] )
			{
				VEngineClient::player_info_t pInfo;

				if( !g_pEngine->GetPlayerInfo( pHurt->GetIndex(), &pInfo ) )
					return;

				std::stringstream szText;

				szText << "Hit " << pInfo.name << " for " << pEvent->GetInt( "dmg_health" ) << " in the " << GetHitgroup( pEvent->GetInt( "hitgroup" ) ) << " ( " << pEvent->GetInt( "health" ) << " remaining ) \n";

				g_pICvar->ConsoleColorPrintf( Color( r, g, b ), "[ KIDUAHOOK ] " );
				g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), szText.str().c_str() );

				CEventLogs::sLog EventLog;

				EventLog.EventText.PushBack( "Hit ", Color( 255, 255, 255 ) );
				EventLog.EventText.PushBack( pInfo.name, Color( r, g, b ) );
				EventLog.EventText.PushBack( " for ", Color( 255, 255, 255 ) );
				EventLog.EventText.PushBack( std::to_string( pEvent->GetInt( "dmg_health" ) ), Color( r, g, b ) );
				EventLog.EventText.PushBack( " in the ", Color( 255, 255, 255 ) );
				EventLog.EventText.PushBack( GetHitgroup( pEvent->GetInt( "hitgroup" ) ), Color( r, g, b ) );

				g_EventLogs.AddLog( EventLog );
			}
		}
	}

	if( !pEvent->GetName().compare( "item_purchase" ) )
	{
		if( pHurt && pAttacker && pAttacker != pLocal && pLocal && pHurt->GetTeamNumber() != pLocal->GetTeamNumber() )
		{
			if( g_Var.Misc.EventLog[ 1 ] )
			{
				VEngineClient::player_info_t pInfo;

				if( !g_pEngine->GetPlayerInfo( pHurt->GetIndex(), &pInfo ) )
					return;

				std::stringstream szText;

				szText << pInfo.name << " bought a " << pEvent->GetString( "weapon" ) << "\n";

				g_pICvar->ConsoleColorPrintf( Color( r, g, b ), "[ KIDUAHOOK ] " );
				g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), szText.str().c_str() );

				CEventLogs::sLog EventLog;

				EventLog.EventText.PushBack( pInfo.name, Color( r, g, b ) );
				EventLog.EventText.PushBack( " bought a ", Color( 255, 255, 255 ) );
				EventLog.EventText.PushBack( pEvent->GetString( "weapon" ), Color( r, g, b ) );

				g_EventLogs.AddLog( EventLog );
			}
		}
	}

	if( !pEvent->GetName().compare( "bullet_impact" ) )
	{
		float x = pEvent->GetFloat( "x" ), y = pEvent->GetFloat( "y" ), z = pEvent->GetFloat( "z" );

		if( pHurt == pLocal && pLocal )
		{
			flImpactTime = g_pGlobalVars->curtime;
			vImpact = Vector( x, y, z );

			if( g_Var.Visuals.World.BulletTracer[ 0 ] )
			{
				BulletEvent_t BulletImpact( pHurt, Vector( x, y, z ), g_pGlobalVars->curtime, g_Var.Color.BulletTracer[ 0 ], g_Var.Color.BulletTracer[ 1 ], g_Var.Color.BulletTracer[ 2 ] );

				g_BulletImpact.push_back( BulletImpact );
			}

			if( g_Var.Visuals.World.BulletTracer[ 1 ] || g_Var.Visuals.World.BulletTracer[ 2 ] )
			{
				sBulletImpact impact;

				impact.vPos = Vector( x, y, z );
				impact.vEye = pLocal->GetEyePosition();
				impact.flTime = g_pGlobalVars->curtime + 1.f;

				g_BulletHit.push_back( impact );
			}
		}

	/*	if( pLocal && pHurt == pLocal )
		{
			CEngineTraceClient::CTraceFilterEntities traceFilter;
			traceFilter.pSkip = pLocal;

			CEngineTraceClient::Ray_t ray;
			CEngineTraceClient::trace_t trace;

			ray.Init( pLocal->GetEyePosition(), Vector( x, y, z ) );
			g_pEngineTrace->TraceRay( ray, 0x4600400B, &traceFilter, &trace );
		//	g_pEngineTrace->ClipRayToEntity( ray, 0x4600400B, pHurt, &trace );

			if( trace.m_pEnt && trace.m_pEnt != pHurt )
			{
				g_ResolveInfo[ trace.m_pEnt->GetIndex() ].iShotsMissed--;

				CEventLogs::sLog EventLog;

				EventLog.EventText.PushBack( "KIDUAHOOK ", Color( r, g, b ) );
				EventLog.EventText.PushBack( "Spread miss.", Color( 255, 255, 255 ) );

				g_EventLogs.AddLog( EventLog );
			}
		}*/
	}

	if( !pEvent->GetName().compare( "player_death" ) )
	{
		if( pHurt != nullptr )
		{
			if( g_Var.Misc.Insults )
				g_Misc.FlameEnemy( pAttacker->GetIndex(), pLocal->GetIndex(), pHurt->GetIndex(), bHeadshot );

		//	PlayerResolveRecord& resolve_record = g_Resolver.player_resolve_records[ pHurt->GetIndex() ];
		//	resolve_record.m_iMissedShots = 0;
		/*	resolve_record.m_iMissedAir = 0;
		    resolve_record.m_iMissedMoving = 0;
			resolve_record.m_iMissedStand = 0;*/
			g_ResolveInfo[ pHurt->GetIndex() ].iShotsMissed = 0;
			g_Rage.iShotsFired[ pHurt->GetIndex() ] = 0;
		}
	}

	if( !pEvent->GetName().compare( "weapon_fire" ) )
	{
		if( pLocal && pHurt == pLocal )
		{
			g_ResolveInfo[ g_Rage.iTarget ].iShotsMissed++;
		//	g_Rage.iShotsFired[ g_Rage.iTarget ]++;
		}
	}

	if( !pEvent->GetName().compare( "game_newmap" ) )
	{
		for (int i = 1; i <= 32; i++)
		{
			PlayerResolveRecord& resolve_record = g_Resolver.player_resolve_records[i];
			resolve_record.m_iMissedShots = 0;
			resolve_record.m_iMissedAir = 0;
			resolve_record.m_iMissedMoving = 0;
			resolve_record.m_iMissedStand = 0;
		}

		g_BulletHit.clear();
		g_Resolver.CleanShotSnapshot();
		g_Visuals.bNewMap = true;
		bNewMap = true;
	}

	if( !pEvent->GetName().compare( "bomb_defused" ) )
		bDefused = true;

	if( !pEvent->GetName().compare( "bomb_exploded" ) )
		bExploded = true;

	if( !pEvent->GetName().compare( "round_freeze_end" ) )
		bResetBomb = false;

	if( !pEvent->GetName().compare( "round_prestart" ) )
		bResetBomb = true;

	if( !pEvent->GetName().compare( "bomb_beginplant" ) )
	{
		//Bomb is starting to get planted.
		bGetsPlanted = true;

		if( g_Var.Misc.EventLog[ 2 ] )
		{
			std::stringstream szText;

			szText << "Bomb is starting to get planted \n";

			g_pICvar->ConsoleColorPrintf( Color( r, g, b ), "[ KIDUAHOOK ] " );
			g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), szText.str().c_str() );

			CEventLogs::sLog EventLog;

			EventLog.EventText.PushBack( "Bomb", Color( r, g, b ) );
			EventLog.EventText.PushBack( " is starting to get planted", Color( 255, 255, 255 ) );

			g_EventLogs.AddLog( EventLog );
		}
	}

	if( !pEvent->GetName().compare( "bomb_begindefuse" ) )
	{
		//pEvent->GetBool( "haskit" )
		//"Bomb is started to be defused ( With Kit ).\n" : "Bomb is started to be defused ( Without Kit ).\n"
		bDefusing = true;
		bUsingKit = bHasKit;

		if( g_Var.Misc.EventLog[ 2 ] )
		{
			std::stringstream szText;

			if( pEvent->GetBool( "haskit" ) )
				szText << "Bomb is started to be defused  ( With Kit ) \n";
			else
				szText << "Bomb is started to be defused  ( Without Kit ) \n";

			g_pICvar->ConsoleColorPrintf( Color( r, g, b ), "[ KIDUAHOOK ] " );
			g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), szText.str().c_str() );

			CEventLogs::sLog EventLog;

			EventLog.EventText.PushBack( "Bomb", Color( r, g, b ) );
			EventLog.EventText.PushBack( " is started to be defused  ", Color( 255, 255, 255 ) );

			if( pEvent->GetBool( "haskit" ) )
				EventLog.EventText.PushBack( "With Kit", Color( r, g, b ) );
			else
				EventLog.EventText.PushBack( "Without Kit", Color( r, g, b ) );

			g_EventLogs.AddLog( EventLog );
		}
	}

	if( !pEvent->GetName().compare( "bomb_abortdefuse" ) )
	{
		//Bomb defusal has been stopped
		bDefusing = false;

		if( g_Var.Misc.EventLog[ 2 ] )
		{
			std::stringstream szText;

			szText << "Bomb defusal has been stopped \n";

			g_pICvar->ConsoleColorPrintf( Color( r, g, b ), "[ KIDUAHOOK ] " );
			g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), szText.str().c_str() );

			CEventLogs::sLog EventLog;

			EventLog.EventText.PushBack( "Bomb", Color( r, g, b ) );
			EventLog.EventText.PushBack( " defusal has been stopped", Color( 255, 255, 255 ) );

			g_EventLogs.AddLog( EventLog );
		}
	}

	if( !pEvent->GetName().compare( "round_start" ) )
	{
		bNewRound = true;
		bRoundStart = true;
	}
}

void CEvents::Registerself()
{
	g_pGameEventManager2->AddListener( this, "player_hurt", false );
	g_pGameEventManager2->AddListener( this, "item_purchase", false );
	g_pGameEventManager2->AddListener( this, "bullet_impact", false );
	g_pGameEventManager2->AddListener( this, "player_death", false );
	g_pGameEventManager2->AddListener( this, "weapon_fire", false );
	g_pGameEventManager2->AddListener( this, "game_newmap", false );
	g_pGameEventManager2->AddListener( this, "bomb_beginplant", false );
	g_pGameEventManager2->AddListener( this, "bomb_begindefuse", false );
	g_pGameEventManager2->AddListener( this, "bomb_abortdefuse", false );
	g_pGameEventManager2->AddListener( this, "round_start", false );
	g_pGameEventManager2->AddListener( this, "bomb_defused", false );
	g_pGameEventManager2->AddListener( this, "bomb_exploded", false );
	g_pGameEventManager2->AddListener( this, "round_freeze_end", false );
	g_pGameEventManager2->AddListener( this, "round_prestart", false );
}

CEvents* g_pEvents = nullptr;
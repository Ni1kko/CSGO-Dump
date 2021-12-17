#include "Misc.hpp"
#include "../../utilities/renderer_d3d.hpp"
#include "grenadePrediction.hpp"
#include "../../menu/menu.hpp"
#include "../Anti-aim/Fakelag.hpp"
#include "Animations.hpp"

#include <algorithm>

CMisc g_Misc;

void CMisc::onMove( float sampleTime, CUserCmd* userCmd )
{
	if ( !g_Interfaces->gameEngine->inGame( ) )
		return;

	if ( !g_Interfaces->gameEngine->connected( ) )
		return;

	if ( !Globals::localPlayer )
		return;
	
	if ( config->get_bool( "miscBunnyhop" ) )
		Bunnyhop( userCmd );

	if ( config->get_bool( "miscStrafe" ) )
		autoStrafe( userCmd );

	//if ( config->get_bool( "miscDuckInAir" ) )
		//duckInAir( userCmd );

	if ( config->get_bool( "aaSlowMotion" ) )
		slowWalk( userCmd );

	if ( config->get_bool( "miscUnlimitedDuck" ) )
		removeDuckDelay( userCmd );
}

void CMisc::onFrameStage( clientFrameStage_t frameStage )
{
	if ( !g_Interfaces->gameEngine->inGame( ) )
		return;

	if ( !Globals::localPlayer )
		return;

	thirdPerson( frameStage );
}

void CMisc::onPaint( unsigned int panel )
{
	if ( !g_Interfaces->gameEngine->inGame( ) && !g_Interfaces->gameEngine->connected( ) )
		return;

	if ( Globals::localPlayer )
		crosshair( );

	removeFlash( );
	removeSmoke( );

	spectatorList( );
	g_GrenadePred.draw( );
}

void CMisc::onEvent( IGameEvent* pEvent )
{

}

void CMisc::movementFix( Vector3& oldang )
{
	Vector3 vMovements( Globals::oCmd->forwardmove, Globals::oCmd->sidemove, 0.f );

	if ( vMovements.Length2D( ) == 0 )
		return;

	Vector3 vRealF, vRealR;
	Vector3 aRealDir = Globals::oCmd->viewAngles;
	aRealDir.Clamp( );

	g_Math.angleVectors( aRealDir, &vRealF, &vRealR, nullptr );
	vRealF[ 2 ] = 0;
	vRealR[ 2 ] = 0;

	Vector3Normalize( vRealF );
	Vector3Normalize( vRealR );

	Vector3 aWishDir = oldang;
	aWishDir.Clamp( );

	Vector3 vWishF, vWishR;
	g_Math.angleVectors( aWishDir, &vWishF, &vWishR, nullptr );

	vWishF[ 2 ] = 0;
	vWishR[ 2 ] = 0;

	Vector3Normalize( vWishF );
	Vector3Normalize( vWishR );

	Vector3 vWishVel;
	vWishVel[ 0 ] = vWishF[ 0 ] * Globals::oCmd->forwardmove + vWishR[ 0 ] * Globals::oCmd->sidemove;
	vWishVel[ 1 ] = vWishF[ 1 ] * Globals::oCmd->forwardmove + vWishR[ 1 ] * Globals::oCmd->sidemove;
	vWishVel[ 2 ] = 0;

	float a = vRealF[ 0 ], b = vRealR[ 0 ], c = vRealF[ 1 ], d = vRealR[ 1 ];
	float v = vWishVel[ 0 ], w = vWishVel[ 1 ];

	float flDivide = ( a * d - b * c );
	float x = ( d * v - b * w ) / flDivide;
	float y = ( a * w - c * v ) / flDivide;

	Globals::oCmd->forwardmove = x;
	Globals::oCmd->sidemove = y;
}

void CMisc::Bunnyhop(CUserCmd* pCmd)
{

	if (!config->get_bool("miscBunnyhop"))
		return;

	if (!Globals::localPlayer->alive())
		return;

	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		pCmd->buttons |= IN_JUMP;
	}
	else if (pCmd->buttons & IN_JUMP)
	{
		if (Globals::localPlayer->flags() & FL_ONGROUND)
			bShouldFake = bLastJumped = true;
		else
		{
			pCmd->buttons &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
}

void CMisc::autoStrafe( CUserCmd* pCmd )
{
	if ( !Globals::localPlayer->alive( ) )
		return;

	if ( Globals::localPlayer->moveType( ) == movetype_fly )
		return;

	/* polak code */

	auto get_move_angle = [ ] ( float speed ) -> const float
	{
		// radian to degree
		auto move_angle = ( asin( 15.f / speed ) ) * ( 180.f / PI );


		if ( !isfinite( move_angle ) || move_angle > 90.f )
			move_angle = 90.f;
		else if ( move_angle < 0.f )
			move_angle = 0.f;

		return move_angle;
	};

	static auto switchKey = 1.f;
	static auto circleYaw = 0.f;
	static auto oldYaw = 0.f;

	auto velocity = Globals::localPlayer->velocity( );
	velocity.z = 0.f;

	auto speed = velocity.Length2D( );
	auto idealStrafe = get_move_angle( speed );

	switchKey *= -1.f;

	if ( Globals::localPlayer->moveType( ) == 2 && !( Globals::localPlayer->flags( ) & FL_ONGROUND && !( pCmd->buttons & IN_JUMP ) ) )
	{

		if ( config->get_bool("miscKeyStrafe") == 1 )
		{
			if ( pCmd->sidemove < 0 )
				Globals::originalViewAngle.y += 90;

			if ( pCmd->sidemove > 0 )
				Globals::originalViewAngle.y -= 90;

			if ( pCmd->forwardmove < 0 )
				Globals::originalViewAngle.x += 90;

			if ( pCmd->forwardmove > 0 )
				Globals::originalViewAngle.x -= 90;
		}
		else if ( pCmd->forwardmove > 0.f )
			pCmd->forwardmove = 0.f;

		auto yawDelta = g_Math.NormalizeFloat( Globals::originalViewAngle.y - oldYaw );
		auto absYawDelta = abs( yawDelta );

		circleYaw = oldYaw = Globals::originalViewAngle.y;

		if ( yawDelta > 0.f )
			pCmd->sidemove = -450.f;
		else if ( yawDelta < 0.f )
			pCmd->sidemove = 450.f;

		if ( absYawDelta <= idealStrafe || absYawDelta >= 30.f )
		{
			Vector3 velocity_angles;
			g_Math.vectorAngles( velocity, velocity_angles );


			auto velocityDelta = g_Math.NormalizeFloat( Globals::originalViewAngle.y - velocity_angles.y );
			auto retrack = idealStrafe * ( 15.f - config->get_int("miscStrafeSpeed") );

			if ( velocityDelta <= retrack || speed <= 15.f )
			{
				if ( -retrack <= velocityDelta || speed <= 15.f )
				{
					Globals::originalViewAngle.y += idealStrafe * switchKey;
					pCmd->sidemove = switchKey * 450.f;
				}
				else
				{
					Globals::originalViewAngle.y = velocity_angles.y - retrack;
					pCmd->sidemove = 450.f;
				}
			}
			else
			{
				Globals::originalViewAngle.y = velocity_angles.y + retrack;
				pCmd->sidemove = -450.f;
			}
		}
	}
}

void CMisc::duckInAir(CUserCmd* pCmd)
{
	//if (!config->get_bool("miscDuckInAir"))
		return;

	int local_flags = Globals::localPlayer->flags();
	if (!(local_flags & FL_ONGROUND))
		pCmd->buttons |= IN_DUCK;
}

void CMisc::slowWalk( CUserCmd* pCmd )
{
	if ( !config->get_bool("aaSlowMotion") )
		return;

	auto weapon = Globals::localPlayer->activeWeapon( );

	if ( !weapon )
		return;

	if ( Globals::localPlayer->moveType( ) == movetype_walk )
	{

		const CBasePlayerAnimState *state = Globals::localPlayer->animState( );
		auto *wpn_data = weapon->weaponData( );

		float desired_speed = ( wpn_data->maxSpeed * .0034f ) * ( 70.f / 1.4f );

		// todo : write handler func for keypress
		/* note:
		* we don't actually care about accuracy until we're actually ready to fire
		* instead what we're looking to do here is "desync" the farthest we are able to.
		* see: CCSGOPlayerAnimState::SetUpVelocity for more information on why we don't want to be at a high speed.
		*/
		Globals::localPlayer->setVelocity( { 0,0,0 } );

		if ( state->speed_2d >= desired_speed && menu->get_hotkey("aaSlowMotionKey") )
		{
			pCmd->sidemove = 0.f;
			pCmd->forwardmove = 0.f;
			pCmd->upmove = 0.f;
		}
	}
}

void CMisc::removeDuckDelay( CUserCmd* pCmd )
{

	if ( !config->get_bool( "miscUnlimitedDuck" ) )
		return;

	if ( !Globals::localPlayer->alive( ) )
		return;

	pCmd->buttons |= IN_BULLRUSH;
}

void CMisc::thirdPerson( clientFrameStage_t frameStage )
{
	if ( !g_Interfaces->gameEngine->inGame( ) && !g_Interfaces->gameEngine->connected( ) )
		return;

	if ( !Globals::localPlayer )
		return;

	static bool init = false;

	if ( config->get_bool("espThirdperson") && menu->get_hotkey("espThirdpersonKey") && Globals::localPlayer->alive( ) ) //reeeeeeeeeeeeeeeeeeee, pasted. have to think about another way since this is shit
	{
		if ( init )
		{
			ConVar* sv_cheats = g_Interfaces->cvar->FindVar( "sv_cheats" );
			*( int* )( ( DWORD )&sv_cheats->m_fnChangeCallbacks + 0xC ) = 0;
			sv_cheats->SetValue( 1 );
			g_Interfaces->gameEngine->executeCmd( "thirdperson" );
			Globals::inThirdperson = true;
		}
		init = false;
	}
	else
	{
		if ( !init )
		{
			ConVar* sv_cheats = g_Interfaces->cvar->FindVar( "sv_cheats" );
			*( int* )( ( DWORD )&sv_cheats->m_fnChangeCallbacks + 0xC ) = 0;
			sv_cheats->SetValue( 1 );
			g_Interfaces->gameEngine->executeCmd( "firstperson" );
			Globals::inThirdperson = false;
		}
		init = true;
	}

	if ( frameStage == FRAME_RENDER_START )
	{
		if ( Globals::inThirdperson && Globals::localPlayer->alive( ) )
		{
			g_Interfaces->prediction->SetLocalViewAngles( Vector3( Globals::realAngle.x, Globals::realAngle.y, 0 ) );
		}
	}
}

void CMisc::crosshair( )
{
	int value = Globals::localPlayer->scoped( ) ? 0 : ( config->get_bool("miscCrosshair") ? 3 : 0 );
	g_Interfaces->cvar->FindVar( "weapon_debug_spread_show" )->SetValue( value );
}

void CMisc::spectatorList( )
{
	if ( !config->get_bool("miscSpectators") )
		return;

	if ( !g_Interfaces->gameEngine->connected( ) && !g_Interfaces->gameEngine->inGame( ) )
		return;

	int index = 0;
	int w, h;
	g_Interfaces->gameEngine->getScreenSize( w, h );

	g_Renderer->text( w - 80, h / 2 - 10, g_Fonts->spectators, "spectators", Color::White );
	for ( int i = 1; i <= g_Interfaces->globalVars->maxClients; ++i )
	{
		auto pEntity = g_Interfaces->clientEntity->GetClientEntity( i );
		playerinfo_t e_info;

		if ( !pEntity || pEntity == Globals::localPlayer )
			continue;

		g_Interfaces->gameEngine->getPlayerInfo( i, &e_info );

		if ( !pEntity->alive( ) && !pEntity->IsDormant( ) )
		{
			auto target = pEntity->observerTarget( );
			if ( target )
			{
				auto pTarget = g_Interfaces->clientEntity->GetClientEntityFromHandle( target );
				if ( pTarget == Globals::localPlayer )
				{
					int x, y;
					g_Interfaces->surface->calculateTextSize( g_Fonts->spectators, g_Utils->toWchar( e_info.name ), x, y );

					std::string player_name = e_info.name;

					g_Renderer->text( w - x - 35, h / 2 + ( 10 * index ), g_Fonts->spectators, player_name.c_str( ), Color::White );
					index++;
				}
			}
		}
	}
}

typedef void( __fastcall* ClanTagFn )( const char*, const char* );
ClanTagFn ClanTag;

void setClanTag( const char* tag, const char* name )
{
	if ( !ClanTag )
		ClanTag = reinterpret_cast< ClanTagFn >( g_Memory->findPattern( "engine.dll", "53 56 57 8B DA 8B F9 FF 15" ) );

	if ( ClanTag )
		ClanTag( tag, name );
}

void CMisc::clanTag( )
{
	static int inc = 0;
	static std::string clantag = " $ dopamine";
	if (++inc > 30) {
		rotate(clantag.begin(), clantag.begin() + 1, clantag.end());
		setClanTag(clantag.c_str(), clantag.c_str());
		inc = 0;
	}
}

void CMisc::drawIndicators( )
{
	if ( !g_Interfaces->gameEngine->inGame( ) || !g_Interfaces->gameEngine->connected( ) )
		return;

	if ( !Globals::localPlayer )
		return;

	if ( !Globals::localPlayer->alive( ) )
		return;

	int x, y;
	g_Interfaces->gameEngine->getScreenSize( x, y );
	y -= 41;

	auto Indicator = [ & ] ( Color color, const char* text, ... ) -> void //from bottom left to top left
	{
		g_renderer_d3d->indicator->draw_text( text, { 20, y }, color, 4 );

		y -= 30;
	};

	if ( config->get_bool("rageEnable") )
	{
		bool baiming = ( config->get_bool("rageForceBodyAim") && menu->get_hotkey("rageBodyAimKey") );
		Color baim = baiming ? Color( 132, 195, 16 ) : Color::Red;

		float desyncAmt = Globals::localPlayer->calculateDesyncDelta( );
		Color fake = desyncAmt <= 29 ? Color::Red : ( desyncAmt >= 55 ? Color( 132, 195, 16 ) : Color( 255 - ( desyncAmt * 2.55 ), desyncAmt * 2.55, 0 ) );

		//if ( !( desyncAmt < 29 ) )
			//Indicator( fake, "desync" );

		if (baiming)
			Indicator( baim, "baim" );
	}
}

void CMisc::removeFlash( )
{
	if ( !config->get_bool("espRemovalsFlash") )
		return;

	if ( !Globals::localPlayer )
		return;

	if ( Globals::localPlayer->flashDuration( ) > 0 )
		*Globals::localPlayer->flashDuration( ) = 0;
}

void CMisc::removeSmoke( )
{
	if ( !Globals::localPlayer )
		return;

	static auto smokeCount = *reinterpret_cast< uint32_t** >( g_Memory->findPattern( "client.dll", "A3 ? ? ? ? 57 8B CB" ) + 1 );

	static std::vector<const char*> smokeMaterials =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
	};

	if ( config->get_bool("espRemovalsSmoke") )
	{
		for ( auto materialName : smokeMaterials )
		{
			IMaterial* material = g_Interfaces->materialSystem->FindMaterial( materialName, TEXTURE_GROUP_OTHER );

			material->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
		}

		*( int* )smokeCount = 0;
	}
}

auto edge_flags_backup = 0;
void CMisc::edge_jump_preprediction(CUserCmd* pCmd)
{
	if (!config->get_bool("miscEdgeJump"))
		return;

	if (!menu->get_hotkey("miscEdgeJumpKey"))
		return;

	if (!Globals::localPlayer)
		return;

	if (Globals::localPlayer->moveType() == movetype_ladder || Globals::localPlayer->moveType() == movetype_noclip)
		return;

	edge_flags_backup = Globals::localPlayer->flags();
}

void CMisc::edge_jump_postprediction(CUserCmd* pCmd)
{
	if (!config->get_bool("miscEdgeJump"))
		return;

	if (!menu->get_hotkey("miscEdgeJumpKey"))
		return;

	if (!Globals::localPlayer)
		return;

	if (Globals::localPlayer->moveType() == movetype_ladder || Globals::localPlayer->moveType() == movetype_noclip)
		return;

	if (edge_flags_backup & FL_ONGROUND && !(Globals::localPlayer->flags() & FL_ONGROUND))
		pCmd->buttons |= IN_JUMP;

	if (!(Globals::localPlayer->flags() & FL_ONGROUND) && config->get_bool("miscDuckInAir"))
		pCmd->buttons |= IN_DUCK;
}
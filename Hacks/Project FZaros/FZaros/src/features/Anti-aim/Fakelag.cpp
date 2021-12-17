#include "Fakelag.hpp"
#include <random>
#include "../../menu/menu.hpp"

CFakeLag* g_FakeLag;

void CFakeLag::createMove( ) // lotta shit is wrong here
{

	if ( !config->get_bool("rageChokeEnable"))
		return;

	if ( !g_Interfaces->gameEngine->inGame( ) && !g_Interfaces->gameEngine->connected( ) )
		return;

	if ( !Globals::localPlayer->alive( ) )
		return;

	if ( Globals::oCmd->buttons & IN_USE )
		return;

	if (config->get_bool("rageFakeDuck") && menu->get_hotkey("rageFakeDuckKey"))
		fakeDuck( );

	if ( !handleActivation( ) )
		return;

	int wishTicks = 0,
		adaptiveTicks = 2,
		wishTicks_1;

	if (config->get_bool("rageFakeDuck") && !menu->get_hotkey("rageFakeDuckKey"))
		wishTicks_1 = config->get_int("rageChokeAmount");
	else
		wishTicks_1 = 13;

	float extrapolatedSpeed = Globals::localPlayer->velocity( ).Length2D( ) * g_Interfaces->globalVars->intervalPerTick;

	std::random_device random;
	std::mt19937 generator( random( ) );
	std::uniform_int_distribution randomLag( wishTicks, wishTicks_1 );

	switch ( config->get_int("rageChokeType") )
	{
	case 0:
		wishTicks = wishTicks_1;
		break;
	case 1:
		if ( Globals::oCmd->tickCount % 8 < 4 )
			wishTicks = wishTicks_1;
		break;
	case 2:
		while ( ( wishTicks * extrapolatedSpeed ) <= 68.f )
		{
			if ( ( ( adaptiveTicks - 1 ) * extrapolatedSpeed ) > 68.f )
			{
				++wishTicks;
				break;
			}
			if ( ( adaptiveTicks * extrapolatedSpeed ) > 68.f )
			{
				wishTicks += 2;
				break;
			}
			if ( ( ( adaptiveTicks + 1 ) * extrapolatedSpeed ) > 68.f )
			{
				wishTicks += 3;
				break;
			}
			if ( ( ( adaptiveTicks + 2 ) * extrapolatedSpeed ) > 68.f )
			{
				wishTicks += 4;
				break;
			}

			adaptiveTicks += 5;
			wishTicks += 5;

			if ( adaptiveTicks > 15 )
				break;

			wishTicks_1 = wishTicks;
		}
		break;
	case 3:
		wishTicks = randomLag( generator );
		break;
	}

	wishTicks = std::clamp<int>( wishTicks, 1, 15 );

	if ( Globals::chokedPackets > min( 15, wishTicks ) )
	{
		Globals::bSendPacket = true;
		Globals::fakeLagging = true;
	}
	else
	{
		Globals::bSendPacket = false;
		Globals::fakeLagging = false;
	}

	Globals::breakingLC = ( Globals::fakeLagOrigin - Globals::localPlayer->vecOrigin( ) ).Length( ) > 64.0f;
}

bool CFakeLag::handleActivation( )
{
	if ( Globals::localPlayer->flags( ) & FL_ONGROUND )
	{
		if ( config->get_int( "rageChokeActivation" ) & ( 1 << 1 ) && Globals::localPlayer->velocity( ).Length2D( ) < 10 )
			return true;

		if ( ( config->get_int( "rageChokeActivation" ) & ( 1 << 2 ) ) && Globals::localPlayer->velocity( ).Length2D( ) > 10 )
			return true;

		if ( menu->get_hotkey( "rageFakeDuckKey" ) )
			return true;
	}
	else
	{
		if ( config->get_int( "rageChokeActivation" ) & ( 1 << 3 ) )
			return true;
	}

	if ( ( config->get_int( "rageChokeActivation" )  & ( 1 << 4 ) ) && Globals::oCmd->buttons & IN_ATTACK )
		return true;

	return false;
}

void CFakeLag::fakeDuck( )
{
	// anti paste
}
#include "round_start.hpp"

void round_start::fireGameEvent( IGameEvent *event )
{
	if ( !Globals::localPlayer || !event )
		return;

	Globals::shouldDormant = false;

	if (g_Interfaces->gameEngine->connected() && g_Interfaces->gameEngine->inGame())
	{
		if (!strcmp(event->GetName(), "round_freeze_end"))
			Globals::disableAA = false;

		if (!strcmp(event->GetName(), "round_prestart") || !strcmp(event->GetName(), "round_end"))
			Globals::disableAA = true;
	}
}

int round_start::getEventDebugID( void )
{
	return EVENT_DEBUG_ID_INIT;
}

void round_start::registerSelf( )
{
	g_Interfaces->gameEvent->AddListener( this, "round_start", false );
	g_Interfaces->gameEvent->AddListener( this, "round_end", false );
	g_Interfaces->gameEvent->AddListener( this, "round_prestart", false );
	g_Interfaces->gameEvent->AddListener( this, "round_freeze_end", false );
}

void round_start::unRegisterSelf( )
{
	g_Interfaces->gameEvent->RemoveListener( this );
	g_Interfaces->gameEvent->RemoveListener( this );
	g_Interfaces->gameEvent->RemoveListener( this );
}

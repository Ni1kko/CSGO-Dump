#include "weapon_fire.hpp"

void weapon_fire::fireGameEvent(IGameEvent *event)
{
	if (!Globals::localPlayer || !event)
		return;

	if (g_Interfaces->gameEngine->getPlayerIndex(event->GetInt("userid")) == g_Interfaces->gameEngine->getLocalPlayer())
	{
		// 
	}
}

int weapon_fire::getEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void weapon_fire::registerSelf()
{
	g_Interfaces->gameEvent->AddListener(this, "weapon_fire", false);
}

void weapon_fire::unRegisterSelf()
{
	g_Interfaces->gameEvent->RemoveListener(this);
}

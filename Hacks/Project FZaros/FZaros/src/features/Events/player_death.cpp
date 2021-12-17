#include "player_death.hpp"
#include "../Aimbot/Resolver.hpp"

void player_death::fireGameEvent(IGameEvent *event)
{
	if (!Globals::localPlayer || !event)
		return;

	auto attacker = event->GetInt("attacker");
	auto userid = event->GetInt("userid");
	auto headshot = event->GetBool("headshot");

	if (attacker == g_Interfaces->gameEngine->getLocalPlayer() && userid != g_Interfaces->gameEngine->getLocalPlayer())
	{
		auto entity = g_Interfaces->clientEntity->GetClientEntity(g_Interfaces->gameEngine->getPlayerIndex(userid));

	//	if (Globals::aimbotTarget->EntIndex() == userid)
	//		Globals::MissedShots[entity->EntIndex()] = 0;
	}
}

int player_death::getEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void player_death::registerSelf()
{
	g_Interfaces->gameEvent->AddListener(this, "player_death", false);
}

void player_death::unRegisterSelf()
{
	g_Interfaces->gameEvent->RemoveListener(this);
}

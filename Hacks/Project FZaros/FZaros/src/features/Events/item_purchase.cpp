#include "item_purchase.hpp"

void item_purchase::fireGameEvent(IGameEvent* event)
{
	if (!Globals::localPlayer || !event)
		return;

	if (!(g_Interfaces->gameEngine->connected() && g_Interfaces->gameEngine->inGame()))
		return;

	auto entity = g_Interfaces->clientEntity->GetClientEntity(g_Interfaces->gameEngine->getPlayerIndex(event->GetInt("userid")));
	if (!entity)
		return;

	if (entity->team() == Globals::localPlayer->team())
		return;

	playerinfo_t player_info;
	if (!g_Interfaces->gameEngine->getPlayerInfo2(entity->index(), &player_info))
		return;

	//Globals::Message( "hit %s in the %s for %s (%s remaining)", enemy_info.name, szHitgroup.c_str( ), dmg_to_health, remaining_health );
	std::stringstream msg;

	if ( config->get_bool( "miscPurchasesLog" ) )
	{
		g_Interfaces->cvar->ConsoleColorPrintf(Color::MenuMain, "[dopamine]");

		EventInfo info;

		info.m_flExpTime = g_Interfaces->globalVars->curtime + 4.f;
		msg << player_info.name << " bought a " << event->GetString("weapon") << " .";
		info.m_szMessage = msg.str();

		Globals::eventInfo.emplace_back(info);

		g_Interfaces->cvar->ConsoleColorPrintf(Color(255, 255, 255), "%s", player_info.name);
		g_Interfaces->cvar->ConsoleColorPrintf(Color(255, 255, 255), " bought a ");
		g_Interfaces->cvar->ConsoleColorPrintf(Color(255, 255, 255), "%s", event->GetString("weapon"));
		g_Interfaces->cvar->ConsoleColorPrintf(Color(255, 255, 255), ".");
	}
	else
		if (Globals::eventInfo.size() > 0)
			Globals::eventInfo.clear();
}

int item_purchase::getEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void item_purchase::registerSelf()
{
	g_Interfaces->gameEvent->AddListener(this, "round_start", false);
}

void item_purchase::unRegisterSelf()
{
	g_Interfaces->gameEvent->RemoveListener(this);
}

#include "hooks.hpp"

#include "..//features/misc.hpp"

void CGameEventListener::FireGameEvent(IGameEvent* event)
{
	if (!strcmp(event->GetName(), "item_purchase"))
		g_Misc->BuyInfo(event);
}
#include "../../utilities/includes.h"
#include "../../features/Misc/Misc.hpp"

using fireEventClientSideFn = bool(__thiscall*)(void*, IGameEvent*);

bool __fastcall CHookManager::fireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent)
{
	static auto originalFn = g_Hooks->gameEventHook->getOriginalFunction<fireEventClientSideFn>(9);

	if (!pEvent)
		return originalFn(ecx, pEvent);

	g_Features->onEvent(pEvent);

	return originalFn(ecx, pEvent);
}
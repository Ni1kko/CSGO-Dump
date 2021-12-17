#include "../../utilities/includes.h"
#include "../../menu/menu.hpp"

using lockCursorFn = void(__thiscall*)(void*);

void __stdcall CHookManager::lockCursor()
{
	static auto originalFn = g_Hooks->surfaceHook->getOriginalFunction<lockCursorFn>(67);

	g_Interfaces->inputSystem->enableInput(!menu->opened);
	if (menu->opened)
	{
		g_Interfaces->surface->unlockCursor();
		return;
	}

	originalFn(g_Interfaces->surface);
}
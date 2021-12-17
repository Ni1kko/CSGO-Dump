#include "../../utilities/includes.h"

#include "../../features/Visuals/Visuals.hpp"
#include "../../features/Events/bullet_impact.hpp"

#include <fstream>

void __fastcall CHookManager::sceneEnd(void* thisptr, void* edx)
{
	static auto originalFn = g_Hooks->renderViewHook->getOriginalFunction<decltype(&sceneEnd)>(9);

	originalFn(thisptr, edx);

	g_Visuals.glow();
}
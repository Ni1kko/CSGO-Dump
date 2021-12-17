#pragma once
#include "../Hooking/Hooks/Hooks.h"
#include "../Configuration/Settings.h"

namespace Cheat
{
	extern void wrapgameoverlay();
	extern void unwrapgameoverlay();
	extern bool Initialize();

	extern void Unload();
}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved );

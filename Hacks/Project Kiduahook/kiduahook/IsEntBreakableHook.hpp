#pragma once

namespace IsEntBreakableHook
{
	//extern bool __fastcall HOOKED_IsEntBreakable(void* entity, void* edx);
	void SetupHook();
	void RemoveHook();
}
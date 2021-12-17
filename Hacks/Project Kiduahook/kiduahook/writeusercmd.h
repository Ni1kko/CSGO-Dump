#pragma once

#include "main.h"

bool __fastcall HOOKED_WriteUserCmdDeltaToBuffer(void* ecx, void* edx, int slot, void* buf, int from, int to, bool newcmd);

namespace AdjustPlayerTickHook
{
	void __fastcall Adjust_Player_Time(void* ecx, void* edx, int simulation_ticks);
	void SetupHook();
}
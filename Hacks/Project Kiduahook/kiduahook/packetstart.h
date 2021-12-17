#pragma once

#include "main.h"

namespace PacketStart
{
	void __fastcall HOOKED_PacketStart(void* ecx, void* edx, int incoming_sequence, int outgoing_acknowledged);
	void SetupHook();
}
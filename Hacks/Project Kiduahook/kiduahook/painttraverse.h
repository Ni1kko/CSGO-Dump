#pragma once

#include "main.h"

void __fastcall HOOKED_PaintTraverse( void* ecx, void* edx, unsigned vguiPanel, bool forceRepaint, bool allowForce );

void HOOKED_LockCursor();

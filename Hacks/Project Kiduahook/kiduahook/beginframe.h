#pragma once

#include "main.h"

void __fastcall HOOKED_BeginFrame( void* thisptr );
int	__stdcall HOOKED_IsBoxVisible( const Vector& mins, const Vector& maxs );
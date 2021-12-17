#pragma once

#include "main.h"

namespace UpdateAnimsHook
{
	void ManualUpdateAnimations(CBaseEntity *pEntity);
	void SetupHook();
	void RemoveHook();
}
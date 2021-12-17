#pragma once

#include "includes.h"
#include "UTILS/interfaces.h"
#include "SDK/IEngine.h"
#include "SDK/CUserCmd.h"
#include "SDK/CGlobalVars.h"
#include "SDK/IEngine.h"
#include "SDK/CBaseEntity.h"
#include "SDK/CBaseAnimState.h"
#include "FEATURES/Resolver.h"
#include "FEATURES/AutoWall.h"

#include "SDK/ConVar.h"
#include "UTILS/render.h"
#include "SDK/CTrace.h"
#include "FEATURES/Aimbot.h"
#include "FEATURES/Backtracking.h"

class LinearExtrapolations
{
public:
	void run();
};
extern LinearExtrapolations linear_extraps;
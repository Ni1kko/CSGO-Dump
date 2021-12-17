#pragma once

#include "../../Include/GlobalIncludes.h"
#include "../../SDK/CBaseEntity.h"
#include "../../Configuration/Config.h"
#include "../../Hooking/KeyHandler.h"
#include "../../SDK/player_info_t.h"
#include "c_legitbot.h"

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TIME_TO_TICKS_INT( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

bool IsLineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
bool hitchance(float hitchance, Vector aimangle, CBaseEntity* target);
float RandomFloat(float a, float b);
float GetFovToPlayer(Vector viewAngle, Vector aimAngle);
float GetRealDistanceFOV(float distance, Vector angle, Vector viewangles);
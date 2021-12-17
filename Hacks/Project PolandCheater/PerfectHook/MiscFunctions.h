
#pragma once

#include "SDK.h"

namespace MiscFunctions
{
	void NormaliseViewAngle(Vector &angle);
	bool IsVisible(C_BaseEntity* pLocal, C_BaseEntity* pEntity, int BoneID);
	bool IsKnife(void* weapon);
	bool IsPistol(void* weapon);
	bool IsSniper(void* weapon);
	bool IsGrenade(void* weapon);
	//	int GetPlayerCompRank(C_BaseEntity* pEntity);
};

// Trace Line Memes
void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	C_BaseEntity *ignore, int collisionGroup, trace_t *ptr);

void UTIL_ClipTraceToPlayers(C_BaseEntity* pEntity, Vector start, Vector end, unsigned int mask, ITraceFilter* filter, trace_t* tr);

bool IsBreakableEntity(C_BaseEntity* ent);

bool TraceToExit(Vector start, Vector& dir, Vector& out_end, CGameTrace& enter_trace, CGameTrace& exit_trace);

void SayInChat(const char *text);

Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox);
Vector GetHitboxPositionFromMatrix(C_BaseEntity* pEntity, matrix3x4 matrix[128], int Hitbox);

void ForceUpdate();
inline void clampMovement(CInput::CUserCmd* cmd)
{
    if (cmd->forwardmove >= 450) cmd->forwardmove = 450;
    if (cmd->sidemove >= 450) cmd->sidemove = 450;
    if (cmd->upmove >= 450) cmd->upmove = 450;
    if (cmd->forwardmove <= -450) cmd->forwardmove = -450;
    if (cmd->sidemove <= -450) cmd->sidemove = -450;
    if (cmd->upmove <= -450) cmd->upmove = -450;
}
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f
inline void movementfix(QAngle vOldAngles, CInput::CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
    // side/forward move correction
    float deltaView;
    float f1;
    float f2;

    if (vOldAngles.y < 0.f)
        f1 = 360.0f + vOldAngles.y;
    else
        f1 = vOldAngles.y;

    if (pCmd->viewangles.y < 0.0f)
        f2 = 360.0f + pCmd->viewangles.y;
    else
        f2 = pCmd->viewangles.y;

    if (f2 < f1)
        deltaView = abs(f2 - f1);
    else
        deltaView = 360.0f - abs(f1 - f2);

    deltaView = 360.0f - deltaView;

    pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}
#pragma once

#include "SDK.h"
#include <Psapi.h>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

using ServerRankRevealAllFn = bool(__cdecl*)(float*);

struct Hitbox
{
	Hitbox(void)
	{
		hitbox = -1;
	}

	Hitbox(int newHitBox)
	{
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

enum
{
	FL_HIGH = (1 << 0),
	FL_LOW = (1 << 1),
	FL_SPECIAL = (1 << 2)
};

struct BestPoint
{
	BestPoint(void)
	{
		hitbox = -1;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	BestPoint(int newHitBox)
	{
		hitbox = newHitBox;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	Vector point;
	int  index;
	int  dmg;
	int  flags;
	int  hitbox;
};


namespace GameUtils
{
	extern DWORD FindPattern1(std::string moduleName, std::string pattern);
	extern ServerRankRevealAllFn ServerRankRevealAllEx;
	void NormaliseViewAngle(Vector &angle);
	void CL_FixMove(CUserCmd* cmd, Vector viewangles);
	bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
	std::uint8_t * pattern_scan(void * module, const char * signature);
	bool World2Screen(const Vector & origin, Vector & screen);

//	bool IsBallisticWeapon(void* weapon);
//	bool IsKnife(void * weapon);

	Vector get_hitbox_location(IClientEntity * obj, int hitbox_id);
	bool IsPistol(void* weapon);
	bool AutoSniper(void * weapon);
	bool IsMachinegun(void * weapon);
	bool IsMP(void * weapon);
	bool AWP(void* weapon);

	bool IsBomb(void * weapon);
	bool IsGrenade(void * weapon);
	bool IsRevolver(void * weapon);
	bool IsSniper(void* weapon);
	bool LightSniper(void * weapon);
	bool IsRifle(void * weapon);
	bool IsShotgun(void * weapon);
	bool IsZeus(void * weapon);
	bool IsScout(void * weapon);
	bool IsScopedWeapon(void* weapon);
	int GetPlayerCompRank(IClientEntity* pEntity);
	extern void ServerRankRevealAll();
	unsigned short UTIL_GetAchievementEventMask();
};

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);
void ClipToPlayer(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);
bool IsBreakableEntity(IClientEntity* ent);

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

void SayInChat(const char *text);

void SayInTeamChat(const char *text);

void SetName(std::string name);

Vector GetHitpointPosition(IClientEntity* pEntity, int Hitbox, Vector *Point);

//bool GetBestPoint(IClientEntity* pEntity, Hitbox* hitbox, Vector *point);

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox);
Vector  hitbox_location(IClientEntity* pEntity, int Hitbox);
//Vector GetHitscan(IClientEntity* pEntity, int Hitbox); hitbox_location
QAngle CalcAngleA(Vector src, Vector dst);
Vector GetEyePosition(IClientEntity* pEntity);

void ForceUpdate();

//unsigned short UTIL_GetAchievementEventMask();
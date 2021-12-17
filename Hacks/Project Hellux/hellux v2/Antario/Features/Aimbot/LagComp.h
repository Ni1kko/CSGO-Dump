#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )

struct PlayerRecords
{
	matrix3x4_t Matrix[128];
	float Velocity;
	float SimTime;
	bool Shot;
	Vector m_vecAbsOrigin;
};

class LagComp
{
public:
	int ShotTick[65];
	std::deque<PlayerRecords> PlayerRecord[65] = {  };
	void StoreRecord(C_BaseEntity* pEnt);
	void ClearRecords(int i);
	float LerpTime();
	bool ValidTick(int tick);

	template<class T, class U>
	T clamp(T in, U low, U high);
private:
};
extern LagComp g_LagComp;
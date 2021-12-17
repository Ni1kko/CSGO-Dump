#pragma once

#include "includes.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / INTERFACES::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( INTERFACES::Globals->interval_per_tick *( t ) )
#define clampa(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))


struct PlayerRecords
{
	matrix3x4_t Matrix[128];
	
	float Velocity;
	float SimTime;
	bool Shot;
};

class LagComp
{
public:
	int ShotTick[65];
	matrix3x4_t MatrixUWU[65][128];
	std::deque<PlayerRecords> PlayerRecord[65] = {};
	void StoreRecord(SDK::CBaseEntity* pEnt);
	void ClearRecords(int i);
	float LerpTime();
	bool ValidTick(int tick);

	template<class T, class U>
	T clamp(T in, U low, U high);

private:
};
extern LagComp g_LagComp;

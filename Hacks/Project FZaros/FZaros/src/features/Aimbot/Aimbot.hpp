#pragma once

#include "../../utilities/includes.h"
#include "LagCompensation.hpp"

class CAimbot
{
public:

	void creatMove( );
public:
	matrix3x4_t matrix[ 65 ][ 128 ];
	tickRecord record[ 65 ];
	int sequence;
	int bestEntDmg;
private:
	bool backtrack[ 65 ];
	bool shotBacktrack[ 65 ];
	float simtime[ 65 ];
	Vector3 hitScan( CBaseEntity* pEnt );
	bool hitChance( CBaseEntity* pEnt, CBaseCombatWeapon* pWeapon, Vector3 Angle, Vector3 Point, int chance );
	void autoStop( );
}; extern CAimbot g_Aimbot;
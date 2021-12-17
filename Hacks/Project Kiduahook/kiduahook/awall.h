#pragma once

#include "main.h"
#include "defines.h"
#include "cweapondata.h"
#include "cenginetraceclient.h"

#define CHAR_TEX_FLESH		'F'
#define CHAR_TEX_GRATE		'G'
#define CHAR_TEX_PLASTIC	'L'
#define CHAR_TEX_METAL		'M'
#define CHAR_TEX_CARDBOARD	'U'
#define CHAR_TEX_WOOD		'W'
#define CHAR_TEX_GLASS		'Y'

class CAutowall
{
public:
	struct FireBulletData
	{
		FireBulletData( const Vector &eye_pos )	: src( eye_pos )
		{
		}

		Vector           src;
		CEngineTraceClient::trace_t          enter_trace;
		Vector           direction;
		CEngineTraceClient::CTraceFilter    filter;
		float           trace_length;
		float           trace_length_remaining;
		float           current_damage;
		int             penetrate_count;
	};

	bool TraceToExit( Vector& vEnd, CEngineTraceClient::trace_t* tEnter, Vector vStart, Vector vDir, CEngineTraceClient::trace_t* tExit );

	void ClipTraceToPlayer( Vector vStart, Vector vEnd, int mask, CEngineTraceClient::trace_t * TraceOld, CBaseEntity * pEntity );

	bool GetDamage( const Vector &point, CBaseEntity* pLocal, CBaseEntity* pEnemy, CBaseCombatWeapon* pWeapon, float& flDamage, float flMinDamage );
	float GetCustomDamage( Vector vFrom, const Vector & vTo, CBaseEntity * pFrom, CBaseEntity * pTo, CBaseCombatWeapon * pWeapon );
	bool CanBangForward( CBaseEntity * pLocal, float & dmg );
	float GetThickness( Vector start, Vector end );
//	bool SimulateFireBullet( CBaseEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data );
	bool HandleBulletPenetration( CWeaponData *wpn_data, FireBulletData &data );
	bool SimulateFireBullet( CBaseEntity * local, CBaseEntity * player, CBaseCombatWeapon * weapon, FireBulletData & data, int spot );
};
extern CAutowall g_Autowall;
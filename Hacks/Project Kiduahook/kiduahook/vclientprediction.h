#pragma once

#include "sdk.h"

class CMoveData
{
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	Vector  m_vecViewAngles;        // Command view angles (local space)
	Vector  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	Vector  m_vecAngles;            // edict::angles
	Vector  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[ 5 ];
	Vector  m_vecAbsOrigin;
};

class CPrediction
{
public:
	void Update(int start_frame, bool valid_frame, int inc_ack, int out_cmd)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, bool, int, int);
		getvfunc<OriginalFn>(this, 3)(this, start_frame, valid_frame, inc_ack, out_cmd);
	}
	void SetLocalViewAngles( QAngle& ang )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, QAngle& );
		getvfunc<OriginalFn>( this, 13 )( this, ang );
	}
	void SetupMove( CBaseEntity *player, CUserCmd *ucmd, PVOID empty, PVOID moveData )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, CBaseEntity *player, CUserCmd *ucmd, PVOID empty, PVOID moveData );
		getvfunc<OriginalFn>( this, 20 )( this, player, ucmd, NULL, moveData );
	}
	void FinishMove( CBaseEntity *player, CUserCmd *ucmd, PVOID moveData )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, CBaseEntity *player, CUserCmd *ucmd, PVOID moveData );
		getvfunc<OriginalFn>( this, 21 )( this, player, ucmd, moveData );
	}
};

extern CPrediction* g_pPred;
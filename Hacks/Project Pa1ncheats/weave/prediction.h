#pragma once
#include "Vector.h"
#include "i_base_player.h"
#include "c_usercmd.h"
#include <rpcndr.h>
class IMoveHelper;
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
	float   m_flUnknown[5];
	Vector  m_vecAbsOrigin;
};
class CPrediction
{
public:
	std::byte	pad0[0x8];				// 0x0000
	bool		bInPrediction;			// 0x0008
	std::byte	pad1[0x1];				// 0x0009
	bool		bEnginePaused;			// 0x000A
	std::byte	pad2[0xD];				// 0x000B
	bool		bIsFirstTimePredicted;	// 0x0018

	void Update(int iStartFrame, bool bValidFrame, int nIncomingAcknowledged, int nOutgoingCommand)
	{
		CallVFunc<void>(this, 3, iStartFrame, bValidFrame, nIncomingAcknowledged, nOutgoingCommand);
	}

	void GetLocalViewAngles(Vector& angView)
	{
		CallVFunc<void, Vector&>(this, 12, angView);
	}

	void SetLocalViewAngles(Vector& angView)
	{
		CallVFunc<void, Vector&>(this, 13, angView);
	}

	void CheckMovingGround(IBasePlayer* pEntity, double dbFrametime)
	{
		CallVFunc<void>(this, 18, pEntity, dbFrametime);
	}

	void SetupMove(IBasePlayer* pEntity, CUserCmd* pCmd, IMoveHelper* pHelper, CMoveData* pMoveData)
	{
		CallVFunc<void>(this, 20, pEntity, pCmd, pHelper, pMoveData);
	}

	void FinishMove(IBasePlayer* pEntity, CUserCmd* pCmd, CMoveData* pMoveData)
	{
		CallVFunc<void>(this, 21, pEntity, pCmd, pMoveData);
	}
};

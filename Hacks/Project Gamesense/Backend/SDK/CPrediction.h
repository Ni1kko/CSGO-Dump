#pragma once
#include <Windows.h>
#include "vfunc.hpp"

class Vector;
class CBaseEntity;
class CUserCmd;

class IClientEntity;
class CMoveData;
class IMoveHelper
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle; // edict index on server, client entity handle on client=
	int m_nImpulseCommand; // Impulse command issued.
	Vector m_vecViewAngles; // Command view angles (local space)
	Vector m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons; // Attack buttons.
	int m_nOldButtons; // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector m_vecVelocity; // edict::velocity // Current movement direction.
	Vector m_vecAngles; // edict::angles
	Vector m_vecOldAngles;
	float m_outStepHeight; // how much you climbed this move
	Vector m_outWishVel; // This is where you tried 
	Vector m_outJumpVel; // This is your jump velocity
	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	float m_flUnknown[5];
	Vector m_vecAbsOrigin;

	virtual	void _vpad() = 0;
	virtual void SetHost(IClientEntity* host) = 0;
};

class IGameMovement {
public:
	virtual			~IGameMovement(void) { }

	virtual void	          ProcessMovement(CBaseEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	          Reset(void) = 0;
	virtual void	          StartTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void	          FinishTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void	          DiffPrint(char const *fmt, ...) = 0;
	virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool		        IsMovingPlayerStuck(void) const = 0;
	virtual CBaseEntity*    GetMovingPlayer(void) const = 0;
	virtual void		        UnblockPusher(CBaseEntity *pPlayer, CBaseEntity *pPusher) = 0;
	virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class IPrediction {
public:
	bool InPrediction() {
		typedef bool(__thiscall* oInPrediction)(void*);
		return vfunc<oInPrediction>(this, 14)(this);
	}

	void RunCommand(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper) {
		typedef void(__thiscall* oRunCommand)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
		return vfunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData) {
		typedef void(__thiscall* oSetupMove)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*, void*);
		return vfunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(CBaseEntity *player, CUserCmd *ucmd, void*pMoveData) {
		typedef void(__thiscall* oFinishMove)(void*, CBaseEntity*, CUserCmd*, void*);
		return vfunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};

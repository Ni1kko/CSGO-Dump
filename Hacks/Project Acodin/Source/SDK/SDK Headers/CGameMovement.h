#pragma once
#include "stdafx.h"

class CMoveData;

class IMoveHelper
{
public:
	void SetHost( CBaseEntity* pPlayer )
	{
		typedef void (__thiscall* OriginalFn)( PVOID, CBaseEntity* pPlayer );
		getvfunc< OriginalFn >( this, 1 )( this, pPlayer );
	}
};

//class CPrediction
//{
//public:
//	void SetupMove(CBaseEntity *player, CInput::CUserCmd *ucmd, PVOID empty, PVOID moveData);
//	void FinishMove(CBaseEntity *player, CInput::CUserCmd *ucmd, PVOID moveData);
//};

class CGameMovement
{
public:
	virtual			~CGameMovement(void) {}

	virtual void	ProcessMovement(CBaseEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CBaseEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual CBaseEntity*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(CBaseEntity* pPlayer, CBaseEntity *pPusher) = 0;

	virtual void    SetupMovementBounds(CMoveData *pMove) = 0;
};
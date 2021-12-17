#pragma once

#include "CommonIncludes.h"
#include "SDK.h"
#include "CClientState.h"
#include "glow_outline_effect.h"
#include "IClientMode.h"
#include <d3d9.h>
#include "CUtlVector.h"

class IMoveHelper : public Interface
{
public:
	void SetHost(IClientEntity* pEnt)
	{
		typedef void(__thiscall* SetHostFn)(void*, IClientEntity*);
		return VFunc<SetHostFn>(1)(this, pEnt);
	}
};

class IPrediction : public Interface
{
public:
	void	SetupMove(IClientEntity *player, CUserCmd *pCmd, IMoveHelper *pHelper, void *move)
	{
		typedef void(__thiscall* SetupMoveFn)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, void*);
		return VFunc<SetupMoveFn>(20)(this, player, pCmd, pHelper, move);
	}

	void	FinishMove(IClientEntity *player, CUserCmd *pCmd, void *move)
	{
		typedef void(__thiscall* FinishMoveFn)(void*, IClientEntity*, CUserCmd*, void*);
		return VFunc<FinishMoveFn>(21)(this, player, pCmd, move);
	}

	void set_local_viewangles_rebuilt(Vector& angle)
	{
		typedef void(__thiscall *o_updateClientSideAnimation)(void*, Vector&);
		call_vfunc<o_updateClientSideAnimation>(this, 13)(this, angle);
	}

};

class IGameMovement : public Interface
{
public:
	void ProcessMovement(IClientEntity *pPlayer, void *pMove)
	{
		typedef void(__thiscall* ProcessMovementFn)(void*, IClientEntity*, void*);
		return VFunc<ProcessMovementFn>(1)(this, pPlayer, pMove);
	}

	void StartTrackPredictionErrors(IClientEntity *pPlayer)
	{
		typedef void(__thiscall *o_StartTrackPredictionErrors)(void*, void*);
		return VFunc<o_StartTrackPredictionErrors>(3)(this, pPlayer);
	}

	void FinishTrackPredictionErrors(IClientEntity *pPlayer)
	{
		typedef void(__thiscall *o_FinishTrackPredictionErrors)(void*, void*);
		return VFunc<o_FinishTrackPredictionErrors>(4)(this, pPlayer);
	}
};

class CMoveData {
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	bool m_bNoAirControl : 1;

	unsigned long m_nPlayerHandle;
	int m_nImpulseCommand;
	QAngle m_vecViewAngles;
	QAngle m_vecAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	Vector m_vecVelocity;
	Vector m_vecOldVelocity;
	float somefloat;
	QAngle m_vecAngles;
	QAngle m_vecOldAngles;

	float m_outStepHeight;
	Vector m_outWishVel;
	Vector m_outJumpVel;

	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	bool m_bConstraintPastRadius;

	void SetAbsOrigin(const Vector& vec);
	const Vector& GetOrigin() const;

private:
	Vector m_vecAbsOrigin;
};
class CGameMovement
{
public:
	virtual			~CGameMovement(void) {}

	virtual void	ProcessMovement(IClientEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual IClientEntity*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(IClientEntity* pPlayer, IClientEntity *pPusher) = 0;

	virtual void    SetupMovementBounds(CMoveData *pMove) = 0;
};
namespace Interfaces
{
	void Initialise();
	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IPanel* Panels;
	extern IClientEntityList* EntList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern IClientModeShared *ClientMode;
	extern CGlobalVarsBase *Globals;
	extern DWORD *Prediction;
	extern CMaterialSystem* MaterialSystem;
	extern CVRenderView* RenderView;
	extern IDirect3DDevice9 *g_pD3DDevice9;
	extern IVModelRender* ModelRender;
	extern CGlowObjectManager* m_pGlowObjManager;
	extern CModelInfo* ModelInfo;
	extern HWND window;
	extern IInputSystem* m_iInputSys;
	extern CUtlVectorSimple *g_ClientSideAnimationList;
	extern IEngineTrace* Trace;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICVar *CVar;
	extern CInput* pInput;
	extern IGameEventManager2 *EventManager;
	extern HANDLE __FNTHANDLE;
	extern IVEffects* m_pEffects;
	extern IVEffects* m_pEffects;
	extern IMoveHelper* MoveHelper;
	extern IPrediction *Prediction1;
	extern CClientState* m_pClientState;
	extern IGameMovement* GameMovement;
	extern IViewRenderBeams* g_pViewRenderBeams;
	extern	CGameMovement * c_gamemove;
};
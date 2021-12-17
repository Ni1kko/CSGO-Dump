#pragma once

#include "../math/QAngle.hpp"
#include "../misc/CUserCmd.hpp"
#include "IMoveHelper.hpp"

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

class c_base_player;

class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	          ProcessMovement(c_base_player *pPlayer, void *pMove) = 0;
    virtual void	          Reset(void) = 0;
    virtual void	          StartTrackPredictionErrors(c_base_player *pPlayer) = 0;
    virtual void	          FinishTrackPredictionErrors(c_base_player *pPlayer) = 0;
    virtual void	          DiffPrint(char const *fmt, ...) = 0;
    virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
    virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
    virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
    virtual bool		        IsMovingPlayerStuck(void) const = 0;
    virtual c_base_player*   GetMovingPlayer(void) const = 0;
    virtual void		        UnblockPusher(c_base_player *pPlayer, c_base_player *pPusher) = 0;
    virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class CGameMovement
    : public IGameMovement
{
public:
    virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
    bool InPrediction()
    {
        typedef bool(__thiscall* oInPrediction)(void*);
        return CallVFunction<oInPrediction>(this, 14)(this);
    }

    void RunCommand(c_base_player *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
    {
        typedef void(__thiscall* oRunCommand)(void*, c_base_player*, CUserCmd*, IMoveHelper*);
        return CallVFunction<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
    }

    void SetupMove(c_base_player *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
    {
        typedef void(__thiscall* oSetupMove)(void*, c_base_player*, CUserCmd*, IMoveHelper*, void*);
        return CallVFunction<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
    }

    void FinishMove(c_base_player *player, CUserCmd *ucmd, void*pMoveData)
    {
        typedef void(__thiscall* oFinishMove)(void*, c_base_player*, CUserCmd*, void*);
        return CallVFunction<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
    }
};
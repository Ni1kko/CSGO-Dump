#pragma once

#include "../math/QAngle.hpp"
#include "../misc/usercmd_t.hpp"
#include "IMoveHelper.hpp"

class CMoveData
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
    Vector m_vecVelocity; // edict::velocity        // Current movement direction.
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
};

class C_BasePlayer;

class IGameMovement
{
public:
    virtual ~IGameMovement ( void )
    {
    }

    virtual void process_movement ( C_BasePlayer* pPlayer, CMoveData* pMove ) = 0;
    virtual void reset ( void ) = 0;
    virtual void start_track_prediction_errors ( C_BasePlayer* pPlayer ) = 0;
    virtual void finish_track_prediction_errors ( C_BasePlayer* pPlayer ) = 0;
    virtual void diff_print ( char const* fmt, ... ) = 0;
    virtual Vector const& get_player_mins ( bool ducked ) const = 0;
    virtual Vector const& get_player_maxs ( bool ducked ) const = 0;
    virtual Vector const& get_player_view_offset ( bool ducked ) const = 0;
    virtual bool is_moving_player_stuck ( void ) const = 0;
    virtual C_BasePlayer* get_moving_player ( void ) const = 0;
    virtual void unblock_pusher ( C_BasePlayer* pPlayer, C_BasePlayer* pPusher ) = 0;
    virtual void setup_movement_bounds ( CMoveData* pMove ) = 0;
};

class CGameMovement
    : public IGameMovement
{
public:
    virtual ~CGameMovement ( void )
    {
    }
};

class IPrediction
{
public:
    bool in_prediction ( )
    {
        typedef bool (__thiscall* oInPrediction) ( void* );
        return CallVFunction< oInPrediction > ( this, 14 ) ( this );
    }

    void run_command ( C_BasePlayer* player, usercmd_t* ucmd, IMoveHelper* moveHelper )
    {
        typedef void (__thiscall* oRunCommand) ( void*, C_BasePlayer*, usercmd_t*, IMoveHelper* );
        return CallVFunction< oRunCommand > ( this, 19 ) ( this, player, ucmd, moveHelper );
    }

    void setup_move ( C_BasePlayer* player, usercmd_t* ucmd, IMoveHelper* moveHelper, void* pMoveData )
    {
        typedef void (__thiscall* oSetupMove) ( void*, C_BasePlayer*, usercmd_t*, IMoveHelper*, void* );
        return CallVFunction< oSetupMove > ( this, 20 ) ( this, player, ucmd, moveHelper, pMoveData );
    }

    void finish_move ( C_BasePlayer* player, usercmd_t* ucmd, void* pMoveData )
    {
        typedef void (__thiscall* oFinishMove) ( void*, C_BasePlayer*, usercmd_t*, void* );
        return CallVFunction< oFinishMove > ( this, 21 ) ( this, player, ucmd, pMoveData );
    }
};

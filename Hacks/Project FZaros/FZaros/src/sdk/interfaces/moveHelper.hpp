#pragma once

class IClientEntity;

class IMoveHelper
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle; // edict index on server, client entity handle on client=
	int m_nImpulseCommand; // Impulse command issued.
	Vector3 m_vecViewAngles; // Command view angles (local space)
	Vector3 m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons; // Attack buttons.
	int m_nOldButtons; // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector3 m_vecVelocity; // edict::velocity // Current movement direction.
	Vector3 m_vecAngles; // edict::angles
	Vector3 m_vecOldAngles;
	float m_outStepHeight; // how much you climbed this move
	Vector3 m_outWishVel; // This is where you tried 
	Vector3 m_outJumpVel; // This is your jump velocity
	Vector3 m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	float m_flUnknown[5];
	Vector3 m_vecAbsOrigin;

    virtual	void _vpad() = 0;
    virtual void SetHost(IClientEntity* host) = 0;
};
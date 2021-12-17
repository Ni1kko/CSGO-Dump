#pragma once
#include "..\Utils\GlobalVars.h"
#include "..\Settings.h"

class Misc
{
public:
    void OnCreateMove()
    {
        this->pCmd   = g::pCmd;
        this->pLocal = g::pLocalEntity;


        if (g_Settings.bBhopEnabled)
            this->DoBhop();

		if (g_Settings.bAirStrafer)
			this->air_strafer_create_move();
        // sum future shit
    };
private:
    CUserCmd*     pCmd;
    C_BaseEntity* pLocal;

    void DoBhop() const
    {
        if (this->pLocal->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
            return;
			
        static bool bLastJumped = false;
        static bool bShouldFake = false;

        if (!bLastJumped && bShouldFake)
        {
            bShouldFake = false;
            pCmd->buttons |= IN_JUMP;
        }
        else if (pCmd->buttons & IN_JUMP)
        {
            if (pLocal->GetFlags() & FL_ONGROUND)
                bShouldFake = bLastJumped = true;
            else 
            {
                pCmd->buttons &= ~IN_JUMP;
                bLastJumped = false;
            }
        }
        else
            bShouldFake = bLastJumped = false;
    }

	template<class T, class U>
	static T clamp(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;

	}
	void air_strafer_create_move() 
	{
		if (!GetAsyncKeyState(VK_SPACE))
			return;

		if (GetAsyncKeyState('S') || GetAsyncKeyState('D') || GetAsyncKeyState('A') || GetAsyncKeyState('W'))
			return;

		if (g::pLocalEntity->GetMoveType() == MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MOVETYPE_LADDER)
			return;

		if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		{
			pCmd->forwardmove = (1550.f * 5)  / g::pLocalEntity->GetVelocity().Length();
			pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.0f : 450.0f;
			if (pCmd->forwardmove > 450.0f)
				pCmd->forwardmove = 450.0f;
		}
	}
};
extern Misc g_Misc;

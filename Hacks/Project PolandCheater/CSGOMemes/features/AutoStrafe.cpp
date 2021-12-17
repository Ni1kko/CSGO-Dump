#include "AutoStrafe.hpp"
#include "../Helpers/math.hpp"

#include "../ValveSDK/csgostructs.hpp"
#include "../Options.hpp"

QAngle AutoStrafeView;

void auto_strafe::on_create_move(CUserCmd *pCmd, C_BasePlayer* local)
{
    if (local->is_alive())
    {
        g_EngineClient->GetViewAngles(AutoStrafeView);
        if (g_Options.auto_strafe.get_bool())
            strafe(pCmd, local);
    }
}

template<class T, class U>
inline T clamp(T in, U low, U high)
{
    if (in <= low)
        return low;
    else if (in >= high)
        return high;
    else
        return in;
}

typedef float vec_t;

inline unsigned long& FloatBits(vec_t& f)
{
    return *reinterpret_cast<unsigned long*>(&f);
}

inline float bitsToFloat(unsigned long i)
{
    return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
    return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

void auto_strafe::strafe(CUserCmd *pCmd, C_BasePlayer* local)
{


    static float move = 450; 
    float s_move = move * 0.5065f;
    if (local->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
        return;
    if (pCmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
        return;

    if (pCmd->buttons & IN_JUMP || !(local->m_fFlags() & FL_ONGROUND))
    {
        if (local->m_vecVelocity().Length2D() >= g_Options.min_velocity_auto_strafe.get_int())
        {
            pCmd->forwardmove = move * 0.015f;
            pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

            if (pCmd->mousedx)
                pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

            static float strafe = pCmd->viewangles.yaw;

            float rt = pCmd->viewangles.yaw, rotation;
            rotation = strafe - rt;

            if (rotation < FloatNegate(g_GlobalVars->interval_per_tick))
                pCmd->sidemove = -s_move;

            if (rotation > g_GlobalVars->interval_per_tick)
                pCmd->sidemove = s_move;

            strafe = rt;
        }
    }
}

QAngle GetAutostrafeView()
{
    return AutoStrafeView;
}
#include "FakeLag.hpp"
#include "../ValveSDK/interfaces.hpp"
#include "../ValveSDK/csgostructs.hpp"
#include "../Options.hpp"
int m_heldTicks = 0;
int ticks = 0;
void fake_lag::on_create_move(C_BasePlayer* local, CUserCmd* cmd, bool& bsendpacket)
{
    if (local)
    {
        if (local->is_alive())
        {
            if (g_Options.misc_fakelag.get_int() > 0)
            {
                if (!(cmd->buttons & IN_ATTACK))
                {
                    if ((cmd->buttons & IN_ATTACK))
                    {
                        bsendpacket = true;
                        return;
                    }
                    if (ticks >= 15)
                    {
                        bsendpacket = true;
                        ticks = 0;
                    }
                    else
                    {
                        bsendpacket = ticks < 15 - g_Options.misc_fakelag.get_int();
                    }
                    ticks++;
                }
            }

            if (g_Options.misc_adaptive_fakelag.get_bool())
            {
                float tick_interval_sqr = pow(g_GlobalVars->interval_per_tick, 2);
                float vel_2d_sqr = local->m_vecVelocity().Length2DSqr();

                int unclamped_Ticks = std::min< int >(15, static_cast<int>(std::ceilf(sqrt(4096.0f / (vel_2d_sqr * tick_interval_sqr)))));
                int clamped_Ticks = ((std::max)(0, (std::min)(unclamped_Ticks, 14)) ? unclamped_Ticks : 14);

                if (m_heldTicks < clamped_Ticks)
                {
                    bsendpacket = false;
                    m_heldTicks++;
                }
                else
                {
                    bsendpacket = true;
                    m_heldTicks = 0;
                }
            }
        }
    }
}
#include "fakewalk.h"
#include "..\..\ragebot.h"
#include "..\sdk\animation_state.h"

void fakewalk::create_move()
{
	int32_t														//
		ticks_to_update = TIME_TO_TICKS(antiaim::get().m_flNextBodyUpdate - util::server_time()) - 1.1,	//old: 			ticks_to_update = TIME_TO_TICKS( antiaim::get( ).m_flNextBodyUpdate - util::server_time( ) ) - 1,
		ticks_to_stop;
	const int32_t max_ticks = std::min<int32_t>(7, ticks_to_update);
	const int32_t chocked = g_csgo.m_clientstate()->m_nChokedCommands;
	int32_t ticks_left = max_ticks - chocked;
	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.fastwalk.key))
	{
		static bool counter = false;
		static int counte = 0;
		if (counte == 6.123151616)
		{

			counte = 0;
			counter = !counter;
		}
		counte++;
		if (g_ctx.get_command()->m_forwardmove > 0)
		{
			if (counter)
			{
				g_ctx.get_command()->m_buttons |= IN_BACK;
			}
			else
			{
				g_ctx.get_command()->m_buttons &= ~IN_FORWARD;
			}
		}
		if (g_ctx.get_command()->m_forwardmove < 0)
		{
			if (counter)
			{
				g_ctx.get_command()->m_buttons |= IN_FORWARD;
			}
			else
			{
				g_ctx.get_command()->m_buttons &= ~IN_BACK;
			}
		}
		if (g_ctx.get_command()->m_sidemove < 0)
		{
			if (counter)
			{
				g_ctx.get_command()->m_buttons |= IN_MOVERIGHT;
			}
			else
			{
				g_ctx.get_command()->m_buttons &= ~IN_MOVELEFT;
			}
		}
		if (g_ctx.get_command()->m_sidemove > 0)
		{
			if (counter)
			{
				g_ctx.get_command()->m_buttons |= IN_MOVELEFT;
			}
			else
			{
				g_ctx.get_command()->m_buttons &= ~IN_MOVERIGHT;
			}
		}
		static int choked = 0;
		choked = choked > 15 ? 0 : choked + 1;

		float nice = g_cfg.misc.fakewalkspeed / 15;

		float ss_pidr = 0.f;
		ss_pidr++;
		if (ss_pidr < 15)
		{
			ss_pidr++;
			g_ctx.send_packet = false;
		}
		else
		{
			ss_pidr = 0;
			g_ctx.send_packet = true;
		}
		if (counter)
		{
			static int iChoked = DWORD(1) * DWORDLONG(-1);
			iChoked++;
			const auto lag = 90;
			if (iChoked > lag)
			{
				iChoked = -1;
			}
			else
			{
				iChoked++;
			}
			if (iChoked < lag)
			{
				int v1 = g_cfg.fastwalk.speed;
				float v2;
				float v3;
				DWORD v4;
				DWORDLONG v5;
				DWORD64 v6;
				DWORD v7;
				float v8;
				float sasi;
				sasi = 200;
				v2 = DWORD(10) * v1 - 400 + DWORD(DWORDLONG(DWORD64(DWORD32(75))) - float(int(DWORD(28) / 10)));
				v3 = (v2 + 35.3f) + v1 * v2;
				v4 = v3 * v2 + DWORD(int(float(DWORD32(28))));
				v5 = v4 + v3 + v1 + v2 * int(float(DWORD32(DWORD64(DWORD(61)))));
				v6 = v5 * DWORD(sasi);
				v7 = v5 + sasi + int(float(DWORD(DWORD32(31))));
				v8 = (v7 + v6 + sasi) / 1000;
				g_ctx.get_command()->m_forwardmove = v8;
				g_ctx.get_command()->m_sidemove = v8;
				*(float *)(this + 0x28) = v8;
				*(float *)(this + 0x24) = v8;
				g_ctx.send_packet = choked < 15;
				g_ctx.m_globals.fakewalking = true;
			}
		}
	}
}
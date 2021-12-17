#include "fakelag.h"
#include "..\autowall\autowall.h"

int fakelag::BreakLagCompensation()
{
	float speed = g_ctx.m_local->m_vecVelocity().Length2D();
	if (speed > 0.f)
	{
		auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
}

void fakelag::create_move(CUserCmd *pCmd) {
	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	if (g_csgo.m_engine()->IsVoiceRecording())
		return;

	bool should_fakelag = false;
	bool should_exploit = false;
	static bool new_factor = false;

	int
		tick_times = ((int)(1.0f / g_csgo.m_globals()->m_interval_per_tick)) / 64,
		maxchockeamount = 14,
		choke = 0;

	if (g_cfg.antiaim.breaklagcomp)
		BreakLagCompensation();

	switch (g_cfg.antiaim.fakelag_mode)
	{
	case 1:
	{
		if (standing)
			choke = min(fakelag_amountstand * tick_times, 14);
		else if (shooting)
			choke = min(fakelag_amountshoot * tick_times, 14);
		else if (moving)
			choke = min(fakelag_amountmove * tick_times, 14);
		else if (inair)
			choke = min(fakelag_amountair * tick_times, 14);
	}break;
	case 2:
	{
		if (standing)
			choke = math::clamp(static_cast<int>(std::ceilf(69 / (g_ctx.m_local->m_vecVelocity().Length() * g_csgo.m_globals()->m_interval_per_tick))), 1, 14);
		else if (shooting)
			choke = math::clamp(static_cast<int>(std::ceilf(69 / (g_ctx.m_local->m_vecVelocity().Length() * g_csgo.m_globals()->m_interval_per_tick))), 1, 14);
		else if (moving)
			choke = math::clamp(static_cast<int>(std::ceilf(69 / (g_ctx.m_local->m_vecVelocity().Length() * g_csgo.m_globals()->m_interval_per_tick))), 1, 14);
		else if (inair)
			choke = math::clamp(static_cast<int>(std::ceilf(69 / (g_ctx.m_local->m_vecVelocity().Length() * g_csgo.m_globals()->m_interval_per_tick))), 1, 14);
	}break;
	case 3:
	{
		if (standing)
			choke = max(1, min((int)(fabs(g_ctx.m_local->m_vecVelocity().Length() / 80.f)), 5));
		else if (shooting)
			choke = max(1, min((int)(fabs(g_ctx.m_local->m_vecVelocity().Length() / 80.f)), 5));
		else if (moving)
			choke = max(1, min((int)(fabs(g_ctx.m_local->m_vecVelocity().Length() / 80.f)), 5));
		else if (inair)
			choke = max(1, min((int)(fabs(g_ctx.m_local->m_vecVelocity().Length() / 80.f)), 5));
	}break;
	case 4:
	{
		if (new_factor)
		{
			static int FakelagFactor = 10;
			static int m_iOldHealth = 0;
			static int iChoked;
			if (m_iOldHealth != g_ctx.m_local->m_iHealth())
			{
				m_iOldHealth = g_ctx.m_local->m_iHealth();
				if (standing)
					choke = rand() % 7 + 6 + 1;
				else if (shooting)
					choke = rand() % 7 + 6 + 1;
				else if (moving)
					choke = rand() % 7 + 6 + 1;
				else if (inair)
					choke = rand() % 7 + 6 + 1;
				new_factor = false;
			}
		}
	}break;
	case 5:
	{
		if (new_factor)
		{
			int min_packets = 0;
			Vector vel_p_t = Vector(g_ctx.m_local->m_vecVelocity() *  g_csgo.m_globals()->m_interval_per_tick);
			for (int i = 1; i <= 14; i++)
			{
				if (Vector(vel_p_t * i).Length() > 64)
				{
					min_packets = i;
					break;
				}
			}
			if (standing)
				choke = max(min_packets, fakelag_amountstand);
			else if (shooting)
				choke = max(min_packets, fakelag_amountshoot);
			else if (moving)
				choke = max(min_packets, fakelag_amountmove);
			else if (inair)
				choke = max(min_packets, fakelag_amountair);
		}
	}break;

	case 6:
	{
		Vector velocity = g_ctx.m_local->m_vecVelocity();
		velocity.z = 0;
		float speed = velocity.Length();
		if (speed > 0.f) {
			auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
			int choked_ticks = std::ceilf(65.f / distance_per_tick);
			if (g_ctx.m_local->m_fFlags() & FL_ONGROUND)
				choke = std::min<int>(choked_ticks, g_cfg.antiaim.fakelag_amountmoving);
			else
				choke = std::min<int>(choked_ticks, g_cfg.antiaim.fakelag_amountinair);
		}
		else
			choke = 1;
	}break;
	}

	if (g_cfg.antiaim.fakelag_enablers[FAKELAG_AIR].enabled)
	{
		if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND))
			should_fakelag = true;
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND)
	{
		if (g_cfg.antiaim.fakelag_enablers[FAKELAG_MOVING].enabled)
			should_fakelag = true;
	}

	if (g_cfg.antiaim.fakelag_enablers[FAKELAG_SHOOTING].enabled)
	{
		if (g_ctx.get_command()->m_buttons & IN_ATTACK && weapon->can_fire())
			should_fakelag = true;
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND)
	{
		if (g_cfg.antiaim.fakelag_enablers[FAKELAG_STANDING].enabled)
		{
			should_fakelag = true;
		}
	}

	if (!g_cfg.antiaim.fakelag_enablers[FAKELAG_SHOOTING].enabled) {
		if (g_ctx.get_command()->m_buttons & IN_ATTACK && weapon->can_fire())
			should_fakelag = false;
	}

	if (g_ctx.m_globals.fakewalking)
		return;

	if (should_fakelag)
	{
		static int choked = 0;

		if (choked > g_cfg.antiaim.fakelag_amount)
		{
			g_ctx.send_packet = true;
			choked = 0;
		}
		else
		{
			g_ctx.send_packet = false;
			choked++;
		}
		g_ctx.m_globals.fakelagamt = choked;

		g_ctx.m_globals.fakelagging = true;
	}
}

#include "..\..\ragebot.h"

void manual_aa::update() {
	static bool
		holding_left = false,
		holding_right = false,
		holding_back = false;

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.left_manualkey) && !holding_left)
	{
		if (side == SIDE_LEFT)
		{
			side = SIDE_NONE;
		}
		else 
		{
			side = SIDE_LEFT;
		}

		holding_left = true;
	}
	else if (holding_left && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.left_manualkey)) {
		holding_left = false;
	}

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.right_manualkey) && !holding_right) {
		if (side == SIDE_RIGHT) {
			side = SIDE_NONE;
		}
		else {
			side = SIDE_RIGHT;
		}

		holding_right = true;
	}
	else if (holding_right && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.right_manualkey)) {
		holding_right = false;
	}

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.back_manualkey) && !holding_back) {
		if (side == SIDE_BACK) {
			side = SIDE_NONE;
		}
		else {
			side = SIDE_BACK;
		}

		holding_back = true;
	}
	else if (holding_back && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.back_manualkey)) {
		holding_back = false;
	}
	// 	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.disable_manualkey) && !holding_back)
	// 	{
	// 		int side = SIDE_NONE;
	// 	}
}

bool manual_aa::run() {
	vec3_t engine_angles; g_csgo.m_engine()->GetViewAngles(engine_angles);
	float yaw = engine_angles.y;
	float LBYDelta = g_ctx.get_command()->m_viewangles.y - g_ctx.m_local->m_flLowerBodyYawTarget(); bool breakinglby;
	if (fabsf(LBYDelta) > 35.f || fabsf(LBYDelta) < -35.f)
		breakinglby = true;
	else
		breakinglby = false;
	switch (side)
	{
	case SIDE_LEFT:
		if (g_ctx.send_packet) {
			switch (g_cfg.antiaim.manualaafakes)
			{
			case 0:
				g_ctx.get_command()->m_viewangles.y = yaw - (90.0f + (math::random_float(0.f, 30.f) - 15.f)); break;
			case 1:
			{
				if (breakinglby)
					g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget();
				else
					g_ctx.get_command()->m_viewangles.y = (g_ctx.m_local->m_flLowerBodyYawTarget() + 140 + rand() % 123) - rand() % 20;
			}break;
			case 2:
			{
				g_ctx.get_command()->m_viewangles.y -= (float)(fmod(util::server_time() / 0.5f * 75.0f, 105.0f));
			}break;
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw + 90.0f;
		}

		break;
	case SIDE_RIGHT:
		if (g_ctx.send_packet) {
			switch (g_cfg.antiaim.manualaafakes)
			{
			case 0:
				g_ctx.get_command()->m_viewangles.y = yaw + (90.0f + (math::random_float(0.f, 30.f) - 15.f)); break;
			case 1:
			{
				if (breakinglby)
					g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget();
				else
					g_ctx.get_command()->m_viewangles.y = (g_ctx.m_local->m_flLowerBodyYawTarget() + 140 + rand() % 123) - rand() % 20;
			}break;
			case 2:
			{
				g_ctx.get_command()->m_viewangles.y += (float)(fmod(util::server_time() / 0.5f * 75.0f, 105.0f));
			}break;
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw - 90.0f;
		}

		break;
	case SIDE_BACK:
		if (g_ctx.send_packet) {
			static bool side[2] = { false, true };

			float range = side[1] ? 100 : 80;

			g_ctx.get_command()->m_viewangles.y = yaw + (side[0] ? range : -range);

			side[1] = !side[1];

			if (side[1]) {
				side[0] = !side[0];
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw + 180.0f;
		}

		break;
	}

	return side != SIDE_NONE;
}
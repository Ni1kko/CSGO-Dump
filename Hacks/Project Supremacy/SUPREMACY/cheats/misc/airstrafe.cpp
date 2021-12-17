#include "airstrafe.h"
#include "circlestrafer.h"

#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

void airstrafe::create_move() {
	static auto down = [](ButtonCode_t bt) -> bool {
		return g_csgo.m_inputsys()->IsButtonDown(bt);
	};

	if (g_ctx.m_local->get_move_type() == MOVETYPE_NOCLIP || g_ctx.m_local->get_move_type() == MOVETYPE_LADDER)
		return;

	if (down(KEY_A) || down(KEY_D) || down(KEY_S) || down(KEY_W) || down(KEY_LSHIFT))
		return;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND) && g_ctx.m_local->m_vecVelocity().Length2D() > 20) {
		if (g_ctx.get_command()->m_mousedx > 1 || g_ctx.get_command()->m_mousedx < -1) {
			g_ctx.get_command()->m_sidemove = math::clamp(g_ctx.get_command()->m_mousedx < 0.f ? -450.0f : 450.0f, -450.0f, 450.0f);
		}
		else {
			g_ctx.get_command()->m_forwardmove = 10000.f / g_ctx.m_local->m_vecVelocity().Length();
			g_ctx.get_command()->m_sidemove = (g_ctx.get_command()->m_command_number % 2) == 0 ? -450.0f : 450.0f;
			if (g_ctx.get_command()->m_forwardmove > 450.0f)
				g_ctx.get_command()->m_forwardmove = 450.0f;
		}
	}
	//hm
}
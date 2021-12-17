#include "bunnyhop.h"

void bunnyhop::create_move() {
	if (g_ctx.m_local->get_move_type() == MOVETYPE_NOCLIP || g_ctx.m_local->get_move_type() == MOVETYPE_LADDER)
		return;

	if (g_ctx.get_command()->m_buttons & IN_JUMP && !(g_ctx.m_local->m_fFlags() & FL_ONGROUND))
		g_ctx.get_command()->m_buttons &= ~IN_JUMP;
}
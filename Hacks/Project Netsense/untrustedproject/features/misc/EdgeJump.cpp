#include "EdgeJump.hpp"
#include "../../helpers/input.hpp"
#include "misc.hpp"

int flags_backup = 0;

void EdgeJump::PrePredictionCreateMove(CUserCmd* cmd)
{
	if (!g_Misc->options.edge_jump) return;
	if (!g_Misc->options.edge_jumping) return;
	if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP) return;

	flags_backup = g_LocalPlayer->m_fFlags();
}

void EdgeJump::PostPredictionCreateMove(CUserCmd* cmd)
{
	if (!g_Misc->options.edge_jump) return;
	if (!g_Misc->options.edge_jumping) return;
	if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP) return;

	if (flags_backup & FL_ONGROUND && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		cmd->buttons |= IN_JUMP;
}
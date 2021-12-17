#include "LegitAA.hpp"
#include "../../helpers/math.hpp"
#include "../../helpers/input.hpp"
#include "../../render.hpp"
#include "misc.hpp"

LegitAA* g_LegitAA = new LegitAA();

void LegitAA::CreateMove(CUserCmd* pCmd, bool* bSendPacket) {
	if (pCmd->buttons & (IN_ATTACK | IN_ATTACK2 | IN_USE) ||
		g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP ||
		!g_LocalPlayer->IsAlive())
		return;
	auto pressing_button = pCmd->buttons & IN_MOVELEFT || pCmd->buttons & IN_MOVERIGHT || pCmd->buttons & IN_FORWARD || pCmd->buttons & IN_BACK;
	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();
	if (!weapon) 
		return;

	short weapon_idx = weapon->m_Item().m_iItemDefinitionIndex();
	if ((weapon_idx == WEAPON_GLOCK || weapon_idx == WEAPON_FAMAS) && weapon->m_flNextPrimaryAttack() >= g_GlobalVars->curtime)
		return;


	if (weapon_idx == WEAPON_HEGRENADE ||
		weapon_idx == WEAPON_FRAG_GRENADE ||
		weapon_idx == WEAPON_INCGRENADE ||
		weapon_idx == WEAPON_SMOKEGRENADE ||
		weapon_idx == WEAPON_TAGRENADE ||
		weapon_idx == WEAPON_DECOY ||
		weapon_idx == WEAPON_FLASHBANG ||
		weapon_idx == WEAPON_MOLOTOV) { // hadrcoded aswell
		if (!weapon->m_bPinPulled() && weapon->m_fThrowTime() > 0.f)
			return;

		if (((pCmd->buttons & IN_ATTACK) || (pCmd->buttons & IN_ATTACK2)) && weapon->m_fThrowTime() > 0.f)
			return;
	}



	QAngle OldAngles = pCmd->viewangles;

	static bool broke_lby = false;
	bool change_side = false;

	if (g_Misc->options.legit_aa_flip) {
		change_side = true;
		g_Misc->options.legit_aa_flip = false;
	}
		//side = -side;

	float minimal_move;
	bool should_move;
	switch (g_Misc->options.legit_aa_mode) {

	case 0: // balance



		if (change_side)
			side = -side;
		if (next_lby >= g_GlobalVars->curtime) {
			if (!broke_lby && *bSendPacket && g_ClientState->chokedcommands > 0)
				return;

			broke_lby = false;
			*bSendPacket = true;
			pCmd->viewangles.yaw += 120.0f * side;
		}
		else {
			broke_lby = true;
			*bSendPacket = true;
			pCmd->viewangles.yaw += 120.0f * -side;
		}
		break;
	}
	Math::FixAngles(pCmd->viewangles);
	Math::MovementFix(pCmd, OldAngles, pCmd->viewangles);



	

	if (*bSendPacket) {
		real_angle = AnimState.m_flGoalFeetYaw;
		view_angle = AnimState.m_flEyeYaw;
	}
}

void LegitAA::DrawArrows() {
	if (!g_Misc->options.legit_aa) 
		return;

	auto drawAngleLine = [&](const Vector& origin, const Vector& w2sOrigin, const float& angle, const char* text, Color clr) {
		Vector forward;
		Math::AngleVectors(QAngle(0.0f, angle, 0.0f), forward);
		float AngleLinesLength = (float)g_Misc->options.aa_arrows_length;

		Vector w2sReal;
		if (Math::WorldToScreen(origin + forward * AngleLinesLength, w2sReal) && w2sOrigin.x > 0.f && w2sOrigin.y > 0) {
			Render::Get().RenderLine(w2sOrigin.x, w2sOrigin.y, w2sReal.x, w2sReal.y, Color::White, 1.0f);
			Render::Get().RenderText(text, w2sReal.x, w2sReal.y + 10.0f, 14.0f, clr, true, true);
		}
	};

	if (g_Misc->options.aa_arrows) {
		Vector w2sOrigin;
		if (Math::WorldToScreen(g_LocalPlayer->m_vecOrigin(), w2sOrigin)) {
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, view_angle, "viewangles", Color(0.937f, 0.713f, 0.094f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, g_LocalPlayer->m_flLowerBodyYawTarget(), "lby", Color(0.0f, 0.0f, 1.0f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, real_angle, "real", Color(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}
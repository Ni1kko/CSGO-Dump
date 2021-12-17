#include "ctx.hpp"

#include "../ragebot.h"

ctx_t g_ctx;

void ctx_t::Globals::add_shot() {
	auto m_weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	if (!m_weapon) {
		return;
	}

	if ((g_ctx.get_command()->m_buttons & IN_ATTACK) && m_weapon->can_fire()) {
		if (aimbot::get().target_data.tid != -1) {
			missed_shots[aimbot::get().target_data.tid] += 1;
		}
	}
}

void ctx_t::Globals::remove_shot(IGameEvent * event) {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!g_ctx.m_local)
		return;

	if (!strcmp(event->GetName(), "player_hurt")) {
		int
			attackerid = g_csgo.m_engine()->GetPlayerForUserID(event->GetInt("attacker")),
			userid = g_csgo.m_engine()->GetPlayerForUserID(event->GetInt("userid"));

		if (!local_player)
			return;

		if (attackerid != local_player->EntIndex())
			return;

		if (event->GetInt("hitgroup") != 1)
			return;

		missed_shots[userid] = 0;
	}
}

bool ctx_t::available() {
	if (!g_csgo.m_engine()) {
		return false;
	}

	return g_csgo.m_engine()->IsInGame()
		&& g_csgo.m_engine()->IsConnected();
}

CUserCmd * ctx_t::get_command() {
	return m_pcmd;
}
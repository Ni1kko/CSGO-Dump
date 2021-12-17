#include "legitbot.h"

void legitbot::create_move() {
	if (!g_cfg.legit.aimassist || !g_csgo.m_inputsys()->IsButtonDown(g_cfg.legit.aim_assist_bind))
		return;

	if (!g_cfg.legit.fov)
		return;

	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++) {
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e->valid(true))
			continue;
	}
}
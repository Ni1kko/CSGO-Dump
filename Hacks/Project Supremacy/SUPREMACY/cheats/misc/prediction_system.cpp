#include "prediction_system.h"

float flOldCurtime;
float flOldFrametime;

void engineprediction::start()
{
	if (!g_csgo.m_movehelper())
		return;

	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!local_player)
		return;

	auto weapon = local_player->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	weapon->update_accuracy_penality();

	CMoveData movedata;

	m_flags_backup = local_player->m_fFlags();

	local_player->m_pCurrentCommand() = g_ctx.get_command();

	if (!m_random_seed || !m_prediction_player) {
		m_random_seed = *reinterpret_cast<int **>(util::pattern_scan("client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86") + 1);
		m_prediction_player = *reinterpret_cast<int **>(util::pattern_scan("client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48") + 2);
	}

	if (!m_movedata)
		m_movedata = (char *)(calloc(1, sizeof(CMoveData)));

	g_csgo.m_movehelper()->SetHost(local_player);

	*m_random_seed = MD5_PseudoRandom(g_ctx.get_command()->m_command_number) & 0x7FFFFFFF;
	*m_prediction_player = uintptr_t(local_player);

	g_csgo.m_globals()->m_curtime = local_player->m_nTickBase() *g_csgo.m_globals()->m_interval_per_tick;
	g_csgo.m_globals()->m_frametime = g_csgo.m_globals()->m_interval_per_tick;

	g_csgo.m_gamemovement()->StartTrackPredictionErrors(local_player);

	m_tickbase_backup = local_player->m_nTickBase();

	g_csgo.m_prediction()->SetupMove(local_player, g_ctx.get_command(), g_csgo.m_movehelper(), reinterpret_cast<CMoveData *>(m_movedata));
	g_csgo.m_gamemovement()->ProcessMovement(local_player, reinterpret_cast<CMoveData*>(m_movedata));
	g_csgo.m_prediction()->FinishMove(local_player, g_ctx.get_command(), reinterpret_cast<CMoveData*>(m_movedata));

	local_player->m_nTickBase() = m_tickbase_backup;
	local_player->m_pCurrentCommand() = nullptr;

	*m_random_seed = -1;
	*m_prediction_player = 0;

	g_csgo.m_movehelper()->SetHost(0);

	local_player->m_fFlags() = m_flags_backup;
}
#include "..\hooks.hpp"

bool __fastcall hooks::hooked_setupbones(void * ecx, void * edx, matrix3x4_t *bone_world_out, int max_bones, int bone_mask, float current_time) {
	if (ecx && ((IClientRenderable*)ecx)->GetIClientUnknown()) {
		IClientNetworkable* pNetworkable = ((IClientRenderable*)ecx)->GetIClientUnknown()->GetClientNetworkable();
		if (pNetworkable && pNetworkable->GetClientClass() && pNetworkable->GetClientClass()->m_ClassID == 38) {
			static auto host_timescale = g_csgo.m_cvar()->FindVar(("host_timescale"));
			auto player = (player_t*)ecx;
			float OldCurTime = g_csgo.m_globals()->m_curtime;
			float OldRealTime = g_csgo.m_globals()->m_realtime;
			float OldFrameTime = g_csgo.m_globals()->m_frametime;
			float OldAbsFrameTime = g_csgo.m_globals()->m_absoluteframetime;
			float OldAbsFrameTimeStart = g_csgo.m_globals()->m_absoluteframestarttimestddev;
			float OldInterpAmount = g_csgo.m_globals()->m_interpolation_amount;
			int OldFrameCount = g_csgo.m_globals()->m_framecount;
			int OldTickCount = g_csgo.m_globals()->m_tickcount;

			g_csgo.m_globals()->m_curtime = player->m_flSimulationTime();
			g_csgo.m_globals()->m_realtime = player->m_flSimulationTime();
			g_csgo.m_globals()->m_frametime = g_csgo.m_globals()->m_interval_per_tick * host_timescale->GetFloat();
			g_csgo.m_globals()->m_absoluteframetime = g_csgo.m_globals()->m_interval_per_tick * host_timescale->GetFloat();
			g_csgo.m_globals()->m_absoluteframestarttimestddev = player->m_flSimulationTime() - g_csgo.m_globals()->m_interval_per_tick * host_timescale->GetFloat();
			g_csgo.m_globals()->m_interpolation_amount = 0;
			g_csgo.m_globals()->m_framecount = TIME_TO_TICKS(player->m_flSimulationTime()); // Wrong if backtracking.
			g_csgo.m_globals()->m_tickcount = TIME_TO_TICKS(player->m_flSimulationTime());

			//*( int* ) ( ( int ) player + 224 ) |= 8;
			*(int*)((int)player + 236) |= 8; // IsNoInterpolationFrame
			bool ret_value = original_setupbones(player, bone_world_out, max_bones, bone_mask, g_csgo.m_globals()->m_curtime);
			*(int*)((int)player + 236) &= ~8; // (1 << 3)
													//*( int* ) ( ( int ) player + 224 ) &= ~8;

			g_csgo.m_globals()->m_curtime = OldCurTime;
			g_csgo.m_globals()->m_realtime = OldRealTime;
			g_csgo.m_globals()->m_frametime = OldFrameTime;
			g_csgo.m_globals()->m_absoluteframetime = OldAbsFrameTime;
			g_csgo.m_globals()->m_absoluteframestarttimestddev = OldAbsFrameTimeStart;
			g_csgo.m_globals()->m_interpolation_amount = OldInterpAmount;
			g_csgo.m_globals()->m_framecount = OldFrameCount;
			g_csgo.m_globals()->m_tickcount = OldTickCount;

			return ret_value;
		}
	}

	return original_setupbones(ecx, bone_world_out, max_bones, bone_mask, current_time);
}
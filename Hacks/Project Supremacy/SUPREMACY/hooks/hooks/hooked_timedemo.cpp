#include "..\hooks.hpp"

using IsPlayingTimeDemo_t = bool(__thiscall*)(void *);

bool __stdcall hooks::hooked_timedemo() {
	static auto original_fn = engine_hook->get_func_address< IsPlayingTimeDemo_t >(83);

	static auto interpolated_server_entities = reinterpret_cast<void *>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 EC 1C 8B 0D ? ? ? ? 53"));

	static ConVar* interp = g_csgo.m_cvar()->FindVar("cl_interpolate");
	if (interp->GetInt() == 0 && abs((int)((int*)_ReturnAddress()) - (int)interpolated_server_entities) < 400 && g_ctx.m_local && g_ctx.m_local->is_alive43())
		return true;

	return original_fn(g_csgo.m_engine());
}
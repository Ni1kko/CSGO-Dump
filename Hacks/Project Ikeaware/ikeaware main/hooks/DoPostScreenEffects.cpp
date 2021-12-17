#include "hooks.hpp"

int __fastcall Hooks::DoPostScreenEffects(void* _this, int edx, int a1)
{
	static auto ofunc = clientmode_hook.get_original<decltype(&DoPostScreenEffects)>(index::DoPostScreenSpaceEffects);

	if (g_LocalPlayer)
		g_Glow->Run();

	return ofunc(g_ClientMode, edx, a1);
}
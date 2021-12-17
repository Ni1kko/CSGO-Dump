#include "hooks.hpp"

bool __fastcall Hooks::ShouldDrawFog(void* _this, int)
{
	static auto ofunc = clientmode_hook.get_original<decltype(&ShouldDrawFog)>(index::ShouldDrawFog);

	return g_Options.misc.removals.no_fog ? false : ofunc(g_ClientMode, 0);
}
#include "hooks.hpp"

void __fastcall Hooks::RenderSmokeOverlay(void* _this, int, const bool unk)
{
	static auto ofunc = viewrender_hook.get_original<decltype(&RenderSmokeOverlay)>(index::RenderSmokeOverlay);

	if (!g_Options.misc.removals.no_smoke)
		ofunc(g_ViewRender, 0, unk);
}
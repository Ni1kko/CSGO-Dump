#include "hooks.hpp"

void __fastcall Hooks::OverrideView(void* _this, int edx, CViewSetup* vsView)
{
	static auto ofunc = clientmode_hook.get_original<decltype(&OverrideView)>(index::OverrideView);

	if (g_EngineClient->IsInGame() && g_LocalPlayer && vsView)
	{
		g_Misc->ThirdPerson();
		if (g_LocalPlayer->IsAlive() && !g_LocalPlayer->m_bIsScoped())
			vsView->fov = g_Options.misc.fov.override;
		else if (g_LocalPlayer->IsAlive() && g_LocalPlayer->m_bIsScoped() && g_Options.misc.removals.no_zoom)
		{
			vsView->fov = g_Options.misc.fov.override;
		}
		else if (!g_LocalPlayer->IsAlive() && g_LocalPlayer->m_hObserverTarget() && g_LocalPlayer->m_hObserverTarget()->IsAlive() && !g_LocalPlayer->m_hObserverTarget()->m_bIsScoped())
			vsView->fov = g_Options.misc.fov.override;

	}

	ofunc(g_ClientMode, edx, vsView);
}
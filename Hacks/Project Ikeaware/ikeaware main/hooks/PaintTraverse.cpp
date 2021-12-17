#include "hooks.hpp"

void __fastcall Hooks::PaintTraverse(void* _this, int, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	static auto ofunc = vguipanel_hook.get_original<decltype(&PaintTraverse)>(index::PaintTraverse);
	const auto panelName = g_VGuiPanel->GetName(panel);

	bool gameUIBeingRendered = false;

	if (g_Options.misc.removals.no_scope && !strcmp("HudZoom", panelName))
		return;

	if (!strcmp("Fullscreen Root Panel", panelName) || !strcmp("CounterStrike Root Panel", panelName))
		gameUIBeingRendered = true;

	if (gameUIBeingRendered)
		g_Misc->ToggleVGuiModulation(false);

	ofunc(g_VGuiPanel, 0, panel, forceRepaint, allowForce);

	if (gameUIBeingRendered)
		g_Misc->ToggleVGuiModulation(true);

	if (!panelId)
	{
		if (!strcmp(panelName, "FocusOverlayPanel"))
			panelId = panel;
	}
	else if (panelId == panel)
	{
		g_Render->BeginScene();
	}
}
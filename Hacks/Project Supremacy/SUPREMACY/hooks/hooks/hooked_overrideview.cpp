#include "..\hooks.hpp"
using OverrideView_t = void(__stdcall*)(CViewSetup*);
void __stdcall hooks::hooked_overrideview(CViewSetup* viewsetup) {
	static auto original_fn = clientmode_hook->get_func_address<OverrideView_t >(18);
	if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		return original_fn(viewsetup);;
	if (g_ctx.available()) {
		auto scoped = g_ctx.m_local->m_bIsScoped();
		auto zoomsensration = g_csgo.m_cvar()->FindVar("zoom_sensitivity_ratio_mouse");
		if (g_cfg.esp.removescopesensitivity)
			zoomsensration->SetValue("0");
		else
			zoomsensration->SetValue("1");
		if (!scoped)
			viewsetup->fov = g_cfg.esp.fov;
		else if (scoped && g_cfg.esp.removezoom)
			viewsetup->fov = g_cfg.esp.fov;
		if (!scoped || (scoped && g_cfg.esp.override_fov_scoped)) {
			viewsetup->fov += g_cfg.esp.fov;
		}
	}
	return original_fn(viewsetup);
}
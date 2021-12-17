#include "..\hooks.hpp"
#include "../../cheats/visuals/other_esp.h"
#include "..\..\NewMenu.h"
using DoPostScreenEffects_t = void(__thiscall*)(void * thisptr, CViewSetup * setup);

#define COLOUR( r, g, b ) D3DCOLOR_ARGB( 255, r, g, b )

HRESULT __stdcall hooks::hooked_present(IDirect3DDevice9* m_device, const RECT* source_rect,
	const RECT* dest_rect,
	HWND dest_window_override, const RGNDATA* dirty_region)
{
	static auto original_fn = directx_hook->get_func_address<PresentFn>(17);
	IDirect3DStateBlock9* state_block = NULL;
	if (m_device->CreateStateBlock(D3DSBT_PIXELSTATE, &state_block) < 0)
		[m_device]()
	{
		if (!render::get().initalized)
		{
			render::get().initalize_objects(m_device);
			render::get().initalized = true;
		}
		else
		{
			render::get().setup_states();
			rect_t m_pos = rect_t(
				menu::get().area.x - 74,
				menu::get().area.y,
				menu::get().area.w + 74, 1
			);
			//if (!menu::get().active())
			otheresp::get().spread_crosshair(m_device);
		}
	}();
	state_block->Apply();
	state_block->Release();
	return original_fn(m_device, source_rect, dest_rect, dest_window_override, dirty_region);
}

HRESULT __stdcall hooks::hooked_reset(IDirect3DDevice9* m_device, D3DPRESENT_PARAMETERS* presentation_parameters)
{
	static auto original_fn = directx_hook->get_func_address<ResetFn>(16);
	NewMenu::get().OnDeviceLost();
	auto hr = original_fn(m_device, presentation_parameters);
	if (hr >= 0)
	{
		NewMenu::get().OnDeviceReset();
	}
	if (render::get().initalized)
	{
		render::get().invalidate_objects();
		HRESULT hr = original_fn(m_device, presentation_parameters);
		render::get().restore_objects(m_device);
		return hr;
	}
	return original_fn(m_device, presentation_parameters);
}

void __fastcall hooks::hooked_postscreeneffects(void * thisptr, void * edx, CViewSetup * setup) {
	static auto run_glow = []() -> void {
		if (!g_ctx.available() || !g_ctx.m_local) {
			return;
		}
		for (int i = 0; i < g_csgo.m_glow()->m_GlowObjectDefinitions.Count(); i++) {
			if (g_csgo.m_glow()->m_GlowObjectDefinitions[i].IsUnused() || !g_csgo.m_glow()->m_GlowObjectDefinitions[i].GetEnt()) {
				continue;
			}
			auto * object = &g_csgo.m_glow()->m_GlowObjectDefinitions[i];
			auto entity = reinterpret_cast<entity_t*>(object->GetEnt());
			float color[3];
			if (!entity || entity->IsDormant()) {
				continue;
			}
			if (entity == g_ctx.m_local) {
				if (!g_cfg.player.glow[GLOW_LOCAL].enabled)
					continue;
				color[0] = g_cfg.player.glow_color_local[0] / 255.0f;
				color[1] = g_cfg.player.glow_color_local[1] / 255.0f;
				color[2] = g_cfg.player.glow_color_local[2] / 255.0f;
			}
			if (entity->m_iTeamNum() != g_ctx.m_local->m_iTeamNum() && entity != g_ctx.m_local) {
				if (!g_cfg.player.glow[GLOW_ENEMY].enabled)
					continue;
				color[0] = g_cfg.player.glow_color_enemy[0] / 255.0f;
				color[1] = g_cfg.player.glow_color_enemy[1] / 255.0f;
				color[2] = g_cfg.player.glow_color_enemy[2] / 255.0f;
			}
			if (entity->m_iTeamNum() == g_ctx.m_local->m_iTeamNum() && entity != g_ctx.m_local) {
				if (!g_cfg.player.glow[GLOW_TEAMMATE].enabled)
					continue;
				color[0] = g_cfg.player.glow_color_teammate[0] / 255.0f;
				color[1] = g_cfg.player.glow_color_teammate[1] / 255.0f;
				color[2] = g_cfg.player.glow_color_teammate[2] / 255.0f;
			}
			if (entity->GetClientClass()->m_ClassID == 38) {
				object->Set(color[0], color[1], color[2],
					g_cfg.player.glowopacity / 100.0f,
					g_cfg.player.glowbloom / 100.0f,
					g_cfg.player.glow_type);
			}
		}
	};
	static auto original_fn = clientmode_hook->get_func_address<DoPostScreenEffects_t>(44);
	if (g_cfg.player.glow[GLOW_TEAMMATE].enabled || g_cfg.player.glow[GLOW_ENEMY].enabled || g_cfg.player.glow[GLOW_LOCAL].enabled)
		run_glow();
	return original_fn(thisptr, setup);
}
#include "..\hooks.hpp"
#include "..\..\cheats\visuals\player_esp.h"
#include "..\..\cheats\visuals\other_esp.h"
#include "..\..\cheats\misc\logs.h"
#include "..\..\cheats\legit\legit_backtrack.h"
#include "..\..\cheats\visuals\world_esp.h"
#include "..\..\NewMenu.h"
using PaintTraverse_t = void(__thiscall*)(void*, vgui::VPANEL, bool, bool);
template<class T>
static T* Find_Hud_Element(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(util::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(util::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
bool reload_fonts()
{
	static int old_width, old_height;
	int width, height;
	g_csgo.m_engine()->GetScreenSize(width, height);
	if (width != old_width || height != old_height)
	{
		old_width = width;
		old_height = height;
		return true;
	}
	return false;
}
void __fastcall hooks::hooked_painttraverse(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force) {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	static auto draw_animation = []() -> void {
		if (!g_ctx.m_local->is_alive38()) return;
		auto screen = render::get().viewport();
		for (int i = 0; i < 13; i++) {
			AnimationLayer layer = g_ctx.m_local->get_animlayers()[i];
			render::get().text(fonts[VERDANA12BOLDOUT], screen.right / 2 + 10, screen.bottom / 2 + 10 + (i * 13), Color::White, HFONT_CENTERED_NONE, "layer: %i, activity: %i, weight: %0.2f, cycle: %0.2f", i, g_ctx.m_local->sequence_activity(layer.m_nSequence), layer.m_flWeight, layer.m_flCycle);
		}
	};
	static uint32_t HudZoomPanel;
	if (!HudZoomPanel)
		if (!strcmp("HudZoom", g_csgo.m_panel()->GetName(panel))) HudZoomPanel = panel;
	if (g_cfg.esp.removescope && HudZoomPanel == panel) return;
	static auto panel_id = vgui::VPANEL{ 0 };
	static auto original_fn = panel_hook->get_func_address< PaintTraverse_t >(41);
	original_fn(ecx, panel, force_repaint, allow_force);
	if (!panel_id) {
		const auto panelName = g_csgo.m_panel()->GetName(panel);
		if (strstr(panelName, "MatSystemTopPanel")) panel_id = panel;
	}
	if (reload_fonts()) render::get().setup();
	if (g_cfg.esp.bright)
	{
		static DWORD* deathNotice = deathNotice = Find_Hud_Element<DWORD>("CCSGO_HudDeathNotice");
		static void(__thiscall *ClearDeathNotices)(DWORD) = (void(__thiscall*)(DWORD))util::FindSignature("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");
		if (local_player)
		{
			if (!deathNotice) deathNotice = Find_Hud_Element<DWORD>("CCSGO_HudDeathNotice");
			if (deathNotice)
			{
				float* localDeathNotice = (float*)((DWORD)deathNotice + 0x50);
				if (localDeathNotice) *localDeathNotice = 60 ? FLT_MAX : 1.5f;
				if (g_ctx.m_globals.NewRound)
				{
					if (!deathNotice) deathNotice = Find_Hud_Element<DWORD>("CCSGO_HudDeathNotice");
					if (deathNotice - 20 && localDeathNotice)
					{
						ClearDeathNotices(((DWORD)deathNotice - 20));
					}
					g_ctx.m_globals.NewRound = false;
				}
			}
		}
	}

	if (panel_id == panel) {
		if (g_ctx.available() && local_player) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					g_ctx.m_globals.w2s_matrix[i][j] = g_csgo.m_engine()->WorldToScreenMatrix()[i][j];
				}
			}
			if (g_cfg.player.enable) playeresp::get().paint_traverse();
			if (g_cfg.esp.indicators) otheresp::get().lby_indicator();
			otheresp::get().spread_crosshair();
			if (g_cfg.legit.backtracking_visualize) legitbacktrack::get().paint_traverse();
			worldesp::get().paint_traverse();
			if (g_cfg.esp.removescope && local_player->m_bIsScoped()) {
				auto resolution = render::get().viewport();
				render::get().line(0, resolution.bottom / 2, resolution.right, resolution.bottom / 2, g_cfg.menu.removescopecol[0]);
				render::get().line(resolution.right / 2, 0, resolution.right / 2, resolution.bottom, g_cfg.menu.removescopecol[0]);
			}
			//if (g_cfg.esp.hitmarker[HITMARKER_STATIC].enabled) otheresp::get().hitmarker_paint();
			//if (g_cfg.esp.hitmarker[HITMARKER_DYNAMIC].enabled) otheresp::get().hitmarkerdynamic_paint();
			otheresp::get().antiaim_indicator();
			otheresp::get().penetration_reticle();
			otheresp::get().watermark();
			//otheresp::get().onetap_indigos();
			//otheresp::get().bomb_timer();
			playeresp::get().pov_arrow();
		}
		cfg_manager->config_files();
		eventlogs::get().paint_traverse();
		menu::get().update();
	}
}
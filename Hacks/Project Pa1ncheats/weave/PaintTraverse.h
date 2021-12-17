#include "Features.h"

int ScreenSize2W, ScreenSize2H;

//interfaces.v_panel->SetMouseInputEnabled(vguiPanel, csgo->DoUnload ? 0 : vars.menu.open);

void __stdcall Hooked_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static auto PaintTraverse = g_pPanelHook->GetOriginal< PaintTraverseFn >(41);

	if (csgo->DoUnload) {
		interfaces.v_panel->SetMouseInputEnabled(vguiPanel, 0);
		return PaintTraverse(interfaces.v_panel, vguiPanel, forceRepaint, allowForce);
	}
	std::string panel_name = interfaces.v_panel->GetName(vguiPanel);
	if (panel_name == "HudZoom" && vars.visuals.remove & 8)
		return;

	PaintTraverse(interfaces.v_panel, vguiPanel, forceRepaint, allowForce);

	if (panel_name == "FocusOverlayPanel") {

		static bool bResChange = false;

		interfaces.engine->GetScreenSize(ScreenSize2W, ScreenSize2H);

		if (!bResChange && (ScreenSize2W != csgo->w
			|| ScreenSize2H != csgo->h))
		{
			csgo->w = ScreenSize2W;
			csgo->h = ScreenSize2H;
			bResChange = true;
		}

		if (bResChange)
			Drawing::CreateFonts();

		if (interfaces.engine->IsInGame() && interfaces.engine->IsConnected() && csgo->local)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					csgo->viewMatrix[i][j] = interfaces.engine->WorldToScreenMatrix()[i][j];

			features->Eventlog->Draw();

			if (csgo->local->isAlive()) {
				CGrenadePrediction::Get().Paint();
			}

			features->Visuals->Draw();
		}

		if (bResChange)
			bResChange = false;

		interfaces.v_panel->SetMouseInputEnabled(vguiPanel, vars.menu.open);

		if (interfaces.engine->IsInGame() && interfaces.engine->IsConnected() && csgo->local) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					csgo->viewMatrix[i][j] = interfaces.engine->WorldToScreenMatrix()[i][j];
		}
	}

	if (interfaces.engine->IsInGame() && interfaces.engine->IsConnected() && csgo->local)
	{
		static auto cl_foot_contact_shadows = interfaces.cvars->FindVar(hs::cl_foot_contact_shadows::s().c_str());
		static auto viewmodel_offset_x = interfaces.cvars->FindVar(hs::viewmodel_offset_x::s().c_str());
		static auto viewmodel_offset_y = interfaces.cvars->FindVar(hs::viewmodel_offset_y::s().c_str());
		static auto viewmodel_offset_z = interfaces.cvars->FindVar(hs::viewmodel_offset_z::s().c_str());
		static auto mat_postprocess_enable = interfaces.cvars->FindVar(hs::mat_postprocess_enable::s().c_str());
		static auto weapon_debug_spread_show = interfaces.cvars->FindVar(hs::weapon_debug_spread_show::s().c_str());

		static float old_value_x = viewmodel_offset_x->GetFloat();
		static float old_value_y = viewmodel_offset_y->GetFloat();
		static float old_value_z = viewmodel_offset_z->GetFloat();

		cl_foot_contact_shadows->m_fnChangeCallbacks.m_Size = 0;
		viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;
		viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;
		viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;
		mat_postprocess_enable->m_fnChangeCallbacks.m_Size = 0;
		weapon_debug_spread_show->m_fnChangeCallbacks.m_Size = 0;

		cl_foot_contact_shadows->SetValue(0);
		mat_postprocess_enable->SetValue(vars.visuals.remove & 32 ? 0 : 1);
		viewmodel_offset_x->SetValue(vars.misc.viewmodel_x);
		viewmodel_offset_y->SetValue(vars.misc.viewmodel_y);
		viewmodel_offset_z->SetValue(vars.misc.viewmodel_z);

		if (csgo->local->isAlive())
			weapon_debug_spread_show->SetValue(vars.visuals.force_crosshair && !csgo->local->IsScoped() ? 3 : 0);
		
	}
}
float __fastcall Hooked_GetScreenAspectRatio(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight)
{
	auto GetScreenAspectRatio = g_pEngineHook->GetOriginal< AspectRatioFn >(101);

	if (vars.visuals.aspect_ratio == 0.f)
		return GetScreenAspectRatio(pEcx, iWidth, iHeight);
	else
		return vars.visuals.aspect_ratio / 100.f;
}

_declspec(noinline) const char* GetForeignFallbackFontName_Detour(uintptr_t* ecx)
{
	if (strlen(Drawing::LastFontName) > 1)
		return Drawing::LastFontName;
	else
		return H::GetForeignFallbackFontName(ecx);
}

const char* __fastcall Hooked_GetForeignFallbackFontName(uintptr_t* ecx, uint32_t)
{
	return GetForeignFallbackFontName_Detour(ecx);
}
#include "hooks.hpp"

DWORD dwOld_D3DRS_COLORWRITEENABLE;
IDirect3DVertexDeclaration9* vertDec;
IDirect3DVertexShader9* vertShader;

void SaveState(IDirect3DDevice9* pDevice)
{
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->GetVertexDeclaration(&vertDec);
	pDevice->GetVertexShader(&vertShader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
}

void RestoreState(IDirect3DDevice9* pDevice) // not restoring everything. Because its not needed.
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	pDevice->SetVertexDeclaration(vertDec);
	pDevice->SetVertexShader(vertShader);
}

long __stdcall Hooks::EndScene(IDirect3DDevice9* pDevice)
{
	static auto ofunc = direct3d_hook.get_original<decltype(&EndScene)>(index::EndScene);

	static auto viewmodel_fov = g_CVar->FindVar("viewmodel_fov");

	viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_fov->SetValue(g_Options.misc.fov.viewmodel);

	SaveState(pDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImDrawList* esp_drawlist = nullptr;

	esp_drawlist = g_Render->RenderScene();

	g_Misc->SpectatorList();
	g_Menu->Render();

	ImGui::Render(esp_drawlist);

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	RestoreState(pDevice);

	return ofunc(pDevice);
}

long __stdcall Hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static auto ofunc = direct3d_hook.get_original<decltype(&Reset)>(index::Reset);

	g_Menu->OnDeviceLost();

	auto hr = ofunc(device, pPresentationParameters);

	if (hr >= 0)
		g_Menu->OnDeviceReset();

	return hr;
}
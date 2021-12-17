#include "Hooks.h"
#include "Menu.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_freetype.h"
#include <d3dx9tex.h>
#include "AntiAims.h"

#include <chrono>

void GUI_Init(IDirect3DDevice9* pDevice)
{
	if (!vars.menu.guiinited)
	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(csgo->Init.Window);
		ImGui_ImplDX9_Init(pDevice);

		ImGuiIO& io = ImGui::GetIO();

		ImFontConfig cfg;
		fonts::menu_main = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/tahomabd.ttf", 12, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		fonts::menu_desc = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/tahoma.ttf", 13, &cfg, io.Fonts->GetGlyphRangesCyrillic());

		fonts::lby_indicator = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/verdanab.ttf", 25, &cfg, io.Fonts->GetGlyphRangesCyrillic());

		ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00
		/*ImGuiFreeType::RasterizerFlags::MonoHinting | ImGuiFreeType::RasterizerFlags::Monochrome*/);

		

		vars.menu.guiinited = true;
	}
}

LPDIRECT3DTEXTURE9 img = nullptr;
LPD3DXSPRITE sprite = nullptr;

//HRESULT __stdcall Hooked_Present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect,
//	HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
//{
//	//static auto Present = g_pDirectXHook->GetOriginal< PresentFn >(17);
//
//	//if (csgo->DoUnload)
//	//	return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
//
//	//IDirect3DStateBlock9* stateBlock = nullptr;
//	//IDirect3DVertexDeclaration9* vertDec = nullptr;
//
//	//pDevice->GetVertexDeclaration(&vertDec);
//	//if (pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &stateBlock))
//	//	return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
//
//	//GUI_Init(pDevice);
//	//csgo->render_device = pDevice;
//	//g_Render->SetupPresent(pDevice);
//	//static void* dwReturnAddress = _ReturnAddress();
//
//	//if (dwReturnAddress == _ReturnAddress())
//	//{
//	//	g_Render->PreRender(pDevice);
//
//	//	//features->Eventlog->Draw();
//
//	//	//features->Visuals->Draw();
//
//	//	features->Visuals->DrawLocalShit(pDevice);
//
//	//	g_Hitmarker->Paint();
//
//	//	g_Render->PostRender(pDevice);
//
//	////	ImGui::GetIO().MouseDrawCursor = vars.menu.open;
//
//	//	g_Menu->render();
//
//	//	//if (vars.visuals.speclist && interfaces.engine->IsInGame() && interfaces.engine->IsConnected())
//	//	//{
//	//	//	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
//	//	//	ImGui::Begin("###sssspectators", &vars.visuals.speclist, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
//	//	//	{
//	//	//		//ImGui::PushStylecolor_t(ImGuiCol_Text, ImVec4(vars.misc.col[0], vars.misc.col[1], vars.misc.col[2], 1.f));
//	//	//		ImGui::Text("spectators: ");
//	//	//		//ImGui::PopStylecolor_t();
//	//	//		ImGui::Separator();
//	//	//		for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
//	//	//		{
//	//	//			auto pEntity = interfaces.ent_list->GetClientEntity(i);
//	//	//			player_info_t pinfo;
//
//	//	//			if (pEntity &&  pEntity != csgo->local)
//	//	//			{
//	//	//				if (interfaces.engine->GetPlayerInfo(i, &pinfo) && !pEntity->isAlive() && !pEntity->IsDormant())
//	//	//				{
//	//	//					HANDLE obs = pEntity->GetObserverTargetHandle();
//
//	//	//					if (obs)
//	//	//					{
//	//	//						auto pTarget = interfaces.ent_list->GetClientEntityFromHandle((uint32_t)obs);
//	//	//						player_info_t pinfo2;
//	//	//						if (pTarget)
//	//	//						{
//	//	//							if (interfaces.engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
//	//	//							{
//	//	//								ImGui::Textcolor_ted(pTarget->GetIndex() == csgo->local->GetIndex() ? ImVec4(1, 0, 0, 1) : ImVec4(1, 1, 1, 1), "%s -> %s", pinfo.name, pinfo2.name);
//	//	//							}
//	//	//						}
//	//	//					}
//	//	//				}
//	//	//			}
//	//	//		}
//
//	//	//	}ImGui::End(false);
//	//	//}
//	//	//if (vars.visuals.monitor && interfaces.engine->IsInGame() && interfaces.engine->IsConnected())
//	//	//{
//	//	//	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
//	//	//	ImGui::Begin("###monitor", &vars.visuals.monitor, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
//	//	//	{
//	//	//		ImGui::Text("choked packets:\t%i", csgo->chockedticks);
//	//	//		if (csgo->local && csgo->local->isAlive())
//	//	//			ImGui::Text("desync:\t%.1f", csgo->delta);
//	//	//		if (csgo->not_damagedshots == 0)
//	//	//			csgo->not_damagedshots = 1;
//	//	//		ImGui::Text("total accuracy:\t%.1f", (float)((float)csgo->damagedshots / (float)csgo->not_damagedshots) * (float)100.f);
//	//	//		ImGui::Text("ping:\t\t%.1f", csgo->ping);
//	//	//	}
//	//	//	ImGui::End(false);
//	//	//}
//
//	//	
//
//	//	g_Render->EndPresent(pDevice);
//	//}
//	//stateBlock->Apply();
//	//stateBlock->Release();
//	//pDevice->SetVertexDeclaration(vertDec);
//
//	return false;
//}
//
//long __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
//{
//	//static auto Reset = g_pDirectXHook->GetOriginal< ResetFn >(16);
//
//	//if (csgo->DoUnload)
//	//	return Reset(pDevice, pPresentationParameters);
//
//	//g_Render->InvalidateObjects();
//	////ImGui_ImplDX9_InvalidateDeviceObjects();
//
//	//auto hr = Reset(pDevice, pPresentationParameters);
//	//if (hr >= 0) {
//
//	//	g_Render->CreateObjects(pDevice);
//	//	//	ImGui_ImplDX9_CreateDeviceObjects();
//
//	//}
//	return false;//return present
//}

string UnixTimeToExpiryDate(long long unixTime) {
	string s, m, h;
	if (unixTime < 0) {
		csgo->DoUnload = true;
		return string();
	}

	s = std::to_string(unixTime % 60);
	unixTime /= 60;
	m = std::to_string(unixTime % 60);
	unixTime /= 60;
	h = std::to_string(unixTime % 24);
	unixTime /= 24;

	if (atoi(h.c_str()) > 1000)
		return "Never :)";

	return std::to_string(unixTime)
		+ "d:" + ("0" + h).substr(h.length() - 1)
		+ "h:" + ("0" + m).substr(m.length() - 1)
		+ "m:" + ("0" + s).substr(s.length() - 1)
		+ "s";
}
static HRESULT __stdcall Hooked_Present(IDirect3DDevice9Ex* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
{
	//H::SteamPresent = g_pD3DX9->GetOriginal<Present_t>(17);
	IDirect3DStateBlock9* stateBlock = nullptr;
	IDirect3DVertexDeclaration9* vertDec = nullptr;

	device->GetVertexDeclaration(&vertDec);

	if (device->CreateStateBlock(D3DSBT_PIXELSTATE, &stateBlock))
		return H::SteamPresent(device, src, dest, windowOverride, dirtyRegion);

	GUI_Init(device);
	csgo->render_device = device;
	g_Render->SetupPresent(device);
	static void* dwReturnAddress = _ReturnAddress();

	if (dwReturnAddress == _ReturnAddress())
	{
		g_Render->PreRender(device);

		features->Visuals->DrawLocalShit(device);

		g_Hitmarker->Paint();

		g_Render->PostRender(device);

		g_Menu->DrawWatermark();

		g_Menu->render();

		g_Render->EndPresent(device);
	}

	stateBlock->Apply();
	stateBlock->Release();
	device->SetVertexDeclaration(vertDec);

	return H::SteamPresent(device, src, dest, windowOverride, dirtyRegion);//return present here
}



static HRESULT D3DAPI Hooked_Reset(IDirect3DDevice9Ex* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	//H::SteamReset = g_pD3DX9->GetOriginal<Reset_t>(16);

	if (csgo->DoUnload)
		return H::SteamReset(device, params);

	g_Render->InvalidateObjects();

	auto hr = H::SteamReset(device, params);

	if (hr >= 0)
		g_Render->CreateObjects(device);

	return hr;
}
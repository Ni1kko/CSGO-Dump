#pragma once
#include "HookIncludes.h"
#include "Renderer.h"
#include "Interface.h"
#include <d3d9.h>
uint8_t* m_present;
uint8_t* m_reset;

IDirect3DDevice9* d3d9_device;
typedef HRESULT(__stdcall *EndScene_t) (IDirect3DDevice9*);

typedef HRESULT(__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

EndScene_t	oEndScene = nullptr;
Reset_t		oReset = nullptr;

HRESULT __stdcall hkReset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {

	if (!renderer->IsReady())
		return oReset(thisptr, params);


	ImGui_ImplDX9_InvalidateDeviceObjects();
	renderer->~Renderer();

	HRESULT result = oReset(thisptr, params);

	ImGui_ImplDX9_CreateDeviceObjects();

	renderer->Initialize(FindWindowA("Valve001", NULL), thisptr);
    g_Render->SetupFonts();
	return result;
}

HRESULT __stdcall hkEndScene(IDirect3DDevice9* thisptr) {



	static bool mouse_enabled = false;
	thisptr->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	bool is_renderer_active = renderer->IsActive();

	if (is_renderer_active) {
		if (mouse_enabled) {
			g_Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
			mouse_enabled = false;
		}
	}
	else {
		if (!mouse_enabled) {
			g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
			mouse_enabled = true;
		}
	}

	ImGui::GetIO().MouseDrawCursor = is_renderer_active;

	if (!is_renderer_active)
		return oEndScene(thisptr);

	ImGui_ImplDX9_NewFrame();

	RenderInterface();

	ImGui::Render();

	return oEndScene(thisptr);
}

typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
Present_T oPresent;
HRESULT _stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
    if (!renderer->IsReady())
    {
        renderer->Initialize(FindWindowA("Valve001", NULL), pDevice);
    }
    static bool mouse_enabled = false;
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    bool is_renderer_active = renderer->IsActive();

    if (is_renderer_active) {
        if (mouse_enabled) {
            g_Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
            mouse_enabled = false;
        }
    }
    else {
        if (!mouse_enabled) {
            g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
            mouse_enabled = true;
        }
    }

    ImGui::GetIO().MouseDrawCursor = is_renderer_active;

    if (!is_renderer_active)
        return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

    ImGui_ImplDX9_NewFrame();

    RenderInterface();

    ImGui::Render();

    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

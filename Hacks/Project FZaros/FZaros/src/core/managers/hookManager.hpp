#pragma once

#include "../../sdk/userCmd.hpp"
#include "../../utilities/vftHook.hpp"
#include "../../sdk/baseClient.hpp"

#include "../../utilities/includes.h"

class CHookManager
{
private:
	static void __fastcall onFrameStage(void *, void *, clientFrameStage_t);
	static bool __fastcall onMove(void *, void *, float, CUserCmd *);
	static void __fastcall onPaint(void *, void *, unsigned int, bool, bool);
	static long __stdcall  onPaintDX9(LPDIRECT3DDEVICE9, LPRECT, LPRECT, HWND, LPRGNDATA);
	static void __fastcall onResize(void *, void *, int, int);
	static long __stdcall  onResizeDX9(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *);
	static void __stdcall  lockCursor();
	static void __fastcall overrideView(void*, void*, CViewSetup*);
	static void __fastcall sceneEnd(void*, void* edx);
	static bool __fastcall fireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent);
	static void __fastcall beginFrame(void*, void*, float ft);
	static void __stdcall  drawModelExecute( void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* matrix );
	static float __fastcall aspectRatio( void* ecx, void* edx, int32_t width, int32_t height );

	long __stdcall present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
	long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	using present_fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	using reset_fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	present_fn original_present;
	reset_fn original_reset;

	CVFTHook *dx9DeviceHook;
	CVFTHook *baseClientHook;
	CVFTHook *clientModeHook;
	CVFTHook *surfaceHook;
	CVFTHook *panelHook;
	CVFTHook *renderViewHook;
	CVFTHook *gameEventHook;
	CVFTHook *materialHook;
	CVFTHook *modelRenderHook;
	CVFTHook *gameEngineHook;

public:
	void initialize();
	void shutdown();
};

extern CHookManager *g_Hooks;
#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <map>
#include <ostream>
#include <Shlobj.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <cmath>
#include <float.h>
#include <codecvt>
using namespace std;
#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>
#include <algorithm>
#include <iterator>

namespace SDK
{
	class IMatRenderContext;
	class Ray_t;
	class ITraceFilter;
	class NetChannel;

	struct CViewSetup;
	struct DrawModelState_t;
	struct ModelRenderInfo_t;
	struct trace_t;
}

enum EGCResultxd
{
	k_EGCResultOKxd = 0,
	k_EGCResultNoMessagexd = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmallxd = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOnxd = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessagexd = 4,      // Something was wrong with the message being sent with SendMessage
};

class Protobuffs;
extern Protobuffs write;

class CHackManager
{
private:

public:
	SDK::CUserCmd* CurrentCmd;



};
namespace vtable_indexes
{

	constexpr auto send_message = 0;
	constexpr auto retrieve_message = 2;

}
namespace HOOKS
{
	typedef bool(__stdcall* CreateMoveFn)(float , SDK::CUserCmd*);
	typedef void(__thiscall* FrameStageNotifyFn)(void*, int);
	typedef void(__thiscall* PaintFn)(void*, void*, int);
	typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
	typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* SceneEndFn)(void*);
	typedef void(__thiscall* TraceRayFn)(void *thisptr, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter* pTraceFilter, SDK::trace_t *pTrace);
	typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);
	typedef void(__thiscall* RenderViewFn)(void*, SDK::CViewSetup&, SDK::CViewSetup&, unsigned int nClearFlags, int whatToDraw);
	typedef int(__thiscall* SendDatagramFn)(void*, void*);
	typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
	typedef float(__thiscall* GetViewmodelFOVFn)();
	typedef long(__stdcall* PresentFn) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
	typedef long(__stdcall *EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	extern CreateMoveFn original_create_move;
	extern PaintFn original_paint;
	extern PaintTraverseFn original_paint_traverse;
	extern FrameStageNotifyFn original_frame_stage_notify;
	extern DrawModelExecuteFn original_draw_model_execute;
	extern SceneEndFn original_scene_end;
	extern TraceRayFn original_trace_ray;
	extern SendDatagramFn original_send_datagram;
	extern OverrideViewFn original_override_view;
	extern RenderViewFn original_render_view;
	extern SvCheatsGetBoolFn original_get_bool;
	extern GetViewmodelFOVFn original_viewmodel_fov;

	extern VMT::VMTHookManager iclient_hook_manager;
	extern VMT::VMTHookManager panel_hook_manager;
	extern VMT::VMTHookManager paint_hook_manager;
	extern VMT::VMTHookManager model_render_hook_manager;
	extern VMT::VMTHookManager scene_end_hook_manager;
	extern VMT::VMTHookManager render_view_hook_manager;
	extern VMT::VMTHookManager trace_hook_manager;
	extern VMT::VMTHookManager net_channel_hook_manager;
	extern VMT::VMTHookManager override_view_hook_manager;
	extern VMT::VMTHookManager input_table_manager;
	extern VMT::VMTHookManager get_bool_manager;
	extern VMT::VMTHookManager direct;

	void InitHooks();

	// virtual function hooks
	bool __stdcall   HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd);
	void __fastcall  HookedFrameStageNotify(void*, void*, int);
	void __stdcall   HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);
	void __fastcall  HookedDrawModelExecute(void*, void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	void __fastcall  HookedSceneEnd(void*, void*);
	void __fastcall  HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace);
	int __fastcall   HookedSendDatagram(SDK::NetChannel* ecx, void* edx, void* data);
	void __fastcall  HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup);
	bool __fastcall  HookedGetBool(void* pConVar, void* edx);
	float __fastcall GetViewmodelFOV();
	using GCRetrieveMessage = EGCResultxd(__thiscall*)(void*, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize);
	using GCSendMessage = EGCResultxd(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);

	// netvar hooks
	void InitNetvarHooks();
	void UnHook();

}
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
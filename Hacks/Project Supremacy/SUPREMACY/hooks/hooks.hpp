#pragma once

#include "..\includes.hpp"
#include "..\sdk\interfaces\IBaseClientDll.hpp"
#include "..\sdk\interfaces\IClientMode.hpp"
#include "..\sdk\misc\CUserCmd.hpp"
#include "..\sdk\interfaces\IInputSystem.hpp"
#include "..\sdk\interfaces\IMDLCache.hpp"
#include "..\sdk\interfaces\IGameEventManager.hpp"

#include "vfunc_hook.hpp"

class C_HookedEvents : public IGameEventListener2 {
public:
	void FireGameEvent(IGameEvent * event);
	void RegisterSelf();
	void RemoveSelf();
	int GetEventDebugID(void);
};

namespace hooks {
	extern HWND window;
	extern WNDPROC old_window_proc;
	typedef long(__stdcall*  ResetFn)      (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef long(__stdcall*  PresentFn)    (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);

	void load();
	void shutdown();

	extern PresentFn original_present;
	extern ResetFn original_reset;

	extern vmthook * directx_hook;
	extern vmthook * client_hook;
	extern vmthook * engine_hook;
	extern vmthook * clientmode_hook;
	extern vmthook * inputinternal_hook;
	extern vmthook * renderview_hook;
	extern vmthook * modelcache_hook;
	extern vmthook * panel_hook;
	extern vmthook * materialsys_hook;
	extern vmthook * modelrender_hook;
	extern vmthook * netchannel_hook;
	extern vmthook * surface_hook;
	extern C_HookedEvents hooked_events;

	using SetupBonesFn = bool(__thiscall*)(void*, matrix3x4_t*, int, int, float);
	extern SetupBonesFn original_setupbones;

	LRESULT __stdcall hooked_wndproc(HWND, UINT, WPARAM, LPARAM);
	void __stdcall hooked_fsn(ClientFrameStage_t);
	bool __stdcall hooked_createmove(float, CUserCmd *);
	void __stdcall hooked_overrideview(CViewSetup * setup);
	bool __stdcall hooked_timedemo();
	void __stdcall hooked_dme(IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & info, matrix3x4_t * bone_to_world);
	HRESULT __stdcall hooked_present(IDirect3DDevice9 * m_device, const RECT * source_rect, const RECT * dest_rect, HWND dest_window_override, const RGNDATA * dirty_region);
	HRESULT __stdcall hooked_reset(IDirect3DDevice9 * m_device, D3DPRESENT_PARAMETERS * presentation_parameters);
	void  __fastcall hooked_postscreeneffects(void * thisptr, void * edx, CViewSetup * setup);
	void __fastcall hooked_setkeycodestate(void* thisptr, void* edx, ButtonCode_t code, bool pressed);
	void __fastcall hooked_setmousecodestate(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state);
	void __fastcall hooked_sceneend(void* thisptr, void* edx);
	void __fastcall hooked_findmdl(void* ecx, void* edx, char* FilePath);
	void __fastcall hooked_painttraverse(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force);
	void __fastcall hooked_beginframe(void * ecx, void * edx, float ft);
	bool __fastcall hooked_setupbones(void * ecx, void * edx, matrix3x4_t *bone_world_out, int max_bones, int bone_mask, float current_time);
	int __fastcall hooked_senddatagram(void* netchan, void*, void *datagram);
	void __stdcall hooked_lockcursor();
}
#pragma once
#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>
namespace Global //global variables
{
	extern float hitmarkerAlpha;

	extern float smt;
	extern QAngle FakeAngle;
	extern QAngle RealAngle;
	extern QAngle visualAngles;
	extern bool bSendPacket;
	extern bool bShouldChoke;
	extern bool bAimbotting;
	extern bool bVisualAimbotting;
	extern QAngle vecVisualAimbotAngs;
	extern CUserCmd *userCMD;
	extern int nChockedTicks;
	extern Vector vecUnpredictedVel;
	extern bool bFakelag;
	extern float flFakewalked;
}
namespace index
{
    constexpr auto EndScene                 = 42;
    constexpr auto Reset                    = 16;
    constexpr auto PaintTraverse            = 41;
    constexpr auto CreateMove               = 22;//hlclient index 22 // clientmode 24
    constexpr auto PlaySound                = 82;
    constexpr auto FrameStageNotify         = 37;
    constexpr auto DrawModelExecute         = 21;
    constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool          = 13;
	constexpr auto OverrideView             = 18;
	constexpr auto SceneEnd				    = 9;
	constexpr auto FireEvent = 9;
}
namespace Hooks
{
    void Initialize();
    void Shutdown();
    extern vfunc_hook hlclient_hook;
    extern vfunc_hook direct3d_hook;
    extern vfunc_hook vguipanel_hook;
    extern vfunc_hook vguisurf_hook;
    extern vfunc_hook mdlrender_hook;
    extern vfunc_hook viewrender_hook;
    using EndScene            = long(__stdcall *)(IDirect3DDevice9*);
    using Reset               = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
    using PaintTraverse       = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
    using FrameStageNotify    = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
    using PlaySound           = void(__thiscall*)(ISurface*, const char* name);
    using DrawModelExecute    = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
    using FireEvent           = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
    using DoPostScreenEffects =  int(__thiscall*)(IClientMode*, int);
	using OverrideView		  = void(__thiscall*)(IClientMode*, CViewSetup*);
	using SceneEnd			  = void(__thiscall*)(void*);
	long __stdcall hkEndScene(IDirect3DDevice9* device);
    long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
    void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active);
    void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
    void __stdcall hkPlaySound(const char* name);
    void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
    void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
	void __stdcall hkOverrideView(CViewSetup * vsView);
	void __fastcall hkSceneEnd(void* thisptr, void* edx);
    int  __stdcall hkDoPostScreenEffects(int a1);
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx);
	bool __stdcall hkFireEvent(IGameEvent* pEvent);
	void hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output);
	void __fastcall SetKeyCodeState_h(void* thisptr, void* EDX, ButtonCode_t code, bool bDown);
	void __fastcall SetMouseCodeState_h(void* thisptr, void* EDX, ButtonCode_t code, MouseCodeState_t state);
	void __stdcall hkLockCursor();
	//extern HRESULT(STDMETHODCALLTYPE *oPresent) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
}

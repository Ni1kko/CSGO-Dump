#include "Math.h"
#include "render.h"
#include "interfaces.h"
#include "VMT_Manager.h"
#include "Variables.h"
#include "Chams.h"
#include "Glow.h"
class c_studio_hdr;
#pragma region Typedefs
typedef void(__thiscall *PaintTraverseFn)(void*, unsigned int, bool, bool);
using SendDatagramFn = int(__thiscall*)(void*, void *);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*);
typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9*);
typedef long(__stdcall *ResetFn)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef void(__fastcall* SceneEndFn)(void*, void*);
typedef bool(__thiscall* DoPostScreenEffectsFn)(void*, CViewSetup*);
typedef void(__thiscall* BeginFrameFn)(void*);
typedef bool(__thiscall* temp_entities_fn)(void*, void*);
using ProcessPacket_t = void(__thiscall*)(void*, void*, bool);
typedef bool(__thiscall *FireEventClientSide)(void*, IGameEvent*);
typedef bool(*LineGoesThroughSmokeFn) (Vector, Vector, int16_t);
typedef void(__stdcall *OverRideViewFn)(void*);
typedef void(__thiscall* fnDrawModel)(void*, void*, const DrawModelInfo_t&, matrix*, float*, float*, const Vector&, int);
typedef void(__fastcall *HudDeathNotifyFn)(PVOID, IGameEvent*);
typedef void(__thiscall* DrawModelExecuteFn)(void*, void*, void*, const ModelRenderInfo_t&, matrix*);
typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
typedef float(__thiscall* AspectRatioFn)(void* pEcx, int32_t iWidth, int32_t iHeight);
using TraceRayFn = void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
typedef bool(__thiscall* InPredictionFn)(CPrediction*);
typedef void(__thiscall* fire_event_t)(IEngineClient*);
typedef void(__thiscall* fnDrawModel)(void*, void*, const DrawModelInfo_t&, matrix*, float*, float*, const Vector&, int);
typedef void(__thiscall* fnCamera)();
typedef void(__thiscall* RunCommandFn)(void*, IBasePlayer*, CUserCmd*, IMoveHelper*);
typedef float(__stdcall* GetViewModelFOVFn)();
typedef int(__thiscall * ListLeavesInBoxFn)(void*, const Vector&, const Vector&, unsigned short*, int);
typedef long(__stdcall*  PresentFn)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using Present_t = HRESULT(__stdcall*)(IDirect3DDevice9Ex*, const RECT*, const RECT*, HWND, const RGNDATA*);
using Reset_t = HRESULT(__stdcall*)(IDirect3DDevice9Ex*, D3DPRESENT_PARAMETERS*);
typedef void(__thiscall* DoExtraBoneProcessingFn)(IBasePlayer*, c_studio_hdr*, Vector*, Quaternion*, const matrix&, uint8_t*, void*);
typedef void(__thiscall* StandardBlendingRulesFn)(IBasePlayer*, c_studio_hdr*, Vector*, Quaternion*, float, int);
typedef void(__thiscall* UpdateClientSideAnimationFn)(IBasePlayer*);
typedef bool(__fastcall* ShouldSkipAnimFrameFn)();
typedef void(__thiscall* BuildTransformationsFn)(void* ecx, int a2, int a3, int a4, int a5, int a6, int a7);
typedef int32_t(__thiscall* BoxVisibleFn)(IEngineClient*, Vector&, Vector&);
typedef bool(__thiscall* IsHLTVFn)(IEngineClient*);
typedef bool(__thiscall* IsRenderableInPVSFn)(void*, IClientRenderable*);
typedef Vector*(__thiscall* EyeAnglesFn)(IBasePlayer*);
typedef bool(__fastcall* SetupBonesFn) (void*, void*, matrix*, int, int, float);
typedef void(__thiscall* FireBulletsFn)(C_TEFireBullets*, DataUpdateType_t);
typedef bool (__thiscall * WriteUsercmdDeltaToBufferFn) (void *, int, void *, int, int, bool);
using do_animation_event_fn = void * (__thiscall*)(void*, unsigned int, int);
using send_net_msg_fn = bool(__thiscall*)(INetChannel*, INetMessage&, bool, bool);
using send_datagram_fn = int(__thiscall*)(void*, void*);
using net_showfragments_t = bool(__thiscall *)(void*);
typedef const char* (__thiscall* GetForeignFallbackFontNameFn)(void*);
typedef void(__thiscall* PaintFn)(IEngineVGui*, int);
//void __fastcall hkClMove(void* ecx,void* edx, float accumulated_extra_samples, bool bFinalTick)
typedef void(__cdecl* ClMoveFn)(float, bool);
#pragma endregion

#pragma region ExternClases
class CNetVarManager;

extern c_interfaces interfaces;
extern CNetVarManager netvars;
extern cStructManager* csgo;

#pragma endregion

#pragma region NameSpaces

namespace I
{
	extern void Setup();
}

namespace F
{
	extern float KillDelayTime;
	extern LineGoesThroughSmokeFn LineToSmoke;
	extern bool Shooting();
}

#pragma endregion

namespace VMT
{
	extern std::unique_ptr< VMTHook > g_pDirectXHook;
	extern std::unique_ptr< VMTHook > g_pClientStateAdd;
	extern std::unique_ptr< VMTHook > g_pPanelHook;
	extern std::unique_ptr< VMTHook > g_pClientModeHook;
	extern std::unique_ptr< VMTHook > g_pClientHook;
	extern std::unique_ptr< VMTHook > g_pRenderViewHook;
	extern std::unique_ptr< VMTHook > g_pStudioRenderHook;
	//extern std::unique_ptr< VMTHook > g_pFireEventHook;
	extern std::unique_ptr< VMTHook > g_pModelRenderHook;
	extern std::unique_ptr< VMTHook > g_pPredictHook;
	extern std::unique_ptr< VMTHook > g_pQueryHook;
	extern std::unique_ptr< VMTHook > g_pGetBoolHook;
	extern std::unique_ptr< VMTHook > g_pCameraHook;
	extern std::unique_ptr< VMTHook > g_pEngineHook;
	//extern std::unique_ptr< VMTHook > g_pPlayerHook;
	extern std::unique_ptr< VMTHook > g_pTEFireBullets;
	extern std::unique_ptr< VMTHook > g_pNetShowFragments;
	extern std::unique_ptr< VMTHook > g_pLeafSystemHook;
	extern std::unique_ptr< VMTHook > g_pVGUIHook;
	extern std::unique_ptr< VMTHook > g_pD3DX9;
}
using namespace VMT;

namespace H
{
	/*extern PaintTraverseFn PaintTraverse;
	extern CreateMoveFn CreateMove;
	extern FrameStageNotifyFn FrameStageNotify;
	extern EndSceneFn EndScene;
	extern ResetFn Reset;
	extern SceneEndFn SceneEnd;
	extern fire_event_t FireEvent;
	extern DoPostScreenEffectsFn DoPostScreenEffects;
	extern FireEventClientSide FireEventClientSideNine;
	extern OverRideViewFn OverRideView;
	extern BeginFrameFn BeginFrame;
	extern HudDeathNotifyFn HudDeathNotify;
	extern DrawModelExecuteFn DrawModelExecute;
	extern TraceRayFn TraceRay;

	extern SvCheatsGetBoolFn SvCheatsGetBool;
	extern InPredictionFn InPrediction;
	extern fnDrawModel DrawModel;
	extern fnCamera Camera;
	
	extern ListLeavesInBoxFn ListLeavesInBox;
	extern PresentFn Present;

	

	
	extern BoxVisibleFn BoxVisible;
	extern IsHLTVFn IsHLTV;
	extern do_animation_event_fn DoAnimEvent;
	extern IsRenderableInPVSFn IsRenderableInPVS;
	extern EyeAnglesFn EyeAngles;
	
	extern FireBulletsFn FireBullets;
	extern net_showfragments_t NetShowFragments;
	extern SetupBonesFn SetupBones;*/

	extern ClMoveFn ClMove;
	extern SetupBonesFn SetupBones;
	extern send_net_msg_fn SendNetMsg;
	extern send_datagram_fn SendDatagram;
	extern temp_entities_fn TempEntities;
	extern CClientEffectRegistration* Effects;
	extern GetForeignFallbackFontNameFn GetForeignFallbackFontName;
	extern StandardBlendingRulesFn StandardBlendingRules;
	extern ShouldSkipAnimFrameFn ShouldSkipAnimFrame;
	extern DoExtraBoneProcessingFn DoExtraBoneProcessing;
	extern UpdateClientSideAnimationFn UpdateClientSideAnimation;
	extern ClientEffectCallback ImpactCallback;
	extern Present_t SteamPresent;
	extern Reset_t SteamReset;
	extern BuildTransformationsFn BuildTransformations;

	void Hook();
	void UnHook();
}

extern void Msg(string str, color_t clr);

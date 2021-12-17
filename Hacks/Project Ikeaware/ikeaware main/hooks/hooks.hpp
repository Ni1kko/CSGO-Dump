#pragma once

#include "..//valve_sdk/sdk.hpp"
#include "..//helpers/vfunc_hook.hpp"
#include "..//helpers/memory.hpp"
#include "..//helpers/input.hpp"

#include "..//options.hpp"
#include "..//render.hpp"

#include "..//features/aimbot.hpp"
#include "..//features/ragebot.h"
#include "..//features/animation.h"
#include "..//features/chams.hpp"
#include "..//features/glow.hpp"
#include "..//features/misc.hpp"
#include "..//features/skins.hpp"
#include "..//features/visuals.hpp"
#include "..//menu.hpp"
#include "EventListener.hpp"

#include <d3d9.h>
#include <mutex>


namespace index
{
	constexpr auto EmitSound                = 5;
    constexpr auto EndScene                 = 42;
    constexpr auto Reset                    = 16;
    constexpr auto PaintTraverse            = 41;
	constexpr auto FrameStageNotify         = 37;
    constexpr auto CreateMove               = 24;
    constexpr auto DrawModelExecute         = 29;
    constexpr auto RenderSmokeOverlay       = 41;
    constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto OverrideView             = 18;
	constexpr auto GetViewModelFov			= 35;
	constexpr auto ShouldDrawFog			= 17;
	constexpr auto ListLeavesInBox			= 6;
	constexpr auto SuppressLists			= 16;
	constexpr auto ReadPacketEntities		= 64;
	constexpr auto GetBool		            = 13;
	constexpr auto SendMsg					= 0;
	constexpr auto RetrieveMsg              = 2;
}

namespace Hooks
{
    void Initialize();
    void Shutdown();

	inline vfunc_hook hlclient_hook;
	inline vfunc_hook direct3d_hook;
	inline vfunc_hook vguipanel_hook;
	inline vfunc_hook stdrender_hook;
	inline vfunc_hook viewrender_hook;
	inline vfunc_hook sound_hook;
	inline vfunc_hook clientmode_hook;
	inline vfunc_hook clientstate_hook;
	inline vfunc_hook partition_hook;
	inline vfunc_hook bspquery_hook;
	inline vfunc_hook sv_cheats;
	inline vfunc_hook steamapi_hook;

	inline recv_prop_hook* sequence_hook;

    long __stdcall EndScene(IDirect3DDevice9* device);
    long __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __fastcall FrameStageNotify(void* _this, ClientFrameStage_t stage);
	bool __fastcall CreateMove(void* _this, int, float input_sample_frametime, CUserCmd* cmd);
	void __fastcall PaintTraverse(void* _this, int, vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __fastcall EmitSound(void* _this, int, IRecipientFilter & filter, int iEntIndex, int iChannel, const char * pSoundEntry, unsigned int nSoundEntryHash, const char * pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector * pOrigin, const Vector * pDirection, void * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
    void __fastcall DrawModelExecute(void* _this, int, void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int32_t iFlags);
	void __fastcall RenderSmokeOverlay(void* _this, int, const bool unk);
	void __fastcall OverrideView(void* _this, int, CViewSetup * vsView);
    int  __fastcall DoPostScreenEffects(void* _this, int, int a1);
	float __fastcall GetViewModelFov(void* _this, int);
	bool __fastcall ShouldDrawFog(void* _this, int);
	void __fastcall SuppressLists(void* _this, int, int a2, bool a3);
	int  __fastcall ListLeavesInBox(void* _this, int, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);
	void __fastcall ReadPacketEntities(void* _this, int, void* entInfo);
	bool __fastcall SvCheatsGetBool(void* pConVar, void* edx);

	EGCResult __fastcall GCSendMessage(void* _this, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);
	EGCResult __fastcall GCRetrieveMsg(void* _this, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);

	void RecvProxy(const CRecvProxyData* pData, void* entity, void* output);

	inline CGameEventListener* listener;
}

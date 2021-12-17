#pragma once

#include "Utils\DrawManager.h"
#include "Utils\Interfaces.h"
#include "SDK\IClientMode.h"
#include "SDK\ISurface.h"
#include "SDK\Materials.h"
#include "EventListener.h"
#include "SDK\CInput.h"
#include "GUI\GUI.h"
#include "SDK\CPanel.h"
#include "SDK\IBaseClientDll.h"
#include "SDK\ConVar.h"
#include "intrin.h"
#include "SDK\IClientString.h"
#include "SDK\CPrediction.h"

namespace vtable_indexes
{
	constexpr auto reset        = 16;
	constexpr auto present      = 17;
	constexpr auto createMove   = 24;
	constexpr auto lockCursor   = 67;
	constexpr auto framestage   = 37;
	constexpr auto sceneend     = 9;
	constexpr auto postscreenfx = 44;
	constexpr auto dmeidx       = 21;
	constexpr auto overrideview = 18;
	constexpr auto fov          = 35;
	constexpr auto painttraverse = 41;
}

class VMTHook;
class Hooks
{
public:
    // Initialization setup, called on injection
    static void Init();
    static void Restore();

    /*---------------------------------------------*/
    /*-------------Hooked functions----------------*/
    /*---------------------------------------------*/

    static bool     __fastcall  CreateMove(IClientMode*, void*, float, CUserCmd*);
    static void     __fastcall  LockCursor(ISurface*, void*);
    static HRESULT  __stdcall   Reset     (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
    static HRESULT  __stdcall   Present   (IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    static LRESULT  __stdcall   WndProc   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void		__fastcall  SceneEnd   (void*, void*);
	static int      __fastcall  DoPostScreenEffects(void* ecx, void* edx, int a1);
	static void		__fastcall  FrameStageNotify(void*, void*, int);
	static void		__fastcall  OverrideView(void * thisptr, void * edx, CViewSetup * setup);
	static float    __stdcall GGetViewModelFOV();
	static void	    __fastcall DrawModelExecute(void * ecx, void * edx, IMatRenderContext * context, const DrawModelState_t & state, const ModelRenderInfo_t & render_info, matrix3x4_t * matrix);
	static void		__stdcall PaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);
	//static bool		__fastcall	HookedGetBool(void* pConVar, void* edx);

	//static int		__fastcall  SendDatagram(NetChannel* ecx, void* edx, void* data);
	//static void     __fastcall  DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix);

	//static void		__fastcall  RenderView (void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
	HWND                           hCSGOWindow = nullptr; // CSGO window handle

private:
    /*---------------------------------------------*/
    /*-------------VMT Hook pointers---------------*/
    /*---------------------------------------------*/

    std::unique_ptr<VMTHook> pD3DDevice9Hook;
    std::unique_ptr<VMTHook> pClientModeHook;
    std::unique_ptr<VMTHook> pSurfaceHook;
	std::unique_ptr<VMTHook> pRenderViewHook;
	std::unique_ptr<VMTHook> pModelRenderHook;
	std::unique_ptr<VMTHook> pClientDllHook;
	std::unique_ptr<VMTHook> pPanelHook;
	//std::unique_ptr<VMTHook> pSvCheatsGetBoolHook;

    /*---------------------------------------------*/
    /*-------------Hook prototypes-----------------*/
    /*---------------------------------------------*/

    typedef bool (__fastcall* CreateMove_t) (IClientMode*, void*, float, CUserCmd*);
    typedef void (__fastcall* LockCursor_t) (ISurface*, void*);
    typedef long (__stdcall*  Reset_t)      (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    typedef long (__stdcall*  Present_t)    (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	typedef void (__fastcall* SceneEnd_t)	(void*);
	typedef void (__fastcall* RenderView_t)	(void*, CViewSetup&, CViewSetup&, int, int);
	typedef int  (__fastcall* DoPostScreenEffects_t) (void*, void*, int);
	typedef int  (__fastcall* SendDatagram_t)(void*, void*);
	typedef void (__thiscall* FrameStageNotify_t)(void*, int);
	typedef void(__thiscall* OverrideView_t)(void*, CViewSetup*);
	typedef float(__stdcall* oGetViewModelFOV)();
	typedef void(__thiscall* PaintTraverse_t)(void*, unsigned int, bool, bool);
	typedef void(__thiscall* DrawModelExecute_t)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	//typedef bool(__fastcall* SvCheatsGetBoolFn)(void*);

private:
    ui::MenuMain                   nMenu;
    bool                           bInitializedDrawManager = false;   // Check if we initialized our draw manager
    WNDPROC                        pOriginalWNDProc        = nullptr; // Original CSGO window proc
    std::unique_ptr<EventListener> pEventListener          = nullptr; // Listens to csgo events, needs to be created
};

extern Hooks g_Hooks;


class VMTHook
{
public:
    VMTHook(void* ppClass)
    {
        this->ppBaseClass = static_cast<std::uintptr_t**>(ppClass);

        // loop through all valid class indexes. When it will hit invalid (not existing) it will end the loop
        while (static_cast<std::uintptr_t*>(*this->ppBaseClass)[this->indexCount])
            ++this->indexCount;

        const std::size_t kSizeTable = this->indexCount * sizeof(std::uintptr_t);


        this->pOriginalVMT = *this->ppBaseClass;
        this->pNewVMT      = std::make_unique<std::uintptr_t[]>(this->indexCount);

        // copy original vtable to our local copy of it
        std::memcpy(this->pNewVMT.get(), this->pOriginalVMT, kSizeTable);

        // replace original class with our local copy
        *this->ppBaseClass = this->pNewVMT.get();
    };
    ~VMTHook() { *this->ppBaseClass = this->pOriginalVMT; };

    template<class Type>
    Type GetOriginal(const std::size_t index)
    {
        return reinterpret_cast<Type>(this->pOriginalVMT[index]);
    };

    HRESULT Hook(const std::size_t index, void* fnNew)
    {
        if (index > this->indexCount)   // check if given index is valid
            return E_INVALIDARG;

        this->pNewVMT[index] = reinterpret_cast<std::uintptr_t>(fnNew);
        return S_OK;
    };

    HRESULT Unhook(const std::size_t index)
    {
        if (index > this->indexCount)
            return E_INVALIDARG;

        this->pNewVMT[index] = this->pOriginalVMT[index];
        return S_OK;
    };

private:
    std::unique_ptr<std::uintptr_t[]> pNewVMT      = nullptr;    // Actual used vtable
    std::uintptr_t**                  ppBaseClass  = nullptr;             // Saved pointer to original class
    std::uintptr_t*                   pOriginalVMT = nullptr;             // Saved original pointer to the VMT
    std::size_t                       indexCount   = 0;                     // Count of indexes inside out f-ction
};

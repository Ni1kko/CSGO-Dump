#include "Interfaces.h"
#include "Utils.h"

#include "..\SDK\IClientMode.h"
#include "..\SDK\IBaseClientDll.h"
#include "..\SDK\IClientEntityList.h"
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CPrediction.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\SDK\Materials.h"
#include "..\Features\Glow.h"
#include "..\SDK\ConVar.h"
#include "..\SDK\CTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\CPanel.h"
#include "..\SDK\IViewRenderBeams.h"
#include "..\SDK\IClientString.h"
#include "..\SDK\IVDebugOverlay.h"
// Initializing global interfaces

IBaseClientDLL*     g_pClientDll    = nullptr;
IClientMode*        g_pClientMode   = nullptr;
IClientEntityList*  g_pEntityList   = nullptr;
IVEngineClient*     g_pEngine       = nullptr;
CPrediction*        g_pPrediction   = nullptr;
IGameMovement*      g_pMovement     = nullptr;
IMoveHelper*        g_pMoveHelper   = nullptr;
CGlobalVarsBase*    g_pGlobalVars   = nullptr;
IGameEventManager2* g_pEventManager = nullptr;
ISurface*           g_pSurface      = nullptr;
CVRenderView*       g_pRenderView   = nullptr;
IVModelRender*		g_pModelRender  = nullptr;
CMaterialSystem*    g_pMaterialSystem = nullptr;
CGlowObjectManager* g_pGlowManager  = nullptr;
CModelInfo*			g_pModelInfo    = nullptr;
ICVar*              g_pConVar       = nullptr;
CTrace*				g_pTrace        = nullptr;
IPhysicsProps*      g_pPhysProps    = nullptr;
CInput*				g_pInput	    = nullptr;
CPanel*				g_pPanel		= nullptr;
IViewRenderBeams*	g_pBeam			= nullptr;
IVDebugOverlay* g_pDebugOverlay		= nullptr;
CNetworkStringTableContainer* g_pNetworkString = nullptr;
CClientState* g_pClientState = nullptr;


namespace interfaces
{
    template<typename T>
    T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
    {
        HMODULE moduleHandle = GetModuleHandleA(szModuleName);
        if (moduleHandle)   /* In case of not finding module handle, throw an error. */
        {
            CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, "CreateInterface"));
            return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
        }
        Utils::Log("Error getting interface %", szInterfaceVersion);
        return nullptr;
    }

    void Init()
    {
        g_pClientDll    = CaptureInterface<IBaseClientDLL>("client.dll", "VClient018");					// Get IBaseClientDLL
        g_pClientMode   = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5);  // Get IClientMode
        g_pGlobalVars   = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0]  + 0xA); // Get CGlobalVarsBase
		g_pPanel		= CaptureInterface<CPanel>("vgui2.dll", "VGUI_Panel009");
        g_pEntityList   = CaptureInterface<IClientEntityList>("client.dll", "VClientEntityList003");    // Get IClientEntityList
        g_pEngine       = CaptureInterface<IVEngineClient>("engine.dll", "VEngineClient014");						// Get IVEngineClient
        g_pPrediction   = CaptureInterface<CPrediction>("client.dll", "VClientPrediction001");             // Get CPrediction
        g_pMovement     = CaptureInterface<IGameMovement>("client.dll", "GameMovement001");                // Get IGameMovement
        g_pMoveHelper   = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2));  // Get IMoveHelper
        g_pEventManager = CaptureInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");				// Get IGameEventManager2
        g_pSurface      = CaptureInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface031");						// Get ISurface
		g_pRenderView   = CaptureInterface<CVRenderView>("engine.dll", "VEngineRenderView014");						// Get RenderView
		g_pModelRender  = CaptureInterface<IVModelRender>("engine.dll", "VEngineModel016");							// Get ModelRender
		g_pMaterialSystem = CaptureInterface<CMaterialSystem>("materialsystem.dll", "VMaterialSystem080");			// Get MaterialSystem
		g_pGlowManager  = (CGlowObjectManager*)(*(DWORD*)(Utils::FindSignature("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 0x3));  // Get GlowManager
		g_pInput		= *(CInput**)(Utils::FindSignature("client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 0x1);
		g_pModelInfo    = CaptureInterface<CModelInfo>("engine.dll", "VModelInfoClient004");						// Get ModelInfo
		g_pConVar       = CaptureInterface<ICVar>("vstdlib.dll", "VEngineCvar007");						            // Get ConVar
		g_pTrace        = CaptureInterface<CTrace>("engine.dll", "EngineTraceClient004");						    // Get Trace
		g_pPhysProps    = CaptureInterface<IPhysicsProps>("vphysics.dll", "VPhysicsSurfaceProps001");               // Get PhysProps
		g_pBeam		    = *(IViewRenderBeams**)(Utils::FindSignature("client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 0x1);
		g_pNetworkString = CaptureInterface<CNetworkStringTableContainer>("engine.dll", "VEngineClientStringTable001");
		g_pDebugOverlay = CaptureInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
		g_pClientState = **reinterpret_cast<CClientState***> ((*reinterpret_cast<uintptr_t**> (g_pEngine))[12] + 0x10);
    }
}

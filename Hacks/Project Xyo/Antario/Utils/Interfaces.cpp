#include "Interfaces.h"
#include "Utils.h"

#include "..\SDK\IClientMode.h"
#include "..\SDK\IBaseClientDll.h"
#include "..\SDK\IClientEntityList.h"
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CPrediction.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\SDK\IEngineTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\ICvar.h"
#include "..\SDK\IVModelInfo.h"
#include "..\SDK\CModelRender.h"
#include "..\SDK\IMaterial.h"
#include "..\SDK\IVRenderView.h"
#include "..\SDK\CGlowObjectManager.h"
#include "..\SDK\IVRenderBeams.h"
#include <fstream>

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

// Initializing global interfaces
IBaseClientDLL*     g_pClientDll    = nullptr;
IClientMode*        g_pClientMode   = nullptr;
IClientEntityList*  g_pEntityList   = nullptr;
IVEngineClient*     g_pEngine       = nullptr;
CPrediction*        g_pPrediction   = nullptr;
IGameMovement*      g_pMovement     = nullptr;
IMoveHelper*        g_pMoveHelper   = nullptr;
CGlobalVarsBase*    g_pGlobalVars   = nullptr;
IGameEventManager*  g_pEventManager = nullptr;
trace*				g_pTraceV2		= nullptr;
ISurface*           g_pSurface      = nullptr;
IEngineTrace*       g_pTrace        = nullptr;
IPhysicsSurfaceProps* g_pSurfaceData = nullptr;
ICVar*              g_pCvar			= nullptr;
IPanel*				g_pPanel		= nullptr;
IVModelInfo*		g_pModelInfo	= nullptr;
IInputSystem*		  g_InputSystem = nullptr;
CModelRender*       g_pModelRender = nullptr;
IMaterialSystem*    g_pMaterialSys = nullptr;
IVRenderView*       g_pRenderView = nullptr;
CInput*		  g_GameInput = nullptr;
IViewRenderBeams* g_pRenderBeams = nullptr;
CGlowObjectManager* g_GlowManager = nullptr;
CClientState*		g_pClientState = nullptr;
namespace interfaces
{
	template< class T >
	T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
	{
		if (szModuleName.empty() || szInterfaceName.empty())
			return nullptr;
		typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
		CreateInterfaceFn hInterface = nullptr;
		while (!hInterface)
		{
			hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
			Sleep(5);
		}

		char pszBuffer[256];
		for (int i = 0; i < 100; i++)
		{
			sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
			PVOID pInterface = hInterface(pszBuffer, nullptr);

			if (pInterface && pInterface != NULL)
			{
				if (bSkip)
					sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

				Sleep(5);
				break;
			}
		}

		return (T*)hInterface(pszBuffer, nullptr);
	}
	void* FindInterface(const char* Module, const char* InterfaceName)
	{
		void* Interface = nullptr;
		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(
			GetModuleHandleA(Module), enc_char("CreateInterface")));

		char PossibleInterfaceName[1024];
		for (int i = 1; i < 100; i++)
		{
			sprintf(PossibleInterfaceName, "%s0%i", InterfaceName, i);
			Interface = CreateInterface(PossibleInterfaceName, 0);
			if (Interface)
				break;

			sprintf(PossibleInterfaceName, "%s00%i", InterfaceName, i);
			Interface = CreateInterface(PossibleInterfaceName, 0);
			if (Interface)
				break;
		}

//		if (!Interface)
//			std::cout << (enc_str("Failed to find interface - ") + PossibleInterfaceName) << std::endl;
//		else
//			std::cout <<  (enc_str("Successfully found interface - ") + PossibleInterfaceName) << std::endl;

		return Interface;
	}

	void* FindInterfaceEx(const char* Module, const char* InterfaceName)
	{
		void* Interface = nullptr;
		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(
			GetModuleHandleA(Module), "CreateInterface"));

		Interface = CreateInterface(InterfaceName, 0);

//		if (!Interface)
//			std::cout << (enc_str("Failed to find interface - ") + InterfaceName) << std::endl;
//		else
//			std::cout << (enc_str("Successfully found interface - ") + InterfaceName) << std::endl;

		return Interface;
	}

    void Init()
    {
        g_pClientDll    = reinterpret_cast<IBaseClientDLL*>(FindInterface("client.dll", "VClient"));
        g_pClientMode   = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5u);  
        g_pGlobalVars   = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0]  + 0x1F); 
        g_pEntityList   = reinterpret_cast<IClientEntityList*>(FindInterface("client.dll", "VClientEntityList"));
        g_pEngine       = reinterpret_cast<IVEngineClient*>(FindInterface("engine.dll", "VEngineClient"));
        g_pPrediction   = reinterpret_cast<CPrediction*>(FindInterface("client.dll", "VClientPrediction"));
        g_pMovement     = reinterpret_cast<IGameMovement*>(FindInterface("client.dll", "GameMovement"));
        g_pMoveHelper   = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2));  
        g_pEventManager = reinterpret_cast<IGameEventManager*>(FindInterfaceEx("engine.dll", "GAMEEVENTSMANAGER002"));
        g_pSurface      = reinterpret_cast<ISurface*>(FindInterface("vguimatsurface.dll", "VGUI_Surface"));
		g_pTrace        = reinterpret_cast<IEngineTrace*>(FindInterface("engine.dll", "EngineTraceClient"));
		g_pTraceV2		= reinterpret_cast<trace*>(FindInterface("engine.dll", "EngineTraceClient"));
		g_pSurfaceData  = reinterpret_cast<IPhysicsSurfaceProps*>(FindInterface("vphysics.dll", "VPhysicsSurfaceProps"));
		g_pCvar         = reinterpret_cast<ICVar*>(FindInterface("vstdlib.dll", "VEngineCvar"));
		g_pPanel		= reinterpret_cast<IPanel*>(FindInterface("vgui2.dll", "VGUI_Panel"));
		g_pModelInfo    = reinterpret_cast<IVModelInfo*>(FindInterface("engine.dll", "VModelInfoClient"));
		g_InputSystem = reinterpret_cast<IInputSystem*>(FindInterface("inputsystem.dll", "InputSystemVersion"));
		g_pModelRender = FindClass<CModelRender>("engine.dll", "VEngineModel");
		g_pMaterialSys = FindClass<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		g_pRenderView = FindClass<IVRenderView>("engine.dll", "VEngineRenderView");
		g_pRenderBeams = *reinterpret_cast<IViewRenderBeams**>(Utils::FindSignature("client.dll", "A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 0x1);
		g_GlowManager = *reinterpret_cast<CGlowObjectManager**>(Utils::FindSignature("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		g_pClientState = util::get_method(g_pEngine, 12).add(16).get(2).as< CClientState* >();

		auto pdwClient = *(PDWORD_PTR*)g_pClientDll;
		g_GameInput = *(CInput**)(Utils::FindSignature("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		//thanks monarch (from yeti)

		std::ofstream("csgo\\materials\\nonflat.vmt") << R"#("VertexLitGeneric"
		{
		 "$basetexture" "vgui/white_additive"
		 "$ignorez"      "0"
		 "$envmap"       ""
		 "$nofog"        "1"
		 "$model"        "1"
		 "$nocull"       "0"
		 "$selfillum"    "1"
		 "$halflambert"  "1"
		 "$znearer"      "0"
		 "$flat"         "1"
		 "$reflectivity" "[1 1 1]"
		} )#";

		std::ofstream("csgo\\materials\\metallic.vmt") << R"#("VertexLitGeneric" {
          "$basetexture" "vgui/white_additive"
          "$ignorez" "0"
          "$envmap" "env_cubemap"
          "$normalmapalphaenvmapmask" "1"
          "$envmapcontrast"  "1"
          "$nofog" "1"
          "$model" "1"
          "$nocull" "0"
          "$selfillum" "1"
          "$halflambert" "1"
          "$znearer" "0"
          "$flat" "1" 
    }
    )#";

		std::ofstream("csgo\\materials\\flat.vmt") << R"#("UnlitGeneric"
		{
		 "$basetexture" "vgui/white_additive"
		  "$no_fullbright" "0"
		 "$ignorez"      "1"
		 "$envmap"       "env_cubemap"
		 "$nofog"        "1"
		"$model"        "1"
		"$nocull"       "0"
		"$selfillum"    "1"
		 "$halflambert"  "1"
		 "$znearer"      "0"
		 "$flat"         "0"
		} )#";
    }

}

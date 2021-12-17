#pragma once
#include "../FEATURES/rankchanger/IMemAlloc.h"
#include "../FEATURES/rankchanger/steam.h"
#include "../FEATURES/rankchanger/Protobuffs.h"

namespace SDK
{
	class IClient;
	class CInput;
	class CPanel;
	class EngineVGui;
	class ISurface;
	class IEngine;
	class CClientEntityList;
	class CClientEntityListTWO;
	class CGlobalVars;
	class IVDebugOverlay;
	class CTrace;
	class CModelInfo;
	class ICVar;
	class CInputSystem;
	class CModelRender;
	class CRenderView;
	class IMaterialSystem;
	class IPhysicsProps;
	class COverrideView;
	class IGameEventManager;
	class IViewRender;
	class IViewRenderBeams;
	class CGlowObjectManager;
	class CPrediction;
	class IGameMovement;
	class IMoveHelper;
	class IMemAlloc;
}
#define STEAMUSER INTERFACES->g_SteamUser()

namespace INTERFACES
{
	extern SDK::IClient* Client;
	extern SDK::CInput* Input;
	extern SDK::CPanel* Panel;
	extern SDK::EngineVGui* EngineVGui;
	extern SDK::ISurface* Surface;
	extern SDK::IEngine* Engine;
	extern SDK::CClientEntityList* ClientEntityList;
	extern SDK::CClientEntityListTWO* ClientEntityList2;
	extern SDK::CGlobalVars* Globals;
	extern SDK::IVDebugOverlay* DebugOverlay;
	extern SDK::CTrace* Trace;
	extern SDK::CModelInfo* ModelInfo;
	extern SDK::ICVar* cvar;
	extern SDK::CInputSystem* InputSystem;
	extern SDK::CModelRender* ModelRender;
	extern SDK::CRenderView* RenderView;
	extern SDK::IMaterialSystem* MaterialSystem;
	extern SDK::IPhysicsProps* PhysicsProps;
	extern SDK::COverrideView* ClientMode;
	extern SDK::IGameEventManager* GameEventManager;
	extern SDK::IViewRender* ViewRender;
	extern SDK::IViewRenderBeams* ViewRenderBeams;
	extern SDK::CGlowObjectManager* GlowObjManager;
	extern SDK::CPrediction* pPrediction;
	extern SDK::IGameMovement* pMovement;
	extern SDK::IMoveHelper* pMoveHelper;

	extern ISteamGameCoordinator* g_SteamGameCoordinator;
	extern IMemAlloc*   g_pMemAlloc;
	extern ISteamUser*  g_SteamUser;




	void InitInterfaces();

	typedef void* (*CreateInterfaceFn)(const char *Name, int *ReturnCode);
	void* FindInterface(const char* Module, const char* InterfaceName);
	void* FindInterfaceEx(const char* Module, const char* InterfaceName);
}
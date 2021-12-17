#pragma once

#include "BaseTypes.h"
#include "Const.h"
#include "Definitions.hpp"
#include "CRC.hpp"
#include "IAppSystem.hpp"
#include "Vector.hpp"
#include "Vplane.h"
#include "Vector2D.hpp"
#include "Vector4D.hpp"
#include "QAngle.hpp"
#include "VMatrix.hpp"
#include "CHandle.hpp"
#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"
#include "Color.hpp"
#include "IBaseClientDll.hpp"
#include "IClientEntity.hpp"
#include "IClientEntityList.hpp"
#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientUnknown.hpp"
#include "IVEngineClient.hpp"
#include "IEngineTrace.hpp"
#include "ILocalize.hpp"
#include "PlayerInfo.hpp"
#include "Recv.hpp"
#include "IClientMode.hpp"
#include "IVModelInfoClient.hpp"
#include "CInput.hpp"
#include "ICollideable.h"
#include "IEngineSound.h"
#include "IVModelRender.h"
#include "IVRenderView.h"
#include "IMaterial.h"
#include "IMaterialSystem.h"
#include "KeyValues.h"
#include "ISurface.h"
#include "IGameEvent.h"
#include "TextureGroupNames.h"
#include "CVar.h"
//#include "memalloc.h"

#include <Windows.h>
#include <WinInet.h>

#include "../CSX/CSX.h"
#include "../../steam_sdk/steam_api.h"

#pragma comment(lib, "Wininet.lib")

#define CREATEINTERFACE_PROCNAME XorStr("CreateInterface")
#define VENGINE_CLIENT_INTERFACE_VERSION XorStr("VEngineClient014")
#define CLIENT_DLL_INTERFACE_VERSION XorStr("VClient018")
#define VCLIENTENTITYLIST_INTERFACE_VERSION	XorStr("VClientEntityList003")
#define INTERFACEVERSION_ENGINETRACE_CLIENT	XorStr("EngineTraceClient004")
#define VMODELINFO_CLIENT_INTERACE_VERSION XorStr("VModelInfoClient004")
#define IENGINESOUND_CLIENT_INTERFACE_VERSION XorStr("IEngineSoundClient003")
#define VENGINE_HUDMODEL_INTERFACE_VERSION XorStr("VEngineModel016")
#define VENGINE_RENDERVIEW_INTERFACE_VERSION XorStr("VEngineRenderView014")
#define MATERIAL_SYSTEM_INTERFACE_VERSION XorStr("VMaterialSystem080")
#define VGUI_SURFACE_INTERFACE_VERSION XorStr("VGUI_Surface031")
#define GAMEEVENTMANAGER_INTERFACE_VERSION	XorStr("GAMEEVENTSMANAGER002")
#define VENGINEVCAR_INTERFACE_VERSION XorStr("VEngineCvar007")
#define INPUTSYSTEM_INTERFACE_VERSION XorStr("InputSystemVersion001")

#define ENGINE_DLL XorStr("engine.dll")
#define CLIENT_DLL XorStr("client.dll")
#define MATERIAL_DLL XorStr("materialsystem.dll")
#define VGUIMT_DLL XorStr("vguimatsurface.dll")
#define VSTDLIB_DLL	XorStr("vstdlib.dll")
#define INPUTSYSTEM_DLL	XorStr("inputsystem.dll")
#define STEAMAPI_DLL	XorStr("steam_api.dll")
#define SERVERBROWSER_DLL	XorStr("serverbrowser.dll")

#define GLOBAL_VARS_PATTERN XorStr("\xA1\x00\x00\x00\x00\x8B\x4D\xFC\x8B\x55\x08")
#define GLOBAL_VARS_MASK XorStr("x????xxxxxx")

#define CLIENT_MODE_PATTERN XorStr("\xB9\x00\x00\x00\x00\x83\x38\x02\x75\x0D\xFF\xB0\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x5D\xC3")
#define CLIENT_MODE_MASK XorStr("x????xxxxxxx????x????xx")

namespace SDK
{
	class I
	{
	public:
		static IVEngineClient*		Engine();
		static IBaseClientDLL*		Client();
		static IClientEntityList*	EntityList();
		static CClientEntityList*	ClientEntityList();
		static IGameEventManager2*	GameEvents();
		static CGlobalVarsBase*		GlobalVars();
		static CInput*				Input();
	//	static CClientState*		ClientState();
		static IEngineTrace*		EngineTrace();
		static CPanel* Panel();
		static IClientMode*			ClientMode();
		static IVModelInfoClient*	ModelInfo();
		static IEngineSound*		Sound();
		static IVModelRender*		ModelRender();
		static IVRenderView*		RenderView();
		static IMaterialSystem*		MaterialSystem();
		static ISurface*			Surface();
		static IGameEventManager2*	GameEvent();
		static IInputSystem*		InputSystem();
		static ConVar*              GetConVar();
		static IClientEntityList*	g_pEntityList;
		static CClientEntityList*	g_pClientEntityList;
		static IGameEventManager2*  g_pGameEvents;
		static ILocalize*           Localize();
		static ISteamGameCoordinator* SteamGameCoordinator();
		static ISteamUser* SteamUser();
	private:
		static IVEngineClient*		g_pEngine;
		static IBaseClientDLL*		g_pClient;
		//static IClientEntityList*	g_pEntityList;
		static CGlobalVarsBase*		g_pGlobals;
		static CInput*				g_pInput;
	//	static CClientState*		g_pClientState;
		static IEngineTrace*		g_pEngineTrace;
		static IClientMode*			g_pClientMode;
		static CPanel* g_pPanel;
		static IVModelInfoClient*	g_pModelInfo;
		static IEngineSound*		g_pSound;
		static IVModelRender*		g_pModelRender;
		static IVRenderView*		g_pRenderView;
		static IMaterialSystem*		g_pMaterialSystem;
		static ISurface*			g_pSurface;
		static IGameEventManager2*	g_pGameEventMgr;
		static IInputSystem*		g_pInputSystem;
		static ConVar*              g_pConVar;
		static ILocalize*           g_pLocalize;
		static ISteamGameCoordinator* g_pSteamGameCoordinator;
		static ISteamUser* g_pSteamUser;
	};

	template <typename T>
	T CallVFunc(void *vTable, int iIndex) {
		return (*(T**)vTable)[iIndex];
	}

}


inline void**& GetVTable( void* instance )
{
	return *reinterpret_cast<void***>( (size_t)instance );
}

inline const void** GetVTable( const void* instance )
{
	return *reinterpret_cast<const void***>( (size_t)instance );
}

template<typename T>
inline T GetMethod( const void* instance , size_t index )
{
	return reinterpret_cast<T> ( GetVTable( instance )[index] );
}                                                                                                                                                 
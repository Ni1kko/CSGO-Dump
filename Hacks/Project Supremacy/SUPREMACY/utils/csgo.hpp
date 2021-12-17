#pragma once

#include "..\sdk\interfaces\IVEngineClient.hpp"
#include "..\sdk\interfaces\IInputSystem.hpp"
#include "..\sdk\interfaces\IBaseClientDll.hpp"
#include "..\sdk\interfaces\IClientEntityList.hpp"
#include "..\sdk\interfaces\IClientMode.hpp"
#include "..\sdk\interfaces\ICvar.hpp"
#include "..\sdk\interfaces\IEngineTrace.hpp"
#include "..\sdk\interfaces\IRenderView.hpp"
#include "..\sdk\interfaces\IVModelRender.hpp"
#include "..\sdk\interfaces\IMaterialSystem.hpp"
#include "..\sdk\interfaces\IPanel.hpp"
#include "..\sdk\interfaces\IVModelInfoClient.hpp"
#include "..\sdk\interfaces\IMDLCache.hpp"
#include "..\sdk\interfaces\memalloc.h"
#include "..\sdk\interfaces\CClientState.hpp"
#include "..\sdk\interfaces\IPrediction.hpp"
#include "..\sdk\interfaces\IMoveHelper.hpp"
#include "..\sdk\interfaces\CInput.hpp"
#include "..\sdk\interfaces\ISurface.hpp"
#include "..\sdk\interfaces\IVDebugOverlay.hpp"
#include "..\sdk\interfaces\IViewRenderBeams.hpp"
#include "..\sdk\interfaces\IPhysics.hpp"
#include "..\sdk\misc\GlobalVars.hpp"
#include "..\sdk\misc\glow_outline_effect.hpp"
#include "..\sdk\interfaces\IGameEventManager.hpp"
#include "..\sdk\misc\UtlVectorSimple.hpp"
#include "..\sdk\misc\C_CSPlayerResource.h"

#include "convar_spoofer.h"

#include <d3dx9.h>
#include <d3d9.h>

class InterfaceReg {
private:
	using InstantiateInterfaceFn = void*(*)();
public:
	InstantiateInterfaceFn m_CreateFn;
	const char *m_pName;
	InterfaceReg *m_pNext;
};

class C_CSGO {
public:
	IDirect3DDevice9 * m_device();
	IVEngineClient * m_engine();
	IInputSystem * m_inputsys();
	IBaseClientDLL * m_client();
	IClientEntityList * m_entitylist();
	ICvar * m_cvar();
	IEngineTrace * m_trace();
	DWORD * m_inputinternal();
	IVRenderView * m_renderview();
	IMDLCache * m_modelcache();
	IVModelRender * m_modelrender();
	IMaterialSystem * m_materialsystem();
	IPanel * m_panel();
	IVModelInfoClient * m_modelinfo();
	IPrediction * m_prediction();
	IGameMovement * m_gamemovement();
	ISurface * m_surface();
	IVDebugOverlay * m_debugoverlay();
	IPhysicsSurfaceProps * m_physsurface();
	IGameEventManager2 * m_eventmanager();
	IViewRenderBeams * m_viewrenderbeams();
	IMemAlloc * m_memalloc();
	IClientMode * m_clientmode();
	CGlobalVarsBase * m_globals();
	CGlowObjectManager * m_glow();
	CClientState * m_clientstate();
	IMoveHelper * m_movehelper();
	CInput * m_input();
	C_CSPlayerResource * m_playerresource();

	C_ConvarSpoofer * m_cvarspoofer();
	DWORD m_postprocessing();
	DWORD m_ccsplayerrenderablevftable();
private:
	IDirect3DDevice9 * p_device;
	IVEngineClient * p_engine;
	IInputSystem * p_inputsys;
	IBaseClientDLL * p_client;
	IClientEntityList * p_entitylist;
	IEngineTrace * p_trace;
	ICvar * p_cvar;
	DWORD * p_inputinternal;
	IVRenderView * p_renderview;
	IMDLCache* p_modelcache;
	IVModelRender * p_modelrender;
	IMaterialSystem * p_materialsystem;
	IPanel * p_panel;
	IVModelInfoClient * p_modelinfo;
	IPrediction * p_prediciton;
	IGameMovement * p_gamemovement;
	ISurface * p_surface;
	IVDebugOverlay * p_debugoverlay;
	IPhysicsSurfaceProps * p_physsurface;
	IGameEventManager2 * p_eventmanager;
	IViewRenderBeams * p_viewrenderbeams;
	IMemAlloc * p_memalloc;
	IClientMode * p_clientmode;
	CGlobalVarsBase * p_globals;
	CGlowObjectManager * p_glow;
	CClientState * p_clientstate;
	IMoveHelper * p_movehelper;
	CInput * p_input;
	C_CSPlayerResource * p_playerresource;

	C_ConvarSpoofer * p_cvarspoofer;
	DWORD p_postprocessing;
	DWORD p_ccsplayerrenderablevftable;
};

extern C_CSGO g_csgo;
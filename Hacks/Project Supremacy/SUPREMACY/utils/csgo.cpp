#include "csgo.hpp"
#include "..\includes.hpp"

C_CSGO g_csgo;

template<typename T>
static T *get_interface(const char *mod_name, const char *interface_name, bool exact = false) {
	T *iface = nullptr;
	InterfaceReg *register_list;
	int part_match_len = strlen(interface_name);

	DWORD interface_fn = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandleA(mod_name), "CreateInterface"));

	if (!interface_fn) {
		return nullptr;
	}

	unsigned int jump_start = (unsigned int)(interface_fn)+4;
	unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

	register_list = **reinterpret_cast<InterfaceReg***>(jump_target + 6);

	for (InterfaceReg *cur = register_list; cur; cur = cur->m_pNext) {
		if (exact == true) {
			if (strcmp(cur->m_pName, interface_name) == 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
		else {
			if (!strncmp(cur->m_pName, interface_name, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
	}
	return iface;
}

IDirect3DDevice9 * C_CSGO::m_device() {
	if (!p_device)
		p_device = **reinterpret_cast<IDirect3DDevice9 ***>(util::pattern_scan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

	return p_device;
}

IVEngineClient * C_CSGO::m_engine() {
	if (!p_engine)
		p_engine = get_interface<IVEngineClient>("engine.dll", "VEngineClient");

	return p_engine;
}

IInputSystem * C_CSGO::m_inputsys() {
	if (!p_inputsys)
		p_inputsys = get_interface<IInputSystem>("inputsystem.dll", "InputSystemVersion");

	return p_inputsys;
}

IBaseClientDLL * C_CSGO::m_client() {
	if (!p_client)
		p_client = get_interface<IBaseClientDLL>("client_panorama.dll", "VClient0");

	return p_client;
}

IClientEntityList * C_CSGO::m_entitylist() {
	if (!p_entitylist)
		p_entitylist = get_interface<IClientEntityList>("client_panorama.dll", "VClientEntityList");

	return p_entitylist;
}

ICvar * C_CSGO::m_cvar() {
	if (!p_cvar)
		p_cvar = get_interface<ICvar>("vstdlib.dll", "VEngineCvar");

	return p_cvar;
}

IEngineTrace * C_CSGO::m_trace() {
	if (!p_trace)
		p_trace = get_interface<IEngineTrace>("engine.dll", "EngineTraceClient");

	return p_trace;
}

DWORD * C_CSGO::m_inputinternal() {
	if (!p_inputinternal)
		p_inputinternal = get_interface<DWORD>("vgui2.dll", "VGUI_InputInternal");

	return p_inputinternal;
}

IVRenderView * C_CSGO::m_renderview() {
	if (!p_renderview)
		p_renderview = get_interface<IVRenderView>("engine.dll", "VEngineRenderView");

	return p_renderview;
}

IMDLCache * C_CSGO::m_modelcache() {
	if (!p_modelcache)
		p_modelcache = get_interface<IMDLCache>("datacache.dll", "MDLCache");

	return p_modelcache;
}

IVModelRender * C_CSGO::m_modelrender() {
	if (!p_modelrender)
		p_modelrender = get_interface<IVModelRender>("engine.dll", "VEngineModel");

	return p_modelrender;
}

IMaterialSystem * C_CSGO::m_materialsystem() {
	if (!p_materialsystem)
		p_materialsystem = get_interface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");

	return p_materialsystem;
}

IPanel * C_CSGO::m_panel() {
	if (!p_panel)
		p_panel = get_interface<IPanel>("vgui2.dll", "VGUI_Panel");

	return p_panel;
}

IVModelInfoClient * C_CSGO::m_modelinfo() {
	if (!p_modelinfo)
		p_modelinfo = get_interface<IVModelInfoClient>("engine.dll", "VModelInfoClient");

	return p_modelinfo;
}

IPrediction * C_CSGO::m_prediction() {
	if (!p_prediciton)
		p_prediciton = get_interface<IPrediction>("client_panorama.dll", "VClientPrediction");

	return p_prediciton;
}

IGameMovement * C_CSGO::m_gamemovement() {
	if (!p_gamemovement)
		p_gamemovement = get_interface<IGameMovement>("client_panorama.dll", "GameMovement");

	return p_gamemovement;
}

ISurface * C_CSGO::m_surface() {
	if (!p_surface)
		p_surface = get_interface<ISurface>("vguimatsurface.dll", "VGUI_Surface");

	return p_surface;
}

IVDebugOverlay * C_CSGO::m_debugoverlay() {
	if (!p_debugoverlay)
		p_debugoverlay = get_interface<IVDebugOverlay>("engine.dll", "VDebugOverlay");

	return p_debugoverlay;
}

IPhysicsSurfaceProps * C_CSGO::m_physsurface() {
	if (!p_physsurface)
		p_physsurface = get_interface< IPhysicsSurfaceProps >("vphysics.dll", "VPhysicsSurfaceProps");

	return p_physsurface;
}

IGameEventManager2 * C_CSGO::m_eventmanager() {
	if (!p_eventmanager)
		p_eventmanager = get_interface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002", true);

	return p_eventmanager;
}

IViewRenderBeams * C_CSGO::m_viewrenderbeams() {
	if (!p_viewrenderbeams)
		p_viewrenderbeams = *reinterpret_cast<IViewRenderBeams **>(util::pattern_scan("client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);

	return p_viewrenderbeams;
}

IMemAlloc * C_CSGO::m_memalloc() {
	if (!p_memalloc)
		p_memalloc = *reinterpret_cast<IMemAlloc**>(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc"));

	return p_memalloc;
}

IClientMode * C_CSGO::m_clientmode() {
	if (!p_clientmode)
		p_clientmode = **reinterpret_cast<IClientMode***>((*(DWORD**)(m_client()))[10] + 0x5);

	return p_clientmode;
}

CGlobalVarsBase * C_CSGO::m_globals() {
	if (!p_globals)
		p_globals = **reinterpret_cast<CGlobalVarsBase***>((*(DWORD**)(m_client()))[0] + 0x1B);

	return p_globals;
}

CGlowObjectManager * C_CSGO::m_glow() {
	if (!p_glow)
		p_glow = *reinterpret_cast<CGlowObjectManager**>(util::pattern_scan("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);

	return p_glow;
}

CClientState * C_CSGO::m_clientstate() {
	if (!p_clientstate)
		p_clientstate = **reinterpret_cast<CClientState***> ((*reinterpret_cast<uintptr_t**> (m_engine()))[12] + 0x10);

	return p_clientstate;
}

IMoveHelper * C_CSGO::m_movehelper() {
	if (!p_movehelper)
		p_movehelper = **reinterpret_cast<IMoveHelper***>(util::pattern_scan("client_panorama.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);

	return p_movehelper;
}

CInput * C_CSGO::m_input() {
	if (!p_input)
		p_input = *reinterpret_cast<CInput**>(util::pattern_scan("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10 ") + 0x1);

	return p_input;
}

C_CSPlayerResource * C_CSGO::m_playerresource() {
	if (!p_playerresource)
		p_playerresource = **reinterpret_cast<C_CSPlayerResource ***>(util::pattern_scan("client_panorama.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2);

	return p_playerresource;
}

C_ConvarSpoofer * C_CSGO::m_cvarspoofer() {
	return p_cvarspoofer;
}

DWORD C_CSGO::m_postprocessing() {
	if (!p_postprocessing)
		p_postprocessing = *(DWORD*)(util::pattern_scan("client_panorama.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 2);

	return p_postprocessing;
}

DWORD C_CSGO::m_ccsplayerrenderablevftable() {
	if (!p_ccsplayerrenderablevftable)
		p_ccsplayerrenderablevftable = *(DWORD *)(util::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x4E);

	return p_ccsplayerrenderablevftable;
}
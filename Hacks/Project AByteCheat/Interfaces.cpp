#include "Interfaces.h"
#include "glow_outline_effect.h"
#include "Utilities.h"
#include <d3d9.h>
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUISurfaceFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
CreateInterface_t MatFactory = NULL;
CreateInterface_t InputSys = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;
void interfaces::Initialise()
{
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	InputSys = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::InputSys, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern("client_panorama.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");
	char* effectthing = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineEffects001");
	client = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	engine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	panels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	window = FindWindow("Valve001", NULL);
	surface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	ent_list = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	DebugOverlay = (IVDebugOverlay*)EngineFactory(EngineDebugThingInterface, NULL);
	prediction_dword = (DWORD*)ClientFactory(ClientPredictionInterface, NULL);
	materialsystem = (CMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	render_view = (CVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	model_render = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	model_info = (CModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	trace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	trace_2 = (Trace2*)EngineFactory(EngineTraceInterfaceName, NULL);
	m_iInputSys = (IInputSystem*)InputSys("InputSystemVersion001", NULL);
	phys_props = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	cvar = (ICVar*)StdFactory(VEngineCvarName, NULL);
	event_manager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);
	g_pD3DDevice9 = **(IDirect3DDevice9***)(Utilities::Memory::FindPatternV2("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	effects = (IVEffects*)EngineFactory(effectthing, NULL);
	ClientMode = **(IClientModeShared***)((*(uintptr_t**)client)[10] + 0x5);
	globals = **reinterpret_cast< CGlobalVarsBase*** >((*reinterpret_cast< DWORD** >(client))[0] + 0x1B);
	g_pViewRenderBeams = *(IViewRenderBeams**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);
	PDWORD pdwClientVMT = *(PDWORD*)client;
	glow_manager = *(CGlowObjectManager**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	client_state = **(CClientState***)(Utilities::Memory::FindPatternV2("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	g_ClientSideAnimationList = (CUtlVectorSimple*)*(DWORD*)(Utilities::Memory::FindPatternV2("client_panorama.dll", "A1 ? ? ? ? F6 44 F0 04 01 74 0B") + 1);
	pinput = *(CInput**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
}
namespace interfaces
{
	IBaseClientDLL* client;
	IVEngineClient* engine;
	IPanel* panels;
	IClientEntityList* ent_list;
	ISurface* surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared *ClientMode;
	CGlobalVarsBase *globals;
	DWORD *prediction_dword;
	CGlowObjectManager* glow_manager;
	CMaterialSystem* materialsystem;
	CVRenderView* render_view;
	IVModelRender* model_render;
	IInputSystem* m_iInputSys;
	CModelInfo* model_info;
	IEngineTrace* trace;
	IPhysicsSurfaceProps* phys_props;
	CClientState* client_state;
	IVEffects* effects;
	Trace2 *trace_2;
	HWND window;
	ICVar *cvar;
	CInput* pinput;
	CUtlVectorSimple *g_ClientSideAnimationList;
	IDirect3DDevice9        *g_pD3DDevice9;
	IGameEventManager2 *event_manager;
	HANDLE __FNTHANDLE;
	IMoveHelper* movehelper;
	IPrediction *c_prediction;
	IGameMovement* gamemovement;
	IViewRenderBeams* g_pViewRenderBeams;
	CGameMovement * c_gamemove;
};

























































































































































































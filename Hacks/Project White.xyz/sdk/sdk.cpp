#include "sdk.hpp"
#include "structs.hpp"

#include "../Helpers/Utils.hpp"

int( __cdecl* RandomInt )( int min, int max );
void( __cdecl* RandomSeed )( uint32_t seed );
float( __cdecl* RandomFloat )( float min, float max );

namespace Interfaces {
    CreateInterfaceFn get_module_factory(HMODULE module) {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)  {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
        
        return result;
    }

    void Initialize() {
        auto client = GetModuleHandleW(L"client.dll");
        auto engine = GetModuleHandleW(L"engine.dll");
        auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");
        auto stdlib = GetModuleHandleA("vstdlib.dll");

        auto engineFactory = get_module_factory(GetModuleHandleW(L"engine.dll"));
        auto clientFactory = get_module_factory(GetModuleHandleW(L"client.dll"));
        auto vguiFactory = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
        auto vgui2Factory = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
        auto matSysFactory = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
        auto dataCacheFactory = get_module_factory(GetModuleHandleW(L"datacache.dll"));
        auto vphysicsFactory = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
        auto inputSysFactory = get_module_factory(GetModuleHandleW(L"inputsystem.dll"));
        auto localizeFactory = get_module_factory(GetModuleHandleW(L"localize.dll"));
        auto studioFactory = get_module_factory(GetModuleHandleW(L"studiorender.dll"));
        auto filesysFactory = get_module_factory(GetModuleHandleW(L"filesystem_stdio.dll"));
        auto valveStdFactory = get_module_factory(stdlib);

        g_CHLClient = get_interface<IBaseClientDLL>(clientFactory, "VClient018");
        g_EntityList = get_interface<IClientEntityList>(clientFactory, "VClientEntityList003");
        g_Prediction = get_interface<IPrediction>(clientFactory, "VClientPrediction001");
        g_GameMovement = get_interface<CGameMovement>(clientFactory, "GameMovement001");
        g_MdlCache = get_interface<IMDLCache>(dataCacheFactory, "MDLCache004");
        g_EngineClient = get_interface<IVEngineClient>(engineFactory, "VEngineClient014");
        g_MdlInfo = get_interface<IVModelInfoClient>(engineFactory, "VModelInfoClient004");
        g_MdlRender = get_interface<IVModelRender>(engineFactory, "VEngineModel016");
        g_RenderView = get_interface<IVRenderView>(engineFactory, "VEngineRenderView014");
        g_EngineTrace = get_interface<IEngineTrace>(engineFactory, "EngineTraceClient004");
        g_DebugOverlay = get_interface<IVDebugOverlay>(engineFactory, "VDebugOverlay004");
        g_GameEvents = get_interface<IGameEventManager2>(engineFactory, "GAMEEVENTSMANAGER002");
        g_EngineSound = get_interface<IEngineSound>(engineFactory, "IEngineSoundClient003");
        g_MatSystem = get_interface<IMaterialSystem>(matSysFactory, "VMaterialSystem080");
        g_CVar = get_interface<ICvar>(valveStdFactory, "VEngineCvar007");
        g_VGuiPanel = get_interface<IPanel>(vgui2Factory, "VGUI_Panel009");
        g_VGuiSurface = get_interface<ISurface>(vguiFactory, "VGUI_Surface031");
        g_PhysSurface = get_interface<IPhysicsSurfaceProps>(vphysicsFactory, "VPhysicsSurfaceProps001");
        g_InputSystem = get_interface<IInputSystem>(inputSysFactory, "InputSystemVersion001");
        g_SpatialPartition = get_interface<uintptr_t>(engineFactory, "SpatialPartition001");
        g_Localize = get_interface<ILocalize>(localizeFactory, "Localize_001");
        g_StudioRender = get_interface< IStudioRender >(studioFactory, "VStudioRender026");
        g_BaseFileSystem = get_interface<IBaseFileSystem>(filesysFactory, "VBaseFileSystem011");
        g_MemAlloc = *(IMemAlloc**)GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc");
        g_GlobalVars = **(CGlobalVarsBase***)(Utils::PatternScan(client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
        g_ClientMode = *(IClientMode**)(Utils::PatternScan(client, "B9 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 53") + 1);
        g_Input = *(CInput**)(Utils::PatternScan(client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
        g_MoveHelper = **(IMoveHelper***)(Utils::PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
        g_GlowObjManager = *(CGlowObjectManager**)(Utils::PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
        g_ViewRender = *(IViewRender**)(Utils::PatternScan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
        g_D3DDevice9 = **(IDirect3DDevice9***)(Utils::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        g_ClientState = **(CClientState***)(Utils::PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
        g_GameRules = *(CGameRules**)(Utils::PatternScan(client, "8B 0D ?? ?? ?? ?? 85 C0 74 0A 8B 01 FF 50 78 83 C0 54") + 2);

        g_LocalPlayer = *(C_LocalPlayer*)(Utils::PatternScan(client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);

        RandomInt = reinterpret_cast<decltype(RandomInt)>(GetProcAddress(stdlib, "RandomInt"));
        RandomSeed = reinterpret_cast<decltype(RandomSeed)>(GetProcAddress(stdlib, "RandomSeed"));
        RandomFloat = reinterpret_cast<decltype(RandomFloat)>(GetProcAddress(stdlib, "RandomFloat"));
    }
}
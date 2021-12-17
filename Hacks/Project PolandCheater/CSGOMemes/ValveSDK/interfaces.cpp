#include "Interfaces.hpp"

#include "../Helpers/utils.hpp"
template<class T>
static T* FindHudElement(const char* name)
{
    static auto pThis = *reinterpret_cast<DWORD**>(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 83 7D F8 ?") + 1);

    static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
    return (T*)find_hud_element(pThis, name);
}

IVEngineClient*       g_EngineClient = nullptr;
IBaseClientDLL*       g_CHLClient = nullptr;
IClientEntityList*    g_EntityList = nullptr;
CGlobalVarsBase*      g_GlobalVars = nullptr;
IEngineTrace*         g_EngineTrace = nullptr;
ICvar*                g_CVar = nullptr;
IPanel*               g_VGuiPanel = nullptr;
IClientMode*          g_ClientMode = nullptr;
IVDebugOverlay*       g_DebugOverlay = nullptr;
ISurface*             g_VGuiSurface = nullptr;
CInput*               g_Input = nullptr;
IVModelInfoClient*    g_MdlInfo = nullptr;
IVModelRender*        g_MdlRender = nullptr;
IVRenderView*         g_RenderView = nullptr;
IMaterialSystem*      g_MatSystem = nullptr;
IGameEventManager2*   g_GameEvents = nullptr;
IMoveHelper*          g_MoveHelper = nullptr;
IMDLCache*            g_MdlCache = nullptr;
IPrediction*          g_Prediction = nullptr;
CGameMovement*        g_GameMovement = nullptr;
IEngineSound*         g_EngineSound = nullptr;
CGlowObjectManager*   g_GlowObjManager = nullptr;
//IViewRender*          g_ViewRender = nullptr;
IDirect3DDevice9*     g_D3DDevice9 = nullptr;
CClientState*         g_ClientState = nullptr;
IPhysicsSurfaceProps* g_PhysSurface = nullptr;
ILocalize*            g_Localize = nullptr;
IVEffects*            g_DLight = nullptr;
DWORD*                g_SteamNetworkingUtils = nullptr;
CLobbyScreen*         g_LobbyScreen = nullptr;
CHudChat*             g_ChatElement = nullptr;

namespace csgo
{
    CreateInterfaceFn get_module_factory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }


    void initialize()
    {
        auto engineFactory     = get_module_factory(GetModuleHandleW(L"engine.dll"));
        auto clientFactory     = get_module_factory(GetModuleHandleW(L"client.dll"));
        auto valveStdFactory   = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
        auto vguiFactory       = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
        auto vgui2Factory      = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
        auto matSysFactory     = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
        auto dataCacheFactory  = get_module_factory(GetModuleHandleW(L"datacache.dll"));
        auto vphysicsFactory   = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
        auto localizefactory   = get_module_factory(GetModuleHandleW(L"localize.dll"));

        g_EngineClient = (IVEngineClient*)engineFactory("VEngineClient014", nullptr);
        g_CHLClient    = (IBaseClientDLL*)clientFactory("VClient018", nullptr);
        g_EntityList   = (IClientEntityList*)clientFactory("VClientEntityList003", nullptr);
        g_EngineTrace  = (IEngineTrace*)engineFactory("EngineTraceClient004", nullptr);
        g_CVar         = (ICvar*)valveStdFactory("VEngineCvar007", nullptr);
        g_VGuiPanel    = (IPanel*)vgui2Factory("VGUI_Panel009", nullptr);
        g_DebugOverlay = (IVDebugOverlay*)engineFactory("VDebugOverlay004", nullptr);
        g_VGuiSurface  = (ISurface*)vguiFactory("VGUI_Surface031", nullptr);
        g_MdlInfo      = (IVModelInfoClient*)engineFactory("VModelInfoClient004", nullptr);
        g_MdlRender    = (IVModelRender*)engineFactory("VEngineModel016", nullptr);
        g_RenderView   = (IVRenderView*)engineFactory("VEngineRenderView014", nullptr);
        g_MatSystem    = (IMaterialSystem*)matSysFactory("VMaterialSystem080", nullptr);
        g_GameEvents   = (IGameEventManager2*)engineFactory("GAMEEVENTSMANAGER002", nullptr);
        g_MdlCache     = (IMDLCache*)dataCacheFactory("MDLCache004", nullptr);
        g_Prediction   = (IPrediction*)clientFactory("VClientPrediction001", nullptr);
        g_GameMovement = (CGameMovement*)clientFactory("GameMovement001", nullptr);
        g_EngineSound  = (IEngineSound*)engineFactory("IEngineSoundClient003", nullptr);
        g_PhysSurface  = (IPhysicsSurfaceProps*)vphysicsFactory("VPhysicsSurfaceProps001", nullptr);
        g_Localize     = (ILocalize*)localizefactory(ILOCALIZE_CLIENT_INTERFACE_VERSION, nullptr);
        g_DLight       = (IVEffects*)engineFactory("VEngineEffects001", nullptr);
        
        g_ChatElement  = FindHudElement<CHudChat>("CHudChat");

        g_Input        = *(CInput**)((*reinterpret_cast<DWORD**>(g_CHLClient))[15] + 0x1);
        g_GlobalVars   = **(CGlobalVarsBase***)((*(DWORD**)g_CHLClient)[0] + 0x1B); 
        g_ClientMode   = **(IClientMode***)((*(DWORD**)g_CHLClient)[10] + 0x5);

        auto client = GetModuleHandleW(L"client.dll");
        auto engine = GetModuleHandleW(L"engine.dll");

        auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");
        
        static auto get_steam_networking_utils =
            reinterpret_cast<DWORD*(__stdcall*)()>(utils::pattern_scan(GetModuleHandleW(L"engine.dll"),
                "A1 ? ? ? ? A8 01 75 12 83 C8 01 C7 05 ? ? ? ? ? ? ? ? A3 ? ? ? ? B8"));
        g_SteamNetworkingUtils = get_steam_networking_utils();
        g_LobbyScreen = *(CLobbyScreen**)(utils::pattern_scan(client, "A1 ? ? ? ? 85 C0 74 0F 6A 00") + 0x1);

        //g_Input          = *(CInput**)(utils::pattern_scan(client, "B9 ? ? ? ? F3 0F 10 40 ? F3 0F 11 04 24") + 1);
        g_MoveHelper       = **(IMoveHelper***)(utils::pattern_scan(client, "8B 0D ? ? ? ? 8B 46 08 68") + 2);
        //g_GlowObjManager = *(CGlowObjectManager**)(utils::pattern_scan(client, "0F 57 C0 C7 05 ? ? ? ? 00 00 00 00 0F 11 05 ? ? ? ? 68 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? 00 00 00 00 C7 05 ? ? ? ? 00 00 00 00") + 16);
        g_GlowObjManager   = *(CGlowObjectManager**)(utils::pattern_scan(client, "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3);
        //g_ViewRender     = *(IViewRender**)(utils::pattern_scan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
        g_D3DDevice9       = **(IDirect3DDevice9***)(utils::pattern_scan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        g_ClientState      = **(CClientState***)(utils::pattern_scan(engine, "8B 3D ? ? ? ? 8A F9") + 2);

        netvars.initialize();
    }

    void cleanup()
    {
    }

    void dump_info()
    {
        utils::console_print("=========== INTERFACES =============\n");
        utils::console_print("IVEngineClient          : 0x%p\n", g_EngineClient);
        utils::console_print("IBaseClientDLL          : 0x%p\n", g_CHLClient);
        utils::console_print("IClientEntityList       : 0x%p\n", g_EntityList);
        utils::console_print("CGlobalVarsBase         : 0x%p\n", g_GlobalVars);
        utils::console_print("IEngineTrace            : 0x%p\n", g_EngineTrace);
        utils::console_print("ICvar                   : 0x%p\n", g_CVar);
        utils::console_print("IPanel                  : 0x%p\n", g_VGuiPanel);
        utils::console_print("IClientMode             : 0x%p\n", g_ClientMode);
        utils::console_print("IVDebugOverlay          : 0x%p\n", g_DebugOverlay);
        utils::console_print("ISurface                : 0x%p\n", g_VGuiSurface);
        utils::console_print("CInput                  : 0x%p\n", g_Input);
        utils::console_print("IVModelInfoClient       : 0x%p\n", g_MdlInfo);
        utils::console_print("IVModelRender           : 0x%p\n", g_MdlRender);
        utils::console_print("IVRenderView            : 0x%p\n", g_RenderView);
        utils::console_print("IMaterialSystem         : 0x%p\n", g_MatSystem);
        utils::console_print("IGameEventManager2      : 0x%p\n", g_GameEvents);
        utils::console_print("IMoveHelper             : 0x%p\n", g_MoveHelper);
        utils::console_print("IMDLCache               : 0x%p\n", g_MdlCache);
        utils::console_print("IPrediction             : 0x%p\n", g_Prediction);
        utils::console_print("CGameMovement           : 0x%p\n", g_GameMovement);
        utils::console_print("IEngineSoundClient      : 0x%p\n", g_EngineSound);
        utils::console_print("CGlowObjectManager      : 0x%p\n", g_GlowObjManager);
        //utils::console_print("CViewRender             : 0x%p\n", g_ViewRender);
        utils::console_print("IPhysicsSurfaceProps    : 0x%p\n", g_PhysSurface);
        utils::console_print("CClientState            : 0x%p\n", g_ClientState);
        utils::console_print("ILocalize               : 0x%p\n", g_Localize);
    }
}

void CClientState::ForceFullUpdate()
{
    auto fn = (void(*)(void))
        utils::pattern_scan(
            GetModuleHandleW(L"engine.dll"),
            "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85"
        );

    fn();
}
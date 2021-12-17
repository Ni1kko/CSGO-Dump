#include "sdk.hpp"
#include "csgostructs.hpp"

#include "../Helpers/Utils.hpp"

namespace Interfaces
{
    CreateInterfaceFn GetModuleFactory(const char* module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(Utils::GetModule(module), "CreateInterface"));
    }

    template<typename T>
    T* GetInterface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) {
			MessageBoxA(nullptr, (std::ostringstream{ } << "Failed to find " << szInterfaceVersion << '!').str().c_str(), "ERROR", MB_OK);
			exit(EXIT_FAILURE);
        }

        return result;
    }

    void Initialize()
    {
        auto engineFactory		= GetModuleFactory("engine.dll");
        auto clientFactory		= GetModuleFactory("client.dll");
        auto valveStdFactory	= GetModuleFactory("vstdlib.dll");
        auto vguiFactory		= GetModuleFactory("vguimatsurface.dll");
        auto vgui2Factory		= GetModuleFactory("vgui2.dll");
        auto matSysFactory		= GetModuleFactory("materialsystem.dll");
        auto dataCacheFactory	= GetModuleFactory("datacache.dll");
        auto vphysicsFactory	= GetModuleFactory("vphysics.dll");
        auto inputSysFactory	= GetModuleFactory("inputsystem.dll");
		auto matchmakingFactory = GetModuleFactory("matchmaking.dll");
		auto serverFactory		= GetModuleFactory("server.dll");
		auto studioFactory		= GetModuleFactory("studiorender.dll");

        g_CHLClient           = GetInterface<IBaseClientDLL>      (clientFactory		, "VClient018");
        g_EntityList          = GetInterface<IClientEntityList>   (clientFactory		, "VClientEntityList003");
        g_Prediction          = GetInterface<IPrediction>         (clientFactory		, "VClientPrediction001");
        g_GameMovement        = GetInterface<CGameMovement>       (clientFactory		, "GameMovement001");
        g_MdlCache            = GetInterface<IMDLCache>           (dataCacheFactory		, "MDLCache004");
        g_EngineClient        = GetInterface<IVEngineClient>      (engineFactory		, "VEngineClient014");
        g_MdlInfo             = GetInterface<IVModelInfoClient>   (engineFactory		, "VModelInfoClient004");
        g_MdlRender           = GetInterface<IVModelRender>       (engineFactory		, "VEngineModel016");
        g_RenderView          = GetInterface<IVRenderView>        (engineFactory		, "VEngineRenderView014");
        g_EngineTrace         = GetInterface<IEngineTrace>        (engineFactory		, "EngineTraceClient004");
        g_DebugOverlay        = GetInterface<IVDebugOverlay>      (engineFactory		, "VDebugOverlay004");
        g_GameEvents          = GetInterface<IGameEventManager2>  (engineFactory		, "GAMEEVENTSMANAGER002");
        g_EngineSound         = GetInterface<IEngineSound>        (engineFactory		, "IEngineSoundClient003");
        g_MatSystem           = GetInterface<IMaterialSystem>     (matSysFactory		, "VMaterialSystem080");
        g_CVar                = GetInterface<ICvar>               (valveStdFactory		, "VEngineCvar007");
        g_VGuiPanel           = GetInterface<IPanel>              (vgui2Factory			, "VGUI_Panel009");
        g_VGuiSurface         = GetInterface<ISurface>            (vguiFactory			, "VGUI_Surface031");
        g_PhysSurface         = GetInterface<IPhysicsSurfaceProps>(vphysicsFactory		, "VPhysicsSurfaceProps001");
		g_GameTypes			  = GetInterface<IGameTypes>		  (matchmakingFactory	, "VENGINE_GAMETYPES_VERSION002");
		g_ServerGameDll		  = GetInterface<IServerGameDLL>	  (serverFactory		, "ServerGameDLL005");
		g_StudioRender		  = GetInterface<IStudioRender>		  (studioFactory		, "VStudioRender026");
		g_SpatialPartition	  = GetInterface<uintptr_t>			  (engineFactory		, "SpatialPartition001");
		g_InputSystem		  = GetInterface<IInputSystem>		  (inputSysFactory		, "InputSystemVersion001");

        g_GlobalVars		= **(CGlobalVarsBase***)((*(DWORD * *)g_CHLClient)[0] + 0x1F);
        g_ClientMode		=	  **(IClientMode***)((*(DWORD * *)g_CHLClient)[10] + 0x5);

		g_Input				=			  *(CInput**)(Utils::PatternScan("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		g_MoveHelper		=      **(IMoveHelper***)(Utils::PatternScan("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
		g_GlowObjManager	= *(CGlowObjectManager**)(Utils::PatternScan("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		g_ViewRender		=        *(IViewRender**)(Utils::PatternScan("client.dll", "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
		g_D3DDevice9		= **(IDirect3DDevice9***)(Utils::PatternScan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		g_ClientState		=     **(CClientState***)(Utils::PatternScan("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		g_LocalPlayer		=	    *(C_LocalPlayer*)(Utils::PatternScan("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
		g_GameRules			=		  *(CGameRules**)(Utils::PatternScan("client.dll", "8B 0D ?? ?? ?? ?? 85 C0 74 0A 8B 01 FF 50 78 83 C0 54") + 2);

		g_ChatElement		= Utils::FindHudElement<CHudChat>("CHudChat");

		g_MemAlloc = *reinterpret_cast<IMemAlloc**>(GetProcAddress(Utils::GetModule("tier0.dll"), "g_pMemAlloc"));

		auto SteamClient = ((ISteamClient*(__cdecl*)(void))GetProcAddress(Utils::GetModule("steam_api.dll"), "SteamClient"))();
		g_SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface((void*)1, (void*)1, "SteamGameCoordinator001");
		g_SteamUser = SteamClient->GetISteamUser((void*)1, (void*)1, "SteamUser019");

		RandomSeed = reinterpret_cast<decltype(RandomSeed)>(GetProcAddress(Utils::GetModule("vstdlib.dll"), "RandomSeed"));
		RandomInt = reinterpret_cast<decltype(RandomInt)>(GetProcAddress(Utils::GetModule("vstdlib.dll"), "RandomInt"));
		RandomFloat = reinterpret_cast<decltype(RandomFloat)>(GetProcAddress(Utils::GetModule("vstdlib.dll"), "RandomFloat"));
    }

    void Dump()
    {
        #define PRINT_INTERFACE(name) Utils::ConsolePrint("%-20s: %p\n", #name, name)

        PRINT_INTERFACE(g_CHLClient    );
        PRINT_INTERFACE(g_EntityList   );
        PRINT_INTERFACE(g_Prediction   );
        PRINT_INTERFACE(g_GameMovement );
        PRINT_INTERFACE(g_MdlCache     );
        PRINT_INTERFACE(g_EngineClient );
        PRINT_INTERFACE(g_MdlInfo      );
        PRINT_INTERFACE(g_MdlRender    );
        PRINT_INTERFACE(g_RenderView   );
        PRINT_INTERFACE(g_EngineTrace  );
        PRINT_INTERFACE(g_DebugOverlay );
        PRINT_INTERFACE(g_GameEvents   );
        PRINT_INTERFACE(g_EngineSound  );
        PRINT_INTERFACE(g_MatSystem    );
        PRINT_INTERFACE(g_CVar         );
        PRINT_INTERFACE(g_VGuiPanel    );
        PRINT_INTERFACE(g_VGuiSurface  );
        PRINT_INTERFACE(g_PhysSurface  );
        PRINT_INTERFACE(g_GameTypes	   );
        PRINT_INTERFACE(g_ServerGameDll);
        PRINT_INTERFACE(g_StudioRender );
    }
}

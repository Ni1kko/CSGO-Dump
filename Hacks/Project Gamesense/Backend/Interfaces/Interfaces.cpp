#include "Interfaces.h"

#include "../Configuration/Settings.h"

using namespace Interfaces;

CInterfaces* Interfaces::InterfaceLoader = new CInterfaces();

CInterfaceManager::CInterfaceManager( std::string szModuleName )
{
	m_hCreateInterface = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleA( szModuleName.c_str() ), ("CreateInterface") );
}

template<typename T>
T* CInterfaceManager::GetInterface( std::string szInterfaceVersion )
{
	return reinterpret_cast<T*>( m_hCreateInterface( szInterfaceVersion.c_str(), nullptr ) );
}

template<typename T>
T* CInterfaceManager::BruteInterface( std::string szInterfaceVersion )
{
	char buffer[64];
	int count = 0;
	sprintf( buffer, ("%s%03i"), szInterfaceVersion.c_str(), 0 );
	auto hInterface = reinterpret_cast<T*>( m_hCreateInterface( buffer, nullptr ) );

	while ( hInterface == nullptr )
	{
		count++;

		if ( count == 1000 )
			return nullptr;

		sprintf( buffer, ("%s%03i"), szInterfaceVersion.c_str(), count );
		hInterface = reinterpret_cast<T*>( m_hCreateInterface( buffer, nullptr ) );
	}

	return hInterface;
}

void CInterfaces::Initialize()
{
	ClientManager			= new CInterfaceManager( Cheat::Settings->ClientDLL );
	EngineManager			= new CInterfaceManager( xorstr_("engine.dll") );
	VGUIMatSurfaceManager	= new CInterfaceManager( xorstr_("vguimatsurface.dll") );
	VGUI2Manager			= new CInterfaceManager( xorstr_("vgui2.dll") );
	VPhysicsManager			= new CInterfaceManager( xorstr_("vphysics.dll") );
	VSTDLibManager			= new CInterfaceManager( xorstr_("vstdlib.dll") );
	MaterialSYSManager		= new CInterfaceManager( xorstr_("materialsystem.dll") );
	VStudioRender			= new CInterfaceManager( xorstr_("studiorender.dll") );
	LocalizeManager			= new CInterfaceManager( xorstr_("localize.dll") );

	Client = ClientManager->BruteInterface<IClient>( xorstr_("VClient") );
	ModelInfo = EngineManager->BruteInterface<CModelInfo>( xorstr_("VModelInfoClient") );
	Surface = VGUIMatSurfaceManager->BruteInterface<ISurface>( xorstr_("VGUI_Surface") );
	Engine = EngineManager->BruteInterface<IEngine>( xorstr_("VEngineClient") );
	EntityList = ClientManager->BruteInterface<CClientEntityList>( xorstr_("VClientEntityList") );
	GuiPanel = VGUI2Manager->BruteInterface<CPanel>( xorstr_("VGUI_Panel") );
	GameEvents = EngineManager->GetInterface<IGameEventManager>( xorstr_("GAMEEVENTSMANAGER002") );
	ModelRender = EngineManager->BruteInterface<CModelRender>( xorstr_("VEngineModel") );
	Trace = EngineManager->BruteInterface<CTrace>( xorstr_("EngineTraceClient") );
	Var = VSTDLibManager->BruteInterface<ICvar>( xorstr_("VEngineCvar") );
	MaterialSystem = MaterialSYSManager->BruteInterface<IMaterialSystem>( xorstr_("VMaterialSystem") );
	RenderView = EngineManager->BruteInterface<CRenderView>( xorstr_("VEngineRenderView") );
	DebugOverlay = EngineManager->BruteInterface<IVDebugOverlay>( xorstr_("VDebugOverlay") );
	EngineSound = EngineManager->BruteInterface<void>( xorstr_("IEngineSoundClient") );
	PhysSurface = VPhysicsManager->BruteInterface<IPhysicsSurfaceProps>( xorstr_("VPhysicsSurfaceProps") );
	MemAlloc = *(IMemAlloc**)(GetProcAddress(GetModuleHandle(xorstr_("tier0.dll")), xorstr_("g_pMemAlloc")));
	Interfaces::GameMovement = ClientManager->BruteInterface<IGameMovement>(xorstr_("GameMovement"));
	Input = *( CInput** )( Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, ( xorstr_("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") ) ) + 1 );
	//GameRules = *(IGameRules***)(Cheat::Utilities->Memory_PatternScan(Cheat::Settings->ClientDLL, (xorstr_("E8 ? ? ? ? A1 ? ? ? ? 85 C0 0F 84 ? ? ? ?"))) + 0x6);
	Globals = **( CGlobalVars*** )( ( *( DWORD** )( Client ) )[0] + 0x1F );
	ClientMode = **( IClientMode*** )( ( *( DWORD** )Client )[10] + 0x5 );
	ClientState = **( CClientState*** )( ( *reinterpret_cast<uintptr_t**>( Engine ) )[12] + 0x10 );
	Interfaces::Prediction = ClientManager->BruteInterface<IPrediction>(xorstr_("VClientPrediction"));
	GlowObjectManager = *( CGlowObjectManager** )( Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") ) + 3 );
	RenderBeams = *( IViewRenderBeams** )( Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") ) + 1 );
	Interfaces::MoveHelper = **reinterpret_cast<IMoveHelper***>(Cheat::Utilities->Memory_PatternScan(xorstr_("client.dll"), xorstr_("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01")) + 2);
	Localize = LocalizeManager->BruteInterface<ILocalize>(xorstr_("Localize_"));
	HMODULE h_vstdlib = GetModuleHandleA(xorstr_("vstdlib.dll"));
	RandomSeed = (decltype(RandomSeed))GetProcAddress(h_vstdlib, xorstr_("RandomSeed"));
	RandomInt = (decltype(RandomInt))GetProcAddress(h_vstdlib, xorstr_("RandomInt"));
}

void CInterfaces::Free()
{
	delete ClientManager, EngineManager, VGUIMatSurfaceManager, VGUI2Manager, VPhysicsManager, VSTDLibManager, MaterialSYSManager, VStudioRender, LocalizeManager;
}

IEngine* Interfaces::Engine = nullptr;
IClient* Interfaces::Client = nullptr;
CPanel*  Interfaces::GuiPanel = nullptr;
CClientEntityList* Interfaces::EntityList = nullptr;
CModelInfo* Interfaces::ModelInfo = nullptr;
IMoveHelper* Interfaces::MoveHelper = nullptr;
IPrediction* Interfaces::Prediction = nullptr;
IGameMovement* Interfaces::GameMovement = nullptr;
ISurface* Interfaces::Surface = nullptr;
IClientMode* Interfaces::ClientMode = nullptr;
IGameEventManager* Interfaces::GameEvents = nullptr;
CModelRender* Interfaces::ModelRender = nullptr;
CGlowObjectManager* Interfaces::GlowObjectManager = nullptr;
CGlobalVars* Interfaces::Globals = nullptr;
CInput* Interfaces::Input = nullptr;
CTrace* Interfaces::Trace = nullptr;
ICvar* Interfaces::Var = nullptr;
IMaterialSystem* Interfaces::MaterialSystem = nullptr;
CRenderView* Interfaces::RenderView = nullptr;
IVDebugOverlay* Interfaces::DebugOverlay = nullptr;
IViewRenderBeams* Interfaces::RenderBeams = nullptr;
void* Interfaces::EngineSound = nullptr;
IPhysicsSurfaceProps* Interfaces::PhysSurface = nullptr;
CClientState* Interfaces::ClientState = nullptr;
ILocalize* Interfaces::Localize = nullptr;
IMemAlloc *Interfaces::MemAlloc = nullptr;
//IGameRules** Interfaces::GameRules = nullptr;
decltype(Interfaces::RandomSeed) Interfaces::RandomSeed = nullptr;
decltype(Interfaces::RandomInt) Interfaces::RandomInt = nullptr;
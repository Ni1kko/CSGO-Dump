#include "interfaces.h"

CInterfaces g_pInterfaces;

HMODULE CInterfaces::GetModuleHandleSafe( const char* pszModuleName )
{
	HMODULE hmModuleHandle = NULL;

	do
	{
		hmModuleHandle = GetModuleHandle( pszModuleName );
		Sleep( 1 );
	} while( hmModuleHandle == NULL );

	return hmModuleHandle;
}

void* CInterfaces::GetInterfaces( char* szBaseName, const char* interfaceName, const char* ptrName, CreateInterface_t pInterface )
{
	char szDebugString[ 1024 ];
	std::string sinterface = "";
	std::string interfaceVersion = "0";

	for( int i = 0; i <= 99; i++ )
	{
		sinterface = interfaceName;
		sinterface += interfaceVersion;
		sinterface += std::to_string( i );

		void* funcPtr = pInterface( sinterface.c_str(), NULL );

		if( ( DWORD )funcPtr != 0x0 )
		{
			return funcPtr;
		}
		if( i >= 99 && interfaceVersion == "0" )
		{
			interfaceVersion = "00";
			i = 0;
		}
		else if( i >= 99 && interfaceVersion == "00" )
		{
			sprintf( szDebugString, "%s: 0x%x (error)", ptrName, ( DWORD )funcPtr );
			printf( szDebugString );
			printf( "\n" );
		}
	}
	return 0;
}

template< class T > T* GetGameEventInterface( std::string szModuleName, std::string szInterfaceName, bool bSkip = false )
{
	if( szModuleName.empty() || szInterfaceName.empty() )
		return nullptr;
	typedef PVOID( *CreateInterfaceFn )( const char* pszName, int* piReturnCode );
	CreateInterfaceFn hInterface = nullptr;
	while( !hInterface )
	{
		hInterface = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleA( szModuleName.c_str() ), "CreateInterface" );
		Sleep( 5 );
	}

	char pszBuffer[ 256 ];
	for( int i = 0; i < 100; i++ )
	{
		sprintf_s( pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i );
		PVOID pInterface = hInterface( pszBuffer, nullptr );

		if( pInterface && pInterface != NULL )
		{
			if( bSkip )
				sprintf_s( pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1 );

			Sleep( 5 );
			break;
		}
	}

	return ( T* )hInterface( pszBuffer, nullptr );
}

/*template<class T>
static T* FindHudElement( const char* name )
{
	static auto pThis = *reinterpret_cast<DWORD**>( g_Pattern.PatternSearchIDA( "client.dll", "B9 ? ? ? ? E8 ? ? ? ? 83 7D F8 ?" ) + 1 );

	static auto find_hud_element = reinterpret_cast<DWORD( __thiscall* )( void*, const char* )>( g_Pattern.PatternSearchIDA( "client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
	return ( T* ) find_hud_element( pThis, name );
}*/

void CInterfaces::InitInterfaces()
{
	CreateInterface_t EngineFactory = NULL;
	CreateInterface_t ClientFactory = NULL;
	CreateInterface_t VGUIFactory = NULL;
	CreateInterface_t VGUI2Factory = NULL;
	CreateInterface_t PhysicsFactory = NULL;
	CreateInterface_t VSTFactory = NULL;
	CreateInterface_t MaterialSystemFactory = NULL;
	CreateInterface_t InputSystemFactory = NULL;
	CreateInterface_t StudioRenderFatory = NULL;

	dwClient = ( DWORD ) GetModuleHandle( g_Offset.szClient.c_str() );

	dwVGUIMAT = ( DWORD )GetModuleHandle( "vguimatsurface.dll" );
	dwVGUI2 = ( DWORD )GetModuleHandle( "vgui2.dll" );
	dwEngine = ( DWORD )GetModuleHandle( "engine.dll" );

	ClientFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( g_Offset.szClient.c_str() ), "CreateInterface" );
	EngineFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "engine.dll" ), "CreateInterface" );
	VGUIFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "vguimatsurface.dll" ), "CreateInterface" );
	VGUI2Factory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "vgui2.dll" ), "CreateInterface" );
	PhysicsFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "vphysics.dll" ), "CreateInterface" );
	VSTFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "vstdlib.dll" ), "CreateInterface" );
	MaterialSystemFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "materialsystem.dll" ), "CreateInterface" );
	InputSystemFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleSafe( "inputsystem.dll" ), "CreateInterface" );
	StudioRenderFatory = ( CreateInterfaceFn ) GetProcAddress( GetModuleHandleSafe( "studiorender.dll" ), "CreateInterface" );

	g_pMemAlloc = *( IMemAlloc** ) ( GetProcAddress( GetModuleHandleSafe( "tier0.dll" ), "g_pMemAlloc" ) );

	g_pClient = ( HLCLient* )GetInterfaces( "g_pClient", "VClient", "g_pClient", ClientFactory );
	g_pEntList = ( VClientEntityList* )GetInterfaces( "g_pBaseEntityList", "VClientEntityList", "g_pEntList", ClientFactory );
	g_pPred = ( CPrediction* )GetInterfaces( "g_pPrediction", "VClientPrediction", "g_pPred", ClientFactory );
	g_pGameMovement = ( CGameMovement* )GetInterfaces( "g_pGameMovement", "GameMovement", "g_pGameMovement", ClientFactory );

	g_pEngine = ( VEngineClient* )GetInterfaces( "g_pEngine", "VEngineClient", "g_pEngine", EngineFactory );
	g_pModel = ( VModelInfoClient* )GetInterfaces( "g_pModel", "VModelInfoClient", "g_pModel", EngineFactory );
	g_pDebugOverlay = ( VDebugOverlay* )GetInterfaces( "g_pDebugOverlay", "VDebugOverlay", "g_pDebugOverlay", EngineFactory );
	g_pEngineTrace = ( CEngineTraceClient* )GetInterfaces( "g_pEngineTrace", "EngineTraceClient", "g_pEngineTrace", EngineFactory );
	g_pEffects = ( IEffects* )GetInterfaces( "g_pEffects", "VEngineEffects", "g_pEffects", EngineFactory );
	g_pEngineModel = ( VEngineModel* )GetInterfaces( "g_pEngineModel", "VEngineModel", "g_pEngineModel", EngineFactory );
	g_pEngineSound = ( IEngineSoundClient* )GetInterfaces( "g_pEngineSound", "IEngineSoundClient", "g_pEngineSound", EngineFactory );
	g_pRenderView = ( IVRenderView* )GetInterfaces( "g_pRenderView", "VEngineRenderView", "g_pRenderView", EngineFactory );

	g_pPhysics = ( VPhysicsSurfaceProps* )GetInterfaces( "g_pPhysics", "VPhysicsSurfaceProps", "g_pPhysics", PhysicsFactory );

	g_pSurface = ( VGUI_Surface* )GetInterfaces( "g_pSurface", "VGUI_Surface", "g_pSurface", VGUIFactory );
	g_pPanel = ( VGUI_Panel* )GetInterfaces( "g_pPanel", "VGUI_Panel", "g_pPanel", VGUI2Factory );

	g_pMatsystem = ( IMaterialSystem* )GetInterfaces( "g_pMaterialSystem", "VMaterialSystem", "g_pMatsystem", MaterialSystemFactory );

	g_pICvar = ( ICVar* )GetInterfaces( "g_pICvar", "VEngineCvar", "g_pICvar", VSTFactory );

	g_pInputSystem = ( IInputSystem* )GetInterfaces( "g_pInputSystem", "InputSystemVersion", "g_pInputSystem", InputSystemFactory );

	g_pGlobalVars = ( CGlobalVars* )*( PDWORD )*( PDWORD )( g_Pattern.PatternSearch( g_Offset.szClient.c_str(), ( PBYTE )"\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04", "x????xxxxxx", NULL, NULL ) + 0x1 );

	g_pStudioRender = ( IStudioRender* )GetInterfaces( "g_pStudioRender", "VStudioRender", "g_pStudioRender", StudioRenderFatory );

	g_pRenderBeams = *( IViewRenderBeams** )( ( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ) ) + 0x1 );

	g_pGameEventManager2 = GetGameEventInterface< IGameEventManager2 >( "engine.dll", "GAMEEVENTSMANAGER", true );

	//Failed to execute event for classId
	g_pClientState = **( CBaseClientState*** ) ( ( *( uintptr_t** ) ( g_pEngine ) ) [ 12 ] + 0x10 );

	g_pInput = *( CInput** )( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 1 );

//	g_pHudChat = FindHudElement<CHudChat>( "CHudChat" );
}
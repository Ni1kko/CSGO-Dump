#include "main.h"
#include "weaponicon.h"
#include "IsEntBreakableHook.hpp"
// /Zc:threadSafeInit- DISABLE TLS & ENABLE MT FOR 2k15

std::string GetSystemFolderPaths(int csidl)
{
	wchar_t Folder[1024];
	HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
	if (SUCCEEDED(hr))
	{
		char str[1024];
		wcstombs(str, Folder, 1023);
		return str;
	}
	else return "";
}

std::string GetTemp()
{
	char buffer[MAX_PATH];
	GetTempPathA(MAX_PATH, buffer);
	std::string str = std::string(buffer);
	return str;
}

void InitAll( LPARAM lpParam )
{
//	g_pDebug.AttachDebugConsole();

	while( !GetModuleHandleA( "serverbrowser.dll" ) )
		Sleep( 50 );

	DWORD dwFonts;
	AddFontMemResourceEx( csgo_icons_raw, sizeof( csgo_icons_raw ), NULL, &dwFonts );

    g_Offset.init();

	g_pInterfaces.InitInterfaces();

	g_pNetVars = new CNetVars();
	g_pEvents = new CEvents();
	Offsets::Init();

//	g_Visuals.InitMaterials();

	g_Hooks.InitHooks();

	g_Draw.InitFonts();

	g_pEvents->Registerself();

/*	std::ofstream output; output.open( "c:/kidua/classids.txt", std::ofstream::out | std::ofstream::app );
	output << "enum ClientClassIDs" << std::endl << "{" << std::endl;

	for( CClientClass* pClass = g_pClient->AllClasses(); pClass; pClass = pClass->m_pNext )
	{
		output << "\t" << pClass->m_pNetworkName << " = " << pClass->m_ClassID << "," << std::endl;
	}

	output << "};";
	output.close();*/

//	if( !exists( "c:/kidua/" ) )
//		_mkdir( "c:/kidua/" );

	g_ConfigFile.LoadConfig();
	g_ConfigFile.LoadSkinConfig();

	GetTemp().c_str();
	GetSystemFolderPaths(CSIDL_MYDOCUMENTS).c_str();

	g_pEngine->ExecuteClientCmd( "clear" );

	g_pICvar->ConsoleColorPrintf( Color( g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ] ), "[ KIDUAHOOK ] " );

	if( g_Offset.bIsPanorama )
		g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), "Panorama Build: " __DATE__ " - " __TIME__  "\n" );
	else
		g_pICvar->ConsoleColorPrintf( Color( 255, 255, 255 ), "Binary Build: " __DATE__ " - " __TIME__  "\n" );
	
	CEventLogs::sLog EventLog;

	EventLog.EventText.PushBack( "Binary Build: " __DATE__ " - " __TIME__, Color( 255, 255, 255 ) );

	g_EventLogs.AddLog( EventLog );
}

void CloseAll( LPARAM lpParam )
{
	HMODULE hModule = ( HMODULE )lpParam;
	while( true )
	{
		if( GetAsyncKeyState( VK_MULTIPLY ) & 1 && GetForegroundWindow() == FindWindowA( NULL, "Counter-Strike: Global Offensive" ) )
		{
			Sleep( 1000 );
			SetWindowLongPtr( hwndWindow, GWLP_WNDPROC, ( LONG_PTR ) OldWindowProc );
		/*    g_pClientModeHOOKED->UnHook();
			g_pRunCommandHOOKED->UnHook();
			g_pEngineHOOKED->UnHook();
			g_pClientHOOKED->UnHook();*/
			g_pPaintTraverseHOOKED->UnHook();
			g_pSurfaceHOOKED->UnHook();
		/*	g_pDrawModelHOOKED->UnHook();
			g_pStudioRenderHOOKED->UnHook();
			g_pMaterialSystemHOOKED->UnHook();
			g_pRenderViewHOOKED->UnHook();
			g_pGameEventHOOKED->UnHook();
			g_pEngineTraceHOOKED->UnHook();
			IsEntBreakableHook::RemoveHook();*/
			Sleep( 2000 );

			FreeConsole();
			FreeLibraryAndExitThread( hModule, 0 );
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hModule );
		CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )InitAll, 0, 0, 0 );
	//	CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )CloseAll, 0, 0, 0 );
		break;
	}

	return TRUE;
}
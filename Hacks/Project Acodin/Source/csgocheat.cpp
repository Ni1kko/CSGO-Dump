#include "stdafx.h"
#include "Tools/Hacks/Misc.h"
#include "Tools\Menu\Menu.h"

extern void savesets();

extern std::string settingstostring();

bool Unhook_Cheat = false;

auto ExitCheat() -> bool;

Variables Vars;

void Unhook()
{
	Unhook_Cheat = true;
}

//void SaveSettings()
//{
//	settingstostring();
//}
//
//void LoadSettings()
//{
//	savesets();
//}

bool ExitCheat()
{
	if( !Unhook_Cheat )
		return false;

	Interfaces.pEngine->ClientCmd_Unrestricted("clear", 0);
	Interfaces.pEngine->ClientCmd_Unrestricted( "crosshair 1", 0 );
	Interfaces.pEngine->ClientCmd_Unrestricted( "firstperson", 0 );

	if( Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() )
	{
		Misc::SetClanTag( "", Hacks.LocalPlayer->GetName().c_str() );
	}

	Hacks.UnHook();
	SetWindowLongPtr( INIT::Window, GWL_WNDPROC, reinterpret_cast< LONG_PTR >( INIT::OldWindow ) );
	FreeLibraryAndExitThread( INIT::Dll, 0 );
}

DWORD WINAPI CheatMain(LPVOID lpThreadParameter)
{
	while (!(INIT::Window = FindWindowA("Valve001", nullptr)))
		Sleep(200);

	Interfaces.InitialiseSDK();

	CNetvarManager::Instance()->CreateDatabase();
	Hacks.Hook();

	Interfaces.pEngine->ClientCmd_Unrestricted("clear", 0);

	/*while (!Unhook_Cheat)
	{
		Sleep(5);
	}

	ExitCheat();*/
	return 0;
}

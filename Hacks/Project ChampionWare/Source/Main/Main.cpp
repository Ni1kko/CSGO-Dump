#include "Main.h"
#include "../RL/ReflectiveLoader.h"


//[enc_string_enable /]
//[junk_enable /]
DWORD WINAPI CheatEntry(LPVOID lpThreadParameter)
{
	HMODULE hModule = (HMODULE)lpThreadParameter;

	if (Engine::Initialize())
	{
		return 0;
	}

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		/*Beep(659.26, 200);
		Beep(659.26, 200);
		Sleep(200);
		Beep(659.26, 200);
		Sleep(100);
		Beep(523.26, 200);
		Beep(659.26, 200);
		Sleep(200);
		Beep(783.98, 200);
		Sleep(400);
		Beep(391.99, 200); */

		DisableThreadLibraryCalls(hinstDLL);

		Client::BaseDir = CSX::Utils::GetHackWorkingDirectory().c_str();

		if (Client::BaseDir.size() < 1)
		{
			Client::BaseDir = CSX::Utils::GetModuleBaseDir(hinstDLL);
		}



		CreateThread(0, 0, CheatEntry, hinstDLL, 0, 0);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		Engine::Shutdown();
	}

	return TRUE;
}












































































































































































#include "Includes.h"
#include "Triggerbot.h"
#include "HackProcess.h"
#include "CheatManager.h"
#include "ReadProcessMemory.h"
#include "Aimbot.h"
#include "GlowEsp.h"
#include <stdio.h>
#include "Misc.h"
InitMemory Init;
Offsets O;
CheatManager CM;
MemoryRead Read;
Triggerbot Tr;
Aimbot Am;
Glow G;
Misc Mi;
using namespace std;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*IF YOU USE THE INTERNAL/EXTERNAL VERSION FOLLOW THESE STEPS: //////////////
  - go to CheatManager.cpp                                     //////////////
  - Change the path to the path where you placed Offsets.ini   //////////////
  - Example: Hardrive = "C:\\Offsets.ini"                      //////////////
  IF YOU USE THE FULLY EXTERNAL VERSION:                       //////////////
  - Place Offsets.ini in the same folder where your cheat is   //////////////
  - go to CheatManager.cpp                                     //////////////
  - Change the path to Offsets.ini to "./Offsets.ini"          //////////////
*////////////////////////////////////////////////////////////////////////////


int main(void)
{
	Init.StartMemory();
	CM.InitializeCheat();
	cout << endl << endl << endl << endl << "PROCESS FOUND" << endl;
	while (!CM.Shutdown)
	{
		Mi.RenderChange();
		Mi.RadarHack();
		Mi.RankReveal();
		Mi.Bhop();
		CM.Options();
		G.GlowEsp();
		Sleep(1);
		Read.GetAll();
		Tr.TriggerBot(0x45);
		Am.Run();
	}
	FreeConsole();
	return 0;
}

int EntryThread(void)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	return main();
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)EntryThread, 0, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


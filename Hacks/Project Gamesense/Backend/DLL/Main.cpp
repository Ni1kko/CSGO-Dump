#include "../Include/GlobalIncludes.h"
#include "../Utility/Utilities.h"
#include "../Configuration/Settings.h"
#include "../Interfaces/Interfaces.h"
#include "../Configuration/Config.h"
#include "../Features/EventListener.h"
#include "../Security/Security.h"
#include "../Features/Visuals/EventLogger.h"
#include "../DLL/HWID/HWID.h"
#include "../lazyimports.h"
#include "../_fixes/anticrash.h"

#include "Engine.h"
#include <fstream>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <string>

#include "Client/main.h"
#include "../../Other/fonts.h"

#include "../Security/base64.h"
#include "../Security/md5.h"
#include "../Hashing/xorstr.h"

#include "../Lua/CLua.h"
#include "../Features/skinchanger/cskin.h"
#include "../Features/Skinchanger/skinchanger2.h"

using namespace std::chrono_literals;

void __stdcall MainThread(LPVOID lpParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved);

void quit(int) {
	Cheat::Settings->SocketConnection.Close();
	PostQuitMessage(0);
}

std::string ssprintf(const char* text, ...) {
	char bf[4096];

	va_list ap;
	va_start(ap, text);
	vsprintf(bf, text, ap);
	va_end(ap);

	return std::string(bf);
}

__forceinline std::vector<std::string> PseudoCode(size_t length = 0) {

	std::vector<std::string> a;
	a.push_back("abcdefghijklmnopqrstuvwxyz");

	HMODULE hMods[1024];
	int procID = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_stricmp(entry.szExeFile, xorstr_("x32dbg.exe")) == 0)
				procID = entry.th32ProcessID;
		}
	}

	CloseHandle(snapshot);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	DWORD cbNeeded;
	HMODULE hModArr[1024] = { 0 };

	if (K32EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			if (K32GetModuleFileNameExA(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{

				if (strstr(szModName, xorstr_("kernel.32")))
					;
			}
		}
	}

	float sinus = sin(423);
	sinus *= 2;
	pow(sinus, 44);
	return a;
}

void __stdcall ProtectionThread(LPVOID lpParam) {
	while (1) {
		PseudoCode();
		if (FindWindowA(0, xorstr_("IDA v7.0.170914")) || FindWindowA(0, xorstr_("x64dbg")) || FindWindowA(0, xorstr_("Scylla x64 v0.9.8")) || FindWindowA(0, xorstr_("IAT Autosearch"))) {
			LI_FN(TerminateProcess).get()(LI_FN(GetCurrentProcess).get()(), 0);
			PseudoCode();
			quit(0);
			PseudoCode();
			BYTE* trash = (BYTE*)("0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBEEF0x0DEADBE");
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), MainThread, trash, 1024, 0);
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), DllMain, trash, 1024, 0);
			LI_FN(WriteProcessMemory).get()(GetCurrentProcess(), GetAsyncKeyState, trash, 1024, 0);
		}
		PseudoCode();
	}
}

__forceinline void antibot() {

}

__forceinline void shitasscode() {
	
}

__forceinline void retard() {
	
}

void __stdcall MainThread(LPVOID lpParam)
{
	//LI_FN(SetUnhandledExceptionFilter).get()(&My_UnhandledExceptionFilter);
	//Cheat::Utilities->Console_Create("skeetsu");

	Cheat::Settings->LoadDefaults();
	c_config::get()->_load_defaults();
	c_config::get()->load_keys();

	std::string cmdln = LI_FN(GetCommandLineA).get()();
	if (cmdln.find("-d3d9ex") != std::string::npos) {
		LI_FN(MessageBoxA).get()(0, "Cannot run source with -d3d9ex parameter. Please, remove -d3d9ex parameter from CS:GO launch options and try again.", "Fatal error", MB_APPLMODAL | MB_OK | MB_ICONERROR);
		quit(0);
	}

	//CopyFile("C:\\source\\undefeated.ttf", "C:\\Windows\\Fonts\\undefeated.ttf", 0);
	AddFontResource("C:\\source\\undefeated.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	LoadLibraryA(xorstr_("mscorlib.dll"));

	Cheat::Settings->PanoramaMode = true;

	Interfaces::InterfaceLoader->Initialize();
	/*if (Cheat::Settings->ShowConsole)
		Cheat::Utilities->Console_Create(xorstr_("source BETA"));
	//LoadLibraryA(xorstr_("mscorlib.dll"));

	if (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
	{
		Cheat::Utilities->Console_Log(xorstr_("Waiting for CS:GO..."));

		while (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
			Sleep(1000);
	}

	Cheat::Utilities->Console_Log(xorstr_("source %i.%i build %i"),
		Cheat::Settings->VersionMajor,
		Cheat::Settings->VersionMinor,
		Cheat::Settings->VersionBuild);

	if (Cheat::Settings->UIDevMode)
		Cheat::Utilities->Console_Log(xorstr_("ImGui development mode! Only ImGUI was loaded..."));

	if (LI_FN(GetModuleHandleA).get()(xorstr_("client.dll")))
		Cheat::Settings->PanoramaMode = true;

	if (!Cheat::Settings->UIDevMode)
	{
		Interfaces::InterfaceLoader->Initialize();
	}*/
	CreateDirectory("C:/source/", NULL);
	CreateDirectory("C:/source/cfg/", NULL);
	CreateDirectory("C:/source/lua/", NULL);
	Cheat::wrapgameoverlay();
	Cheat::Initialize();

	c_lua::get()->initialize();

/*	Interfaces::Engine->ClientCmd_Unrestricted(xorstr_("clear"));
	LI_FN(Sleep).get()(500);
	Cheat::Utilities->Game_Msg((std::string(xorstr_("Welcome ")) + Cheat::Settings->UserName).c_str());*/

    //SkinChanger::setup_skins();
	//skins.skinChanger[WEAPON_KNIFE] = {};
	//auto knife_skin = &skins.skinChanger[WEAPON_KNIFE];
	//knife_skin->itemId = WEAPON_KNIFE;
	//knife_skin->definition_override_index = ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY;
	//knife_skin->paintKit = 418;
	//knife_skin->quality = 3;
	//knife_skin->wear = 0.01f;
	//knife_skin->stat_trak = 1337;
	//knife_skin->seed = 1;

	//auto glock_skin = &skins.skinChanger[WEAPON_GLOCK];
	//glock_skin->itemId = WEAPON_GLOCK;
	//glock_skin->paintKit = 353;
	//glock_skin->wear = 0.40f;
	//glock_skin->stat_trak = 228;
	//glock_skin->seed = 1;

	while (true)
	{
		if (Cheat::Settings->UnloadCheat)
		{
			bool _break = true;
			static size_t m_count = sizeof(Cheat::Settings->UnloadReadyTable) / sizeof(int);
			for (size_t i = 0; i < m_count; ++i)
			{
				int unload_state = ((int*)&Cheat::Settings->UnloadReadyTable)[i];
				bool can_unload = (unload_state == 0) || (((int)GetTickCount() - unload_state) >= 1000);
				if (!can_unload)
				{
					_break = false;
					break;
				}
			}
			if(_break)
				break;
		}
		LI_FN(Sleep).get()(1000);
	}

	Cheat::Unload();
	Cheat::unwrapgameoverlay();
	Interfaces::InterfaceLoader->Free();
	Cheat::Utilities->Console_Close();
	LI_FN(SetUnhandledExceptionFilter).get()(nullptr);
	LI_FN(FreeLibraryAndExitThread).get()((HMODULE)lpParam, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	if (dwCallReason == DLL_PROCESS_ATTACH) {
		LI_FN(CreateThread).get()(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
		return TRUE;
	}
	return FALSE;
}

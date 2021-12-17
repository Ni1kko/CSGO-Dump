#include <stdio.h>
#include <string>
#include <iostream>
#include <ctime>

#include "includes.hpp"
#include "AutisticImageL0ader.h"
#include "resource.h"
#include <wchar.h>
#include <thread>
#include <webapplication.h>
#include <fstream>

#define OWO 1337

using namespace std;
bool pCodenz;

//DON'T change things here!

float licenseyear = 2018;
float licensemonth = 5;
float licenseday = 9; //8 oder 9
const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 1800;
//DON'T change things here!
// Returns true if
// given year is valid.
bool isLeap(int year)
{
	// Return true if year
	// is a multiple pf 4 and
	// not multiple of 100.
	// OR year is multiple of 400.
	return (((year % 4 == 0) &&
		(year % 100 != 0)) ||
		(year % 400 == 0));
}
UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

bool unload = false;
HINSTANCE HThisModule;
bool DoUnload;
void ErasePE()
{
	//removed
}
bool isValidDate(int d, int m, int y)
{
	// If year, month and day
	// are not in given range
	if (y > MAX_VALID_YR ||
		y < MIN_VALID_YR)
		return false;
	if (m < 1 || m > 12)
		return false;
	if (d < 1 || d > 31)
		return false;

	// Handle February month
	// with leap year
	if (m == 2)
	{
		if (isLeap(y))
			return (d <= 29);
		else
			return (d <= 28);
	}

	// Months of April, June,
	// Sept and Nov must have
	// number of days less than
	// or equal to 30.
	if (m == 4 || m == 6 ||
		m == 9 || m == 11)
		return (d <= 30);

	return true;
}
//DWORD WINAPI OnDllAttach(PVOID base) {
//	materials::get().setup();
//#ifdef _DEBUG
//	fridge::AttachConsole();
//#endif
//	fridge::ConsolePrint("Initializing...\n");
//	render::get().setup();
//	cfg_manager->setup();
//	setup_main_menu();
//	netvars::get().init();
//	hooks::load();
//	auto logodraw = c_texture(SUBLIMINAL, 300, 300);
//	logodraw.paint(100, 100);
//	fridge::ConsolePrint("Finished Initializing!\n");
//	while (!unload) {
//		using namespace literals::chrono_literals;
//		this_thread::sleep_for(1s);
//	}
//	fridge::ConsolePrint("Uninjecting...!\n");
//	this_thread::sleep_for(chrono::milliseconds(100));
//	hooks::shutdown();
//	this_thread::sleep_for(chrono::milliseconds(100));
//	fridge::ConsolePrint("Uninjected!\n");
//	this_thread::sleep_for(chrono::milliseconds(100));
//	fridge::DetachConsole();
//	this_thread::sleep_for(chrono::milliseconds(100));
//	fclose((FILE*)stdin);
//	fclose((FILE*)stdout);
//	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
//}

DWORD WINAPI OnDllAttach(PVOID base)
{

	materials::get().setup();

	render::get().setup();

	cfg_manager->setup();

	setup_main_menu();

	netvars::get().init();

	//NetvarHook();

	hooks::load();

	while (!unload) {
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(1s);
	}

	hooks::shutdown();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	fclose((FILE*)stdin);
	fclose((FILE*)stdout);
	FreeConsole();

	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	std::time_t result = std::time(nullptr);
	if (dwReason == DLL_PROCESS_ATTACH) {
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
		/////////////////////////////////////////////////////

		static int MaxYear = 0; // 1016;
		static int MaxMonth = 0; // 5;
		static float MaxDay = 0; // 15.5f;
		static int nineteenhundred = 0; // 950;

		//change values HERE!!!!!!
		MaxYear = 1019;
		MaxMonth = 3;
		MaxDay = 15.5f;
		nineteenhundred = 950;

		licensemonth = 10;
		licenseyear = MaxYear;
		licenseyear += 1000;
		licenseyear += 2;
		licensemonth--;
		licenseday = 31;
		//removed protection so you pasters dont scream EEEEEEEEEEEEEEEEEEEE ITS NOT WORKING FAKING BAD RELEASED -REP
		//and yes it was a date protection (if the dll is older than 15 days you coudnt inject the dll bc bad date)
		g_ctx.m_globals.lpReserved = hModule;
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, NULL, OnDllAttach, hModule, NULL, nullptr);
	}
	return TRUE;
}
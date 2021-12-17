#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>

#include "..//valve_sdk/misc/datamap.hpp"

namespace Utils {
	int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);

	unsigned int FindInDataMap(datamap_t * pMap, const char * name);

	void AttachConsole();

    void DetachConsole();

    bool ConsolePrint(const char* fmt, ...);
    
    char ConsoleReadKey();

	HMODULE GetModule(const char* name);

    std::uint8_t* PatternScan(const char* module_name, const char* signature);

    void SetClantag(const char* tag);

    void SetName(const char* name);

    void RankRevealAll();

	bool IsDangerZone();

	template<class T>
	static T* FindHudElement(const char* name)
	{
		static auto pThis = *reinterpret_cast<DWORD**>(PatternScan("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 0x1);
		static auto pFindHudElement = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(PatternScan("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
		return reinterpret_cast<T*>(pFindHudElement(pThis, name));
	}
}

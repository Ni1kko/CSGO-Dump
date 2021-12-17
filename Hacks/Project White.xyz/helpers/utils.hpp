#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "../sdk/sdk.hpp"

namespace Utils {
	unsigned int FindInDataMap(datamap_t* pMap, const char* name);
	void AttachConsole();
	void DetachConsole();
	bool ConsolePrint(const char* fmt, ...);
	char ConsoleReadKey();
	int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);
	std::uint8_t* PatternScan(void* module, const char* signature);
	bool LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
	void SetClantag(const char* tag);
	void SetName(const char* name);
	void RankRevealAll();
	float AngleDiff(float destAngle, float srcAngle);
}

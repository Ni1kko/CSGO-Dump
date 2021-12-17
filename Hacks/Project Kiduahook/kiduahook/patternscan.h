#pragma once

#include "main.h"

class CPatternScan
{
public:
	ULONG PatternSearch( std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode );
	DWORD PatternSearchIDA( std::string moduleName, std::string pattern );
};

extern CPatternScan g_Pattern;
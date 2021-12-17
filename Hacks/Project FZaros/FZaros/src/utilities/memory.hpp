#pragma once
#include <string>

class CMemoryUtils
{
public:
	unsigned long findPattern(const std::string &moduleName, const std::string &pattern);

	unsigned long findPattern2(const char* module_name, const unsigned char* mask, const char* mask_string);

	std::uint8_t* pattern_scan(void* module, const char* signature);

};

extern CMemoryUtils *g_Memory;
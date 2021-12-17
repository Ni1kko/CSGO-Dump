#include <windows.h>
#include <psapi.h>

#include "memory.hpp"

#define IS_IN_RANGE(value, min_, max_)	((value >= min_) && (value <= max_))
#define GET_BITS(data)					(IS_IN_RANGE((data & ~32), 'A', 'F') ? ((data & ~32) - 'A' + 10) : (IS_IN_RANGE(data, '0', '9') ? data - '0' : 0))
#define GET_BYTE(data)					((GET_BITS(data[0]) << 4) | GET_BITS(data[1]))

unsigned long CMemoryUtils::findPattern(const std::string &moduleName, const std::string &pattern)
{
	const char *patternPtr = pattern.c_str();
	unsigned long firstMatch = 0;

	HMODULE moduleHandle = GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo;

	GetModuleInformation(GetCurrentProcess(), moduleHandle, &miModInfo, sizeof(MODULEINFO));

	HMODULE moduleEnd = moduleHandle + miModInfo.SizeOfImage;
	for (unsigned int pCur = (unsigned int)moduleHandle; pCur < (unsigned int)moduleEnd; pCur++)
	{
		if (!*patternPtr)
			return firstMatch;

		if (*(PBYTE)patternPtr == '\?' || *(BYTE*)pCur == GET_BYTE(patternPtr))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!patternPtr[2])
				return firstMatch;

			if (*(PWORD)patternPtr == '\?\?' || *(PBYTE)patternPtr != '\?')
				patternPtr += 3;

			else
				patternPtr += 2;
		}
		else
		{
			patternPtr = pattern.c_str();
			firstMatch = 0;
		}
	}

	return NULL;
}

unsigned long CMemoryUtils::findPattern2(const char* module_name, const unsigned char* mask, const char* mask_string)
{
	/// Get module address
	const unsigned int module_address = reinterpret_cast<unsigned int>(GetModuleHandle(module_name));

	/// Get module information to the size
	MODULEINFO module_info;
	GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_address), &module_info, sizeof(MODULEINFO));

	auto IsCorrectMask = [](const unsigned char* data, const unsigned char* mask, const char* mask_string) -> bool
	{
		for (; *mask_string; ++mask_string, ++mask, ++data)
			if (*mask_string == 'x' && *mask != *data)
				return false;

		return (*mask_string) == 0;
	};

	/// Iterate until we find a matching mask
	for (unsigned int c = 0; c < module_info.SizeOfImage; c += 1)
	{
		/// does it match?
		if (IsCorrectMask(reinterpret_cast<unsigned char*>(module_address + c), mask, mask_string))
			return (module_address + c);
	}

	return 0;
}

#include <vector>
std::uint8_t* CMemoryUtils::pattern_scan(void* module, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_headers = (PIMAGE_DOS_HEADER)module;
	auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dos_headers->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scan_bytes[i];
		}
	}
	return nullptr;
}

CMemoryUtils* g_Memory = new CMemoryUtils();
#pragma once
#include "../stdafx.h"
#include <Psapi.h>

#define INRANGE(x, a, b)	(x >= a && x <= b)
#define GETBITS(x)			(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GETBYTE(x)			(GETBITS(x[0]) << 4 | GETBITS(x[1]))

#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#undef KEY
#undef BUFLEN

template <int XORSTART, int BUFLEN, int XREFKILLER>
class Xor
{
public:
	char s[BUFLEN];
	Xor(const char* xs);
#ifndef DEBUG_OPTION
	~Xor() { for (int i = 0; i < BUFLEN; i++)s[i] = 0; } // clear string from stack
#endif

private:
	Xor();
};

template <int XORSTART, int BUFLEN, int XREFKILLER>
Xor<XORSTART, BUFLEN, XREFKILLER>::Xor(const char* xs)
{
	int xvalue = XORSTART;
	int i = 0;
	for (; i < (BUFLEN - 1); i++) {
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = 0;
}


class CUtils
{
public:


	DWORD FindPattern(std::string moduleName, std::string pattern)
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;    //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}


	ULONG PatternSearch( std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode )
	{
		BOOL bPatternDidMatch = FALSE;
		HMODULE hModule = GetModuleHandle( sModuleName.c_str() );

		if( !hModule )
			return 0x0;

		PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER( hModule );
		PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS( LONG( hModule ) + pDsHeader->e_lfanew );
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

		if( uCodeBase == 0x0 )
			uCodeBase = ( ULONG )hModule + pOptionalHeader->BaseOfCode;

		if( uSizeOfCode == 0x0 )
			uSizeOfCode = pOptionalHeader->SizeOfCode;

		ULONG uArraySize = sMask.length();

		if( !uCodeBase || !uSizeOfCode || !uArraySize )
			return 0x0;

		for( size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++ )
		{
			for( size_t t = 0; t < uArraySize; t++ )
			{
				if( *( ( PBYTE )i + t ) == pbPattern[ t ] || sMask.c_str()[ t ] == '?' )
					bPatternDidMatch = TRUE;

				else
				{
					bPatternDidMatch = FALSE;
					break;
				}
			}

			if( bPatternDidMatch )
				return i;
		}

		return 0x0;
	}

	typedef void(*CL_FullUpdate_t) (void);

	CL_FullUpdate_t CL_FullUpdate = (CL_FullUpdate_t)FindPattern(/*engine.dll*/Xor<0xF1, 11, 0x39AB18D2>("\x94\x9C\x94\x9D\x9B\x93\xD9\x9C\x95\x96" + 0x39AB18D2).s, /*A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85*/Xor<0x6F, 43, 0xCCC757C2>("\x2E\x41\x51\x4D\x53\x4B\x55\x49\x57\x47\x59\x38\x42\x5C\x42\x5E\x40\xA0\xBE\xA2\xBC\xA4\xB0\xB0\xA7\xCE\xCF\xAA\xBE\xBC\xAD\xBF\xBB\xB0\xA9\xD0\xB3\xA7\xA1\xB6\xAF\xAD" + 0xCCC757C2).s);

	HMODULE GetModuleHandleSafe( const char* pszModuleName )
	{
		HMODULE hmModuleHandle = nullptr;

		do
		{
			hmModuleHandle = GetModuleHandle( pszModuleName );
			Sleep( 1 );
		}
		while( hmModuleHandle == nullptr );

		return hmModuleHandle;
	}
};

extern CUtils Utils;

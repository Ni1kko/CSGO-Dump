#include "Security.h"

using namespace Cheat;
CSecurity* Cheat::Security = new CSecurity();

void CSecurity::UnlinkModule( const char* szModule )
{
	DWORD dwPEB = 0, dwOffset = 0;
	PLIST_ENTRY pUserModuleHead, pUserModule;
	PPEB_LDR_DATA pLdrData;
	PLDR_MODULE pLdrModule = NULL;
	PUNICODE_STRING lpModule = NULL;
	char szModuleName[512];
	int i = 0, n = 0;
	#ifndef _WIN64
	_asm
	{
		pushad
		mov eax, fs: [48]
		mov dwPEB, eax
		popad
	}
	pLdrData = ( PPEB_LDR_DATA )( PDWORD )( *( PDWORD )( dwPEB + 12 ) );
	#else
	BYTE* _teb = ( BYTE* )__readgsqword( 0x30 );
	pLdrData = ( PPEB_LDR_DATA )( PULONGLONG )( *( PULONGLONG )( ( *( PULONGLONG )( _teb + 0x60 ) ) + 0x18 ) );
	#endif

	for ( ; i < 3; i++ )
	{
		switch ( i )
		{
			case 0:
				pUserModuleHead = pUserModule = ( PLIST_ENTRY )( &( pLdrData->InLoadOrderModuleList ) );
				dwOffset = 0;
				break;

			case 1:
				pUserModuleHead = pUserModule = ( PLIST_ENTRY )( &( pLdrData->InMemoryOrderModuleList ) );
				#ifndef _WIN64
				dwOffset = 8;
				#else
				dwOffset = 16;
				#endif
				break;

			case 2:
				pUserModuleHead = pUserModule = ( PLIST_ENTRY )( &( pLdrData->InInitializationOrderModuleList ) );
				#ifndef _WIN64
				dwOffset = 16;
				#else
				dwOffset = 32;
				#endif
				break;
		}

		while ( pUserModule->Flink != pUserModuleHead )
		{
			pUserModule = pUserModule->Flink;
			#ifndef _WIN64
			lpModule = ( PUNICODE_STRING )( ( ( DWORD )( pUserModule ) ) + ( 36 - dwOffset ) );
			#else
			lpModule = ( PUNICODE_STRING )( ( ( LONGLONG )( pUserModule ) ) + ( 72 - dwOffset ) );
			#endif

			for ( n = 0; n < ( lpModule->Length ) / 2 && n < 512; n++ )
				szModuleName[n] = ( CHAR )( *( ( lpModule->Buffer ) + ( n ) ) );

			szModuleName[n] = '\0';

			if ( strstr( szModuleName, szModule ) )
			{
				#ifndef _WIN64

				if ( !pLdrModule )
					pLdrModule = ( PLDR_MODULE )( ( ( DWORD )( pUserModule ) ) - dwOffset );

				#else

				if ( !pLdrModule )
					pLdrModule = ( PLDR_MODULE )( ( ( LONGLONG )( pUserModule ) ) - dwOffset );

				#endif
				pUserModule->Blink->Flink = pUserModule->Flink;
				pUserModule->Flink->Blink = pUserModule->Blink;
			}
		}
	}

	if ( pLdrModule )
	{
		pLdrModule->HashTableEntry.Blink->Flink = pLdrModule->HashTableEntry.Flink;
		pLdrModule->HashTableEntry.Flink->Blink = pLdrModule->HashTableEntry.Blink;
	}
}

void CSecurity::ErasePEHeader( HANDLE hModule )
{
	PIMAGE_DOS_HEADER pDosHeader = ( PIMAGE_DOS_HEADER )hModule;
	PIMAGE_NT_HEADERS pNTHeader = ( PIMAGE_NT_HEADERS )( ( PBYTE )pDosHeader + ( DWORD )pDosHeader->e_lfanew );

	if ( pNTHeader->Signature != IMAGE_NT_SIGNATURE )
		return;

	if ( pNTHeader->FileHeader.SizeOfOptionalHeader )
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect( ( void* )hModule, Size, PAGE_EXECUTE_READWRITE, &Protect );
		RtlZeroMemory( ( void* )hModule, Size );
		VirtualProtect( ( void* )hModule, Size, Protect, &Protect );
	}
}

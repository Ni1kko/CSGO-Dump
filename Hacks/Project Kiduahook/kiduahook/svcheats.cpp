#include "svcheats.h"

bool __fastcall HOOKED_SvCheats( PVOID pConVar, PVOID edx )
{
	PDWORD pdwVMT = *( PDWORD_PTR* ) g_pInput;

	if( ( DWORD ) _ReturnAddress() == pdwVMT [ 31 ] + 63 )
		return true;

	typedef bool( __thiscall* OriginalFuncFn )( PVOID );
	OriginalFuncFn dwOriginal = ( OriginalFuncFn ) dwSvCheatsVMT;

	return dwOriginal( pConVar );
}
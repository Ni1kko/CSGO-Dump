#include "csmshadow.h"

bool __fastcall HOOKED_CSMShadow( PVOID pConVar, PVOID edx )
{
	return false;

/*	typedef bool( __thiscall* OriginalFuncFn )( PVOID );
	OriginalFuncFn dwOriginal = ( OriginalFuncFn ) dwCSMShadowVMT;

	return dwOriginal( pConVar );*/
}
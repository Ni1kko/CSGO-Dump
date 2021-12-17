#pragma once
#include "HookIncludes.h"
#include <intrin.h>  
typedef bool(__thiscall *svc_get_bool_t)(PVOID);




auto dwCAM_Think = U::pattern_scan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");

bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
{
    static auto ofunc = hooks::sv_cheats.get_original<svc_get_bool_t>(13);
	if (!ofunc)
		return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
		return true;
	return ofunc(pConVar);
}
#include "TraceRay.hpp"
#include "awall.h"

TraceRayFn oTraceRay = nullptr;

#pragma optimize("", off)
bool IsCalledFromAwall(const uintptr_t* ebp)
{
	if (!ebp)
		return false;

	auto ebp_val = ebp;
/*	for (int i = 0; i < 10; i++)
	{
		if (!ebp_val)
			break;
		auto rt_address = ebp_val[1];
		auto min_limit = (DWORD)&CGamePenetration::GetPenetrationDamage;
		auto max_limit = (DWORD)&CGamePenetration::GetPenetrationDamage + 0x1000;
		auto min_limit_1 = (DWORD)&CGamePenetration::GetPenetrationDamageAfterFirstWall;
		auto max_limit_1 = (DWORD)&CGamePenetration::GetPenetrationDamageAfterFirstWall + 0x1000;
		if (rt_address >= min_limit && rt_address <= max_limit ||
			rt_address >= min_limit_1 && rt_address <= max_limit_1)
			return true;
		ebp_val = *(uintptr_t**)(ebp_val);
	}*/
	return false;
}

void __fastcall hkTraceRay(void* pThis, void*, void* pRay, int nMask, void* pFilter, void* pTrace)
{
	oTraceRay(pThis, pRay, nMask, pFilter, pTrace);

	uintptr_t* rEBP;
	__asm mov rEBP, ebp
	if (IsCalledFromAwall(rEBP))
		((CEngineTraceClient::trace_t*)pTrace)->surface.flags |= 4;
}
#pragma optimize("", on)


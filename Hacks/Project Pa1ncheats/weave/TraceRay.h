#pragma once
#include "Hooks.h"

//void __fastcall Hooked_TraceRay(void *thisptr, void*, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
//{
//	if (!csgo->InAutowall)
//		return H::TraceRay(thisptr, ray, fMask, pTraceFilter, pTrace);
//
//	CTraceFilterSkipTwoEntities filter;
//	filter.pPassEntity1 = reinterpret_cast< void* >(csgo->local);
//	filter.pPassEntity1 = reinterpret_cast< void* >(reinterpret_cast< IBasePlayer* >(filter.pPassEntity1)->GetWeapon());
//
//	H::TraceRay(thisptr, ray, fMask, &filter, pTrace);
//
//	pTrace->surface.flags |= SURF_SKY;
//}
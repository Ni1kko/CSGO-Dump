#pragma once

typedef void(__thiscall *TraceRayFn)(void*, void*, int, void*, void*);
extern TraceRayFn oTraceRay;

void __fastcall hkTraceRay(void* pThis, void*, void* pRay, int nMask, void* pFilter, void* pTrace);
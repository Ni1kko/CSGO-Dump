#pragma once
#include "Hooks.h"

void __fastcall Hooked_BeginFrame(void* thisptr)
{
	static auto BeginFrame = g_pStudioRenderHook->GetOriginal< BeginFrameFn > (9);
	features->BulletTracer->Draw();
	BeginFrame(thisptr);
}
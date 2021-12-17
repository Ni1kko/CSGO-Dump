#pragma once

#include "main.h"

bool __fastcall SetupBonesLocal(void* ECX, void* EDX, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

namespace SetupBonesHook
{
	void SetupHook();
	void RemoveHook();
}
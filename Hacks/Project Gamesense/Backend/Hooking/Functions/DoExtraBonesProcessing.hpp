#pragma once
#include "../Hooks/Hooks.h"
#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
//typedef void(__fastcall* fnhkDoExtraBonesProcessing)(CBaseEntity*, void*, studiohdr_t*, Vector*, void*, matrix3x4_t, void*, void*);
//void __fastcall hkDoExtraBonesProcessing(CBaseEntity* e, void*, studiohdr_t* a, Vector* b, void* c, matrix3x4_t d, void* f, void* g) {
//	return;
//}

// all hooks related to anims, bones, etc

namespace Cheat
{
	namespace Hooked
	{
		_declspec(noinline)void DoExtraBonesProcessing_Detour(uintptr_t* ecx, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context)
		{
			return;
		}

		void __fastcall DoExtraBonesProcessing(uintptr_t* ecx, uint32_t, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context)
		{
			DoExtraBonesProcessing_Detour(ecx, hdr, pos, q, matrix, bone_computed, context);
		}

		bool __fastcall ShouldSkipAnimFrame()
		{
			return false;
		}
	}
}
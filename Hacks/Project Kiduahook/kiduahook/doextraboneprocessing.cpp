#include "doextraboneprocessing.h"
#include "Detour.h"
#include "patternscan.h"

namespace DoExtraBoneProcessingHook
{
	Detour _d;

	void __fastcall DoExtraBonesProcessing(CBaseEntity *pEntity, void *edx, studiohdr_t *hdr, Vector *vector, Quaternion *something, matrix3x4_t *matrix, /*CBoneBitList &bone_list*/void* bone_list, /*CIKContext*/void *context)
	{
		using org_fn = void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, matrix3x4_t*, void*, void*);

		__asm PUSHAD

		int iLocalPlayer = g_pEngine->GetLocalPlayer();

		int iLocalTeam = 0;

		CBaseEntity* pLocal = g_pEntList->GetClientEntity(iLocalPlayer);

		if (pLocal)
			iLocalTeam = pLocal->GetTeamNumber();

		if (pEntity && pEntity->IsPlayer() && (pEntity->GetIndex() == iLocalPlayer || pEntity->GetTeamNumber() != iLocalTeam))
		{
			CAnimState* pAnimState = pEntity->GetAnimState();

			if (pAnimState && pAnimState->pBaseEntity)
			{
				const bool bBackup = pAnimState->m_bOnGround;

				pAnimState->m_bOnGround = false;

				__asm POPAD

				_d.GetOriginal<org_fn>()(pEntity, hdr, vector, something, matrix, bone_list, context);

				pAnimState->m_bOnGround = bBackup;
			}
		}
		else
		{
			__asm POPAD

			_d.GetOriginal<org_fn>()(pEntity, hdr, vector, something, matrix, bone_list, context);
		}
	}

	/*
	int __thiscall HookedDoExtraBoneProcessing(char *this, int a2, int a3, int a4, int a5, int a6, int a7)
	{
	int v7; // edi
	int v8; // esi
	int result; // eax

	v7 = 0;
	v8 = *(_DWORD *)&this[offset_m_animation_state];
	if ( v8 )
	{
	v7 = *(_DWORD *)(v8 + 0x60);
	*(_DWORD *)(v8 + 96) = 0;
	}
	result = ((int (__stdcall *)(int, int, int, int, int, int))*(&xmmword_1011AA78 + 1))(a2, a3, a4, a5, a6, a7);
	if ( v8 )
	{
	if ( v7 )
	*(_DWORD *)(v8 + 96) = v7;
	}
	return result;
	}*/

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C");

		if (!PatternResult)
			throw "Failed to signature scan DoExtraBoneProcessing!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(DoExtraBonesProcessing));
	}

	void RemoveHook()
	{
		_d.Detach();
	}
}

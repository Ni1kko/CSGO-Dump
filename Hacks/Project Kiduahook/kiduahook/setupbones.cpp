#include "setupbones.h"
#include "Detour.h"

//55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D | client
//have to hook it and give a shit about cuz we dont need it

SetupBones_t oSetupBones;
bool __fastcall SetupBonesLocal(void* ECX, void* EDX, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	CAnimState* pAnimState = pLocal->GetAnimState();

	if (!ECX || !pBoneToWorldOut || !pLocal || pLocal->GetHealth() <= 0 || !pAnimState)
		return oSetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	static Vector vLastOrigin = Vector(0, 0, 0);

	if (!g_pClientState->GetChokedCmds())
		vLastOrigin = Vector(pAnimState->m_vOrigin_x, pAnimState->m_vOrigin_y, pAnimState->m_vOrigin_z);

	if (UserCmd::bInFakeDuck)
		pLocal->SetAbsOrigin(vLastOrigin);

	return oSetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

namespace SetupBonesHook
{
	Detour _d;


	bool __fastcall HOOKED_SetupBones(void* ECX, void* EDX, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		using org_fn = bool(__thiscall*)( void*, matrix3x4_t*, int, int, float);

		CBaseEntity* pEntity = ((CBaseEntity*)ECX);
		CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (!pEntity)
		{
			return _d.GetOriginal<org_fn>()(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		}

		if (pLocal)
		{
			if (pLocal->GetTeamNumber() != pEntity->GetTeamNumber())
			{
				return false;
			}
		}

		return _d.GetOriginal<org_fn>()(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

/*		__asm PUSHAD

		__asm POPAD

		return _d.GetOriginal<org_fn>()(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	
		CBaseEntity* pEntity = ((CBaseEntity*)ECX);
	
		if (pEntity && pEntity->GetIndex() > 0 && pEntity->GetIndex() < 65)
		{*/
			//check if the setupbones call is from us if so then return

			//check if matrix is null ( then we have to return a valid bone matrix ourselves since game wants a new one or just wants to verify it was setup )

			//return matrix
			
			//func like that ?

		/*	if(bone_count > 128)
			    bone_count = 128;

			if (g_History.m_ValidMatrix)
				return false;

			memcpy(matrix, g_History.m_ValidMatrix, sizeof(matrix3x4_t) * bone_count);
			return true;*/
//		}

		//return original and we are done here...
	}

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D");

		if (!PatternResult)
			throw "Failed to signature scan SetupBones!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(HOOKED_SetupBones));
	}

	void RemoveHook()
	{
		_d.Detach();
	}
}
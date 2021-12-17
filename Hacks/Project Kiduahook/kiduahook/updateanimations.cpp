#include "updateanimations.h"
#include "Detour.h"
#include "patternscan.h"

namespace UpdateAnimsHook
{
	Detour _d;
	using org_fn = bool(__thiscall*)(CBaseEntity*);
	org_fn org_call = nullptr;

	void ManualUpdateAnimations(CBaseEntity *pEntity)
	{
		org_call(pEntity);
	}

	void __fastcall UpdatePlayerAnimations(CBaseEntity *pEntity, void *edx)
	{
		if (!pEntity)
		{
			org_call(pEntity);
			return;
		}

		CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (pLocal)
		{
			if (pEntity->GetIndex() == pLocal->GetIndex() || pLocal->GetTeamNumber() != pEntity->GetTeamNumber())
			{
				return;
			}
		}
		
		// For others (teammates?) animate them normally
		org_call(pEntity);
	}

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36");

		if (!PatternResult)
			throw "Failed to signature scan UpdatePlayerAnimations!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(UpdatePlayerAnimations));
		org_call = _d.GetOriginal<org_fn>();;
	}

	void RemoveHook()
	{
		_d.Detach();
	}
}

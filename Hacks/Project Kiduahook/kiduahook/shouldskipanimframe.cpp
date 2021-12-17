#include "shouldskipanimframe.h"
#include "Detour.h"
#include "patternscan.h"

namespace ShouldSkipAnimFrameHook
{
	Detour _d;
	using org_fn = bool(__thiscall*)(CBaseEntity*);
	org_fn org_call = nullptr;
	
	bool __fastcall ShouldSkipAnimationFrame(CBaseEntity *pEntity, void *edx)
	{
		return false;
	}

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02");

		if (!PatternResult)
			throw "Failed to signature scan ShouldSkipAnimationFrame!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(ShouldSkipAnimationFrame));
		org_call = _d.GetOriginal<org_fn>();
	}

	void RemoveHook()
	{
		_d.Detach();
	}
}

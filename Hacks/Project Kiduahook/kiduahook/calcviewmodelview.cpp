#include "calcviewmodelview.h"
#include "Detour.h"

//55 8B EC 83 EC 64 56 57

namespace CalcViewModelView
{
	Detour _d;

	void __fastcall HOOKED_CalcViewModelView(void* ecx, void* edx, CBaseEntity* owner, Vector& pos, Vector& angles)
	{
		using org_fn = void(__thiscall*)(void*, CBaseEntity*, Vector&, Vector&);

		__asm PUSHAD


		CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (pLocal && pLocal->GetHealth() > 0)
		{
			Vector vPitch, vYaw, vRoll;
			g_Math.AngleVector(angles, &vPitch, &vYaw, &vRoll);
			pos += vPitch * ( g_Var.Visuals.World.ViewModelOffset[0] - 90.f );
			pos += vYaw * ( g_Var.Visuals.World.ViewModelOffset[1] - 90.f );
			pos += vRoll * ( g_Var.Visuals.World.ViewModelOffset[2] - 90.f);
		}

		__asm POPAD

		_d.GetOriginal<org_fn>()(ecx, owner, pos, angles);
	}

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 83 EC 58 56 57 8B 7D 08");

		if (!PatternResult)
			throw "Failed to signature scan CalcViewModelView!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(HOOKED_CalcViewModelView));
	}
}
#pragma once
#include "HookIncludes.h"
#include <intrin.h>  
typedef bool(__thiscall* in_prediction_t)(PVOID);

bool __stdcall hkInPrediction()
{
    static auto sig = U::FindPattern("client.dll", (PBYTE)"\x84\xC0\x75\x08\x57\x8B\xCE\xE8\x00\x00\x00\x00\x8B\x06", "xxxxxxxx????xx");
	bool result;
    static auto ofunc = hooks::prediction.get_original<in_prediction_t>(14);
	static IPrediction *ecxVal = g_Prediction;
	result = ofunc(ecxVal);
	if (g_Options.Visuals.NoVisualRecoil && (DWORD)(_ReturnAddress()) == sig)
	{
		C_BaseEntity* pLocalEntity = NULL;
		float* m_LocalViewAngles = NULL;
		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}
		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();
		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}
	return result;
}
#pragma once
#include "HookIncludes.h"

typedef void(__thiscall *override_view_t)(void* _this, CViewSetup* setup);
typedef float(__thiscall *get_fov_t)(void*);



float __fastcall hkGetViewModelFOV(void* ecx, void* edx)
{
    static auto ofunc = hooks::clientmode.get_original<get_fov_t>(35);
    float viewmodelFOV = ofunc(ecx);
    return g_Options.Visuals.viewmodelChanger;
}

void __fastcall hkOverrideView(void* _this, void* _edx, CViewSetup* setup)
{
    static auto ofunc = hooks::clientmode.get_original<override_view_t>(18);
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (pLocal && g_Engine->IsInGame())
	{
		if (!pLocal->IsScoped())
			setup->fov += g_Options.Visuals.FOVChanger;
        if (g_Options.Visuals.ThirdPerson && pLocal->IsAlive())
        {
            if (!g_Input->m_fCameraInThirdPerson)
            {
                g_Input->m_fCameraInThirdPerson = true;
            }
        }
        else
        {
            g_Input->m_fCameraInThirdPerson = false;
        }
	}
    grenade_prediction::instance().View(setup);
    ofunc(_this, setup);
}

#pragma once
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CViewSetup.h"
#include "../../SDK/CBaseEntity.h"
#include "../../Features/Visuals/Visuals.h"

typedef int( __stdcall* fnOverrideView )( CViewSetup* );
fnOverrideView oOverrideView;

#define CAM_HULL_OFFSET		14.0

inline void UTIL_TraceHull(const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &hullMin,
	const Vector &hullMax, unsigned int mask, ITraceFilter *pFilter, trace_t *ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);

	Interfaces::Trace->TraceRay(ray, mask, pFilter, ptr);
}

static Vector CAM_HULL_MIN(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
static Vector CAM_HULL_MAX(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);

namespace Cheat
{
	namespace Hooked
	{
		void __stdcall OverrideViewHook( CViewSetup* pSetup )
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.OverrideView = 0;
				oOverrideView(pSetup);
				return;
			}
			Cheat::Settings->UnloadReadyTable.OverrideView = (int)GetTickCount();

			oOverrideView(pSetup);

			static bool oldfd = false;
			static float oldz = 0.f;
			if ( Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() )
			{
				if ((G::LocalPlayer->IsScoped() && c_config::get()->b["vis_noscope"]) || !G::LocalPlayer->IsScoped())
					pSetup->fov = c_config::get()->i["misc_overridefov"];
				else {
					if (G::LocalPlayer->IsScoped() && c_config::get()->i["misc_overridefov"] > 90)
						pSetup->fov = pSetup->fov + (c_config::get()->i["misc_overridefov"] - 60);
				}
				/*if (!G::LocalPlayer->IsScoped())
					pSetup->fov = c_config::get()->i["misc_overridefov"];*/
				/*else {
					if (G::LocalPlayer->IsScoped() && c_config::get()->i["misc_overridezoomfov"] > 60)
						pSetup->fov = pSetup->fov + (c_config::get()->i["misc_overridezoomfov"] - 60);
				}*/
				/*if (G::LocalPlayer->IsScoped() && c_config::get()->i["misc_overridezoomfov"] != 60) {
					pSetup->fov = c_config::get()->i["misc_overridezoomfov"];
				}	
				else if (!G::LocalPlayer->IsScoped()) {
					pSetup->fov = c_config::get()->i["misc_overridefov"];
				}*/

				Features::grenade_prediction::get()->View(pSetup);

				auto shouldfd = c_config::get()->b["rage_fakeduck"] && c_keyhandler::get()->auto_check("rage_fakeduck_key");
				if (shouldfd) {
					if (!G::IsInThirdPerson)
						pSetup->origin.z = G::LocalPlayer->GetAbsOrigin().z + 64.f;
					else {
						trace_t trace;
						Vector cam_forward, cam_right, cam_up;

						Utilities->Math_AngleVectors(pSetup->angles, &cam_forward, &cam_right, &cam_up);
						Vector end_point = Vector(G::LocalPlayer->GetAbsOrigin().x, G::LocalPlayer->GetAbsOrigin().y, G::LocalPlayer->GetAbsOrigin().z + 64.f);
						Vector cam_offset = end_point + (cam_forward * -150) + (cam_right * 0) + (cam_up * 0);

						CTraceFilter trace_filter;
						trace_filter.pSkip1 = G::LocalPlayer;

						UTIL_TraceHull(end_point, cam_offset, CAM_HULL_MIN, CAM_HULL_MAX, MASK_SOLID & ~CONTENTS_MONSTER, &trace_filter, &trace);
						pSetup->origin.z = trace.end.z;
					}
				}

				for (auto hk : c_lua::get()->hooks->getHooks("on_override_view"))
					hk.func(pSetup);
			}
		}
	}
}
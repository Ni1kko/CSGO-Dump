#include "NoVisualRecoil.hpp"
#include "../../options.hpp"
#include "misc.hpp"

namespace NoVisualRecoil {
	void OverrideView(CViewSetup* pSetup) {
		if (!g_Misc->options.no_visual_recoil || !g_EngineClient->IsInGame())
			return;

		QAngle viewPunch = g_LocalPlayer->m_viewPunchAngle();
		QAngle aimPunch = g_LocalPlayer->m_aimPunchAngle();

		pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
	}
};
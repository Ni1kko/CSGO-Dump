#include "Nightmode.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../options.hpp"
#include "misc.hpp"


namespace NightMode {
	void EndScene() {
		static auto r_modelAmbientMin = g_CVar->FindVar("r_modelAmbientMin");
		static auto mat_force_tonemap_scale = g_CVar->FindVar("mat_force_tonemap_scale");

		r_modelAmbientMin->SetValue(g_Misc->options.nightmode_value > 1.f ? 1.f : 0.0f);
		mat_force_tonemap_scale->SetValue(g_Misc->options.nightmode_value);
	}
};
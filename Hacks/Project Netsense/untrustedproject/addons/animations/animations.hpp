#pragma once
#include "../../menu.hpp"
#include "../../valve_sdk/sdk.hpp"

class CAnimations : public Singleton<CAnimations> {
private:
	float Alpha = 0.f;
public:
	void Begin(bool visible) {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Alpha);
		float animtime = g_GlobalVars->frametime * 4.f;
		if (Alpha < 1.f && visible) {
			if (Alpha + animtime > 1.f)
				Alpha = 1.f;
			else
				Alpha += animtime;
		}
		else if (Alpha > 0.f && !visible) {
			if (Alpha - animtime < 0.f)
				Alpha = 0.f;
			else
				Alpha -= animtime;
		}
	}
	void End() {
		ImGui::PopStyleVar();
	}
	float GetAlpha(bool mult = true) {
		if (mult)
			return Alpha * 255.f;
		return Alpha;
	}
};
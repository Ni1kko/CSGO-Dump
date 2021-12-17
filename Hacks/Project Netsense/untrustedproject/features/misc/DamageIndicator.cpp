#include "DamageIndicator.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../helpers/math.hpp"
#include "../../render.hpp"
#include "../../imgui/imgui.h"
#include "misc.hpp"


void DamageIndicators::fireEvent(IGameEvent* pEvent) {
	if (!g_Misc->options.damage_indicator) return;
	if (!strcmp(pEvent->GetName(), "player_hurt")) {
		C_BasePlayer* hurt = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(pEvent->GetInt("userid")));
		C_BasePlayer* attacker = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(pEvent->GetInt("attacker")));

		if (hurt != g_LocalPlayer && attacker == g_LocalPlayer)
		{
			DamageIndicator_t DmgIndicator;
			DmgIndicator.iDamage = pEvent->GetInt("dmg_health");
			DmgIndicator.Player = hurt;
			DmgIndicator.flEraseTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick + 3.f;
			DmgIndicator.bInitialized = false;

			DamageIndicator.push_back(DmgIndicator);
		}
	}
}

void DamageIndicators::paint() {
	if (!g_Misc->options.damage_indicator) return;
	float CurrentTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	for (size_t i = 0; i < DamageIndicator.size(); i++)
	{
		if (DamageIndicator[i].flEraseTime < CurrentTime)
		{
			DamageIndicator.erase(DamageIndicator.begin() + i);
			continue;
		}

		if (!DamageIndicator[i].bInitialized)
		{
			DamageIndicator[i].Position = DamageIndicator[i].Player->GetBonePos(HITBOX_HEAD);
			DamageIndicator[i].bInitialized = true;
		}

		if (CurrentTime - DamageIndicator[i].flLastUpdate > 0.0001f)
		{
			DamageIndicator[i].Position.z -= (0.1f * (CurrentTime - DamageIndicator[i].flEraseTime));
			DamageIndicator[i].flLastUpdate = CurrentTime;
		}

		Vector ScreenPosition;

		if (Math::WorldToScreen(DamageIndicator[i].Position, ScreenPosition))
			Render::Get().RenderText(std::to_string(DamageIndicator[i].iDamage), ImVec2(ScreenPosition.x, ScreenPosition.y), 16.f, g_Misc->colors.damage_indicator_color);
	}
}

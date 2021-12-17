#include "glow.hpp"

#include "../../sdk/structs.hpp"
#include "../config/options.hpp"

Glow::Glow() { }

Glow::~Glow() { }

void Glow::Shutdown() {
	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
		auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

		if (glowObject.IsUnused()) continue;

		if (!entity || entity->IsDormant()) continue;

		glowObject.m_flAlpha = 0.0f;
	}
}

void Glow::Run() {
	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
		auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

		if (glowObject.IsUnused()) continue;

		if (!entity || entity->IsDormant()) continue;

		auto class_id = entity->GetClientClass()->m_ClassID;
		auto color = Color{};

		switch (class_id) {
		case 40: {
			auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();
			auto is_visible = entity->IsVisible(true);

			if (!entity->IsAlive()) continue;
			if (!is_enemy && Cfg.glow_enemies_only) continue;
			if (!is_visible && Cfg.glow_visible_only) continue;

			color = (is_enemy ? (is_visible ? Color(Cfg.color_glow_enemy[0] * 255, Cfg.color_glow_enemy[1] * 255, Cfg.color_glow_enemy[2] * 255) : Color(Cfg.color_glow_enemy_invis[0] * 255, Cfg.color_glow_enemy_invis[1] * 255, Cfg.color_glow_enemy_invis[2] * 255)) : (is_visible ? Color(Cfg.color_glow_ally[0] * 255, Cfg.color_glow_ally[1] * 255, Cfg.color_glow_ally[2] * 255) : Color(Cfg.color_glow_ally_invis[0] * 255, Cfg.color_glow_ally_invis[1] * 255, Cfg.color_glow_ally_invis[2] * 255)));
			color = is_enemy ? Color(Cfg.color_glow_enemy) : Color(Cfg.color_glow_ally);

			break;
		}
		case ClassId_CChicken:
			if (!Cfg.glow_chickens) continue;
			entity->m_bShouldGlow() = true;
			color = Color(Cfg.color_glow_chickens);
			break;
		case ClassId_CBaseAnimating:
			if (!Cfg.glow_defuse_kits) continue;
			color = Color(Cfg.color_glow_defuse);
			break;
		case ClassId_CPlantedC4:
			if (!Cfg.glow_planted_c4) continue;
			color = Color(Cfg.color_glow_planted_c4);
			break;
		default: {
			if (entity->IsWeapon()) {
				if (!Cfg.glow_weapons) continue;
				color = Color(Cfg.color_glow_weapons);
			}
		}
		}

		switch (Cfg.glow_style) {
		case 0: glowObject.m_nGlowStyle = 0;
			break;
		case 1: glowObject.m_nGlowStyle = 2;
			break;
		case 2: glowObject.m_nGlowStyle = 1;
			break;
		}

		glowObject.m_flRed = color.r() / 255.0f;
		glowObject.m_flGreen = color.g() / 255.0f;
		glowObject.m_flBlue = color.b() / 255.0f;
		glowObject.m_flAlpha = color.a() / 255.0f;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_bRenderWhenUnoccluded = false;
	}
}
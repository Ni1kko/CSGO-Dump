	#include "..\hooks.hpp"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#include "..\..\NewMenu.h"

using SceneEnd_t = void(__thiscall*)(void*);

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

void __fastcall hooks::hooked_sceneend(void * thisptr, void * edx) {
	static auto original_fn = renderview_hook->get_func_address< SceneEnd_t >(9);
	if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		return original_fn(thisptr);;
	if (!g_ctx.m_local)
		return original_fn(thisptr);

	static IMaterial * materials[4] = {
		g_csgo.m_materialsystem()->FindMaterial("vitality_regular", TEXTURE_GROUP_MODEL),
		g_csgo.m_materialsystem()->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL),
		g_csgo.m_materialsystem()->FindMaterial("vitality_rubber", TEXTURE_GROUP_MODEL),
		g_csgo.m_materialsystem()->FindMaterial("vitality_metallic", TEXTURE_GROUP_MODEL)
	};

	if (g_cfg.esp.ghost_chams && g_ctx.m_local->is_alive39()) {
		vec3_t abs_angles = g_ctx.m_local->abs_angles();

		if (g_cfg.misc.ghostcharmsangle == 1)
			g_ctx.m_local->set_abs_angles(vec3_t(0, g_ctx.m_globals.real_angles.y, 0));
		else
			g_ctx.m_local->set_abs_angles(vec3_t(0, g_ctx.m_local->m_flLowerBodyYawTarget(), 0));
		float normal_color[4] = {
			g_cfg.esp.ghost_chams_color[0] / 255.f,
			g_cfg.esp.ghost_chams_color[1] / 255.f,
			g_cfg.esp.ghost_chams_color[2] / 255.f,
			g_cfg.esp.ghost_chams_color[3] / 255.f
		};

		g_csgo.m_renderview()->SetColorModulation(normal_color);

		g_csgo.m_renderview()->SetBlend(0.3f);

		g_csgo.m_modelrender()->ForcedMaterialOverride(materials[0]);

		g_ctx.m_local->DrawModel(0x1, 255);

		g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);

		g_csgo.m_renderview()->SetBlend(1.f);

		g_ctx.m_local->set_abs_angles(vec3_t(0, abs_angles.y, 0));
	}

	if (g_cfg.player.chams[PLAYER_CHAMS_VISIBLE].enabled) {
		float alpha = g_cfg.player.chams_opacity / 100.0f;

		static IMaterial * backtrack_material = g_csgo.m_materialsystem()->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);

		static bool color_modulate[4] = { true, true, true, true };

		IMaterial * material = materials[g_cfg.player.chams_type];

		if (material && !material->IsErrorMaterial()) {
			for (int i = 1; i < g_csgo.m_globals()->m_maxclients; i++) {
				auto e = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));

				if (!e->valid(true))
					continue;

				if (g_cfg.ragebot.lagcomp && g_cfg.player.backtrack_chams && g_ctx.m_local->is_alive40()) {
					auto & player = lagcompensation::get().players[e->EntIndex()];

					if (player.m_e && !player.get_valid_track().empty()) {
						g_ctx.backtrack_chams = true;

						// -------------- CHAMS --------------
						float color[4] =  {
							g_cfg.player.backtrack_chams_color[0] / 255.f,
							g_cfg.player.backtrack_chams_color[1] / 255.f,
							g_cfg.player.backtrack_chams_color[2] / 255.f,
							g_cfg.player.backtrack_chams_color[3] / 255.f
						};

						// -------------- XQZ --------------
						g_csgo.m_renderview()->SetColorModulation(color);
						g_csgo.m_renderview()->SetBlend(g_cfg.player.backtrack_chams_opacity / 100.f);

						backtrack_material->IncrementReferenceCount();
						backtrack_material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

						g_csgo.m_modelrender()->ForcedMaterialOverride(backtrack_material);

						e->DrawModel(0x1, 255);

						g_csgo.m_renderview()->SetBlend(1.f);
						g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);

						// ------------- NORMAL -------------
						g_csgo.m_renderview()->SetColorModulation(color);
						g_csgo.m_renderview()->SetBlend(g_cfg.player.backtrack_chams_opacity / 100.f);

						backtrack_material->IncrementReferenceCount();
						backtrack_material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

						g_csgo.m_modelrender()->ForcedMaterialOverride(backtrack_material);

						e->DrawModel(0x1, 255);

						g_csgo.m_renderview()->SetBlend(1.f);
						g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);

						g_ctx.backtrack_chams = false;
					}
				}

				if (g_cfg.player.chams[PLAYER_CHAMS_INVISIBLE].enabled) {
					float xqz_color[4] = {
						g_cfg.player.xqz_color[0] / 255.f,
						g_cfg.player.xqz_color[1] / 255.f,
						g_cfg.player.xqz_color[2] / 255.f,
						g_cfg.player.xqz_color[3] / 255.f
					};

					if (color_modulate[g_cfg.player.chams_type]) g_csgo.m_renderview()->SetColorModulation(xqz_color);

					if (alpha < 1)
						g_csgo.m_renderview()->SetBlend(alpha);

					material->IncrementReferenceCount();
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true); //crash

					g_csgo.m_modelrender()->ForcedMaterialOverride(material);

					e->DrawModel(0x1, 255);

					g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);

					float normal_color[4] = {
						g_cfg.player.chams_color[0] / 255.f,
						g_cfg.player.chams_color[1] / 255.f,
						g_cfg.player.chams_color[2] / 255.f,
						g_cfg.player.chams_color[3] / 255.f
					};

					if (color_modulate[g_cfg.player.chams_type]) g_csgo.m_renderview()->SetColorModulation(normal_color);

					if (alpha < 1)
						g_csgo.m_renderview()->SetBlend(alpha);

					material->IncrementReferenceCount();
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

					g_csgo.m_modelrender()->ForcedMaterialOverride(material);

					e->DrawModel(0x1, 255);

					g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);
				}
				else {
					float normal_color[4] = {
						g_cfg.player.chams_color[0] / 255.f,
						g_cfg.player.chams_color[1] / 255.f,
						g_cfg.player.chams_color[2] / 255.f,
						g_cfg.player.chams_color[3] / 255.f
					};

					if (color_modulate[g_cfg.player.chams_type]) g_csgo.m_renderview()->SetColorModulation(normal_color);

					if (alpha < 1)
						g_csgo.m_renderview()->SetBlend(alpha);

					g_csgo.m_modelrender()->ForcedMaterialOverride(material);

					e->DrawModel(0x1, 255);

					g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);
				}
			}
		}
	}

	if (g_cfg.player.local && g_ctx.m_local->is_alive41())
	{
		float clr[4] = {
			g_cfg.esp.local_chams_color[0] / 255.f,
			g_cfg.esp.local_chams_color[1] / 255.f,
			g_cfg.esp.local_chams_color[2] / 255.f,
			g_cfg.esp.local_chams_color[3] / 255.f
		};

		g_csgo.m_renderview()->SetColorModulation(clr);

		float alpha = 0.f;

		alpha++;

		if (alpha > 99.f)
		{
			alpha = 0.f;
		}
		else
		{
			alpha++;
		}


		float al = alpha / 100.f;

		g_csgo.m_renderview()->SetBlend(al);

		g_csgo.m_modelrender()->ForcedMaterialOverride(materials[2]);

		g_ctx.m_local->DrawModel(0x1, 255);

		g_csgo.m_modelrender()->ForcedMaterialOverride(nullptr);
	}

	original_fn(thisptr);
}
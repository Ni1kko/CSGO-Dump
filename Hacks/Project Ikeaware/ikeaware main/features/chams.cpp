#include "chams.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../hooks/hooks.hpp"

void modulate(const Color color, IMaterial* material)
{
	if (!g_EngineClient->IsInGame())
		return;

	if (material)
	{
		material->ColorModulate(color[0] / 255.f, color[1] / 255.f, color[2] / 255.f);
		material->AlphaModulate(color[3] / 255.f);
		const auto tint = material->find_var("$envmaptint");

		if (tint)
		{
			tint->set_vector(Vector(color.r(), color.g(), color.b()));
		}
	}

	g_RenderView->SetColorModulation(color[0] / 255.f, color[1] / 255.f, color[2] / 255.f);
}

void Chams::OnDrawModelExecute(void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int32_t iFlags)
{
	static auto fnDME = Hooks::stdrender_hook.get_original<decltype(&Hooks::DrawModelExecute)>(index::DrawModelExecute);

	if (!pInfo->m_pClientEntity || !g_LocalPlayer)
		return;

	const auto mdl = pInfo->m_pClientEntity->GetModel();
	bool is_arm = strstr(mdl->szName, "arms") != nullptr;
	bool is_player = strstr(mdl->szName, "models/player") != nullptr;
	bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
	bool is_weapon = strstr(mdl->szName, "weapons/v_") != nullptr && !is_arm;
	static IMaterial* shine = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
	if (is_player) {
		// 
		// Draw player Chams.
		// 
		const auto ent = (C_BasePlayer*)pInfo->m_pClientEntity->GetIClientUnknown()->GetBaseEntity();

		if (ent && ent->IsAlive()) {
			const auto enemy = ent->IsEnemy();
			const auto local = (ent == g_LocalPlayer);
			if (enemy)
			{
				if (!g_Options.chams.player_enemies && !g_Options.chams.player_enemies_occluded)
				{
					return;
				}
				else
				{
					static IMaterial* player_enemies_type = nullptr;
					switch (g_Options.chams.player_enemies_type)
					{
					case 0:
						player_enemies_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_enemies_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_enemies_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_enemies_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					static IMaterial* player_enemies_occluded_type = nullptr;
					switch (g_Options.chams.player_enemies_occluded_type)
					{
					case 0:
						player_enemies_occluded_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_enemies_occluded_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_enemies_occluded_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_enemies_occluded_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					if (player_enemies_type != nullptr && player_enemies_occluded_type != nullptr && shine != nullptr)
					{
						if (g_Options.chams.player_enemies_occluded)
						{
							player_enemies_occluded_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							modulate(g_Options.colors.chams.player_enemy_occluded, player_enemies_occluded_type);
							g_StudioRender->ForcedMaterialOverride(player_enemies_occluded_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_enemies_occluded_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								shine->AlphaModulate(g_Options.colors.chams.player_enemy_occluded_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_enemy_occluded_shine[0] / 255.f, g_Options.colors.chams.player_enemy_occluded_shine[1] / 255.f, g_Options.colors.chams.player_enemy_occluded_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
						if (g_Options.chams.player_enemies)
						{
							player_enemies_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							modulate(g_Options.colors.chams.player_enemy_visible, player_enemies_type);
							g_StudioRender->ForcedMaterialOverride(player_enemies_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_enemies_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
								shine->AlphaModulate(g_Options.colors.chams.player_enemy_visible_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_enemy_visible_shine[0] / 255.f, g_Options.colors.chams.player_enemy_visible_shine[1] / 255.f, g_Options.colors.chams.player_enemy_visible_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
					}
					else
					{
						return;
					}
				}
			}

			if (!enemy && !local)
			{
				if (!g_Options.chams.player_teammates && !g_Options.chams.player_teammates_occluded)
				{
					return;
				}
				else
				{
					static IMaterial* player_teammates_type = nullptr;
					switch (g_Options.chams.player_teammates_type)
					{
					case 0:
						player_teammates_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_teammates_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_teammates_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_teammates_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					static IMaterial* player_teammates_occluded_type = nullptr;
					switch (g_Options.chams.player_teammates_occluded_type)
					{
					case 0:
						player_teammates_occluded_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_teammates_occluded_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_teammates_occluded_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_teammates_occluded_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					if (player_teammates_type != nullptr && player_teammates_occluded_type != nullptr && shine != nullptr)
					{
						if (g_Options.chams.player_teammates_occluded)
						{
							player_teammates_occluded_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							modulate(g_Options.colors.chams.player_ally_occluded_shine, player_teammates_occluded_type);
							g_StudioRender->ForcedMaterialOverride(player_teammates_occluded_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_teammates_occluded_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								shine->AlphaModulate(g_Options.colors.chams.player_ally_occluded_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_ally_occluded_shine[0] / 255.f, g_Options.colors.chams.player_ally_occluded_shine[1] / 255.f, g_Options.colors.chams.player_ally_occluded_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
						if (g_Options.chams.player_teammates)
						{
							player_teammates_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							modulate(g_Options.colors.chams.player_ally_visible, player_teammates_type);
							g_StudioRender->ForcedMaterialOverride(player_teammates_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_teammates_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
								shine->AlphaModulate(g_Options.colors.chams.player_ally_visible_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_ally_visible_shine[0] / 255.f, g_Options.colors.chams.player_ally_visible_shine[1] / 255.f, g_Options.colors.chams.player_ally_visible_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
					}
					else
					{
						return;
					}
				}
			}

			if (local)
			{
				if (!g_Options.chams.player_local && !g_Options.chams.player_local_occluded)
				{
					return;
				}
				else
				{
					static IMaterial* player_local_type = nullptr;
					switch (g_Options.chams.player_local_type)
					{
					case 0:
						player_local_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_local_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_local_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_local_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					static IMaterial* player_local_occluded_type = nullptr;
					switch (g_Options.chams.player_local_occluded_type)
					{
					case 0:
						player_local_occluded_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;

					case 1:
						player_local_occluded_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;

					case 2:
						player_local_occluded_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
						break;

					case 3:
						player_local_occluded_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
						break;
					}
					if (player_local_type != nullptr && player_local_occluded_type != nullptr && shine != nullptr)
					{
						if (g_Options.chams.player_local_occluded)
						{
							player_local_occluded_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							modulate(g_Options.colors.chams.player_local_occluded, player_local_occluded_type);
							g_StudioRender->ForcedMaterialOverride(player_local_occluded_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_local_occluded_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								shine->AlphaModulate(g_Options.colors.chams.player_local_occluded_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_local_occluded_shine[0] / 255.f, g_Options.colors.chams.player_local_occluded_shine[1] / 255.f, g_Options.colors.chams.player_local_occluded_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
						if (g_Options.chams.player_local)
						{
							player_local_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							modulate(g_Options.colors.chams.player_local_visible, player_local_type);
							g_StudioRender->ForcedMaterialOverride(player_local_type);
							fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
							if (g_Options.chams.player_local_shine)
							{
								shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
								shine->AlphaModulate(g_Options.colors.chams.player_local_visible_shine[3] / 255.f);
								bool bFound = false;
								auto pVar = shine->FindVar("$envmaptint", &bFound);
								if (bFound)
								{
									(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.player_local_visible_shine[0] / 255.f, g_Options.colors.chams.player_local_visible_shine[1] / 255.f, g_Options.colors.chams.player_local_visible_shine[2] / 255.f);
								}
								g_StudioRender->ForcedMaterialOverride(shine);
							}
						}
					}
					else
					{
						return;
					}
				}
			}
		}
	}
	else if (is_sleeve && g_Options.misc.removals.no_sleeves) {
		auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		if (!material)
			return;
		// 
		// No sleeves.
		// 
		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		g_StudioRender->ForcedMaterialOverride(material);
	}
	else if (is_arm) {
		if (g_Options.misc.removals.no_hands) {
			auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
			if (!material)
				return;
			// 
			// No hands.
			// 
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_StudioRender->ForcedMaterialOverride(material);
		}
		else if (g_Options.chams.arms || g_Options.chams.arms_occluded)
		{
			static IMaterial* arms_type = nullptr;
			switch (g_Options.chams.arms_type)
			{
			case 0:
				arms_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
				break;

			case 1:
				arms_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
				break;

			case 2:
				arms_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
				break;

			case 3:
				arms_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
				break;
			}
			static IMaterial* arms_occluded_type = nullptr;
			switch (g_Options.chams.arms_occluded_type)
			{
			case 0:
				arms_occluded_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
				break;

			case 1:
				arms_occluded_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
				break;

			case 2:
				arms_occluded_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
				break;

			case 3:
				arms_occluded_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
				break;
			}
			if (arms_type != nullptr && arms_occluded_type != nullptr && shine != nullptr)
			{
				if (g_Options.chams.arms_occluded)
				{
					arms_occluded_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
					modulate(g_Options.colors.chams.arms_occluded, arms_occluded_type);
					g_StudioRender->ForcedMaterialOverride(arms_occluded_type);
					fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
					if (g_Options.chams.arms_occluded_shine)
					{
						shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						shine->AlphaModulate(g_Options.colors.chams.arms_occluded_shine[3] / 255.f);
						bool bFound = false;
						auto pVar = shine->FindVar("$envmaptint", &bFound);
						if (bFound)
						{
							(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.arms_occluded_shine[0] / 255.f, g_Options.colors.chams.arms_occluded_shine[1] / 255.f, g_Options.colors.chams.arms_occluded_shine[2] / 255.f);
						}
						g_StudioRender->ForcedMaterialOverride(shine);
					}
				}
				if (g_Options.chams.arms)
				{
					arms_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					modulate(g_Options.colors.chams.arms_visible, arms_type);
					g_StudioRender->ForcedMaterialOverride(arms_type);
					fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
					if (g_Options.chams.arms_shine)
					{
						shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						shine->AlphaModulate(g_Options.colors.chams.arms_visible_shine[3] / 255.f);
						bool bFound = false;
						auto pVar = shine->FindVar("$envmaptint", &bFound);
						if (bFound)
						{
							(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.arms_visible_shine[0] / 255.f, g_Options.colors.chams.arms_visible_shine[1] / 255.f, g_Options.colors.chams.arms_visible_shine[2] / 255.f);
						}
						g_StudioRender->ForcedMaterialOverride(shine);
					}
				}
			}
			else
			{
				return;
			}
		}
	}
	else if (is_weapon) {
		if (g_Options.misc.removals.no_weapon) {
			auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
			if (!material)
				return;
			// 
			// No weapon.
			// 
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_StudioRender->ForcedMaterialOverride(material);
		}
		else if (g_Options.chams.weapon || g_Options.chams.weapon_occluded)
		{
			static IMaterial* weapon_type = nullptr;
			switch (g_Options.chams.weapon_type)
			{
			case 0:
				weapon_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
				break;

			case 1:
				weapon_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
				break;

			case 2:
				weapon_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
				break;

			case 3:
				weapon_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
				break;
			}
			static IMaterial* weapon_occluded_type = nullptr;
			switch (g_Options.chams.weapon_occluded_type)
			{
			case 0:
				weapon_occluded_type = g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
				break;

			case 1:
				weapon_occluded_type = g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
				break;

			case 2:
				weapon_occluded_type = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
				break;

			case 3:
				weapon_occluded_type = g_MatSystem->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);
				break;
			}
			if (weapon_type != nullptr && weapon_occluded_type != nullptr && shine != nullptr)
			{
				if (g_Options.chams.weapon_occluded)
				{
					weapon_occluded_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
					modulate(g_Options.colors.chams.weapon_occluded, weapon_occluded_type);
					g_StudioRender->ForcedMaterialOverride(weapon_occluded_type);
					fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
					if (g_Options.chams.weapon_occluded_shine)
					{
						shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						shine->AlphaModulate(g_Options.colors.chams.weapon_occluded_shine[3] / 255.f);
						bool bFound = false;
						auto pVar = shine->FindVar("$envmaptint", &bFound);
						if (bFound)
						{
							(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.weapon_occluded_shine[0] / 255.f, g_Options.colors.chams.weapon_occluded_shine[1] / 255.f, g_Options.colors.chams.weapon_occluded_shine[2] / 255.f);
						}
						g_StudioRender->ForcedMaterialOverride(shine);
					}
				}
				if (g_Options.chams.weapon)
				{
					weapon_type->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					modulate(g_Options.colors.chams.weapon_visible, weapon_type);
					g_StudioRender->ForcedMaterialOverride(weapon_type);
					fnDME(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);
					if (g_Options.chams.weapon_shine)
					{
						shine->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						shine->AlphaModulate(g_Options.colors.chams.weapon_visible_shine[3] / 255.f);
						bool bFound = false;
						auto pVar = shine->FindVar("$envmaptint", &bFound);
						if (bFound)
						{
							(*(void(__thiscall * *)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, g_Options.colors.chams.weapon_visible_shine[0] / 255.f, g_Options.colors.chams.weapon_visible_shine[1] / 255.f, g_Options.colors.chams.weapon_visible_shine[2] / 255.f);
						}
						g_StudioRender->ForcedMaterialOverride(shine);
					}
				}
			}
			else
			{
				return;
			}
		}
	}
}
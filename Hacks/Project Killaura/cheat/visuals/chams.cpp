#include "chams.hpp"
#include <fstream>

#include "../config/options.hpp"
#include "../hooks/hooks.hpp"
#include "../../helpers/input.hpp"

#include "../legit/backtrack.hpp"

Chams::Chams() {
	materialRegular = g_MatSystem->FindMaterial("debug/debugambientcube");
	materialFlat = g_MatSystem->FindMaterial("debug/debugdrawflat");
	std::ofstream("csgo\\materials\\material_glow.vmt") << R"#("VertexLitGeneric" 
{
 "$additive"               "1"
 "$envmap"                 "models/effects/cube_white"
 "$envmaptint"             "[1 1 1]"
 "$envmapfresnel"          "1"
 "$envmapfresnelminmaxexp" "[0 1 2]"
 "$ignorez"                "0"
}
)#";
	materialGlow = g_MatSystem->FindMaterial("material_glow");
}

Chams::~Chams() { }

void Chams::OverrideMaterial(bool ignoreZ, bool wireframe, int type, const Color& rgba) {
	bool bFound = false;
	IMaterial* material = ChangeMaterial(type);
	IMaterialVar* pMatVar = material->FindVar("$envmaptint", &bFound);

	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);
	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(rgba.r() / 255.0f, rgba.g() / 255.0f, rgba.b() / 255.0f);
	material->AlphaModulate(rgba.a() / 255.0f);
	material->IncrementReferenceCount();

	if (bFound)
		(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pMatVar + 0x2c))((uintptr_t)pMatVar, rgba.r() / 255.f, rgba.g() / 255.f, rgba.b() / 255.f);

	g_MdlRender->ForcedMaterialOverride(material);
}

void Chams::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix) {
	bool is_arm = strstr(info.pModel->szName, "arms") != nullptr;
	bool is_sleeve = strstr(info.pModel->szName, "sleeve") != nullptr;
	bool is_weapon = strstr(info.pModel->szName, "models/weapons/v_") != nullptr;

	static float rainbow;
	rainbow += g_GlobalVars->frametime * 0.03;
	if (rainbow > 1.f)        rainbow = 0.f;
	Color clr = Color::FromHSB(rainbow, 1.f, 1.f);

	if (is_sleeve && Cfg.chams_sleeve_enabled) {
		OverrideMaterial(false, false, Cfg.chams_material_sleeve, Cfg.chams_sleeve_rainbow ? clr : Color(Cfg.color_chams_sleeve));
	}
	else if (is_arm && Cfg.chams_arms_enabled) {
		OverrideMaterial(false, false, Cfg.chams_material_arms, Cfg.chams_arms_rainbow ? clr : Color(Cfg.color_chams_arms));
	}
	else if (is_weapon && Cfg.chams_weapon_enabled && !is_arm) {
		OverrideMaterial(false, false, Cfg.chams_material_weapon, Cfg.chams_weapon_rainbow ? clr : Color(Cfg.color_chams_weapon));
	}
	else if (Cfg.chams_player_enabled) {
		C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(info.entity_index);
		if (!entity || entity->IsDormant() || !entity->IsPlayer()) return;

		const auto enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();
		if (!enemy && Cfg.chams_player_enemies_only) return;

		auto is_local = entity->EntIndex() == g_LocalPlayer->EntIndex();
		if (is_local) return;

		if (Cfg.chams_backtrack && Backtrack::Get().data.count(entity->EntIndex()) > 0) {
			auto& data = Backtrack::Get().data.at(entity->EntIndex());
			if (data.size() > 0) {
				if (Cfg.chams_backtrack == 2) {
					for (auto& record : data) {
						OverrideMaterial(false, Cfg.chams_backtrack_wireframe, Cfg.chams_material_backtrack, Cfg.chams_backtrack_rainbow ? clr : Color(Cfg.color_chams_backtrack));
					}
				}
				else if (Cfg.chams_backtrack == 1) {
					auto& back = data.back();
					OverrideMaterial(false, Cfg.chams_backtrack_wireframe, Cfg.chams_material_backtrack, Cfg.chams_backtrack_rainbow ? clr : Color(Cfg.color_chams_backtrack));
				}
			}
		}

		const auto clr_front = enemy ? Color(Cfg.color_chams_player_enemy_visible) : Color(Cfg.color_chams_player_ally_visible);
		const auto clr_back = enemy ? Color(Cfg.color_chams_player_enemy_occluded) : Color(Cfg.color_chams_player_ally_occluded);

		if (Cfg.chams_player_ignorez) {
			OverrideMaterial(true, Cfg.chams_player_wireframe, Cfg.chams_material, clr_back);
			Hooks::dme::o_dme(g_MdlRender, 0, ctx, state, info, matrix);
			OverrideMaterial(false, Cfg.chams_player_wireframe, Cfg.chams_material, clr_front);
		}
		else OverrideMaterial(false, Cfg.chams_player_wireframe, Cfg.chams_material, clr_front);
	}
}
#include "hooks.hpp"

void __fastcall Hooks::DrawModelExecute(void* _this, int, void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int32_t iFlags)
{
	static auto ofunc = stdrender_hook.get_original<decltype(&DrawModelExecute)>(index::DrawModelExecute);

	static auto flash = g_MatSystem->FindMaterial("effects/flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);
	static auto flash_white = g_MatSystem->FindMaterial("effects/flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);

	static auto vistasmokev1_emods = g_MatSystem->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_OTHER);
	static auto vistasmokev1_emods_impactdust = g_MatSystem->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_OTHER);
	static auto vistasmokev1_smokegrenade = g_MatSystem->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_OTHER);

	static auto xblur_mat = g_MatSystem->FindMaterial("dev/blurfilterx_nohdr", TEXTURE_GROUP_OTHER, true);
	static auto yblur_mat = g_MatSystem->FindMaterial("dev/blurfiltery_nohdr", TEXTURE_GROUP_OTHER, true);
	static auto scope_dot_green = g_MatSystem->FindMaterial("models/weapons/shared/scope/scope_dot_green", TEXTURE_GROUP_OTHER, true);
	static auto scope_dot_red = g_MatSystem->FindMaterial("models/weapons/shared/scope/scope_dot_red", TEXTURE_GROUP_OTHER, true);
	static auto scope = g_MatSystem->FindMaterial("dev/scope_bluroverlay", TEXTURE_GROUP_OTHER, true);

	flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_flash);
	flash_white->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_flash);

	vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Options.misc.removals.no_smoke);
	vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Options.misc.removals.no_smoke);
	vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Options.misc.removals.no_smoke);

	xblur_mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_blur);
	yblur_mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_blur);
	scope_dot_green->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_blur);
	scope_dot_red->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_blur);
	scope->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_Options.misc.removals.no_blur);

	if (g_StudioRender->IsForcedMaterialOverride())
		return ofunc(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);

	g_Chams->OnDrawModelExecute(pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);

	ofunc(g_StudioRender, 0, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags);

	g_StudioRender->ForcedMaterialOverride(nullptr);
}
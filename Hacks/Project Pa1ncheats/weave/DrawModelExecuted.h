#pragma once
#include "Hooks.h"
#include <intrin.h>
extern bool getSmoothMatrix(int ent_index, matrix* out);
extern bool getSmoothMatrixFakelag(matrix* out);

static bool bNightModulationComplete = false;
static bool bNightModulationRemoveComplete = false;

void ClearSmokeEffect()
{
	static auto smokeCount = *reinterpret_cast<uint32_t**>(csgo->Utils.FindPatternIDA(GetModuleHandleA(
		g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::smoke_effect::s().c_str()) + 1);
	*(int*)smokeCount = 0;
}

CChams* Chams = new CChams();

enum EDrawModelFlags : unsigned int
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	STUDIO_SKIP_DECALS = 0x10000000,
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	STUDIO_TRANSPARENCY = 0x80000000
};

void __fastcall Hooked_DrawModel(void* ecx, void*, void* results, const DrawModelInfo_t& info,
	matrix* bone_to_world, float* flex_weights, float* flex_delayed_weights, const Vector& model_origin, int flags)
{
	auto DrawModel = g_pStudioRenderHook->GetOriginal<fnDrawModel>(29);

	if (csgo->DoUnload || !csgo->local)
		return DrawModel(ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags);

	static bool fl = false;
	if (!fl)
	{
		std::ofstream("csgo\\materials\\regular_glow.vmt") << R"#("VertexLitGeneric" {
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "1"
		})#";

		std::ofstream("csgo\\materials\\regular_ref.vmt") << R"#("VertexLitGeneric" {
			"$basetexture"				    "vgui/white"
			"$envmap"						"env_cubemap"
			"$envmaptint"                   "[.10 .10 .10]"
			"$pearlescent"					"0"
			"$phong"						"1"
			"$phongexponent"				"10"
			"$phongboost"					"1.0"
			"$rimlight"					    "1"
			"$rimlightexponent"		        "1"
			"$rimlightboost"		        "1"
			"$model"						"1"
			"$nocull"						"0"
			"$halflambert"				    "1"
			"$lightwarptexture"             "metalic"
		})#";

		std::ofstream("csgo/materials/glowOverlay.vmt") << R"#("VertexLitGeneric" {
 
     "$additive" "1"
     "$envmap" "models/effects/cube_white"
     "$envmaptint" "[1 1 1]"
     "$envmapfresnel" "1"
     "$envmapfresnelminmaxexp" "[0 1 2]"
     "$alpha" "0.8"
      })#";

		fl = true;
	}

	if (info.pStudioHdr != nullptr && csgo->local != nullptr) {
		if (interfaces.engine->IsConnected() && interfaces.engine->IsInGame()) {

			const auto mN = info.pStudioHdr->name;

			if (flags & (STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS))
				return DrawModel(ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags);

			Chams->Draw(ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags);

			if (csgo->local && csgo->local->isAlive() && vars.visuals.interpolated_model && interfaces.input->m_fCameraInThirdPerson)
			{
				if (strstr(mN, "player/") != nullptr)
				{
					csgo->local->SetAbsOrigin(csgo->local->GetOrigin());
					DrawModel(ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags);
					csgo->local->SetAbsOrigin(csgo->local->GetAbsOriginVec());
				}
			}

			if (strstr(mN, "props") != nullptr)
			{
				interfaces.render_view->SetBlend(vars.visuals.nightmode_prop_color.get_alpha() / 255.f);
			}

			static IMaterial* mat1 = interfaces.material_system->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", "Other textures");
			static IMaterial* mat2 = interfaces.material_system->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", "Other textures");
			static IMaterial* mat3 = interfaces.material_system->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", "Other textures");
			static IMaterial* mat4 = interfaces.material_system->FindMaterial("particle/vistasmokev1/vistasmokev1_fire", "Other textures");

			mat1->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, (vars.visuals.remove & 2));
			mat2->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, (vars.visuals.remove & 2));
			mat3->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, (vars.visuals.remove & 2));
			mat4->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, (vars.visuals.remove & 2));
			if (vars.visuals.remove & 2)
				ClearSmokeEffect();

			static bool m_Init = false;
			if (!m_Init)
			{
				static ConVar* asdasd = interfaces.cvars->FindVar(hs::r_DrawSpecificStaticProp::s().c_str());
				asdasd->m_fnChangeCallbacks.m_Size = 0;
				asdasd->SetValue(1);
				m_Init = true;
			}
		}
	}
	DrawModel(ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags);
	interfaces.models.model_render->ForcedMaterialOverride(nullptr);
}

void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& info, matrix* pCustomBoneToWorld)
{
	static auto DrawModelExecute = g_pModelRenderHook->GetOriginal< DrawModelExecuteFn >(21);

	if (csgo->DoUnload)
		return DrawModelExecute(interfaces.models.model_render, ctx, state, info, pCustomBoneToWorld);

	static bool fl = false;
	if (!fl)
	{
		std::ofstream("csgo\\materials\\regular_glow.vmt") << R"#("VertexLitGeneric" {
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "1"
		})#";

		std::ofstream("csgo\\materials\\regular_ref.vmt") << R"#("VertexLitGeneric" {
			"$basetexture"				    "vgui/white"
			"$envmap"						"env_cubemap"
			"$envmaptint"                   "[.10 .10 .10]"
			"$pearlescent"					"0"
			"$phong"						"1"
			"$phongexponent"				"10"
			"$phongboost"					"1.0"
			"$rimlight"					    "1"
			"$rimlightexponent"		        "1"
			"$rimlightboost"		        "1"
			"$model"						"1"
			"$nocull"						"0"
			"$halflambert"				    "1"
			"$lightwarptexture"             "metalic"
		})#";

		std::ofstream("csgo/materials/glowOverlay.vmt") << R"#("VertexLitGeneric" {
 
     "$additive" "1"
     "$envmap" "models/effects/cube_white"
     "$envmaptint" "[1 1 1]"
     "$envmapfresnel" "1"
     "$envmapfresnelminmaxexp" "[0 1 2]"
     "$alpha" "0.8"
      })#";

		fl = true;
	}

	if (interfaces.engine->IsInGame() && csgo->local && info.pModel)
	{
		const auto mN = info.pModel->name/*interfaces.models.model_info->GetModelName(info.pModel)*/;

		

	}

	DrawModelExecute(interfaces.models.model_render, ctx, state, info, pCustomBoneToWorld);
	interfaces.models.model_render->ForcedMaterialOverride(nullptr);
}

// disable model occlusion
#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
int __fastcall Hooked_ListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax) 
{
	static auto ListLeavesInBox = g_pQueryHook->GetOriginal< ListLeavesInBoxFn >(6);

	// occulusion getting updated on player movement/angle change,
	// in RecomputeRenderableLeaves ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674 );
	// check for return in CClientLeafSystem::InsertIntoTree
	if (*(uint32_t*)_ReturnAddress() != 0x14244489) // 89 44 24 14 ( 0x14244489 ) - new / 8B 7D 08 8B ( 0x8B087D8B ) - old
		return ListLeavesInBox(bsp, mins, maxs, pList, listMax);

	// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
	auto info = *(RenderableInfo_t**)((uintptr_t)_AddressOfReturnAddress() + 0x14);
	if (!info || !info->m_pRenderable)
		return ListLeavesInBox(bsp, mins, maxs, pList, listMax);

	// check if disabling occulusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
	auto base_entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
	if (!base_entity || !base_entity->IsPlayer())
		return ListLeavesInBox(bsp, mins, maxs, pList, listMax);

	// fix render order, force translucent group ( https://www.unknowncheats.me/forum/2429206-post15.html )
	// AddRenderablesToRenderLists: https://i.imgur.com/hcg0NB5.png ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473 )
	info->m_Flags &= ~0x100;
	info->m_Flags2 |= 0xC0;

	// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
	static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
	auto count = ListLeavesInBox(bsp, map_min, map_max, pList, listMax);
	return count;
}
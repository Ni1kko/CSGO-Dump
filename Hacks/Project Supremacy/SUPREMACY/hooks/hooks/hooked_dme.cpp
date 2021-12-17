#include "..\hooks.hpp"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#define XorStr( s ) ( s )
using DrawModelExecute_t = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
Color urmamasuckmylargegenetalia(int speed, int offset)
{
	float hue = (float)((GetCurrentTime() + offset) % speed);
	hue /= speed;
	return Color::FromHSB(hue, 1.0F, 1.0F);
}
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void InitKeyValues(KeyValues* kv_, std::string name_)
{
	static auto address = util::FindSignature("client_panorama.dll", "55 8B EC 51 33 C0 C7 45");
	using Fn = void(__thiscall*)(void* thisptr, const char* name);
	reinterpret_cast<Fn>(address)(kv_, name_.c_str());
}

void LoadFromBuffer(KeyValues* vk_, std::string name_, std::string buffer_)
{
	static auto address = util::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	using Fn = void(__thiscall*)(
		void* thisptr, const char* resourceName,
		const char* pBuffer, void* pFileSystem,
		const char* pPathID, void* pfnEvaluateSymbolProc);

	reinterpret_cast<Fn>(address)(
		vk_, name_.c_str(), buffer_.c_str(), nullptr, nullptr, nullptr);
}
//IMaterial *CreateMaterialLit()
//{
//	static int created = 0;
//
//	//i have no fucking idea why this keeps being flat
//
//	static const char tmp[] =
//	{
//		"\"%s\"\
//		\n{\
//		\n\t\"$basetexture\" \"vgui/white_additive\"\
//		\n\t\"$envmap\" \"\"\
//		\n\t\"$model\" \"1\"\
//		\n\t\"$nocull\" \"0\"\
//		\n\t\"$ambientocclusion\" \"1\"\
//		\n\t\"$selfillum\" \"1\"\
//		\n\t\"$halflambert\" \"1\"\
//        \n\t\"$flat\" \"0\"\
//		\n\t\"$nofog\" \"1\"\
//		\n\t\"$phong\" \"1\"\
//		\n\t\"$phongboost\" \"1\"\
//		\n\t\"$reflectivity\" \"[.89 .89 .89]\"\
//		\n\t\"$ignorez\" \"%i\"\
//		\n\t\"$znearer\" \"0\"\
//		\n\t\"$wireframe\" \"%i\"\
//        \n}\n"
//	};
//
//	char* baseType = "VertexLitGeneric";
//	char material[512];
//	sprintf_s(material, sizeof(material), tmp, baseType, 0, 0);
//
//	char name[512];
//	sprintf_s(name, sizeof(name), "#skeetus%i.vmt", created);
//	++created;
//
//	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
//	InitKeyValues(keyValues, baseType);
//	LoadFromBuffer(keyValues, name, material);
//
//	IMaterial *createdMaterial = g_csgo.m_materialsystem()->CreateMaterial(name, keyValues);
//	createdMaterial->IncrementReferenceCount();
//
//	return createdMaterial;
//}
void __stdcall hooks::hooked_dme(IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & info, matrix3x4_t * bone_to_world) {
	static auto original_fn = modelrender_hook->get_func_address< DrawModelExecute_t >(21);

	//auto btchams = [&](player_t * e) -> bool
	//{
	//	auto & player = lagcompensation::get().players[e->EntIndex()];
	//	auto track = player.get_valid_track();

	//	if (!player.m_e || track.empty())
	//		return false;

	//	for (int i = track.size() - 1; i >= 0; i--) {
	//		auto & record = track.at(i);

	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, record.m_bone_matrix);

	//		return true;
	//	}

	//	return false;
	//};
	//float flColor[3] = { 0.f };
	//bool isplayer = strstr(info.pModel->name, "models/player") != nullptr;
	//auto model_entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(info.entity_index));
	//static IMaterial* mat = CreateMaterialLit();
	//const char* ModelName = g_csgo.m_modelinfo()->GetModelName((model_t*)info.pModel);

	//if (g_ctx.m_local->m_bIsScoped() && isplayer && model_entity != nullptr && model_entity->m_iTeamNum() == g_ctx.m_local->m_iTeamNum() && model_entity == g_ctx.m_local) {
	//	if (g_cfg.esp.transparency_scoped && g_ctx.m_local->is_alive())
	//		g_csgo.m_renderview()->SetBlend(g_cfg.esp.transparency_scoped_alpha * 0.01f);
	//}
	//int gunchams = g_cfg.esp.gunchams;
	//int handchams = g_cfg.esp.handchams;
	//if (gunchams != 0 && strstr(ModelName, "models/weapons/v_") && !strstr(ModelName, "arms") && g_ctx.m_local && g_ctx.m_local->is_alive())
	//{
	//	switch (gunchams) // this shit was done in "else if"s with " == " instead of A > B or switch statements which is fucking HECKIN trash for optimization
	//	{
	//	case 1:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 2:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	case 3:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 4:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 5:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, true);
	//		Color colRainbow = urmamasuckmylargegenetalia(5000, 1022);
	//		flColor[0] = colRainbow.r() / 255.f;
	//		flColor[1] = colRainbow.g() / 255.f;
	//		flColor[2] = colRainbow.b() / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	case 6:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(0, 0, 0);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	}
	//}
	//if (handchams != 0 && strstr(ModelName, XorStr("arms")) && g_ctx.m_local && g_ctx.m_local->is_alive())
	//{
	//	switch (handchams)
	//	{
	//	case 1:
	//	{
	//		flColor[0] = g_cfg.esp.handchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.handchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.handchamscolor[2] / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 2:
	//	{
	//		flColor[0] = g_cfg.esp.handchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.handchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.handchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 3:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 4:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	}
	//}
	auto btchams = [&](player_t * e) -> bool
	{
		auto & player = lagcompensation::get().players[e->EntIndex()];
		auto track = player.get_valid_track();

		if (!player.m_e || track.empty())
			return false;

		for (int i = track.size() - 1; i >= 0; i--) {
			auto & record = track.at(i);

			original_fn(g_csgo.m_modelrender(), ctx, state, info, record.m_bone_matrix);

			return true;
		}

	};
	float flColor[3] = { 0.f };
	//static IMaterial* mat = CreateMaterialLit();
	const char* ModelName = g_csgo.m_modelinfo()->GetModelName((model_t*)info.pModel);

	int gunchams = g_cfg.esp.gunchams;
	int handchams = g_cfg.esp.handchams;
	auto model_entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(info.entity_index));
	bool isplayer = strstr(info.pModel->name, "models/player") != nullptr;

	//if (gunchams != 0 && strstr(ModelName, "models/weapons/v_") && !strstr(ModelName, "arms") && g_ctx.m_local && g_ctx.m_local->is_alive())
	//{
	//	switch (gunchams) // this shit was done in "else if"s with " == " instead of A > B or switch statements which is fucking HECKIN trash for optimization
	//	/{
	//	case 1:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	///		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 2:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	case 3:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 4:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 5:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, true);
	//		Color colRainbow = urmamasuckmylargegenetalia(5000, 1022);
	//		flColor[0] = colRainbow.r() / 255.f;
	//		flColor[1] = colRainbow.g() / 255.f;
	//		flColor[2] = colRainbow.b() / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	case 6:
	//	{
	//		flColor[0] = g_cfg.esp.gunchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.gunchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.gunchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(0, 0, 0);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	}
	//}
	//if (handchams != 0 && strstr(ModelName, XorStr("arms")) && g_ctx.m_local && g_ctx.m_local->is_alive())
	//{
	//	switch (handchams)
	//	{
	//	case 1:
	//	{
	//		flColor[0] = g_cfg.esp.handchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.handchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.handchamscolor[2] / 255.f;
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 2:
	//	{
	//		flColor[0] = g_cfg.esp.handchamscolor[0] / 255.f;
	//		flColor[1] = g_cfg.esp.handchamscolor[1] / 255.f;
	//		flColor[2] = g_cfg.esp.handchamscolor[2] / 255.f;
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		g_csgo.m_renderview()->SetColorModulation(flColor);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(mat);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 3:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;

	//	case 4:
	//	{
	//		g_csgo.m_renderview()->SetBlend(1.f);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		IMaterial *material = g_csgo.m_materialsystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	//		g_csgo.m_modelrender()->ForcedMaterialOverride(material);
	//		original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);
	//	}
	//	break;
	//	}
	//}

	if (isplayer) {
		if (!model_entity->valid(true) && model_entity != g_ctx.m_local && g_cfg.esp.dont_render_teammates)
			return;

		if (g_ctx.backtrack_chams && model_entity->valid(true)) {
			if (btchams(model_entity))
				return;
		}
	}

	original_fn(g_csgo.m_modelrender(), ctx, state, info, bone_to_world);

}
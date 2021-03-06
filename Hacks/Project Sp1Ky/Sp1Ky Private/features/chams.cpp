#include "chams.hpp"
#include <fstream>
#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../hooks.hpp"
#include "../helpers/input.hpp"
#include "../features/XorCompileTime.hpp"
Chams::Chams()
{
    std::ofstream("csgo\\materials\\simple_regular_reflective.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    std::ofstream("csgo\\materials\\simple_ignorez_reflective.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    std::ofstream("csgo\\materials\\simple_ignorez_reflective.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    materialRegular = g_MatSystem->FindMaterial(XorStr("simple_regular_reflective"), TEXTURE_GROUP_MODEL);
    materialRegularIgnoreZ = g_MatSystem->FindMaterial(XorStr("simple_ignorez_reflective"), TEXTURE_GROUP_MODEL);
    materialFlatIgnoreZ = g_MatSystem->FindMaterial(XorStr("simple_flat_ignorez"), TEXTURE_GROUP_MODEL);
    materialFlat = g_MatSystem->FindMaterial(XorStr("simple_ignorez_reflective"), TEXTURE_GROUP_MODEL);
}
Chams::~Chams()
{
    std::remove(XorStr("csgo\\materials\\simple_regular_reflective.vmt"));
    std::remove(XorStr("csgo\\materials\\simple_ignorez_reflective.vmt"));
    std::remove(XorStr("csgo\\materials\\simple_ignorez_reflective.vmt"));
    std::remove(XorStr("csgo\\materials\\simple_flat_ignorez.vmt"));
}
void Chams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba)
{
    IMaterial* material = nullptr;
    if(flat) {
        if(ignoreZ)
            material = materialFlatIgnoreZ;
        else
            material = materialFlat;
    } else {
        if(ignoreZ)
            material = materialRegularIgnoreZ;
        else
            material = materialRegular;
    }
    if(glass) {
        material = materialFlat;
        material->AlphaModulate(0.45f);
    } else {
        material->AlphaModulate(
            rgba.a() / 255.0f);
    }
    material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
    material->ColorModulate(
        rgba.r() / 255.0f,
        rgba.g() / 255.0f,
        rgba.b() / 255.0f);
    g_MdlRender->ForcedMaterialOverride(material);
}
void Chams::OnDrawModelExecute(
    IMatRenderContext* ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &info,
    matrix3x4_t *matrix)
{
    static auto fnDME = Hooks::mdlrender_hook.get_original<Hooks::DrawModelExecute>(index::DrawModelExecute);
    const auto mdl = info.pModel;
    bool is_arm = strstr(mdl->szName, XorStr("arms"))             != nullptr;
    bool is_player = strstr(mdl->szName, XorStr("models/player")) != nullptr;
    bool is_sleeve = strstr(mdl->szName, XorStr("sleeve"))        != nullptr;
    if(is_player && g_Options.legit_dog_cham_enb) {
        auto ent = C_BasePlayer::GetPlayerByIndex(info.entity_index);
        if(ent && g_LocalPlayer && ent->IsAlive()) {
            const auto enemy = ent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();
            if(!enemy && g_Options.legit_dog_cham_only_en)
                return;
            const auto clr_front = enemy ? g_Options.legit_dog_cham_entm_vis : g_Options.legit_dog_cham_tm_vis;
            const auto clr_back = enemy ? g_Options.legit_dog_cham_entm_invis : g_Options.legit_dog_cham_tm_invs;
            if(g_Options.legit_dog_ignz) {
                OverrideMaterial(
                    true,
                    g_Options.legit_dog_flat,
                    g_Options.legit_dog_kostickovany,
                    false,
                    clr_back);
                fnDME(g_MdlRender, ctx, state, info, matrix);
                OverrideMaterial(
                    false,
                    g_Options.legit_dog_flat,
                    g_Options.legit_dog_kostickovany,
                    false,
                    clr_front);
            } else {
                OverrideMaterial(
                    false,
                    g_Options.legit_dog_flat,
                    g_Options.legit_dog_kostickovany,
                    g_Options.legit_dog_sklicko,
                    clr_front);
            }
        }
    } else if(is_sleeve && g_Options.legit_chsm_amr_en) {
        auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
        if(!material)
            return;
        material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
        g_MdlRender->ForcedMaterialOverride(material);
    } else if(is_arm) {
        auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
        if(!material)
            return;
        if(g_Options.misc_no_hands) {
            material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
            g_MdlRender->ForcedMaterialOverride(material);
        } else if(g_Options.legit_chsm_amr_en) {
            if(g_Options.legit_chsm_amr_igz) {
                OverrideMaterial(
                    true,
                    g_Options.legit_chsm_amr_fl,
                    g_Options.legit_chsm_amr_kostickovany,
                    false,
                    g_Options.clcl_legit_chsm_amr_invs);
                fnDME(g_MdlRender, ctx, state, info, matrix);
                OverrideMaterial(
                    false,
                    g_Options.legit_chsm_amr_fl,
                    g_Options.legit_chsm_amr_kostickovany,
                    false,
                    g_Options.cl_legit_chsm_amr_visvis);
            } else {
                OverrideMaterial(
                    false,
                    g_Options.legit_chsm_amr_fl,
                    g_Options.legit_chsm_amr_kostickovany,
                    g_Options.legit_chsm_amr_sklicko,
                    g_Options.cl_legit_chsm_amr_visvis);
            }
        }
    }
}


























































































































































































































































































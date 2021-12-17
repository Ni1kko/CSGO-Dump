#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"

struct Material {
    bool enabled{ false };
    bool healthBased{ false };
    Color color = Color(255, 255, 255);
    bool blinking{ false };
    int material{ 0 };
    bool wireframe{ false };
};
struct ConfChams {
    std::array<Material, 2> materials;
    bool DoNotRemoveModel{ false };
};

class Chams {
public:
    ConfChams chams_player_allies_all;
    ConfChams chams_player_allies_visible;
    ConfChams chams_player_allies_occluded;

    ConfChams chams_player_enemies_all;
    ConfChams chams_player_enemies_visible;
    ConfChams chams_player_enemies_occluded;

    ConfChams chams_player_backtrack;

    ConfChams chams_player_local_player;
    ConfChams chams_weapons;
    ConfChams chams_hands;
    ConfChams chams_sleeves;
private:
    int menuMaterial = 1;
public:
    void Menu();
    void MenuColors();
public:
    Chams() noexcept;
    bool render(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept;
private:
    bool renderPlayers(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept;
    void renderWeapons(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept;
    void renderHands(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept;
    void draw_hit_matrix();
    void renderSleeves(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept;

    IMaterial* normal;
    IMaterial* flat;
    IMaterial* animated;
    IMaterial* glass;
    IMaterial* crystal;
    IMaterial* silver;
    IMaterial* glow_armsrace;
    IMaterial* glow;
    IMaterial* gloss;

    constexpr auto dispatchMaterial(int id) const noexcept
    {
        switch (id) {
        default:
        case 0: return normal;
        case 1: return flat;
        case 2: return animated;
        case 3: return glass;
        case 4: return crystal;
        case 5: return silver;;
        case 6: return glow_armsrace;
        case 7: return glow;
        case 8: return gloss;
        }
    }

    void applyChams(const Material& chams, bool ignorez, int health = 0) const noexcept
    {
        auto material = dispatchMaterial(chams.material);
        bool found;
        auto color = material->FindVar("$envmaptint", &found);

      

        if (chams.healthBased && health) {
            material->ColorModulate(1.0f - health / 100.0f, health / 100.0f, 0.0f);
            if (chams.material == 9 || chams.material == 10 || found)
                color->SetVecValue(1.0f - health / 100.0f, health / 100.0f, 0.0f);
        }
        else {

            material->ColorModulate(chams.color[0] / 255.f, chams.color[1] / 255.f, chams.color[2] / 255.f);
            if (chams.material == 9 || chams.material == 10 || found)
                color->SetVecValue(chams.color[0] / 255.f, chams.color[1] / 255.f, chams.color[2] / 255.f);
        }

        material->AlphaModulate((chams.color.a() / 255.f) *(chams.blinking ? sinf(g_GlobalVars->curtime * 5) * 0.5f + 0.5f : 1.0f));

        material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
        material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, chams.wireframe);
        g_MdlRender->ForcedMaterialOverride(material);
    }
};

extern Chams* g_Chams;
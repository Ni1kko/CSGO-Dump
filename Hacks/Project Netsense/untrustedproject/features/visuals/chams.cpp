#include "chams.hpp"
#include "../../hooks.hpp"
#include "../legitbot/aimbot.hpp"
#include "../../addons/ui_addons/ui_addons.hpp"
#include "../misc/misc.hpp"


Chams* g_Chams;


Chams::Chams() noexcept
{
    KeyValues* normalKV = new KeyValues("chamsNormal");
    normalKV->LoadFromBuffer(normalKV, "chamsNormal", "VertexLitGeneric { }");
    normal = g_MatSystem->CreateMaterial("chamsNormal", normalKV);
    normal->IncrementReferenceCount();
    
    KeyValues* flatKV = new KeyValues("chamsFlat");
    flatKV->LoadFromBuffer(flatKV, "chamsFlat", "UnlitGeneric { }");
    flat = g_MatSystem->CreateMaterial("chamsFlat", flatKV);
    flat->IncrementReferenceCount();

    KeyValues* animatedKV = new KeyValues("chamsAnimated");
    animatedKV->LoadFromBuffer(animatedKV, "chamsAnimated", "VertexLitGeneric { $envmap editor/cube_vertigo $envmapcontrast 1 $envmaptint \"[.7 .7 .7]\" $basetexture dev/zone_warning proxies { texturescroll { texturescrollvar $basetexturetransform texturescrollrate 0.6 texturescrollangle 90 } } }");
    animated = g_MatSystem->CreateMaterial("chamsAnimated", animatedKV);
    animated->IncrementReferenceCount();
    
    glass = g_MatSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_MODEL);
    
    crystal = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL);
    
    silver = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/silver_plain", TEXTURE_GROUP_MODEL);
    
    glow_armsrace = g_MatSystem->FindMaterial("dev/glow_armsrace", TEXTURE_GROUP_MODEL);

    KeyValues* glowKV = new KeyValues("chamsGlow");
    glowKV->LoadFromBuffer(glowKV, "chamsGlow", R"#("VertexLitGeneric" 
        {
          "$additive" "1"
          "$envmap" "models/effects/cube_white"
          "$envmaptint" "[1 1 1]"
          "$envmapfresnel" "1"
          "$envmapfresnelminmaxexp" "[0 1 2]"
          "$alpha" "1"  
        })#");
    glow = g_MatSystem->CreateMaterial("chamsGlow", glowKV);
    glow->IncrementReferenceCount();

    gloss = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_MODEL);
}

bool Chams::render(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept
{
    if (std::strstr(info.pModel->szName, "models/player"))
        return renderPlayers(ctx, state, info, customBoneToWorld);
    else if (std::strstr(info.pModel->szName, "sleeve"))
        renderSleeves(ctx, state, info, customBoneToWorld);
    else if (std::strstr(info.pModel->szName, "arms"))
        renderHands(ctx, state, info, customBoneToWorld);
    else if (std::strstr(info.pModel->szName, "models/weapons/v_")
        && !std::strstr(info.pModel->szName, "tablet")
        && !std::strstr(info.pModel->szName, "parachute")
        && !std::strstr(info.pModel->szName, "fists"))
        renderWeapons(ctx, state, info, customBoneToWorld);
    return true;
}

bool Chams::renderPlayers(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept
{
    if (g_MdlRender->IsForcedMaterialOverride())
        return true;

    C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(info.entity_index);
    if (!entity || entity->IsDormant() || !entity->IsPlayer()) return true;

    bool needRedraw = true;
    static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

    auto applied{ false };
    for (size_t i = 0; i < chams_player_allies_all.materials.size(); ++i) {
        /*if (const auto activeWeapon = entity->m_hActiveWeapon().Get(); activeWeapon && activeWeapon->GetClientClass()->m_ClassID == WEAPON_C4 && activeWeapon->IsPlantedC4()) {
            if (chams[PLANTING_ALL].materials[i].enabled) {
                if (applied)
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                auto elem = chams[PLANTING_ALL].materials[i];
                applyChams(elem, true, entity->m_iHealth());
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams[PLANTING_ALL].materials[i], false, entity->m_iHealth());
                applied = true;
            }
            else {
                if (chams[PLANTING_OCCLUDED].materials[i].enabled) {
                    if (applied)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams[PLANTING_OCCLUDED].materials[i], true, entity->m_iHealth());
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    if (!chams[PLANTING_VISIBLE].materials[i].enabled)
                        g_MdlRender->ForcedMaterialOverride(nullptr);
                    applied = true;
                }
                if (chams[PLANTING_VISIBLE].materials[i].enabled) {
                    if (applied)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams[PLANTING_VISIBLE].materials[i], false, entity->m_iHealth());
                    applied = true;
                }
            }
        }
        else if (entity->m_hBombDefuser()) {
            if (chams[DEFUSING_ALL].materials[i].enabled) {
                if (applied)
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams[DEFUSING_ALL].materials[i], true, entity->m_iHealth());
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams[DEFUSING_ALL].materials[i], false, entity->m_iHealth());
                applied = true;
            }
            else {
                if (chams[DEFUSING_OCCLUDED].materials[i].enabled) {
                    if (applied)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams[DEFUSING_OCCLUDED].materials[i], true, entity->m_iHealth());
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    if (!chams[DEFUSING_VISIBLE].materials[i].enabled)
                        g_MdlRender->ForcedMaterialOverride(nullptr);
                    applied = true;
                }
                if (chams[DEFUSING_VISIBLE].materials[i].enabled) {
                    if (applied)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams[DEFUSING_VISIBLE].materials[i], false, entity->m_iHealth());
                    applied = true;
                }
            }
        }
        else */
        if (chams_player_backtrack.materials[i].enabled && !g_Backtrack->data.empty() && entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum()) {
            if (applied || chams_player_backtrack.DoNotRemoveModel)
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
            auto data = g_Backtrack->data[entity->EntIndex()];
            if (data.size() > 0) {
                if (g_Misc->options.chams_backtrack_all_ticks) {
                    for (auto& record : data) {
                        applyChams(chams_player_backtrack.materials[i], true, entity->m_iHealth());
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), record.boneMatrix);
                        g_MdlRender->ForcedMaterialOverride(nullptr);
                        applied = true;
                    }
                }
                else {
                    auto& back = data.back();
                    applyChams(chams_player_backtrack.materials[i], true, entity->m_iHealth());
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), back.boneMatrix);
                    g_MdlRender->ForcedMaterialOverride(nullptr);
                    applied = true;
                }
            }
        }
        if (entity == g_LocalPlayer) {
            if (chams_player_local_player.materials[i].enabled) {
                if (applied || chams_player_local_player.DoNotRemoveModel)
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams_player_local_player.materials[i], false, entity->m_iHealth());
                applied = true;
            }
        }
        else if (entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum()) {
            if (chams_player_enemies_all.materials[i].enabled) {
                if (applied || chams_player_enemies_all.DoNotRemoveModel)
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams_player_enemies_all.materials[i], true, entity->m_iHealth());
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams_player_enemies_all.materials[i], false, entity->m_iHealth());
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                needRedraw = false;
                applied = true;
            }
            else {
                if (chams_player_enemies_occluded.materials[i].enabled) {
                    if (applied || chams_player_enemies_occluded.DoNotRemoveModel)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams_player_enemies_occluded.materials[i], true, entity->m_iHealth());
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    if (!chams_player_enemies_visible.materials[i].enabled)
                        g_MdlRender->ForcedMaterialOverride(nullptr);
                    applied = true;
                }
                if (chams_player_enemies_visible.materials[i].enabled) {
                    if (!g_Aimbot.IsLineGoesThroughSmoke(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_CHEST))) {
                        if (applied || chams_player_enemies_visible.DoNotRemoveModel)
                            fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                        applyChams(chams_player_enemies_visible.materials[i], false, entity->m_iHealth());
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    }
                    needRedraw = false;
                    applied = true;
                }
            }
        }
        else {
            if (chams_player_allies_all.materials[i].enabled) {
                if (applied || chams_player_allies_all.DoNotRemoveModel)
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams_player_allies_all.materials[i], true, entity->m_iHealth());
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                applyChams(chams_player_allies_all.materials[i], false, entity->m_iHealth());
                applied = true;
            }
            else {
                if (chams_player_allies_occluded.materials[i].enabled) {
                    if (applied || chams_player_allies_occluded.DoNotRemoveModel)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams_player_allies_occluded.materials[i], true, entity->m_iHealth());
                    fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    if (!chams_player_allies_visible.materials[i].enabled)
                        g_MdlRender->ForcedMaterialOverride(nullptr);
                    applied = true;
                }
                if (chams_player_allies_visible.materials[i].enabled) {
                    if (applied || chams_player_allies_visible.DoNotRemoveModel)
                        fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
                    applyChams(chams_player_allies_visible.materials[i], false, entity->m_iHealth());
                    applied = true;
                }
            }
        }
    }
    return needRedraw;
}

void Chams::renderWeapons(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept
{
    if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || g_LocalPlayer->m_bIsScoped())
        return;

    static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

    bool applied = false;
    for (size_t i = 0; i < chams_weapons.materials.size(); ++i) {
        if (chams_weapons.materials[i].enabled) {
            if (applied || chams_weapons.DoNotRemoveModel)
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
            applyChams(chams_weapons.materials[i], false, g_LocalPlayer->m_iHealth());
            applied = true;
        }
    }
}

void Chams::renderHands(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept
{
    if (!g_LocalPlayer || !g_LocalPlayer->IsAlive()) return;

    static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

    bool applied = false;
    for (size_t i = 0; i < chams_hands.materials.size(); ++i) {
        if (chams_hands.materials[i].enabled) {
            if (applied || chams_hands.DoNotRemoveModel)
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
            applyChams(chams_hands.materials[i], false, g_LocalPlayer->m_iHealth());
            applied = true;
        }
    }
}



void Chams::renderSleeves(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* customBoneToWorld) const noexcept
{
    if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
        return;

    static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

    bool applied = false;
    for (size_t i = 0; i < chams_sleeves.materials.size(); ++i) {
        if (chams_sleeves.materials[i].enabled) {
            if (applied || chams_sleeves.DoNotRemoveModel)
                fnDME(g_MdlRender, 0, ctx, state, std::cref(info), customBoneToWorld);
            applyChams(chams_sleeves.materials[i], false, g_LocalPlayer->m_iHealth());
            applied = true;
        }
    }
}

struct chamsExtraParams {
    bool* value;
    std::string name;
};

void Chams::Menu() {

    if (ImGui::ArrowButton("##left", ImGuiDir_Left) && menuMaterial > 1)
        --menuMaterial;
    ImGui::SameLine();
    ImGui::Text("%d", menuMaterial);
    ImGui::SameLine();
    if (ImGui::ArrowButton("##right", ImGuiDir_Right) && menuMaterial < int(chams_player_allies_all.materials.size()))
        ++menuMaterial;


    int matWorking = menuMaterial - 1;
    const char* materials[] = {
        ("Normal"),
        ("Flat"),
        ("Animated"),
        ("Glass"),
        ("Crystal"),
        ("Silver"),
        ("thin Glow"),
        ("Glow"),
        ("Gloss")
    };

    auto chamsSettings = [&](std::string show, std::string safe, ConfChams* settings, std::vector<chamsExtraParams> extraParams = {}) -> void {
        ImGui::Checkbox(show.c_str(), &settings->materials[matWorking].enabled);

        if (settings->materials[matWorking].enabled) {
            ImGui::SameLine();

            bool openPopup = ImGui::Button(("Settings" + safe).c_str());

            if (openPopup)
                ImGui::OpenPopup((safe + "##popup").c_str());

            if (ImGui::BeginPopup((safe + "##popup").c_str())) {
                ImGui::Text(show.c_str());
                ImGui::Separator();

                ImGui::PushItemWidth(120.f);
                ImGui::Combo(("Material" + safe).c_str(), &settings->materials[matWorking].material, materials, ARRAYSIZE(materials));
                ImGui::PopItemWidth();
                
                ImGui::ColorEdit("Color", settings->materials[matWorking].color);
                ImGui::Checkbox(("Do not remove model" + safe).c_str(), &settings->DoNotRemoveModel);
                ImGui::Checkbox(("Wireframe" + safe).c_str(), &settings->materials[matWorking].wireframe);
                ImGui::Checkbox(("Pulsing" + safe).c_str(), &settings->materials[matWorking].blinking);
                ImGui::Checkbox(("Health-Based" + safe).c_str(), &settings->materials[matWorking].healthBased);

                if (!extraParams.empty()) {
                    ImGui::Spacing();
                    for (auto& data : extraParams) {
                        ImGui::Checkbox(data.name.c_str(), data.value);
                    }
                }

                ImGui::Separator();

                ImGui::EndPopup();
            }
        }
    };

    chamsSettings("Allies all", "##alliesall", &chams_player_allies_all);
    chamsSettings("Allies visible", "##alliesvis", &chams_player_allies_visible);
    chamsSettings("Allies occluded", "##alliesoccluded", &chams_player_allies_occluded);

    ImGui::Spacing();

    chamsSettings("Enemies all", "##enemiesall", &chams_player_enemies_all);
    chamsSettings("Enemies Visible", "##enemiesvisible", &chams_player_enemies_visible);
    chamsSettings("Enemies Occluded", "##enemiesoccluded", &chams_player_enemies_occluded);

    ImGui::Spacing();

    chamsSettings("Backtrack", "##backtrackc", &chams_player_backtrack, { {&g_Misc->options.chams_backtrack_all_ticks, "All ticks"} });
    chamsSettings("Local Player", "##localplayer", &chams_player_local_player);
    chamsSettings("Weapons", "##weapons", &chams_weapons);
    chamsSettings("Hands", "##hands", &chams_hands);
    chamsSettings("Sleeves", "##sleeves", &chams_sleeves);
}



void Chams::MenuColors() {
    int matWorking = menuMaterial - 1;

    ImGui::ColorEdit("Chams Allies All", chams_player_allies_all.materials[matWorking].color);
    ImGui::ColorEdit("Chams Allies Visible", chams_player_allies_visible.materials[matWorking].color);
    ImGui::ColorEdit("Chams Allies Occluded", chams_player_allies_occluded.materials[matWorking].color);
    ImGui::ColorEdit("Chams Enemies All", chams_player_enemies_all.materials[matWorking].color);
    ImGui::ColorEdit("Chams Enemies Visible", chams_player_enemies_visible.materials[matWorking].color);
    ImGui::ColorEdit("Chams Enemies Occluded", chams_player_enemies_occluded.materials[matWorking].color);
    ImGui::ColorEdit("Chams Backtrack", chams_player_backtrack.materials[matWorking].color);
    ImGui::ColorEdit("Chams Local Player", chams_player_local_player.materials[matWorking].color);
    ImGui::ColorEdit("Chams Weapons", chams_weapons.materials[matWorking].color);
    ImGui::ColorEdit("Chams Hands", chams_hands.materials[matWorking].color);
    ImGui::ColorEdit("Chams Sleeves", chams_sleeves.materials[matWorking].color);
}
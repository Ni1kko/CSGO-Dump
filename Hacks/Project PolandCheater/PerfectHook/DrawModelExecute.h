#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

float flColor[3] = { 0.9686274, 0.7254901,  0.0784313 };

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
    static bool DontDraw = false;
    static auto ofunc = hooks::modelrender.get_original<dme_t>(21);
    if (g_Options.Visuals.Enabled && g_Options.Visuals.Chams)
    {
        static IMaterial* ignorez = CreateMaterial("VertexLitGeneric", "vgui/white_additive", true, true, true, true, true);
        static IMaterial* notignorez = CreateMaterial("VertexLitGeneric", "vgui/white_additive", false, true, true, true, true);
        DontDraw = false;

        const char * ModelName = g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
        C_BaseEntity* pModelEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(pInfo.entity_index);
        C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());




        
        if (ModelName && g_Options.Visuals.Chams && g_Options.Visuals.Filter.Players && strstr(ModelName, "models/player"))
        {
            if (pModelEntity && pLocal)
            {
                if (!g_Options.Visuals.Filter.EnemyOnly ||
                    pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
                {
                    pModelEntity = g_EntityList->GetClientEntity(pInfo.entity_index);
                    if (pModelEntity)
                    {


                        if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
                        {
                            float alpha = 1.f;
                            if (pModelEntity->HasGunGameImmunity())
                                alpha = 0.5f;

                            g_RenderView->SetColorModulation(flColor);
                            g_RenderView->SetBlend(alpha);
                            g_ModelRender->ForcedMaterialOverride(ignorez);
                            ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);


                            g_RenderView->SetColorModulation(flColor);
                            g_RenderView->SetBlend(alpha);
                            g_ModelRender->ForcedMaterialOverride(notignorez);

                        }
                        else
                        {
                            ForceMaterial(Color(255, 255, 255), ignorez);
                        }

                    }
                }
            }
        }
        else if (g_Options.Visuals.Hands && strstr(ModelName, "arms"))
        {
            if (g_Options.Visuals.Hands == 1)
            {
                DontDraw = true;
            }
            if (g_Options.Visuals.Hands == 2)
            {
                IMaterial* Hands = g_MaterialSystem->FindMaterial(ModelName, "Model textures");
                if (!g_Input->m_fCameraInThirdPerson)
                {
                    Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
                    g_ModelRender->ForcedMaterialOverride(Hands);
                }
                else
                {
                    Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
                    g_ModelRender->ForcedMaterialOverride(Hands);
                }
            }
        }
        else if (g_Options.Visuals.Chams && g_Options.Visuals.WeaponsWorld && strstr(ModelName, "_dropped.mdl"))
        {
            ForceMaterial(Color(255, 255, 255), ignorez);
        }
    }
    if (!DontDraw)
        ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
    g_ModelRender->ForcedMaterialOverride(NULL);


}
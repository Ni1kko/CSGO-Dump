#pragma once
#include "hooks.hpp"
#include "ValveSDK/csgostructs.hpp"
#include "ValveSDK/interfaces.hpp"
#include "../Listener.hpp"
std::unordered_map<char*, char*> killIcons = {};

int default_t;
int default_ct;
int iBayonet;
int iButterfly;
int iFlip;
int iGut;
int iKarambit;
int iM9Bayonet;
int iHuntsman;
int iFalchion;
int iDagger;
int iBowie;
void knife_changer()
{
    auto local = C_BasePlayer::get_local_player();
    if (mapchanged && g_EngineClient->IsInGame())
    {
        default_t = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
        default_ct = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
        iBayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
        iBowie = g_MdlInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
        iFalchion = g_MdlInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
        iButterfly = g_MdlInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
        iFlip = g_MdlInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
        iGut = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
        iHuntsman = g_MdlInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
        iKarambit = g_MdlInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
        iM9Bayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
        iDagger = g_MdlInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
        mapchanged = false;
    }
    for (int i = 0; i <= g_EntityList->GetHighestEntityIndex(); i++) // CHANGE
    {
        C_BasePlayer *pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
        if (pEntity)
        {

            auto hOwnerEntity = pEntity->m_hOwnerEntity();
            auto pOwner = g_EntityList->GetClientEntityFromHandle(hOwnerEntity);
            if (pOwner)
            {
                if (pOwner == local)
                {

                    C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;
                    C_BaseViewModel* viewmodel = local->m_hViewModel().Get();
                    auto index = viewmodel->m_nModelIndex();
                    if (index == iBayonet
                        || index == iBowie
                        || index == iFalchion
                        || index == iButterfly
                        || index == iFlip
                        || index == iGut
                        || index == iHuntsman
                        || index == iKarambit
                        || index == iM9Bayonet
                        || index == iDagger
                        || index == default_t
                        || index == default_ct
                        )
                    {
                        if (local->is_alive() && pWeapon->is_knife())
                        {
                            if (g_Options.knife_model.get_int() == 1)
                            {

                                pWeapon->m_nModelIndex() = iBayonet;
                                viewmodel->m_nModelIndex() = iBayonet;
                                pWeapon->m_iWorldModelIndex() = iBayonet + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 500;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "bayonet";
                                killIcons["knife_t"] = "bayonet";
                            }
                            else if (g_Options.knife_model.get_int() == 2)
                            {

                                pWeapon->m_nModelIndex() = iBowie;
                                viewmodel->m_nModelIndex() = iBowie;
                                pWeapon->m_iWorldModelIndex() = iBowie + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 514;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_survival_bowie";
                                killIcons["knife_t"] = "knife_survival_bowie";

                            }
                            else if (g_Options.knife_model.get_int() == 3)
                            {
                                pWeapon->m_nModelIndex() = iButterfly;
                                viewmodel->m_nModelIndex() = iButterfly;
                                pWeapon->m_iWorldModelIndex() = iButterfly + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 515;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_butterfly";
                                killIcons["knife_t"] = "knife_butterfly";

                            }
                            else if (g_Options.knife_model.get_int() == 4)
                            {

                                pWeapon->m_nModelIndex() = iFalchion;
                                viewmodel->m_nModelIndex() = iFalchion;
                                pWeapon->m_iWorldModelIndex() = iFalchion + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 512;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_falchion";
                                killIcons["knife_t"] = "knife_falchion";

                            }
                            else if (g_Options.knife_model.get_int() == 5)
                            {
                                pWeapon->m_nModelIndex() = iFlip;
                                viewmodel->m_nModelIndex() = iFlip;
                                pWeapon->m_iWorldModelIndex() = iFlip + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 505;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_flip";
                                killIcons["knife_t"] = "knife_flip";

                            }
                            else if (g_Options.knife_model.get_int() == 6)
                            {
                                pWeapon->m_nModelIndex() = iGut;
                                viewmodel->m_nModelIndex() = iGut;
                                pWeapon->m_iWorldModelIndex() = iGut + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 506;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_gut";
                                killIcons["knife_t"] = "knife_gut";

                            }
                            else if (g_Options.knife_model.get_int() == 7)
                            {
                                pWeapon->m_nModelIndex() = iHuntsman;
                                viewmodel->m_nModelIndex() = iHuntsman;
                                pWeapon->m_iWorldModelIndex() = iHuntsman + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 509;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_tactical";
                                killIcons["knife_t"] = "knife_tactical";

                            }
                            else if (g_Options.knife_model.get_int() == 8)
                            {
                                pWeapon->m_nModelIndex() = iKarambit;
                                viewmodel->m_nModelIndex() = iKarambit;
                                pWeapon->m_iWorldModelIndex() = iKarambit + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 507;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_karambit";
                                killIcons["knife_t"] = "knife_karambit";

                            }
                            else if (g_Options.knife_model.get_int() == 9)
                            {
                                pWeapon->m_nModelIndex() = iM9Bayonet;
                                viewmodel->m_nModelIndex() = iM9Bayonet;
                                pWeapon->m_iWorldModelIndex() = iM9Bayonet + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 508;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_m9_bayonet";
                                killIcons["knife_t"] = "knife_m9_bayonet";

                            }
                            else if (g_Options.knife_model.get_int() == 10)
                            {
                                pWeapon->m_nModelIndex() = iDagger;
                                viewmodel->m_nModelIndex() = iDagger;
                                pWeapon->m_iWorldModelIndex() = iDagger + 1;
                                pWeapon->m_Item().m_iItemDefinitionIndex() = 516;
                                pWeapon->m_Item().m_iEntityQuality() = 3;

                                killIcons.clear();
                                killIcons["knife_default_ct"] = "knife_push";
                                killIcons["knife_t"] = "knife_push";
                            }
                        }
                    }
                }
            }
        }
    }
}


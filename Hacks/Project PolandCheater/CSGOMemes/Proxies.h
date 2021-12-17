#pragma once
#include "hooks.hpp"

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

namespace hooks
{
    RecvVarProxyFn fnNoSmoke;
    RecvVarProxyFn fnSequenceProxyFn;
    void no_smoke_hook(const CRecvProxyData *pData, void *pStruct, void *pOut)
    {
        if (g_Options.visual_no_smoke.get_bool()) *(bool*)((DWORD)pOut + 0x1) = true;

        fnNoSmoke(pData, pStruct, pOut);
    }
    void sequence_hook(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
    {

        // Make the incoming data editable.
        CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

        // Confirm that we are replacing our view model and not someone elses.
        C_BaseViewModel* pViewModel = static_cast<C_BaseViewModel*>(pStruct);

        if (pViewModel)
        {
            IClientEntity* pOwner = static_cast<IClientEntity*>(g_EntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

            // Compare the owner entity of this view model to the local player entity.
            if (pOwner && pOwner->EntIndex() == g_EngineClient->GetLocalPlayer())
            {
                // Get the filename of the current view model.
                auto pModel = g_MdlInfo->GetModel(pViewModel->m_nModelIndex());
                std::string szModel = g_MdlInfo->GetModelName(pModel);

                // Store the current sequence.
                int m_nSequence = pData->m_Value.m_Int;
                if (szModel == "models/weapons/v_knife_butterfly.mdl")
                {
                    // Fix animations for the Butterfly Knife.
                    switch (m_nSequence)
                    {
                    case SEQUENCE_DEFAULT_DRAW:
                        m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                        break;
                    default:
                        m_nSequence++;
                    }
                }
                else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
                {
                    // Fix animations for the Falchion Knife.
                    switch (m_nSequence)
                    {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                        break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence--;
                    }
                }
                else if (szModel == "models/weapons/v_knife_push.mdl")
                {
                    // Fix animations for the Shadow Daggers.
                    switch (m_nSequence)
                    {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
                    case SEQUENCE_DEFAULT_LIGHT_MISS1:
                    case SEQUENCE_DEFAULT_LIGHT_MISS2:
                        m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_HIT1:
                    case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence += 3; break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence += 2;
                    }
                }
                else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
                {
                    // Fix animations for the Bowie Knife.
                    switch (m_nSequence)
                    {
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_BOWIE_IDLE1;
                        break;
                    default:
                        m_nSequence--;
                    }
                }

                // Set the fixed sequence.
                pData->m_Value.m_Int = m_nSequence;
            }
        }

        // Call original function with the modified data.

        fnSequenceProxyFn(pData, pStruct, pOut);
    }
}
#include "EnginePrediction.hpp"
#include "Helpers/utils.hpp"

float m_flOldCurtime;
float m_flOldFrametime;
CMoveData m_MoveData;

int* m_pPredictionRandomSeed;

void StartPrediction(CUserCmd* pCmd) {
    auto pLocal = C_BasePlayer::get_local_player();
    static bool bInit = false;
    if (!bInit) {
        m_pPredictionRandomSeed = *(int**)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
        bInit = true;
    }


    *m_pPredictionRandomSeed = pCmd->random_seed;


    m_flOldCurtime = g_GlobalVars->curtime;
    m_flOldFrametime = g_GlobalVars->frametime;

    g_GlobalVars->curtime = pLocal->m_nTickBase() * g_GlobalVars->interval_per_tick;
    g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

    g_GameMovement->StartTrackPredictionErrors(pLocal);

    memset(&m_MoveData, 0, sizeof(m_MoveData));
    g_MoveHelper->SetHost(pLocal);
    g_Prediction->SetupMove(pLocal, pCmd, g_MoveHelper, &m_MoveData);
    g_GameMovement->ProcessMovement(pLocal, &m_MoveData);
    g_Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

}

void EndPrediction(CUserCmd* pCmd) {
    auto pLocal = C_BasePlayer::get_local_player();
    g_GameMovement->FinishTrackPredictionErrors(pLocal);
    g_MoveHelper->SetHost(0);

    *m_pPredictionRandomSeed = -1;

    g_GlobalVars->curtime = m_flOldCurtime;
    g_GlobalVars->frametime = m_flOldFrametime;
}
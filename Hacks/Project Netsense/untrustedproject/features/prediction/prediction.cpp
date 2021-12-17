#include "Prediction.hpp"
#include "../../helpers/utils.hpp"


void CPredictionSystem::StartPrediction(C_BasePlayer* pLocal, CUserCmd* cmd)
{
	static bool bInit = false;

	if (!pLocal) return;
	if (!cmd) return;
	if (!g_MoveHelper) return;
	if (!g_GameMovement) return;
	if (!g_Prediction) return;


	if (!bInit) {
		//.text:1016F1ED                 mov     ecx, dword_10C18244           m_pPredictionRandomSeed
		//.text:1016F1F3                 mov     edx, offset aSelectweighted; "SelectWeightedSequence"
		m_pPredictionRandomSeed = *(int**)(Utils::PatternScan(GetModuleHandle(L"client.dll"), "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2);
		bInit = true;
	}

	*m_pPredictionRandomSeed = cmd->random_seed;

	m_flOldCurtime = g_GlobalVars->curtime;
	m_flOldFrametime = g_GlobalVars->frametime;
	m_fFlags = pLocal->m_fFlags();

	g_GlobalVars->curtime = pLocal->m_nTickBase() * g_GlobalVars->interval_per_tick;
	g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

	g_GameMovement->StartTrackPredictionErrors(pLocal);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	g_MoveHelper->SetHost(pLocal);
	g_Prediction->SetupMove(pLocal, cmd, g_MoveHelper, &m_MoveData);
	g_GameMovement->ProcessMovement(pLocal, &m_MoveData);
	g_Prediction->FinishMove(pLocal, cmd, &m_MoveData);
}

void CPredictionSystem::EndPrediction(C_BasePlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal) return;
	if (!cmd) return;
	if (!g_MoveHelper) return;
	if (!g_GameMovement) return;
	if (!g_Prediction) return;

	g_GameMovement->FinishTrackPredictionErrors(pLocal);
	g_MoveHelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	g_GlobalVars->curtime = m_flOldCurtime;
	g_GlobalVars->frametime = m_flOldFrametime;
}
#include "EnginePrediction.h"
#include "NetVars.h"
#include "XorStr.hpp"
#include "MathFunctions.h"
#include "Hacks.h"
#include "EnginePrediction.h"
BYTE bMoveData[200];

void CPredictionSystem::StartPrediction(CUserCmd* pCmd) 
{
	static int flTickBase;

	if (pCmd)
	{
		if (pCmd->hasbeenpredicted)
			flTickBase = hackManager.pLocal()->GetTickBase();
		else
			++flTickBase;
	}

	static bool bInit = false;
	if (!bInit) {
		m_pPredictionRandomSeed = *(int**)(Utilities::Memory::FindPatternV2("client_panorama.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
		bInit = true;
	}

//	*m_pPredictionRandomSeed = MD5_PseudoRandom(Hacks.CurrentCmd->command_number) & 0x7FFFFFFF; /* no thanks */

	m_flOldCurtime = interfaces::globals->curtime;
	m_flOldFrametime = interfaces::globals->frametime;

	if (!interfaces::c_gamemove)
		return;

	interfaces::globals->curtime = flTickBase * interfaces::globals->interval_per_tick;
	interfaces::globals->frametime = interfaces::globals->interval_per_tick;

	interfaces::c_gamemove->StartTrackPredictionErrors(hackManager.pLocal());

	memset(&m_MoveData, 0, sizeof(m_MoveData));

	interfaces::movehelper->SetHost(hackManager.pLocal());
	interfaces::c_prediction->SetupMove(hackManager.pLocal(), pCmd, interfaces::movehelper, &m_MoveData);
	interfaces::c_gamemove->ProcessMovement(hackManager.pLocal(), &m_MoveData);
	interfaces::c_prediction->FinishMove(hackManager.pLocal(), pCmd, &m_MoveData);
}
void CPredictionSystem::EndPrediction(CUserCmd* pCmd) 
{
	if (!interfaces::c_gamemove)
		return;

	interfaces::c_gamemove->FinishTrackPredictionErrors(hackManager.pLocal());
	interfaces::movehelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	interfaces::globals->curtime = m_flOldCurtime;
	interfaces::globals->frametime = m_flOldFrametime;
}































































































































































































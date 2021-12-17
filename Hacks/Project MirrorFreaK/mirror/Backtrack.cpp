#include "Hacks.h"
#include "Backtrack.h"

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

CUserCmd*					Globals::UserCmd;


//std::unique_ptr<CBackTrack> pBackTrack(std::make_unique<CBackTrack>());


int timeToTicks(float dt) {
	return (int)(0.5f + (float)(dt) / Interfaces::Globals->interval_per_tick);
}




void CBackTrack::Inititalise() {
	if (m_bInitialized)
		return;

	for (int s = 0; s <= 65; s++) {
		m_RestoreData[s].Init();
	}

	m_flTeleportDistanceSqr = 64 * 64;

	m_bInitialized = true;
}

void CBackTrack::Run(CUserCmd* pCmd, IClientEntity* pEnt) {

	int index = pEnt->GetIndex();

	if (!m_RestoreData[index].m_bValidRecord) return;

	if (m_iTicks < 10) return;

	if (!IsValidTick(pEnt, m_RestoreData[index].m_flSimulationTime)) return;

	pCmd->tick_count = TIME_TO_TICKS(m_RestoreData[index].m_flSimulationTime);
}

void CBackTrack::RestoreEntity(IClientEntity* pEnt) {

	int index = pEnt->GetIndex();
	if (!m_RestoreData[index].m_bValidRecord) return;
	if (m_iTicks < 10) return;

	if (!IsValidTick(pEnt, m_RestoreData[index].m_flSimulationTime)) return;

	/*THIS SHIT IS ONLY GOOD AGAINST LEGITS , IN HVH u will sucks everyones cock*/

	pEnt->InvalidateBoneCache();

	*(int*)((DWORD)pEnt + 0x100) = m_RestoreData[index].m_flags;
	*(float*)((DWORD)pEnt + 0xA14) = m_RestoreData[index].m_flCycle;
	*(int*)((DWORD)pEnt + 0x28AC) = m_RestoreData[index].m_nSequence;
	pEnt->setAbsOriginal(m_RestoreData[index].m_vecOrigin);
	pEnt->setAbsAechse(m_RestoreData[index].m_vecAngles);
	*(Vector*)((DWORD)pEnt + 0x110) = m_RestoreData[pEnt->GetIndex()].m_vecVelocity;
	pEnt->GetEyeAnglesXY()->x = m_RestoreData[index].m_vecEyeAngles.x;
	pEnt->GetEyeAnglesXY()->y = m_RestoreData[index].m_vecEyeAngles.y;
	pEnt->GetEyeAnglesXY()->z = m_RestoreData[index].m_vecEyeAngles.z;
	*(float*)((DWORD)pEnt + 0x39D8) = m_RestoreData[index].m_flLowerBodyYawTarget;
	*(float*)((DWORD)pEnt + 0x264) = m_RestoreData[index].m_flSimulationTime;

	pEnt->updateClientSideAnimation();

	//for (int i = 0; i < 24; i++) {
	//*(float*)((DWORD)pEnt + 0x2764 + sizeof(float) * i) = m_RestoreData[index].m_flPoseParameter[i];
	//}

}

static float OldLowerBodyYaw[65];

void CBackTrack::StoreEntity(IClientEntity* local, IClientEntity* pEntity) {


	int index = pEntity->GetIndex();

	m_PlayerTable.Store(pEntity);

	m_RestoreData[index].m_bValidRecord = false;

	// If the Player has >= 10 (Ticks /Record) 
	if (m_BackTrackData[index].m_PlayerRecords.size() >= static_cast<size_t>(10)) {
		// Please delete the last Record
		m_BackTrackData[index].m_PlayerRecords.pop_back();
	}
	// It pushes the actual Record to the front example: { Record_10, Record_7, Record_8, Record_9 }    actual Record is: Record_10
	m_BackTrackData[index].m_PlayerRecords.push_front(m_PlayerTable);

	// Saves the Record Size for example 7 Records for this Player in m_Ticks
	m_iTicks = m_BackTrackData[index].m_PlayerRecords.size();


	if (!local) return;
	// if 7 < 10 ?
	if (m_iTicks < 10) return;

	int targetTick = 10 - 1;

	Vector delta;
	delta = m_BackTrackData[index].m_PlayerRecords.at(targetTick).m_vecOrigin - pEntity->GetAbsOrigin();

	if (delta.Length2DSqr() > m_flTeleportDistanceSqr) return;
	if (m_BackTrackData[index].m_PlayerRecords.at(targetTick).m_flSimulationTime >= pEntity->GetSimulationTime()) return;

	m_RestoreData[index].Store(m_BackTrackData[index].m_PlayerRecords.at(targetTick));

}




bool CBackTrack::IsValidTick(IClientEntity* pEnt, float simTime) {

	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();

	pInterpVar = Interfaces::CVar->FindVar("cl_interp");
	pRatioVar = Interfaces::CVar->FindVar("cl_interp_ratio");
	pUpdateVar = Interfaces::CVar->FindVar("cl_updaterate");
	pUnlagVar = Interfaces::CVar->FindVar("sv_unlag");
	pInterpolateVar = Interfaces::CVar->FindVar("cl_interpolate");

	int iTickCount = Interfaces::Globals->tickcount;

	float flLerpTime = max(pInterpVar->GetFloat(), pRatioVar->GetFloat() / pUpdateVar->GetFloat());
	int iTargetTick = TIME_TO_TICKS(flLerpTime + simTime);
	int iServerTick = iTickCount + 1 + TIME_TO_TICKS(nci->GetLatency(FLOW_INCOMING) + nci->GetLatency(FLOW_OUTGOING));
	float flDeltaTime = clamp(flLerpTime + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(iServerTick + TIME_TO_TICKS(flLerpTime) - iTargetTick);

	if (fabs(flDeltaTime) > 0.2f) return false;

	return true;
}


/*------------------------BACKTRACK ENW ------------------------------------------------------------*/
/*--------------BACKTRACK ENW ------------------------------------------------------------*/
/*---------------------------BACKTRACK ENW ------------------------------------------------------------*/


template< class T, class Y >
T clamp(T const &val, Y const &minVal, Y const &maxVal) {
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}
std::unique_ptr<CTimeSimulator> newtestBacktrack(std::make_unique<CTimeSimulator>());
float CTimeSimulator::lerpTime()
{
	float ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	return max(lerp, (ratio / ud_rate));
}

bool CTimeSimulator::IsTickValid(int Tick)
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	float latency = nci->GetLatency(FLOW_OUTGOING);
	const float SV_MAXUNLAG = 1.0f;
	float m_flLerpTime = lerpTime();
	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
	int targettick = Tick - lerpTicks;
	int servertick = floorf(TIME_TO_TICKS(latency)) + 1 + Interfaces::Globals->tickcount;
	float deltaTime = correct - TICKS_TO_TIME(servertick - targettick);

	if (fabs(deltaTime) > 0.2f) {
		return false;
	}

	return true;
}


void CTimeSimulator::UpdatePlayerTable(IClientEntity* pEntity)
{
	// This is a vapse shitfest but still.

	int s = pEntity->GetIndex();
	float flSimTime = pEntity->GetSimulationTime();

	// Wenn LBY updaten sollte speichere die Zeit
	if (LBYShouldUpdate(pEntity))
		m_PlayerTable[s].m_iTargetTickcount = flSimTime;

	// Informationen speichern
	m_PlayerTable[s].m_flAPoseParameters = pEntity->GetPoseParameters();
	m_PlayerTable[s].m_vecOrigin = pEntity->GetOrigin();
	m_PlayerTable[s].m_vecAngles = *pEntity->GetEyeAnglesXY();
	m_PlayerTable[s].m_flSimulationTime = pEntity->GetSimulationTime();

	// Enemy Active Weapon und ob geschossen wurde
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();

	if (Bullets < m_PlayerTable[s].m_iPrevBullets)
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_HIGH;
	} // Setze hihe priorität weil enemy geschossen hat .------------
	else if (OldLowerBodyYaw[s] != pEntity->GetLowerBodyYaw()) { // WENN Oldlby ist ungleich neue LBY
																 // It is necessary else, if it has already been removed in another function
		if (TIME_TO_TICKS(flSimTime) - m_PlayerTable[s].m_iTargetTickcount > 5) // Because my prediction is 
		{
			m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		}
		m_PlayerTable[s].m_bHasBetterRecord = true;
		m_PlayerTable[s].m_vecAngles = Vector(pEntity->GetEyeAnglesXY()->x, pEntity->GetLowerBodyYaw(), 0);

		// Would be high if we would predict it p100
		//
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_MEDIUM;
	}/*
	 else if (((*pEntity->Pos()).at(1) > (0.85f) && OldPoses[s].at(1) <= (0.85f)) ||
	 ((*pEntity->GetPosePosition()).at(1) <= (0.85f)) && OldPoses[s].at(1) > (0.85f)) // Crouch parameter, good for those crouch peekers. 1 = fully standing.
	 {
	 m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
	 m_PlayerTable[s].m_bHasBetterRecord = true;

	 m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_MEDIUM;
	 } */
	else if (pEntity->GetFlags() == (int)FL_ONGROUND && m_PlayerTable[s].m_iPrevFlags == (int)FL_ONGROUND) // Landed, unreliable.
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}
	else if (pEntity->GetEyeAnglesXY()->x < -80) // Assuming you cant fake pitch in that server (that is it has untrusted checks/is valve server)
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}

	else
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_NONE;
	}

	m_PlayerTable[s].m_iPrevBullets = Bullets;
	OldLowerBodyYaw[s] = pEntity->GetLowerBodyYaw();
	//OldPoses[s] = pEntity->GetPosePosition();
	m_PlayerTable[s].m_iPrevFlags = pEntity->GetFlags();


	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "backtracked: PriorityFlag %i\n", m_PlayerTable[s].RecordPriorityFlag);
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "TargetTick %i\n", m_PlayerTable[s].m_iTargetTickcount);
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "Index %i\n", pEntity->GetIndex());
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "-----------------------------\n");


}

bool CTimeSimulator::FindOldestRecord(IClientEntity* pEntity)
{
	int s = pEntity->GetIndex();

	// We don't have any records, update the table
	// and just use the latest record.
	//
	if (m_PlayerGoodRecords[s].empty())
	{
		// How the hell did we even end up here?
		//
		return false;
	}

	for (auto Record = m_PlayerGoodRecords[s].begin(); Record != m_PlayerGoodRecords[s].end(); Record++)
	{
		// We've found an old and good record, set it to
		// our current one and return to backtrack.
		//
		if (IsTickValid(TIME_TO_TICKS(Record->m_flSimulationTime)))
		{
			m_PlayerTable[s] = *Record;
			m_PlayerGoodRecords[s].erase(Record);
			return true;
		}
		else
		{
			// Record isn't valid, erase it and start from the beginning,
			//
			m_PlayerGoodRecords[s].erase(Record);
			if (!m_PlayerGoodRecords[s].empty())
			{
				Record = m_PlayerGoodRecords[s].begin();
			}
			else
			{
				// Again, we didn't find a valid record and we've run out
				// of records to check, proceed to update and use latest record. SMH
				//
				return false;
			}
		}
	}

	return false;
}








void CTimeSimulator::Cache()
{
	IClientEntity* pLP = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	float flCurrentTime = Interfaces::Globals->interval_per_tick * (pLP->GetTickBase());

	for (int s = 1; s <= Interfaces::Engine->GetMaxClients(); ++s)
	{
		IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(s);

		if (!pEntity)
			continue;

		if (pEntity->GetTeamNum() == pLP->GetTeamNum() || pLP == pEntity) {
			continue;
		}

		if (pEntity->IsDormant() || !pLP->IsAlive()) {
			continue;
		}


		m_PlayerTable[s].m_bIsValidRecord = false;

		// We're going to recreate the bone cache anyways so it's good to have FRESH pasta
		pEntity->InvalidateBoneCache();

		float flSimTime = pEntity->GetSimulationTime();

		UpdatePlayerTable(pEntity);

		if (IsRecordValid(pEntity, m_PlayerTable[s]))
		{
			m_PlayerTable[s].SaveInfo(pEntity);
			if (!m_PlayerGoodRecords[s].empty() && m_PlayerGoodRecords[s].size() > 10)
				m_PlayerGoodRecords[s].pop_back();
			m_PlayerGoodRecords[s].push_back(m_PlayerTable[s]);
		}

		pEntity->updateClientSideAnimation();

		if (m_BacktrackedPlayerTable[s].m_bIsValidRecord)
		{
			if (IsRecordValid(pEntity, m_BacktrackedPlayerTable[s]))
			{
				// Record is good, don't invalidate
				m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
				m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
				m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
				m_PrevPlayerTable[s] = m_PlayerTable[s];
				continue;
			}
			else
			{
				m_BacktrackedPlayerTable[s].InvalidateRecord();
			}
		}

		//
		// If we got here that means our previous record has outdated
		//

		// Finding the best record, setting it to m_PlayerTable and going to backtracking
		//
		if (!FindOldestRecord(pEntity)) {
			m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
			m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
			m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
			m_PrevPlayerTable[s] = m_PlayerTable[s];
			continue;
		}

		// Save current record for backtracking as it is the best we have.
		//
		m_PlayerTable[s].m_bIsValidRecord = true;
		m_BacktrackedPlayerTable[s] = m_PlayerTable[s];

		m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
		m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
		m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
		m_PrevPlayerTable[s] = m_PlayerTable[s];
	}
}


bool CTimeSimulator::IsRecordValid(IClientEntity* pEntity, LagRecord_tT& src)
{
	IClientEntity* pLP = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	int s = pEntity->GetIndex();
	float flCurrentTime = Interfaces::Globals->interval_per_tick * pLP->GetTickBase();
	float flSimTime = pEntity->GetSimulationTime();

	if (src.m_vecPreviousOrigin.Length() > 0)
	{
		//SSDK::Vector delta = *pEntity->GetOrigin() - src.m_vecOrigin;
		Vector delta = src.m_vecPreviousOrigin - src.m_vecOrigin;
		if (delta.Length2DSqr() > (64 * 64))
		{
			return false;
		}
	}

	// That's because if something goes wrong, then it will be necessary to correct it if
	// seriously we can return to mexico
	if (fabs(flSimTime - flCurrentTime) > 0.3f) {
		return false;
	}

	if (!IsTickValid(TIME_TO_TICKS(src.m_flSimulationTime))) {
		return false;
	}

	return true;
}

bool CTimeSimulator::LBYShouldUpdate(IClientEntity* pEntity)
{

	float  flCurTime = Interfaces::Globals->curtime;

	bool IsMoving = ((pEntity->GetVelocity()).Length2D() > 0.1f && pEntity->GetFlags() & (int)FL_ONGROUND);
	bool bResult = (IsMoving ||
		((flCurTime - m_PlayerTable[pEntity->GetIndex()].m_flBaseTime) >= 1.1 && !IsMoving)); //Not moving, on ground timer.

	if (bResult)
		m_PlayerTable[pEntity->GetIndex()].m_flBaseTime = flCurTime;

	return bResult;
}


bool CTimeSimulator::ProcessCmd(int iTargetIndex, CUserCmd* pCmd)
{
	if (iTargetIndex == -1 || iTargetIndex > Interfaces::Engine->GetMaxClients())
		return false;

	if (!m_BacktrackedPlayerTable[iTargetIndex].m_bIsValidRecord)
		return false;

	pCmd->tick_count = TIME_TO_TICKS(m_BacktrackedPlayerTable[iTargetIndex].m_flSimulationTime);
	return true;
}
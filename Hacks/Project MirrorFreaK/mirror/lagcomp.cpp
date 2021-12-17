#include "lagcomp.h"
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

LagCompensation::LagCompensation()
{

}

void LagCompensation::logEntity(IClientEntity *player)
{
	int idx = player->GetIndex();
	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

	if (!player || !player->GetHealth() > 0)
	{
		for (int i = 0; i < 11; i++)
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}

	float simTime = player->GetSimulationTime();

	if (!isValidTick(simTime))
		return;

	int highestRecordIdx = -1;
	float highestSimTime = 0.0f;
	static float oldLBY[65];

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(player->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();


	for (int i = 0; i < 11; i++)
	{
		if (m_LagRecords[i].m_fSimulationTime > simTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;

		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;

		if (m_LagRecords[i].m_fSimulationTime == simTime)
			return;

		if (m_LagRecords[i].m_fSimulationTime > highestSimTime)
		{
			highestRecordIdx = i;
			highestSimTime = m_LagRecords[i].m_fSimulationTime;
		}

	}

	highestRecordIdx++;
	highestRecordIdx = highestRecordIdx % 11;

	m_LagRecords[highestRecordIdx].m_bIsFixed = false;
	m_LagRecords[highestRecordIdx].m_iTargetID = idx;

	m_LagRecords[highestRecordIdx].headSpot = player->GetBonePos(8);
	m_LagRecords[highestRecordIdx].m_iPrevBullets = Bullets;
	m_LagRecords[highestRecordIdx].m_vAbsOrigin = player->getAbsOriginal();
	m_LagRecords[highestRecordIdx].m_vecVelocity = player->GetVelocity();
	m_LagRecords[highestRecordIdx].m_fSimulationTime = player->GetSimulationTime();
	m_LagRecords[highestRecordIdx].m_vecAngles = player->getAbsAechse();
	m_LagRecords[highestRecordIdx].m_flCycle = player->getCycle();
	m_LagRecords[highestRecordIdx].m_nSequence = player->getSequence();
	m_LagRecords[highestRecordIdx].flags = player->GetFlags();
	m_LagRecords[highestRecordIdx].m_flLowerBodyYawTarget = player->GetLowerBodyYaw();

	for (int i = 0; i < 24; i++)
		m_LagRecords[highestRecordIdx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::logCurrentEnt(IClientEntity *player)
{
	int idx = player->GetIndex();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(player->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();

	this->m_PrevRecords[idx].m_iPrevBullets = Bullets;
	this->m_PrevRecords[idx].m_vAbsOrigin = player->getAbsOriginal();
	this->m_PrevRecords[idx].m_vecVelocity = player->GetVelocity();
	this->m_PrevRecords[idx].m_fSimulationTime = player->GetSimulationTime();
	this->m_PrevRecords[idx].m_vecAngles = player->getAbsAechse();
	this->m_PrevRecords[idx].m_eyeAngles = *player->GetEyeAnglesXY();
	this->m_PrevRecords[idx].m_flCycle = player->getCycle();
	this->m_PrevRecords[idx].m_nSequence = player->getSequence();
	this->m_PrevRecords[idx].flags = player->GetFlags();
	this->m_PrevRecords[idx].m_flLowerBodyYawTarget = player->GetLowerBodyYaw();

	for (int i = 0; i < 24; i++)
		this->m_PrevRecords[idx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::setEntity(IClientEntity *player, LagRecord_A record)
{
	if (!player || record.m_fSimulationTime == 0.0f)
	{
		record.m_bIsFixed = false;
		return;
	}

	this->logCurrentEnt(player);



	player->setAbsOriginal(record.m_vAbsOrigin);
	*(float*)((DWORD)player + 0x264) = record.m_fSimulationTime;
	*(Vector*)((DWORD)player + 0x110) = record.m_vecVelocity;
	player->setAbsAechse(record.m_vecAngles);

	player->GetEyeAnglesXY()->x = record.m_eyeAngles.x;
	player->GetEyeAnglesXY()->y = record.m_eyeAngles.y;
	player->GetEyeAnglesXY()->z = record.m_eyeAngles.z;

	*(float*)((DWORD)player + 0xA14) = record.m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = record.m_nSequence;
	*(int*)((DWORD)player + 0x100) = record.flags;
	*(float*)((DWORD)player + 0x39D8) = record.m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = record.m_flPoseParameter[i];


	record.m_iWantedTick = record.m_fSimulationTime;
	record.m_bIsFixed = true;

	player->updateClientSideAnimation();
	static DWORD invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1);

}

void LagCompensation::setCurrentEnt(IClientEntity *player)
{
	int idx = player->GetIndex();

	m_PrevRecords[idx].m_iWantedTick = m_PrevRecords[idx].m_fSimulationTime;


	player->setAbsOriginal(m_PrevRecords[idx].m_vAbsOrigin);
	*(Vector*)((DWORD)player + 0x110) = m_PrevRecords[idx].m_vecVelocity;
	*(float*)((DWORD)player + 0x264) = m_PrevRecords[idx].m_fSimulationTime;
	player->setAbsAechse(m_PrevRecords[idx].m_vecAngles);

	player->GetEyeAnglesXY()->x = m_PrevRecords[idx].m_eyeAngles.x;
	player->GetEyeAnglesXY()->y = m_PrevRecords[idx].m_eyeAngles.y;
	player->GetEyeAnglesXY()->z = m_PrevRecords[idx].m_eyeAngles.z;


	*(float*)((DWORD)player + 0xA14) = m_PrevRecords[idx].m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = m_PrevRecords[idx].m_nSequence;
	*(int*)((DWORD)player + 0x100) = m_PrevRecords[idx].flags;
	*(float*)((DWORD)player + 0x39D8) = m_PrevRecords[idx].m_flLowerBodyYawTarget;


	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = m_PrevRecords[idx].m_flPoseParameter[i];

	player->updateClientSideAnimation();

	static DWORD invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1);

}

template<class T, class U>
T LagCompensation::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float LagCompensation::lerpTime()
{

	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

void LagCompensation::initLagRecord()
{
	for (int i = 0; i <= 32; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			m_LagRecord[i][j].m_fSimulationTime = 0.0f;
		}
	}
}

int LagCompensation::fixTickcount(IClientEntity *player)
{
	static int BacktrackTicks = 3;
	int idx = player->GetIndex();

	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

	LagRecord_A recentLR = m_LagRecords[BacktrackTicks];

	if (recentLR.m_fSimulationTime == 0.0f)
		return TIME_TO_TICKS(player->GetSimulationTime()) + 1;

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime);


	return iTargetTickCount + 1;
}

bool LagCompensation::isValidTick(float simTime)
{

	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(lerpTime());

	int predCmdArrivTick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(lerpTime())));

	return abs(flCorrect) <= 0.2f;

}


int LagCompensation::fakeLagFix(IClientEntity *player, int historyIdx)
{

	int idx = player->GetIndex();

	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

	LagRecord_A recentLR = m_LagRecords[historyIdx];
	LagRecord_A prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];

	if (recentLR.m_fSimulationTime == 0.0f)
		return -1;


	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	int predCmdArrivTick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)); // (c) n0xius @ uc

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;



	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + TIME_TO_TICKS(lerpTime()) - iTargetTickCount); // (c) n0xius @ uc

	if (fabs(flCorrect) > 0.2f) // Too big deltatime, can't lagfix here
	{

		//BacktrackCanShotEnemy = false;
		Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), "StartLagCompensation: delta too big (%.3f)\n", flCorrect);
		return -1;

	}

	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).LengthSqr() > 4096.f)
	{
		//recentLR.m_bIsBreakingLagComp = true;
		return -1;
	}
	else
	{
		setEntity(player, recentLR);
	}
	ResolverStage[idx] = 20;
	//BacktrackCanShotEnemy = true;
	return TIME_TO_TICKS(recentLR.m_fSimulationTime);
}
#include "position_adjust.h"
#include "RageBot.h"
#include "laggycompensation.h"
#define TICK_INTERVAL			(Interfaces::Globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

void BackTrack::Update(int tick_count)
{
	if (Options::Menu.RageBotTab.lag_pred.GetState() )
	{
		if (Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		{
			latest_tick = tick_count;
			for (int i = 0; i < 64; i++)
			{
				UpdateRecord(i);
			}
		}
	}
}
float GetLerpTime() {
	static ConVar* cl_interp = Interfaces::CVar->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces::CVar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");
	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;
	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;
	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;
	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;
	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;
	else
		return v20;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	if (in >= high)
		return high;
	return in;
}
bool BackTrack::IsTickValid(int tick)
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (!nci)
		return false;
	float correct = 0;
	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += GetLerpTime();
	ConVar *sv_maxunlag = Interfaces::CVar->FindVar("sv_maxunlag");
	correct = clamp(correct, 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (Interfaces::Globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}
void BackTrack::UpdateRecord(int i)
{
	IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
	if (Options::Menu.RageBotTab.lag_pred.GetState() )
	{
		if (pEntity->isValidPlayer())
		{
			float lby = pEntity->GetLowerBodyYaw();
			records[i].m_nFlags = pEntity->GetFlags();
			if (lby != records[i].lby)
			{
				records[i].tick_count = latest_tick;
				records[i].lby = lby;
				records[i].headPosition = pEntity->GetBonePos(0);
			}
		}
		else
		{
			records[i].tick_count = 0;
		}
	}
}
bool BackTrack::RunLBYBackTrack(CUserCmd* cmd, Vector& aimPoint, IClientEntity * ent)
{
	if (Options::Menu.RageBotTab.lag_pred.GetState())
	{
		for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
		{
			if (ent->GetVelocity().Length2D() > 1.f)
				continue;
		
			if (IsTickValid(records[i].tick_count))
			{
				aimPoint = records[i].headPosition;
				cmd->tick_count = records[i].tick_count;
				return true;
			}
		}
		return false;
	}
	else
		return false;
}
void BackTrack::legitBackTrack(CUserCmd* cmd, IClientEntity* pLocal)
{
	if (Options::Menu.LegitBotTab.BackTrack.GetState())
	{
		int bestTargetIndex = -1;
		int tickxd = 12;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!pLocal->IsAlive())
			return;
		for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
		{
			auto entity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
			if (!entity || !pLocal)
				continue;
			if (entity == pLocal)
				continue;
			if (!Interfaces::Engine->GetPlayerInfo(i, &info))
				continue;
			if (entity->is_dormant())
				continue;
			if (entity->team() == pLocal->team())
				continue;
			if (entity->IsAlive())
			{
				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = entity->GetBonePos(ragebot->HitScan(entity));
				Vector oriboy = entity->m_VecORIGIN();
				Vector angs = entity->GetAngles();
				headPositions[i][cmd->command_number % tickxd] = backtrackData{ simtime, hitboxPos ,oriboy ,angs ,		entity->GetFlags() };
				hdfdunn[i][cmd->command_number % tickxd] = thiccboi{ entity->dw_boneMatrix(),entity->GetAbsOriginlol() };
				Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
				float FOVDistance = DistPointToLine(hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}
		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = DistPointToLine(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}
void BackTrack::rageBackTrack(CUserCmd* cmd, IClientEntity* pLocal)
{
	if (!Options::Menu.RageBotTab.lag_pred.GetState())
		return;
	int bestTargetIndex = -1;
	int tickxd = 12;
	float bestFov = FLT_MAX;
	player_info_t info;

	CTickRecord * rec;
	
	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		auto entity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		if (!entity || !pLocal->isValidPlayer())
			continue;

		if (!Interfaces::Engine->GetPlayerInfo(i, &info))
			continue;
		if (!entity->isValidPlayer())
			continue;
		if (entity->team() == hackManager.pLocal()->team())
			continue;

		float simtime = entity->GetSimulationTime();
		Vector hitboxPos = entity->GetBonePos(ragebot->HitScan(entity));
		Vector oriboy = entity->m_VecORIGIN();
		Vector angs = entity->GetAngles();
//		cm_backtrack->AnimationFix(entity);
		headPositions[i][cmd->command_number % tickxd] = backtrackData{ simtime, hitboxPos ,oriboy ,angs ,		entity->GetFlags() };
		hdfdunn[i][cmd->command_number % tickxd] = thiccboi{ entity->dw_boneMatrix(),entity->GetAbsOriginlol() };
		Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
		float FOVDistance = DistPointToLine(hitboxPos, pLocal->GetEyePosition(), ViewDir);
		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}
	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
		for (int t = 0; t < 12; ++t)
		{
			float tempFOVDistance = DistPointToLine(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
			}
		}
		if (cmd->buttons & IN_ATTACK)
		{
			//no
			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
		}
	}
}
bool CBacktracking::bTickIsValid(int tick)
{
	static auto cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (Options::Menu.RageBotTab.lag_pred.GetState())
	{
		tick += TIME_TO_TICKS(m_flLerpTime);
		return (fabsf(TICKS_TO_TIME(Interfaces::Globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
	}
}
void CBacktracking::SaveTemporaryRecord(IClientEntity* ent, CTickRecord record)
{
	if (!record.m_flSimulationTime)
		record = CTickRecord(ent);
	arr_infos.at(ent->GetIndex()).temporaryRecord = record;
}
void CBacktracking::RestoreTemporaryRecord(IClientEntity* ent)
{
	ApplyTickRecordCM(ent, arr_infos.at(ent->GetIndex()).temporaryRecord);
}
void CBacktracking::ApplyTickrecord(IClientEntity* ent, CTickRecord record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}
void CBacktracking::ApplyTickRecordFSN(IClientEntity* pEntity, CTickRecord record)
{
	*(QAngle*)((uintptr_t)pEntity + 0x0000B23C) = record.m_angEyeAngles;
	*(float*)((uintptr_t)pEntity + 0x00000264) = record.m_flSimulationTime;
	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;
	*(Vector*)((uintptr_t)pEntity + 0x00000110) = record.m_vecVelocity;
	*(float*)((uintptr_t)pEntity + 0x00002F9C) = record.m_flDuckAmount;
	*(float*)((uintptr_t)pEntity + 0x00002FA0) = record.m_flDuckSpeed;
	//pEntity->SetPoseParameters(record.m_flPoseParameter);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsAngles_2() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);  
}
void CBacktracking::ApplyTickRecordCM(IClientEntity* pEntity, CTickRecord record) {
	pEntity->setAbsAechse(record.m_angEyeAngles);
	pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsAngles_2() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);
//	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;  
	//pEntity->SetPoseParameters(record.m_flPoseParameter);
}
int CBacktracking::ShotBackTrackedTick(IClientEntity* ent) {
	if (!ent)
		return 0;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;
	return 0;
}
void CBacktracking::ShotBackTrackStoreFSN(IClientEntity* ent) {
	if (!ent)
		return;
	static std::array<float, 33> lastsimulationtimes;
	if (lastsimulationtimes[ent->GetIndex()] == ent->GetSimulationTime())
		return;
	lastsimulationtimes[ent->GetIndex()] = ent->GetSimulationTime();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	bool PlayerShot = false;
	C_PlayerStored* pCPlayer = &arr_infos[ent->GetIndex()].player;
	if (ent->GetEyeAngles() != pCPlayer->EyeAngles)
		pCPlayer->EyeAnglesUpdated = true;
	if (!pWeapon)
		return;
	if (pCPlayer->CurrentWeapon == pWeapon && pWeapon->IsGun()) {
		//don't count weapon changes
		if (pWeapon->GetAmmoInClip() == pCPlayer->BulletsLeft - 1) {
			if (pCPlayer->EyeAnglesUpdated) {
				//Don't count reloading
				pCPlayer->IsFiring = true;
				PlayerShot = true;
			}
		}
		else
			pCPlayer->IsFiring = false;
	}
	else {
		pCPlayer->IsFiring = false;
	}
	if (PlayerShot) {
		if (Options::Menu.RageBotTab.lag_pred.GetState())
			pCPlayer->TickCount = TIME_TO_TICKS(ent->GetSimulationTime() - Interfaces::Globals->interval_per_tick) + TIME_TO_TICKS(GetLerpTime()) + 1;
		else
			pCPlayer->TickCount = Interfaces::Globals->tickcount;
		pCPlayer->ValidTick = CTickRecord(ent);
		pCPlayer->ValidTick.tickcount = pCPlayer->TickCount;
		pCPlayer->ValidTick.m_angEyeAngles = ent->GetEyeAngles();			//CURRENT angles yaw and pitch
																			//pCPlayer->ValidTick.m_flPoseParameter = std::array<float, 24>();
																			//if (ent->GetSimulationTime() - pCPlayer->SimulationTime > Interfaces::Globals->interval_per_tick) //if psilent or fake lag																//pCPlayer->ValidTick.m_angEyeAngles.x = pCPlayer->EyeAngles.x; //LAST angles pitch!
		arr_infos[ent->GetIndex()].shot = true;
	}
	pCPlayer->BulletsLeft = pWeapon->GetAmmoInClip();
	pCPlayer->EyeAngles = ent->GetEyeAngles();
	pCPlayer->CurrentWeapon = pWeapon;
	if (!bTickIsValid(pCPlayer->TickCount)) {
		pCPlayer->TickCount = 0;
		arr_infos[ent->GetIndex()].shot = false;
	}
}

void CBacktracking::ShotBackTrackBeforeAimbot(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !Interfaces::Globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * Interfaces::Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (Interfaces::Globals)
		Interfaces::Globals->tickcount = Interfaces::Globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount))
	{
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}
void CBacktracking::ShotBackTrackAimbotStart(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !Interfaces::Globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * Interfaces::Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (Interfaces::Globals)
		Interfaces::Globals->tickcount = Interfaces::Globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}
void CBacktracking::ShotBackTrackAimbotEnd(IClientEntity* ent) {
	return;
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;
	RestoreTemporaryRecord(ent);
}
void CBacktracking::ClearRecord(IClientEntity* Entity) {
	arr_infos[Entity->GetIndex()] = CBacktrack_Info();
}
BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];
thiccboi hdfdunn[64][12];
CBacktracking* bt_2 = new CBacktracking();
CTickRecord * tickrec = new CTickRecord();
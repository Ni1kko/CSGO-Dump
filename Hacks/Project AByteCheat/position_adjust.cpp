#include "position_adjust.h"
#include "RageBot.h"
#include "laggycompensation.h"
#define TICK_INTERVAL			(interfaces::globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

void BackTrack::Update(int tick_count)
{
	if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
	{
		if (interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		{
			latest_tick = tick_count;
			for (int i = 0; i < 64; i++)
			{
				UpdateRecord(i);
			}
		}
	}
}
float BackTrack::GetLerpTime() {
	static ConVar* cl_interp = interfaces::cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = interfaces::cvar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = interfaces::cvar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
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
	INetChannelInfo *nci = interfaces::engine->GetNetChannelInfo();
	if (!nci)
		return false;
	float correct = 0;
	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += GetLerpTime();
	ConVar *sv_maxunlag = interfaces::cvar->FindVar("sv_maxunlag");
	correct = clamp(correct, 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (interfaces::globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}

bool BackTrack::good_tick(int tick)
{
	auto nci = interfaces::engine->GetNetChannelInfo();
	if (!nci) return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, 1.f);
	float delta_time = correct - (interfaces::globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(delta_time) < 0.2f;
}

void BackTrack::UpdateRecord(int i)
{
	IClientEntity* pEntity = interfaces::ent_list->get_client_entity(i);
	if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
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
	if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
	{
		for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
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
void BackTrack::rageBackTrack(CUserCmd* cmd, IClientEntity* pLocal)
{
	/*
			if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
		return;
	
	int tickxd = 12;
	float bestFov = FLT_MAX;
	player_info_t info;

	CTickRecord * rec;
	
	for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
	{
		auto entity = (IClientEntity*)interfaces::ent_list->get_client_entity(i);
		if (!entity || !pLocal->isValidPlayer())
			continue;

		if (!interfaces::engine->GetPlayerInfo(i, &info))
			continue;
		if (!entity->isValidPlayer())
			continue;
		if (entity->team() == hackManager.pLocal()->team())
			continue;

		float simtime = entity->GetSimulationTime();
		Vector hitboxPos = entity->GetBonePos(ragebot->HitScan(entity, pLocal));
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
		if (cmd->buttons & IN_ATTACK && !interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		{
			//no
			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
		}
	}
	*/
}
bool cbacktracking::bTickIsValid(int tick)
{
	static auto cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
	{
		tick += TIME_TO_TICKS(m_flLerpTime);
		return (fabsf(TICKS_TO_TIME(interfaces::globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
	}
}
void cbacktracking::SaveTemporaryRecord(IClientEntity* ent, CTickRecord record)
{
	if (!record.m_flSimulationTime)
		record = CTickRecord(ent);
	arr_infos.at(ent->GetIndex()).temporaryRecord = record;
}
void cbacktracking::RestoreTemporaryRecord(IClientEntity* ent)
{
	ApplyTickRecordCM(ent, arr_infos.at(ent->GetIndex()).temporaryRecord);
}
void cbacktracking::ApplyTickrecord(IClientEntity* ent, CTickRecord record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}
void cbacktracking::ApplyTickRecordFSN(IClientEntity* pEntity, CTickRecord record)
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
void cbacktracking::ApplyTickRecordCM(IClientEntity* pEntity, CTickRecord record) {
	pEntity->setAbsAechse(record.m_angEyeAngles);
	pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsAngles_2() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);
//	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;  
	//pEntity->SetPoseParameters(record.m_flPoseParameter);
}
int cbacktracking::ShotBackTrackedTick(IClientEntity* ent) {
	if (!ent)
		return 0;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;
	return 0;
}
void cbacktracking::ShotBackTrackStoreFSN(IClientEntity* ent) {
	if (!ent)
		return;
	static std::array<float, 33> lastsimulationtimes;
	if (lastsimulationtimes[ent->GetIndex()] == ent->GetSimulationTime())
		return;
	lastsimulationtimes[ent->GetIndex()] = ent->GetSimulationTime();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
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
		if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0)
			pCPlayer->TickCount = TIME_TO_TICKS(ent->GetSimulationTime() - interfaces::globals->interval_per_tick) + TIME_TO_TICKS(backtracking->GetLerpTime()) + 1;
		else
			pCPlayer->TickCount = interfaces::globals->tickcount;
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

void cbacktracking::ShotBackTrackBeforeAimbot(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !interfaces::globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * interfaces::globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (interfaces::globals)
		interfaces::globals->tickcount = interfaces::globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount))
	{
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}
void cbacktracking::ShotBackTrackAimbotStart(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !interfaces::globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * interfaces::globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (interfaces::globals)
		interfaces::globals->tickcount = interfaces::globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}
void cbacktracking::ShotBackTrackAimbotEnd(IClientEntity* ent) {
	return;
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;
	RestoreTemporaryRecord(ent);
}
void cbacktracking::ClearRecord(IClientEntity* Entity) {
	arr_infos[Entity->GetIndex()] = CBacktrack_Info();
}
BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];
thiccboi hdfdunn[64][12];
cbacktracking* bt_2 = new cbacktracking();
CTickRecord * tickrec = new CTickRecord();
























































































































































































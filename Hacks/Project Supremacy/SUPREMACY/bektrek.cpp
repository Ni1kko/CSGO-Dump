#include "Backtracking.h"
#define TICK_INTERVAL			( g_csgo.m_globals( )->m_interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
void BackTrack::Update(int tick_count)
{
	if (g_cfg.ragebot.lagcomp)
	{
		if (g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		{
			latest_tick = tick_count;
			for (int i = 0; i < 64; i++)
			{
				UpdateRecord(i);
			}
		}
	}
}
float GetLerpTime666() {
	static ConVar* cl_interp = g_csgo.m_cvar()->FindVar("cl_interp");
	static ConVar* cl_updaterate = g_csgo.m_cvar()->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");
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
	INetChannelInfo *nci = g_csgo.m_engine()->GetNetChannelInfo();
	if (!nci)
		return false;
	float correct = 0;
	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += GetLerpTime666();
	ConVar *sv_maxunlag = g_csgo.m_cvar()->FindVar("sv_maxunlag");
	correct = clamp(correct, 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (g_csgo.m_globals()->m_curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}
void BackTrack::UpdateRecord(int i)
{
	player_t* pEntity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
	if (g_cfg.ragebot.lagcomp)
	{
		if (pEntity && pEntity->is_alive11() && !pEntity->IsDormant())
		{
			float lby = pEntity->m_flLowerBodyYawTarget();
			records[i].m_nFlags = pEntity->m_fFlags();
			if (lby != records[i].lby)
			{
				records[i].tick_count = latest_tick;
				records[i].lby = lby;
				records[i].headPosition = pEntity->bone_pos(0);
			}
		}
		else
		{
			records[i].tick_count = 0;
		}
	}
}
bool BackTrack::RunLBYBackTrack(CUserCmd* cmd, Vector& aimPoint)
{
	if (g_cfg.ragebot.lagcomp)
	{
		for (int i = 0; i < g_csgo.m_engine()->GetMaxClients(); i++)
		{
			if (IsTickValid(records[i].tick_count))
			{
				aimPoint = records[i].headPosition;
				cmd->m_tickcount = records[i].tick_count;
				return true;
			}
		}
		return false;
	}
	else
		return false;
}

void BackTrack::rageBackTrack(CUserCmd* cmd, player_t* pLocal)
{
	if (!g_cfg.ragebot.lagcomp)
		return;
	int bestTarEntIndex = -1;
	int tickxd = 12;
	float bestFov = FLT_MAX;
	player_info_t info;

	CTickRecord * rec;
	if (!pLocal->is_alive9())
		return;
	for (int i = 0; i < g_csgo.m_engine()->GetMaxClients(); i++)
	{
		auto entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
		if (!entity || !pLocal)
			continue;
		if (entity == pLocal)
			continue;
		if (!g_csgo.m_engine()->GetPlayerInfo(i, &info))
			continue;
		if (entity->IsDormant())
			continue;
		if (entity->m_iTeamNum() == pLocal->m_iTeamNum())
			continue;
		if (entity->is_alive10())
		{
			float simtime = entity->m_flSimulationTime();
			Vector hitboxPos = entity->bone_pos(0);//change me pls
			Vector oriboy = entity->m_vecOrigin();
			Vector angs = entity->GetRenderAngles();
			headPositions[i][cmd->m_command_number % tickxd] = backtrackData{ simtime, hitboxPos ,oriboy ,angs ,		entity->m_fFlags() };
			hdfdunn[i][cmd->m_command_number % tickxd] = who_reads_that_isGay{ entity->dw_boneMatrix(),entity->abs_origin() };
			Vector ViewDir = AngleVector(cmd->m_viewangles + (pLocal->m_aimPunchAngle() * 2.f));
			float FOVDistance = DistPointToLine(hitboxPos, pLocal->get_eye_pos(), ViewDir);
			if (bestFov > FOVDistance)
			{
				bestFov = FOVDistance;
				bestTarEntIndex = i;
			}
		}
	}
	float bestTargetSimTime;
	if (bestTarEntIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = AngleVector(cmd->m_viewangles + (pLocal->m_aimPunchAngle() * 2.f));
		for (int t = 0; t < 12; ++t)
		{
			float tempFOVDistance = DistPointToLine(headPositions[bestTarEntIndex][t].hitboxPos, pLocal->get_eye_pos(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTarEntIndex][t].simtime > pLocal->m_flSimulationTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTarEntIndex][t].simtime;
			}
		}
		if (cmd->m_buttons & IN_ATTACK)
		{
			//no
			cmd->m_tickcount = TIME_TO_TICKS(bestTargetSimTime);
		}
	}
}
bool CBacktracking::bTickIsValid(int tick)
{
	static auto cl_interp_ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio");
	static auto cl_updaterate = g_csgo.m_cvar()->FindVar("cl_updaterate");
	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (g_cfg.ragebot.lagcomp)
	{
		tick += TIME_TO_TICKS(m_flLerpTime);
		return (fabsf(TICKS_TO_TIME(g_csgo.m_globals()->m_tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
	}
}
void CBacktracking::SaveTemporaryRecord(player_t* ent, CTickRecord record)
{
	if (!record.m_flSimulationTime)
		record = CTickRecord(ent);
	arr_infos.at(ent->EntIndex()).temporaryRecord = record;
}
void CBacktracking::RestoreTemporaryRecord(player_t* ent)
{
	ApplyTickRecordCM(ent, arr_infos.at(ent->EntIndex()).temporaryRecord);
}
void CBacktracking::ApplyTickrecord(player_t* ent, CTickRecord record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}
void CBacktracking::ApplyTickRecordFSN(player_t* pEntity, CTickRecord record)
{
	pEntity->m_angEyeAngles() = record.m_angEyeAngles;
	pEntity->m_flSimulationTime() = record.m_flSimulationTime;
	pEntity->m_vecOrigin() = record.m_vecOrigin;
	pEntity->m_vecVelocity() = record.m_vecVelocity;
	pEntity->m_flDuckAmount() = record.m_flDuckAmount;
	pEntity->m_flDuckSpeed() = record.m_flDuckSpeed;
	//pEntity->SetPoseParameters(record.m_flPoseParameter);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->abs_angles() != pEntity->m_angEyeAngles())
		pEntity->setAbsThereIsAFlyInMyRoom(record.m_angEyeAngles);
}
void CBacktracking::ApplyTickRecordCM(player_t* pEntity, CTickRecord record) {
	pEntity->setAbsThereIsAFlyInMyRoom(record.m_angEyeAngles);
	pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->abs_angles() != pEntity->m_angEyeAngles())
		pEntity->setAbsThereIsAFlyInMyRoom(record.m_angEyeAngles);
	//	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;
		//pEntity->SetPoseParameters(record.m_flPoseParameter);
}
int CBacktracking::ShotBackTrackedTick(player_t* ent) {
	if (!ent)
		return 0;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->EntIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;
	return 0;
}
void CBacktracking::ShotBackTrackStoreFSN(player_t* ent) {
	if (!ent)
		return;
	static std::array<float, 33> lastsimulationtimes;
	if (lastsimulationtimes[ent->EntIndex()] == ent->m_flSimulationTime())
		return;
	lastsimulationtimes[ent->EntIndex()] = ent->m_flSimulationTime();
	weapon_t* pWeapon = (weapon_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(g_ctx.m_local->m_hActiveWeapon());
	bool PlayerShot = false;
	C_PlayerStored* pCPlayer = &arr_infos[ent->EntIndex()].player;
	if (ent->m_angEyeAngles() != pCPlayer->EyeAngles)
		pCPlayer->EyeAnglesUpdated = true;
	if (!pWeapon)
		return;
	if (pCPlayer->CurrentWeapon == pWeapon && !pWeapon->is_non_aim()) {
		//don't count weapon changes
		if (pWeapon->m_iClip1() == pCPlayer->BulletsLeft - 1) {
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
		if (g_cfg.ragebot.lagcomp)
			pCPlayer->TickCount = TIME_TO_TICKS(ent->m_flSimulationTime() - g_csgo.m_globals()->m_interval_per_tick) + TIME_TO_TICKS(GetLerpTime666()) + 1;
		else
			pCPlayer->TickCount = g_csgo.m_globals()->m_tickcount;
		pCPlayer->ValidTick = CTickRecord(ent);
		pCPlayer->ValidTick.tickcount = pCPlayer->TickCount;
		pCPlayer->ValidTick.m_angEyeAngles = ent->m_angEyeAngles();			//CURRENT angles yaw and pitch
																			//pCPlayer->ValidTick.m_flPoseParameter = std::array<float, 24>();
																			//if (ent->GetSimulationTime() - pCPlayer->SimulationTime > g_csgo.m_globals()->interval_per_tick) //if psilent or fake lag																//pCPlayer->ValidTick.m_angEyeAngles.x = pCPlayer->EyeAngles.x; //LAST angles pitch!
		arr_infos[ent->EntIndex()].shot = true;
	}
	pCPlayer->BulletsLeft = pWeapon->m_iClip1();
	pCPlayer->EyeAngles = ent->m_angEyeAngles();
	pCPlayer->CurrentWeapon = pWeapon;
	if (!bTickIsValid(pCPlayer->TickCount)) {
		pCPlayer->TickCount = 0;
		arr_infos[ent->EntIndex()].shot = false;
	}
}

void CBacktracking::ShotBackTrackBeforeAimbot(player_t* ent) {
	weapon_t* pWeapon = (weapon_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(g_ctx.m_local->m_hActiveWeapon());
	if (!ent || !g_ctx.m_local || !g_ctx.m_local->m_hActiveWeapon() || !g_csgo.m_globals())
		return;
	bool can_shoot = (pWeapon->m_flNextPrimaryAttack() <= (g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->EntIndex()).player;
	if (g_csgo.m_globals())
		g_csgo.m_globals()->m_tickcount = g_csgo.m_globals()->m_tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount))
	{
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}
void CBacktracking::ShotBackTrackAimbotStart(player_t* ent) {
	weapon_t* pWeapon = (weapon_t*)g_csgo.m_entitylist()->GetClientEntityFromHandle(g_ctx.m_local->m_hActiveWeapon());
	if (!ent || !g_ctx.m_local || !g_ctx.m_local->m_hActiveWeapon() || !g_csgo.m_globals())
		return;
	bool can_shoot = (pWeapon->m_flNextPrimaryAttack() <= (g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->EntIndex()).player;
	if (g_csgo.m_globals())
		g_csgo.m_globals()->m_tickcount = g_csgo.m_globals()->m_tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}
void CBacktracking::ShotBackTrackAimbotEnd(player_t* ent) {
	return;
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->EntIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;
	RestoreTemporaryRecord(ent);
}
void CBacktracking::ClearRecord(player_t* Entity) {
	arr_infos[Entity->EntIndex()] = CBacktrack_Info();
}
BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];
who_reads_that_isGay hdfdunn[64][12];
CBacktracking* bt_2 = new CBacktracking();
CTickRecord * tickrec = new CTickRecord();
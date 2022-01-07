#include "newbacktrack.h"
#include "RageBot.h"

#define MAX_BACKTRACKABLE_TICKS miscconfig.backtrackticks

std::array<float, 24> getPoseParams(IClientEntity* pEnt) {
	return *(std::array<float, 24>*)((uintptr_t)pEnt + 0x2764); // + m_flPoseParameter
}
void SetPoseParameters(std::array<float, 24> arr, IClientEntity* pEnt) {
	*(std::array<float, 24>*)((uintptr_t)pEnt + 0x2764) = arr; // +m_flPoseParameter
}
void c_newbacktrack::update(int tick_count) {
	latest_tick = tick_count;
	for (int i = 0; i < 64; i++) {
		update_record(i);
	}
}
float GetLerpTimeXX() {
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
bool c_newbacktrack::is_valid_tick(int tick) {
	int delta = latest_tick - tick;
	float deltaTime = delta * interfaces::globals->interval_per_tick;
	static auto cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	tick += TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING));
	tick += TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING));
	tick += TIME_TO_TICKS(GetLerpTimeXX());
	INetChannelInfo* gaye = interfaces::engine->GetNetChannelInfo();
	float ping = 200.f;
	return (fabs(deltaTime) <= ping);
}
int c_newbacktrack::getChockedTicks(IClientEntity* ent)
{
	if (ent->GetSimulationTime() > ent->m_flOldSimulationTime())
		return TIME_TO_TICKS(fabsf(ent->GetSimulationTime() - ent->m_flOldSimulationTime()));
	return 0;
}
void setAbsAechse(Vector aechse, IClientEntity* Xd)
{
	using SetAbsAnglesFn = void(__thiscall*)(IClientEntity*, const Vector &angles);
	static SetAbsAnglesFn SetAbsAngles;
	if (!SetAbsAngles)
		SetAbsAngles = (SetAbsAnglesFn)(Utilities::Memory::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));
	SetAbsAngles(Xd, aechse);
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
bool c_newbacktrack::isValidTick(int simTime)
{
	INetChannelInfo *nci = interfaces::engine->GetNetChannelInfo();
	if (!nci)
		return false;
	auto LerpTicks = TIME_TO_TICKS(GetLerpTimeXX());
	int predCmdArrivTick = interfaces::globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
	float flCorrect = clamp(GetLerpTimeXX() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(GetLerpTimeXX())));
	return abs(flCorrect) < 0.2f;
}
bool c_newbacktrack::bTickIsValid(int tick)
{
	static auto cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (options::menu.aimbot_tab.AccuracyBoost.GetIndex() > 0) {
		tick += TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING));
		tick += TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING));
		tick += TIME_TO_TICKS(GetLerpTimeXX());
	}
	return (fabsf(TICKS_TO_TIME(interfaces::globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
}
void c_newbacktrack::SaveTemporaryRecord(IClientEntity* ent, c_tickrec record)
{
	if (!record.m_flSimulationTime)
		record = c_tickrec(ent);
	arr_infos.at(ent->GetIndex()).temporaryRecord = record;
}
void c_newbacktrack::RestoreTemporaryRecord(IClientEntity* ent)
{
	ApplyTickRecordCM(ent, arr_infos.at(ent->GetIndex()).temporaryRecord);
}
void c_newbacktrack::ApplyTickrecord(IClientEntity* ent, c_tickrec record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}
void c_newbacktrack::ApplyTickRecordFSN(IClientEntity* pEntity, c_tickrec record)
{
	*(QAngle*)((uintptr_t)pEntity + 0x0000B23C) = record.m_angEyeAngles;
	*(float*)((uintptr_t)pEntity + 0x00000264) = record.m_flSimulationTime;
	*(Vector*)((uintptr_t)pEntity + 0x00000110) = record.m_vecVelocity;
	*(float*)((uintptr_t)pEntity + 0x00002F9C) = record.m_flDuckAmount;
	*(float*)((uintptr_t)pEntity + 0x00002FA0) = record.m_flDuckSpeed;
	SetPoseParameters(record.m_flPoseParameter, pEntity);
	if (pEntity->GetAbsAngles() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);
}
void c_newbacktrack::ApplyTickRecordCM(IClientEntity* pEntity, c_tickrec record) {
	pEntity->setAbsAechse(record.m_angEyeAngles);
	SetPoseParameters(record.m_flPoseParameter, pEntity);
}
int c_newbacktrack::ShotBackTrackedTick(IClientEntity* ent) {
	if (!ent)
		return 0;
	c_ent_stored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;
	return 0;
}
float lerpusmaximus() {
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
void c_newbacktrack::ShotBackTrackStoreFSN(IClientEntity* ent) {
	if (!ent)
		return;
	static std::array<float, 33> lastsimulationtimes;
	if (lastsimulationtimes[ent->GetIndex()] == ent->GetSimulationTime())
		return;
	lastsimulationtimes[ent->GetIndex()] = ent->GetSimulationTime();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	bool PlayerShot = false;
	c_ent_stored* pCPlayer = &arr_infos[ent->GetIndex()].player;
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
			pCPlayer->TickCount = TIME_TO_TICKS(ent->GetSimulationTime() - interfaces::globals->interval_per_tick) + TIME_TO_TICKS(lerpusmaximus()) + 1;
		else
			pCPlayer->TickCount = interfaces::globals->tickcount;
		pCPlayer->ValidTick = c_tickrec(ent);
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
/*
void c_newbacktrack::OverridePosesFsnRenderStart(IClientEntity* ent) {
if (!ent)
return;
c_ent_stored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
pCPlayer->ValidTick.m_flPoseParameter = ent->GetPoseParameters();
}*/
void c_newbacktrack::ShotBackTrackBeforeAimbot(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !interfaces::globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * interfaces::globals->interval_per_tick));
	c_ent_stored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (interfaces::globals)
		interfaces::globals->tickcount = interfaces::globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount))
	{
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}
void c_newbacktrack::ShotBackTrackAimbotStart(IClientEntity* ent) {
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !interfaces::globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * interfaces::globals->interval_per_tick));
	c_ent_stored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (interfaces::globals)
		interfaces::globals->tickcount = interfaces::globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}
void c_newbacktrack::ShotBackTrackAimbotEnd(IClientEntity* ent) {
	return;
	if (!ent)
		return;
	c_ent_stored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;
	RestoreTemporaryRecord(ent);
}
void c_newbacktrack::ClearRecord(IClientEntity* Entity) {
	arr_infos[Entity->GetIndex()] = c_backtrackinfo();
}
static int TIME_TO_TICKSX(int dt)
{
	return (int)(0.5f + (float)(dt) / interfaces::globals->interval_per_tick);
}
inline Vector angle_vxector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));
	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));
	return Vector(cp*cy, cp*sy, -sp);
}
static Vector hitbox_location(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 bone_matrix[128];

	if (pEntity->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
		if (pEntity->GetModel()) {
			auto studio_model =	interfaces::model_info->GetStudiomodel(pEntity->GetModel());
			if (studio_model) {
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(Hitbox);
				if (hitbox) {
					auto min = Vector{}, max = Vector{};

					VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
					VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
	}
	return Vector{};
}
void c_newbacktrack::update_record(int i) {
	IClientEntity* m_entity = interfaces::ent_list->get_client_entity(i);
	if (m_entity && m_entity->IsAlive() && !m_entity->IsDormant() && m_entity->team() != hackManager.pLocal()->team()) {
		float lby = m_entity->GetLowerBodyYaw();
		if (lby != records[i].lby) {
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].head_position = GetHitboxPosition(m_entity, 0);
			records[i].m_nFlags = m_entity->GetFlags();
		}
	}
	else {
		records[i].tick_count = 0;
	}
}
void ClampAngles(Vector& v)
{
	if (v.x > 89.0f && v.x <= 180.0f)
		v.x = 89.0f;
	if (v.x > 180.0f)
		v.x = v.x - 360.0f;
	if (v.x < -89.0f)
		v.x = -89.0f;
	v.y = fmodf(v.y + 180, 360) - 180;
	v.z = 0;
}
void VectorAnglesX(const Vector& forward, Vector &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / 3.14159265358979323846f;
		angles[1] = atan2(forward[1], forward[0]) * 180 / 3.14159265358979323846f;
		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}
	angles[2] = 0.0f;
}
Vector CalcAngle(Vector src, Vector dst)
{
	Vector angles;
	auto delta = src - dst;
	VectorAnglesX(delta, angles);
	ClampAngles(delta);
	return angles;
}
static float GetFOV(Vector qAngles, Vector vecSource, Vector vecDestination, bool bDistanceBased)
{
	auto MakeVector = [](Vector qAngles)
	{
		auto ret = Vector();
		auto pitch = float(qAngles[0] * 3.14159265358979323846 / 180.f);
		auto yaw = float(qAngles[1] * 3.14159265358979323846 / 180.f);
		auto tmp = float(cos(pitch));
		ret.x = float(-tmp * -cos(yaw));
		ret.y = float(sin(yaw)*tmp);
		ret.z = float(-sin(pitch));
		return ret;
	};
	Vector ang, aim;
	double fov;
	ang = CalcAngle(vecSource, vecDestination);
	aim = MakeVector(qAngles);
	ang = MakeVector(ang);
	auto mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	auto mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	auto u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.f / 3.14159265358979323846);
	if (bDistanceBased) {
		fov *= 1.4;
		float xDist = abs(vecSource[0] - vecDestination[0]);
		float yDist = abs(vecSource[1] - vecDestination[1]);
		float Distance = sqrt((xDist * xDist) + (yDist * yDist));
		Distance /= 650.f;
		if (Distance < 0.7f)
			Distance = 0.7f;
		if (Distance > 6.5)
			Distance = 6.5;
		fov *= Distance;
	}
	return (float)fov;
}
inline Vector AngleVectorXX(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));
	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));
	return Vector(cp*cy, cp*sy, -sp);
}
inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;
	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;
	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);
	return (Point - PerpendicularPoint).Length();
}
void c_newbacktrack::legitBackTrackT(CUserCmd* cmd, IClientEntity* pLocal) {
	int bestTargetIndex = -1;
	int tickxd = 12;
	float bestFov = FLT_MAX;
	player_info_t info;
	if (!pLocal->IsAlive())
		return;
	for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
	{
		auto entity = (IClientEntity*)interfaces::ent_list->get_client_entity(i);
		if (!entity || !pLocal)
			continue;
		if (entity == pLocal)
			continue;
		if (!interfaces::engine->GetPlayerInfo(i, &info))
			continue;
		if (entity->IsDormant())
			continue;
		if (entity->team() == pLocal->team())
			continue;
		if (entity->team() != pLocal->team()) {
			if (entity->IsAlive())
			{
				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = entity->GetBonePos(8);
				Vector origin = entity->GetAbsOriginlol();
				Vector absangles = entity->GetAngles();
				matrix3x4 matrix[128];
				entity->SetupBones(matrix, 128, 0x00000100, interfaces::globals->curtime);
				head_positions[i][cmd->command_number % tickxd + 1] = backtrack_data{ simtime, hitboxPos,origin,absangles,entity->GetFlags() ,matrix };
				entity->SetupBones(head_positions[i][cmd->command_number % tickxd + 1].ys, 128, 0x00000100, interfaces::globals->curtime);
				Vector ViewDir = AngleVectorXX(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
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
			bool fuckingnigger;
			Vector ViewDir = AngleVectorXX(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < tickxd + 1; ++t)
			{
				float tempFOVDistance = DistPointToLine(head_positions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && head_positions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					fuckingnigger = true;
					bestTargetSimTime = head_positions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK && fuckingnigger)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}
void c_newbacktrack::legitBackTrack(CUserCmd* cmd, IClientEntity* pLocal)
{
	legitBackTrackT(cmd, pLocal);
}
bool c_newbacktrack::run_lby_backtrack(CUserCmd* m_pcmd, Vector& aim_point) {
	for (int i = 0; i < interfaces::engine->GetMaxClients(); i++)
	{
		if (is_valid_tick(records[i].tick_count))
		{
			aim_point = records[i].head_position;
			m_pcmd->tick_count = records[i].tick_count;
			return true;
		}
		return false;
	}
}
float urdad() {
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
c_newbacktrack* new_backtracking = new c_newbacktrack();
backtrack_data head_positions[64][12];
























































































































































































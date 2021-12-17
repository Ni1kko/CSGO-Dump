#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Movement.h"
#include "../HOOKS/hooks.h"
#include "../NewBacktracking.h"
#include "../LagCompensation.h"

//--- Variable Initaliztion ---//
int bestHitbox = -1, mostDamage;
Vector multipoints[128];
int multipointCount = 0;
bool lag_comp;
bool IsAimStepping = false;

int highest_damage = 0, best_tick, last_extrapolated_ticks;

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

int GetPriority(int priority) {
	if (priority == 0)
		return SDK::HitboxList::HITBOX_HEAD;
	else if (priority == 1)
		return SDK::HitboxList::HITBOX_CHEST;
	else if (priority == 2)
		return SDK::HitboxList::HITBOX_BODY;
	else if (priority == 3)
		return SDK::HitboxList::HITBOX_PELVIS;
}
void Do_pSilent(Vector AimbotAng, Vector Antiaimang, SDK::CUserCmd* cmd)
{
	bool UnLagNextTick = false;
	auto LocalPlayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(LocalPlayer->GetActiveWeaponIndex()));
	float flServerTime = (float)LocalPlayer->GetTickBase() * INTERFACES::Globals->interval_per_tick;
	float flNextPrimaryAttack = weapon->GetNextPrimaryAttack();

	bool BulletTime = true;

	if (flNextPrimaryAttack > flServerTime)
		BulletTime = false;

	if (cmd->buttons & IN_ATTACK && BulletTime && !UnLagNextTick)
	{
		cmd->viewangles = AimbotAng;

		GLOBAL::should_send_packet = false;
		UnLagNextTick = true;
	}
	else
	{
		cmd->buttons &= ~IN_ATTACK;
		if (UnLagNextTick)
		{
			UnLagNextTick = false;

			GLOBAL::should_send_packet = true;
		}
		cmd->viewangles = Antiaimang;
	}
}
int GetFxPS()
{
	static int fps = 0;
	static int count = 0;
	using namespace std::chrono;
	auto now = high_resolution_clock::now();
	static auto last = high_resolution_clock::now();
	count++;
	if (duration_cast< milliseconds >(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}
	return fps;
}

void CAimbot::rotate_movement(float yaw, SDK::CUserCmd* cmd)
{
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * cmd->move.x) - (sin_rot * cmd->move.y);
	float new_sidemove = (sin_rot * cmd->move.x) + (cos_rot * cmd->move.y);

	cmd->move.x = new_forwardmove;
	cmd->move.y = new_sidemove;
}

int lerped_ticks()
{
	static const auto cl_interp_ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");
	static const auto cl_updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static const auto cl_interp = INTERFACES::cvar->FindVar("cl_interp");

	return TIME_TO_TICKS(max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat()));
}

static SDK::ConVar *big_ud_rate = nullptr,
*min_ud_rate = nullptr, *max_ud_rate = nullptr,
*interp_ratio = nullptr, *cl_interp = nullptr,
*cl_min_interp = nullptr, *cl_max_interp = nullptr;


float GetLerpTime() 
{
	static SDK::ConVar* cl_interp = INTERFACES::cvar->FindVar("cl_interp");
	static SDK::ConVar* cl_updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static SDK::ConVar* cl_interp_ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");
	static SDK::ConVar* sv_maxupdaterate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	static SDK::ConVar* sv_minupdaterate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	static SDK::ConVar* sv_client_min_interp_ratio = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	static SDK::ConVar* sv_client_max_interp_ratio = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
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

float LerpTime()
{
	static SDK::ConVar* updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static SDK::ConVar* minupdate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	static SDK::ConVar* maxupdate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	static SDK::ConVar* lerp = INTERFACES::cvar->FindVar("cl_interp");
	static SDK::ConVar* cmin = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	static SDK::ConVar* cmax = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
	static SDK::ConVar* ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");

	float lerpurmom = lerp->GetFloat(), maxupdateurmom = maxupdate->GetFloat(),
		ratiourmom = ratio->GetFloat(), cminurmom = cmin->GetFloat(), cmaxurmom = cmax->GetFloat();
	int updaterateurmom = updaterate->GetInt(),
		sv_maxupdaterate = maxupdate->GetInt(), sv_minupdaterate = minupdate->GetInt();

	if (sv_maxupdaterate && sv_minupdaterate) updaterateurmom = maxupdateurmom;
	if (ratiourmom == 0) ratiourmom = 1.0f;
	if (cmin && cmax && cmin->GetFloat() != 1) ratiourmom = clamp(ratiourmom, cminurmom, cmaxurmom);
	return max(lerpurmom, ratiourmom / updaterateurmom);
}

bool CAimbot::good_backtrack_tick(int simTime)
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci) return false;

	/*float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + LerpTime(), 0.f, 1.f);
	float delta_time = correct - (INTERFACES::Globals->curtime - TICKS_TO_TIME(simTime));
	return fabsf(delta_time) < 0.2f;*/

	auto LerpTicks = TIME_TO_TICKS(LerpTime());

	int predCmdArrivTick = INTERFACES::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(LerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(LerpTime())));

	return abs(flCorrect) < 0.2f;
}

void CAimbot::run_aimbot(SDK::CUserCmd* cmd)
{
	Entities.clear();
	SelectTarget();
	auto_revolver(cmd);
	shoot_enemy(cmd);
}

float CAimbot::GetLatency1()
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return 0.f;

	return nci->GetLatency(FLOW_OUTGOING);
}


void NormalizeMeme(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

void NormaliseViewAngleMeme(Vector &angle)
{
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;
}



void CAimbot::SelectTarget()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	for (int index = 1; index <= 65; index++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);
		if (!entity) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		//if (entity->GetClientClass()->m_ClassID != 38) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;
		if (entity->GetImmunity()) continue;
		if (entity->GetIsDormant())	continue;
		AimbotData_t data = AimbotData_t(entity, index);
		Entities.push_back(data);
	}
}
void CAimbot::lby_backtrack(SDK::CUserCmd *pCmd, SDK::CBaseEntity* pLocal, SDK::CBaseEntity* pEntity)
{
	int index = pEntity->GetIndex();
	float PlayerVel = abs(pEntity->GetVelocity().Length2D());

	bool playermoving;

	if (PlayerVel > 0.f)
		playermoving = true;
	else
		playermoving = false;

	float lby = pEntity->GetLowerBodyYaw();
	static float lby_timer[65];
	static float lby_proxy[65];

	if (lby_proxy[index] != pEntity->GetLowerBodyYaw() && playermoving == false)
	{
		lby_timer[index] = 0;
		lby_proxy[index] = pEntity->GetLowerBodyYaw();
	}

	if (playermoving == false)
	{
		if (pEntity->GetSimTime() >= lby_timer[index])
		{
			tick_to_back[index] = pEntity->GetSimTime();
			lby_to_back[index] = pEntity->GetLowerBodyYaw();
			lby_timer[index] = pEntity->GetSimTime() + INTERFACES::Globals->interval_per_tick + 1.1;
		}
	}
	else
	{
		tick_to_back[index] = 0;
		lby_timer[index] = 0;
	}

	if (good_backtrack_tick(TIME_TO_TICKS(tick_to_back[index])))
		backtrack_tick[index] = true;
	else
		backtrack_tick[index] = false;
}

inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void slowmogang(SDK::CUserCmd* cmd, float fMaxSpeed) {
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(GLOBAL::originalCMD.move.x) + square(GLOBAL::originalCMD.move.y) + square(GLOBAL::originalCMD.move.z)));
	if (fSpeed <= 0.f)
		return;
	if (cmd->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	GLOBAL::originalCMD.move.x *= fRatio;
	GLOBAL::originalCMD.move.y *= fRatio;
	GLOBAL::originalCMD.move.z *= fRatio;
}

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast< float >(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast< float >(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast< float >(M_PI));
	auto cp = cos(meme.x / 180.f* static_cast< float >(M_PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

SDK::mstudiobbox_t* CAimbot::GetHitbox(SDK::CBaseEntity* entity, int hitbox_index)
{
	if (entity->GetIsDormant() || entity->GetHealth() <= 0)
		return nullptr;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return nullptr;

	auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
	if (!pStudioHdr)
		return nullptr;

	auto pSet = pStudioHdr->pHitboxSet(0);
	if (!pSet)
		return nullptr;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return nullptr;

	return pSet->GetHitbox(hitbox_index);
}

bool CAimbot::is_slow_walking(SDK::CBaseEntity* entity) 
{
	float velocity_2D[64], old_velocity_2D[64];

	if (entity->GetVelocity().Length2D() != velocity_2D[entity->GetIndex()] && entity->GetVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->GetIndex()] = velocity_2D[entity->GetIndex()];
		velocity_2D[entity->GetIndex()] = entity->GetVelocity().Length2D();
	}

	if (velocity_2D[entity->GetIndex()] > 0.1) {
		int tick_counter[64];

		if (velocity_2D[entity->GetIndex()] == old_velocity_2D[entity->GetIndex()])
			++tick_counter[entity->GetIndex()];
		else
			tick_counter[entity->GetIndex()] = 0;

		while (tick_counter[entity->GetIndex()] > (1 / INTERFACES::Globals->interval_per_tick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}
	return false;
}

Vector CAimbot::GetHitboxPosition(SDK::CBaseEntity* entity, int hitbox_index)
{
	const auto hitbox = GetHitbox(entity, hitbox_index);
	if (!hitbox)
		return Vector(0, 0, 0);

	const auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
	MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

void inline SinCos1(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void AAngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos1(DEG2RAD(angles[1]), &sy, &cy);
	SinCos1(DEG2RAD(angles[0]), &sp, &cp);
	SinCos1(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}
	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

bool IsAccurate(SDK::CBaseWeapon* weapon, Vector position)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	 

	if (!local_player)
		return false;

	if (!weapon)
		return false;

	

	switch (SETTINGS::settings.rbot_accuraccy_type)
	{
	case 0: /// hitchance
	{
		if (aimbot->accepted_inaccuracy(weapon) >= SETTINGS::settings.chance_val)
			return true;
		else
			return false;
		 // stackhack hc
	}
	break;
	case 1: /// this would be spread limit
	{
		if (!weapon)
			return false;

		return weapon->GetInaccuracy() <= SETTINGS::settings.spread_val;
	}
	break;
	}
	
	return false;
}


void RotateMovement(SDK::CUserCmd* cmd, float yaw)
{
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * cmd->move.y) - (sin_rot * cmd->move.x);
	float new_sidemove = (sin_rot * cmd->move.y) + (cos_rot * cmd->move.x);

	cmd->move.x = new_forwardmove;
	cmd->move.y = new_sidemove;
}

int TicksToStop(Vector velocity)
{
	static auto sv_maxspeed = INTERFACES::cvar->FindVar("sv_maxspeed");

	const float max_speed = 320.f;
	const float acceleration = 5.5f;
	const float max_accelspeed = acceleration * max_speed * INTERFACES::Globals->interval_per_tick;

	return velocity.Length() / max_accelspeed;
}

Vector TickPrediction(Vector AimPoint, SDK::CBaseEntity* entity)
{
	return AimPoint + (entity->GetVelocity() * INTERFACES::Globals->interval_per_tick);
}

void InvalidateBones(SDK::CBaseEntity* entity)
{
	static uintptr_t InvalidateBoneCache = (DWORD)UTILS::FindSignature("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	static uintptr_t g_iModelBoneCounter = **(uintptr_t**)(InvalidateBoneCache + 10); //	Offsets::InvalidateBoneCache = FindPatternIDA("client.dll", "80 3D ? ? ? ? 00 74 16 A1");
	*(int*)((uintptr_t)entity + OFFSETS::m_nForceBone + 0x20) = 0; //m_nForceBone + 0x20
	*(uintptr_t*)((uintptr_t)entity + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(uintptr_t*)((uintptr_t)entity + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}
void ClampMovement(SDK::CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;

	float fSpeed = (float)(FastSqrt(square(pCommand->move.x) + square(pCommand->move.y)));
	if (fSpeed <= 0.f)
		return;

	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f; // TO DO: Maybe look trough the leaked sdk for an exact value since this is straight out of my ass...

	if (fSpeed <= fMaxSpeed)
		return;

	float fRatio = fMaxSpeed / fSpeed;

	pCommand->move.x *= fRatio;
	pCommand->move.y *= fRatio;
}


int random_ticks = 0;
void CAimbot::shoot_enemy(SDK::CUserCmd* cmd)
{
	SDK::CBaseEntity* best_entity = nullptr;
	int targetID = 0;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	auto weapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_C4 || weapon->is_grenade() || weapon->is_knife()) return;
	if (!can_shoot()) { cmd->buttons &= ~IN_ATTACK;	return; }
	if (GetAsyncKeyState(VK_LBUTTON)) return;
	Vector aim_angles;

	LagRecord record;

	for (auto players : Entities)
	{
		auto entity = players.pPlayer;
		auto class_id = entity->GetClientClass()->m_ClassID;

		if (!entity) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
	//	if (class_id != 38) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;
		if (entity->GetImmunity()) continue;
		if (entity->GetIsDormant()) continue;



		Vector where2Shoot;

		if (SETTINGS::settings.multi_bool) where2Shoot = aimbot->multipoint(entity, SETTINGS::settings.acc_type);
		else where2Shoot = aimbot->point(entity, SETTINGS::settings.acc_type);
		if (where2Shoot == Vector(0, 0, 0)) continue;

		if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_AWP || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SSG08 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_G3SG1 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_AUG || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SG556)
		{
			if (!local_player->GetIsScoped())
				cmd->buttons |= IN_ATTACK2;
		}


	   static auto auto_stop = [&cmd]()
		{
			if (!GLOBAL::is_slowwalking)
			{
				if (SETTINGS::settings.autostop)
					slowmogang(cmd, 30);
			}
		};
		auto_stop();
		
		aim_angles = MATH::NormalizeAngle(UTILS::CalcAngle(local_player->GetEyePosition(), where2Shoot));
		if (aim_angles == Vector(0, 0, 0)) continue;

	
	
		if (IsAccurate(weapon, aim_angles)) 
		{
			cmd->buttons |= IN_ATTACK;
		}
	
		if (entity->GetVelocity().Length2D() > 0)
		{
			if (can_shoot() && good_backtrack_tick(TIME_TO_TICKS(entity->GetSimTime())))
				cmd->tick_count = TIME_TO_TICKS(entity->GetSimTime() + LerpTime());
		}
		else
		{
			if (backtrack_tick[entity->GetIndex()])
				cmd->tick_count = TIME_TO_TICKS(tick_to_back[entity->GetIndex()] + LerpTime());
		}

		shots_fired[entity->GetIndex()]++;
		break;
	}

	if (cmd->buttons & IN_ATTACK)
	{
		GLOBAL::is_shooting = true;
		float recoil_scale = INTERFACES::cvar->FindVar("weapon_recoil_scale")->GetFloat(); GLOBAL::should_send_packet = true;
		aim_angles -= local_player->GetPunchAngles() * recoil_scale; cmd->viewangles = aim_angles;
		//	Do_pSilent(aim_angles, Vector(89, 180, 0), cmd);
	}
	else
		GLOBAL::is_shooting = false;
	if (SETTINGS::settings.accuracystanding && !(cmd->buttons & IN_DUCK))
		ClampMovement(cmd, 81.f);
}

float CAimbot::accepted_inaccuracy(SDK::CBaseWeapon* weapon)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return 0;

	if (!weapon) return 0;
	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_TASER) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}



/*std::vector<Vector> GetMultiplePointsForHitbox(SDK::CBaseEntity* local, SDK::CBaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128])
{
	auto VectorTransform_Wrapper = [](const Vector& in1, const matrix3x4_t &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const matrix3x4_t& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	SDK::mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);
	SDK::mstudiobbox_t *hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max, min;

	VectorTransform_Wrapper(hitbox->bbmax, BoneMatrix[hitbox->bone], max);
	VectorTransform_Wrapper(hitbox->bbmin, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = UTILS::CalcAngle(center, local->GetVecOrigin() + local->GetViewOffset()),
		Forward;

	MATH::AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1)),
		Left = Vector(-Right.x, -Right.y, Right.z),
		Top = Vector(0, 0, 1),
		Bot = Vector(0, 0, -1);

	float headscale = SETTINGS::settings.headscale / 100.f;
	float neckscale = SETTINGS::settings.neckscale / 100.f;
	float chestscale = SETTINGS::settings.chestscale / 100.f;
	float pelvisscale = SETTINGS::settings.pelvisscale / 100.f;
	float bodyscale = SETTINGS::settings.bodyscale / 100.f;
	float armsscale = SETTINGS::settings.armsscale / 100.f;
	float leggscale = SETTINGS::settings.legsscale / 100.f;

	switch (iHitbox) {
	case SDK::HITBOX_HEAD:
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_head) 
		{
			vecArray[1] += Top * (hitbox->radius * headscale);
			vecArray[2] += Right * (hitbox->radius * headscale);
			vecArray[3] += Left * (hitbox->radius * headscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_NECK:
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_neck) {
			vecArray[1] += Top * (hitbox->radius * neckscale);
			vecArray[2] += Right * (hitbox->radius * neckscale);
			vecArray[3] += Left * (hitbox->radius * neckscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_UPPER_CHEST:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_chest) {
			vecArray[1] += Right * (hitbox->radius * chestscale);
			vecArray[2] += Left * (hitbox->radius * chestscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_CHEST:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_chest) {
			vecArray[1] += Right * (hitbox->radius * chestscale);
			vecArray[2] += Left * (hitbox->radius * chestscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_THORAX:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_chest) {
			vecArray[1] += Right * (hitbox->radius * chestscale);
			vecArray[2] += Left * (hitbox->radius * chestscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_BODY:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_body) {
			vecArray[1] += Right * (hitbox->radius * bodyscale);
			vecArray[2] += Left * (hitbox->radius * bodyscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_PELVIS:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_pelvis) {
			vecArray[1] += Right * (hitbox->radius * pelvisscale);
			vecArray[2] += Left * (hitbox->radius * pelvisscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_LEFT_UPPER_ARM:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_arms) {
			vecArray[0] += Right * (hitbox->radius * armsscale);
			vecArray[1] += Left * (hitbox->radius * armsscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_RIGHT_UPPER_ARM:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_arms) {
			vecArray[0] += Right * (hitbox->radius * armsscale);
			vecArray[1] += Left * (hitbox->radius * armsscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_LEFT_FOREARM:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_arms) {
			vecArray[0] += Right * (hitbox->radius * armsscale);
			vecArray[1] += Left * (hitbox->radius * armsscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_RIGHT_FOREARM:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_arms) {
			vecArray[0] += Right * (hitbox->radius * armsscale);
			vecArray[1] += Left * (hitbox->radius * armsscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_LEFT_THIGH:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[1] += Right * (hitbox->radius * leggscale);
			vecArray[2] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_RIGHT_THIGH:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[1] += Right * (hitbox->radius * leggscale);
			vecArray[2] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_LEFT_CALF:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[1] += Right * (hitbox->radius * leggscale);
			vecArray[2] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_RIGHT_CALF:
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[1] += Right * (hitbox->radius * leggscale);
			vecArray[2] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}
		break;
	case SDK::HITBOX_LEFT_FOOT:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[0] += Right * (hitbox->radius * leggscale);
			vecArray[1] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}

		break;
	case SDK::HITBOX_RIGHT_FOOT:
		for (auto i = 0; i < 2; ++i)
			vecArray.emplace_back(center);

		if (SETTINGS::settings.multi_legs) {
			vecArray[0] += Right * (hitbox->radius * leggscale);
			vecArray[1] += Left * (hitbox->radius * leggscale);
		}
		else {
			vecArray[0] += Right * (hitbox->radius * 0);
			vecArray[1] += Left * (hitbox->radius * 0);
		}

		break;
	}
	return vecArray;
}*/

std::vector<Vector> CAimbot::GetMultiplePointsForHitbox(SDK::CBaseEntity* local, SDK::CBaseEntity* entity, int iHitbox, VMatrix BoneMatrix[128])
{
	auto VectorTransform_Wrapper = [](const Vector& in1, const VMatrix &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const VMatrix& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	SDK::mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);
	SDK::mstudiobbox_t *hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	VectorTransform_Wrapper(hitbox->bbmax, BoneMatrix[hitbox->bone], max);
	VectorTransform_Wrapper(hitbox->bbmin, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = UTILS::CalcAngle(center, local->GetEyePosition());

	Vector Forward;
	MATH::AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(-Right.x, -Right.y, Right.z);

	Vector Top = Vector(0, 0, 1);
	Vector Bot = Vector(0, 0, -1);

	switch (iHitbox) {
	case 0:
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Top * (hitbox->radius * SETTINGS::settings.point_val / 100.f);
		vecArray[2] += Right * (hitbox->radius * SETTINGS::settings.point_val / 100.f);
		vecArray[3] += Left * (hitbox->radius * SETTINGS::settings.point_val / 100.f);
		break;

	default:

		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Right * (hitbox->radius * SETTINGS::settings.body_val / 100.f);
		vecArray[2] += Left * (hitbox->radius * SETTINGS::settings.body_val / 100.f);
		break;
	}
	return vecArray;
}

Vector CAimbot::get_hitbox_pos(SDK::CBaseEntity* entity, int hitbox_id)
{
	auto getHitbox = [](SDK::CBaseEntity* entity, int hitboxIndex) -> SDK::mstudiobbox_t*
	{
		if (entity->GetIsDormant() || entity->GetHealth() <= 0) return NULL;

		const auto pModel = entity->GetModel();
		if (!pModel) return NULL;

		auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
		if (!pStudioHdr) return NULL;

		auto pSet = pStudioHdr->pHitboxSet(0);
		if (!pSet) return NULL;

		if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0) return NULL;

		return pSet->GetHitbox(hitboxIndex);
	};

	auto hitbox = getHitbox(entity, hitbox_id);
	if (!hitbox) return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
	MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}


enum WPDamageList
{
	AutoSniper = 50,
	Scout = 70,
	Awp = 90,
	Rifles = 35,
	Pistols = 20,
	Deagle_R8 = 50
};

bool BaimIfLethal(SDK::CBaseEntity* player)
{
	SDK::CBaseWeapon* weapon = nullptr;

	if (!player) return false;
	if (!weapon) return false;
	

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_G3SG1 && player->GetHealth() <= WPDamageList::AutoSniper)
	{
		return true;
	}
	else if (weapon->is_revolver() || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_DEAGLE && player->GetHealth() <= WPDamageList::Deagle_R8)
	{
		return true;
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_AWP && player->GetHealth() <= WPDamageList::Awp)
	{
		return true;
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SSG08 && player->GetHealth() <= WPDamageList::Scout)
	{
		return true;
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_CZ75A || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_USP_SILENCER ||
		weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_ELITE || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_P250 ||
		weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_GLOCK || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_HKP2000 ||
		weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_TEC9 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_FIVESEVEN && player->GetHealth() <= WPDamageList::Pistols)
	{
		return true;
	}
	else
	{
		return false;
	}

}

Vector CAimbot::multipoint(SDK::CBaseEntity* entity, int option)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);


	//PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[entity->GetIndex()].at(0);

	Vector vector_best_point = Vector(0, 0, 0);
	int maxDamage = 0;
	maxDamage = SETTINGS::settings.damage_val;

	//matrix3x4_t matrix[128];
	VMatrix matrix[128];
	if (!entity->SetupBonesOld(matrix, 128, 256, 0)) return Vector(0, 0, 0);

	//if (!entity->SetupBones(matrix, 128, 256, 0)) return Vector(0, 0, 0);

	std::vector<int> hitscan;
	std::vector<int> hitscan_priority;
	
	
	
	/*if (SETTINGS::settings.head_scan)
		hitscan.push_back((int)SDK::HitboxList::HITBOX_HEAD);
	if (SETTINGS::settings.neck_scan)
		hitscan.push_back((int)SDK::HitboxList::HITBOX_NECK);
	if (SETTINGS::settings.chest_scan) 
	{
		hitscan.push_back((int)SDK::HitboxList::HITBOX_CHEST);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_UPPER_CHEST);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_THORAX);
	}
	if (SETTINGS::settings.body_scan)
		hitscan.push_back((int)SDK::HitboxList::HITBOX_BODY);
	if (SETTINGS::settings.pelvis_scan)
		hitscan.push_back((int)SDK::HitboxList::HITBOX_PELVIS);
	if (SETTINGS::settings.arms_scan) 
	{
		hitscan.push_back((int)SDK::HitboxList::HITBOX_LEFT_FOREARM);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_RIGHT_FOREARM);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_LEFT_HAND);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_RIGHT_HAND);
	}
	if (SETTINGS::settings.legs_scan) 
	{
		hitscan.push_back((int)SDK::HitboxList::HITBOX_LEFT_CALF);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_RIGHT_CALF);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_LEFT_THIGH);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_RIGHT_THIGH);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_LEFT_FOOT);
		hitscan.push_back((int)SDK::HitboxList::HITBOX_RIGHT_FOOT);
	}*/

	
	
	
	


	int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD, SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST, SDK::HitboxList::HITBOX_LEFT_CALF, SDK::HitboxList::HITBOX_RIGHT_CALF,SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT };

	for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
	{
		for (auto point : GetMultiplePointsForHitbox(local_player, entity, hitboxes[i], matrix))
		{
			int damage = AutoWall->GetDamagez(point);
			if (damage > maxDamage)
			{
				bestHitbox = hitboxes[i];
				maxDamage = damage;
				vector_best_point = point;

				if (maxDamage >= entity->GetHealth())
					return vector_best_point;
			}
		}

		
	}

	if (is_slow_walking(entity))
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST };
	}

	if (entity->GetHealth() <= 30)
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST };
		maxDamage = entity->GetHealth();
	}

	if (BaimIfLethal(entity))
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST };
	}
	//PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[entity->GetIndex()].at(0);

	

	
	

	return vector_best_point;
}
Vector CAimbot::point(SDK::CBaseEntity* entity, int option)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector vector_best_point = Vector(0, 0, 0);
	int maxDamage = SETTINGS::settings.damage_val;

	switch (option)
	{
	case 0:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{

			int damage = AutoWall->GetDamagez(entity->GetBonePosition(hitboxes[i]));
			if (damage > maxDamage)
			{
				bestHitbox = hitboxes[i];
				maxDamage = damage;
				vector_best_point = get_hitbox_pos(entity, bestHitbox);

				if (maxDamage >= entity->GetHealth())
					return vector_best_point;
			}
		}
	} break;
	case 1:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_THORAX, SDK::HitboxList::HITBOX_MAX };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			int damage = AutoWall->GetDamagez(entity->GetBonePosition(hitboxes[i]));
			if (damage > maxDamage)
			{
				bestHitbox = hitboxes[i];
				maxDamage = damage;
				vector_best_point = get_hitbox_pos(entity, bestHitbox);

				if (maxDamage >= entity->GetHealth())
					return vector_best_point;
			}
		}
	} break;
	case 2:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD, SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST, SDK::HitboxList::HITBOX_LEFT_CALF, SDK::HitboxList::HITBOX_RIGHT_CALF,SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			int damage = AutoWall->GetDamagez(entity->GetBonePosition(hitboxes[i]));
			if (damage > maxDamage)
			{
				bestHitbox = hitboxes[i];
				maxDamage = damage;
				vector_best_point = get_hitbox_pos(entity, bestHitbox);

				if (maxDamage >= entity->GetHealth())
					return vector_best_point;
			}
		}
	} break;
	}
	return vector_best_point;
}

bool CAimbot::can_shoot()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;
	if (local_player->GetHealth() <= 0) return false;

	auto weapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;

	return (weapon->GetNextPrimaryAttack() < UTILS::GetCurtime()) && (local_player->GetNextAttack() < UTILS::GetCurtime());
}

void CAimbot::auto_revolver(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player && local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return;

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_REVOLVER) {
		cmd->buttons |= IN_ATTACK;
		float flPostponeFireReady = weapon->GetPostponeFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady - 1 < INTERFACES::Globals->curtime) {
			cmd->buttons &= ~IN_ATTACK;
		}
	}
}

int CAimbot::get_damage(Vector position)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return 0;

	auto weapon = reinterpret_cast< SDK::CBaseWeapon* >(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon)
		return 0;

	SDK::trace_t trace;
	SDK::Ray_t ray;
	SDK::CTraceWorldOnly filter;
	ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), position);

	INTERFACES::Trace->TraceRay(ray, MASK_ALL, (SDK::ITraceFilter*)&filter, &trace);

	if (trace.flFraction == 1.f)
	{
		auto weapon_info = weapon->get_full_info();
		if (!weapon_info)
			return -1;

		return weapon_info->m_Damage;
		return 1;
	}
	else
		return 0;
}	



CAimbot* aimbot = new CAimbot();














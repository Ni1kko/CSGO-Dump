#include "Legitbot.h"




bool CLegitBot::entity_is_valid(int i)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);
	if (!pEntity) return false;
	if (pEntity == local_player) return false;
	if (pEntity->GetTeam() == local_player->GetTeam()) return false;
	if (!pEntity->GetIsDormant()) return false;
	if (!pEntity->GetImmunity()) return false;
	if (pEntity->GetHealth() <= 0) return false;
	return true;
}

void CLegitBot::triggerbot(SDK::CUserCmd* cmd)
{
	Vector src, dst, forward;
	SDK::trace_t tr;
	SDK::Ray_t ray;
	SDK::CTraceFilter filter;
	Vector viewangle = cmd->viewangles;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->is_grenade() || weapon->is_knife() || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_C4) return;

	viewangle += local_player->GetPunchAngles() * 2.f;
	MATH::AngleVectors(viewangle, &forward);
	forward *= weapon->get_full_info()->m_Range;
	filter.pSkip1 = local_player;

	src = local_player->GetEyePosition();
	dst = src + forward;

	ray.Init(src, dst);

	INTERFACES::Trace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.m_pEnt)
		return;

	int hitgroup = tr.hitGroup;
	bool didHit = false;
	didHit = false;

	if (hitgroup == HITGROUP_HEAD || hitgroup == HITGROUP_CHEST || hitgroup == HITGROUP_STOMACH || hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG)
		didHit = true;
	else
		didHit = false;

	if (tr.m_pEnt->GetTeam() == local_player->GetTeam()) return;
	if (tr.m_pEnt->GetHealth() <= 0) return;
	if (tr.m_pEnt->GetImmunity()) return;
	if (didHit) cmd->buttons |= IN_ATTACK;
}
void normalisememe(Vector &angle)
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

inline void SinCos(float radians, float *sine, float *cosine)
{
	_asm
	{
		fld		DWORD PTR[radians]
		fsincos

		mov edx, DWORD PTR[cosine]
		mov eax, DWORD PTR[sine]

		fstp DWORD PTR[edx]
		fstp DWORD PTR[eax]
	}
}

inline void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

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
inline void VectorAngles(Vector &forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;

		if (forward.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = RAD2DEG(atan2f(forward.y, forward.x));

		if (yaw < 0)
			yaw += 360;

		tmp = forward.Length2D();
		pitch = RAD2DEG(atan2f(-forward.z, tmp));

		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

inline void ClampAngle(Vector& qaAng)
{
	while (qaAng.x > 89)
		qaAng.x -= 180;

	while (qaAng.x < -89)
		qaAng.x += 180;

	while (qaAng.y > 180)
		qaAng.y -= 360;

	while (qaAng.y < -180)
		qaAng.y += 360;

	while (qaAng.z != 0)
		qaAng.z = 0;
}

inline void AngleVectors(const Vector &angles, Vector *forward)
{

	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sinf(DEG2RAD(angles[1])); // yaw
	cy = cosf(DEG2RAD(angles[1]));

	sp = sinf(DEG2RAD(angles[0])); // pitch
	cp = cosf(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

inline void AngleNormalize(Vector& v)
{
	for (auto i = 0; i < 3; i++)
	{
		if (v[i] < -180.0f) v[i] += 360.0f;
		if (v[i] >  180.0f) v[i] -= 360.0f;
	}
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

void pNormalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, SDK::CBaseEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	MATH::AngleVectors(Angles, &Forward);
	Vector AimPos = pEntity->GetHitboxPosition(pEntity, aHitBox);

	MATH::VectorSubtract(AimPos, Origin, Delta);

	pNormalize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}

void NormaliseVAngleThing(Vector &angle)
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

void CalcAngleReal(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}


bool CLegitBot::AimAtPoint(SDK::CBaseEntity* pLocal, Vector point, SDK::CUserCmd *pCmd)
{

	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetVecOrigin() + pLocal->GetViewOffset();

	CalcAngleReal(src, point, angles);
	NormaliseVAngleThing(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}
	
	if (RecoilControl > 0)
	{

		Vector AimPunch = pLocal->GetPunchAngles();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * RecoilControl;
			NormaliseVAngleThing(angles);
		}
	}

	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	NormaliseVAngleThing(shit);
	if (shit.Length() > Speed)
	{
		pNormalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;

	}

	pCmd->viewangles += shit;
	INTERFACES::Engine->SetViewAngles(pCmd->viewangles);

	return v;
}

int CLegitBot::GetEntUnderCrosshair()
{
	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	Vector View; INTERFACES::Engine->GetViewAngles(View);
	Vector ViewOffset = pLocal->GetVecOrigin() + pLocal->GetViewOffset();



	for (int i = 1; i < INTERFACES::Engine->GetMaxClients(); i++)
	{
		SDK::CBaseEntity* pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		float fov1 = legitbot->FovToPlayer(ViewOffset, View, pEntity, 0);
		float fov2 = legitbot->FovToPlayer(ViewOffset, View, pEntity, 4);
		float fov3 = legitbot->FovToPlayer(ViewOffset, View, pEntity, 6);

		legitbot->FoV = SETTINGS::settings.legit_bot_fov / 10;

		if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
		{
			FoV = fov1;
			target = i;
			legitbot->bestHitbox = 0;
		}

		if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
		{
			FoV = fov2;
			target = i;
			legitbot->bestHitbox = 4;
		}

		if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
		{
			FoV = fov3;
			target = i;
			legitbot->bestHitbox = 6;
		}

	}

	return target;
}


/*int CLegitBot::ClosestBone(SDK::CBaseEntity *Entity)
{
	float BestDist = 360.f;
	int aimbone;

	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());


	SDK::matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!Entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Engine->GetLastTimeStamp()))
		return -1;

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(Entity->GetModel());
	if (!pStudioModel)
		return -1;

	SDK::mstudiohitboxset_t* set = pStudioModel->pHitboxSet(Entity->GetHitboxSet());
	if (!set)
		return -1;

	for (int i = 0; i < set->numhitboxes; i++)
	{
		SDK::mstudiobbox_t* hitbox = set->GetHitbox(i);

		if (!hitbox)
			continue;

		auto thisdist = FovToPlayer(GLOBAL::originalCMD.viewangles, UTILS::CalcAngle(pLocal->GetEyePosition(), Vector(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3])));

		if (BestDist > thisdist)
		{
			BestDist = thisdist;
			aimbone = hitbox->bone;
			continue;
		}
	}

	return aimbone;
}*/


void CLegitBot::Aimbot(SDK::CUserCmd* cmd)
{
	TargetID = -1;

	SDK::CBaseEntity* pTarget = nullptr;
	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	
   


	 
}
void CLegitBot::RCS(SDK::CUserCmd* cmd)
{
	Vector angles;

	auto pLocal= INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!pLocal) return;

	

	if (pLocal)
	{
		Vector Aimpunch = pLocal->GetPunchAngles();

		if (Aimpunch.Length2D() > 0 && Aimpunch.Length2D() < 150)
		{
			cmd->viewangles -= Aimpunch * 2;
		    normalisememe(cmd->viewangles);
		}
	}
}
static int custom_delay = 0;
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

LegitAimbot legit_aimbot;

void LegitAimbot::Update(SDK::CUserCmd* cmd)
{
	if (cmd->buttons & IN_ATTACK)
		duration_spent_clicking += INTERFACES::Globals->interval_per_tick;
	else
		duration_spent_clicking = 0.f;
}

#define PI 3.14159265358979323846f


void LegitAimbot::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}

float LegitAimbot::flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0f)
		return 1.0f;

	return m_fDistance;

}
float LegitAimbot::GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = UTILS::CalcAngle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}

SDK::CBaseWeapon* GetWeapon12(SDK::CBaseEntity* entity)
{
	return reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
}


int LegitAimbot::GetBestTarget(SDK::CBaseEntity* pLocal, SDK::CUserCmd* pCommand)
{
	int iBestTarget = -1;
	int bestTarget = -1;				 //this num could be smaller 
	float flDistToBest = 99999.f;

	double minimalDistance = 99999.0;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= INTERFACES::Engine->GetMaxClients(); i++)
	{
		if (pLocal)
			continue;

	    SDK::CBaseEntity* pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->GetIsDormant())
			continue;

		if (!pEntity->IsAlive() || pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetBonePosition(iBestHitbox);


		float flFOV = GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = flGetDistance(vEntity, pLocal->GetEyePosition());

		if (distance < minimalDistance)
		{
			if (flFOV < flDistToBest && flFOV < 3)
			{

				flDistToBest = flFOV;
				//gCvars.iAimbotIndex = i;
				iBestTarget = i;
			}
		}

	
	}

	return iBestTarget;
}

bool LegitAimbot::IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	SDK::trace_t Trace;
	SDK::Ray_t Ray;			 // the future of variable naming
	SDK::CTraceFilter Filter;

	Filter.pSkip1 = pLocal;

	Ray.Init(vStart, vEnd);

	INTERFACES::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity);
}

int LegitAimbot::GetBestHitbox(SDK::CBaseEntity* pLocal, SDK::CBaseEntity* pEntity)
{
	int iBestHitbox = -1;


	if (SETTINGS::settings.lbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetBonePosition(i)))
				return i;
		}
	}

	if (pEntity->GetBonePosition(iBestHitbox).IsZero())
		return -1;




	if (!IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetBonePosition(iBestHitbox)))
		return -1;

	return iBestHitbox;
}

void LegitAimbot::Run(SDK::CBaseEntity* pLocal, SDK::CUserCmd* pCommand)
{

	Vector m_vOldViewAngle = pCommand->viewangles;
	float m_fOldSideMove = pCommand->move.y;
	float m_fOldForwardMove = pCommand->move.x;

	//gCvars.iAimbotIndex = -1;

	if (!SETTINGS::settings.legit_bot_aimbot)
		return;

	if (!pCommand->buttons & IN_ATTACK)
		return;

	if (!pLocal->GetActiveWeaponIndex())
		return;

	SDK::CBaseEntity* pEntity = INTERFACES::ClientEntityList->GetClientEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity;

	vEntity = pEntity->GetBonePosition(iBestHitbox);


	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;

	VectorAngles((vEntity - vLocal), vAngs);
	ClampAngle(vAngs);
	pCommand->viewangles = vAngs;
	INTERFACES::Engine->SetViewAngles(pCommand->viewangles);

	if (SETTINGS::settings.legit_bot_smooth > 0.0)
	{
		Vector vDelta(pCommand->viewangles - vAngs);
		AngleNormalize(vDelta);
		vAngs = pCommand->viewangles - vDelta / SETTINGS::settings.legit_bot_smooth;
	}




	//FixMove(pCommand, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
}
void LegitAimbot::Do(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;



	if (!cmd->buttons & IN_ATTACK) return;


	auto weapon = GetWeapon12(local_player);
	if (!weapon)
		return;

	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return;

	static constexpr int hitboxes_to_check[] = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 18, 16 };
	static constexpr bool aimbot_enabled = false;

	auto DoAimbot = [local_player, this](Vector& viewangles, float aimbot_fov) -> void
	{
		auto CanHit = [](Vector start, Vector end, SDK::CBaseEntity* ignore) -> bool
		{
			SDK::trace_t trace;
			SDK::CTraceFilter filter;
			filter.pSkip1 = ignore;
			SDK::Ray_t ray;
			ray.Init(start, end);

			INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

			return trace.hitGroup > 0 && trace.hitGroup <= 7;
		};
		auto GetFov = [](Vector viewangles, Vector start, Vector end) -> float
		{
			const auto target_angles = MATH::NormalizeAngle(UTILS::CalcAngle(start, end));
			return (viewangles - target_angles).Length2D();
		};
		auto GetHitbox = [](SDK::CBaseEntity* entity, int hitbox_index) -> SDK::mstudiobbox_t*

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
		};

		if (duration_spent_clicking <= 0.f || duration_spent_clicking > 0.5f)
			return;

		const auto eye_position = local_player->GetVecOrigin() + local_player->GetViewOffset();
		const auto true_viewangles = viewangles + (local_player->GetPunchAngles() * 2.f);

		bool has_target = false;
		float lowest_fov = FLT_MAX;
		Vector best_position;
		for (int i = 0; i < 64; i++)
		{
			auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
			if (!entity || entity->GetHealth() <= 0 || entity->GetIsDormant() || entity->GetImmunity() || entity->GetTeam() == local_player->GetTeam())
				continue;

			auto GetClosestHitboxPosition = [local_player, entity, eye_position, true_viewangles, CanHit, GetFov, GetHitbox]() -> Vector
			{
				float lowest_fov = FLT_MAX;
				Vector closest_position;
				for (const int i : hitboxes_to_check)
				{
					const auto hitbox = GetHitbox(entity, i);
					const auto bone_position = Vector(entity->GetBoneMatrix(hitbox->bone)[0][3], entity->GetBoneMatrix(hitbox->bone)[1][3], entity->GetBoneMatrix(hitbox->bone)[2][3]);
					const auto hitbox_position = bone_position + ((hitbox->bbmax + hitbox->bbmin) * 0.5f);

					if (const float fov = GetFov(true_viewangles, eye_position, hitbox_position))
					{
						if (fov < lowest_fov && CanHit(eye_position, hitbox_position, local_player))
						{
							lowest_fov = fov;
							closest_position = hitbox_position;
						}
					}
				}

				return closest_position;
			};

			const auto closest_hitbox = GetClosestHitboxPosition();
			const float current_fov = GetFov(true_viewangles, eye_position, closest_hitbox);
			if (current_fov < lowest_fov)
			{
				lowest_fov = current_fov;
				best_position = closest_hitbox;
				has_target = true;
			}
		}

		if (!has_target || lowest_fov > aimbot_fov)
			return;

		viewangles = MATH::NormalizeAngle(UTILS::CalcAngle(eye_position, best_position)) - (local_player->GetPunchAngles() * 2.f);
	};

	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	if (SETTINGS::settings.legit_bot_aimbot)
		DoAimbot(viewangles, ((viewangles - last_viewangles).Length() * 2.f) + 1.f);

	viewangles = MATH::NormalizeAngle(viewangles);
	INTERFACES::Engine->SetViewAngles(viewangles);

	FEATURESES::RAGEBOT::CBacktracking batrkact;

	
		cmd->viewangles = viewangles;
	    cmd->tick_count += batrkact.GetLerpTicks();
	

	INTERFACES::Engine->GetViewAngles(last_viewangles);
}
	

/*CLegitBot::CLegitBot()
{
	best_target = -1;

	view_angle = Vector(0.0f, 0.0f, 0.0f);
	aim_angle = Vector(0.0f, 0.0f, 0.0f);
	delta_angle = Vector(0.0f, 0.0f, 0.0f);
	final_angle = Vector(0.0f, 0.0f, 0.0f);
	hitbox_position = Vector(0.0f, 0.0f, 0.0f);

	aim_smooth = 1;
	aim_fov = 0;
	randomized_smooth = 0;
	recoil_min = 0;
	recoil_max = 0;
	randomized_angle = 0;
	shoot = false;
}*/

/*void CLegitBot::do_aimbot(SDK::CBaseEntity *local, SDK::CBaseWeapon *weapon, SDK::CUserCmd  *cmd)
{
	if (!local) return;
	if (!weapon) return;
	if (!local->GetHealth() > 0) return;
	if (weapon->get_full_info()->WeaponType == 9) return;
	if (weapon->GetLoadedAmmo() == 0) return;

	weapon_settings(weapon);
	best_target = get_target(local, weapon, cmd, hitbox_position);

	if (best_target == -1) return;
	
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(best_target);
	if (!entity) return;

	if (MATH::get_distance(local->GetEyePosition(), hitbox_position) > 8192.0f)
		return;
//
//	MATH::compute_angle(local->GetEyePosition(), hitbox_position, aim_angle);
//	MATH::NormalizeAngle(aim_angle);
//
//	aim_angle -= get_randomized_recoil(local);
//	aim_angle += get_randomized_angles(local);
//
//	MATH::NormalizeAngle(view_angle);
//
//	delta_angle = view_angle - aim_angle;
//	MATH::NormalizeAngle(delta_angle);
//
//	float randomSmoothing = 1.0f;
//
//	if (randomized_smooth > 1.0f)
//		randomSmoothing = UTILS::random_number_range(randomized_smooth / 10.0f, 1.0f);
//
//	final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
//	MATH::NormalizeAngle(final_angle);
//
//	if (!MATH::sanitize_angles(final_angle))
//		return;
//
//	cmd->viewangles = final_angle;
//	INTERFACES::Engine->SetViewAngles(cmd->viewangles);
//}
//
//void CLegitBot::weapon_settings(SDK::CBaseWeapon* weapon)
//{
//	if (!weapon)
//		return;
//
//
//	//if (weapon->IsPistol())
//	//{
//	aim_smooth = SETTINGS::settings.legitaim_val;
//	aim_fov = SETTINGS::settings.legitfov_val;
//	randomized_smooth = 1;
//	recoil_min = SETTINGS::settings.rcsamount_min;
//	recoil_max = SETTINGS::settings.rcsamount_max;
//	randomized_angle = 1;
//	//}
//	/*else if (weapon->IsRifle())
//	{
//	aim_key = g_Options.MainKey;
//	aim_smooth = g_Options.aim_RiflesSmooth;
//	aim_fov = g_Options.aim_RiflesFOV;
//	randomized_smooth = 1;
//	recoil_min = g_Options.aim_RiflesRCSMin;
//	recoil_max = g_Options.aim_RiflesRCSMax;
//	randomized_angle = 1;
//	}*/
//}
//
//Vector CLegitBot::get_randomized_recoil(SDK::CBaseEntity *local)
//{
//	Vector compensatedAngles = (local->GetPunchAngles() * 2.0f) * (UTILS::random_number_range(recoil_min, recoil_max) / 100.0f);
//	MATH::NormalizeAngle(compensatedAngles);
//
//	return (local->ShotsFired() > 1 ? compensatedAngles : Vector(0.0f, 0.0f, 0.0f));
//}
//
//Vector CLegitBot::get_randomized_angles(SDK::CBaseEntity *local)
//{
//	Vector randomizedValue = Vector(0.0f, 0.0f, 0.0f);
//
//	float randomRate = UTILS::random_number_range(-randomized_angle, randomized_angle);
//	float randomDeviation = UTILS::random_number_range(-randomized_angle, randomized_angle);
//
//	switch (rand() % 2)
//	{
//	case 0:
//		randomizedValue.x = (randomRate * cos(randomDeviation));
//		randomizedValue.y = (randomRate * cos(randomDeviation));
//		randomizedValue.z = (randomRate * cos(randomDeviation));
//		break;
//	case 1:
//		randomizedValue.x = (randomRate * sin(randomDeviation));
//		randomizedValue.y = (randomRate * sin(randomDeviation));
//		randomizedValue.z = (randomRate * sin(randomDeviation));
//		break;
//	}
//
//	MATH::NormalizeAngle(randomizedValue);
//
//	return (local->ShotsFired() > 1 ? randomizedValue : Vector(0.0f, 0.0f, 0.0f));
//}
//
//bool CLegitBot::get_hitbox(SDK::CBaseEntity *local, SDK::CBaseEntity *entity, Vector &destination)
//{
//	int bestHitbox = -1;
//	float best_fov = aim_fov;
//
//	int hitboxspots;
//
//	/*switch (g_Options.hitbox)
//	{
//	case 0:
//	break;
//	case 1:
//	hitboxspots = HITBOX_HEAD;
//	break;
//	case 2:
//	hitboxspots = HITBOX_NECK;
//	break;
//	case 3:
//	hitboxspots = HITBOX_CHEST;
//	break;
//	case 4:
//	hitboxspots = HITBOX_STOMACH;
//	break;
//	case 5:*/
//	hitboxspots = SDK::HITBOX_HEAD, SDK::HITBOX_NECK, SDK::HITBOX_CHEST, SDK::HITBOX_STOMACH;
//	/*	break;
//	}*/
//
//	std::vector<int> hitboxes = { hitboxspots };
//	for (auto hitbox : hitboxes)
//	{
//		Vector temp;
//		if (!entity->GetHitboxPos(hitbox, temp))
//			continue;
//
//		float fov = MATH::GetFov(view_angle, MATH::CalcAngle(local->GetEyePosition(), temp));
//		if (fov < best_fov)
//		{
//			best_fov = fov;
//			bestHitbox = hitbox;
//		}
//	}
//
//	if (bestHitbox != -1)
//	{
//		if (!entity->GetHitboxPos(bestHitbox, destination))
//			return true;
//	}
//
//	return false;
//}
//
//int CLegitBot::get_target(SDK::CBaseEntity *local, SDK::CBaseWeapon *weapon, SDK::CUserCmd *cmd, Vector &destination)
//{
//	int best_target = -1;
//	float best_fov = FLT_MAX;
//
//	INTERFACES::Engine->GetViewAngles(view_angle);
//
//	for (int i = 1; i <= 65; i++)
//	{
//		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
//		if (!entity_is_valid(i)) continue;
//
//		Vector hitbox;
//		if (get_hitbox(local, entity, hitbox)) continue;
//
//		float fov = MATH::GetFov(view_angle + (local->GetPunchAngles() * 2.0f), MATH::CalcAngle(local->GetEyePosition(), hitbox));
//		if (fov < best_fov && fov < aim_fov)
//		{
//			if (local->CanSeePlayer(entity, hitbox))
//			{
//				best_fov = fov;
//				destination = hitbox;
//				best_target = i;
//			}
//		}
//	}
//
//	return best_target;
//}*/
CLegitBot* legitbot = new CLegitBot();












































































































































































































































































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

//--- Variable Initaliztion ---//
int bestHitbox = -1, mostDamage;
Vector multipoints[128];
int multipointCount = 0;
bool lag_comp;
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}

#define square( x ) ( x * x )
void ClampMovement(SDK::CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f; // TO DO: Maybe look trough the leaked sdk for an exact value since this is straight out of my ass...
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

void CAimbot::rotate_movement(float yaw, SDK::CUserCmd* cmd)
{
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
	float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

	cmd->forwardmove = new_forwardmove;
	cmd->sidemove = new_sidemove;
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

bool CAimbot::good_backtrack_tick(int tick)
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci) return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + LerpTime(), 0.f, 1.f);
	float delta_time = correct - (INTERFACES::Globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(delta_time) < 0.2f;
}

void CAimbot::run_aimbot(SDK::CUserCmd* cmd)
{
	Entities.clear();
	SelectTarget();
	shoot_enemy(cmd);
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
		if (entity->GetClientClass()->m_ClassID != 40) continue;
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
void CAimbot::shoot_enemy(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_C4 || weapon->is_grenade() || weapon->is_knife()) return;
	if (!can_shoot(cmd)) { cmd->buttons &= ~IN_ATTACK;	return; }
	if (GetAsyncKeyState(VK_LBUTTON)) return;
	Vector aim_angles;
	for (auto players : Entities)
	{
		auto entity = players.pPlayer;
		auto class_id = entity->GetClientClass()->m_ClassID;

		if (!entity) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		if (class_id != 40) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;
		if (entity->GetImmunity()) continue;
		if (entity->GetIsDormant()) continue;

		Vector where2Shoot;

		if (SETTINGS::settings.acc_type == 2) where2Shoot = aimbot->multipoint(entity, SETTINGS::settings.acc_type);
		else where2Shoot = aimbot->point(entity, SETTINGS::settings.acc_type);
		if (where2Shoot == Vector(0, 0, 0)) continue;

		if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_AWP || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SSG08 ||
			weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_G3SG1 ||
			weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_AUG || weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SG553)

		if (SETTINGS::settings.auto_scope)
		{
			if (weapon->is_sniper() && !local_player->GetIsScoped())
			{
				cmd->buttons |= IN_ATTACK2;
			}
		}

		if (SETTINGS::settings.auto_stop)
		{
			if (local_player->GetFlags() & FL_ONGROUND)
			{

				if (weapon->is_knife() || weapon->is_grenade() || weapon->GetItemDefenitionIndex() == SDK::WEAPON_ZEUS) //we dont wanna stop if we holdin a knife, grenade or zeus
					return;

				ClampMovement(cmd, 2);
			}

		}

		aim_angles = MATH::NormalizeAngle(UTILS::CalcAngle(local_player->GetEyePosition(), where2Shoot));
		if (aim_angles == Vector(0, 0, 0)) continue;

		Vector vec_position[65], origin_delta[65];
		if (entity->GetVecOrigin() != vec_position[entity->GetIndex()])
		{
			origin_delta[entity->GetIndex()] = entity->GetVecOrigin() - vec_position[entity->GetIndex()];
			vec_position[entity->GetIndex()] = entity->GetVecOrigin();

			lag_comp = fabs(origin_delta[entity->GetIndex()].Length()) > 64;
		}

		if (lag_comp && entity->GetVelocity().Length2D() > 300 && SETTINGS::settings.delay_shot == 1) return;

		if (accepted_inaccuracy(weapon) < SETTINGS::settings.chance_val) continue;

		if (good_backtrack_tick(TIME_TO_TICKS(entity->GetSimTime() + LerpTime())))
			cmd->tick_count = TIME_TO_TICKS(entity->GetSimTime() + LerpTime());

		cmd->buttons |= IN_ATTACK;
		break;
	}

	if (cmd->buttons & IN_ATTACK)
	{
		float recoil_scale = INTERFACES::cvar->FindVar("weapon_recoil_scale")->GetFloat(); GLOBAL::should_send_packet = true;
		aim_angles -= local_player->GetPunchAngles() * recoil_scale; cmd->viewangles = aim_angles;
	}
}

float CAimbot::accepted_inaccuracy(SDK::CBaseWeapon* weapon)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return 0;

	if (!weapon) return 0;
	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_ZEUS) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}

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

		vecArray[1] += Top * (hitbox->radius * SETTINGS::settings.point_val);
		vecArray[2] += Right * (hitbox->radius * SETTINGS::settings.point_val);
		vecArray[3] += Left * (hitbox->radius * SETTINGS::settings.point_val);
		break;

	default:

		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Right * (hitbox->radius * SETTINGS::settings.body_val);
		vecArray[2] += Left * (hitbox->radius * SETTINGS::settings.body_val);
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
Vector CAimbot::multipoint(SDK::CBaseEntity* entity, int option)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector vector_best_point = Vector(0, 0, 0);
	int maxDamage = SETTINGS::settings.damage_val;

	VMatrix matrix[128];
	if (!entity->SetupBones(matrix, 128, 256, 0)) return Vector(0, 0, 0);

	switch (option)
	{
	case 0:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			for (auto point : GetMultiplePointsForHitbox(local_player, entity, hitboxes[i], matrix))
			{
				int damage = AutoWall->CanHit(point);
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
	} break;
	case 1:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			for (auto point : GetMultiplePointsForHitbox(local_player, entity, hitboxes[i], matrix))
			{
				int damage = AutoWall->CanHit(point);
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
	} break;
	case 2:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD, SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST ,SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			for (auto point : GetMultiplePointsForHitbox(local_player, entity, hitboxes[i], matrix))
			{
				int damage = AutoWall->CanHit(point);
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
	} break;
	}
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
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD, SDK::HitboxList::HITBOX_NECK, SDK::HitboxList::HITBOX_CHEST, SDK::HitboxList::HITBOX_UPPER_CHEST };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{

			int damage = AutoWall->CanHit(entity->GetBonePosition(hitboxes[i]));
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
		int hitboxes[] = { SDK::HitboxList::HITBOX_NECK, SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_THORAX, SDK::HitboxList::HITBOX_LEFT_CALF, SDK::HitboxList::HITBOX_RIGHT_CALF,SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT, SDK::HitboxList::HITBOX_MAX };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			int damage = AutoWall->CanHit(entity->GetBonePosition(hitboxes[i]));
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
		int hitboxes[] = { SDK::HitboxList::HITBOX_HEAD, SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_LEFT_UPPER_ARM, SDK::HitboxList::HITBOX_LEFT_FOREARM, SDK::HitboxList::HITBOX_RIGHT_UPPER_ARM, SDK::HitboxList::HITBOX_RIGHT_FOREARM, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST, SDK::HitboxList::HITBOX_LEFT_CALF, SDK::HitboxList::HITBOX_RIGHT_CALF,SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			int damage = AutoWall->CanHit(entity->GetBonePosition(hitboxes[i]));
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
	case 3:
	{
		int hitboxes[] = { SDK::HitboxList::HITBOX_BODY, SDK::HitboxList::HITBOX_PELVIS, SDK::HitboxList::HITBOX_LEFT_UPPER_ARM, SDK::HitboxList::HITBOX_LEFT_FOREARM, SDK::HitboxList::HITBOX_RIGHT_UPPER_ARM, SDK::HitboxList::HITBOX_RIGHT_FOREARM, SDK::HitboxList::HITBOX_UPPER_CHEST, SDK::HitboxList::HITBOX_CHEST, SDK::HitboxList::HITBOX_LEFT_FOOT, SDK::HitboxList::HITBOX_RIGHT_FOOT };
		for (int i = 0; i < ARRAYSIZE(hitboxes); i++)
		{
			int damage = AutoWall->CanHit(entity->GetBonePosition(hitboxes[i]));
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

bool CAimbot::can_shoot(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;
	if (local_player->GetHealth() <= 0) return false;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;

	return (weapon->GetNextPrimaryAttack() < UTILS::GetCurtime()) && (local_player->GetNextAttack() < UTILS::GetCurtime());
}

void CAimbot::auto_revolver(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player && local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return;

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_REVOLVER) {
		static int ticks = 0;
		ticks++;
		if (ticks <= 15)
			cmd->buttons |= IN_ATTACK;
		else
			ticks = 0;
	}
}

int CAimbot::get_damage(Vector position)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return 0;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

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

int CAimbot::zeus_hitbox(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	float closest = 180.f;

	bestHitbox = -1;

	Vector point = get_hitbox_pos(entity, SDK::HitboxList::HITBOX_PELVIS/*HITBOX_PELVIS*/);

	if (point != Vector(0, 0, 0))
	{
		float distance = fabs((point - local_position).Length());

		if (distance <= closest)
		{
			bestHitbox = SDK::HitboxList::HITBOX_PELVIS;
			closest = distance;
		}
	}

	return bestHitbox;
}

void CAimbot::autozeus(SDK::CUserCmd *cmd) //b1g zeusbot 
{
	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!entity)
			continue;

		if (!local_player)
			continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player)
			continue;

		if (!entity->IsAlive())
			continue;

		if (is_teammate)
			continue;

		if (!local_player->IsAlive())
			continue;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

		if (!weapon)
			continue;

		if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_ZEUS) //if we have a taser men!1!!1 
		{
			if (can_shoot(cmd))
			{
				int bone = zeus_hitbox(entity); //you can change this but keep in mind this has range stuff. it only has pelvis as a bone but why do other stuff really it will make it inaccurate shooting at arms and legs if they arent resolved right 

				if (bone != 1)
				{
					Vector fucknigga = get_hitbox_pos(entity, bone);
					Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

					if (fucknigga != Vector(0, 0, 0))
					{
						SDK::trace_t trace;

						AutoWall->trace_line_esp(local_position, fucknigga, MASK_SOLID, local_player, 0, &trace);

						SDK::player_info_t info;

						if (!(INTERFACES::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
							continue;

						if (fucknigga != Vector(0, 0, 0))
						{
							cmd->viewangles = MATH::NormalizeAngle(UTILS::CalcAngle(local_position, fucknigga));
							GLOBAL::should_send_packet = true;
							cmd->buttons |= IN_ATTACK;
						}
					}
				}
			}
			continue;
		}

	}
}

CAimbot* aimbot = new CAimbot();
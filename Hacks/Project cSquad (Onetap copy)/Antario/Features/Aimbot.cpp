#include "Aimbot.h"
#include "Antiaim.h"
#include "Autowall.h"

#define TIME_TO_TICKS( dt )    ( ( int )( 0.5f + ( float )( dt ) / g_pGlobalVars->intervalPerTick ) )

RageBot* aimbot = new RageBot();

#define M_PI		3.14159265358979323846

float NormalizeYaw(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}

Vector CalcAngle(const Vector & vecSource, const Vector & vecDestination)
{
	Vector qAngles;
	Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}

inline void CrossProduct(const Vector & a, const Vector & b, Vector & result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}


std::vector<Vector> GetMultiplePointsForHitbox(C_BaseEntity* local, C_BaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128])
{
	studiohdr_t* pStudioModel = g_pModelInfo->GetStudiomodel(entity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	Utils::VectorTransform(hitbox->bbmax, BoneMatrix[hitbox->bone], max);
	Utils::VectorTransform(hitbox->bbmin, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = Utils::CalcAngle(center, local->GetEyeOrigin());

	Vector Forward;
	Utils::AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(-Right.x, -Right.y, Right.z);

	Vector Top = Vector(0, 0, 1);
	Vector Bot = Vector(0, 0, -1);

	switch (iHitbox)
	{
	case 0:
		if (g_Settings.bMultiPointSelection[0])
		{
			for (auto i = 0; i < 8; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray.at(1) += Top * (hitbox->radius * g_Settings.iHeadScale / 100.f);
			vecArray.at(2) += Right * (hitbox->radius * g_Settings.iHeadScale / 100.f);
			vecArray.at(3) += Left * (hitbox->radius * g_Settings.iHeadScale / 100.f);
		}
		else
		{
			for (auto i = 0; i < 8; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 1:
		if (g_Settings.bMultiPointSelection[1])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Right * (hitbox->radius * g_Settings.iNeckScale / 100.f);
			vecArray[2] += Left * (hitbox->radius * g_Settings.iNeckScale / 100.f);
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 2:
		if (g_Settings.bMultiPointSelection[2])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iPelvisScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iPelvisScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 3:
		if (g_Settings.bMultiPointSelection[4])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iStomachScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iStomachScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 4:
		if (g_Settings.bMultiPointSelection[3])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iChestScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iChestScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 5:
		if (g_Settings.bMultiPointSelection[3])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iChestScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iChestScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 6:
		if (g_Settings.bMultiPointSelection[3])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iChestScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iChestScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 7:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 8:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 9:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 10:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 11:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	case 12:
		if (g_Settings.bMultiPointSelection[6])
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += Left * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
			vecArray[2] += Right * (hitbox->radius * (g_Settings.iLegsScale / 100.f));
		}
		else
		{
			for (auto i = 0; i < 3; ++i)
			{
				vecArray.emplace_back(center);
			}
		}
		break;
	}
	return vecArray;
}

std::vector<int> get_priority(C_BaseEntity * ent) {
	std::vector<int> hitbox_array;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon)
		return hitbox_array;

	if (GetAsyncKeyState(g_Settings.iBodyKey)
		|| weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_TASER)
	{
		hitbox_array.push_back((int)HitboxList::Pelvis);
		hitbox_array.push_back((int)HitboxList::Stomach);
		hitbox_array.push_back((int)HitboxList::Chest);
		hitbox_array.push_back((int)HitboxList::LowerChest);
		hitbox_array.push_back((int)HitboxList::UpperChest);
		return hitbox_array;
	}
	else
	{
		if (g_Settings.bHitboxSelection[4])
		{
			hitbox_array.push_back((int)HitboxList::Pelvis);
		}

		if (g_Settings.bHitboxSelection[3])
		{
			hitbox_array.push_back((int)HitboxList::Stomach);
		}

		if (g_Settings.bHitboxSelection[2])
		{
			hitbox_array.push_back((int)HitboxList::Chest);
			hitbox_array.push_back((int)HitboxList::LowerChest);
			hitbox_array.push_back((int)HitboxList::UpperChest);
		}

		if (g_Settings.bHitboxSelection[0])
		{
			hitbox_array.push_back((int)HitboxList::Head);
		}


		if (g_Settings.bHitboxSelection[1])
		{
			hitbox_array.push_back((int)HitboxList::Neck);
		}

		if (g_Settings.bHitboxSelection[5])
		{
			hitbox_array.push_back((int)HitboxList::LeftUpperArm);
			hitbox_array.push_back((int)HitboxList::RightUpperArm);
			hitbox_array.push_back((int)HitboxList::LeftLowerArm);
			hitbox_array.push_back((int)HitboxList::RightLowerArm);
		}
		if (g_Settings.bHitboxSelection[6])
		{
			hitbox_array.push_back((int)HitboxList::LeftFoot);
			hitbox_array.push_back((int)HitboxList::RightFoot);
		}
		return hitbox_array;
	}
	
}

float accepted_inaccuracy(C_BaseCombatWeapon * weapon) //ayyyyyywareeee
{
	auto local_player = g::pLocalEntity;

	if (!local_player)
		return 0;

	if (!weapon)
		return 0;

	float hitchance = 101; //lol idk why, its pasted anyway so w/e
	float inaccuracy = weapon->GetInaccuracy();

	if (inaccuracy == 0)
		inaccuracy = 0.0000001;

	inaccuracy = 1 / inaccuracy;
	hitchance = inaccuracy;

	return hitchance;
}

float LerpTime() 
{
	int updaterate = g_pConVar->FindVar("cl_updaterate")->GetInt();
	ConVar* minupdate = g_pConVar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = g_pConVar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = g_pConVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_pConVar->FindVar("cl_interp")->GetFloat();
	ConVar * cmin = g_pConVar->FindVar("sv_client_min_interp_ratio");
	ConVar * cmax = g_pConVar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = std::clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, (ratio / updaterate));
}

float ClampYaw(float y) {
	if (y > 180)
	{
		y -= (round(y / 360) * 360.f);
	}
	else if (y < -180)
	{
		y += (round(y / 360) * -360.f);
	}
	return y;
}

void RageBot::PVSFix(ClientFrameStage_t stage) {
	if (stage == FRAME_RENDER_START) {
		for (int o = 0; o < 64; ++o) {
			auto pPlayerEntity = (C_BaseEntity*)g_pEntityList->GetClientEntity(o);
			if (pPlayerEntity == nullptr || !g::pLocalEntity || !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant())
				continue;

			*(int*)((uintptr_t)pPlayerEntity + 0xA30) = g_pGlobalVars->framecount; //we'll skip occlusion checks now
			*(int*)((uintptr_t)pPlayerEntity + 0xA28) = 0;//clear occlusion flags
		}
	}
}

void RageBot::minspeed(C_BaseEntity* local, CUserCmd* cmd)
{
	C_BaseCombatWeapon* weapon = local->GetActiveWeapon();
	if (!weapon)
		return;

	Vector velocity = local->GetVelocity();
	Vector direction;

	Utils::VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.y = cmd->viewangles.y - direction.y;

	Vector forward;

	Utils::AngleVectors(direction, &forward);

	Vector source = forward * -speed;

	float maxspeed = weapon->GetCSWpnData()->max_speed;
	float maxspeedalt = weapon->GetCSWpnData()->max_speed_alt;

	if (maxspeed == maxspeedalt)
	{
		if (speed >= (maxspeed * .34))
		{
			cmd->forwardmove = source.x;
			cmd->sidemove = source.y;
		}
	}
	else
	{
		if (speed >= (maxspeedalt * .34))
		{
			cmd->forwardmove = source.x;
			cmd->sidemove = source.y;
		}
	}
}

void RageBot::DisableInterpolation(ClientFrameStage_t stage) {

	for (int o = 0; o < 64; ++o) {
		auto pPlayerEntity = (C_BaseEntity*)g_pEntityList->GetClientEntity(o);
		if (pPlayerEntity == nullptr || !g::pLocalEntity || !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant() || pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;
		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != g::pLocalEntity)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;

			static bool fl = false;
			if (!fl)
			{
				Utils::Log("interpolated");
				fl = true;
			}
		}
	}
}

void Autostop()
{
	if (!g_Settings.bAutoStop)
		return;

	Vector Velocity = g::pLocalEntity->GetVelocity();
	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	Utils::VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	Utils::AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;
}

VOID sinCos(float radians, PFLOAT sine, PFLOAT cosine)
{
	__asm
	{
		fld dword ptr[radians]
		fsincos
		mov edx, dword ptr[cosine]
		mov eax, dword ptr[sine]
		fstp dword ptr[edx]
		fstp dword ptr[eax]
	}
}


bool HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0)
		return true;

	auto RandomFloat = [](float min, float max) -> float
	{
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat");
		return m_RandomFloat(min, max);
	};

	float Seeds = 256.f;

	Angle -= (g::pLocalEntity->GetAimPunchAngle() * g_pConVar->FindVar("weapon_recoil_scale")->GetFloat());

	Vector forward, right, up;

	Utils::AngleVectorsbeta(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpreadCone(), weapInaccuracy = pWeapon->GetInaccuracy();
	auto VectorAngles = [](const Vector & forward, const Vector & pseudoup, Vector & angles)
	{
		auto CrossProducts = [](const Vector & a, const Vector & b)
		{
			return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
		};
		Vector left = CrossProducts(pseudoup, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(forward.y, forward.x) * 180 / 3.14159265358979323846f;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / 3.14159265358979323846f;
		}
		else
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(-left.x, left.y) * 180 / 3.14159265358979323846f;
			angles.z = 0;
		}
	};
	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		VectorAngles(direction, up, viewanglesSpread);
		Utils::NormalizeAngle(viewanglesSpread);

		Utils::AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

		trace_t Trace;
		Ray_t ray;
		ray.Init(g::pLocalEntity->GetEyePosition(), viewForward);
		g_pTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}


bool accepted_inaccuracy(C_BaseCombatWeapon* weapon, C_BaseEntity* entity, Vector position)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return false;

	if (!weapon)
		return false;

	//	if (weapon->sGetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_TASER)
	//		return true;

	if (g_Settings.iHitChance <= 0)
		return true;
	auto RandomFloat = [](float min, float max) -> float
	{
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat");
		return m_RandomFloat(min, max);
	};
	/*

	Vector angle = Utils::NormalizeAngle(Utils::CalcAngle(local_player->GetEyeOrigin(), position));

	Vector forward, right, up;
	Utils::AngleVectorsbeta(angle, &forward, &right, &up);

	int iHit = 0;
	weapon->UpdateAccuracyPenalty();
	for (int i = 0; i < 256; i++)
	{
		float RandomA = RandomFloat(0.0f, 1.0f);
		float RandomB = 1.0f - RandomA * RandomA;

		RandomA = RandomFloat(0.0f, M_PI_F * 2.0f);
		RandomB *= weapon->GetSpreadCone();

		float SpreadX1 = (cos(RandomA) * RandomB);
		float SpreadY1 = (sin(RandomA) * RandomB);

		float RandomC = RandomFloat(0.0f, 1.0f);
		float RandomF = 1.0f - RandomC * RandomC;

		RandomC = RandomFloat(0.0f, M_PI_F * 2.0f);
		RandomF *= weapon->GetInaccuracy();

		float SpreadX2 = (cos(RandomC) * RandomF);
		float SpreadY2 = (sin(RandomC) * RandomF);

		float fSpreadX = SpreadX1 + SpreadX2;
		float fSpreadY = SpreadY1 + SpreadY2;

		Vector vSpreadForward;
		vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
		vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
		vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
		vSpreadForward.NormalizeInPlace();

		Vector qaNewAngle;
		Utils::VectorAngles(vSpreadForward, qaNewAngle);
		qaNewAngle = Utils::NormalizeAngle(qaNewAngle);

		Vector vEnd;
		Utils::AngleVectors(qaNewAngle, &vEnd);
		vEnd = local_player->GetEyeOrigin() + (vEnd * 8192.f);

		trace_t trace;
		CTraceFilterOneEntity2 filter;
		filter.pEntity = entity;
		Ray_t ray;
		ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), vEnd);

		g_pTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
		if (trace.m_pEnt == entity)
			iHit++;

		if (iHit / 256.f >= g_Settings.iHitChance / 100.f)
			return true;
	}
	return false;*/
	auto AngleVectorr = [](const Vector & angles, Vector & forward, Vector & right, Vector & up)
	{
		float sr, sp, sy, cr, cp, cy;

		sinCos(DEG2RAD(angles[1]), &sy, &cy);
		sinCos(DEG2RAD(angles[0]), &sp, &cp);
		sinCos(DEG2RAD(angles[2]), &sr, &cr);

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = (cr * cp);
	};
	auto VectorAngles = [](const Vector & forward, const Vector & pseudoup, Vector & angles)
	{
		auto CrossProducts = [](const Vector & a, const Vector & b)
		{
			return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
		};
		Vector left = CrossProducts(pseudoup, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(forward.y, forward.x) * 180 / 3.14159265358979323846f;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / 3.14159265358979323846f;
		}
		else
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(-left.x, left.y) * 180 / 3.14159265358979323846f;
			angles.z = 0;
		}
	};
	auto AngleVectors = [](Vector q, Vector & vec)
	{
		float sp, sy, cp, cy;

		sinCos(DEG2RAD(q[1]), &sy, &cy);
		sinCos(DEG2RAD(q[0]), &sp, &cp);

		vec.x = cp * cy;
		vec.y = cp * sy;
		vec.z = -sp;
	};

	//auto weapon = g::pLocalEntity->GetActiveWeapon();

	Vector angles = Utils::FastCalcAngle(g::pLocalEntity->GetEyePosition(), position);

	Vector forward, right, up;
	Vector src = g::pLocalEntity->GetEyePosition();
	AngleVectorr(angles, forward, right, up);
	int seeds = 256;
	int cHits = 0;

	weapon->UpdateAccuracyPenalty();
	float weap_spread = weapon->GetSpreadCone();
	float weap_inaccuracy = weapon->GetInaccuracy();
	int cNeededHits = weapon->GetItemDefinitionIndex() == WEAPON_TASER ? seeds * 0.75f : static_cast<int>(seeds * (g_Settings.iHitChance / 100.f));
	float min_zeus_damage = 175.f; // 100% kill distance

	for (int i = 0; i < seeds; i++)
	{
		float a = RandomFloat(0.f, 1.0f);
		float b = RandomFloat(0.f, 2.f * M_PI);
		float c = RandomFloat(0.f, 1.0f);
		float d = RandomFloat(0.f, 2.f * M_PI);

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if (weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalize();

		Vector viewAnglesSpread;
		VectorAngles(direction, up, viewAnglesSpread);
		viewAnglesSpread.Normalize();

		Vector viewForward;
		AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();
		viewForward = src + (viewForward * (weapon->GetCSWpnData()->range - 30));

		trace_t tr;
		Ray_t ray;

		ray.Init(src, viewForward);

		g_pTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, entity, &tr);

		auto ent = tr.m_pEnt;
		if (ent)
		{
			if (ent->GetClientClass()->m_ClassID == 40 && ent->IsAlive() && !ent->IsDormant() && ent != g::pLocalEntity && ent->GetTeam() != g::pLocalEntity->GetTeam())
				++cHits;

		}

		if (static_cast<int>((static_cast<float>(cHits) / seeds) * 100.f) >= g_Settings.iHitChance)
			return true;

		if ((seeds - i + cHits) < cNeededHits)
			return false;

	}
	return false;
}


bool  IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity * weaponEnt = (C_BaseEntity*)weapon;
	ClientClass * pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)EClassIds::CKnife || pWeaponClass->m_ClassID == (int)EClassIds::CHEGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CDecoyGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CIncendiaryGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CSmokeGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CC4 ||
		pWeaponClass->m_ClassID == (int)EClassIds::CMolotovGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CFlashbang)
		return false;
	else
		return true;
}

bool IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity * weaponEnt = (C_BaseEntity*)weapon;
	ClientClass * pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)EClassIds::CDEagle || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponElite || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponFiveSeven || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponGlock || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponHKP2000 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponP250 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponP228 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponTec9 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponUSP)
		return true;
	else
		return false;
}

bool IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity * weaponEnt = (C_BaseEntity*)weapon;
	ClientClass * pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)EClassIds::CWeaponAWP || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponSSG08 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponSCAR20 || pWeaponClass->m_ClassID == (int)EClassIds::CWeaponG3SG1)
		return true;
	else
		return false;
}


bool IsWeaponGrenade2(C_BaseCombatWeapon* weapon)
{
	if (weapon == nullptr) return false;
	int id = weapon->sGetItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool RageBot::CanShoot()
{
	if (!g_pGlobalVars)
		return false;

	if (!g::pLocalEntity || !g::pLocalEntity->GetActiveWeapon())
		return false;

	auto flServerTime = (float)g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	auto flNextPrimaryAttack = g::pLocalEntity->GetActiveWeapon()->GetNextPrimaryAttack();

	return(!(flNextPrimaryAttack > flServerTime));
}

float GetCurtime(CUserCmd* ucmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_pGlobalVars->intervalPerTick;
	return curtime;
}

void AutoRevolver111(CUserCmd * cmd, C_BaseEntity * local_player)
{
	if (!g_Settings.bAutoRevolver)
		return;

	C_BaseCombatWeapon* weapon = local_player->GetActiveWeapon();
	if (!weapon || weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
		return;

	cmd->buttons |= IN_ATTACK;
	float flPostponeFireReady = weapon->GetPostponeFireReadyTime();
	if (flPostponeFireReady > 0 && flPostponeFireReady < GetCurtime(cmd))
	{
		cmd->buttons &= ~IN_ATTACK;
		if (flPostponeFireReady + g_pGlobalVars->intervalPerTick * 5 > GetCurtime(cmd))
			cmd->buttons |= IN_ATTACK2;
	}
}

bool IsKnife31(C_BaseCombatWeapon * pWeapon)
{
	if (pWeapon == nullptr) return false;
	int id = pWeapon->sGetItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_KNIFE_BAYONET, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GUT, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_TACTICAL , WEAPON_KNIFE, WEAPON_KNIFE_T, WEAPON_KNIFEGG };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool RageBot::CanShootAntiaim(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0 || local_player->GetNextAttack() > GetCurtime(cmd))
		return false;

	C_BaseCombatWeapon * weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon)
		return false;

	if (IsWeaponGrenade2(weapon))
	{
		if (weapon->IsPinPulled())
			return false;

		if (weapon->GetThrowTime() <= 0 || weapon->GetThrowTime() > GetCurtime(cmd))
			return false;

		return true;
	}

	if (weapon->GetNextPrimaryAttack() > GetCurtime(cmd))
		return false;

	/// revolver
	if (weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER && weapon->GetPostponeFireReadyTime() > GetCurtime(cmd))
		return false;


	return true;
}


mstudiobbox_t* RageBot::GetHitbox(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return nullptr;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return nullptr;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return nullptr;

	auto pSet = pStudioHdr->pHitboxSet(0);
	if (!pSet)
		return nullptr;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return nullptr;

	return pSet->GetHitbox(hitbox_index);
}

Vector RageBot::GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
{
	auto hitbox = GetHitbox(pEntity, Hitbox);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = pEntity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax, vCenter;
	Utils::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
	Utils::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

	vCenter = (bbmin + bbmax) * 0.5f;

	return vCenter;
}

void RageBot::OnCreateMove() {
	static bool shot = false;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!g::pLocalEntity || !weapon || weapon->GetCSWpnData() == nullptr
		|| weapon->GetCSWpnData()->type == NULL) {
		shot = false;
		return;
	}

	if (IsKnife31(weapon) || IsWeaponGrenade2(weapon) || weapon->GetAmmo() == 0 || !IsBallisticWeapon(weapon) || !CanShoot())
	{
		shot = false;
		return;
	}

	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (weapon->GetNextPrimaryAttack() <= flServerTime);

	if (!g_Settings.bAimBotEnabled) {
		shot = false;
		return;
	}

	AutoRevolver111(g::pCmd, g::pLocalEntity);

	std::vector<C_BaseEntity*> targets;

	bool target_found = false;

	auto target_sort = [&](C_BaseEntity * a, C_BaseEntity * b) {
		return g::pLocalEntity->GetOrigin().DistTo(a->GetOrigin())
			< g::pLocalEntity->GetOrigin().DistTo(b->GetOrigin());
	};

	for (int o = 0; o < 64; ++o) {
		auto entity = (C_BaseEntity*)g_pEntityList->GetClientEntity(o);
		if (entity == nullptr || !g::pLocalEntity || !entity->IsAlive()
			|| entity->IsDormant() || entity == g::pLocalEntity || entity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		targets.push_back(entity);
	}

	std::sort(targets.begin(), targets.end(), target_sort);

	for (auto target : targets) {
		if (!target)
			continue;
		if (target_found)
			break;

		for (auto hitbox : get_priority(target))
		{
			if (target_found)
				break;

			if (!target->/*Proper*/SetupBones(bonematrix[target->EntIndex()], 128, 0x100, g_pGlobalVars->curtime))
				continue;

			for (auto aim_point : GetMultiplePointsForHitbox(g::pLocalEntity, target, hitbox, bonematrix[target->EntIndex()]))
			{
				int cur_hitchance = g_Settings.iHitChance;
				int cur_mindmg = g_Settings.iMinDamage;
				float damage = 0.f;

				float BestDmg = -1;
				Vector BestPos = Vector();

				if (autowall->CanHit2(aim_point, &damage))
				{
					if (BestDmg < damage && damage >= cur_mindmg)
					{
						BestDmg = damage;
						target_found = true;
						BestPos = aim_point;
					}
				}

				auto Angle = Utils::FastCalcAngle(g::pLocalEntity->GetEyePosition(), BestPos);
				if (BestDmg != -1 && BestPos != Vector()) {

					if (g_Settings.bAutoScope && IsSniper(weapon) && !g::pLocalEntity->IsScoped())
						g::pCmd->buttons |= IN_ATTACK2;

					if (!(g::pCmd->buttons & IN_ATTACK) && canShoot &&
						HitChance(target, g::pLocalEntity->GetActiveWeapon(), Angle, BestPos, cur_hitchance)) {
						shot = true;
						//Globals::bsendpacket = true;
						g::pCmd->buttons |= IN_ATTACK;
						Globals::shots_fired++;
						g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pConVar->FindVar("weapon_recoil_scale")->GetFloat());
						g::pCmd->tick_count = TIME_TO_TICKS(target->m_flSimulationTime() + LerpTime());

						Utils::Log("fire");
					}

					if (canShoot) {
						if (!HitChance(target, g::pLocalEntity->GetActiveWeapon(), Angle, BestPos, cur_hitchance))
						{
							g::pCmd->forwardmove = 0;
							g::pCmd->sidemove = 0;
							g::pCmd->upmove = 0;
						}
					}
				}
				if (target_found)
					break;
			}
			if (target_found)
				break;
		}
		if (target_found)
			break;
	}
	targets.clear();
	target_found = false;
}
#include "DesyncCorrect.h"
#include "UTILS/interfaces.h"
#include "SDK/IEngine.h"
#include "SDK/CUserCmd.h"
#include "SDK/CGlobalVars.h"
#include "SDK/CBaseEntity.h"
#include "SDK/CBaseAnimState.h"
#include "FEATURES\AutoWall.h"
#include "FEATURES\Resolver.h"
float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}


float approach(float cur, float target, float inc) {
	inc = abs(inc);

	if (cur < target)
		return min(cur + inc, target);
	if (cur > target)
		return max(cur - inc, target);

	return target;
}

float angle_difference(float a, float b) {
	auto diff = desync->NormalizeYaw180(a - b);

	if (diff < 180)
		return diff;
	return diff - 360;
}

float approach_angle(float cur, float target, float inc) {
	auto diff = angle_difference(target, cur);
	return approach(cur, cur + diff, inc);
}

void DesyncCorrect::HandleHits(SDK::CBaseEntity* pEnt)
{
	auto NetChannel = INTERFACES::Engine->GetNetChannelInfo();

	if (!NetChannel)
		return;
	
	static float predTime[65];
	static bool init[65];
	const int shots_missed = shots_fired[pEnt->GetIndex()] - shots_hit[pEnt->GetIndex()]; // getting missed shots


	if (Shot[pEnt->GetIndex()])
	{
		if (init[pEnt->GetIndex()])
		{
			pitchHit[pEnt->GetIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->GetIndex()] = INTERFACES::Globals->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(INTERFACES::Engine->GetNetChannel()->m_nChokedPackets);
			init[pEnt->GetIndex()] = false;
		}

		if (INTERFACES::Globals->curtime > predTime[pEnt->GetIndex()] && !Hit[pEnt->GetIndex()])
		{
			MissedShots[pEnt->GetIndex()] += 1;
			Shot[pEnt->GetIndex()] = false;
		}
		else if (INTERFACES::Globals->curtime <= predTime[pEnt->GetIndex()] && Hit[pEnt->GetIndex()])
			Shot[pEnt->GetIndex()] = false;

	}
	else
		init[pEnt->GetIndex()] = true;

	Hit[pEnt->GetIndex()] = false;
}

float DesyncCorrect::desync_delta(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto animstate = uintptr_t(entity->GetAnimState());
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}

	unk3 = *(float *)(animstate + 0x334) * unk2;

	return unk3;

}

float DesyncCorrect::NormalizeYaw180(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}

float AngleDiff(float a, float b)
{
	return a - b;
}
void DesyncCorrect::DesyncDetection()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	int actual_yaw = desync->actual_yaw, opposite_yaw = desync->opposite_yaw;

	CAutowall* autowall = nullptr;
	

	if (!autowall) return;

	std::vector<Vector> last_eye_positions;


	last_eye_positions.insert(last_eye_positions.begin(), local_player->GetVecOrigin() + local_player->GetViewOffset());
	if (last_eye_positions.size() > 128)
		last_eye_positions.pop_back();

	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return;



	const int latency_ticks = TIME_TO_TICKS(nci->GetLatency(FLOW_OUTGOING));
	const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];
	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant())
			continue;

	

		actual_yaw = entity->EasyEyeAngles()->yaw;
		opposite_yaw = -actual_yaw;

		



		const float height = 64;

		Vector direction_1, direction_2;
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + actual_yaw, 0.f), &direction_1);
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - opposite_yaw, 0.f), &direction_2); // + and - = -

		const auto real_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
		const auto opposite_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) - (direction_2 * 16.f);

		actual_yaw = autowall->GetDamagez(real_eye_pos);
		opposite_yaw = autowall->GetDamagez(opposite_eye_pos);

		

	
		
	}
}
int sequence_activity(SDK::CBaseEntity* pEntity, int sequence)
{
	const SDK::model_t* pModel = pEntity->GetModel();
	if (!pModel)
		return 0;

	auto hdr = INTERFACES::ModelInfo->GetStudioModel(pEntity->GetModel());

	if (!hdr)
		return -1;

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, SDK::studiohdr_t*, int)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity(pEntity, hdr, sequence);
}


float DesyncCorrect::Yoinks(SDK::CBaseEntity* pEnt, float yaw)
{
	const int shots_missed = shots_fired[pEnt->GetIndex()] - shots_hit[pEnt->GetIndex()]; // getting missed shots

	

	auto* AnimState = pEnt->GetAnimState();
	float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * AnimState->m_flEyeYaw);
	float Delta = angle_difference(pEnt->EasyEyeAngles()->yaw, AnimState->m_flGoalFeetYaw);
	//float flMaxYawModifier = flYawModifier * maxYaw;
	



	if (Delta < maxYaw)
	{
		desync->CanJitter[pEnt->GetIndex()] = true;
	}
	else
	{
		desync->CanJitter[pEnt->GetIndex()] = false;	
	}

	if (desync->CanJitter[pEnt->GetIndex()])
	{
		
		if (desync->actual_yaw > desync->opposite_yaw)
		{
			switch (shots_missed % 2)
			{
			case 0:
				yaw = pEnt->EasyEyeAngles()->yaw; break;
			case 1: 
				yaw = -pEnt->EasyEyeAngles()->yaw; break;

			} 
			
		}
		else if (desync->opposite_yaw > desync->actual_yaw)
		{
			switch (shots_missed % 2)
			{
			case 0:
				yaw = -pEnt->EasyEyeAngles()->yaw; break;
			case 1:
				yaw = pEnt->EasyEyeAngles()->yaw; break;

			}
		}
		else
		{
			AnimState->m_flGoalFeetYaw = approach_angle(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw, AnimState->m_flLastClientSideAnimationUpdateTime * 100.0f);

		}
		
	}
	
	return yaw;
}

bool DesyncCorrect::lby_prediction(SDK::CBaseEntity* entity)
{
	

	if (!entity)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	local_update = next_lby_update_time;

	auto animstate = entity->GetAnimState();

	if (!animstate)
		return false;

	if (!(entity->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f && !fake_walk)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}






float ServerCalculations(SDK::CBaseEntity* entity)
{
	const int shots_missed = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()]; // getting missed shots
	auto* AnimState = entity->GetAnimState();
	float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * AnimState->m_flEyeYaw);
	float Delta = angle_difference(entity->EasyEyeAngles()->yaw, AnimState->m_flGoalFeetYaw);
	auto index = entity->GetIndex();
	static Vector oldAngles[65];
	auto velocity = entity->GetVelocity().Length2D();

	enum whatever
	{
		StandingYaw,
		MovingYaw,
		JumpingYaw
	};

	static float oldGoalfeetYaw[65];

	Vector angles = entity->GetEyeAngles();

	
	

	if (velocity < 0.5)
	{
		float vel = AnimState->m_flUnknownVelocityLean;
		float delta = AnimState->m_flLastClientSideAnimationUpdateTime * 60.0f;
		float newvel;
		if ((80.0f - vel) <= delta)
		{
			if (-delta <= (80.0f - vel))
				newvel = 80.0f;
			else
				newvel = vel - delta;
		}
		else
		{
			newvel = vel + delta;
		}
		AnimState->m_flUnknownVelocityLean = newvel;
		AnimState->m_flGoalFeetYaw -= newvel;
	}
	else if (velocity > 0.5)
	{
		AnimState->m_flGoalFeetYaw = approach_angle(entity->GetLowerBodyYaw(), AnimState->m_flGoalFeetYaw, AnimState->m_flLastClientSideAnimationUpdateTime * 100.0f);
	}
	else
	{
		float vel = AnimState->m_flUnknownVelocityLean;
		float delta = AnimState->m_flLastClientSideAnimationUpdateTime * 60.0f;
		float newvel;
		if ((80.0f - vel) <= delta)
		{
			if (-delta <= (80.0f - vel))
				newvel = 80.0f;
			else
				newvel = vel - delta;
		}
		else
		{
			newvel = vel + delta;
		}
		AnimState->m_flUnknownVelocityLean = newvel;

		AnimState->m_flGoalFeetYaw += newvel;
	}

	return AnimState->m_flGoalFeetYaw;
}

SDK::CBaseWeapon* SDK::CBaseEntity::GetWeapon2()
{
	static int iOffset = UTILS::netvar_hook_manager.GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (SDK::CBaseWeapon*)(INTERFACES::ClientEntityList->GetClientEntityFromHandle((HANDLE)pWeepEhandle));
}

bool PredictedNextLbyUpdate(SDK::CBaseEntity* entity)
{


	if (!entity)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	float animTime = entity->GetSimTime() + INTERFACES::Globals->interval_per_tick;
	local_update = next_lby_update_time;

	auto animstate = entity->GetAnimState();

	if (!animstate)
		return false;

	if (!(entity->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f && !fake_walk)
		next_lby_update_time = animTime + 0.22f;

	if (next_lby_update_time < animTime)
	{
		next_lby_update_time = animTime + 1.1f;
		return true;
	}

	return false;

}

void DesyncCorrect::AnimationFix(SDK::CBaseEntity* pEnt)
{
	//who needs structs or classes not me lol
	static float oldSimtime[65];
	static float storedSimtime[65];
	static float ShotTime[65];
	static float SideTime[65][3];
	static int LastDesyncSide[65];
	static bool Delaying[65];
	static SDK::CAnimationLayer StoredLayers[64][15];
	static SDK::CBaseAnimState* StoredAnimState[65];
	static float StoredPosParams[65][24];
	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	static bool IsYoinks[65];

	if (pEnt == nullptr) return;

	const int shots_missed = shots_fired[pEnt->GetIndex()] - shots_hit[pEnt->GetIndex()]; // getting missed shots


	float* PosParams = (float*)((uintptr_t)pEnt + 0x2774);
	bool update = false;
	bool shot = false;
	auto Local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	float at_target_yaw = UTILS::CalcAngle(pEnt->GetVecOrigin(), Local->GetVecOrigin()).y; // self explanatory


	static bool jittering[65];

	auto* AnimState = pEnt->GetAnimState();
	if (!AnimState || !pEnt->GetAnimOverlays() || !PosParams)
		return;

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};


	auto is_jitter = [&pEnt]() -> bool
	{
		for (size_t i = 0; i < pEnt->GetNumAnimOverlays(); i++)
		{
			auto layer = pEnt->GetAnimOverlays()[i];

			if (sequence_activity(pEnt, layer.m_nSequence) == 979)
			{
				if (layer.m_flPrevCycle > 0.92f && layer.m_flCycle > 0.92f && layer.m_flWeight == 0.f)
				{
					return true;
				}
			}
		}
		return false;
	};

	auto desync_boi = [&pEnt]() -> bool
	{
		for (int s = 0; s < 14; s++)
		{


			auto anim_layer = pEnt->GetAnimOverlay(s);
			if (!anim_layer.m_pOwner)
				continue;
			auto anime = &pEnt->GetAnimOverlays()[1];

			if (anim_layer.m_flWeight == 0.f && anim_layer.m_flCycle == 0.f)
			{
				return true;
			}
			else if (anim_layer.m_flWeight == 1.f && anim_layer.m_flCycle != anim_layer.m_flPrevCycle)
			{
				return true;
			}
			else if (anim_layer.m_flWeight >= 0.041f && anim_layer.m_flWeight < 0.042f && anim_layer.m_flCycle >= 0.012f && anim_layer.m_flCycle < 0.013f)
			{
				return true;
			}
			else if (anim_layer.m_flWeight >= 0.67f && anim_layer.m_flWeight < 0.69f && anim_layer.m_flCycle >= 0.061f && anim_layer.m_flCycle < 0.062f)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	IsYoinks[pEnt->GetIndex()] = false;

	if (storedSimtime[pEnt->GetIndex()] != pEnt->GetSimTime())
	{
		jittering[pEnt->GetIndex()] = false;
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientSideAnimation();

		memcpy(StoredPosParams[pEnt->GetIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[pEnt->GetIndex()], pEnt->GetAnimOverlays(), (sizeof(SDK::CAnimationLayer) * 15));

		oldGoalfeetYaw[pEnt->GetIndex()] = AnimState->m_flGoalFeetYaw;

		if (pEnt->GetWeapon2() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->GetIndex()] != pEnt->GetWeapon2()->GetLastShotTime())
			{
				shot = true;
				ShotTime[pEnt->GetIndex()] = pEnt->GetWeapon2()->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[pEnt->GetIndex()] = 0.f;
		}

		float angToLocal = MATH::NormalizeYaw(UTILS::CalcAngle(Local->GetVecOrigin(), pEnt->GetVecOrigin()).y);

		float Back = MATH::NormalizeYaw(angToLocal);
		float DesyncFix = 0;
		float Resim = MATH::NormalizeYaw((0.24f / (pEnt->GetSimTime() - oldSimtime[pEnt->GetIndex()]))*(oldEyeAngles[pEnt->GetIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 58.f)
			Resim = 58.f;
		if (Resim < -58.f)
			Resim = -58.f;

		if (pEnt->GetVelocity().Length2D() > 0.5f && !shot)
		{
			float Delta = MATH::NormalizeYaw(MATH::NormalizeYaw(UTILS::CalcAngle(Vector(0, 0, 0), pEnt->GetVelocity()).y) - MATH::NormalizeYaw(MATH::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim));

			int CurrentSide = 0;

			if (Delta < 0)
			{
				CurrentSide = 1;
				SideTime[pEnt->GetIndex()][1] = INTERFACES::Globals->curtime;
			}
			else if (Delta > 0)
			{
				CurrentSide = 2;
				SideTime[pEnt->GetIndex()][2] = INTERFACES::Globals->curtime;
			}

			if (LastDesyncSide[pEnt->GetIndex()] == 1)
			{
				Resim += (58.f - Resim);
				DesyncFix += (58.f - Resim);
			}
			if (LastDesyncSide[pEnt->GetIndex()] == 2)
			{
				Resim += (-58.f - Resim);
				DesyncFix += (-58.f - Resim);
			}

			if (LastDesyncSide[pEnt->GetIndex()] != CurrentSide)
			{
				Delaying[pEnt->GetIndex()] = true;

				if (.5f < (INTERFACES::Globals->curtime - SideTime[pEnt->GetIndex()][LastDesyncSide[pEnt->GetIndex()]]))
				{
					LastDesyncSide[pEnt->GetIndex()] = CurrentSide;
					Delaying[pEnt->GetIndex()] = false;
				}
			}

			if (!Delaying[pEnt->GetIndex()])
				LastDesyncSide[pEnt->GetIndex()] = CurrentSide;

			//resolvedYaw[pEnt->GetIndex()] = desync->NormalizeYaw180(MATH::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim);
		}
		else if (!shot)
		{

			float Brute = UseFreestandAngle[pEnt->GetIndex()] ? NormalizeYaw180(Back + FreestandAngle[pEnt->GetIndex()]) : pEnt->GetLowerBodyYaw();

			float Delta = NormalizeYaw180(NormalizeYaw180(Brute - NormalizeYaw180(NormalizeYaw180(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60))) + Resim));

			if (Delta > 58.f)
				Delta = 58.f;
			if (Delta < -58.f)
				Delta = -58.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 58.f)
				Resim = 58.f;
			if (Resim < -58.f)
				Resim = -58.f;

		}

		float Equalized = MATH::NormalizeYaw(MATH::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim);


		float JitterDelta = fabs(MATH::NormalizeYaw(oldEyeAngles[pEnt->GetIndex()].y - pEnt->GetEyeAngles().y));

		if (JitterDelta >= 70.f && !shot) // no
			jittering[pEnt->GetIndex()] = true;

		if (pEnt != Local && pEnt->GetTeam() != Local->GetTeam() && SETTINGS::settings.desync_resolver_bool)// we arent overriding
		{

			if (jittering[pEnt->GetIndex()]) // if is jittering by stickrpg and [censored] check, but "Yoinks" isnt active, set the eye to "DesyncFix"
			{
				if (MissedShots[pEnt->GetIndex()] == 0)
				{
					AnimState->m_flGoalFeetYaw = desync->NormalizeYaw180(pEnt->GetEyeAngles().y + DesyncFix);
				}
				else if (MissedShots[pEnt->GetIndex()] == 1) // me missed 2 or more shots
				{
					AnimState->m_flGoalFeetYaw = desync->NormalizeYaw180(pEnt->GetEyeAngles().y - DesyncFix);
				}
				else if (MissedShots[pEnt->GetIndex()] > 1)
				{
					AnimState->m_flGoalFeetYaw = approach_angle(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw, AnimState->m_flLastClientSideAnimationUpdateTime * 100.0f);
				}
			}
			else
			{
				AnimState->m_flGoalFeetYaw = Equalized; // c00l
			}

			pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);

		}


		StoredAnimState[pEnt->GetIndex()] = AnimState;

		oldEyeAngles[pEnt->GetIndex()] = pEnt->GetEyeAngles();

		oldSimtime[pEnt->GetIndex()] = storedSimtime[pEnt->GetIndex()];

		storedSimtime[pEnt->GetIndex()] = pEnt->GetSimTime();

		update = true;

	}

	pEnt->ClientAnimations(false);

	if (pEnt != Local && pEnt->GetTeam() != Local->GetTeam() && SETTINGS::settings.desync_resolver_bool)
	{
		pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
	}

	AnimState = StoredAnimState[pEnt->GetIndex()];

	memcpy((void*)PosParams, &StoredPosParams[pEnt->GetIndex()], (sizeof(float) * 24));
	memcpy(pEnt->GetAnimOverlays(), StoredLayers[pEnt->GetIndex()], (sizeof(SDK::CAnimationLayer) * 15));

	if (pEnt != Local && pEnt->GetTeam() != Local->GetTeam() && SETTINGS::settings.desync_resolver_bool && jittering[pEnt->GetIndex()])
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));
	else
		pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->GetIndex()], 0));

	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA30) = INTERFACES::Globals->framecount;
	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA28) = 0;
}



void DesyncCorrect::Main(SDK::CBaseEntity* pPlayerEntity)
{
	auto pLocalEntity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (pPlayerEntity == nullptr) return;
	if (!SETTINGS::settings.ResolverMode == 1) return;

	if (!pLocalEntity->IsAlive()) return;


	for (int i = 1; i < INTERFACES::Engine->GetMaxClients(); ++i)
	{
		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->GetIsDormant()
			|| pPlayerEntity == pLocalEntity
			|| pPlayerEntity->GetTeam() == pLocalEntity->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}


		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->GetIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->GetIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = UTILS::CalcAngle(pLocalEntity->GetVecOrigin(), pPlayerEntity->GetVecOrigin()).y;
			Vector ViewPoint = pLocalEntity->GetVecOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(GRD_TO_BOG(angToLocal))),(45 * cos(GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(GRD_TO_BOG(angToLocal))),(50 * cos(GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetVecOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (AutoWall->GetDamagez(OriginAutowall) > 0 && AutoWall->GetDamagez(ViewPoint) > 0)
				{
					
						if (side == 0)
						{
							HitSide1 = true;
							FreestandAngle[pPlayerEntity->GetIndex()] = 90;
						}
						else if (side == 1)
						{
							HitSide2 = true;
							FreestandAngle[pPlayerEntity->GetIndex()] = -90;
						}

						Autowalled = true;
					
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (AutoWall->GetDamagez(OriginAutowall222) > 0 && AutoWall->GetDamagez(OriginAutowall2) > 0)
						{
						
								if (side222 == 0)
								{
									HitSide1 = true;
									FreestandAngle[pPlayerEntity->GetIndex()] = 90;
								}
								else if (side222 == 1)
								{
									HitSide2 = true;
									FreestandAngle[pPlayerEntity->GetIndex()] = -90;
								}

								Autowalled = true;
							
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->GetIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->GetIndex()] = true;
			}
		}

	}

}
DesyncCorrect* desync = new DesyncCorrect();
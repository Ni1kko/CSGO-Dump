#include "Resolver.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Menu\config.h"
Resolver g_Resolver;



float Resolver::get_gun(C_BaseCombatWeapon* weapon)
{

	if (!weapon)
		return 0.f;

	if (weapon->isAuto())
		return 50.f;

	else if (weapon->is_scout())
		return 40.f;

	else if (weapon->is_awp())
		return 30.f;

	else
		return 27.f;
}
bool Resolver::enemy_is_slow_walking(C_BaseEntity* entity)
{
	C_BaseCombatWeapon* weapon = entity->GetActiveWeapon();
	if (!weapon)
		return false;

	float speed = entity->GetVelocity().Length2D();
	float get_gun = g_Resolver.get_gun(weapon);

	if (speed - get_gun < 34) // if it's more or less the same.
	{
		return true;
	}
	return false;
}
bool is_slow_walking(C_BaseEntity* entity) {
	float large = 0;
	float velocity_2D[64], old_velocity_2D[64];
	if (entity->GetVelocity().Length2D() != velocity_2D[entity->EntIndex()] && entity->GetVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->EntIndex()] = velocity_2D[entity->EntIndex()];
		velocity_2D[entity->EntIndex()] = entity->GetVelocity().Length2D();
	}
	if (large == 0)return false;
	Vector velocity = entity->GetVelocity();
	Vector direction = entity->GetEyeAngles();

	float speed = velocity.Length();
	direction.y = entity->GetEyeAngles().y - direction.y;
	//method 1
	if (velocity_2D[entity->EntIndex()] > 1) {
		int tick_counter[64];
		if (velocity_2D[entity->EntIndex()] == old_velocity_2D[entity->EntIndex()])
			tick_counter[entity->EntIndex()] += 1;
		else
			tick_counter[entity->EntIndex()] = 0;

		while (tick_counter[entity->EntIndex()] > (1 / g_pGlobalVars->intervalPerTick) * fabsf(0.1f))//should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;
	}


	return false;
}


float max_desync_angle(C_BaseEntity * entity) {

	auto animstate = uintptr_t(entity->AnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}


void Resolver::AnimationFix(C_BaseEntity* pEnt)
{
	if (pEnt == Globals::LocalPlayer) {
		pEnt->ClientAnimations(true);
		auto player_animation_state = pEnt->AnimState();
		player_animation_state->m_flLeanAmount = 20;
		player_animation_state->m_flCurrentTorsoYaw += 15;
		pEnt->UpdateClientAnimation();
		pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
		pEnt->ClientAnimations(false);
	}
	else {
		auto player_index = pEnt->EntIndex() - 1;

		pEnt->ClientAnimations(true);

		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;

		g_pGlobalVars->curtime = pEnt->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

		auto player_animation_state = pEnt->AnimState();
		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
		if (player_animation_state != nullptr && player_model_time != nullptr)
			if (*player_model_time == g_pGlobalVars->framecount)
				* player_model_time = g_pGlobalVars->framecount - 1;


		pEnt->UpdateClientAnimation();

		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;

		//pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));

		pEnt->ClientAnimations(false);
	}

}
float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
float ApproachAngle(float target, float value, float speed)
{
	target = flAngleMod(target);
	value = flAngleMod(value);

	float delta = target - value;

	// Speed is assumed to be positive
	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

/*void Resolver::animation_fix(C_BaseEntity * pEnt) { // Изначально здесь был другой анимфикс. Взял рандомный из сурса с юц, т.к анимфикс легендвара слишком хорош для паблика



	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (pEnt == Globals::LocalPlayer) {
			pEnt->ClientAnimations(true);
			auto player_animation_state = pEnt->AnimState();
			player_animation_state->m_flLeanAmount = 20;
			player_animation_state->m_flCurrentTorsoYaw += 15;
			pEnt->UpdateClientAnimation();
			pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
			pEnt->ClientAnimations(false);
		}
		else {
			auto player_index = pEnt->EntIndex() - 1;

			pEnt->ClientAnimations(true);

			auto old_curtime = g_pGlobalVars->curtime;
			auto old_frametime = g_pGlobalVars->frametime;

			g_pGlobalVars->curtime = pEnt->GetSimulationTime();
			g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

			auto player_animation_state = pEnt->AnimState();
			auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
			if (player_animation_state != nullptr && player_model_time != nullptr)
				if (*player_model_time == g_pGlobalVars->framecount)
					* player_model_time = g_pGlobalVars->framecount - 1;


			pEnt->UpdateClientAnimation();

			g_pGlobalVars->curtime = old_curtime;
			g_pGlobalVars->frametime = old_frametime;

			//pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));

			pEnt->ClientAnimations(false);
		}
	}
}*/

void HandleHits(C_BaseEntity * pEnt) // def handle
{
	auto NetChannel = g_pEngine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (Globals::Shot[pEnt->EntIndex()])
	{
		if (!init[pEnt->EntIndex()])
		{
			g_Resolver.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_pGlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_pEngine->GetNetChannel()->m_nChokedPackets);
			pEnt->SetAbsAngles(Vector(0.f, g_Resolver.pitchHit[pEnt->EntIndex()], 0.f));
			init[pEnt->EntIndex()] = true;
		}

		if (g_pGlobalVars->curtime > predTime[pEnt->EntIndex()] && !Globals::Hit[pEnt->EntIndex()])
		{
			Globals::MissedShots[pEnt->EntIndex()] += 1;
			Globals::Shot[pEnt->EntIndex()] = true;
		}
		else if (g_pGlobalVars->curtime <= predTime[pEnt->EntIndex()] && Globals::Hit[pEnt->EntIndex()])
			Globals::Shot[pEnt->EntIndex()] = true;

	}
	else
		init[pEnt->EntIndex()] = true;

	Globals::Hit[pEnt->EntIndex()] = false;
}

void Resolver::OnCreateMove() // shit
{
	if (!c_config::get().aimbot_resolver)
		return;

	if (!Globals::LocalPlayer->IsAlive())
		return;

	if (!Globals::LocalPlayer->GetActiveWeapon() || Globals::LocalPlayer->IsKnifeorNade())
		return;


	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == Globals::LocalPlayer
			|| pPlayerEntity->GetTeam() == Globals::LocalPlayer->GetTeam()) Globals::LocalPlayer;
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 27.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 27.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = g_Math.CalcAngle(Globals::LocalPlayer->GetOrigin(), pPlayerEntity->GetOrigin()).y;
			Vector ViewPoint = Globals::LocalPlayer->GetOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}
	}
}
void FixPitch(C_BaseEntity * pEnt)
{
	float last_simtime[64] = { 0.f };
	float stored_pitch_1[64] = { 0.f };
	float fixed_pitch[64] = { 0.f };

	bool has_been_set[64] = { false };

	const auto local = Globals::LocalPlayer;
	if (!local)
		return;
	if (c_config::get().pitchresolver != 1) return;
	for (auto i = 0; i < g_pEngine->GetMaxClients(); ++i)
	{

		const auto eye = pEnt->GetEyeAnglesPointer();

		auto pitch = 0.f;

		if (stored_pitch_1[i] == FLT_MAX || !has_been_set[i])
		{
			stored_pitch_1[i] = eye->x;
			has_been_set[i] = true;
		}

		if (stored_pitch_1[i] - eye->x < 30 && stored_pitch_1[i] - eye->x > -30)
		{
			pitch = eye->x;
		}
		else
		{
			pitch = stored_pitch_1[i];
		}

		pEnt->GetEyeAnglesPointer()->x = pitch;
	}
}
bool solve_desync_simple(C_BaseEntity * e) // 979
{
	if (!e || e->IsDormant() || !e->IsAlive())
		return false;

	for (size_t i = 0; i < e->NumOverlays(); i++)
	{
		auto layer = e->GetAnimOverlay4(i);
		if (!layer)
			continue;

		if (e->GetSequenceActivity(layer->m_nSequence) == 979)
		{
			if (layer->m_flWeight == 0.0f && (layer->m_flCycle == 0.0f || layer->m_flCycle != layer->m_flPrevCycle))
				return true;
		}
	}
	return false;
}
float NormalizeYaw180(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}
float NormalizeX(float yaw)
{
	if (yaw != yaw)
		yaw = 0.f;

	return fmod(yaw + 180.f, 360.f) - 180.f;
}

float approach(float cur, float target, float inc) {
	inc = abs(inc);

	if (cur < target)
		return min(cur + inc, target);
	if (cur > target)
		return max(cur - inc, target);

	return target;
}
bool delta_58(float first, float second)
{
	if (first - second < 90.f && first - second > -90.f)
	{
		return true;
	}
	return false;
}
float angle_difference(float a, float b) {
	auto diff = NormalizeYaw180(a - b);

	if (diff < 180)
		return diff;
	return diff - 360;
}
bool delta_35(float first, float second)
{
	if (first - second <= 27.f && first - second >= -27.f)
	{
		return true;
	}
	return false;
}
float approach_angle(float cur, float target, float inc) {
	auto diff = angle_difference(target, cur);
	return approach(cur, cur + diff, inc);
}

bool breaking_lby_animations(C_BaseEntity * e)
{
	if (!e || e->IsDormant() || !e->IsAlive())
		return false;

	for (size_t i = 0; i < e->NumOverlays(); i++)
	{
		auto layer = e->GetAnimOverlay4(i);
		if (e->GetSequenceActivity(layer->m_nSequence) == 979)
		{
			if (layer->m_flCycle != layer->m_flCycle || layer->m_flWeight == 1.f)
				return true;
		}
	}

	return false;
}
void Resolver::resolveOldMethod(C_BaseEntity * player)
{
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return;



	if (!player)
		return;


	bool is_local_player = player == local_player;
	bool is_teammate = local_player->GetTeam() == player->GetTeam() && !is_local_player;

	if (is_local_player)
		return;

	if (is_teammate)
		return;

	if (player->GetHealth() <= 0)
		return;


	if (local_player->GetHealth() <= 0)
		return;

	auto idx = player->EntIndex();
	auto state = player->AnimStatev2();
	auto lby = player->GetLowerBodyYaw();
	static float oldSimtimeShot[65];
	static float storedSimtimeShot[65];
	static float ShotTime[65];
	bool shot = false;
	Globals::enemyshot[idx] = false;


	if (storedSimtimeShot[idx] != player->GetSimulationTime())
	{
		if (player->GetActiveWeapon() && !player->IsKnifeorNade())
		{
			if (ShotTime[idx] != player->GetActiveWeapon()->GetLastShotTime())
			{
				shot = true;
				Globals::enemyshot[idx] = true;
				ShotTime[idx] = player->GetActiveWeapon()->GetLastShotTime();
			}
			else
			{
				shot = false;
				Globals::enemyshot[idx] = false;
			}
		}
		else
		{
			Globals::enemyshot[idx] = false;
			shot = false;
			ShotTime[idx] = 0.f;
		}

		oldSimtimeShot[idx] = storedSimtimeShot[idx];

		storedSimtimeShot[idx] = player->GetSimulationTime();
	}




	auto& record = _player[idx];

	if (state)
	{
		state->m_flEyeYaw = player->GetEyeAnglesPointer()->y;
		state->m_flEyePitch = player->GetEyeAnglesPointer()->x;
		player->setAnimationState(state);
	}

	if (state && state->speed_2d > 0.1f || fabs(state->flUpVelocity) > 100.0f)
	{
		state->m_flGoalFeetYaw = approach_angle(state->m_flEyeYaw, state->m_flGoalFeetYaw,
			((state->m_flUnknownFraction * 20.0f) + 30.0f) * state->m_flLastClientSideAnimationUpdateTime);

		player->setAnimationState(state);
	}
	else
	{
		if (state)
		{
			state->m_flGoalFeetYaw = approach_angle(lby, state->m_flGoalFeetYaw,
				state->m_flLastClientSideAnimationUpdateTime * 100.0f);

			player->setAnimationState(state);
		}
	}

	record.current_tick.eyes = player->GetEyeAnglesPointer()->y;

	if (record.current_tick.last_lby == FLT_MAX)
	{
		record.current_tick.last_lby = lby;
	}

	bool one = false;
	bool two = false;

	record.current_tick.lby_delta = record.current_tick.last_lby - lby;
	record.current_tick.eye_lby_delta = lby - record.current_tick.eyes;
	if (record.current_tick.last_lby != FLT_MAX)
	{
		if (player->GetVelocity().Length2D() > 0.1f)
		{
			if (record.current_tick.last_lby != lby)
				record.current_tick.last_lby = lby;

			one = false, two = false;
		}

		else
		{
			record.current_tick.lby_delta = record.current_tick.last_lby - lby;

			if (record.current_tick.last_lby != lby)
			{
				if (solve_desync_simple(player))
				{
					if (delta_58(record.current_tick.last_lby, lby))
					{
						record.current_tick.low_delta = true;
						record.current_tick.high_delta = false;
					}
					else
					{
						record.current_tick.low_delta = false;
						record.current_tick.high_delta = true;
					}

					if (delta_58(lby, record.current_tick.eyes) && delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						if (!one || !two)
							record.current_tick.jitter_desync = false;

						if (one && two)
							record.current_tick.jitter_desync = true;
					}

					if (record.current_tick.low_delta && !delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						if (!one)
							one = true;

						if (!two && one)
							two = true;

						if (one && two)
							record.current_tick.jitter_desync = true;
					}

					//		else
					//			record.current_tick.low_delta = true;
				}
				else
					record.current_tick.static_desync = true;
			}
			else
			{
				if (!solve_desync_simple(player))
				{
					one = false, two = false;

					record.current_tick.static_desync = true;
					record.current_tick.jitter_desync = false;

				}
				else
				{
					if (!delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						record.current_tick.low_delta = true;
						record.current_tick.high_delta = true;
						record.current_tick.jitter_desync = true;
					}
					else
					{
						record.current_tick.low_delta = true;
						record.current_tick.high_delta = false;
						record.current_tick.jitter_desync = true;
					}
				}
			}

		}
	}
	auto slow_walking = [&player]() -> bool
	{
		bool s = g_Resolver.enemy_is_slow_walking(player);

		if (s)
		{
			if (!(player->GetFlags() & FL_DUCKING))
			{
				g_Resolver.enemy_slowwalk = true;
				return true;
			}
			else
			{
				g_Resolver.enemy_slowwalk = false;
				return false;
			}
		}
		else
			return false;
	};
	if (slow_walking() && Globals::MissedShots[player->EntIndex()] >= 1)
	{
		Globals::enemyslow[player->EntIndex()] = true;
	}
	auto max_rotate = max_desync_angle(player);
	auto delta = angle_difference(player->GetEyeAnglesPointer()->y, player->AnimState()->m_flCurrentFeetYaw);


	if (record.current_tick.low_delta && !record.current_tick.jitter_desync && record.current_tick.lby_delta < 58.f)
	{
		player->GetEyeAnglesPointer()->y = record.current_tick.last_lby;
		Globals::rmode[player->EntIndex()] = 4;
	}

	else if (record.current_tick.jitter_desync && (!record.current_tick.high_delta || record.current_tick.low_delta))
	{
		// record.current_tick.eye_lby_delta

		player->GetEyeAnglesPointer()->y = lby + (-record.current_tick.eye_lby_delta);
		Globals::rmode[player->EntIndex()] = 5;
	}

	else if (record.current_tick.jitter_desync && record.current_tick.high_delta)
	{
		player->GetEyeAnglesPointer()->y = record.current_tick.last_lby;
		Globals::rmode[player->EntIndex()] = 6;
	}

	else if (record.current_tick.low_delta && record.current_tick.jitter_desync)
	{
		player->GetEyeAnglesPointer()->y = lby - record.current_tick.lby_delta;
		Globals::rmode[player->EntIndex()] = 7;
	}

	else
	{
		if (delta < 27.f && delta > -27) {
			if (player->GetVelocity().Length2D() > 0.1f)
			{
				player->GetEyeAnglesPointer()->y = player->GetLowerBodyYaw();
				Globals::rmode[player->EntIndex()] = 1;
			}
		}
		else if (delta > 27.f) {
			Globals::rmode[player->EntIndex()] = 2;
			if (player->GetEyeAnglesPointer()->y < 0.f && player->GetEyeAnglesPointer()->y > -180.f)
				player->GetEyeAnglesPointer()->y = g_Math.NormalizeYaw(player->GetEyeAnglesPointer()->y + delta);
			else if (player->GetEyeAnglesPointer()->y > 0.f && player->GetEyeAnglesPointer()->y < 180.f)
				player->GetEyeAnglesPointer()->y = g_Math.NormalizeYaw(player->GetEyeAnglesPointer()->y - delta);
		}
		else if (delta < -27.f) {
			Globals::rmode[player->EntIndex()] = 3;
			if (player->GetEyeAnglesPointer()->y < 0.f && player->GetEyeAnglesPointer()->y > -180.f)
				player->GetEyeAnglesPointer()->y = g_Math.NormalizeYaw(player->GetEyeAnglesPointer()->y - delta);
			else if (player->GetEyeAnglesPointer()->y > 0.f && player->GetEyeAnglesPointer()->y < 180.f)
				player->GetEyeAnglesPointer()->y = g_Math.NormalizeYaw(player->GetEyeAnglesPointer()->y + delta);
		}
		else
		{
			Globals::rmode[player->EntIndex()] = 9;
			auto player_animation_state = player->AnimState();
			float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

			auto FeetSpeed = 0.f;

			if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
			{
				FeetSpeed = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
			}
			else
			{
				FeetSpeed = 0.0f;
			}

			float Fraction = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * FeetSpeed;

			float YawMod = Fraction + 1.0;

			if (player_animation_state->m_fDuckAmount > 0.0) // duck checks
			{
				float curfeet = 0.0f;

				if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
				{
					curfeet = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
				}
				else
				{
					curfeet = 0.0f;
				}
			}

			float flMaxYawModifier = player_animation_state->pad10[516] * YawMod;
			float flMinYawModifier = player_animation_state->pad10[512] * YawMod;

			float newFeetYaw = 0.f;

			auto eyeYaw = player_animation_state->m_flEyeYaw;

			auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

			float eye_feet_delta = fabs(eyeYaw - lbyYaw);

			if (eye_feet_delta <= flMaxYawModifier) // SetupVelocity in help
			{
				if (flMinYawModifier > eye_feet_delta)
				{
					newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
				}
			}
			else
			{
				newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
			}

			float EndYaw = fmod(newFeetYaw, 360.0);

			if (EndYaw > 180.0)
			{
				EndYaw = EndYaw - 360.0;
			}

			if (EndYaw < 180.0)
			{
				EndYaw = EndYaw + 360.0;
			}

			player_animation_state->m_flGoalFeetYaw = EndYaw;

		}
	}
}
void Resolver::resolve(C_BaseEntity * player)
{
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return;



	if (!player)
		return;


	bool is_local_player = player == local_player;
	bool is_teammate = local_player->GetTeam() == player->GetTeam() && !is_local_player;

	if (is_local_player)
		return;

	if (is_teammate)
		return;

	if (player->GetHealth() <= 0)
		return;


	if (local_player->GetHealth() <= 0)
		return;

	auto idx = player->EntIndex();
	auto state = player->AnimStatev2();
	auto lby = player->GetLowerBodyYaw();
	static float oldSimtimeShot[65];
	static float storedSimtimeShot[65];
	static float ShotTime[65];
	bool shot = false;
	Globals::enemyshot[idx] = false;


	if (storedSimtimeShot[idx] != player->GetSimulationTime())
	{
		if (player->GetActiveWeapon() && !player->IsKnifeorNade())
		{
			if (ShotTime[idx] != player->GetActiveWeapon()->GetLastShotTime())
			{
				shot = true;
				Globals::enemyshot[idx] = true;
				ShotTime[idx] = player->GetActiveWeapon()->GetLastShotTime();
			}
			else
			{
				shot = false;
				Globals::enemyshot[idx] = false;
			}
		}
		else
		{
			Globals::enemyshot[idx] = false;
			shot = false;
			ShotTime[idx] = 0.f;
		}

		oldSimtimeShot[idx] = storedSimtimeShot[idx];

		storedSimtimeShot[idx] = player->GetSimulationTime();
	}




	auto& record = _player[idx];

	if (state)
	{
		state->m_flEyeYaw = player->GetEyeAnglesPointer()->y;
		state->m_flEyePitch = player->GetEyeAnglesPointer()->x;
		player->setAnimationState(state);
	}

	if (state && state->speed_2d > 0.1f || fabs(state->flUpVelocity) > 100.0f)
	{
		state->m_flGoalFeetYaw = approach_angle(state->m_flEyeYaw, state->m_flGoalFeetYaw,
			((state->m_flUnknownFraction * 20.0f) + 30.0f) * state->m_flLastClientSideAnimationUpdateTime);

		player->setAnimationState(state);
	}
	else
	{
		if (state)
		{
			state->m_flGoalFeetYaw = approach_angle(lby, state->m_flGoalFeetYaw,
				state->m_flLastClientSideAnimationUpdateTime * 100.0f);

			player->setAnimationState(state);
		}
	}

	record.current_tick.eyes = player->GetEyeAnglesPointer()->y;

	if (record.current_tick.last_lby == FLT_MAX)
	{
		record.current_tick.last_lby = lby;
	}

	bool one = false;
	bool two = false;

	record.current_tick.lby_delta = record.current_tick.last_lby - lby;
	record.current_tick.eye_lby_delta = lby - record.current_tick.eyes;
	if (record.current_tick.last_lby != FLT_MAX)
	{
		if (player->GetVelocity().Length2D() > 0.1f)
		{
			if (record.current_tick.last_lby != lby)
				record.current_tick.last_lby = lby;

			one = false, two = false;
		}

		else
		{
			record.current_tick.lby_delta = record.current_tick.last_lby - lby;

			if (record.current_tick.last_lby != lby)
			{
				if (solve_desync_simple(player))
				{
					if (delta_58(record.current_tick.last_lby, lby))
					{
						record.current_tick.low_delta = true;
						record.current_tick.high_delta = false;
					}
					else
					{
						record.current_tick.low_delta = false;
						record.current_tick.high_delta = true;
					}

					if (delta_58(lby, record.current_tick.eyes) && delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						if (!one || !two)
							record.current_tick.jitter_desync = false;

						if (one && two)
							record.current_tick.jitter_desync = true;
					}

					if (record.current_tick.low_delta && !delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						if (!one)
							one = true;

						if (!two && one)
							two = true;

						if (one && two)
							record.current_tick.jitter_desync = true;
					}

					//		else
					//			record.current_tick.low_delta = true;
				}
				else
					record.current_tick.static_desync = true;
			}
			else
			{
				if (!solve_desync_simple(player))
				{
					one = false, two = false;

					record.current_tick.static_desync = true;
					record.current_tick.jitter_desync = false;

				}
				else
				{
					if (!delta_58(record.current_tick.last_lby, record.current_tick.eyes))
					{
						record.current_tick.low_delta = false;
						record.current_tick.high_delta = true;
						record.current_tick.jitter_desync = true;
					}
					else
					{
						record.current_tick.low_delta = true;
						record.current_tick.high_delta = false;
						record.current_tick.jitter_desync = true;
					}
				}
			}

		}
	}
	auto slow_walking = [&player]() -> bool
	{
		bool s = g_Resolver.enemy_is_slow_walking(player);

		if (s)
		{
			if (!(player->GetFlags() & FL_DUCKING))
			{
				g_Resolver.enemy_slowwalk = true;
				return true;
			}
			else
			{
				g_Resolver.enemy_slowwalk = false;
				return false;
			}
		}
		else
			return false;
	};
	if (slow_walking() && Globals::MissedShots[player->EntIndex()] >= 1)
	{
		Globals::enemyslow[player->EntIndex()] = true;
	}
	auto max_rotate = max_desync_angle(player);
	auto delta = angle_difference(player->GetEyeAnglesPointer()->y, player->AnimState()->m_flCurrentFeetYaw);


	if (record.current_tick.low_delta && !record.current_tick.jitter_desync && record.current_tick.lby_delta < 58.f)
	{
		player->GetEyeAnglesPointer()->y = record.current_tick.last_lby;
		Globals::rmode[player->EntIndex()] = 4;
	}

	else if (record.current_tick.jitter_desync && (!record.current_tick.high_delta || record.current_tick.low_delta))
	{
		// record.current_tick.eye_lby_delta

		player->GetEyeAnglesPointer()->y = lby + (-record.current_tick.eye_lby_delta);
		Globals::rmode[player->EntIndex()] = 5;
	}

	else if (record.current_tick.jitter_desync && record.current_tick.high_delta)
	{
		player->GetEyeAnglesPointer()->y = record.current_tick.last_lby;
		Globals::rmode[player->EntIndex()] = 6;
	}

	else if (record.current_tick.low_delta && record.current_tick.jitter_desync)
	{
		player->GetEyeAnglesPointer()->y = lby - record.current_tick.lby_delta;
		Globals::rmode[player->EntIndex()] = 7;
	}

	else
	{

		Globals::rmode[player->EntIndex()] = 9;
		auto player_animation_state = player->AnimState();
		float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

		auto v48 = 0.f;

		if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
		{
			v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
		}
		else
		{
			v48 = 0.0f;
		}

		float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

		float flYawModifier = v49 + 1.0;

		if (player_animation_state->m_fDuckAmount > 0.0)
		{
			float v53 = 0.0f;

			if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
			{
				v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
			}
			else
			{
				v53 = 0.0f;
			}
		}

		float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
		float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

		float newFeetYaw = 0.f;

		auto eyeYaw = player_animation_state->m_flEyeYaw;

		auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

		float eye_feet_delta = fabs(eyeYaw - lbyYaw);

		if (eye_feet_delta <= flMaxYawModifier)
		{
			if (flMinYawModifier > eye_feet_delta)
			{
				newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
			}
		}
		else
		{
			newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
		}

		float v136 = fmod(newFeetYaw, 360.0);

		if (v136 > 180.0)
		{
			v136 = v136 - 360.0;
		}

		if (v136 < 180.0)
		{
			v136 = v136 + 360.0;
		}

		player_animation_state->m_flGoalFeetYaw = v136;


	}



}


void Resolver::FrameStage(ClientFrameStage_t stage)
{
	if (!Globals::LocalPlayer || !g_pEngine->IsInGame())
		return;

	static bool  wasDormant[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->IsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity); // на него похуй, он работает и ваще поебать

		}
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (c_config::get().resolvemethod == 0)
				resolve(pPlayerEntity); // 8/10
			else if (c_config::get().resolvemethod == 1)
				resolveOldMethod(pPlayerEntity); // 6.5/10
			FixPitch(pPlayerEntity); // golden (meme)
		}
		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != Globals::LocalPlayer)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}

/*
my attempt at fixing desync and i was pretty successful
it can resolve static desync pretty perfectly
and can resolve some jitter desync but
it still gets rekt by other things
*/

/*
void Resolver::AnimationFix(C_BaseEntity* pEnt)
{
	if (pEnt == Globals::LocalPlayer) {
		pEnt->ClientAnimations(true);
		auto player_animation_state = pEnt->AnimState();
		player_animation_state->m_flLeanAmount = 20;
		player_animation_state->m_flCurrentTorsoYaw += 15;
		pEnt->UpdateClientAnimation();
		pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
		pEnt->ClientAnimations(false);
	}
	else {
		auto player_index = pEnt->EntIndex() - 1;

		pEnt->ClientAnimations(true);

		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;

		g_pGlobalVars->curtime = pEnt->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

		auto player_animation_state = pEnt->AnimState();
		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
		if (player_animation_state != nullptr && player_model_time != nullptr)
			if (*player_model_time == g_pGlobalVars->framecount)
				*player_model_time = g_pGlobalVars->framecount - 1;


		pEnt->UpdateClientAnimation();

		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;

		//pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));

		pEnt->ClientAnimations(false);
	}
	
}
float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
float ApproachAngle(float target, float value, float speed)
{
	target = flAngleMod(target);
	value = flAngleMod(value);

	float delta = target - value;

	// Speed is assumed to be positive
	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

void update_state(C_AnimState * state, Vector angles) {
	using Fn = void(__vectorcall*)(void *, void *, float, float, float, void *);
	static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
	fn(state, nullptr, 0.0f, angles[1], angles[0], nullptr);
}

void HandleBackUpResolve(C_BaseEntity* pEnt) {

	if (!c_config::get().aimbot_resolver)
		return;

	if (pEnt->GetTeam() == Globals::LocalPlayer->GetTeam())
		return;

	const auto player_animation_state = pEnt->AnimState();

	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v48 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	player_animation_state->m_flGoalFeetYaw = v136;

	/*static int stored_yaw = 0;

	if (pEnt->GetEyeAnglesPointer()->y != stored_yaw) {
		if ((pEnt->GetEyeAnglesPointer()->y - stored_yaw > 120)) { // Arbitrary high angle value.
			if (pEnt->GetEyeAnglesPointer()->y - stored_yaw > 120) {
				pEnt->GetEyeAnglesPointer()->y = pEnt->GetEyeAnglesPointer()->y - (pEnt->GetEyeAnglesPointer()->y - stored_yaw);
			}

			stored_yaw = pEnt->GetEyeAnglesPointer()->y;
		}
	}*/
	//if (pEnt->GetVelocity().Length2D() > 0.1f)
	//{
	//	player_animation_state->m_flGoalFeetYaw = ApproachAngle(pEnt->GetLowerBodyYaw(), player_animation_state->m_flGoalFeetYaw, (player_animation_state->m_flStopToFullRunningFraction * 20.0f) + 30.0f *player_animation_state->m_flLastClientSideAnimationUpdateTime);
	//}
	//else
	//{
	//	player_animation_state->m_flGoalFeetYaw = ApproachAngle(pEnt->GetLowerBodyYaw(), player_animation_state->m_flGoalFeetYaw, (m_flLastClientSideAnimationUpdateTimeDelta * 100.0f));
	//}
	//if (Globals::MissedShots[pEnt->EntIndex()] > 3) {
	//	switch (Globals::MissedShots[pEnt->EntIndex()] % 4) {
	//	case 0: pEnt->GetEyeAnglesPointer()->y = pEnt->GetEyeAnglesPointer()->y + 45; break;
	//	case 1: pEnt->GetEyeAnglesPointer()->y = pEnt->GetEyeAnglesPointer()->y - 45; break;
	//	case 2: pEnt->GetEyeAnglesPointer()->y = pEnt->GetEyeAnglesPointer()->y - 30; break;
	//	case 3: pEnt->GetEyeAnglesPointer()->y = pEnt->GetEyeAnglesPointer()->y + 30; break;
	//	}
	//}
//}
/*
void HandleHits(C_BaseEntity* pEnt)
{
	auto NetChannel = g_pEngine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (Globals::Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			g_Resolver.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_pGlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_pEngine->GetNetChannel()->m_nChokedPackets);
			init[pEnt->EntIndex()] = false;
		}

		if (g_pGlobalVars->curtime > predTime[pEnt->EntIndex()] && !Globals::Hit[pEnt->EntIndex()])
		{
			Globals::MissedShots[pEnt->EntIndex()] += 1;
			Globals::Shot[pEnt->EntIndex()] = false;
		}
		else if (g_pGlobalVars->curtime <= predTime[pEnt->EntIndex()] && Globals::Hit[pEnt->EntIndex()])
			Globals::Shot[pEnt->EntIndex()] = false;

	}
	else
		init[pEnt->EntIndex()] = true;

	Globals::Hit[pEnt->EntIndex()] = false;
}

void Resolver::OnCreateMove() // cancer v2
{
	if (!c_config::get().aimbot_resolver)
		return;

	if (!Globals::LocalPlayer->IsAlive())
		return;

	if (!Globals::LocalPlayer->GetActiveWeapon() || Globals::LocalPlayer->IsKnifeorNade())
		return;


	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == Globals::LocalPlayer
			|| pPlayerEntity->GetTeam() == Globals::LocalPlayer->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = g_Math.CalcAngle(Globals::LocalPlayer->GetOrigin(), pPlayerEntity->GetOrigin()).y;
			Vector ViewPoint = Globals::LocalPlayer->GetOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}
	}
}

void Resolver::FrameStage(ClientFrameStage_t stage)
{
	if (!Globals::LocalPlayer || !g_pEngine->IsInGame())
		return;

	static bool  wasDormant[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->IsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity);
			AnimationFix(pPlayerEntity);
			
			
		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			HandleBackUpResolve(pPlayerEntity);
		}

		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != Globals::LocalPlayer)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}*/
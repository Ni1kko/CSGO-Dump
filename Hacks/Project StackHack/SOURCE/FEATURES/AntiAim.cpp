#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../DesyncCorrect.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/AutoWall.h"
#include <ctime>
#include <cstdlib>
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

int d3sync = 60; // global int 
template <class T>
constexpr const T& Min(const T& x, const T& y)
{
	return (x > y) ? y : x;
}

template <class T>
constexpr const T& Max(const T& x, const T& y)
{
	return (x < y) ? y : x;
}

float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

float NormalizeYaw122(float yaw)
{


	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;

}

bool IsPressingMovementKeys(SDK::CUserCmd* cmd)
{
	if (!cmd)
		return false;

	if (cmd->buttons & IN_FORWARD ||
		cmd->buttons & IN_BACK || cmd->buttons & IN_LEFT || cmd->buttons & IN_RIGHT ||
		cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		return true;

	return false;
}

inline float N0rmalizePiTCch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}

float GetLBYRotatedYaw122(float lby, float yaw)
{
	float delta = NormalizeYaw122(yaw - lby);
	if (fabs(delta) < 25.f)
		return lby;

	if (delta > 0.f)
		return yaw + 25.f;

	return yaw;
}

bool flip;

void fliptime()
{
	static clock_t start1_t1 = clock();
	double timeSoFar1 = (double)(clock() - start1_t1) / CLOCKS_PER_SEC;
	if (timeSoFar1 < 0.04)
		return;
	flip = !flip;
	start1_t1 = clock();
}


bool next_lby_update(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = local_player->GetSimTime() + INTERFACES::Globals->interval_per_tick;;
	local_update = next_lby_update_time;

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
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

float fov_player(Vector ViewOffSet, Vector View, SDK::CBaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->get_hitbox_pos(entity, hitbox);

	MATH::AngleVectors(Angles, &Forward);
	MATH::VectorSubtract(AimPos, Origin, Delta);
	MATH::NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

void AngleVectorsp(const Vector angles, Vector& forward, Vector& right, Vector& up)
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[0] * (M_PI / 180.f);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[1] * (M_PI / 180.f);
	sy = sin(angle);
	cy = cos(angle);


	forward[0] = cp * cy;
	forward[1] = cp * sy;
	forward[2] = -sp;


	static float sr, cr;

	angle = angles[2] * (M_PI / 180.f);
	sr = sin(angle);
	cr = cos(angle);


	right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
	right[1] = -1 * sr * sp * sy + -1 * cr *cy;
	right[2] = -1 * sr * cp;

	up[0] = cr * sp *cy + -sr * -sy;
	up[1] = cr * sp *sy + -sr * cy;
	up[2] = cr * cp;


}

float GetBestHeadAngle(float yaw)
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	SDK::trace_t tr;
	SDK::Ray_t ray, ray2, ray3, ray4, ray5;
	SDK::CTraceFilter filter;

	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	viewangles.x = 0;

	AngleVectorsp(viewangles, forward, right, up);
	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);
	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	filter.pSkip1 = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	src3D = local->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.end - tr.start).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	INTERFACES::Trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.end - tr.start).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	INTERFACES::Trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.end - tr.start).Length();

	if (Back < Right && Back < Left && Left == Right && !(entity == nullptr)) {
		return (yaw); //if left and right are equal and better than back
	}
	else if (Back > Right && Back > Left && !(entity == nullptr)) {
		return (yaw + 180); //if back is the best angle
	}
	else if (Right > Left && Right > Back && !(entity == nullptr)) {
		return (yaw + 90); //if right is the best angle
	}
	else if (Right > Left && Right == Back && !(entity == nullptr)) {
		return (yaw + 135); //if right is equal to back
	}
	else if (Left > Right && Left > Back && !(entity == nullptr)) {
		return (yaw - 90); //if left is the best angle
	}
	else if (Left > Right && Left == Back && !(entity == nullptr)) {
		return (yaw - 135); //if left is equal to back
	}
	else { //if none of the above cases is true
		if (entity == nullptr) //if there is no enemy around / the enemy is null
			return (yaw + 180); //do backward antiaim
		else //if the enemy isn't null
			return (yaw + 180); //do backward at target antiaim
	}

	return 0;

}

float setup_pitch(int mode) {
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	if (!mode) //off
		return viewangles.x;

	switch (mode) {
	case 1: { //emotion
		return 89;
	} break;
	case 2: { //down
		return 179.99;
	} break;
	case 3: { //up
		return -89.99;
	} break;
	case 4: { //unsafe up
		return -179.99;
	} break;
	case 5: { //zero
		return 0;
	} break;
	case 6: { //unsafe zero
		return 1080.99;
	} break;
	} return false;
}


float GetDesyncDelta()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());	
	auto* animstate = local_player->GetAnimState();
	float flRunningSpeed = clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
	float flYawModifier = ((animstate->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * flRunningSpeed;
	float flYawModifier2 = flYawModifier + 1.f;

	if (animstate->m_fDuckAmount > 0.f)
	{
		float maxVelocity = clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
		float duckSpeed = animstate->m_fDuckAmount * maxVelocity;
		flYawModifier2 += (duckSpeed * (0.5f - flYawModifier2));
	}

	return *(float*)((uintptr_t)animstate + 0x334) * flYawModifier2;
}
	
float get_max_desync_delta() 
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto animstate = uintptr_t(local_player->GetAnimState());
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
float DoEdging(float yaw, float lby, SDK::CUserCmd* cmd, int move_state)
{
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;

	bool is_jumping = move_state == 2,
		is_moving = move_state == 1;

	auto DoFreestanding = [local_player, viewangles, is_moving](float& yaw, float& lby) -> bool
	{

		float last_move = UTILS::GetCurtime();

		static constexpr int damage_tolerance = 10;

		std::vector<SDK::CBaseEntity*> enemies;

		/// Find the lowest fov enemy
		SDK::CBaseEntity* closest_enemy = nullptr;
		float lowest_fov = 360.f;
		for (int i = 0; i < 64; i++)
		{
			auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
			if (!entity || entity->GetIsDormant() || entity->GetTeam() == local_player->GetTeam() || entity->GetHealth() <= 0)
				continue;

			const float current_fov = fabs(MATH::NormalizeYaw(UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - viewangles.y));
			if (current_fov < lowest_fov)
			{
				lowest_fov = current_fov;
				closest_enemy = entity;
			}

			enemies.push_back(entity);
		}

		/// No enemies
		if (closest_enemy == nullptr)
			return false;


		const float at_target_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).y;
		const float right_yaw = at_target_yaw - 90.f;
		const float left_yaw = at_target_yaw + 90.f;

		/// Misc functions
		auto CalcDamage = [local_player, enemies](Vector point) -> int
		{
			int damage = 0;
			for (auto& enemy : enemies)
			{
				damage += Max<int>(AutoWall->GetDamagez(enemy->GetVecOrigin() + Vector(0, 0, 64) + point),
					AutoWall->GetDamagez(enemy->GetVecOrigin() + Vector(0, 0, 49.f) + point));
			}

			return damage;
		};

		auto RotateAndExtendPosition = [](Vector position, float yaw, float distance) -> Vector
		{
			Vector direction;
			MATH::AngleVectors(Vector(0, yaw, 0), &direction);

			return position + (direction * distance);
		};
		auto RotateLBYAndYaw = [right_yaw, left_yaw, local_player](int right_damage, int left_damage, float lby_delta, float& yaw, float& lby, bool prefect_angle = false, bool symetrical = true) -> bool
		{
			const bool prefer_right = right_damage < left_damage;

			yaw = prefer_right ? right_yaw : left_yaw;
			if (symetrical)
				lby_delta = lby_delta * (prefer_right ? -1.f : 1.f);

			/// If not moving
			if (prefect_angle)
			{
				if (local_player->GetVelocity().Length2D() < 0.1f)
					yaw = GetLBYRotatedYaw122(yaw + lby_delta, yaw);
				else
					yaw = GetLBYRotatedYaw122(local_player->GetLowerBodyYaw(), yaw);
			}

			lby = yaw + lby_delta;

			if (fabs(MATH::NormalizeYaw(local_player->GetLowerBodyYaw() - lby)) < 35.f)
			{
				yaw = local_player->GetLowerBodyYaw() - lby_delta;
				lby = yaw + lby_delta;
			}

			return true;
		};
		auto DoBackFreestanding = [at_target_yaw, local_player](float& yaw, float& lby) -> bool
		{
			yaw += 180.f;
			lby = yaw;

			return true;
		};

		const auto head_position = local_player->GetVecOrigin() + Vector(0, 0, 74.f);
		const auto back_head_position = RotateAndExtendPosition(head_position, at_target_yaw - 180.f, 17.f);
		auto right_head_position = RotateAndExtendPosition(head_position, right_yaw, 17.f);
		auto left_head_position = RotateAndExtendPosition(head_position, left_yaw, 17.f);

		int right_damage = CalcDamage(right_head_position), left_damage = CalcDamage(left_head_position), back_damage = CalcDamage(back_head_position);

		/// too much damage to both sides
		if (right_damage > damage_tolerance && left_damage > damage_tolerance)
		{
			/// do backwards if valid
			if (back_damage < damage_tolerance)
				return DoBackFreestanding(yaw, lby);

			return false;
		}

		/// keep searching for a better angle
		if (right_damage == left_damage)
		{
			/// if on top of them, prioritise backwards antiaim
			if (N0rmalizePiTCch(UTILS::CalcAngle(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
				return DoBackFreestanding(yaw, lby);

			/// do some traces a bit further out
			right_head_position = RotateAndExtendPosition(head_position, right_yaw, 50.f);
			left_head_position = RotateAndExtendPosition(head_position, left_yaw, 50.f);

			right_damage = CalcDamage(right_head_position), left_damage = CalcDamage(left_head_position);
			if (right_damage == left_damage)
			{
				/// just return the side closest to a wall
				right_head_position = UTILS::TraceToEnd(head_position, RotateAndExtendPosition(head_position, right_yaw, 17.f));
				left_head_position = UTILS::TraceToEnd(head_position, RotateAndExtendPosition(head_position, left_yaw, 17.f));

				float distance_1, distance_2;
				SDK::trace_t trace;
				SDK::Ray_t ray;
				SDK::CTraceWorldOnly filter;
				const auto end_pos = closest_enemy->GetVecOrigin() + Vector(0, 0, 64.f);

				/// right position
				ray.Init(right_head_position, end_pos);
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
				distance_1 = (right_head_position - trace.end).Length();

				/// left position
				ray.Init(left_head_position, end_pos);
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
				distance_2 = (left_head_position - trace.end).Length();

				if (fabs(distance_1 - distance_2) > 15.f)
				{
					RotateLBYAndYaw(distance_1, distance_2, 119, yaw, lby);

					return true;
				}

				return DoBackFreestanding(yaw, lby);
			}
			else
			{
				RotateLBYAndYaw(right_damage, left_damage, 119, yaw, lby);

				return true;
			}
		}
		else /// found an angle that does less damage than the other
		{
			/// if on top of them, prioritise backwards antiaim
			if (N0rmalizePiTCch(UTILS::CalcAngle(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
				return DoBackFreestanding(yaw, lby);

			const bool prefer_right = (right_damage < left_damage);
			const float lby_delta = 119;

			yaw = prefer_right ? right_yaw : left_yaw;
			lby = yaw + lby_delta;

			if (fabs(MATH::NormalizeYaw(local_player->GetLowerBodyYaw() - lby)) < 35.f)
			{
				yaw = local_player->GetLowerBodyYaw() - lby_delta;
				lby = yaw + lby_delta;
			}

			if (CalcDamage(RotateAndExtendPosition(head_position, lby, 18.f)) > 0)
			{
				lby = yaw + (prefer_right ? -115.f : 115.f);
				if (CalcDamage(RotateAndExtendPosition(head_position, lby, 18.f)) > 0)
					lby = yaw;
			}


			return true;
		}

		return false;

	};


	if (DoFreestanding(yaw, lby)) {
		GLOBAL::found_angle = true;
		if (next_lby_update(cmd->viewangles.y, cmd)) {
			return lby;
		}
		else {
			return yaw;
		}
	}
	else
		GLOBAL::found_angle = false;

	return 0;
}
bool kamaz_lby_update(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	static float kamaz_update_time = 0.f;

	float curtime = (float)(local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick);

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (local_player->GetFlags() & FL_ONGROUND)
	{
		if (animstate->m_velocity > 15)  // скорость > 15 (игрок двигается вперёд назад вперёд назад)
		{
			kamaz_update_time = curtime + 0.22; // на муве таймер + 0.22
		}

		if (SETTINGS::settings.flipup)
		{
			if (kamaz_update_time < curtime)
			{
				cmd->viewangles.x = -89;
				kamaz_update_time = curtime + 1.1; // на стендах таймер + 1.1
				return true;
			}
			else
			{
				cmd->viewangles.x = 89;
			}
		}
		else
		{
			if (kamaz_update_time < curtime)
			{
				kamaz_update_time = curtime + 1.1; // на стендах таймер + 1.1
				return true;
			}
		}
	}
	return false;
}	

float setup_yaw(int mode, SDK::CUserCmd* cmd) {
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	bool is_air = local_player->GetFlags() & FL_ONGROUND ? false : true;
	bool is_move = ((local_player->GetVelocity().Length2D() > SETTINGS::settings.stand_velocity_threshold ? true : false) && !is_air);
	bool is_stand = ((local_player->GetVelocity().Length2D() <= SETTINGS::settings.stand_velocity_threshold ? true : false) && !is_air);

	int move_state = 0;
	if (is_air) {
		move_state = 2;
	}
	else if (is_move) {
		move_state = 1;
	}
	float last_move = UTILS::GetCurtime();

	float lby = local_player->GetLowerBodyYaw();
	float lby_flick = viewangles.y + 120;

	float value = 0;
	if (!mode) //off
		return viewangles.y;

	switch (mode) {
	case 1: { //backwards
		value = viewangles.y + 180;
	} break;
	case 2: { //lby
		value = local_player->GetLowerBodyYaw();
	} break;
	case 3: { //freestanding
		value = GetBestHeadAngle(viewangles.y);
	} break;
	case 4:
	{
		// bame freestanding
		if (GLOBAL::found_angle)
		{
			value = viewangles.y + DoEdging(viewangles.y, lby_flick, cmd, move_state);
		}
		else
		{
			value = viewangles.y - 180;
		}
	} break;
	}

	//jitter desync related stuff
	/*if (is_stand && SETTINGS::settings.stand_desync == 2 || is_move && SETTINGS::settings.move_desync == 2 || is_air && SETTINGS::settings.air_desync == 2 ) 
	{
		if (flip) {
			value += SETTINGS::settings.jitter_width;
		}
	}*/


	return value;
}


void DesyncAnimations(int type) {
	
	
	type = SETTINGS::settings.desync_mode;
	auto stand = SETTINGS::settings.stand_yaw;
	auto move = SETTINGS::settings.move_yaw;
	auto air = SETTINGS::settings.air_yaw;

	int m_iJitter = 0;
	SDK::CUserCmd* cmd = nullptr;
	auto net_channel = INTERFACES::Engine->GetNetChannel();

	if (stand == 3 || stand == 4 && move == 3 || move == 4 && air == 3 || air == 4) {
		if (stand == 3 || stand == 4 && move == 3 || move == 4 && air == 3 || air == 4) {
			type = 3 - (GetBestHeadAngle(cmd->viewangles.y) != 1);
			cmd->viewangles.y -= 90.0f * static_cast< float >(GetBestHeadAngle(cmd->viewangles.y));
		}
		else if (stand == 3 && move == 3 && air == 3) {
			type = 4;
		}
	}

	if (!type)
		return;

	

	cmd->viewangles.y = NormalizeYaw122(cmd->viewangles.y);

	if (type == 4) {
		int jitter_side = 1;
		if (stand == 3 || stand == 4 && move == 3 || move == 4 && air == 3 || air == 4) {
			jitter_side = GetBestHeadAngle(cmd->viewangles.y);
			if (!GetBestHeadAngle(cmd->viewangles.y))
				cmd->viewangles.y += 90.0f * static_cast< float >(GetBestHeadAngle(cmd->viewangles.y));
		}
		else {
			cmd->viewangles.y += 90.0f;
		}

		float desync = get_max_desync_delta();
		float lby_delta = 180.0f - desync + 10.0f;
		float desync_length = 180.0f - lby_delta * 0.5f;
		float jitter = 90.0f * jitter_side;

		if (jitter_side == 1)
			cmd->viewangles.y += desync_length;
		else if (jitter_side == -1)
			cmd->viewangles.y -= desync_length;

		//if ( DesyncRotate( jitter, jitter_side ) ) {
		//  m_iJitter = 0;
		//  return;
		//}

		int v19 = 0;
		if (INTERFACES::Globals->curtime < next_lby_update(cmd->viewangles.y, cmd)) {
			v19 = m_iJitter;
		}
		else {
			m_iJitter = 0;
		}

		int v20 = v19 - 1;
		if (v20) {
			if (v20 == 1) {
				if (jitter_side == 1)
					cmd->viewangles.y += lby_delta;
				else
					cmd->viewangles.y += desync - 190.0f;
			}
		}
		else {
			if (jitter_side == 1)
				cmd->viewangles.y += desync - 190.0;
			else
				cmd->viewangles.y += lby_delta;
			GLOBAL::should_send_packet = false;
		}

		if (++m_iJitter >= 3)
			m_iJitter = 0;

		cmd->viewangles.y = NormalizeYaw122(cmd->viewangles.y);
		return;
	}


	float stretch = cmd->viewangles.y;
	if (type == 3)
		stretch -= 90.0f;
	else
		stretch += 90.0f;

	/* if ( DesyncRotate( stretch, type == Stretch ) )
	return;*/

	float desync = get_max_desync_delta();
	float balance = 1.0f;
	if (type == 2)
		balance = -1.0f;

	if (INTERFACES::Globals->curtime <= next_lby_update(cmd->viewangles.y, cmd)) 
	{
		if (net_channel->m_nChokedPackets >= 2) 
		{
			cmd->viewangles.y = NormalizeYaw122(cmd->viewangles.y);
			return;
		}

		if (type == 1)
			cmd->viewangles.y -= 100.0f;
		else
			cmd->viewangles.y += (balance * 120.0f);
	}
	else if (type != 1) {
		cmd->viewangles.y -= (desync + 30.0f) * balance;
	}

	GLOBAL::should_send_packet = true;
	cmd->viewangles.y = NormalizeYaw122(cmd->viewangles.y);
	return;
}

float adiff(float a, float b) {
	auto diff = desync->NormalizeYaw180(a - b);

	if (diff < 180)
		return diff;
	return diff - 360;
}

auto get_fixed_feet_yaw = [](void) {

	auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));

	auto state = local_player->GetAnimState();

	float current_feet_yaw = *(float *)(state + 0x80);

	if (current_feet_yaw >= -360)
		current_feet_yaw = min(current_feet_yaw, 360.0);

	return current_feet_yaw;
};

float BalanceDumpedFromPolak(int type) {

	auto net_channel = INTERFACES::Engine->GetNetChannel();

	float desync = GetDesyncDelta();

	float balance = 1.0f;
	if (type == 2)
		balance = -1.0f;

	if (INTERFACES::Globals->curtime <= next_lby_update(GLOBAL::originalCMD.viewangles.y, &GLOBAL::originalCMD)) {
		if (net_channel->m_nChokedPackets >= 2)
			return NormalizeYaw122(GLOBAL::originalCMD.viewangles.y);

		if (type == 1)
			return -100.0f;
		else
			return +(balance * 120.0f);
	}
	else if (type != 1) {
		return -((desync + 30.0f) * balance);
	}
}

float setup_desync(int mode) 
{
	//SDK::CUserCmd* cmd = nullptr;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());


	auto RealWontUpdate = [&local_player]() -> bool // pro lambda
	{
		auto* AnimState = local_player->GetAnimState();
		float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * AnimState->m_flEyeYaw);
		float Delta = adiff(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw);

		if (Delta <= maxYaw)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	float max_delta = 0;

	if (!mode)
		return false;

	

	switch (mode) 
	{
	case 1: 
	{ // normal
		max_delta = GetDesyncDelta();

	} break;
	case 2: 
	{ // jitter
		
		if ((GLOBAL::originalCMD.viewangles.y - get_fixed_feet_yaw()) < GetDesyncDelta())
		{
			max_delta = 180;
		}
		else
		{
			max_delta = GetDesyncDelta();
		}
		GLOBAL::jitter = true;

	
	} break;
	case 3: // balance
	{
		max_delta = BalanceDumpedFromPolak(1); // doesnt work
	} break;
	case 4: // stretch
	{
		max_delta = BalanceDumpedFromPolak(2); // thanks bad poopoo wanheda aa :(((
	}

	}
	if (mode != 2)
		GLOBAL::jitter = false;

	return max_delta;
}
 /*float BalanceDumpedFromPolak(int type) {
    auto net_channel = *reinterpret_cast<INetChannel**>(reinterpret_cast<std::uintptr_t>(g_pClientState) + 0x9C);

    float desync = max_desync_angle();

    float balance = 1.0f;
    if (type == 2)
        balance = -1.0f;

    if (g_pGlobalVars->curtime <= next_lby_update(g::pCmd)) {
        if (net_channel->choked_packets >= 2)
            return quick_normalize(g::pCmd->viewangles.y, -180.f, 180.f);

        if (type == 1)
            return -100.0f;
        else
            return +(balance * 120.0f);
    }
    else if (type != 1) {
        return -((desync + 30.0f) * balance);
    }
}

/* fake yaw value function */
/*float DoFakeYaw() {

	switch (GetCorrectFakeYawSetting()) {
	case 1:
		return BalanceDumpedFromPolak(1);    //balance
	case 2:
		return BalanceDumpedFromPolak(2);    //strech
	case 3:
		//jitter
		if ((g::pCmd->viewangles.y - get_fixed_feet_yaw()) < max_desync_angle())
			return 180;
		else
			return max_desync_angle();
	}
}*/
float setup_lby(SDK::CUserCmd* cmd) {

	float delta = 0;

	if (next_lby_update(cmd->viewangles.y, cmd))
	{
		delta = 120;
	}
	else
	delta = 0;


	return delta;
}
float detectdatwallbitch(float yaw) //actual freestanding part
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	SDK::trace_t tr;
	SDK::Ray_t ray, ray2, ray3, ray4, ray5;
	SDK::CTraceFilter filter;

	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	viewangles.x = 0;

	//	Vector(viewangles, forward, right, up);
	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);
	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	filter.pSkip1 = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	src3D = local->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.end - tr.start).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	INTERFACES::Trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.end - tr.start).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	INTERFACES::Trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.end - tr.start).Length();

	if (Back < Right && Back < Left && Left == Right && !(entity == nullptr)) {
		return (yaw);
	}
	if (Back > Right && Back > Left && !(entity == nullptr)) {
		return (yaw - 180);
	}
	else if (Right > Left && Right > Back && !(entity == nullptr)) {
		return (yaw + 90);
	}
	else if (Right > Left && Right == Back && !(entity == nullptr)) {
		return (yaw + 135);
	}
	else if (Left > Right && Left > Back && !(entity == nullptr)) {
		return (yaw - 90);
	}
	else if (Left > Right && Left == Back && !(entity == nullptr)) {
		return (yaw - 135);
	}
	else {
		if (entity == nullptr)
			return (yaw - 180); //backward 0.o
		else //if the entity isn't null
			return ((UTILS::CalcAngle(local->GetVecOrigin(), entity->GetVecOrigin()).y) + 180); //else do backward at target antiaim
	}

	return 0;
}
void CAntiAim::freestand(SDK::CUserCmd* cmd, float yawangs)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
		return endpos1.Dist(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();

	static SDK::CBaseEntity* entity;

	if (index != -1)
		entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}

	if (!GLOBAL::should_send_packet)
	{
		if (next_lby_update(cmd->viewangles.y, cmd))
		{
			cmd->viewangles.y = last_real + SETTINGS::settings.delta_val;
		}
		else
		{
			if (no_active)
			{
				cmd->viewangles.y += 179.000000;
			}
			else
				cmd->viewangles.y = RAD2DEG(bestrotation);

			last_real = cmd->viewangles.y;
		}
	}
}

void autoDirection(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f;
	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}
	if (no_active)
		cmd->viewangles.y += 180.f;
	else
		cmd->viewangles.y = RAD2DEG(bestrotation);

	last_real = cmd->viewangles.y;
}



void CAntiAim::do_antiaim(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return; auto grenade = (SDK::CBaseCSGrenade*)weapon;
	if (local_player->GetMoveType() == SDK::MOVETYPE_LADDER || local_player->GetMoveType() == SDK::MOVETYPE_NOCLIP) return;
	if (cmd->buttons & IN_USE) return;
	if (cmd->buttons & IN_ATTACK) return;
	if (weapon->is_grenade()) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (!SETTINGS::settings.aa_bool) return;
	if (GLOBAL::DisableAA) return;
	fliptime();
	bool is_air = local_player->GetFlags() & FL_ONGROUND ? false : true;
	bool is_move = ((local_player->GetVelocity().Length2D() > SETTINGS::settings.stand_velocity_threshold ? true : false) && !is_air);
	bool is_stand = ((local_player->GetVelocity().Length2D() <= SETTINGS::settings.stand_velocity_threshold ? true : false) && !is_air);
	static float last_real; int local_flags = local_player->GetFlags();
	static int Ticks;
	Ticks += 2;
	if (Ticks > 210)
		Ticks = 150;


	if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()) 
	{
		switch (SETTINGS::settings.aa_pitch_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		case 5:	cmd->viewangles.z -= 180;
			cmd->viewangles.x -= 15; break;
		}
		switch (SETTINGS::settings.aa_pitch1_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		}
		switch (SETTINGS::settings.aa_pitch2_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		}
		if (SETTINGS::settings.a_mode == 0)
		{
			switch (SETTINGS::settings.aa_real_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 179.000000f + SETTINGS::settings.aa_realadditive_val; break;
			case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
			case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
			case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
			case 6: {
				static int y2 = -SETTINGS::settings.spinangle;
				int speed = SETTINGS::settings.spinspeed;
				y2 += speed; if (y2 >= SETTINGS::settings.spinangle) y2 = -SETTINGS::settings.spinangle;
				SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive_val;
			} break;
			case 7: autoDirection(cmd);
			case 8: freestand(cmd, 0);
			case 9: // lby breaker by pasteware.team / gamesense.pub
			{
				if (kamaz_lby_update(cmd)) cmd->viewangles.y = local_player->GetLowerBodyYaw() + SETTINGS::settings.delta_val;//last_real + SETTINGS::settings.delta_val;
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= 90 + Ticks;
					last_real = cmd->viewangles.y;
				}
				else
				{
					cmd->viewangles.y += 90 - Ticks;
					last_real = cmd->viewangles.y;
				}
				break;
			}
			}
		}
	}
	if (SETTINGS::settings.a_mode == 1)
	{
		switch (SETTINGS::settings.aa_real1_type)
		{
		case 0: break;
		case 1: cmd->viewangles.y += 179.000000f + SETTINGS::settings.aa_realadditive_val; break;
		case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
		case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
		case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
		case 6: {
			static int y2 = -SETTINGS::settings.spinangle;
			int speed = SETTINGS::settings.spinspeed;
			y2 += speed; if (y2 >= SETTINGS::settings.spinangle) y2 = -SETTINGS::settings.spinangle;
			SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive_val;
		} break;
		case 7: autoDirection(cmd);
		case 8: freestand(cmd, 0);
		}
	}
	if (SETTINGS::settings.a_mode == 2)
	{
		switch (SETTINGS::settings.aa_real2_type)
		{
		case 0: break;
		case 1: cmd->viewangles.y += 179.000000f + SETTINGS::settings.aa_realadditive_val; break;
		case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
		case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
		case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
		case 6: {
			static int y2 = -SETTINGS::settings.spinangle;
			int speed = SETTINGS::settings.spinspeed;
			y2 += speed; if (y2 >= SETTINGS::settings.spinangle) y2 = -SETTINGS::settings.spinangle;
			SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive_val;
		} break;
		case 7: autoDirection(cmd);
		case 8: freestand(cmd, 0);
		}
	}

	switch (SETTINGS::settings.real3)
	{
	case 0: break;
	case 1: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;//type 1
	case 2: cmd->viewangles.y += 180 + ((rand() % 35) - (35 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
	case 3: cmd->viewangles.y += 180 + ((rand() % 58) - (58 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
	case 4: cmd->viewangles.y += 180 + ((rand() % 65) - (65 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
	case 5: cmd->viewangles.y += 180 + ((rand() % 80) - (80 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
	case 6: cmd->viewangles.y += 180 + ((rand() % 95) - (95 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;

	}

	if (SETTINGS::settings.desync_aa3)
	{

		float omi = rand() % 100;
		if (GLOBAL::should_send_packet)

		{
			float server_time = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;
			float time = TIME_TO_TICKS(server_time);

			while (time >= server_time)
				time = 0.f;

			if (omi < 50)
			{
				cmd->viewangles.y += detectdatwallbitch(-152 + UTILS::RandomFloat(0, 180));


			}
			else
			{
				cmd->viewangles.y += detectdatwallbitch(175 + UTILS::RandomFloat(0, 180));
			}
		}
	}
}





void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (SETTINGS::settings.anti_untrusted)
	{
		cmd->move.x = clamp(cmd->move.x, -450.f, 450.f);
		cmd->move.y = clamp(cmd->move.y, -450.f, 450.f);
		cmd->move.z = clamp(cmd->move.z, -320.f, 320.f);

		cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);
		cmd->viewangles.y = clamp(cmd->viewangles.y, -180.f, 180.f);
	}

	Vector real_viewangles;
	INTERFACES::Engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->move.x, cmd->move.y, cmd->move.z);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	MATH::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->move.x = cos(yaw) * speed;
	cmd->move.y = sin(yaw) * speed;

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->move.x *= -1;
}

Vector CAntiAim::fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd originalCMD)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

	auto viewangles = cmd->viewangles;
	auto movedata = originalCMD.move;
	viewangles.Normalize();

	if (!(local_player->GetFlags() & FL_ONGROUND) && viewangles.z != 0.f)
		movedata.y = 0.f;

	MATH::AngleVectors2(originalCMD.viewangles, &wish_forward, &wish_right, &wish_up);
	MATH::AngleVectors2(viewangles, &cmd_forward, &cmd_right, &cmd_up);

	auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

	Vector wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
		wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
		wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

	auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

	Vector cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
		cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
		cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

	auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

	Vector correct_movement;
	correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25
		+ (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28)
		+ (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
	correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25
		+ (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28)
		+ (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
	correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25
		+ (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28)
		+ (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

	correct_movement.x = clamp(correct_movement.x, -450.f, 450.f);
	correct_movement.y = clamp(correct_movement.y, -450.f, 450.f);
	correct_movement.z = clamp(correct_movement.z, -320.f, 320.f);

	return correct_movement;
}

CAntiAim* antiaim = new CAntiAim();














































































































































































































































































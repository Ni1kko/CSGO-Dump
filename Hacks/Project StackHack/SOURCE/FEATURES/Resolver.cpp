#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseAnimState.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/EventListener.h"
#include "../FEATURES/AutoWall.h"
#include "Backtracking.h"
#include "../SDK/ConVar.h"
#include "../UTILS/render.h"
#include "../DesyncCorrect.h"
#include "../SDK/CTrace.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Backtracking.h"
#include "../NewResolverh.h"

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / INTERFACES::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( INTERFACES::Globals->interval_per_tick *( t ) )
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))


#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

double randd() {
	return (double)rand() / ((double)RAND_MAX + 1);
}

float approach1(float cur, float target, float inc) {
	inc = abs(inc);

	if (cur < target)
		return min(cur + inc, target);
	if (cur > target)
		return max(cur - inc, target);

	return target;
}

float angle_difference1(float a, float b) {
	auto diff = desync->NormalizeYaw180(a - b);

	if (diff < 180)
		return diff;
	return diff - 360;
}

float approach_angle1(float cur, float target, float inc) {
	auto diff = angle_difference1(target, cur);
	return approach1(cur, cur + diff, inc);
}


inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

inline float N0rmalizePitch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}
float NormalizeYaw123(float yaw)
{


	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;

}

float GetLBYRotatedYaw123(float lby, float yaw)
{
	float delta = NormalizeYaw123(yaw - lby);
	if (fabs(delta) < 25.f)
		return lby;

	if (delta > 0.f)
		return yaw + 25.f;

	return yaw;
}

template <class T>
constexpr const T& Max(const T& x, const T& y)
{
	return (x < y) ? y : x;
}


void CResolver::DoCM()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	int right_damage = resolver->right_damage, left_damage = resolver->left_damage, back_damage = resolver->back_damage;
	int left_fraction = resolver->left_fraction, right_fraction = resolver->left_fraction, back_fraction = resolver->back_fraction;

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
	for (int i = 0; i < 64; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant())
			continue;

		

		const float height = 64;

		Vector direction_1, direction_2, direction_3;
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 90.f, 0.f), &direction_1);
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 90.f, 0.f), &direction_2);
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 180.f, 0.f), &direction_3);


		const auto left_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
		const auto right_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);
		const auto back_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_3 * 16.f);

		left_damage = autowall->CalculateDamage(latency_based_eye_pos, left_eye_pos, local_player, entity, 1).damage;

		right_damage = autowall->CalculateDamage(latency_based_eye_pos, right_eye_pos, local_player, entity, 1).damage;

		back_damage = autowall->CalculateDamage(latency_based_eye_pos, back_eye_pos, local_player, entity, 1).damage;

		SDK::Ray_t ray;
		SDK::trace_t trace;
		SDK::CTraceWorldOnly filter;

		ray.Init(left_eye_pos, latency_based_eye_pos);
		INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
		left_fraction = trace.flFraction;

		ray.Init(right_eye_pos, latency_based_eye_pos);
		INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
		right_fraction = trace.flFraction;

		ray.Init(back_eye_pos, latency_based_eye_pos);
		INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
		back_fraction = trace.flFraction;

		
	}
}

bool CResolver::AntiFreestanding(SDK::CBaseEntity* entity, float& yaw)
{
	
	int right_damage = resolver->right_damage, left_damage = resolver->left_damage, back_damage = resolver->back_damage;
	int left_fraction = resolver->left_fraction, right_fraction = resolver->right_fraction;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (local_player)
		return false;

	
	

	const float at_target_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
	
	if (left_damage <= 0 && right_damage <= 0)
	{
		if (left_fraction > right_fraction)
		{
			yaw = at_target_yaw + 90.f;
		}
		else
		{
			yaw = at_target_yaw - 90.f;
		}
		
	}
	else
	{
		if (left_damage > right_damage)
		{
			yaw = at_target_yaw + 90.f;
		}
		else
		{
			yaw = at_target_yaw - 90.f;
		}
	}

	return true;
}


void CResolver::OverrideYaw() // credits to ju$tin, ju$tin > freak
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());


	resolver->Overriding = -1;
	if (!SETTINGS::settings.overrideenable) return;
	if (!GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.overridekey))) return;

	int w, h, cx, cy;
	INTERFACES::Engine->GetScreenSize(w, h);
	cx = w / 2;
	cy = h / 2;
	Vector crosshair = Vector(cx, cy, 0);
	int bestEnt = -1;
	float bestFOV = 0;
	Vector bestHead2D;
	for (int i = 0; i <= INTERFACES::Engine->GetMaxClients(); i++)
	{
		SDK::CBaseEntity* player = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (player == nullptr) continue;
		if (player->GetHealth() <= 0) continue;
		if (player == local_player) continue;
		

		Vector headPos3D = player->GetBonePosition(8), headPos2D;

		if (!RENDER::WorldToScreen(headPos3D, headPos2D)) continue;

		float fov2D = crosshair.Dist(headPos2D);

		if (bestEnt == -1 || fov2D < bestFOV)
		{
			bestEnt = i;
			bestFOV = fov2D;
			bestHead2D = headPos2D;
		}

	}

	if (bestEnt > -1)
	{
		SDK::CBaseEntity* player = INTERFACES::ClientEntityList->GetClientEntity(bestEnt);
		int minX = cx - (w / 10), maxX = cx + (w / 10);

		if (bestHead2D.x < minX || bestHead2D.x > maxX) return; // something isnt quite right

		int totalWidth = maxX - minX;
		int playerX = bestHead2D.x - minX;
		int yawADD = -(((playerX * 360) / totalWidth) - 180);
		player->EasyEyeAngles()->yaw += yawADD;
	}
}


bool CResolver::AntiPener(SDK::CBaseEntity* entity, float &yaw, int damage_tolerance)
{


	CAutowall* autowall = nullptr;

	if (!(entity->GetFlags() & FL_ONGROUND))
		return false;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetLifeState() != 0)
		return false;


	if (!autowall) return false;

	std::vector<SDK::CBaseEntity*> enemies;

	const float height = 64;
	damage_tolerance = resolver->damage_tolerance;

	float leftdamage = 0.f, rightdamage = 0.f, backdamage = 0.f;


	std::vector<Vector> last_eye_positions;

	last_eye_positions.insert(last_eye_positions.begin(), local_player->GetVecOrigin() + local_player->GetViewOffset());
	if (last_eye_positions.size() > 128)
		last_eye_positions.pop_back();

	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return false;

	const int latency_ticks = TIME_TO_TICKS(nci->GetLatency(FLOW_OUTGOING));
	const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];
	float lpos = 0.f;

	Vector direction_1, direction_2, direction_3;
	MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 90.f, 0.f), &direction_1);
	MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 90.f, 0.f), &direction_2);
	MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 180.f, 0.f), &direction_3);

	const auto left_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
	const auto right_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);
	const auto back_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_3 * 16.f);

	leftdamage = autowall->CalculateDamage(latency_based_eye_pos, left_eye_pos, local_player, entity, 1).damage;
	rightdamage = autowall->CalculateDamage(latency_based_eye_pos, right_eye_pos, local_player, entity, 1).damage;
	backdamage = autowall->CalculateDamage(latency_based_eye_pos, back_eye_pos, local_player, entity, 1).damage;

	SDK::Ray_t ray;
	SDK::trace_t trace;
	SDK::CTraceWorldOnly filter;

	ray.Init(left_eye_pos, latency_based_eye_pos);
	INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
	resolver->left_fraction = trace.flFraction;

	ray.Init(right_eye_pos, latency_based_eye_pos);
	INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
	resolver->right_fraction = trace.flFraction;

	ray.Init(back_eye_pos, latency_based_eye_pos);
	INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
	resolver->back_fraction = trace.flFraction;



	int right_damage = rightdamage;
	int left_damage = leftdamage;
	int back_damage = backdamage;

	float at_target_yaw = UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y;
	const float right_yaw = at_target_yaw - 90.f;
	const float left_yaw = at_target_yaw + 90.f;
	auto head_position = entity->GetVecOrigin() + Vector(0, 0, 74.f);

	float calculated_yaw;

	/// Find the lowest fov enemy
	SDK::CBaseEntity* closest_enemy = nullptr;
	float lowest_fov = 360.f;
	for (int i = 0; i <= 64; i++)
	{
		auto ent = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!ent || ent->GetIsDormant() || ent->GetTeam() == entity->GetTeam() || ent->GetHealth() <= 0 || !ent->IsAlive())
			continue;

		const float current_fov = fabs(MATH::NormalizeYaw(UTILS::CalcAngle(entity->GetVecOrigin(), ent->GetVecOrigin()).y - at_target_yaw));
		if (current_fov < lowest_fov)
		{
			lowest_fov = current_fov;
			closest_enemy = ent;
		}

		enemies.push_back(ent);
	}

	if (closest_enemy == nullptr)
		return false;

	auto RotateAndExtendPosition = [](Vector position, float yaw, float distance) -> Vector
	{
		Vector direction;
		MATH::AngleVectors(Vector(0, yaw, 0), &direction);

		return position + (direction * distance);
	};

	auto right_head_position = RotateAndExtendPosition(head_position, right_yaw, 17.f);
	auto left_head_position = RotateAndExtendPosition(head_position, left_yaw, 17.f);

	auto CalcDamage = [autowall, entity, enemies](Vector point) -> int
	{
		int damage = 0;
		for (auto& enemy : enemies)
		{
			damage += Max<int>(autowall->CalculateDamage(enemy->GetVecOrigin() + Vector(0, 0, 64.f), point, enemy, entity, 1).damage,
				autowall->CalculateDamage(enemy->GetVecOrigin() + Vector(0, 0, 49.f), point, enemy, entity, 1).damage);
		}

		return damage;
	};


	auto RotateLBYAndYaw = [right_yaw, left_yaw, entity](int right_damage, int left_damage, float& yaw, bool prefect_angle = false) -> bool
	{
		bool prefer_right = right_damage < left_damage;

		yaw = prefer_right ? right_yaw : left_yaw;

		/// If not moving
		if (prefect_angle)
			yaw = GetLBYRotatedYaw123(entity->GetLowerBodyYaw(), yaw);

		return true;
	};

	if (left_damage >= damage_tolerance && right_damage >= damage_tolerance && back_damage >= damage_tolerance)
		return false;

	if (left_damage >= damage_tolerance && right_damage >= damage_tolerance && back_damage < damage_tolerance)
		calculated_yaw = at_target_yaw + 180.f;
	else if (right_damage == left_damage)
	{
		if (N0rmalizePitch(UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
			calculated_yaw = at_target_yaw + 180.f;
		else
		{
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

				auto end_pos = local_player->GetVecOrigin() + Vector(0, 0, 64.f);

				/// right position
				ray.Init(right_head_position, end_pos);
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
				distance_1 = (right_head_position - trace.end).Length();

				/// left position
				ray.Init(left_head_position, end_pos);
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
				distance_2 = (left_head_position - trace.end).Length();

				if (fabs(distance_1 - distance_2) > 15.f)
					RotateLBYAndYaw(distance_1, distance_2, calculated_yaw);
				else
					calculated_yaw = at_target_yaw + 180;
			}
			else
				RotateLBYAndYaw(right_damage, left_damage, calculated_yaw);
		}
	}
	else
	{
		if (N0rmalizePitch(UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
			calculated_yaw = at_target_yaw + 180.f;
		else
		{
			bool prefer_right = (right_damage < left_damage);
			calculated_yaw = prefer_right ? right_yaw : left_yaw;
		}
	}



	yaw = calculated_yaw;



	return true;
}



float max_desync_delta(SDK::CBaseEntity* player)
{
	auto animstate = player->GetAnimState();

	float duckammount = animstate->m_fDuckAmount;
	float speedfraction = max(0, min(animstate->m_flFeetSpeedForwardsOrSideWays, 1));

	float speedfactor = max(0, min(1, animstate->m_flFeetSpeedUnknownForwardOrSideways));

	float unk1 = ((animstate->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
	{
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	//unk3 = *(float *)(animstate + 0x334) * unk2;

	return RAD2DEG(unk2);
}














bool isfakingyaw(SDK::CBaseEntity* entity)
{
	if (!entity->GetChokedTicks())
		return false;
}



float desync_delta(SDK::CBaseEntity* entity)
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

bool high_velocity(SDK::CBaseEntity* entity)
{
	if (entity->GetVelocity().Length2D() > 215)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool low_velocity(SDK::CBaseEntity* entity)
{
	if (entity->GetVelocity().Length2D() < 215 && entity->GetVelocity().Length2D() > 1 && entity->GetFlags() & FL_ONGROUND)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool standing(SDK::CBaseEntity* entity)
{
	if (entity->GetVelocity().Length2D() < 1 && entity->GetFlags() & FL_ONGROUND)
	{
		return true;
	}
	else
	{
		return false;
	}
}






bool RealWillUpdate(SDK::CBaseEntity* pEnt)
{
	auto* AnimState = pEnt->GetAnimState();
	float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * pEnt->EasyEyeAngles()->yaw);
	float Delta = angle_difference1(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw);

	if (Delta <= maxYaw)
	{
		return false;
	}
	else
	{
		return true;
	}

}


bool nextlbyupdate(SDK::CBaseEntity* entity)
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



bool IsBreakingLby(SDK::CBaseEntity* player, SDK::CAnimationLayer* layer)
{
	for (int i = 0; i < 15; i++)
	{
		const int activity = player->GetSequenceActivity(layer[i].m_nSequence);
		if (activity == 979)
		{
			return true;
		}
	}
	return false;
}






float penis()
{
	SDK::CBaseEntity* player = nullptr;
	auto animstate = player->GetAnimState();

	float duckammount = animstate->m_fDuckAmount;
	float speedfraction = max(0, min(animstate->m_flFeetSpeedForwardsOrSideWays, 1));

	float speedfactor = max(0, min(1, animstate->m_flFeetSpeedUnknownForwardOrSideways));

	float unk1 = ((animstate->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
	{
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	//unk3 = *(float *)(animstate + 0x334) * unk2;

	return RAD2DEG(unk2);
}

void CResolver::AACMain()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	int right_damage = resolver->dmg1, left_damage = resolver->dmg2;
	

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
	for (int i = 0; i < 64; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant())
			continue;



		const float height = 64;

		Vector direction_1, direction_2, direction_3;
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 58, 0.f), &direction_1);
		MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 58, 0.f), &direction_2);
		


		const auto left_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
		const auto right_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);
		

		left_damage = autowall->CalculateDamage(latency_based_eye_pos, left_eye_pos, local_player, entity, HITGROUP_HEAD).damage;

		right_damage = autowall->CalculateDamage(latency_based_eye_pos, right_eye_pos, local_player, entity, HITGROUP_HEAD).damage;


	}
}

void CResolver::VisualResolver(SDK::CBaseEntity* entity)
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

	if (entity == nullptr) return;
	if (!SETTINGS::settings.visual_resolver) return;

	const int shots_missed = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()]; // getting missed shots



	float* PosParams = (float*)((uintptr_t)entity + 0x2774);
	bool update = false;
	bool shot = false;
	auto Local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	float at_target_yaw = UTILS::CalcAngle(entity->GetVecOrigin(), Local->GetVecOrigin()).y; // self explanatory


	static bool jittering[65];

	auto* AnimState = entity->GetAnimState();
	if (!AnimState || !entity->GetAnimOverlays() || !PosParams || entity->GetHealth() <= 0)
		return;

	auto CanHitReal = [&Local]() -> bool // pro lambda
	{
		auto* AnimState = Local->GetAnimState();
		float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * AnimState->m_flEyeYaw);
		float Delta = angle_difference1(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw);

		if (Delta <= maxYaw)
		{
			return false;
		}
		else
		{
			return true;
		}
	};
	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	if (CanHitReal()) return;

	IsYoinks[entity->GetIndex()] = false;

	if (storedSimtime[entity->GetIndex()] != entity->GetSimTime())
	{
		jittering[entity->GetIndex()] = false;
		entity->ClientAnimations(true);
		entity->UpdateClientSideAnimation();

		memcpy(StoredPosParams[entity->GetIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[entity->GetIndex()], entity->GetAnimOverlays(), (sizeof(SDK::CAnimationLayer) * 15));

		oldGoalfeetYaw[entity->GetIndex()] = AnimState->m_flGoalFeetYaw;

		if (entity->GetWeapon2() && !entity->IsKnifeorNade())
		{
			if (ShotTime[entity->GetIndex()] != entity->GetWeapon2()->GetLastShotTime())
			{
				shot = true;
				ShotTime[entity->GetIndex()] = entity->GetWeapon2()->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[entity->GetIndex()] = 0.f;
		}

		StoredAnimState[entity->GetIndex()] = AnimState;


		oldSimtime[entity->GetIndex()] = storedSimtime[entity->GetIndex()];

		//AnimState->m_flUnknownFraction = 0;

		storedSimtime[entity->GetIndex()] = entity->GetSimTime();

		update = true;

	}

	entity->ClientAnimations(false);

	AnimState = StoredAnimState[entity->GetIndex()];

	memcpy((void*)PosParams, &StoredPosParams[entity->GetIndex()], (sizeof(float) * 24));
	memcpy(entity->GetAnimOverlays(), StoredLayers[entity->GetIndex()], (sizeof(SDK::CAnimationLayer) * 15));

	entity->SetAbsAngles(Vector(0, oldGoalfeetYaw[entity->GetIndex()], 0));

	*reinterpret_cast<int*>(uintptr_t(entity) + 0xA30) = INTERFACES::Globals->framecount;
	*reinterpret_cast<int*>(uintptr_t(entity) + 0xA28) = 0;
}

float MaxRotation(SDK::CBaseEntity* entity)
{
	auto* animstate = entity->GetAnimState();
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

bool find_layer(SDK::CBaseEntity* entity, int act, SDK::CAnimationLayer *set)
{
	for (int i = 0; i < 13; i++)
	{
		SDK::CAnimationLayer layer = entity->GetAnimOverlay(i);
		const int activity = entity->GetSequenceActivity(layer.m_nSequence);
		if (activity == act) {
			*set = layer;
			return true;
		}
	}
	return false;
}

bool IsAdjustingBalance(SDK::CBaseEntity* player, SDK::CAnimationLayer *layer)
{
	for (int i = 0; i < 15; i++)
	{
		const int activity = player->GetSequenceActivity(layer[i].m_nSequence);
		if (activity == 979)
		{
			return true;
		}
	}
	return false;
}

/*class fix_animations : public singleton< fix_animations > 
{
private:
	animation_info info[65];
public:
	void fix_animation_data() {
		for (int j = 1; j <= 64; j++) {
			auto fix_goal_feet_yaw = [](float rotation, float invertedrotation, float yawfeetdelta, float yaw, SDK::CBaseAnimState* state) 
			{
				if (yawfeetdelta < rotation) {
					if (invertedrotation > yawfeetdelta)
						*(float*)(uintptr_t(state) + 0x80) = invertedrotation + yaw;
				}
				else
					*(float*)(uintptr_t(state) + 0x80) = yaw - rotation;
			};
			 
			auto get_rotation = [&](int rotation_type, C_AnimState* state) {
				float v43 = *(float*)((uintptr_t)state + 0xA4);
				float v54 = max(0, min(*reinterpret_cast<float*>((uintptr_t)state + 0xF8), 1));
				float v55 = max(0, min(1, *reinterpret_cast<float*>((uintptr_t)state + 0xFC)));

				float v56;
				v56 = ((*reinterpret_cast<float*>((uintptr_t)state + 0x11C) * -0.30000001) - 0.19999999) * v54;
				if (v43 > 0)
					v56 += ((v43 * v55) * (0.5 - v56));

				v56 = *(float*)((uintptr_t)state + rotation_type) * v56;
				return v56;
			};
			auto& rec = info[j];
			auto target = g_pEntityList->GetClientEntity(j);

			if (!target || target->GetHealth() <= 0)
				continue;

			float spawn_time = *(float*)((uintptr_t)target + 0xA290);

			bool  allocate = (rec.server_state == nullptr),
				change = (!allocate) && (target->GetRefEHandle() != rec.ref_handle),
				reset = (!allocate && !change) && (spawn_time != rec.spawn_time);

			if (reset) {
				reset_state(rec.server_state);
				rec.spawn_time = spawn_time;
			}

			if (allocate || change) {
				auto state = (C_AnimState*)(g_pMemAlloc->Alloc(sizeof(C_AnimState)));
				if (state != nullptr)
					create_state(state, target);

				rec.ref_handle = target->GetRefEHandle();
				rec.spawn_time = spawn_time;

				rec.server_state = state;
			}

			if (!rec.server_state)
				continue;

			float inverted = get_rotation(0x2B4, rec.server_state);
			float max = get_rotation(0x2B0, rec.server_state);
			float yawfeetdelta = rec.server_state->m_flEyeYaw - rec.server_state->m_flGoalFeetYaw;
			float yaw = target->GetEyeAngles().y;

			if (!(target->GetNumberOfTicksChoked() > 1) && allocate) {
				*reinterpret_cast<float*>((uintptr_t)rec.server_state + 292) = 0.f;

				for (int j = 0; j < 24; j++)
					rec.poses[j] = target->GetPoseParamsModifiable()[j];
				std::memcpy(rec.backup_layers, target->AnimOverlays(), sizeof(AnimationLayer) * 15);

				fix_goal_feet_yaw(max, inverted, yawfeetdelta, yaw, rec.server_state);
				target->GetEyeAnglesPointer()->y = rec.server_state->m_flGoalFeetYaw;

				rec.angle = target->GetEyeAngles();
				update_state(rec.server_state, target->GetEyeAngles());

				target->SetupBones(rec.matrix, 128, 0x7FF00, target->GetSimulationTime());

				std::memcpy(target->AnimOverlays(), rec.backup_layers, sizeof(AnimationLayer) * 15);
				for (int j = 0; j < 24; j++)
					target->GetPoseParamsModifiable()[j] = rec.poses[j];
			}
			else {
				target->SetEyeAngles(rec.angle);
				target->UpdateClientAnimation();
				*(C_AnimState**)((DWORD)target + 0x3900) = rec.server_state;
			}
		}
	}
	void create_state(C_AnimState* state, C_BaseEntity* player) {
		using fn = void(__thiscall*)(C_AnimState*, C_BaseEntity*);
		static auto ret = reinterpret_cast<fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));

		ret(state, player);
	}
	void update_state(C_AnimState* state, Vector ang) {
		using fn = void(__vectorcall*)(void *, void *, float, float, float, void *);
		static auto ret = reinterpret_cast<fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));

		if (!ret)
			return;

		ret(state, NULL, NULL, ang.y, ang.x, NULL);
	}
	void reset_state(C_AnimState* state) {
		using fn = void(__thiscall*)(C_AnimState*);
		static auto ret = reinterpret_cast<fn>(Utils::FindSignature("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1"));

		ret(state);
	}*/

/*float BalanceDumpedFromPolak(int type) {

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
}*/


void CResolver::AntiAimCorrection(SDK::CBaseEntity* entity)
{

	if (!SETTINGS::settings.apply_freestand) return; // resolver mode whatever :(
	// i give up

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()); // get the local player
	float at_target_yaw = UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y; // self explanatory
	shots_missed[entity->GetIndex()] = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()];

	resolver->resolved_angles.y = resolver->networked_angles.y;

	float animTime = entity->GetSimTime() + INTERFACES::Globals->interval_per_tick;
	float next_predicted_lby_update;
	float last_balance_adjust_trigger_time;
	bool did_lby_flick;

	bool is_balance_adjust = false, is_balance_adjust_triggered = false;
	SDK::CAnimationLayer anim_layers[15];
	SDK::CAnimationLayer layer = entity->GetAnimOverlay(0);
	SDK::CAnimationLayer prev = resolver->prev_layer;


	static float oldGoalfeetYaw[65];

	auto* AnimState = entity->GetAnimState();
	oldGoalfeetYaw[entity->GetIndex()] = AnimState->m_flGoalFeetYaw;

		float* PosParams = (float*)((uintptr_t)entity + 0x2774);

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	for (int j = 1; j <= 64; j++)
	{
		auto fix_goal_feet_yaw = [](float rotation, float invertedrotation, float yawfeetdelta, float yaw, SDK::CBaseAnimState* state)
		{
			if (yawfeetdelta < rotation)
			{
				if (invertedrotation > yawfeetdelta)
					*(float*)(uintptr_t(state) + 0x80) = invertedrotation + yaw;
			}
			else
				*(float*)(uintptr_t(state) + 0x80) = yaw - rotation;
		};

		auto get_rotation = [&](int rotation_type, SDK::CBaseAnimState* state) {
			float v43 = *(float*)((uintptr_t)state + 0xA4);
			float v54 = max(0, min(*reinterpret_cast<float*>((uintptr_t)state + 0xF8), 1));
			float v55 = max(0, min(1, *reinterpret_cast<float*>((uintptr_t)state + 0xFC)));

			float v56;
			v56 = ((*reinterpret_cast<float*>((uintptr_t)state + 0x11C) * -0.30000001) - 0.19999999) * v54;
			if (v43 > 0)
				v56 += ((v43 * v55) * (0.5 - v56));

			v56 = *(float*)((uintptr_t)state + rotation_type) * v56;
			return v56;
		};
		Vector angle;
		float inverted = get_rotation(0x2B4, AnimState);
		float max = get_rotation(0x2B0, AnimState);
		float yawfeetdelta = AnimState->m_flEyeYaw - AnimState->m_flGoalFeetYaw;
		float yaw = entity->GetEyeAngles().y;

		fix_goal_feet_yaw(max, inverted, yawfeetdelta, yaw, AnimState);
		entity->EasyEyeAngles()->yaw = AnimState->m_flGoalFeetYaw;
		angle = entity->GetEyeAngles();


	}

	
	
	auto fYaw = AnimState->m_flGoalFeetYaw;
	const float height = 64;
	auto penis_delta =  fYaw - MaxRotation(entity);
	auto penis_delta2 = fYaw + MaxRotation(entity);


	
	
	auto CanHitReal = [&local_player]() -> bool // pro lambda
	{
		auto* AnimState = local_player->GetAnimState();
		float maxYaw = ((*(float*)((uintptr_t)AnimState + 0x334)) * AnimState->m_flEyeYaw);
		float Delta = angle_difference1(AnimState->m_flEyeYaw, AnimState->m_flGoalFeetYaw);

		if (Delta <= maxYaw)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	auto IsAdjust = [&entity]() -> bool
	{
		for (size_t i = 0; i < entity->GetNumAnimOverlays(); i++)
		{
			auto layer = entity->GetAnimOverlays()[i];

			if (entity->GetSequenceActivity(layer.m_nSequence) == 979)
			{
				if (layer.m_flPrevCycle > 0.92f && layer.m_flCycle > 0.92f && layer.m_flWeight == 0.f)
				{
					return true;
				}
			}
		}
		return false;
	};


	/*if (entity->GetFlags() & FL_ONGROUND) // if they are on ground
	{
		if (shots_missed < 2) // so far we have missed less than 2 shots
		{
			if (entity->GetVelocity().Length2D() <= 200)
			{
				if (resolver->dmg1 > resolver->dmg2) // decide what side does more damage
				{
					switch (shots_missed % 3)
					{
					case 0:
						fYaw = penis_delta; break;
					case 1:
						fYaw = penis_delta2; break;
					case 2:
						fYaw = entity->EasyEyeAngles()->yaw; break; // we dont need more than 2 ( 3 cases )  because we made a check ( if (shots_missed < 2) ) so if it goes over 2, we just brute like big boys
					}

				}
				else
				{
					switch (shots_missed % 3)
					{
					case 0:
						fYaw = penis_delta2; break;
					case 1:
						fYaw = penis_delta; break;
					case 2:
						fYaw = entity->EasyEyeAngles()->yaw; break;
					}

				}
			}
			else if (entity->GetVelocity().Length2D() >= 200)
			{
				switch (shots_missed % 3) // they be movin real fast so desync = 29
				{
				case 0:
					fYaw += MaxRotation(entity); break;
				case 1:
					fYaw -= MaxRotation(entity); break;
				case 2:
					fYaw -= entity->GetAbsAngles().y; break;
				}
			}
			else if (IsAdjust())
			{
				switch (shots_missed % 2)
				{
				case 0:
					fYaw -= ((MaxRotation(entity) - 30.0f) * 120); break; forcing that will 100% resolve balance yikEs
				case 1:
					fYaw += ((MaxRotation(entity) + 30.0f) * 120); break;
				default:
					fYaw -= ((MaxRotation(entity) - 30.0f) * 120); break;
				}
			}
		
			
		}
		else
		{	
				

			switch (shots_missed % 2)
			{
			case 0:
				fYaw -= MaxRotation(entity); break; 
			case 1:
				fYaw += MaxRotation(entity); break;
			default:
				fYaw -= MaxRotation(entity); break;
			}
		}
	}
	else
	{
		switch (shots_missed) // they be in air hAeHAE
		{
		case 0:
			fYaw += MaxRotation(entity); break;
		case 1:
			fYaw -= MaxRotation(entity); break;
		}
	}*/

		
	/// bad and old :(
	//entity->SetLowerBodyYaw(fYaw);



}


void create_state(SDK::CBaseAnimState* state, SDK::CBaseEntity* player) {
	using fn = void(__thiscall*)(SDK::CBaseAnimState*, SDK::CBaseEntity*);
	static auto ret = reinterpret_cast<fn>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));

	ret(state, player);
}
void update_state(SDK::CBaseAnimState* state, Vector ang) {
	using fn = void(__vectorcall*)(void *, void *, float, float, float, void *);
	static auto ret = reinterpret_cast<fn>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));

	if (!ret)
		return;

	ret(state, NULL, NULL, ang.y, ang.x, NULL);
}
void reset_state(SDK::CBaseAnimState* state) {
	using fn = void(__thiscall*)(SDK::CBaseAnimState*);
	static auto ret = reinterpret_cast<fn>(UTILS::FindSignature("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1"));

	ret(state);
}
CResolver* resolver = new CResolver();
























































































































































































































































































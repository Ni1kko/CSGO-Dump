#pragma once
#include "includes.h"
#include "SDK\GameEvents.h"

struct ResolverYawRecord
{
	float one = 0.f;
	float two = 0.f;
	float delta = 0.f;
	float average = 0.f;
};

class ShotBacktrackListener : public  SDK::IGameEventListener
{
public:
	ShotBacktrackListener(std::vector<const char*> events)
	{
		for (auto& it : events)
			INTERFACES::GameEventManager->AddListener(this, it, false);
	}
	~ShotBacktrackListener()
	{
		INTERFACES::GameEventManager->RemoveListener(this);
	}
	void FireGameEvent(SDK::IGameEvent * event);
/*	int GetEventDebugID(void)
	{
		return EVENT_DEBUG_ID_INIT;
	}*/

	float LastShootTime[64] = { 0.f };
};

class Resolver
{
public:


	int   MissedShots[65];

	float iResolve[65];

	bool SawLBYUpdate[64] = { false };
	float HitDelta[64] = { std::numeric_limits<float>::infinity() };
	QAngle Override[64] = { QAngle(std::numeric_limits<float>::infinity(), 0, 0) };
	float RawYaw[64];
	float LastResolve[64];
	float LastSimTime[64];
	int LastMode[64];
	int LastChoke[64];
	float NextLBY[64];
	float LastLBY[64];

	float GetMaxDesync(SDK::CBaseEntity* player);

	std::unique_ptr<ShotBacktrackListener> EventListner;

	
	ResolverYawRecord records[64];
	void ResolveYaw(SDK::CBaseEntity &player);
	void LBYManipulation(SDK::CBaseEntity * Entity);
	void AutoWallResolver(SDK::CBaseEntity * player);
	void ResolvePitch(SDK::CBaseEntity &player);

	float NormalizePitch(float pitch);
	float NormalizeYaw(float yaw);

	float ComputeBodyPitch(float viewangle);
	bool IsFakeAngle(float viewangle, float goal);
	//int index = -1;
	float FindSmallestFake(float goal, int skip);

	int ReverseFreestanding(SDK::CBaseEntity player);
private:

};

/*
void NewResolver::DoCM()
{
auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
if (!local_player || local_player->GetHealth() <= 0)
return;

int right_damage = resolve->right_damage, left_damage = resolve->left_damage;
int fright_damage = resolve->fright_damage, fleft_damage = resolve->fleft_damage;
int bright_damage = resolve->bright_damage, bleft_damage = resolve->bleft_damage;
int forward_damage = resolve->forward_damage, back_damage = resolve->back_damage;

int left_fraction = resolve->left_fraction, right_fraction = resolve->left_fraction;

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

Vector direction_1, direction_2, direction_3, direction_4, direction_5, direction_6, direction_7, direction_8; // 360 / 8 = 45
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 90.f, 0.f), &direction_1);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 90.f, 0.f), &direction_2);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 180.f, 0.f), &direction_3);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 180.f, 0.f), &direction_4);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y + 45.f, 0.f), &direction_5);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 45.f, 0.f), &direction_6);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 135.f, 0.f), &direction_7);
MATH::AngleVectors(Vector(0.f, UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - 225.f, 0.f), &direction_8);


const auto left_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
const auto right_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);
const auto back_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_3 * 16.f);
const auto forward_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_4 * 16.f);
const auto fleft_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_5 * 16.f);
const auto fright_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_6 * 16.f);
const auto bleft_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_7 * 16.f);
const auto bright_eye_pos = entity->GetVecOrigin() + Vector(0, 0, height) + (direction_8 * 16.f);

left_damage = autowall->CalculateDamage(latency_based_eye_pos, left_eye_pos, local_player, entity, 1).damage;
right_damage = autowall->CalculateDamage(latency_based_eye_pos, right_eye_pos, local_player, entity, 1).damage;
back_damage = autowall->CalculateDamage(latency_based_eye_pos, back_eye_pos, local_player, entity, 1).damage;
forward_damage = autowall->CalculateDamage(latency_based_eye_pos, forward_eye_pos, local_player, entity, 1).damage;
fleft_damage = autowall->CalculateDamage(latency_based_eye_pos, fleft_eye_pos, local_player, entity, 1).damage;
fright_damage = autowall->CalculateDamage(latency_based_eye_pos, fright_eye_pos, local_player, entity, 1).damage;
bright_damage = autowall->CalculateDamage(latency_based_eye_pos, bright_eye_pos, local_player, entity, 1).damage;
bleft_damage = autowall->CalculateDamage(latency_based_eye_pos, bleft_eye_pos, local_player, entity, 1).damage;


SDK::Ray_t ray;
SDK::trace_t trace;
SDK::CTraceWorldOnly filter;

ray.Init(left_eye_pos, latency_based_eye_pos);
INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
left_fraction = trace.flFraction;

ray.Init(right_eye_pos, latency_based_eye_pos);
INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
right_fraction = trace.flFraction;


}
}

bool NewResolver::AntiFreestanding(SDK::CBaseEntity* entity, float& yaw)
{

int right_damage = resolve->right_damage, left_damage = resolve->left_damage;
int fright_damage = resolve->fright_damage, fleft_damage = resolve->fleft_damage;
int bright_damage = resolve->bright_damage, bleft_damage = resolve->bleft_damage;
int forward_damage = resolve->forward_damage, back_damage = resolve->back_damage;
int right_fraction = resolve->right_fraction, left_fraction = resolve->left_fraction;
const int shots_missed = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()];
int ms = shots_missed;

auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());


if (!(entity->GetFlags() & FL_ONGROUND))
return false;


const float at_target_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
/*if (left_damage <= 0 && right_damage <= 0 && fleft_damage <= 0 && fright_damage <= 0 && bleft_damage <= 0 && bright_damage <= 0 && back_damage <= 0 && forward_damage <= 0)
{
if (right_fraction < left_fraction)
{
yaw = at_target_yaw + 90.f;
}
else
{
yaw = at_target_yaw - 90.f;
}
}*/
//else
//{
/*if (left_damage > right_damage && left_damage > fleft_damage && left_damage > fright_damage && left_damage > bleft_damage && left_damage > bright_damage && left_damage > back_damage && left_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw + 90.f; break;
	case 1:
		yaw = at_target_yaw + 45.f; break;
	case 2:
		yaw = at_target_yaw - 90.f; break;
	}
}
else if (right_damage > left_damage && right_damage > fleft_damage && right_damage > fright_damage && right_damage > bleft_damage && right_damage > bright_damage && right_damage > back_damage && right_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw - 90.f; break;
	case 1:
		yaw = at_target_yaw - 45.f; break;
	case 2:
		yaw = at_target_yaw + 90.f; break;
	}
}
else if (fright_damage > fleft_damage && fright_damage > left_damage && fright_damage > right_damage && fright_damage > bleft_damage && fright_damage > bright_damage && fright_damage > back_damage && fright_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw - 45.f; break;
	case 1:
		yaw = at_target_yaw - 90.f; break;
	case 2:
		yaw = at_target_yaw + 45.f; break;
	}
}
else if (fright_damage < fleft_damage && fleft_damage > left_damage && fleft_damage > right_damage && fleft_damage > bleft_damage && fleft_damage > bright_damage && fleft_damage > back_damage && fleft_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw + 45.f; break;
	case 1:
		yaw = at_target_yaw + 90.f; break;
	case 2:
		yaw = at_target_yaw - 45.f; break;
	}
}
else if (bright_damage > bleft_damage && bright_damage > left_damage && bright_damage > right_damage && bright_damage > fleft_damage && bright_damage > fright_damage && bright_damage > back_damage && bright_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw - 135.f; break;
	case 1:
		yaw = at_target_yaw - 180.f; break;
	case 2:
		yaw = at_target_yaw - 225.f; break;
	}
}
else if (bright_damage < bleft_damage && bleft_damage > left_damage && bleft_damage > right_damage && bleft_damage > fleft_damage && bleft_damage > fright_damage && bleft_damage > back_damage && bleft_damage > forward_damage)
{
	switch (ms % 2)
	{
	case 0:
		yaw = at_target_yaw - 225.f; break;
	case 1:
		yaw = at_target_yaw - 180.f; break;
	case 2:
		yaw = at_target_yaw - 135.f; break;
	}
}
else if (back_damage > forward_damage && back_damage > left_damage && back_damage > right_damage && back_damage > fleft_damage && back_damage > fright_damage && back_damage > bright_damage && back_damage > bleft_damage)
{
	switch (ms % 3)
	{
	case 0:
		yaw = at_target_yaw - 180.f; break;
	case 1:
		yaw = at_target_yaw - 135.f; break;
	case 2:
		yaw = at_target_yaw - 225.f; break;
	case 3:
		yaw = at_target_yaw + 180.f; break;

	}
}
else if (forward_damage > back_damage && forward_damage > left_damage && forward_damage > right_damage && forward_damage > fleft_damage && forward_damage > fright_damage && forward_damage > bright_damage && forward_damage > bleft_damage)
{
	switch (ms % 3)
	{
	case 0:
		yaw = at_target_yaw + 180.f; break;
	case 1:
		yaw = at_target_yaw - 225.f; break;
	case 2:
		yaw = at_target_yaw - 135.f; break;
	case 3:
		yaw = at_target_yaw - 180.f; break;

	}
}


// boi that took a while
//}


return true;
}
*/
extern Resolver g_Resolver;
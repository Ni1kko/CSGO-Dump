#include "includes.h"
#include "UTILS/interfaces.h"
#include "SDK/IEngine.h"
#include "SDK/CUserCmd.h"
#include "SDK/CGlobalVars.h"
#include "SDK/IEngine.h"
#include "SDK/CBaseEntity.h"
#include "SDK/CBaseAnimState.h"
#include "FEATURES/Resolver.h"
#include "FEATURES/AutoWall.h"
#include "SDK/ConVar.h"
#include "UTILS/render.h"
#include "SDK/CTrace.h"
#include "FEATURES/Aimbot.h"
#include "FEATURES/Backtracking.h"
#include "FEATURES\AntiAim.h"
#include "SDK\GameEvents.h"
#include "NewResolverh.h"

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / INTERFACES::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( INTERFACES::Globals->interval_per_tick *( t ) )
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))


#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

double randd3() {
	return (double)rand() / ((double)RAND_MAX + 1);
}

inline float RandomFloat2(float min, float max)
{
	static auto fn = (decltype(&RandomFloat2))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

inline float N0rmalizePitch1(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}
float NormalizeYaw5(float yaw)
{


	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;

}

float GetLBYRotatedYaw5(float lby, float yaw)
{
	float delta = NormalizeYaw5(yaw - lby);
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



float Resolver::GetMaxDesync(SDK::CBaseEntity* player)
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

bool TriggeringBalanceAdjust(SDK::CBaseEntity* player, SDK::CAnimationLayer* layer)
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

void Resolver::ResolveYaw(SDK::CBaseEntity &player)
{
	SDK::CBaseEntity* pplayer = nullptr;
	SDK::CAnimationLayer* anim = nullptr;
	std::vector<std::pair<std::string, CColor>> stored_info;


	if (!pplayer) return;


	int index = player.GetIndex();


	QAngle targetang = QAngle(player.GetEyeAngles().x, RawYaw[index], 0); // x is pitch u tard
	int reverse = ReverseFreestanding(player);

	auto animstate = player.GetAnimState();
	if (!animstate) return;

	auto fYaw = animstate->m_flGoalFeetYaw;

	//int index = player.GetIndex();
	const int Missedshots = shots_fired[player.GetIndex()] - shots_hit[player.GetIndex()];

	float desyncMax = GetMaxDesync(pplayer);
	bool LBYUpdated = abs(NextLBY[index] - player.GetSimTime()) > 2.f || NextLBY[index] < player.GetSimTime() || LastLBY[index] != player.GetLowerBodyYaw() || player.GetVelocity().Length2D() > 0.1f;

	if (LBYUpdated)
	{
		if (pplayer->GetVelocity().Length2D() > 0.1f)
		{
			SawLBYUpdate[index] = true;
			NextLBY[index] = player.GetSimTime() + 0.22f;
		}
		else if (LastLBY[index] != player.GetLowerBodyYaw())
		{
			SawLBYUpdate[index] = true;
			NextLBY[index] = player.GetSimTime() + 1.1f;
		}
		else
		{
			NextLBY[index] = player.GetSimTime() + 1.1f;
		}
		LastLBY[index] = player.GetLowerBodyYaw();
	}

	if (records[index].one != RawYaw[index])
	{
		records[index].two = records[index].one;
		records[index].one = RawYaw[index];
		records[index].delta = records[index].one - records[index].two;
		if (abs(records[index].delta) < desyncMax)
		{
			records[index].average += RawYaw[index];
			records[index].average /= 2;
		}
	}

	if (Override[index].pitch != std::numeric_limits<float>::infinity())
	{
		fYaw = Override[index].yaw;
		iResolve[index] = 7;
		Override[index].pitch = std::numeric_limits<float>::infinity();
	}
	else if (LastSimTime[index] == player.GetSimTime())                                                                                            //choked, use last data
	{
		iResolve[index] = LastMode[index];
		LastChoke[index] = TIME_TO_TICKS(player.GetAnimationTime() - player.GetSimTime());
		fYaw = LastResolve[index];
	}
	/*else if (abs(NormalizeYaw(RawYaw[index] - player.GetLowerBodyYaw())) < desyncMax && pplayer->GetVelocity().Length2D() < 0.1f && Missedshots < 6)    //no lby break
	{
		targetang.yaw = player.GetLowerBodyYaw();
		iResolve[index] = 6;
	}
	else if (LBYUpdated && SawLBYUpdate[index] && pplayer->GetVelocity().Length2D() < 0.1f && Missedshots < 5)                                                       //lby predict
	{
		targetang.yaw = player.GetLowerBodyYaw();
		iResolve[index] = 5; lby = bad
	}*/
	else if (TIME_TO_TICKS(player.GetAnimationTime() - player.GetSimTime()) < 1 && LastChoke[index] < 1)                                   //not choking
	{
		iResolve[index] = 4;
	}
	else if (desyncMax < 30.f)                                                                                                                                  //low delta
	{
		iResolve[index] = 3;
	}
	else if (HitDelta[index] != std::numeric_limits<float>::infinity())                                                                                         //bruteforce hit
	{
		targetang.yaw += HitDelta[index];
		iResolve[index] = 2;

	}
	else if (reverse && Missedshots < 4)                                                                                                                 //reverse freestanding
	{
		switch (Missedshots % 3)
		{
		case 0: fYaw += (reverse == 1) ? desyncMax : -desyncMax; break;
		case 1: fYaw += (reverse == 1) ? desyncMax / 2 : -desyncMax / 2; break;
		case 2: break;
		}
		iResolve[index] = 1;

	}
	else if (Missedshots < 15 && abs(NormalizeYaw(records[index].delta)) < desyncMax * 2 && abs(NormalizeYaw(records[index].delta)) > desyncMax / 10 && !(Missedshots % 3))       //"logic" bruteforce
	{
		switch (Missedshots % 5)
		{
		case 0: fYaw += records[index].delta; break;
		case 1: fYaw -= records[index].delta; break;
		case 2: break;
		case 3: fYaw += records[index].average; break;
		case 4: fYaw -= records[index].average; break;
		}
	    iResolve[index] = -1;

	}
	else
	{
		switch (Missedshots % 5)                                                                                                                         //bruteforce
		{
		case 0: fYaw += desyncMax; break;
		case 1: fYaw -= desyncMax; break;
		case 2: fYaw += 0.f; break;
		case 3: fYaw += desyncMax / 2; break;
		case 4: fYaw -= desyncMax / 2; break;
		}
		iResolve[index] = 0;
	}

	NormalizeYaw(fYaw);
	player.SetLowerBodyYaw(fYaw);
	LastResolve[index] = targetang.yaw;
	LastMode[index] = iResolve[index];
	LastSimTime[index] = player.GetSimTime();
	LastChoke[index] = TIME_TO_TICKS(player.GetAnimationTime() - player.GetSimTime());
}



void Resolver::LBYManipulation(SDK::CBaseEntity* Entity)
{
	int index = Entity->GetIndex();
	static Vector Angle;
	Angle = Entity->GetEyeAngles();
	SDK::CAnimationLayer* anim = nullptr;

	static float moving_lby[65];
	static float moving_sim[65];
	static float stored_lby[65];
	static float old_lby[65];
	static float lby_delta[65];
	static float predicted_yaw[65];
	static bool lby_changes[65];
	static int shots_check[65];
	static float angle_brute[65];
	static float AtTargetAngle;
	static float FixPitch;
	static float FixPitch2;
	static bool HitNS[65];
	static Vector StoredAngle[65];
	static Vector Hitstored[65];
	static int StoredShots[65];
	static int HitShots[65];
	static int HitShotsStored[65];

	int Missedshots[65];
	Missedshots[Entity->GetIndex()] = shots_fired[Entity->GetIndex()] - shots_hit[Entity->GetIndex()];
	if (stored_lby[index] != Entity->GetLowerBodyYaw())
	{
		old_lby[index] = stored_lby[index];
		lby_changes[index] = true;
		stored_lby[index] = Entity->GetLowerBodyYaw();
	}

	lby_delta[index] = NormalizeYaw(stored_lby[index] - old_lby[index]);

	if (lby_changes[index])
	{
		if ((Entity->GetSimTime() - moving_sim[index]) > .22f)
			predicted_yaw[index] = lby_delta[index];

		lby_changes[index] = false;
	}

	if (stored_lby[index] != Entity->GetLowerBodyYaw())
	{
		old_lby[index] = stored_lby[index];
		Angle.y = Entity->GetLowerBodyYaw();
		lby_changes[index] = true;
		stored_lby[index] = Entity->GetLowerBodyYaw();
		//g_Globals->ResolverStage[index] = "LBY UPDATE";
	}
	else if (abs(Entity->GetVelocity().Length2D()) > 29.f && (Entity->GetFlags() & FL_ONGROUND))
	{
		Angle.y = Entity->GetLowerBodyYaw();
		moving_lby[index] = Entity->GetLowerBodyYaw();
		moving_sim[index] = Entity->GetSimTime();
		lby_changes[index] = false;
		predicted_yaw[index] = 0;
		Missedshots[index] = 0;
		angle_brute[index] = 0;
		//g_Globals->ResolverStage[index] = "MOVING RESOLVE";
	}
	else if ((Entity->GetFlags() & FL_ONGROUND))
	{
		if (shots_check[index] != Missedshots[index])
		{
			angle_brute[index] += predicted_yaw[index];
			shots_check[index] = Missedshots[index];
		}

		Angle.y = NormalizeYaw(angle_brute[index] + moving_lby[index]);
		//g_Globals->ResolverStage[index] = "MOVING-LBY + " + std::to_string(angle_brute[index]);
	}
	else
	{
		Angle.y = Entity->GetLowerBodyYaw();
		//g_Globals->ResolverStage[index] = "AIR RESOLVE";
	}

	Entity->SetEyeAngles(Angle);
}

void Resolver::AutoWallResolver(SDK::CBaseEntity* player)
{
	/// yea all of this is fucking garbage


}

void Resolver::ResolvePitch(SDK::CBaseEntity &player)
{
	int index = player.GetIndex();

	const int Missedshots = shots_fired[player.GetIndex()] - shots_hit[player.GetIndex()];

	player.SetEyeAngle(QAngle(ComputeBodyPitch(FindSmallestFake(player.GetEyeAngles().x, Missedshots % 5)), RawYaw[index], 0));
}

float Resolver::NormalizePitch(float pitch)
{
	while (pitch > 89.f) pitch -= 180.f;
	while (pitch < -89.f) pitch += 180.f;
	return pitch;
}

float Resolver::NormalizeYaw(float yaw)
{
	while (yaw > 179.f) yaw -= 360.f;
	while (yaw < -179.f) yaw += 360.f;
	return yaw;
}

float Resolver::ComputeBodyPitch(float viewangle)
{
	if (viewangle > 180.0f)
		viewangle -= 360.0f;

	return clamp(viewangle, -90.0f, 90.0f);
}
bool Resolver::IsFakeAngle(float viewangle, float goal)
{
	float NormalizedAngle = abs(NormalizePitch(viewangle));

	if (abs(NormalizedAngle - goal) < 1 && ComputeBodyPitch(viewangle) != NormalizedAngle)
		return true;

	return false;
}
float Resolver::FindSmallestFake(float goal, int skip)
{
	float last_angle = goal;
	for (float Angle = 270.0f; Angle < 10000.0f * skip; Angle += 1.0f)
	{
		if (IsFakeAngle(Angle, abs(goal)))
		{
			last_angle = Angle;
			if (!skip)
			{
				if (goal < 0.0f)
					return -Angle;
				else
					return Angle;
			}
			skip--;
		}
	}
	return last_angle * (((goal > 0.0f) * 2) - 1);
}

int Resolver::ReverseFreestanding(SDK::CBaseEntity player)
{
	SDK::CBaseEntity* pplayer = nullptr;

	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (local) return 0;
	if (!pplayer) return 0;
	if (!AutoWall) return 0;

	const float height = 64;
	Vector pos1 = MATH::RotateVectorYaw(player.GetEyePosition(), RawYaw[player.GetIndex() - 1] + 58.f, Vector(0, 0, height)), 
		pos2 = MATH::RotateVectorYaw(player.GetEyePosition(), RawYaw[player.GetIndex() - 1] - 58.f, Vector(0, 0, height));
	



    SDK::Ray_t ray;
	SDK::CTraceFilter filter; filter.pSkip1 = pplayer;
	SDK::trace_t trace;

	ray.Init(player.GetEyePosition(), pos1);
	INTERFACES::Trace->TraceRay(ray, MASK_SHOT_HULL, &filter, &trace); pos1 = trace.end;

	ray.Init(player.GetEyePosition(), pos2);
	INTERFACES::Trace->TraceRay(ray, MASK_SHOT_HULL, &filter, &trace); pos2 = trace.end;

	float dmg1 = AutoWall->CalculateDamage(local->GetEyePosition(), pos1, local, pplayer, HITGROUP_HEAD).damage,
		dmg2 = AutoWall->CalculateDamage(local->GetEyePosition(), pos2, local, pplayer, HITGROUP_HEAD).damage;

	if (abs(dmg1 - dmg2) < 15.f) return 0;
	else if (dmg1 < dmg2)        return 1;
	else                         return 2;
}
void VectorAngels(const Vector& forward, QAngle &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}
QAngle CalcAngel(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAngels(delta, angles);
	delta.Normalize();
	return angles;
}

void ShotBacktrackListener::FireGameEvent(SDK::IGameEvent* event)
{
	if (!event) return;
	if (INTERFACES::Engine->GetPlayerForUserID(event->GetInt("userid") == INTERFACES::Engine->GetLocalPlayer())) return;

	
	SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(event->GetInt(("userid")) - 1));
	if (!entity || !entity->IsAlive()) return;

	Vector pos = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
	QAngle angle = CalcAngel(entity->GetEyePosition(), pos);

	g_Resolver.Override[INTERFACES::Engine->GetPlayerForUserID(event->GetInt(("userid"))) - 1] = angle;
}


Resolver g_Resolver;






















































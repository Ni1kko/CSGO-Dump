#include "includes.hpp"
#include "Resolver_Remake.h"
#include "cheats/autowall/autowall.h"
#include "cheats/lagcompensation/lagcompensation.h"
#include "cheats/misc/logs.h"
#include "ragebot.h"
#include "Backtracking.h"

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#pragma region //	Microwave	//

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f
#define rad(a) a * 0.01745329251

#define M_PI 3.14159265358979323846
constexpr unsigned int FNVHashEx(const char* const data, const unsigned int value = 2166136261)
{
	return (data[0] == '\0') ? value : (FNVHashEx(&data[1], (value * 16777619) ^ data[0]));
}
inline float NormalizeYaw(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}
bool playerStoppedMoving(player_t* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer currentLayer = pEntity->get_animlayers()[1];
		const int activity = pEntity->sequence_activity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING)
			return true;
	}
	return false;
}
float GetCurtime()
{
	if (!g_csgo.m_engine()->IsConnected())
		return 0.f;

	if (!g_csgo.m_engine()->IsInGame())
		return 0.f;

	if (!g_ctx.m_local)
		return 0.f;

	return g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
}

float GetLBYRotatedYaw(float lby, float yaw)
{
	float delta = NormalizeYaw(yaw - lby);
	if (fabs(delta) < 25.f)
		return lby;

	if (delta > 0.f)
		return yaw + 25.f;

	return yaw;
}
bool lowerBodyPrediction(player_t* pEntity)
{
	static float prediction = 0.f;
	static bool secondrun = false;
	float flServerTime = (float)pEntity->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
	if (playerStoppedMoving(pEntity) && !secondrun)
	{
		prediction = flServerTime + 0.22;
		secondrun = true;
	}
	else if (pEntity->m_vecVelocity().Length2D() < 0.1f && secondrun && prediction <= pEntity->m_flSimulationTime())
	{
		prediction = pEntity->m_nTickBase() + 1.1f;
	}
	else//theyre moving
	{
		secondrun = false;
		return false;
	}
	if (prediction <= pEntity->m_flSimulationTime())
	{
		return true;
	}
	return false;
}
inline float NormalizePitch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}

float __fastcall ang_dif(float a1, float a2)
{
	float val = fmodf(a1 - a2, 360.0);

	while (val < -180.0f) val += 360.0f;
	while (val > 180.0f) val -= 360.0f;

	return val;
}
bool adjusting_stop(player_t* player, AnimationLayer *layer)
{
	for (int i = 0; i < 15; i++)
	{
		for (int s = 0; s < 14; s++)
		{
			auto anim_layer = player->get_animlayer(s);
			if (!anim_layer.m_pOwner)
				continue;
			const int activity = player->sequence_activity(layer[i].m_nSequence);
			if (activity == 981 && anim_layer.m_flWeight == 1.f)
			{
				return true;
			}
		}
	}
	return false;
} // ACT_CSGO_FIRE_PRIMARY
float get_average_lby_standing_update_delta(player_t* player) {
	static float last_update_time[64];
	static float second_laste_update_time[64];
	static float oldlowerbody[64];
	float lby = static_cast<int>(fabs(player->get_eye_pos().y - player->m_flLowerBodyYawTarget()));

	if (lby != oldlowerbody[player->EntIndex()]) {
		second_laste_update_time[player->EntIndex()] = last_update_time[player->EntIndex()];
		last_update_time[player->EntIndex()] = g_csgo.m_globals()->m_curtime;
		oldlowerbody[player->EntIndex()] = lby;
	}

	return last_update_time[player->EntIndex()] - second_laste_update_time[player->EntIndex()];
}
float GetCurTime(CUserCmd* ucmd) {
	player_t* local_player = g_ctx.m_local;
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->m_predicted) {
		g_tick = (float)local_player->m_nTickBase();
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_csgo.m_globals()->m_interval_per_tick;
	return curtime;
}
Vector CalcAngle69(Vector dst, Vector src)
{
	Vector angles;

	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles.x = (float)(atan(delta[2] / hyp) * 180.0 / 3.14159265);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles.z = 0.0f;

	if (delta[0] >= 0.0)
	{
		angles.y += 180.0f;
	}

	return angles;
}
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

float lerp_time()
{
	int ud_rate = g_csgo.m_cvar()->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = g_csgo.m_cvar()->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}
bool HasFakeHead(player_t* pEntity) {
	//lby should update if distance from lby to eye angles exceeds 35 degrees
	return abs(pEntity->m_angEyeAngles().y - pEntity->m_flLowerBodyYawTarget()) > 35;
}
bool Lbywithin35(player_t* pEntity) {
	//lby should update if distance from lby to eye angles less than 35 degrees
	return abs(pEntity->m_angEyeAngles().y - pEntity->m_flLowerBodyYawTarget()) < 35;
}
bool IsMovingOnGround(player_t* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->m_vecVelocity().Length2D() > 45.f && pEntity->m_fFlags() & FL_ONGROUND;
}
bool IsMovingOnInAir(player_t* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return !(pEntity->m_fFlags() & FL_ONGROUND);
}
bool OnGround(player_t* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->m_fFlags() & FL_ONGROUND;
}
bool IsFakeWalking(player_t* pEntity) {
	//Check if a player is moving, but at below a velocity of 36
	return IsMovingOnGround(pEntity) && pEntity->m_vecVelocity().Length2D() < 36.0f;
}

float tolerance = 10.f;
const inline float GetDelta(float a, float b) {
	return abs(NormalizeYaw(a - b));
}

const inline float LBYDelta(player_t* v) {
	return v->m_angEyeAngles().y - v->m_flLowerBodyYawTarget();
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}
bool HasStaticYawDifference(const std::deque<player_t*>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;)
	{
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

int GetDifferentDeltas(const std::deque<player_t*>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}

int GetDifferentLBYs(const std::deque<player_t*>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l)
	{
		float curyaw = var->m_flLowerBodyYawTarget();
		bool add = true;
		for (auto fl : vec)
		{
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

bool DeltaKeepsChanging(const std::deque<player_t*>& cur, float tolerance) {
	return (GetDifferentDeltas(cur, tolerance) > (int)cur.size() / 2);
}

bool LBYKeepsChanging(const std::deque<player_t*>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}
void LowerBodyYawFix(Vector* & Angle, player_t* Player)
{
	if (Player->m_vecVelocity().Length() > 1 && (Player->m_fFlags() & FL_ONGROUND))
		Angle->y = Player->m_flLowerBodyYawTarget();
}
static inline bool IsNearEqual(float v1, float v2, float Tolerance)
{
	return std::abs(v1 - v2) <= std::abs(Tolerance);
}
static int GetSequenceActivity(player_t* pEntity, int sequence)
{
	const model_t* pModel = pEntity->GetModel();
	if (!pModel)
		return 0;

	auto hdr = g_csgo.m_modelinfo()->GetStudiomodel(pEntity->GetModel());

	if (!hdr)
		return -1;

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return get_sequence_activity(pEntity, hdr, sequence);
}

int player_t::GetSequenceActivity(int sequence)
{
	auto hdr = g_csgo.m_modelinfo()->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto getSequenceActivity = (DWORD)(util::pattern_scan("client_panorama.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74"));
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return GetSequenceActivity(this, hdr, sequence);
}
bool lby_keeps_updating() {
	return get_average_lby_standing_update_delta;
}
bool IsAdjustingBalance(player_t* player, AnimationLayer *layer)
{
	for (int i = 0; i < 15; i++)
	{
		const int activity = player->sequence_activity(layer[i].m_nSequence);
		if (activity == 979)
		{
			return true;
		}
	}
	return false;
}
bool adjusting_balance(player_t * e, AnimationLayer * set) {
	const auto activity = e->sequence_activity(set[3].m_nSequence);

	if (activity == 979) {
		return true;
	}

	return false;
}

float NormalizeFloatToAngle(float input)
{
	for (auto i = 0; i < 3; i++) {
		while (input < -180.0f) input += 360.0f;
		while (input > 180.0f) input -= 360.0f;
	}
	return input;
}
float FixAusnahmeAngles(float yaw, float desyncdelta)
{
	for (auto i = 0; i < 3; i++) {
		while (yaw < -desyncdelta) yaw += desyncdelta;
		while (yaw > desyncdelta) yaw -= desyncdelta;
	}
	return yaw;
}
#define M_PI 3.14159265358979323846
void LBYBreakerCorrections(player_t* pEntity)
{
	float movinglby[64];
	float lbytomovinglbydelta[64];
	bool onground = pEntity->m_fFlags() & FL_ONGROUND;
	if (g_cfg.ragebot.correctlbybreaker)
	{
		lbytomovinglbydelta[pEntity->EntIndex()] = pEntity->m_flLowerBodyYawTarget() - lbytomovinglbydelta[pEntity->EntIndex()];

		if (pEntity->m_vecVelocity().Length2D() > 6 && pEntity->m_vecVelocity().Length2D() < 42)
		{
			pEntity->m_angEyeAngles().y = pEntity->m_flLowerBodyYawTarget() + 120;
		}
		else if (pEntity->m_vecVelocity().Length2D() < 6 || pEntity->m_vecVelocity().Length2D() > 42) // they are moving
		{
			pEntity->m_angEyeAngles().y = pEntity->m_flLowerBodyYawTarget();
			movinglby[pEntity->EntIndex()] = pEntity->m_flLowerBodyYawTarget();
		}
		else if (lbytomovinglbydelta[pEntity->EntIndex()] > 50 && lbytomovinglbydelta[pEntity->EntIndex()] < -50 &&
			lbytomovinglbydelta[pEntity->EntIndex()] < 112 && lbytomovinglbydelta[pEntity->EntIndex()] < -112) // the 50 will allow you to have a 30 degree margin of error (do the math :))
		{
			pEntity->m_angEyeAngles().y = movinglby[pEntity->EntIndex()];
		}
		else pEntity->m_angEyeAngles().y = pEntity->m_flLowerBodyYawTarget();
	}
}
void VectorAnglesBruteGay(const Vector& forward, Vector &angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0) pitch = 270; else pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0) yaw += 360; tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]); pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0) pitch += 360;
	} angles[0] = pitch; angles[1] = yaw; angles[2] = 0;
}

void AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

Vector calc_angle_trash(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesBruteGay(dst - src, ret);
	return ret;
}
void NormalizeNumX(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
float fov_entX(Vector ViewOffSet, Vector View, player_t* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = entity->hitbox_position(hitbox);

	AngleVectors(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNumX(Delta, Delta);
	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int closestX()
{
	int index = -1;
	float lowest_fov = 180.f; // maybe??

	player_t* local_player = g_ctx.m_local;

	if (!local_player)
		return -1;

	if (!local_player->is_alive21())
		return -1;

	Vector local_position = local_player->GetAbsOrigin() + local_player->m_vecViewOffset();
	Vector angles;
	g_csgo.m_engine()->GetViewAngles(angles);
	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++)
	{
		auto entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!entity || entity->m_iHealth() <= 0 || entity->m_iTeamNum() == local_player->m_iTeamNum() || entity->IsDormant() || entity == local_player)
			continue;

		float fov = fov_entX(local_position, angles, entity, 0);
		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}
	return index;
}
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

float apply_freestanding(player_t *enemy)
{
	auto local_player = g_ctx.m_local;
	if (!(local_player->m_iHealth() > 0))
		return 0.0f;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;

	auto leyepos = enemy->m_vecOrigin() + enemy->m_vecViewOffset();
	auto headpos = enemy->hitbox_position(0);
	auto origin = enemy->m_vecOrigin();

	int index = closestX();

	if (index == -1)
		return 0.0f;

	if (enemy->EntIndex() == closestX())
	{
		auto checkWallThickness = [&](player_t* pPlayer, Vector newhead) -> float
		{
			Vector endpos1, endpos2;

			Vector eyepos = local_player->m_vecOrigin() + local_player->m_vecViewOffset();
			Ray_t ray;
			CTraceFilterSkipTwoEntities filter(local_player, enemy);
			trace_t trace1, trace2;

			ray.Init(newhead, eyepos);
			g_csgo.m_trace()->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

			if (trace1.DidHit())
			{
				endpos1 = trace1.endpos;
				float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 75.f;
				return endpos1.DistTo(eyepos) + add / 2; // endpos2
			}

			else
			{
				endpos1 = trace1.endpos;
				float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) - 75.f;
				return endpos1.DistTo(eyepos) + add / 2; // endpos2
			}
		};

		float radius = Vector(headpos - origin).Length2D();

		for (float besthead = 0; besthead < 7; besthead += 0.1)
		{
			Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
			float totalthickness = 0.f;
			no_active = false;
			totalthickness += checkWallThickness(enemy, newhead);
			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;

				bestrotation = besthead;
			}
		}
		return RAD2DEG(bestrotation);
	}
}
/*
float apply_freestanding_low_performance(player_t *enemy, float lowerbody, float brute_value)
{
	auto local_player = g_ctx.m_local;
	if (!(local_player->m_iHealth() > 0))
		return 0.0f;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;

	auto leyepos = enemy->m_vecOrigin() + enemy->m_vecViewOffset();
	auto headpos = enemy->hitbox_position(0);
	auto origin = enemy->m_vecOrigin();

	int index = closestX();

	if (index == -1)
		return 0.0f;

	auto checkWallThickness = [&](player_t* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;

		Vector eyepos = local_player->m_vecOrigin() + local_player->m_vecViewOffset();
		Ray_t ray;
		CTraceFilterSkipTwoEntities filter(local_player, enemy);
		trace_t trace1, trace2;

		ray.Init(newhead, eyepos);
		g_csgo.m_trace()->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (index == enemy->EntIndex())
		{
			if (trace1.DidHit())
			{
				endpos1 = trace1.endpos;
				float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 75.f;
				return endpos1.DistTo(eyepos) + add / 2; // endpos2
			}

			else
			{
				endpos1 = trace1.endpos;
				float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) - 75.f;
				return endpos1.DistTo(eyepos) + add / 2; // endpos2
			}
		}

		else
		{
			return lowerbody - brute_value;
		}
	};

	float radius = Vector(headpos - origin).Length2D();

	for (float besthead = 0; besthead < 7; besthead += 0.1)
	{
		Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
		float totalthickness = 0.f;
		no_active = false;
		totalthickness += checkWallThickness(enemy, newhead);
		if (totalthickness > highestthickness)
		{
			highestthickness = totalthickness;

			bestrotation = besthead;
		}
	}
	return RAD2DEG(bestrotation);
}
*/

bool predict_lby(player_t* player, float oldlby[64], float lby, float speed)
{
	static bool nextflick[64];

	static float add_time[64];

	const auto sim = player->m_flSimulationTime();

	if (!g_cfg.ragebot.predictlbyupdate)
		return false;

	for (auto i = 0; i < g_csgo.m_globals()->m_maxclients; ++i)
	{
		if (oldlby[i] != lby && speed <= 0.1f)
		{
			add_time[i] = g_csgo.m_globals()->m_interval_per_tick + 1.1f;
		}

		if (speed >= 0.1f)
		{
			add_time[i] = 0.22f;
			nextflick[i] = sim + add_time[i];
		}

		if (sim >= nextflick[i] && speed <= 0.1f)
		{
			add_time[i] = 1.1f;
			nextflick[i] = sim + add_time[i];
			return true;
		}
	}
	return false;
}

namespace UTILS
{
	float GetCurtime()
	{
		if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
			return 0.f;

		if (!g_ctx.m_local)
			return 0.f;

		return g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
	}
	float GetLBYRotatedYaw(float lby, float yaw)
	{
		float delta = MATH::NormalizeYaw(yaw - lby);
		if (fabs(delta) < 25.f)
			return lby;

		if (delta > 0.f)
			return yaw + 25.f;

		return yaw;
	}
}

namespace MATH
{
	void VectorAngles(const Vector& forward, Vector& angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	void inline SinCos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}
	float GRD_TO_BOG(float GRD) {
		return (PI / 180) * GRD;
	}
	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

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

	__forceinline float DotProduct(const float* a, const float* b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	__forceinline float DotProduct(const Vector& a, const Vector& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	void VectorTransform(const float* in1, const matrix3x4_t& in2, float* out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}

	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		VectorTransform(&in1.x, in2, &out.x);
	}

	float CalcAngle2D(const Vector2D& src, const Vector2D& dst)
	{
		float angle;
		VectorAngle2D(dst - src, angle);
		return angle;
	}

	void VectorAngle2D(const Vector2D& direction, float& angle)
	{
		angle = RAD2DEG(std::atan2(direction.y, direction.x)) + 90.f;
	}

	void AngleVectors2D(float angle, Vector2D& forward)
	{
		angle = DEG2RAD(angle);
		Vector2D slope(sin(angle), -cos(angle));
		forward = slope;
	}
}
#pragma endregion

namespace ResolverOvermake
{
	Resolver resolver;

	Resolver::Resolver() { }

	void Resolver::DoFSN(ClientFrameStage_t stage)
	{
		if (!g_cfg.ragebot.resolver_experimental)
			return;
		auto local_player = g_ctx.m_local;
		if (!local_player || local_player->m_iHealth() <= 0)
			return;
		static bool  wasDormant[65];
		for (int i = 0; i < g_csgo.m_globals()->m_maxclients; i++)
		{
			auto entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
			if (!entity || entity->m_iHealth() <= 0 || entity->m_iTeamNum() == local_player->m_iTeamNum() || !entity->is_alive22())
				continue;
			player_info_t player_info;
			g_csgo.m_engine()->GetPlayerInfo(entity->EntIndex(), &player_info);
			if (player_info.fakeplayer)
				return;
			if (stage == FRAME_RENDER_START && g_cfg.ragebot.resolver_experimentalmode == 1)
			{
				if (entity->IsDormant())
				{
					continue;
				}
				HandleHits(entity);
				AnimationFix(entity);
				if (is_slow_walking(entity) || IsFakewalking(entity))
					g_ctx.m_globals.isFakeing = true;
			}
			if (stage == FRAME_NET_UPDATE_END && entity != g_ctx.m_local && g_cfg.ragebot.resolver_experimentalmode == 1)
			{
				if (entity->IsDormant())
				{
					continue;
				}
				auto VarMap = reinterpret_cast<uintptr_t>(entity) + 36;
				auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

				for (auto index = 0; index < VarMapSize; index++)
					*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
			}
			if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			{
				GetPlayerResolveInfo(entity).networked_angles = entity->m_angEyeAngles();

				UpdateResolveRecord(entity);

				/// make sure to do dormant check AFTER calling UpdateResolveRecord()
				if (entity->IsDormant())
					continue;

				if (g_cfg.ragebot.resolver_experimentalmode == 0)
				{
					ResolveYaw(entity);
					ResolvePitch(entity);

					const auto resolve_record = player_resolve_records[i];

					MATH::NormalizeAngle(resolve_record.resolved_angles);
					if (!g_cfg.ragebot.usedesyncresolvemdoe)
						entity->m_angEyeAngles() = resolve_record.resolved_angles;
				}
			}

			wasDormant[i] = false;
		}
	}
	void Resolver::DoCM()
	{
		if (!g_cfg.ragebot.resolver_experimental)
			return;
		auto local_player = g_ctx.m_local;
		if (!local_player || local_player->m_iHealth() <= 0)
			return;

		ProcessSnapShots();

		last_eye_positions.insert(last_eye_positions.begin(), local_player->m_vecOrigin() + local_player->m_vecViewOffset());
		if (last_eye_positions.size() > 128)
			last_eye_positions.pop_back();

		auto nci = g_csgo.m_engine()->GetNetChannelInfo();
		if (!nci)
			return;

		const int latency_ticks = TIME_TO_TICKS(nci->GetLatency(FLOW_OUTGOING));
		const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];
		for (int i = 0; i < g_csgo.m_globals()->m_maxclients; i++)
		{
			auto entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
			if (!entity || entity->m_iHealth() <= 0 || entity->m_iTeamNum() == local_player->m_iTeamNum() || entity->IsDormant() || !entity->is_alive23()) {
				UseFreestandAngle[i] = false;
				continue;
			}
			auto& resolve_record = player_resolve_records[i];

			const float height = 64;

			Vector direction_1, direction_2;
			MATH::AngleVectors(Vector(0.f, MATH::CalcAngle(local_player->m_vecOrigin(), entity->m_vecOrigin()).y - 90.f, 0.f), &direction_1);
			MATH::AngleVectors(Vector(0.f, MATH::CalcAngle(local_player->m_vecOrigin(), entity->m_vecOrigin()).y + 90.f, 0.f), &direction_2);

			const auto left_eye_pos = entity->m_vecOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
			const auto right_eye_pos = entity->m_vecOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);

			resolve_record.anti_freestanding_record.left_damage = autowall::get().calculate_return_info(latency_based_eye_pos,
				left_eye_pos, local_player, entity, 1).m_damage;
			resolve_record.anti_freestanding_record.right_damage = autowall::get().calculate_return_info(latency_based_eye_pos,
				right_eye_pos, local_player, entity, 1).m_damage;

			Ray_t ray;
			trace_t trace;
			CTraceFilterWorldOnly filter;

			ray.Init(left_eye_pos, latency_based_eye_pos);
			g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
			resolve_record.anti_freestanding_record.left_fraction = trace.fraction;

			ray.Init(right_eye_pos, latency_based_eye_pos);
			g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
			resolve_record.anti_freestanding_record.right_fraction = trace.fraction;

			resolve_record.did_predicted_lby_flick = false;
			resolve_record.did_lby_flick = false;

			if (abs(entity->m_vecVelocity().Length2D()) > 29.f)
				UseFreestandAngle[entity->EntIndex()] = false;

			if (abs(entity->m_vecVelocity().Length2D()) <= 29.f && !UseFreestandAngle[entity->EntIndex()])
			{
				bool Autowalled = false, HitSide1 = false, HitSide2 = false;

				if (resolve_record.anti_freestanding_record.left_damage > 15 && resolve_record.anti_freestanding_record.right_damage > 15) {
					if (resolve_record.anti_freestanding_record.left_damage > resolve_record.anti_freestanding_record.right_damage) {
						FreestandAngle[entity->EntIndex()] = 90;
						HitSide1 = true;
					}
					else
					{
						FreestandAngle[entity->EntIndex()] = -90;
						HitSide2 = true;
					}
					Autowalled = true;
				}

				if (Autowalled)
				{
					if (HitSide1 && HitSide2)
						UseFreestandAngle[entity->EntIndex()] = false;
					else
						UseFreestandAngle[entity->EntIndex()] = true;
				}
			}
		}
	}

	void Resolver::AnimationFix(player_t* pEnt)
	{
		//who needs structs or classes not me lol
		static float oldSimtime[65];
		static float storedSimtime[65];
		static float ShotTime[65];
		static float SideTime[65][3];
		static int LastDesyncSide[65];
		static bool Delaying[65];
		static AnimationLayer StoredLayers[64][15];
		static c_baseplayeranimationstate * StoredAnimState[65];
		static float StoredPosParams[65][24];
		static Vector oldEyeAngles[65];
		static float oldGoalfeetYaw[65];
		float* PosParams = pEnt->m_flPoseParameter();
		bool update = false;
		bool shot = false;
		auto& resolve_record = player_resolve_records[pEnt->EntIndex()];
		static bool jittering[65];

		c_baseplayeranimationstate * AnimState = pEnt->get_animation_state();
		if (!AnimState)
			return;
		if (!AnimState || !pEnt->GetAnimOverlays() || !PosParams)
			return;

		auto RemapVal = [](float val, float A, float B, float C, float D) -> float
		{
			if (A == B)
				return val >= B ? D : C;
			return C + (D - C) * (val - A) / (B - A);
		};
		//removed
		if (jittering[pEnt->EntIndex()])
			resolve_record.resolve_type |= RESOLVE_TYPE_DESYNC2;
		else if (is_slow_walking(pEnt))
			resolve_record.resolve_type |= RESOLVE_TYPE_ANTI_SLOWWALK;
		else
			resolve_record.resolve_type |= RESOLVE_TYPE_DESYNC;
		//removed
	}

	void Resolver::HandleHits(player_t* pEnt)
	{
		auto NetChannel = g_csgo.m_engine()->GetNetChannelInfo();

		if (!NetChannel)
			return;

		static float predTime[65];
		static bool init[65];
		int fired = g_ctx.m_globals.shots[pEnt->EntIndex()];
		int missed = g_ctx.m_globals.missed_shots[pEnt->EntIndex()];

		if (g_ctx.m_globals.ShotFired[pEnt->EntIndex()])
		{
			if (init[pEnt->EntIndex()])
			{
				pitchHit[pEnt->EntIndex()] = pEnt->m_angEyeAngles().x;
				predTime[pEnt->EntIndex()] = g_csgo.m_globals()->m_curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_csgo.m_engine()->GetNetChannelInfo()->m_nChokedPackets);
				init[pEnt->EntIndex()] = false;
			}

			if (g_csgo.m_globals()->m_curtime > predTime[pEnt->EntIndex()] && !g_ctx.m_globals.Hit[pEnt->EntIndex()])
			{
				g_ctx.m_globals.MissedShots[pEnt->EntIndex()] += 1;
				g_ctx.m_globals.ShotFired[pEnt->EntIndex()] = false;
			}
			else if (g_csgo.m_globals()->m_curtime <= predTime[pEnt->EntIndex()] && g_ctx.m_globals.Hit[pEnt->EntIndex()])
				g_ctx.m_globals.ShotFired[pEnt->EntIndex()] = false;
		}
		else
			init[pEnt->EntIndex()] = true;

		g_ctx.m_globals.Hit[pEnt->EntIndex()] = false;
	}

	bool Resolver::is_slow_walking(player_t* entity) {
		/*
		*Measures:
		*Slowest -> 65
		*Highest -> 100
		*im an autist and like to walk around in cyrkle
		*/
		if (IsMovingOnInAir(entity))
			return false;
		float large = 0;
		float velocity_2D[64], old_velocity_2D[64];
		auto& resolve_record = player_resolve_records[entity->EntIndex()];
		if (entity->m_vecVelocity().Length2D() != velocity_2D[entity->EntIndex()] && entity->m_vecVelocity().Length2D() != NULL) {
			old_velocity_2D[entity->EntIndex()] = velocity_2D[entity->EntIndex()];
			velocity_2D[entity->EntIndex()] = entity->m_vecVelocity().Length2D();
		}
		if (large == 0)return false;
		Vector velocity = entity->m_vecVelocity();
		Vector direction = entity->m_angEyeAngles();

		float speed = velocity.Length();
		direction.y = entity->m_angEyeAngles().y - direction.y;
		//method 1
		if (velocity_2D[entity->EntIndex()] > 1) {
			int tick_counter[64];
			if (velocity_2D[entity->EntIndex()] == old_velocity_2D[entity->EntIndex()])
				tick_counter[entity->EntIndex()] += 1;
			else
				tick_counter[entity->EntIndex()] = 0;

			while (tick_counter[entity->EntIndex()] > (1 / g_csgo.m_globals()->m_interval_per_tick) * fabsf(0.1f))//should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
				return true;
		}
		//method 2
		//removed
		//method 3  / resource heavy but does most of the work
		//removed
		//method 4
		if (velocity.Length2D() == entity->m_flMaxspeed() / 3)//ima keep that in because its basically public
			return true;
		//method 5
		if (speed < entity->m_flMaxspeed() / 3 && velocity.Length2D() != 0)//same here
			return true;

		return false;
	}
	bool isinfakeduck(player_t * autist, int chokedcommands, bool autsim)
	{
		/*int storedTick = 0;
		int crouchedTicks[64];
		float duckamount = entity->m_flDuckAmount();
		if (!duckamount) return false;
		float duckspeed = entity->m_flDuckSpeed();
		if (!duckspeed) return false;

		if (storedTick != g_pGlobals->tickcount)
		{
			crouchedTicks[entity->GetIndex()]++;
			storedTick = g_pGlobals->tickcount;
		}
		if (duckspeed == 8 && duckamount <= 0.9 && duckamount > 0.01 && (*entity->GetFlags() & FL_ONGROUND) && (crouchedTicks[entity->GetIndex()] >= 5))
			return true;
		else
			return false;*/
		return true;
	}
	bool Resolver::high_delta(player_t * player, AnimationLayer *layer)
	{
		for (int s = 0; s < 14; s++)
		{
			AnimationLayer record[15];

			auto anim_layer = player->get_animlayer(s);
			auto anime = &player->get_animlayers()[1];

			if (!anim_layer.m_pOwner)
				continue;

			for (auto i = 0; i < g_csgo.m_globals()->m_maxclients; ++i)
			{
				auto activity = player->GetSequenceActivity(layer[i].m_nSequence);

				if ((anim_layer.m_flPrevCycle != anim_layer.m_flCycle || anim_layer.m_flWeight == 1.f) && activity == 979)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool  Resolver::low_delta(player_t * player, AnimationLayer *layer)
	{
		for (int s = 0; s < 14; s++)
		{
			auto anim_layer = player->get_animlayer(s);
			auto anime = &player->get_animlayers()[1];

			if (!anim_layer.m_pOwner)
				continue;

			for (auto i = 0; i < g_csgo.m_globals()->m_maxclients; ++i)
			{
				auto activity = player->GetSequenceActivity(layer[i].m_nSequence);

				if (anim_layer.m_flPrevCycle > 0.92f && anim_layer.m_flCycle > 0.92f && anim_layer.m_flWeight == 0.f)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Resolver::is_fakewalk(player_t* player, float speed, AnimationLayer * layer)
	{
		if (g_ctx.m_local->m_iHealth() <= 0)
			return false;

		if (player->IsDormant())
			return false;

		for (int s = 0; s < 14; s++)
		{
			auto activity = player->GetSequenceActivity(layer[s].m_nSequence);
			if (speed > 0.1f && player->get_animlayer(12).m_flWeight != 0.f && activity == 979)
			{
				return true;
			}
		}
		return false;
	}
	void Resolver::max_eye_ang_delta()
	{
		resolver_info_t
			&current = m_current,
			&previous = m_previous;

		float delta = previous.EyeAng - current.EyeAng;
		if (current.Desync >= abs(delta) && current.lastMaxDSC < abs(delta) && abs(delta) > 4.0)
			current.lastMaxDSC = delta;
		if (current.Desync < current.lastMaxDSC)
			current.lastMaxDSC = current.Desync;
	}
	float Resolver::Desync::network_latency() {
		INetChannelInfo *nci = g_csgo.m_engine()->GetNetChannelInfo();
		if (nci)
			return nci->GetAvgLatency(FLOW_INCOMING);

		return 0.0f;
	}

	int Resolver::Desync::NetworkedChockedTicks(player_t * entity) {
		float flSimulationTime = entity->m_flSimulationTime();
		float flSimDiff = g_csgo.m_globals()->m_curtime - flSimulationTime;
		float latency = network_latency();
		return (int)(0.5f + (float)(max(0.0f, flSimDiff - latency)) / g_csgo.m_globals()->m_interval_per_tick);
	}
	void Resolver::Update(player_t* pEnt) {
		resolver_info_t
			&current = m_current,
			&previous = m_previous;

		current.Entity = pEnt;
		current.EyeAng = pEnt->m_angEyeAngles().y;
		current.LBYT = pEnt->m_flLowerBodyYawTarget();
	}

	void Resolver::data(player_t* entity) {
		//removed
	}
	void Resolver::ResolveDesync(int res, player_t * pEnt) {
		//removed
	}
	void Resolver::AddShotSnapshot(player_t* entity, PlayerResolveRecord resolve_record)
	{
		ShotSnapshot snapshot;

		snapshot.entity = entity;
		snapshot.time = UTILS::GetCurtime();
		snapshot.resolve_record = resolve_record;
		snapshot.first_processed_time = 0.f;
		snapshot.was_shot_processed = false;
		snapshot.hitgroup_hit = -1;

		shot_snapshots.push_back(snapshot);
	}
	int total_missed[64];
	int total_hit[64];
	IGameEvent* event = nullptr;
	void Resolver::EventCallback(IGameEvent* game_event)
	{
		auto local_player = g_ctx.m_local;
		if (!local_player || local_player->m_iHealth() <= 0 || !game_event)
			return;

		if (shot_snapshots.size() <= 0)
			return;

		auto& snapshot = shot_snapshots.front();
		switch (FNVHashEx(game_event->GetName()))
		{
		case FNVHashEx("player_hurt"):
		{
			/// it wasn't us who shot
			if (g_csgo.m_engine()->GetPlayerForUserID(game_event->GetInt(enc_char("attacker"))) != g_csgo.m_engine()->GetLocalPlayer())
				return;

			if (!snapshot.was_shot_processed)
			{
				snapshot.was_shot_processed = true;
				snapshot.first_processed_time = UTILS::GetCurtime();
			}

			snapshot.hitgroup_hit = game_event->GetInt(enc_char("hitgroup"));

			break;
		}
		case FNVHashEx("bullet_impact"):
		case FNVHashEx("weapon_fire"):
		{
			/// it wasn't us who shot
			if (g_csgo.m_engine()->GetPlayerForUserID(game_event->GetInt(enc_char("userid"))) != g_csgo.m_engine()->GetLocalPlayer())
				return;

			if (!snapshot.was_shot_processed)
			{
				snapshot.was_shot_processed = true;
				snapshot.first_processed_time = UTILS::GetCurtime();
			}

			break;
		}
		}
	}

	void Resolver::missed_due_to_desync(IGameEvent* event) {
		if (event == nullptr)
			return;
		int user = event->GetInt("userid");
		int attacker = event->GetInt("attacker");
		bool player_hurt[64], hit_entity[64];

		if (g_csgo.m_engine()->GetPlayerForUserID(user) != g_csgo.m_engine()->GetLocalPlayer()
			&& g_csgo.m_engine()->GetPlayerForUserID(attacker) == g_csgo.m_engine()->GetLocalPlayer()) {
			player_hurt[g_csgo.m_engine()->GetPlayerForUserID(user)] = true;
		}

		if (g_csgo.m_engine()->GetPlayerForUserID(user) != g_csgo.m_engine()->GetLocalPlayer())
		{
			Vector bullet_impact_location = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
			Vector Nothing = NULL;
			if (Nothing != bullet_impact_location) return;
			hit_entity[g_csgo.m_engine()->GetPlayerForUserID(user)] = true;
		}
		if (!player_hurt[g_csgo.m_engine()->GetPlayerForUserID(user)] && hit_entity[g_csgo.m_engine()->GetPlayerForUserID(user)]) {
			//	s_extra.current_flag[Interfaces::Engine->GetPlayerForUserID(user)] = correction_flags::DESYNC;
			++total_missed[g_csgo.m_engine()->GetPlayerForUserID(user)];
		}
		if (player_hurt[g_csgo.m_engine()->GetPlayerForUserID(user)] && hit_entity[g_csgo.m_engine()->GetPlayerForUserID(user)]) {
			++total_hit[g_csgo.m_engine()->GetPlayerForUserID(user)];
		}
	}
	void Resolver::ProcessSnapShots()
	{
		if (shot_snapshots.size() <= 0)
			return;

		const auto snapshot = shot_snapshots.front();
		if (fabs(UTILS::GetCurtime() - snapshot.time) > 1.f)
		{
			//LOG(enc_char("** Unhandled shot snapshot!"));
			//FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log(enc_char("Unhandled shot snapshot!"), RED));

			shot_snapshots.erase(shot_snapshots.begin());
			return;
		}

		const int player_index = snapshot.entity->EntIndex();
		if (snapshot.hitgroup_hit != -1) /// hit
		{
			/// increment shots fired and shots hit
			for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
			{
				if (snapshot.resolve_record.resolve_type & (1 << i))
				{
					player_resolve_records[player_index].shots_fired[i]++;
					player_resolve_records[player_index].shots_hit[i]++;
				}
			}

			/*std::stringstream ss;
			ss << "echo [RESOLVER] Hit shot at " << TranslateResolveRecord(snapshot.resolve_record.resolve_type);
			fridge::ConsolePrint("[RESOLVER] Hit shot at %s", TranslateResolveRecord(snapshot.resolve_record.resolve_type));
			eventlogs::get().add(ss.str(), Color::Green);*/
		}
		else if (snapshot.first_processed_time != 0.f && fabs(UTILS::GetCurtime() - snapshot.first_processed_time) > 0.1f) /// missed
		{
			/// increment shots fired but not shots hit
			for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
			{
				if (snapshot.resolve_record.resolve_type & (1 << i))
					player_resolve_records[player_index].shots_fired[i]++;
			}

			if (snapshot.resolve_record.resolve_type & RESOLVE_TYPE_LAST_MOVING_LBY)
				player_resolve_records[player_index].shots_missed_moving_lby++;

			//if (snapshot.resolve_record.resolve_type & RESOLVE_TYPE_NODESYNC)
			//	player_resolve_records[player_index].shots_missed_moving_lby_delta++;

	/*		std::stringstream ss2;
			ss2 << "[RESOLVER] Missed shot at " << TranslateResolveRecord(snapshot.resolve_record.resolve_type);

			fridge::ConsolePrint("[RESOLVER] Missed shot at %", TranslateResolveRecord(snapshot.resolve_record.resolve_type));
			eventlogs::get().add(ss2.str(), Color::Red);*/
		}

		shot_snapshots.erase(shot_snapshots.begin());
	}

	void Resolver::UpdateResolveRecord(player_t* entity)
	{
		/// a copy of the previous record for comparisons and shit
		const auto previous_record = player_resolve_records[entity->EntIndex()];
		auto& record = player_resolve_records[entity->EntIndex()];

		record.resolved_angles = record.networked_angles;
		record.velocity = entity->m_vecVelocity();
		record.origin = entity->m_vecOrigin();
		record.lower_body_yaw = entity->m_flLowerBodyYawTarget();
		record.is_dormant = entity->IsDormant();
		record.has_fake = IsFakingYaw(entity);
		record.desync_max_delta = entity->get_max_desync_delta();
		record.resolve_type = 0;

		record.is_balance_adjust_triggered = false, record.is_balance_adjust_playing = false;
		for (int i = 0; i < 15; i++) /// go through each animation layer
		{
			record.anim_layers[i] = entity->get_animlayer(i);
			/// balanceadjust act
			//changed from getsequence to sequence_activity
			if (entity->sequence_activity(record.anim_layers[i].m_nSequence) == Activity::ACT_CSGO_IDLE_TURN_BALANCEADJUST)
			{
				record.is_balance_adjust_playing = true;

				/// balance adjust is being triggered
				if (record.anim_layers[i].m_flWeight == 1 || record.anim_layers[i].m_flCycle > previous_record.anim_layers[i].m_flCycle)
					record.last_balance_adjust_trigger_time = UTILS::GetCurtime();
				if (fabs(UTILS::GetCurtime() - record.last_balance_adjust_trigger_time) < 0.5f)
					record.is_balance_adjust_triggered = true;
			}
		}

		if (record.is_dormant)
			record.next_predicted_lby_update = FLT_MAX;

		if (record.desync_max_delta > 59)
			record.desync_max_delta = 59;
		else if (record.desync_max_delta < -59)
			record.desync_max_delta = -59;
		/// if lby updated
		if (record.lower_body_yaw != previous_record.lower_body_yaw && !record.is_dormant && !previous_record.is_dormant) {
			record.did_lby_flick = true;
			if ((entity->m_flSimulationTime() - record.last_time_moving) > .22f)
				record.m_predicted_yaw_brute += record.lower_body_yaw - previous_record.lower_body_yaw;
		}

		/// fakewalk
		const bool is_moving_on_ground = record.velocity.Length2D() > 0.34f && entity->m_fFlags() & FL_ONGROUND;
		if (is_moving_on_ground && record.is_balance_adjust_triggered)
			record.is_fakewalking = true;
		else
			record.is_fakewalking = false;

		/// last moving lby, using a high velocity check ***just in case*** of fakewalkers
		if (is_moving_on_ground && !record.is_fakewalking && record.velocity.Length2D() > 1.f && !record.is_dormant)
		{
			record.is_last_moving_lby_valid = true;
			record.is_last_moving_lby_delta_valid = false;
			record.shots_missed_moving_lby = 0;
			record.shots_missed_moving_lby_delta = 0;
			record.last_moving_lby = record.lower_body_yaw + 45;
			record.last_time_moving = entity->m_flSimulationTime();
			//reset brute data
			record.m_predicted_yaw_brute = 0.f;
		}

		/// just came out of dormancy
		if (!record.is_dormant && previous_record.is_dormant)
		{
			/// if moved more than 32 units
			if ((record.origin - previous_record.origin).Length2D() > 16.f)
				record.is_last_moving_lby_valid = false;
		}

		/// get last moving lby delta
		if (!record.is_last_moving_lby_delta_valid && record.is_last_moving_lby_valid && record.velocity.Length2D() < 20 && fabs(UTILS::GetCurtime() - record.last_time_moving) < 1.0)
		{
			/// if lby updated
			if (record.lower_body_yaw != previous_record.lower_body_yaw)
			{
				record.last_moving_lby_delta = MATH::NormalizeYaw(record.last_moving_lby - record.lower_body_yaw);
				record.is_last_moving_lby_delta_valid = true;
			}
		}

		if (MATH::NormalizePitch(record.networked_angles.x) > 5.f)
			record.last_time_down_pitch = UTILS::GetCurtime();
	}
	bool ausnahme = false;
	void Resolver::ResolveYaw(player_t* entity)
	{
		//removed
		if (entity->get_animlayer(6).m_flPlaybackRate == 0)			//experiment
		{
			entity->m_angEyeAngles().y = *entity->AnimationState()->feetyaw();
		}
		m_previous = m_current;
	}

	void Resolver::ResolvePitch(player_t* entity)
	{
		auto& resolve_record = player_resolve_records[entity->EntIndex()];

		if (resolve_record.resolve_type & RESOLVE_TYPE_LBY_UPDATE || resolve_record.resolve_type & RESOLVE_TYPE_PREDICTED_LBY_UPDATE ||
			resolve_record.resolve_type & RESOLVE_TYPE_NO_FAKE)
			resolve_record.resolved_angles.x = resolve_record.networked_angles.x;
		else
		{
			/// super fucking ghetto fix to stop their pitch from going to 0 when they're shooting (only neccesary when they're using psilent)
			if (fabs(UTILS::GetCurtime() - resolve_record.last_time_down_pitch) < 0.5f)
				resolve_record.resolved_angles.x = 70.f;
			else
				resolve_record.resolved_angles.x = resolve_record.networked_angles.x;
		}
		if (resolve_record.networked_angles.x > 90.0f && resolve_record.networked_angles.x < 270.f)//bif fakedown and fakeup fixerino
		{
			if (resolve_record.networked_angles.x > 180.f)
				resolve_record.resolved_angles.x = -89.f;
			else
				resolve_record.resolved_angles.x = 89.0f;
		}
		resolve_record.resolved_angles.x = MATH::NormalizePitch(resolve_record.resolved_angles.x);
	}
	void Resolver::ResolveYawBruteforce(player_t* entity)
	{
		int fired = g_ctx.m_globals.shots[entity->EntIndex()];
		int missed = g_ctx.m_globals.missed_shots[entity->EntIndex()];
		auto& resolve_record = player_resolve_records[entity->EntIndex()];
		float base_angle = resolve_record.is_last_moving_lby_valid ? resolve_record.last_moving_lby : resolve_record.lower_body_yaw;
		auto best = [](float primary, float secondary, float defined, bool accurate) -> float
		{
			if (accurate)
			{
				if (math::angle_distance(math::normalize_yaw(primary), math::normalize_yaw(defined)) <= 50)
					return math::normalize_yaw(primary);
				else if (math::angle_distance(math::normalize_yaw(secondary), math::normalize_yaw(defined)) <= 50)
					return math::normalize_yaw(secondary);
				else
					return math::normalize_yaw(defined);
			}
			else
			{
				if (math::angle_distance(math::normalize_yaw(primary), math::normalize_yaw(defined)) <= 80)
					return math::normalize_yaw(primary);
				else if (math::angle_distance(math::normalize_yaw(secondary), math::normalize_yaw(defined)) <= 80)
					return math::normalize_yaw(secondary);
				else
					return math::normalize_yaw(defined);
			}
		};

		resolve_record.resolved_angles.y = base_angle + (resolve_record.m_predicted_yaw_brute * fired);
	}

	float Resolver::ResolveYawOverride(player_t* entity)
	{
		auto local_player = g_ctx.m_local;

		if (!local_player)
			return FLT_MAX;

		if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.ragebot.override_key))
		{
			Vector viewangles;
			g_csgo.m_engine()->GetViewAngles(viewangles);

			const float at_target_yaw = MATH::CalcAngle(local_player->m_vecOrigin(), entity->m_vecOrigin()).y;
			if (fabs(MATH::NormalizeYaw(viewangles.y - at_target_yaw)) > 10.f)
				return FLT_MAX;

			return UTILS::GetLBYRotatedYaw(entity->m_flLowerBodyYawTarget(), (MATH::NormalizeYaw(viewangles.y - at_target_yaw) > 0) ? at_target_yaw + 90.f : at_target_yaw - 90.f);
		}
		else
		{
			return FLT_MAX;
		}
	}

	void Resolver::ResolveSlowwalk(player_t* ent)
	{
		const auto freestanding_record = player_resolve_records[ent->EntIndex()].anti_freestanding_record;
		auto local_player = g_ctx.m_local;
		int missed = (g_ctx.m_globals.shots[ent->EntIndex()] - g_ctx.m_globals.hit[ent->EntIndex()]);
		auto& resolve_record = player_resolve_records[ent->EntIndex()];
		const float at_target_yaw = MATH::CalcAngle(g_ctx.m_local->m_vecOrigin(), ent->m_vecOrigin()).y;
		if (missed > 4)
			missed = 0;
		if (missed < 3)//if dumping a bit (missed 1st & 2nd shot)
		{
			if (freestanding_record.left_damage <= 0 && freestanding_record.right_damage <= 0) {
				if (freestanding_record.right_fraction < freestanding_record.left_fraction)
					resolve_record.resolved_angles.y = at_target_yaw + 125.f;
				else
					resolve_record.resolved_angles.y = at_target_yaw - 73.f;
			}
			else {
				if (freestanding_record.left_damage > freestanding_record.right_damage)
					resolve_record.resolved_angles.y = at_target_yaw + 130.f;
				else
					resolve_record.resolved_angles.y = at_target_yaw - 55.f;
			}
		}
		else//if dumping (missed >2 shots)
		{
			if (freestanding_record.left_damage <= 0 && freestanding_record.right_damage <= 0) {
				if (freestanding_record.right_fraction < freestanding_record.left_fraction)
					resolve_record.resolved_angles.y = at_target_yaw - 125.f;
				else
					resolve_record.resolved_angles.y = at_target_yaw + 73.f;
			}
			else {
				if (freestanding_record.left_damage > freestanding_record.right_damage)
					resolve_record.resolved_angles.y = at_target_yaw - 130.f;
				else
					resolve_record.resolved_angles.y = at_target_yaw + 55.f;
			}
		}
	}

	bool Resolver::AntiFreestanding(player_t* entity, float& yaw)
	{
		const auto freestanding_record = player_resolve_records[entity->EntIndex()].anti_freestanding_record;

		auto local_player = g_ctx.m_local;
		if (!local_player)
			return false;

		if (freestanding_record.left_damage >= 20 && freestanding_record.right_damage >= 20)
			return false;

		const float at_target_yaw = MATH::CalcAngle(local_player->m_vecOrigin(), entity->m_vecOrigin()).y;
		if (freestanding_record.left_damage <= 0 && freestanding_record.right_damage <= 0)
		{
			if (freestanding_record.right_fraction < freestanding_record.left_fraction)
				yaw = at_target_yaw + 125.f;
			else
				yaw = at_target_yaw - 73.f;
		}
		else if (freestanding_record.left_damage >= 35 && freestanding_record.right_damage >= 35) {
			yaw = at_target_yaw + 180.f;
		}
		else
		{
			if (freestanding_record.left_damage > freestanding_record.right_damage)
				yaw = at_target_yaw + 130.f;
			else
				yaw = at_target_yaw - 49.f;
		}

		return true;
	}

	bool Resolver::IsYawSideways(player_t* entity, float yaw)
	{
		auto local_player = g_ctx.m_local;
		if (!local_player)
			return false;

		const auto at_target_yaw = MATH::CalcAngle(local_player->m_vecOrigin(), entity->m_vecOrigin()).y;
		const float delta = fabs(MATH::NormalizeYaw(at_target_yaw - yaw));

		return delta > 20.f && delta < 160.f;
	}

	bool Resolver::IsFakingYaw(player_t* entity)
	{
		const auto sim = entity->m_flSimulationTime();
		float last_sim[64] = { 0.f };
		if (sim - last_sim[entity->EntIndex()] >= 1)
		{
			if (sim - last_sim[entity->EntIndex()] == 1)
			{
				return false;
			}
			last_sim[entity->EntIndex()] = sim;
		}
		return true;
	}

	std::string Resolver::TranslateResolveRecord(unsigned short resolve_type)
	{
		std::vector<std::string> resolve_strings;

		/// gonna have to use lorge if statements cuz fuck you
		if (resolve_type & RESOLVE_TYPE_OVERRIDE)
			resolve_strings.push_back(enc_str("Override"));
		if (resolve_type & RESOLVE_TYPE_NO_FAKE)
			resolve_strings.push_back(enc_str("No Fake"));
		if (resolve_type & RESOLVE_TYPE_LBY)
			resolve_strings.push_back(enc_str("LBY"));
		if (resolve_type & RESOLVE_TYPE_LBY_UPDATE)
			resolve_strings.push_back(enc_str("LBY Update"));
		if (resolve_type & RESOLVE_TYPE_PREDICTED_LBY_UPDATE)
			resolve_strings.push_back(enc_str("Predicted LBY Update"));
		if (resolve_type & RESOLVE_TYPE_NOT_BREAKING_LBY)
			resolve_strings.push_back(enc_str("Not breaking LBY"));
		if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
			resolve_strings.push_back(enc_str("Bruteforce"));
		if (resolve_type & RESOLVE_TYPE_NODESYNC)
			resolve_strings.push_back(enc_str("No Desync"));
		if (resolve_type & RESOLVE_TYPE_LAST_MOVING_LBY)
			resolve_strings.push_back(enc_str("Last Move LBY"));
		if (resolve_type & RESOLVE_TYPE_ANTI_FREESTANDING)
			resolve_strings.push_back(enc_str("Anti Freestand"));
		if (resolve_type & RESOLVE_TYPE_ANTI_SLOWWALK)
			resolve_strings.push_back(enc_str("Slow/Fakewalk"));
		if (resolve_type & RESOLVE_TYPE_DESYNC)
			resolve_strings.push_back(enc_str("Desync Static"));
		if (resolve_type & RESOLVE_TYPE_DESYNC2)
			resolve_strings.push_back(enc_str("Desync Jitter"));
		//if (ausnahme)
		//	resolve_strings.push_back(enc_str("ERROR | Angles are over max diff"));

		if (resolve_strings.size() <= 0)
			return "---";

		std::string buffer;
		for (const auto& str : resolve_strings)
			buffer.size() ? (buffer += " / " + str) : (buffer = str);

		return buffer;
	}

	Color Resolver::GetResolveColor(unsigned short resolve_type)
	{
		/// gonna have to use lorge if statements cuz fuck you
		if (resolve_type & RESOLVE_TYPE_OVERRIDE)
			return Color::Red;
		else if (resolve_type & RESOLVE_TYPE_NO_FAKE)
			return Color::Green;
		else if (resolve_type & RESOLVE_TYPE_LBY)
			return Color::Green;
		else if (resolve_type & RESOLVE_TYPE_LBY_UPDATE)
			return Color::Green;
		else if (resolve_type & RESOLVE_TYPE_PREDICTED_LBY_UPDATE)
			return Color::Green;

		else if (resolve_type & RESOLVE_TYPE_NOT_BREAKING_LBY)
			return Color::Blue;
		else if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
			return Color::Red;
		else if (resolve_type & RESOLVE_TYPE_NODESYNC)
			return Color::Blue;
		else if (resolve_type & RESOLVE_TYPE_LAST_MOVING_LBY)
			return Color::Blue;
		else if (resolve_type & RESOLVE_TYPE_ANTI_FREESTANDING)
			return Color::Blue;
		else if (resolve_type & RESOLVE_TYPE_ANTI_SLOWWALK)
			return Color::Red;
		else if (resolve_type & RESOLVE_TYPE_DESYNC)
			return Color::Green;
		else if (resolve_type & RESOLVE_TYPE_DESYNC2)
			return Color::Blue;

		return Color::White;
	}

	int Resolver::GetResolveTypeIndex(unsigned short resolve_type)
	{
		/// gonna have to use lorge if statements cuz fuck you
		if (resolve_type & RESOLVE_TYPE_OVERRIDE)
			return 0;
		else if (resolve_type & RESOLVE_TYPE_NO_FAKE)
			return 1;
		else if (resolve_type & RESOLVE_TYPE_LBY)
			return 2;
		else if (resolve_type & RESOLVE_TYPE_LBY_UPDATE)
			return 3;
		else if (resolve_type & RESOLVE_TYPE_PREDICTED_LBY_UPDATE)
			return 4;
		else if (resolve_type & RESOLVE_TYPE_LAST_MOVING_LBY)
			return 5;
		else if (resolve_type & RESOLVE_TYPE_NOT_BREAKING_LBY)
			return 6;
		else if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
			return 7;
		else if (resolve_type & RESOLVE_TYPE_NODESYNC)
			return 8;
		else if (resolve_type & RESOLVE_TYPE_ANTI_FREESTANDING)
			return 9;
		else if (resolve_type & RESOLVE_TYPE_ANTI_SLOWWALK)
			return 10;
		else if (resolve_type & RESOLVE_TYPE_DESYNC)
			return 11;
		else if (resolve_type & RESOLVE_TYPE_DESYNC2)
			return 12;
		return 0;
	}

	int Resolver::GetShotsMissed(player_t* entity, unsigned short resolve_type)
	{
		const int resolve_index = GetResolveTypeIndex(resolve_type);

		return player_resolve_records[entity->EntIndex()].shots_fired[resolve_index] - player_resolve_records[entity->EntIndex()].shots_hit[resolve_index];
	}
}

//removed
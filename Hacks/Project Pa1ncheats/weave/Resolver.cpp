#include "Hooks.h"
#include "Resolver.h"
#include "RageBacktracking.h"
#include "Ragebot.h"
#include "AnimationFix.h"


std::string ResolverMode[65];
int last_ticks[65];
int IBasePlayer::GetChokedPackets() {
	auto ticks = TIME_TO_TICKS(GetSimulationTime() - GetOldSimulationTime());
	if (ticks == 0 && last_ticks[GetIndex()] > 0) {
		return last_ticks[GetIndex()] - 1;
	}
	else {
		last_ticks[GetIndex()] = ticks;
		return ticks;
	}
}

float CResolver::GetAngle(IBasePlayer* player) {
	return Math::NormalizeYaw(player->GetEyeAngles().y); // player->GetEyeAngles().y
}

float CResolver::GetForwardYaw(IBasePlayer* player) {
	return Math::NormalizeYaw(GetBackwardYaw(player) - 180.f);
}

float CResolver::GetBackwardYaw(IBasePlayer* player) {
	return Math::CalculateAngle(csgo->local->GetOrigin(), player->GetOrigin()).y;
}

float CResolver::GetLeftYaw(IBasePlayer* player) {
	return Math::NormalizeYaw(Math::CalculateAngle(csgo->local->GetOrigin(), player->GetOrigin()).y - 90.f);
}

float CResolver::GetRightYaw(IBasePlayer* player) {
	return Math::NormalizeYaw(Math::CalculateAngle(csgo->local->GetOrigin(), player->GetOrigin()).y + 90.f);
}
bool CResolver::TargetJitter(IBasePlayer* player, bool v2) {
	float yaw = v2 ? GetRightYaw(player) : GetLeftYaw(player);
	return fabsf(GetAngle(player) - Math::NormalizeYaw(yaw + 90.f))
		>= fabsf(GetAngle(player) - Math::NormalizeYaw(yaw - 90.f));
}
bool CResolver::TargetSide(IBasePlayer* player) {
	float yaw = Math::NormalizeYaw(GetBackwardYaw(player));
	float angle = GetAngle(player);
	return fabsf(angle - Math::NormalizeYaw(yaw + 90.f))
		>= fabsf(angle - Math::NormalizeYaw(yaw - 90.f));
}

void CResolver::DetectSide(IBasePlayer* player, int* side)
{
	Vector src3D, dst3D, forward, right, up, src, dst;
	float back_two, right_two, left_two;
	trace_t tr;
	Ray_t ray, ray2, ray3, ray4, ray5;
	CTraceFilter filter;

	Math::AngleVectors(Vector(0, GetBackwardYaw(player), 0), &forward, &right, &up);

	filter.pSkip = player;
	src3D = player->GetEyePosition();
	dst3D = src3D + (forward * 384); //Might want to experiment with other numbers, incase you don't know what the number does, its how far the trace will go. Lower = shorter.

	ray.Init(src3D, dst3D);
	interfaces.trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	back_two = (tr.endpos - tr.startpos).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);
	interfaces.trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);
	right_two = (tr.endpos - tr.startpos).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);
	interfaces.trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);
	left_two = (tr.endpos - tr.startpos).Length();

	if (left_two > right_two) {
		*side = -1;
		//Body should be right
	}
	else if (right_two > left_two) {
		*side = 1;
	}
	else
		*side = 0;
}

bool CResolver::DoesHaveJitter(IBasePlayer* player, int* new_side) {
	static float LastAngle[64];
	static int LastBrute[64];
	static bool Switch[64];
	static float LastUpdateTime[64];

	int i = player->GetIndex();

	float CurrentAngle = player->GetEyeAngles().y;
	if (!Math::IsNearEqual(CurrentAngle, LastAngle[i], 50.f)) {
		Switch[i] = !Switch[i];
		LastAngle[i] = CurrentAngle;
		*new_side = Switch[i] ? 1 : -1;
		LastBrute[i] = *new_side;
		LastUpdateTime[i] = interfaces.global_vars->curtime;
		return true;
	}
	else {
		if (fabsf(LastUpdateTime[i] - interfaces.global_vars->curtime >= TICKS_TO_TIME(17))
			|| player->GetSimulationTime() != player->GetOldSimulationTime()) {
			LastAngle[i] = CurrentAngle;
		}
		*new_side = LastBrute[i];
	}
	return false;
}



void CResolver::StoreAntifreestand()
{
	if (!csgo->local->isAlive())
		return;

	if (!csgo->weapon->IsGun())
		return;


	for (int i = 1; i < interfaces.engine->GetMaxClients(); ++i)
	{
		auto player = interfaces.ent_list->GetClientEntity(i);

		if (!player || !player->isAlive() || player->IsDormant() || player->GetTeam() == csgo->local->GetTeam())
			continue;

		bool Autowalled = false, HitSide1 = false, HitSide2 = false;
		auto idx = player->GetIndex();
		float angToLocal = Math::CalculateAngle(csgo->local->GetOrigin(), player->GetOrigin()).y;
		Vector ViewPoint = csgo->local->GetOrigin() + Vector(0, 0, 90);
		Vector2D Side1 = { (45 * sin(DEG2RAD(angToLocal))),(45 * cos(DEG2RAD(angToLocal))) };
		Vector2D Side2 = { (45 * sin(DEG2RAD(angToLocal + 180))) ,(45 * cos(DEG2RAD(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(DEG2RAD(angToLocal))),(50 * cos(DEG2RAD(angToLocal))) };
		Vector2D Side4 = { (50 * sin(DEG2RAD(angToLocal + 180))) ,(50 * cos(DEG2RAD(angToLocal + 180))) };

		Vector Origin = player->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		float niggaangle = GetAngle(player);

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
			Vector ViewPointAutowall = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_AutoWall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (niggaangle >= 45.f) // niggaangle >= 45.f || niggaangle <= -45.f
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandSide[idx] = 1; // -1
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandSide[idx] = -1; // 1
					}
				}
				else {
					if (side == 0)
					{
						HitSide1 = true;
						FreestandSide[idx] = -1; // -1
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandSide[idx] = 1; // 1
					}
				}

				Autowalled = true;
			}
			else
			{
				for (int sidealternative = 0; sidealternative < 2; sidealternative++)
				{
					Vector ViewPointAutowallalternative = { Origin.x + OriginLeftRight[sidealternative].x,  Origin.y - OriginLeftRight[sidealternative].y , Origin.z + 90 };

					if (g_AutoWall.CanHitFloatingPoint(ViewPointAutowallalternative, ViewPointAutowall))
					{

						if (niggaangle >= 45.f)
						{
							if (sidealternative == 0)
							{
								HitSide1 = true;
								FreestandSide[idx] = 1;
							}
							else if (sidealternative == 1)
							{
								HitSide2 = true;
								FreestandSide[idx] = -1;

							}
						}
						else {
							if (sidealternative == 0)
							{
								HitSide1 = true;
								FreestandSide[idx] = -1;
							}
							else if (sidealternative == 1)
							{
								HitSide2 = true;
								FreestandSide[idx] = 1;

							}
						}

						Autowalled = true;
					}
				}
			}
		}
	}
}

float laith_num_rand(float Min, float Max) // random number // (nem a resolverben használatos)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
};


float get_max_desync_delta(IBasePlayer* player) {

	auto animstate = player->GetPlayerAnimState();

	float rate = 180;
	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return rate;
}

int random(int min, int max)
{
	static bool first = true;
	if (first) {
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}



void CResolver::Do(IBasePlayer* player)
{
	if (vars.ragebot.resolver)
	{

		auto animstate = player->GetPlayerAnimState();
		if (!animstate)
			return;
		animstate->m_flGoalFeetYaw = GetAngle(player);

		if (!player || !player->isAlive()) // HOGYHA MEGHALTÁL MI A GECIMNEK RESOLVOLJAD AZ ENEMIT TE GECI?
			return;

		if (player == csgo->local) // MIÉRT RESOLVOLJA A LOCAL PLAYERT???? LE MEGY AZ FPSED TÕLE!!! #POTATO PC
			return;

		if (player->GetTeam() == csgo->local->GetTeam()) // Anti team mate resolver // ne resolvolja a team matet fölöslegesen. KUBU, BAZD+ HOGY NEM JUTOTT AZ ESZÜNKBE???
			return;

		if (player->GetChokedPackets() <= 1)
			return;

		if (player->GetOldSimulationTime() > player->GetSimulationTime())
			return;

		player_info_t info;
		if (!interfaces.engine->GetPlayerInfo(player->GetIndex(), &info))
			return;

		int idx = player->GetIndex();

		if (player->GetPlayerInfo().fakeplayer)
			return;

		if (!(player->GetFlags() & FL_ONGROUND))
			return;

		float angle = GetAngle(player);
		{
			int dumbynigga = GetAngle(player);
			int* dumbnigga = &dumbynigga;

			DoesHaveJitter(player, dumbnigga);

			if (angle >= 45.f) { // angle >= 45.f
				switch (csgo->missedshots[player->GetIndex()] % 3) {
				case 0:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = 90.f;
						if (feet_yaw <= 90)
						{
							if (-90 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = 90.f; resol_delta < -90.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				case 1:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = 19.f;
						if (feet_yaw <= 19)
						{
							if (-19 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = 19.f; resol_delta < -19.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				case 2:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = 119.f;
						if (feet_yaw <= 119)
						{
							if (-119 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = 119.f; resol_delta < -119.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				default:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = 90.f;
						if (feet_yaw <= 90)
						{
							if (-90 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = 90.f; resol_delta < -90.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				}
			}
			else {
				switch (csgo->missedshots[player->GetIndex()] % 3) {
				case 0:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = -90.f;
						if (feet_yaw <= -90)
						{
							if (90 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = -90.f; resol_delta < 90.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				case 1:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = -19.f;
						if (feet_yaw <= -19)
						{
							if (19 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = -19.f; resol_delta < 19.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				case 2:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = -119.f;
						if (feet_yaw <= -119)
						{
							if (119 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = -119.f; resol_delta < 119.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				default:
					for (int i = 1; i <= interfaces.global_vars->maxClients; i++)
					{
						auto player = csgo->local;
						if (!player || !player->isAlive() || player->IsDormant() || player == csgo->local)
							continue;
						auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->GetVelocity().Length2D() < 0.1f;
						auto body_max_rotation = -90.f;
						if (feet_yaw <= -90)
						{
							if (90 > feet_yaw)
								player->GetEyeAngles().y == body_max_rotation + player->GetEyeAngles().y;
						}
						else
						{
							player->GetEyeAngles().y == body_max_rotation - player->GetEyeAngles().y;
						}
						if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
						{
							for (int resol_delta = -90.f; resol_delta < 90.f; resol_delta = resol_delta - 20.f)
							{
								player->GetEyeAngles().y == resol_delta;
							}
						}
					}
					break;
				}
			}

			float newFeetYaw = 1.f;
			float v136 = fmod(newFeetYaw, 360.0);
			auto animstate = player->GetPlayerAnimState();
			const auto player_animation_state = player->GetPlayerAnimState();

			int v19 = csgo->missedshots[player->EntIndex()] % 2; // p2c bruteforce
			switch (v19)
			{
			case 0:
				animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 58.0);
				break;
			case 1:
				animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 116.0);
				break;
			case 2:
				animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 86.0);
				break;
			case 3:
				animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 172.0);
				break;
			default:
				return;

				player_animation_state->m_flGoalFeetYaw = v136;
				animstate->m_flGoalFeetYaw = Math::NormalizeYaw(animstate->m_flGoalFeetYaw);
			}

			if (WEAPON_SCAR20)
			{

				auto animstate = player->GetPlayerAnimState();

				const auto player_animation_state = player->GetPlayerAnimState();

				float newFeetYaw = 1.f;


				if (!player_animation_state)
					return;


				float v136 = fmod(newFeetYaw, 360.0);


				float v6 = 0;
				for (size_t i = 0; i < player->GetNumAnimOverlays(); i++) // hi yougame i hacked skeet
				{
					auto animLayer = player->GetAnimOverlay(i);
					if (!animLayer)
						continue;
					if (player->GetSequenceActivity(animLayer->m_nSequence) == 979);
					auto v6 = player->GetBody();
				}

				int v19 = csgo->missedshots[player->EntIndex()] % 2; // p2c bruteforce
				switch (v19)
				{
				case 0:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 58.0);
					break;
				case 1:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 116.0);
					break;
				case 2:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 86.0);
					break;
				case 3:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 172.0);
					break;
				default:
					return;



					player_animation_state->m_flGoalFeetYaw = v136;
				}


				animstate->m_flGoalFeetYaw = Math::NormalizeYaw(animstate->m_flGoalFeetYaw);
			}

			else if (WEAPON_G3SG1)

			{
				auto animstate = player->GetPlayerAnimState();

				const auto player_animation_state = player->GetPlayerAnimState();

				float newFeetYaw = 1.f;


				if (!player_animation_state)
					return;


				float v136 = fmod(newFeetYaw, 360.0);


				float v6 = 0;
				for (size_t i = 0; i < player->GetNumAnimOverlays(); i++) // hi yougame i hacked skeet
				{
					auto animLayer = player->GetAnimOverlay(i);
					if (!animLayer)
						continue;
					if (player->GetSequenceActivity(animLayer->m_nSequence) == 979);
					auto v6 = player->GetBody();
				}

				int v19 = csgo->missedshots[player->EntIndex()] % 2; // p2c bruteforce
				switch (v19)
				{
				case 0:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 58.0);
					break;
				case 1:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 116.0);
					break;
				case 2:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + laith_num_rand(0.0, 86.0);
					break;
				case 3:
					animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - laith_num_rand(0.0, 172.0);
					break;
				default:
					return;



					player_animation_state->m_flGoalFeetYaw = v136;
				}


				animstate->m_flGoalFeetYaw = Math::NormalizeYaw(animstate->m_flGoalFeetYaw);

			}
		}
	}
}

void ResolvePitch(IBasePlayer* player) {
	// NEM KELL ON SHOT RESOLVER TE FASZ ARCANE GECI. ONSHOTOLNI BUZIS (KAPPA)
	auto animstate = player->GetPlayerAnimState();
	if (!vars.ragebot.PitchResolver) 
		return;

	if (player->GetEyeAngles().x > 0 && player->GetEyeAngles().x >= 90.f) {
		player->SetAbsAngles(Vector(89.f, 0, 0));
	}
	else if (player->GetEyeAngles().x < 0 && player->GetEyeAngles().x <= -90.f) {
		player->SetAbsAngles(Vector(-89.f, 0, 0));
	}
	else if (player->GetEyeAngles().x < 0 && player->GetEyeAngles().x <= 60.f) {
		player->SetAbsAngles(Vector(-89.f, 0, 0));
	}
	else if (player->GetEyeAngles().x < 0 && player->GetEyeAngles().x <= -60.f) {
		player->SetAbsAngles(Vector(-89.f, 0, 0));
	}
	else if (player->GetEyeAngles().x < 0 && player->GetEyeAngles().x <= 58.f) {
		player->SetAbsAngles(Vector(-89.f, 0, 0));
	}
	else if (player->GetEyeAngles().x < 0 && player->GetEyeAngles().x <= -58.f) {
		player->SetAbsAngles(Vector(-89.f, 0, 0));
	}

	animstate->m_flGoalFeetYaw = Math::NormalizeYaw(animstate->m_flGoalFeetYaw);
}

void Run(IBasePlayer* player)
{
	auto animstate = player->GetPlayerAnimState();

	if (!player || !player->isAlive()) // HOGYHA MEGHALTÁL MI A GECIMNEK RESOLVOLJAD AZ ENEMIT TE GECI?
		return;
	if (player == csgo->local) // MIÉRT RESOLVOLJA A LOCAL PLAYERT???? LE MEGY AZ FPSED TÕLE!!! #POTATO PC
		return;
	if (player->GetTeam() == csgo->local->GetTeam()) // Anti team mate resolver // ne resolvolja a team matet fölöslegesen. KUBU, BAZD+ HOGY NEM JUTOTT AZ ESZÜNKBE???
		return;
	if (player->GetChokedPackets() <= 1)
		return;
	if (player->GetOldSimulationTime() > player->GetSimulationTime())
		return;
	player_info_t info;
	if (!interfaces.engine->GetPlayerInfo(player->GetIndex(), &info))
		return;



	ResolvePitch(player);
	animstate->m_flGoalFeetYaw = Math::NormalizeYaw(animstate->m_flGoalFeetYaw);

}

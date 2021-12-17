#include "Legit Aimbot.h"
#include "../Aimbot/Aimbot.h"
#include "../../Menu/Menu.h"
#include "../../Menu/config.h"
#include "../Visuals/ESP.h"
#include "..\..\SDK\IVModelInfo.h"
float m_fbestfov = 20.0f;
int m_ibesttargetlegit = -1;

float deltaTime;
float curAimTime;

LegitAimbot g_LegitAimbot;

void BackTrack::Update(int tick_count)
{
	if (!c_config::get().fakelag)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{
	int delta = latest_tick - tick;
	float deltaTime = delta * g_pGlobalVars->intervalPerTick;
	return (fabs(deltaTime) <= 0.2f);
}

mstudiobbox_t* get_hitboxv3(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}

Vector GetHitboxPositionv3(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitboxv3(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}


void BackTrack::UpdateRecord(int i)
{
	C_BaseEntity* pEntity = g_pEntityList->GetClientEntity(i);
	if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
	{
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = GetHitboxPositionv3(pEntity, 0);
		}
	}
	else
	{
		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CUserCmd * cmd, Vector & aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f * static_cast<float>(M_PI));

	return Vector(cp * cy, cp * sy, -sp);
}

float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}


void BackTrack::legitBackTrack(CUserCmd * cmd, C_BaseEntity * pLocal)
{
	if (c_config::get().legit_aimbot_backtrack)
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		PlayerInfo_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < g_pGlobalVars->maxClients; i++)
		{
			auto entity = (C_BaseEntity*)g_pEntityList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!g_pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == pLocal->GetTeam())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPositionv3(entity, 0);

				headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];

void LegitAimbot::Aim_CreateMove(CUserCmd * cmd)
{

	DropTarget();

	static float oldServerTime = Globals::LocalPlayer->GetTickBase() * g_pGlobalVars->intervalPerTick;
	float serverTime = Globals::LocalPlayer->GetTickBase() * g_pGlobalVars->intervalPerTick;
	deltaTime = serverTime - oldServerTime;
	oldServerTime = serverTime;

	if (!Globals::LocalPlayer->GetActiveWeapon())
		return;

	if (Globals::LocalPlayer->GetActiveWeapon()->IsKnifeorNade() || Globals::LocalPlayer->GetActiveWeapon()->GetAmmo() == 0)
		return;

	if ((!(Globals::pCmd->buttons & IN_ATTACK) || m_ibesttargetlegit == -1) && c_config::get().legit_aimbot_enabled)
		FindTarget();


	if (m_ibesttargetlegit != -1 && GetAsyncKeyState(VK_LBUTTON))
		GoToTarget();

	if (c_config::get().rcs && c_config::get().legit_aimbot_enabled)
		RCS();
}

void LegitAimbot::FindTarget()
{
	m_fbestfov = c_config::get().fov;

	for (int i = 0; i <= g_pGlobalVars->maxClients; i++)
	{
		auto Entity = g_pEntityList->GetClientEntity(i);

		if (!Entity->is_valid(Globals::LocalPlayer))
			continue;

		Vector viewangles = Globals::pCmd->viewangles + Globals::LocalPlayer->GetAimPunchAngle() * 2.f;

		float fov = g_Math.GetFov(Globals::pCmd->viewangles,
		                   g_Math.CalcAngle(Globals::LocalPlayer->GetEyePosition(),
		                            Entity->GetBonePosition(8)));
		if (fov < m_fbestfov)
		{
			m_fbestfov = fov;
			m_ibesttargetlegit = i;
		}
	}
}

void LegitAimbot::GoToTarget()
{
	auto Entity = g_pEntityList->GetClientEntity(m_ibesttargetlegit);

	if (!Entity)
		return;

	Vector predicted = Entity->GetPredicted(Entity->GetBonePosition(8));

	Vector dst = g_Math.CalcAngle(Globals::LocalPlayer->GetEyePosition(), predicted);
	Vector src = Globals::pCmd->viewangles;

	dst.x -= Globals::LocalPlayer->GetAimPunchAngle().x * (c_config::get().rcs_x / 50.f);
	dst.y -= Globals::LocalPlayer->GetAimPunchAngle().y * (c_config::get().rcs_y / 50.f);

	Vector delta = dst - src;

	delta.Clamp();

	if (!delta.IsZero())
	{
		float finalTime = delta.Length() / (c_config::get().linear_progression_threshold / 5);

		curAimTime += deltaTime;

		if (curAimTime > finalTime)
			curAimTime = finalTime;

		float percent = curAimTime / finalTime;

		delta *= percent;
		dst = src + delta;
	}

	g_pEngine->SetViewAngles(dst.Clamp());
}

void LegitAimbot::DropTarget()
{
	if (!EntityIsValid(m_ibesttargetlegit))
	{
		m_ibesttargetlegit = -1;
		curAimTime = 0.f;
	}
}

bool LegitAimbot::EntityIsValid(int i)
{
	auto Entity = g_pEntityList->GetClientEntity(i);

	if (!Entity->is_valid())
		return false;
	
	if (g_Math.GetFov(Globals::pCmd->viewangles, g_Math.CalcAngle(Globals::LocalPlayer->GetEyePosition(), Entity->GetBonePosition(c_config::get().legit_aimbot_priority_hitbox))) > c_config::get().legit_aimbot_fov)
		return false;

	return true;
}

Vector m_oldangle;

void LegitAimbot::RCS()
{
	auto weap = Globals::LocalPlayer->GetActiveWeapon();
	if (weap->IsPistol() || weap->IsSniper() || weap->IsShotgun())
		return;

	if (Globals::pCmd->buttons & IN_ATTACK)
	{
		Vector aimpunch = Globals::LocalPlayer->GetAimPunchAngle() * 2.f;

		Globals::pCmd->viewangles += (m_oldangle - aimpunch);

		m_oldangle = aimpunch;
	}
	else
	{
		m_oldangle.x = m_oldangle.y = m_oldangle.z = 0;
	}
}
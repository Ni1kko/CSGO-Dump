#include "RageBot.h"
#include "autowall.hpp"
#include "..//menu.hpp"

static std::vector<std::tuple<float, float, float>> precomputed_seeds = {};

void CRagebot::Run(CUserCmd* pCmd, bool& bSendPacket)
{
	if (g_EngineClient->IsConnected() && g_EngineClient->IsInGame())
	{
		if (!g_LocalPlayer || !g_LocalPlayer->IsAlive()) return;

		auto m_weapon = g_LocalPlayer->m_hActiveWeapon();
		if (m_weapon)
		{
			for (auto i = 0; i < 255; i++) {
				Math::RandomSeed(i + 1);

				const auto pi_seed = Math::RandomFloat(0.f, M_PI * 2);

				precomputed_seeds.emplace_back(Math::RandomFloat(0.f, 1.f),
					sin(pi_seed), cos(pi_seed));
			}

			if (g_Options.ragebotconfig.bEnabled)
			{
				predict(pCmd);
				Slowwalk(pCmd);
				AntiAim(pCmd, bSendPacket);
				if (m_weapon->IsGun())
				{
					if (m_weapon->m_iClip1() > 0)
					{
						if (g_LocalPlayer->IsAlive())
						{
							aimbotted_in_current_tick = false;
							DoAimbot(pCmd, bSendPacket);

							if (m_weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_REVOLVER)
							{
								pCmd->buttons |= IN_ATTACK;
								if (!aimbotted_in_current_tick)
								{
									if (m_weapon->m_flPostponeFireReadyTime() > 0 && m_weapon->m_flPostponeFireReadyTime() - 1.f < g_GlobalVars->curtime)
									{
										pCmd->buttons &= ~IN_ATTACK;
									}
								}
							}
						}
					}
					else
					{
						pCmd->buttons &= ~IN_ATTACK;
						pCmd->buttons |= IN_RELOAD;
					}
				}
				Math::FixAngles(pCmd->viewangles);
			}
		}
	}
}

float CRagebot::hitchance(QAngle angles, C_BasePlayer* ent, float chance)
{
	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();

	if (!weapon)
		return false;

	if (g_CVar->FindVar("weapon_accuracy_nospread")->GetInt())
		return true;

	const auto round_acc = [](const float accuracy) { return roundf(accuracy * 1000.f) / 1000.f; };
	const auto sniper = g_LocalPlayer->m_hActiveWeapon()->IsSniper();
	const auto crouched = g_LocalPlayer->m_fFlags() & FL_DUCKING;
	const auto weapon_inaccuracy = weapon->GetInaccuracy();

	if (g_LocalPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex() == WEAPON_REVOLVER)
		return weapon_inaccuracy < (crouched ? .0020f : .0055f);

	if (crouched)
	{
		if (round_acc(weapon_inaccuracy) == round_acc(sniper ? weapon->GetCSWeaponData()->flInaccuracyCrouchAlt : weapon->GetCSWeaponData()->flInaccuracyCrouch))
			return true;
	}
	else
	{
		if (round_acc(weapon_inaccuracy) == round_acc(sniper ? weapon->GetCSWeaponData()->flSpreadAlt : weapon->GetCSWeaponData()->flSpread))
			return true;
	}

	Vector forward, right, up;
	Vector src = g_LocalPlayer->GetEyePos();
	Math::AngleVectors(angles, forward, right, up);

	int cHits = 0;
	int cNeededHits = static_cast<int> (256.f * (chance / 100.f));

	weapon->UpdateAccuracyPenalty();
	float weap_spread = weapon->GetSpread();
	float weap_inaccuracy = weapon->GetInaccuracy();

	for (int i = 0; i < 256; i++)
	{
		float a = Math::RandomFloat(0.f, 1.f);
		float b = Math::RandomFloat(0.f, 2.f * PI_F);
		float c = Math::RandomFloat(0.f, 1.f);
		float d = Math::RandomFloat(0.f, 2.f * PI_F);

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalized();

		QAngle viewAnglesSpread;
		Math::VectorAngles1337(direction, up, viewAnglesSpread);
		viewAnglesSpread.Normalize();

		Vector viewForward;
		Math::AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = src + (viewForward * weapon->GetCSWeaponData()->flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, viewForward);
		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, ent, &tr);

		if (tr.hit_entity == ent)
			++cHits;

		if (static_cast<int> ((static_cast<float> (cHits) / 256.f) * 100.f) > chance)
			return true;
	}

	return false;
}

float CRagebot::FovToPlayer(Vector AimPos)
{
	QAngle viewAngles;
	g_EngineClient->GetViewAngles(&viewAngles);
	CONST FLOAT MaxDegrees = 360.0f;
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);
	Math::AngleVectors(viewAngles, Forward);
	VectorSubtractForFOV(AimPos, g_LocalPlayer->GetEyePos(), Delta);
	Normalize(Delta, Delta);
	FLOAT DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI_F));
}

int CRagebot::GetTargetFOV()
{
	int target = -1;
	float mfov = FLT_MAX;
	for (int i = 0; i < g_GlobalVars->maxClients; i++)
	{
		C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);
		if (IsViable(entity))
		{
			Vector aimpoint = HitScan(entity);
			if (aimpoint != Vector(0, 0, 0))
			{
				float fov = FovToPlayer(aimpoint);
				if ((fov < mfov) && fov < (float)g_Options.ragebotconfig.iMaxFOV)
				{
					mfov = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRagebot::GetTargetDistance()
{
	int target = -1;
	int minDist = INT_MAX;

	C_BasePlayer* pLocal = g_LocalPlayer;

	for (int i = 0; i < g_GlobalVars->maxClients; i++)
	{
		C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);
		if (IsViable(entity))
		{
			Vector aimpoint = HitScan(entity);
			if (aimpoint != Vector(0, 0, 0))
			{			
				float fov = FovToPlayer(aimpoint);
				Vector Difference = pLocal->m_vecOrigin() - entity->m_vecOrigin();
				int Distance = Difference.Length();
				if ((Distance < minDist) && fov < (float)g_Options.ragebotconfig.iMaxFOV)
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRagebot::GetTargetHealth()
{
	int target = -1;
	int minHealth = INT_MAX;

	C_BasePlayer* pLocal = g_LocalPlayer;

	for (int i = 0; i < g_GlobalVars->maxClients; i++)
	{
		C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);
		if (IsViable(entity))
		{
			Vector aimpoint = HitScan(entity);
			if (aimpoint != Vector(0, 0, 0))
			{
				float fov = FovToPlayer(aimpoint);
				int Health = entity->m_iHealth();
				if ((Health < minHealth) && fov < (float)g_Options.ragebotconfig.iMaxFOV)
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

bool CRagebot::IsViable(C_BasePlayer* entity)
{
	if (entity && entity->IsDormant() == false && entity->IsAlive() && entity->EntIndex() != g_LocalPlayer->EntIndex())
	{
		ClientClass* pClientClass = entity->GetClientClass();
		if (pClientClass->m_ClassID == ClassId->CCSPlayer)
		{
			if (entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() || g_Options.ragebotconfig.bFriendlyFire)
			{
				if (!entity->m_bGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	return false;
}

Vector CRagebot::HitScan(C_BasePlayer* m_entity)
{
	std::vector<int> hitboxes;
	std::vector<int> baim_hitboxes;
	int best_damage = 1;
	Vector best_position;

	baim_hitboxes.emplace_back(HITBOX_UPPER_CHEST);
	baim_hitboxes.emplace_back(HITBOX_CHEST);
	baim_hitboxes.emplace_back(HITBOX_LOWER_CHEST);
	baim_hitboxes.emplace_back(HITBOX_STOMACH);
	baim_hitboxes.emplace_back(HITBOX_PELVIS);

	bool canseebody = false;
	int bodydmg;

	for (auto HitBoxID : baim_hitboxes)
	{
		Vector aimpoint = m_entity->GetHitboxPos(HitBoxID);
		if (c_autowall::get()->autowall(g_LocalPlayer->GetEyePos(), aimpoint, g_LocalPlayer, m_entity).damage >= bodydmg)
		{
			canseebody = true;
			bodydmg = c_autowall::get()->autowall(g_LocalPlayer->GetEyePos(), aimpoint, g_LocalPlayer, m_entity).damage;
		}
	}

	if (g_Options.ragebotconfig.head)
		hitboxes.emplace_back(HITBOX_HEAD);

	if (g_Options.ragebotconfig.chest)
	{
		hitboxes.emplace_back(HITBOX_UPPER_CHEST);
		hitboxes.emplace_back(HITBOX_CHEST);
		hitboxes.emplace_back(HITBOX_LOWER_CHEST);
	}

	if (g_Options.ragebotconfig.body)
	{
		hitboxes.emplace_back(HITBOX_STOMACH);
		hitboxes.emplace_back(HITBOX_PELVIS);
	}

	if (g_Options.ragebotconfig.hands)
	{
		hitboxes.emplace_back(HITBOX_LEFT_FOREARM);
		hitboxes.emplace_back(HITBOX_LEFT_HAND);
		hitboxes.emplace_back(HITBOX_LEFT_UPPER_ARM);

		hitboxes.emplace_back(HITBOX_RIGHT_FOREARM);
		hitboxes.emplace_back(HITBOX_RIGHT_HAND);
		hitboxes.emplace_back(HITBOX_RIGHT_UPPER_ARM);
	}

	if (g_Options.ragebotconfig.legs)
	{
		hitboxes.emplace_back(HITBOX_LEFT_FOOT);
		hitboxes.emplace_back(HITBOX_LEFT_CALF);
		hitboxes.emplace_back(HITBOX_LEFT_THIGH);

		hitboxes.emplace_back(HITBOX_RIGHT_FOOT);
		hitboxes.emplace_back(HITBOX_LEFT_CALF);
		hitboxes.emplace_back(HITBOX_LEFT_THIGH);
	}

	for (auto HitBoxID : hitboxes)
	{
		if ((HitBoxID ==

			HITBOX_HEAD || HitBoxID == HITBOX_LEFT_FOREARM || HitBoxID == HITBOX_LEFT_HAND || HitBoxID ==

			HITBOX_LEFT_UPPER_ARM || HitBoxID == HITBOX_RIGHT_FOREARM || HitBoxID == HITBOX_RIGHT_HAND ||

			HitBoxID == HITBOX_RIGHT_UPPER_ARM || HitBoxID == HITBOX_LEFT_FOOT || HitBoxID ==

			HITBOX_LEFT_CALF || HitBoxID == HITBOX_LEFT_THIGH || HitBoxID == HITBOX_RIGHT_FOOT ||

			HitBoxID == HITBOX_LEFT_CALF || HitBoxID == HITBOX_LEFT_THIGH) && canseebody) {
			if (g_Options.ragebotconfig.bPreferBaim)
			{
				if (!(m_entity->m_fFlags() & FL_ONGROUND))
					continue;
			}
			if (g_Options.ragebotconfig.flBaimOnX)
			{
				if (m_entity->m_iHealth() <= g_Options.ragebotconfig.flBaimOnX)
					continue;
			}
			if (g_Options.ragebotconfig.bBaimIfDeadly)
			{
				if (m_entity->m_iHealth() <= bodydmg)
					continue;
			}
		}
		Vector aimpoint = m_entity->GetHitboxPos(HitBoxID);
		if (g_Options.ragebotconfig.bAutoWall) {
			float damage = c_autowall::get()->autowall(g_LocalPlayer->GetEyePos(), aimpoint, g_LocalPlayer, m_entity).damage;
			if (damage > best_damage && damage > std::min<int>(g_Options.ragebotconfig.flMinDmg, m_entity->m_iHealth())) {
				best_damage = damage;
				best_position = aimpoint;
				return best_position;
			}
		}
		else {
			if (g_LocalPlayer->CanSeePlayer(m_entity, aimpoint))
			{
				float damage = c_autowall::get()->autowall(g_LocalPlayer->GetEyePos(), aimpoint, g_LocalPlayer, m_entity).damage;
				if (damage > best_damage && damage > std::min<int>(g_Options.ragebotconfig.flMinDmg, m_entity->m_iHealth())) {
					best_damage = damage;
					best_position = aimpoint;
					return best_position;
				}
			}
		}
	}

	return best_position;
}
void CRagebot::DoAimbot(CUserCmd* pCmd, bool& bSendPacket)
{
	bool FindNewTarget = true;

	auto pWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!pWeapon)
		return;

	auto weapon_data = pWeapon->GetCSWeaponData();
	if (!weapon_data)
		return;

	if (pWeapon)
	{
		if (pWeapon->m_iClip1() == 0 || pWeapon->IsKnife() || pWeapon->IsGrenade())
			return;
	}
	else
		return;


	if (IsLocked && TargetID > -0 && HitBox >= 0)
	{
		pTarget = C_BasePlayer::GetPlayerByIndex(TargetID);
		if (pTarget && IsViable(pTarget))
		{
			Vector aimpoint = HitScan(pTarget);
			if (aimpoint != Vector(0, 0, 0))
			{
				float fov = FovToPlayer(aimpoint);
				if (fov < (float)g_Options.ragebotconfig.iMaxFOV)
					FindNewTarget = false;
			}
		}
	}
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;
		switch (g_Options.ragebotconfig.iTargetSelection)
		{
		case 0: TargetID = GetTargetFOV();
		case 1: TargetID = GetTargetDistance();
		case 2: TargetID = GetTargetHealth();
		}

		if (TargetID >= 0)
			pTarget = C_BasePlayer::GetPlayerByIndex(TargetID);
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}
	if (TargetID >= 0 && pTarget)
	{
		if (!pWeapon->CanFire())
			return;

		Vector aimpoint = HitScan(pTarget);
		if (aimpoint != Vector(0, 0, 0))
		{
			QAngle pointtohit = Math::CalcAngle(g_LocalPlayer->GetEyePos(), aimpoint);
			if (g_Options.ragebotconfig.bRemoveRecoil)
			{
				QAngle recoilcontol = (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat());
				pointtohit -= recoilcontol;
			}

			if (g_Options.ragebotconfig.bHitchance && hitchance(pointtohit, pTarget, g_Options.ragebotconfig.flHitchanceAmt) || !g_Options.ragebotconfig.bHitchance)
			{
				IsLocked = true;
				pCmd->viewangles = pointtohit;
				if (!g_Options.ragebotconfig.iAimbotMode) {
					g_EngineClient->SetViewAngles(&pointtohit);
				}
				if (!g_Options.ragebotconfig.bAutoFire && !(pCmd->buttons & IN_ATTACK))
					return;

				if (g_Options.ragebotconfig.bAutoFire)
				{
					pCmd->buttons |= IN_ATTACK;
					aimbotted_in_current_tick = true;
				}
			}
			else
			{
				if (g_Options.ragebotconfig.bAutoScope && pWeapon->IsSniper() && !g_LocalPlayer->m_bIsScoped())
				{
					pCmd->buttons |= IN_ATTACK2;
					return;
				}
				if (g_Options.ragebotconfig.bAutostop)
				{
					if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
						return;

					Vector velocity = g_LocalPlayer->m_vecVelocity();
					QAngle direction;

					Math::VectorAngles(velocity, direction);

					float speed = velocity.Length2D();

					direction.yaw = pCmd->viewangles.yaw - direction.yaw;

					Vector forward;

					Math::AngleVectors(direction, forward);

					Vector source = forward * -speed;

					const float factor = std::max(source.x, source.y) / 450.f;
					source *= factor;

					if (speed > ((g_LocalPlayer->m_bIsScoped() ? weapon_data->flSpread : weapon_data->flSpreadAlt) * .17f))
					{
						pCmd->forwardmove = source.x;
						pCmd->sidemove = source.y;
					}

				}
			}
		}
	}
	return;
}
void CRagebot::AntiAim(CUserCmd* cmd, bool& bSendPacket) {
	if (cmd->buttons & IN_USE)
		return;

	if (g_LocalPlayer->m_nMoveType() & MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() & MOVETYPE_NOCLIP)
		return;

	const auto state = g_LocalPlayer->GetPlayerAnimState();

	if (!state)
		return;

	if (g_LocalPlayer->m_hActiveWeapon()->IsKnife()) {
		if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
			return;
	}
	else
	{
		if (!g_LocalPlayer->m_hActiveWeapon()->IsGrenade()) {
			if (g_LocalPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex() != WEAPON_REVOLVER) {
				if (!g_LocalPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex() != WEAPON_C4) {
					if (cmd->buttons & IN_ATTACK && g_LocalPlayer->m_hActiveWeapon()->CanFire()) {
						return;
					}
				}
				else
				{
					if (cmd->buttons & IN_ATTACK) {
						return;
					}
				}
			}
			else {
				if (cmd->buttons & IN_ATTACK2)
					return;

				if (CanFirePostpone() && (cmd->buttons & IN_ATTACK))
					return;
			}
		}
		else
		{
			if (g_LocalPlayer->m_hActiveWeapon()->m_fThrowTime() > 0)
				return;
		}
	}

	cmd->viewangles.pitch = g_Options.ragebotconfig.pitch;
	cmd->viewangles.yaw += g_Options.ragebotconfig.yaw + Math::RandomFloat(g_Options.ragebotconfig.jittermin, g_Options.ragebotconfig.jittermax);
	if (g_Options.ragebotconfig.desync)
	{
		if (g_InputSys->WasKeyPressed(g_Options.ragebotconfig.switchdesyncside) && !g_Menu->IsVisible())
		{
			desyncside = -desyncside;
		}
		float desyncvalue = (g_LocalPlayer->GetMaxDesyncDelta() * (desyncside == 1 ? ((float)g_Options.ragebotconfig.bodylean / 100.f) : ((float)g_Options.ragebotconfig.invbodylean / 100.f))) * desyncside;
		float lbybreakvalue = (180.f - g_LocalPlayer->GetMaxDesyncDelta()) * -desyncside;
		if (g_Options.ragebotconfig.desynctype == 0)
		{
			if (break_lby)
			{
				if (bSendPacket) {
					bSendPacket = false;
				}
				else
				{
					cmd->viewangles.yaw += lbybreakvalue;					
				}
			}
			else
			{
				if (bSendPacket)
				{
					cmd->viewangles.yaw += desyncvalue;
				}
				else
				{
					cmd->viewangles.yaw -= desyncvalue;
				}
			}
		}
		else if (g_Options.ragebotconfig.desynctype == 1)
		{
			if (!(cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT || cmd->buttons & IN_JUMP) && g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			{
				cmd->sidemove = cmd->tick_count % 2 ? g_LocalPlayer->m_fFlags() & IN_DUCK ? 3.0f : 1.10 : g_LocalPlayer->m_fFlags() & IN_DUCK ? -3.0f : -1.10;
			}
			if (bSendPacket)
			{
				cmd->viewangles.yaw += desyncvalue;
			}
			else
			{
				cmd->viewangles.yaw -= desyncvalue;
			}
		}
	}
}
void CRagebot::predict(CUserCmd* pCmd)
{
	const auto state = g_LocalPlayer->GetPlayerAnimState();

	if (!state)
		return;

	float server_time = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	if (state->speed_2d > 0.1)
	{
		next_lby_update = server_time + 0.22;
	}

	break_lby = false;

	if (next_lby_update <= server_time) {
		next_lby_update = server_time + 1.1;
		break_lby = true;
	}

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
	{
		break_lby = false;
	}
}

void CRagebot::Slowwalk(CUserCmd* pCmd) {
	if (!g_Options.ragebotconfig.slowwalk || !g_InputSys->IsKeyDown(g_Options.ragebotconfig.slowwalkkey) || g_Menu->IsVisible())
		return;

	auto weapon_handle = g_LocalPlayer->m_hActiveWeapon();

	if (!weapon_handle)
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		return;

	float amount = 0.0034f * g_Options.ragebotconfig.slowwalkspeed;

	Vector velocity = g_LocalPlayer->m_vecVelocity();
	QAngle direction;

	Math::VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.yaw = pCmd->viewangles.yaw - direction.yaw;

	Vector forward;

	Math::AngleVectors(direction, forward);

	Vector source = forward * -speed;

	if (speed >= (weapon_handle->GetCSWeaponData()->flMaxPlayerSpeed * amount))
	{
		pCmd->forwardmove = source.x;
		pCmd->sidemove = source.y;
	}
}

void CRagebot::AnimationFix(C_BasePlayer* entity)
{
	auto state = entity->GetPlayerAnimState();
	if (!state) return;

	const float curtime = g_GlobalVars->curtime;
	const float frametime = g_GlobalVars->frametime;
	static auto host_timescale = g_CVar->FindVar(("host_timescale"));

	g_GlobalVars->frametime = g_GlobalVars->interval_per_tick * host_timescale->GetFloat();
	g_GlobalVars->curtime = entity->m_flSimulationTime() + g_GlobalVars->interval_per_tick;

	Vector backup_velocity = entity->m_vecVelocity();

	int backup_flags = entity->m_fFlags();
	int backup_eflags = entity->m_iEFlags();

	AnimationLayer backup_layers[15];
	std::memcpy(backup_layers, entity->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));

	state->m_bOnGround ? entity->m_fFlags() |= FL_ONGROUND : entity->m_fFlags() &= ~FL_ONGROUND;

	entity->m_iEFlags() &= ~0x1000;

	entity->m_vecAbsVelocity() = entity->m_vecVelocity();

	if (state->m_iLastClientSideAnimationUpdateFramecount == g_GlobalVars->framecount)
		state->m_iLastClientSideAnimationUpdateFramecount = g_GlobalVars->framecount - 1;

	*(bool*)((DWORD)g_ClientState + 0x4D40) = true;
	entity->m_bClientSideAnimation() = true;

	if (!entity->m_nMoveType() & MOVETYPE_LADDER && !entity->m_nMoveType() & MOVETYPE_NOCLIP)
	{
		if (!entity->GetPlayerInfo().fakeplayer)
		{
			//resolver
		}
	}

	entity->UpdateClientSideAnimation();
	entity->m_bClientSideAnimation() = false;
	*(bool*)((DWORD)g_ClientState + 0x4D40) = false;

	std::memcpy(entity->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));

	entity->m_vecVelocity() = backup_velocity;
	entity->m_fFlags() = backup_flags;
	entity->m_iEFlags() = backup_eflags;

	g_GlobalVars->curtime = curtime;
	g_GlobalVars->frametime = frametime;

	if (entity->m_flSimulationTime() >= entity->m_flOldSimulationTime())
	{
		entity->InvalidateBoneCache();
		entity->SetupBones(nullptr, -1, 0x7FF00, g_GlobalVars->curtime);
	}
}
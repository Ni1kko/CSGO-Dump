#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include "position_adjust.h"
#include <iostream>
#include <time.h>
#include "UTIL Functions.h"
#include "xostr.h"
#include <chrono>
#include "Hooks.h"
#include "global_count.h"
#include "laggycompensation.h"
#include "MD5.cpp"
//#include "otr_awall.h"
#include "antiaim.h"
#include "fakelag.h"
#include "experimental.h"

//float bigboi::current_yaw;

float current_desync;
Vector LastAngleAA2;
static bool dir = false;
static bool back = false;
static bool up = false;
static bool jitter = false;

static bool backup = false;
static bool default_aa = true;
static bool panic = false;
float hitchance_custom;
#define TICK_INTERVAL			(Interfaces::Globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
CRageBot * ragebot = new CRageBot;
void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{
}
float curtime_fixed(CUserCmd* ucmd) {
	auto local_player = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}

void RandomSeed(UINT seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(seed);
	return;
}

void CRageBot::auto_revolver(CUserCmd* m_pcmd) // credits: https://steamcommunity.com/id/x-87
{
	auto m_local = hackManager.pLocal();
	auto m_weapon = m_local->GetWeapon2();
	if (!m_weapon)
		return;

	if (!shot_this_tick && *m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
	{

		float flPostponeFireReady = m_weapon->GetFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady - 1 < Interfaces::Globals->curtime)
		{
			m_pcmd->buttons &= ~IN_ATTACK;
		}
		static int delay = 0;
		delay++;

		if (delay <= 15)
			m_pcmd->buttons |= IN_ATTACK;
		else
			delay = 0;

	}
	else
	{
		if (*m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			static int delay = 0;
			delay++;

			if (delay <= 15)
				m_pcmd->buttons |= IN_ATTACK;
			else
				delay = 0;
		}
	}
}


bool IsAbleToShoot(IClientEntity* pLocal)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!pLocal)return false;
	if (!pWeapon)return false;
	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}
float hitchance()
{
	float hitchance = 101;
	auto m_local = hackManager.pLocal();
	auto pWeapon = m_local->GetWeapon2();
	if (pWeapon)
	{
		if (Options::Menu.RageBotTab.AccuracyHitchance.GetValue() > 0)
		{
			float inaccuracy = pWeapon->GetInaccuracy();
			if (inaccuracy == 0) inaccuracy = 0.0000001;
			inaccuracy = 1 / inaccuracy;
			hitchance = inaccuracy;
		}
		return hitchance;
	}
}
bool CRageBot::CanOpenFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;
	C_BaseCombatWeapon* entwep = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());
	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();
	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;
	return !(flNextPrimaryAttack > flServerTime);
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

float GetLerpTimeX()
{
	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}
void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame() || !pLocalEntity->isValidPlayer())
		return;

	c_fakelag->Fakelag(pCmd, bSendPacket);

	if (Options::Menu.MiscTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = 1;
		C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;
		if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT) || (ChokedPackets < 1 && pLocalEntity->GetHealth() <= 0.f && !(pWeapon->IsKnife() || pWeapon->IsC4())))
		{
			bSendPacket = false;
		}

		else
		{
			if (pLocalEntity->IsAlive())
			{
				c_antiaim->DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = 1;
		}
	}

	if (Options::Menu.RageBotTab.AimbotEnable.GetState())
	{
		DoAimbot(pCmd, bSendPacket);
		DoNoRecoil(pCmd);
		auto_revolver(pCmd);
	}

	if (Options::Menu.MiscTab.OtherSafeMode.GetIndex() == 1)
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 39.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 39.f;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
	LastAngle = pCmd->viewangles;
}
Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, Options::Menu.RageBotTab.TargetPointscale.GetValue() / 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	final = tr.endpos;
	return final;
}
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

void CRageBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	bool vac_kick = Options::Menu.MiscTab.OtherSafeMode.GetIndex() == 1;
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	bool FindNewTarget = true;
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || pWeapon->IsKnife() || pWeapon->IsC4() || pWeapon->IsGrenade())
			return;
	}
	else
		return;

	if (IsLocked && TargetID > -0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);

			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < vac_kick ? 39.f : Options::Menu.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		Globals::Shots = 0;
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;
		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
	
	}
	
	if (TargetID >= 0 && pTarget)
	{
		float old_sim = 0.f;
		float current_sim = 0.f;
		current_sim = pTarget->GetSimulationTime();
		bool can_shoot;

		float quickspeed = Options::Menu.ColorsTab.quickstop_speed.GetValue();

		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		if (Options::Menu.MiscTab.QuickStop.GetState() && pLocal->GetFlags() & FL_ONGROUND)
			ClampMovement(pCmd, quickspeed);

		if (Options::Menu.RageBotTab.delay_shot.GetState() && old_sim != current_sim)
		{
			can_shoot = true;
			old_sim = current_sim;
		}
		else
			can_shoot = false;

		lagcomp->start_position_adjustment();
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox);
		bool IsAtPeakOfJump = fabs(pLocal->GetVelocity().z) <= 5.0f;
		float dmg = 0.f;

		AimPoint = BestPoint(pTarget, AimPoint);
		pTarget->GetPredicted(AimPoint);
		target_point = AimPoint;
		shot_this_tick = false;

		auto cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");
		auto cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
		int lerpTicks = TIME_TO_TICKS(cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat());
//		backtracking->RunLBYBackTrack(pCmd, hitbox_location(pTarget, 0), pTarget);
		lagcomp->finish_position_adjustment();

		if (Interfaces::m_iInputSys->IsButtonDown(MOUSE_LEFT)) return;

		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Options::Menu.RageBotTab.AccuracyAutoScope.GetState())
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((hitchance() >= Options::Menu.RageBotTab.AccuracyHitchance.GetValue()) * 1.5/** 1.5 && backup_awall->CanHit_2(AimPoint, &dmg)) */)
			{
		/*		CBacktracking::Get().ShotBackTrackAimbotStart(pTarget);
				CBacktracking::Get().ShotBackTrackStoreFSN(pTarget);
				CBacktracking::Get().RestoreTemporaryRecord(pTarget);
				CBacktracking::Get().ShotBackTrackedTick(pTarget); */

				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{

					if (Options::Menu.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{

						if (Options::Menu.RageBotTab.delay_shot.GetState() && can_shoot)
						{
							if (GameUtils::IsZeus(pWeapon))
							{

								if ((((pTarget->GetOrigin() - pLocal->GetOrigin()).Length() <= 115.f && pTarget->GetHealth() < 70.f) || (pTarget->GetOrigin() - pLocal->GetOrigin()).Length() <= 100.f && pTarget->GetHealth() >= 70)) //&& backup_awall->CanHit_2(AimPoint, &dmg))
								{
									pCmd->buttons |= IN_ATTACK;
								}
							}
							else
							{							
								pCmd->buttons |= IN_ATTACK;			
								pCmd->tick_count = TIME_TO_TICKS(lerpTicks + pTarget->GetSimulationTime());
								shot_this_tick = true;
							}
						}

						if (!Options::Menu.RageBotTab.delay_shot.GetState())
						{
							if (GameUtils::IsZeus(pWeapon))
							{
								if ((((pTarget->GetOrigin() - pLocal->GetOrigin()).Length() <= 115.f && pTarget->GetHealth() < 70.f) || (pTarget->GetOrigin() - pLocal->GetOrigin()).Length() <= 100.f && pTarget->GetHealth() >= 70) )
								{
									pCmd->buttons |= IN_ATTACK;
								}
							}
							else
							{
								pCmd->buttons |= IN_ATTACK;
								shot_this_tick = true;
							}
						}

						if (!(pCmd->buttons |= IN_ATTACK))
						{
							shot_this_tick = false;
						}
					}
					else if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
						return;

					if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() != 64)
					{
						static bool WasFiring = false;
						if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 31)
						{
							if (pCmd->buttons & IN_ATTACK)
							{
								if (WasFiring)
								{
									pCmd->buttons &= ~IN_ATTACK;
								}
							}
							WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
						}
					}
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK) {
			Globals::Shots += 1;
		}

	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	auto local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (pEntity->isValidPlayer())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->team() != local->team())
			{
				// Spawn Check
				if (!pEntity->has_gungame_immunity())
				{
					return true;
				}

			}
		}
	}
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{

	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);

	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	FLOAT DotProduct = Forward.Dot(Delta);
	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}
int CRageBot::GetTargetCrosshair()
{
	int target = -1;
	float minFoV = Options::Menu.RageBotTab.AimbotFov.GetValue();
	IClientEntity * pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);
	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}
	return target;
}
float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;
	AngleVectors(viewAngle, &aim);
	AngleVectors(aimAngle, &ang);
	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
bool CRageBot::should_baim(IClientEntity* pEntity)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	int health = Options::Menu.RageBotTab.BaimIfUnderXHealth.GetValue();

	if (GetAsyncKeyState(Options::Menu.RageBotTab.bigbaim.GetKey()) ||
		(Options::Menu.RageBotTab.baim_fakewalk.GetState() && resolver->enemy_fakewalk)
			|| (Options::Menu.RageBotTab.baim_fake.GetState() && resolver->enemy_fake)
			|| (Options::Menu.RageBotTab.baim_inair.GetState() && resolver->enemy_inair)
		|| pEntity->GetHealth() < health
		|| (Options::Menu.RageBotTab.baim_muslim.GetState() && pEntity->GetWeapon2()->IsGrenade()))
	{
		return true;
	}

	return false;
}


int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	float health = Options::Menu.RageBotTab.BaimIfUnderXHealth.GetValue();

	std::vector<int> HitBoxesToScan;
	std::vector<dropdownboxitem> auto_list = Options::Menu.RageBotTab.target_auto.items;
	std::vector<dropdownboxitem> scout_list = Options::Menu.RageBotTab.target_scout.items;
	std::vector<dropdownboxitem> awp_list = Options::Menu.RageBotTab.target_awp.items;
	std::vector<dropdownboxitem> pistol_list = Options::Menu.RageBotTab.target_pistol.items;
	std::vector<dropdownboxitem> otr_list = Options::Menu.RageBotTab.target_otr.items;

//	Vector enemy_head = hitbox_location(pEntity, 0);

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	float dmg = Options::Menu.RageBotTab.AccuracyMinimumDamage.GetValue() + 1; // if we can do more than our min damage

	if (pWeapon != nullptr)
	{
	
		if (GameUtils::AutoSniper(pWeapon))
		{
			if (auto_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOOT);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOOT);
			}

			if (auto_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOREARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOREARM);
			}

			if (auto_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
			}

			if (auto_list[1].GetSelected && !should_baim(pEntity)) // Upper Body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_NECK);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			}

			if (auto_list[0].GetSelected && !should_baim(pEntity)) // Head
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_HEAD);
			}	
		}

		if (pWeapon->isScout())
		{
			if (scout_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_HEAD);
			}

			if (scout_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_NECK);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			}

			if (scout_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
			}

			if (scout_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOREARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOREARM);
			}

			if (scout_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOOT);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOOT);
			}
		}

		if (pWeapon->isAwp())
		{
			if (awp_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_HEAD);
			}

			if (awp_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_NECK);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			}

			if (awp_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
			}

			if (awp_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOREARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOREARM);
			}

			if (awp_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOOT);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOOT);
			}
		}

		if (GameUtils::IsPistol(pWeapon))
		{

			if (pistol_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_HEAD);
			}

			if (pistol_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_NECK);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			}

			if (pistol_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
			}

			if (pistol_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOREARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOREARM);
			}

			if (pistol_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOOT);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOOT);
			}
		}

		if (GameUtils::IsRifle(pWeapon) || GameUtils::IsShotgun(pWeapon) || GameUtils::IsMP(pWeapon) || GameUtils::IsMachinegun(pWeapon) && !should_baim(pEntity))
		{
			if (otr_list[0].GetSelected) // Head
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_HEAD);
			}

			if (otr_list[1].GetSelected) // Upper Body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_NECK);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			}

			if (otr_list[2].GetSelected) // Lower body
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
			}

			if (otr_list[3].GetSelected) // Arms
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_UPPER_ARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOREARM);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOREARM);
			}

			if (otr_list[4].GetSelected) // Legs
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_CALF);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_FOOT);
				HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_FOOT);
			}
		}

		if (GameUtils::IsZeus(pWeapon) && !should_baim(pEntity))
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_UPPER_CHEST);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
		}

		if (should_baim(pEntity))
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LOWER_CHEST);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_BELLY);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_PELVIS);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_LEFT_THIGH);
			HitBoxesToScan.push_back((int)CSGOHitboxID::HITBOX_RIGHT_THIGH);
		}

		for (auto HitBoxID : HitBoxesToScan)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
		
			float dmg = 0.f;
		//	if (new_autowall->PenetrateWall(pEntity, Point))
			if (backup_awall->CanHit_2(Point, &dmg))
		
			{
				return HitBoxID;
			}
		}
		return -1;
	}
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{

	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2.00;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);		
		}
	}
}
void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->range;

	IClientEntity* target = nullptr;

	for (int i = 0; i <= Interfaces::Engine->GetMaxClients(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();
			Vector CurPos = target->GetEyePosition() + target->GetAbsOrigin();

			float temp_dist = eye_position.DistTo(target_position);
			QAngle angle = QAngle(0, 0, 0);
			float lowest = 99999999.f;
			if (CurPos.DistToSqr(eye_position) < lowest)
			{
				lowest = CurPos.DistTo(eye_position);
				CalcAngle(eye_position, target_position, angle);
			}
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;
	if (point.Length() == 0) return ReturnValue;
	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);
	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}
	IsLocked = true;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle;
	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);
	Vector AddAngs = angles - LastAimstepAngle;
	if (fovLeft > 29.0f && Options::Menu.MiscTab.OtherSafeMode.GetIndex() == 1)
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 29.0f;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}
	if (Options::Menu.RageBotTab.AimbotSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}
	if (!Options::Menu.RageBotTab.AimbotSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	if (Options::Menu.MiscTab.choked_shot.GetState())
	{
		Vector oViewangles = pCmd->viewangles;
		float oForwardmove = pCmd->forwardmove;
		float oSidemove = pCmd->sidemove;

		static auto choked = 0.f;
		if (CanOpenFire() && pCmd->buttons & IN_ATTACK)
		{
			bSendPacket = false;
			choked++;
		}
		//So we dont kill ourselfs
		if (choked > 2)
			bSendPacket = true;//this will overwrite fakelag values when psilent is on
		if (bSendPacket)
		{
			choked = 0;
			pCmd->viewangles = oViewangles;
			pCmd->forwardmove = oForwardmove;
			pCmd->sidemove = oSidemove;
		}
	}

	return ReturnValue;
}

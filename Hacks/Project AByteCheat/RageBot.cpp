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
#include "lin_extp.h"
#include "MiscHacks.h"
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
#define TICK_INTERVAL			(interfaces::globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
CAimbot * ragebot = new CAimbot;
void CAimbot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CAimbot::Draw()
{
}
float curtime_fixed(CUserCmd* ucmd) {
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::globals->interval_per_tick;
	return curtime;
}

void RandomSeed(UINT seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(seed);
	return;
}

void CAimbot::auto_revolver(CUserCmd* m_pcmd) // credits: https://steamcommunity.com/id/x-87
{
	auto m_local = hackManager.pLocal();
	auto m_weapon = m_local->GetWeapon2();
	if (!m_weapon)
		return;

	if (!shot_this_tick && *m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
	{

		float flPostponeFireReady = m_weapon->GetFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady - 1 < interfaces::globals->curtime)
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
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!pLocal)return false;
	if (!pWeapon)return false;
	float flServerTime = pLocal->GetTickBase() * interfaces::globals->interval_per_tick;
	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}
float hitchance()
{
	float hitchance = 101;
	auto m_local = hackManager.pLocal();
	auto pWeapon = m_local->GetWeapon2();
	if (pWeapon)
	{
		if (options::menu.aimbot_tab.AccuracyHitchance.GetValue() > 0)
		{
			float inaccuracy = pWeapon->GetInaccuracy();
			if (inaccuracy == 0) inaccuracy = 0.0000001;
			inaccuracy = 1 / inaccuracy;
			hitchance = inaccuracy;
		}
		return hitchance;
	}
}
bool CAimbot::CanOpenFire(IClientEntity * local)
{

	C_BaseCombatWeapon* entwep = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	float flServerTime = (float)local->GetTickBase() * interfaces::globals->interval_per_tick;
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
	int ud_rate = interfaces::cvar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = interfaces::cvar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}
void CAimbot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame() || !pLocalEntity->isValidPlayer())
		return;

	c_fakelag->Fakelag(pCmd, bSendPacket);

	if (options::menu.AntiAimTab.AEnable.GetState())
	{
		static int ChokedPackets = 1;
		//	std::vector<dropdownboxitem> spike = options::menu.MiscTab.fl_spike.items;

		if (!pWeapon)
			return;

		if ((ChokedPackets < 1 && pLocalEntity->GetHealth() <= 0.f && !(pWeapon->IsKnife() || pWeapon->IsC4())))
		{
			bSendPacket = false;
		}

		else
		{
			if (pLocalEntity->IsAlive() && pLocalEntity->GetMoveType() != MOVETYPE_LADDER)
			{
				c_antiaim->DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = 1;
		}
	}

	if (options::menu.aimbot_tab.AimbotEnable.GetState())
	{
		DoAimbot(pCmd, bSendPacket);
		DoNoRecoil(pCmd);
		auto_revolver(pCmd);
	}

	if (options::menu.MiscTab.OtherSafeMode.GetIndex() == 1)
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
	ray.Init(final + Vector(0, 0, options::menu.aimbot_tab.TargetPointscale.GetValue() / 10), final);
	interfaces::trace->TraceRay(ray, MASK_SHOT, &filter, &tr);
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

void CAimbot::delay_shot(IClientEntity* entity, CUserCmd* pcmd)
{
	float old_sim[65] = { 0.f };
	float current_sim[65] = { entity->GetSimulationTime() };

	bool lag_comp;

	int index = options::menu.aimbot_tab.delay_shot.GetIndex();

	switch (index)
	{
	case 1:
	{
		if (old_sim[entity->GetIndex()] != current_sim[entity->GetIndex()])
		{
			can_shoot = true;
			old_sim[entity->GetIndex()] = current_sim[entity->GetIndex()];
		}
		else
			can_shoot = false;
	}
	break;

	case 2:
	{
		Vector vec_position[65], origin_delta[65];

		if (entity->m_VecORIGIN() != vec_position[entity->GetIndex()])
		{
			origin_delta[entity->GetIndex()] = entity->m_VecORIGIN() - vec_position[entity->GetIndex()];
			vec_position[entity->GetIndex()] = entity->m_VecORIGIN();

			lag_comp = fabs(origin_delta[entity->GetIndex()].Length()) > 64;
		}

		if (lag_comp && entity->GetVelocity().Length2D() > 300)
		{
			can_shoot = false;
		}
		else
			can_shoot = true;
	}
	break;

	case 3:
	{
		if (backtracking->good_tick(TIME_TO_TICKS(entity->GetSimulationTime() + backtracking->GetLerpTime())))
		{
			pcmd->tick_count = TIME_TO_TICKS(entity->GetSimulationTime() + backtracking->GetLerpTime());
			can_shoot = true;
		}
		else
		{
			can_shoot = false;
		}
	}
	break;
	}

}
namespace debug_helpers
{
	auto hitbox_to_String = [](int hitgroup) -> std::string
	{
		switch (hitgroup)
		{
		case 0:
		{
			return "HEAD";
		}
		case 1:
			return "NECK";
		case 2:
			return "PELVIS";
		case 3:
			return "STOMACH";
		case 5:
			return "LOWER CHEST";
		case 6:
			return "UPPER CHEST";
		case 7:
			return "THIGHS";
		case 8:
			return"THIGHS";
		case 11:
			return "LEGS";
		case 12:
			return "LEGS";
		case 16:
			return "ARMS";
		case 17:
			return "ARMS";
		default:
			return "BODY";
		}
	};

	auto stage_to_string = [](int stage) -> std::string
	{
		switch (stage)
		{
		case 0:
			return "IN AIR.";
		case 1:
			return "MOVING.";
		case 2:
			return "STANDING.";
		}
	};

	auto resolver_flag_to_string = [](int stage) -> std::string
	{
		switch (stage)
		{
		case 0:
			return "DEFAULT";
		case 1:
			return "LBY RELATIVE";
		case 2:
			return "DESYNC | LBY";
		case 3:
			return "DESYNC | RELATIVE";
		case 4:
			return "STATIC DESYNC";
		case 5:
			return "SLOW WALK (?)";
		case 6:
			return "LBY";
		}
	};

}


void CAimbot::mirror_console_debug(IClientEntity * the_nignog)
{
	bool gay = backtracking->good_tick(TIME_TO_TICKS(pTarget->GetSimulationTime() + backtracking->GetLerpTime()));
	bool delayshot = options::menu.aimbot_tab.delay_shot.GetIndex() != 0;
	bool sw = the_nignog->GetVelocity().Length2D() < 50.f && the_nignog->GetVelocity().Length2D() > 25.f;

	int c = hitchance() / 1.5;
	int s = the_nignog->GetVelocity().Length2D();
	int h = options::menu.aimbot_tab.AccuracyHitchance.GetValue();
	std::string EVENT_2 = " [ Mirror Event ] Shot at hitbox [ ";
	std::string one = debug_helpers::hitbox_to_String(HitBox);;
	std::string two = " ] with an actual hit chance of [ ";
	std::string three = std::to_string(c);
	std::string four = " ] and a configured value of [ ";
	std::string nn = std::to_string(h);
	std::string ffs = " ]";
	//----------------------------------------------
	std::string EVENT_1 = " [ Mirror Info ] ";
	std::string five = " Fake: ";
	std::string six = resolver->enemy_fake ? "TRUE." : "FALSE.";
	std::string seven = " Valid Tick: ";
	std::string eight = gay ? "TRUE." : "FALSE.";
	std::string nine = " Delay Shot: ";
	std::string ten = delayshot ? "TRUE." : "FALSE.";
	std::string eleven = " Enemy Speed: [ ";
	std::string twelve = std::to_string(s);
	std::string thirteen = " ]";
	std::string fourteen = " Enemy Health: [ ";
	std::string fifteen = std::to_string(the_nignog->GetHealth());
	std::string sixteen = " ]";
	std::string newline = ".     \n";

	//----------------------------------------------
	std::string EVENT_3 = " [ Mirror Setting ] ";
	std::string desync1 = " Desync: ";
	std::string desync2 = resolver->has_desync ? "TRUE." : "FALSE.";
	std::string slow1 = " Slow Walk: ";
	std::string slow2 = sw ? "TRUE." : "FALSE.";
	std::string r1 = " Primary Resolver: ";
	std::string r2 = options::menu.aimbot_tab.resolver.GetIndex() == 2 ? "TRUE" : "FALSE";
	//----------------------------------------------
	std::string EVENT_4 = " [ Mirror Resolver ] ";
	std::string resolver_stage_text = " Enemy Stage: ";
	//	std::string resolver_stage_index = debug_helpers::stage_to_string(a_c->resolver_stage);
	std::string resolver_flag_text = " Enemy Flag: ";
	//	std::string resolver_flag_index = debug_helpers::resolver_flag_to_string(a_c->resolver_flag[the_nignog->GetIndex()]);

	std::string homo = "         ";

	std::string uremam = EVENT_2 + one + two + three + four + nn + ffs + newline;
	std::string ruined_is_gay = EVENT_1 + five + six + seven + eight + nine + ten + eleven + twelve + thirteen + fourteen + fifteen + sixteen + newline;
	std::string no_muslim = EVENT_3 + desync1 + desync2 + slow1 + slow2 + r1 + r2 + newline;
	//	std::string i_hate_myself = EVENT_4 + resolver_stage_text + resolver_stage_index + resolver_flag_text + resolver_flag_index + newline;
	std::string killme = homo + newline;

	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), uremam.c_str());
	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), ruined_is_gay.c_str());
	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), no_muslim.c_str());
	//	interfaces::cvar->ConsoleColorPrintf(Color(250, 100, 250, 255), i_hate_myself.c_str()); Doesn't work i guess
	interfaces::cvar->ConsoleColorPrintf(Color(100, 100, 100, 100), killme.c_str());
}

void CAimbot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	bool vac_kick = options::menu.MiscTab.OtherSafeMode.GetIndex() == 1;
	IClientEntity* pLocal = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	bool FindNewTarget = true;
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	CMBacktracking gladbacktrack;
	if (!pLocal)
		return;

	if (pWeapon)
	{
		if ((!pWeapon->isZeus() && pWeapon->GetAmmoInClip() == 0) || pWeapon->IsKnife() || pWeapon->IsC4() || pWeapon->IsGrenade())
			return;
	}
	else
		return;

	if (IsLocked && TargetID > -0 && HitBox >= 0)
	{
		pTarget = interfaces::ent_list->get_client_entity(TargetID);
		if (pTarget && TargetMeetsRequirements(pTarget, pLocal))
		{
			HitBox = HitScan(pTarget, pLocal);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; interfaces::engine->get_viewangles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < vac_kick ? 39.f : options::menu.aimbot_tab.AimbotFov.GetValue())
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
		TargetID = get_target_hp(pLocal);

		if (TargetID >= 0)
		{
			pTarget = interfaces::ent_list->get_client_entity(TargetID);
		}

	}

	if (TargetID >= 0 && pTarget)
	{

		HitBox = HitScan(pTarget, pLocal);

		if (!CanOpenFire(pLocal))
			return;

		bool IsAtPeakOfJump = fabs(pLocal->GetVelocity().z) <= 5.0f;

		Vector AP;
		AP = options::menu.aimbot_tab.Multienable.GetState() ? GetHitboxPosition(pTarget, HitBox) : hitbox_location(pTarget, HitBox);
		pTarget->GetPredicted(AP);
		shot_this_tick = false;

		bt_2->ShotBackTrackStoreFSN(pTarget);

		{
			Vector position = pTarget->GetAbsOriginlol();
			Vector extr_position = position;

			float old_simtime = gladbacktrack.current_record[pTarget->GetIndex()].m_flSimulationTime;
			float simtime = pTarget->GetSimulationTime();

			cm_backtrack->ExtrapolatePosition(pTarget, extr_position, simtime, pTarget->GetVelocity());

			AP -= position;
			AP += extr_position;
		}

		bt_2->ShotBackTrackBeforeAimbot(pTarget);

		bt_2->RestoreTemporaryRecord(pTarget);

		cm_backtrack->StartLagCompensation(pTarget);

		if (options::menu.aimbot_tab.QuickStop.GetIndex() && pLocal->GetFlags() & FL_ONGROUND)
			ClampMovement(pCmd, c_misc->get_gun(pWeapon));


		float hc = hitchance();
		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && options::menu.aimbot_tab.AccuracyAutoScope.GetState())
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if (pWeapon->isZeus27())
			{
				Vector n = GetHitboxPosition(pTarget, HitBox);
				if (AimAtPoint(pLocal, n, pCmd, bSendPacket))
				{
					if ((pLocal->GetOrigin() - pTarget->GetOrigin()).Length() <= 170.f)
					{
						aim_at_point = true;
						Vector pos = CalcAngleA(pLocal->GetEyePosition(), n);
						pCmd->buttons |= IN_ATTACK;
						aim_at_point = false;
					}
					else
					{
						aim_at_point = false;
					}
				}
				else
				{
					aim_at_point = false;
				}
			}
			else if ((hc >= options::menu.aimbot_tab.AccuracyHitchance.GetValue() * 1.5))
			{

				if (AimAtPoint(pLocal, AP, pCmd, bSendPacket))
				{
					if (options::menu.aimbot_tab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						c_fakelag->shot = true;
						if (options::menu.aimbot_tab.delay_shot.GetIndex() != 0)
						{
							delay_shot(pTarget, pCmd);

							if (can_shoot == true)
							{
								pCmd->buttons |= IN_ATTACK;

								shot_this_tick = true;
							}
						}
						else
						{
							if (options::menu.aimbot_tab.toggledebug.GetState())
							{
								mirror_console_debug(pTarget);
							}
							//		c_fakelag->shot = true;
							pCmd->buttons |= IN_ATTACK;
							shot_this_tick = true;
						}

						was_firing_test = true;

						if (!(pCmd->buttons |= IN_ATTACK))
						{
							shot_this_tick = false;
						}

					}
					else if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
					{
						if (options::menu.aimbot_tab.toggledebug.GetState())
						{
							mirror_console_debug(pTarget);
						}
						was_firing_test = false;
						c_fakelag->shot = false;

						return;
					}

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
									//	was_firing = true;
								}
							}
							//		else
							//			was_firing = false;

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

bool CAimbot::TargetMeetsRequirements(IClientEntity* pEntity, IClientEntity* local)
{
	//	auto local = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (pEntity->isValidPlayer())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pEntity->cs_player() && interfaces::engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (options::menu.MiscTab.OtherSafeMode.GetIndex() != 3 ? pEntity->team() != local->team() : pEntity->GetIndex() != local->GetIndex())
			{
				// Spawn Check
				if (!pEntity->has_gungame_immunity())
				{
					if (!(pEntity->GetFlags() & FL_FROZEN))
					{
						return true;
					}
				}

			}
		}
	}
	return false;
}

float CAimbot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
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

	return (acos(DotProduct) * (MaxDegrees / PI));
}
int CAimbot::get_target_hp(IClientEntity* pLocal)
{
	int target = -1;
	int minHealth = 101;

	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; interfaces::engine->get_viewangles(View);

	for (int i = 0; i < 65; i++)
	{
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		if (TargetMeetsRequirements(pEntity, pLocal))
		{
			if (pEntity->GetOrigin() == Vector(0, 0, 0))
				continue;

			int NewHitBox = HitScan(pEntity, pLocal);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov <= 180.f)
				{
					minHealth = Health;
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
bool CAimbot::should_baim(IClientEntity* pEntity)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());

	if (!pWeapon)
		return false;

	int health = options::menu.aimbot_tab.BaimIfUnderXHealth.GetValue();

	int miss = Globals::missedshots;

	while (miss > 3)
		miss = 0;

	bool nn = miss > 1;

	if ((GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey()) ||
		(options::menu.aimbot_tab.baim_fakewalk.GetState() && (enemy_is_slow_walking(pEntity)))
		|| (options::menu.aimbot_tab.baim_fake.GetState() && nn)
		|| (options::menu.aimbot_tab.baim_inair.GetState() && !(pEntity->GetFlags() & FL_ONGROUND))
		|| pEntity->GetHealth() <= health))
	{
		return true;
	}
	else
		return false;

	return false;
}

bool CAimbot::should_prefer_head(IClientEntity* pEntity)
{
	std::vector<dropdownboxitem> factor = options::menu.aimbot_tab.prefer_head.items;

	if (factor[0].GetSelected && !enemy_is_slow_walking(pEntity) && pEntity->GetVelocity().Length2D() >= 100.f)
	{
		return true;
	}

	else if (factor[1].GetSelected && (!resolver->enemy_fake[pEntity->GetIndex()]) && pEntity->GetVelocity().Length2D() < 1.f)
	{
		return true;
	}

	//	else if (factor[2].GetSelected && resolver->enemy_shot[pEntity->GetIndex()] && !enemy_is_slow_walking(pEntity))
	//	{
	//		return true;
	//	}

	else
		return false;

	//	return false;
}

bool CAimbot::enemy_is_slow_walking(IClientEntity * entity)
{
	C_BaseCombatWeapon* weapon = entity->GetWeapon2();
	if (!weapon)
		return false;

	float speed = entity->GetVelocity().Length2D();

	if (speed < 50.f && speed >= 10.f) // if it's more or less the same.
	{
		return true;
	}
	else
		return false;
	return false;
}


int CAimbot::automatic_hitscan(IClientEntity * entity)
{
	int hp = entity->GetHealth();
	int speed = entity->GetVelocity().Length();

	if (entity == nullptr)
		return 0;

#pragma region " 1 = head, pelvis | 2 = head, stomach, pelvis | 3 - full scan | 4- body only | 5 - head, lower body, legs "

	if (speed >= 210)
	{
		if (hp >= 70)
		{
			return 1;
		}

		if (hp < 70 && hp >= 50)
		{
			return 2;
		}

		if (hp < 50)
		{
			return 3;
		}
	}

	if (speed < 210 && speed >= 150)
	{
		if (hp >= 50)
		{
			return 2;
		}

		if (hp < 50)
		{
			return 3;
		}
	}

	if (speed < 150)
	{
		if (hp >= 70)
		{
			return 5;
		}

		if (hp < 70 && hp > 50)
		{
			return 3;
		}

		if (hp <= 50)
		{
			return 4;
		}
	}
}

std::vector<int> CAimbot::head_hitscan()
{
	std::vector<int> hitbox;
	hitbox.push_back((int)csgo_hitboxes::head);
	//	hitbox.push_back((int)csgo_hitboxes::neck);

	return hitbox;
}

std::vector<int> CAimbot::upperbody_hitscan()
{
	std::vector<int> hitbox;
	hitbox.push_back((int)csgo_hitboxes::upper_chest);
	hitbox.push_back((int)csgo_hitboxes::thorax);

	return hitbox;
}

std::vector<int> CAimbot::lowerbody_hitscan()
{
	std::vector<int> hitbox;
	hitbox.push_back((int)csgo_hitboxes::lower_chest);
	hitbox.push_back((int)csgo_hitboxes::pelvis);
	hitbox.push_back((int)csgo_hitboxes::stomach);
	hitbox.push_back((int)csgo_hitboxes::left_thigh);
	hitbox.push_back((int)csgo_hitboxes::right_thigh);

	return hitbox;
}

std::vector<int> CAimbot::arms_hitscan()
{
	std::vector<int> hitbox;
	hitbox.push_back((int)csgo_hitboxes::right_upper_arm);
	hitbox.push_back((int)csgo_hitboxes::left_upper_arm);
	hitbox.push_back((int)csgo_hitboxes::right_lower_arm);
	hitbox.push_back((int)csgo_hitboxes::left_lower_arm);
	hitbox.push_back((int)csgo_hitboxes::right_hand);
	hitbox.push_back((int)csgo_hitboxes::left_hand);

	return hitbox;
}

std::vector<int> CAimbot::legs_hitscan()
{
	std::vector<int> hitbox;
	hitbox.push_back((int)csgo_hitboxes::left_calf);
	hitbox.push_back((int)csgo_hitboxes::left_foot);
	hitbox.push_back((int)csgo_hitboxes::right_calf);
	hitbox.push_back((int)csgo_hitboxes::right_foot);

	return hitbox;
}

int CAimbot::HitScan(IClientEntity* pEntity, IClientEntity* pLocal)
{

	float health = options::menu.aimbot_tab.BaimIfUnderXHealth.GetValue();

	std::vector<int> scan_hitboxes;
	std::vector<dropdownboxitem> auto_list = options::menu.aimbot_tab.target_auto.items;
	std::vector<dropdownboxitem> scout_list = options::menu.aimbot_tab.target_scout.items;
	std::vector<dropdownboxitem> awp_list = options::menu.aimbot_tab.target_awp.items;
	std::vector<dropdownboxitem> pistol_list = options::menu.aimbot_tab.target_pistol.items;
	std::vector<dropdownboxitem> smg_list = options::menu.aimbot_tab.target_smg.items;
	std::vector<dropdownboxitem> otr_list = options::menu.aimbot_tab.target_otr.items;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pWeapon != nullptr)
	{

		if (GameUtils::AutoSniper(pWeapon))
		{
			if (options::menu.aimbot_tab.custom_hitscan.GetState() && !(GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey())))
			{
				switch (automatic_hitscan(pEntity))
				{
				case 1:
				{
					scan_hitboxes = head_hitscan();
				}
				break;

				case 2:
				{
					scan_hitboxes.push_back((int)csgo_hitboxes::head);
					scan_hitboxes.push_back((int)csgo_hitboxes::stomach);
					scan_hitboxes.push_back((int)csgo_hitboxes::pelvis);
				}
				break;

				case 3:
				{
					scan_hitboxes.push_back((int)csgo_hitboxes::head);
					scan_hitboxes.push_back((int)csgo_hitboxes::neck);
					scan_hitboxes.push_back((int)csgo_hitboxes::upper_chest);
					scan_hitboxes.push_back((int)csgo_hitboxes::lower_chest);
					scan_hitboxes.push_back((int)csgo_hitboxes::stomach);
					scan_hitboxes.push_back((int)csgo_hitboxes::pelvis);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_thigh);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_thigh);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_foot);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_foot);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_upper_arm);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_upper_arm);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_lower_arm);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_lower_arm);
				}
				break;

				case 4:
				{
					scan_hitboxes.push_back((int)csgo_hitboxes::lower_chest);
					scan_hitboxes.push_back((int)csgo_hitboxes::stomach);
					scan_hitboxes.push_back((int)csgo_hitboxes::pelvis);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_thigh);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_thigh);
				}
				break;

				case 5:
				{
					scan_hitboxes.push_back((int)csgo_hitboxes::head);
					scan_hitboxes.push_back((int)csgo_hitboxes::pelvis);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_thigh);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_thigh);
					scan_hitboxes.push_back((int)csgo_hitboxes::right_foot);
					scan_hitboxes.push_back((int)csgo_hitboxes::left_foot);
				}
				break;
				}
			}
			else
			{

				if (auto_list[1].GetSelected) // Upper Body
				{
					scan_hitboxes = upperbody_hitscan();
				}

				if (auto_list[2].GetSelected) // Lower body
				{
					scan_hitboxes = lowerbody_hitscan();
				}

				if (auto_list[3].GetSelected) // Arms
				{
					scan_hitboxes = arms_hitscan();
				}

				if (auto_list[4].GetSelected) // Legs
				{
					scan_hitboxes = legs_hitscan();
				}

				if (auto_list[0].GetSelected && !(GetAsyncKeyState(options::menu.aimbot_tab.bigbaim.GetKey()))) // Head
				{
					scan_hitboxes = head_hitscan();
				}
			}

		}
		if (pWeapon->is_scout())
		{
			if (scout_list[0].GetSelected && !should_baim(pEntity)) // Head
			{
				scan_hitboxes = head_hitscan();
			}

			if (scout_list[1].GetSelected) // Upper Body
			{
				scan_hitboxes = upperbody_hitscan();
			}

			if (scout_list[2].GetSelected) // Lower body
			{
				scan_hitboxes = lowerbody_hitscan();
			}

			if (scout_list[3].GetSelected) // Arms
			{
				scan_hitboxes = arms_hitscan();
			}

			if (scout_list[4].GetSelected) // Legs
			{
				scan_hitboxes = legs_hitscan();
			}
		}

		if (pWeapon->is_awp())
		{
			if (awp_list[0].GetSelected && !should_baim(pEntity)) // Head
			{
				scan_hitboxes = head_hitscan();
			}

			if (awp_list[1].GetSelected) // Upper Body
			{
				scan_hitboxes = upperbody_hitscan();
			}

			if (awp_list[2].GetSelected) // Lower body
			{
				scan_hitboxes = lowerbody_hitscan();
			}

			if (awp_list[3].GetSelected) // Arms
			{
				scan_hitboxes = arms_hitscan();
			}

			if (awp_list[4].GetSelected) // Legs
			{
				scan_hitboxes = legs_hitscan();
			}
		}

		if (GameUtils::IsPistol(pWeapon)) // Head
		{

			if (pistol_list[0].GetSelected && !should_baim(pEntity)) // Head
			{
				scan_hitboxes = head_hitscan();
			}

			if (pistol_list[1].GetSelected) // Upper Body
			{
				scan_hitboxes = upperbody_hitscan();
			}

			if (pistol_list[2].GetSelected) // Lower body
			{
				scan_hitboxes = lowerbody_hitscan();
			}

			if (pistol_list[3].GetSelected) // Arms
			{
				scan_hitboxes = arms_hitscan();
			}

			if (pistol_list[4].GetSelected) // Legs
			{
				scan_hitboxes = legs_hitscan();
			}
		}

		if (GameUtils::IsMP(pWeapon) && !should_baim(pEntity)) // Head
		{
			if (smg_list[0].GetSelected && !should_baim(pEntity)) // Head
			{
				scan_hitboxes = head_hitscan();
			}

			if (smg_list[1].GetSelected && !should_baim(pEntity)) // Upper Body
			{
				scan_hitboxes = upperbody_hitscan();
			}

			if (smg_list[2].GetSelected) // Lower body
			{
				scan_hitboxes = lowerbody_hitscan();
			}

			if (smg_list[3].GetSelected) // Arms
			{
				scan_hitboxes = arms_hitscan();
			}

			if (smg_list[4].GetSelected) // Legs
			{
				scan_hitboxes = legs_hitscan();
			}
		}

		if (GameUtils::IsRifle(pWeapon) || GameUtils::IsShotgun(pWeapon) || GameUtils::IsMachinegun(pWeapon))
		{
			if (otr_list[0].GetSelected && !should_baim(pEntity)) // Head // Head
			{
				scan_hitboxes = head_hitscan();
			}

			if (otr_list[1].GetSelected && !should_baim(pEntity)) // Upper Body
			{
				scan_hitboxes = upperbody_hitscan();
			}

			if (otr_list[2].GetSelected) // Lower body
			{
				scan_hitboxes = lowerbody_hitscan();
			}

			if (otr_list[3].GetSelected) // Arms
			{
				scan_hitboxes = arms_hitscan();
			}

			if (otr_list[4].GetSelected) // Legs
			{
				scan_hitboxes = legs_hitscan();
			}
		}

		if (GameUtils::IsZeus(pWeapon))
		{
			scan_hitboxes.push_back((int)csgo_hitboxes::upper_chest);
			scan_hitboxes.push_back((int)csgo_hitboxes::lower_chest);
			scan_hitboxes.push_back((int)csgo_hitboxes::stomach);
			scan_hitboxes.push_back((int)csgo_hitboxes::pelvis);
			scan_hitboxes.push_back((int)csgo_hitboxes::upper_chest);
			scan_hitboxes.push_back((int)csgo_hitboxes::left_thigh);
			scan_hitboxes.push_back((int)csgo_hitboxes::right_thigh);
		}

		if (should_baim(pEntity))
		{
			scan_hitboxes = lowerbody_hitscan();
		}

		if (should_prefer_head(pEntity) && !should_baim(pEntity))
		{
			scan_hitboxes = head_hitscan();
		}

		for (auto HitBoxID : scan_hitboxes)
		{
			Vector Point, lol;

			Point = hitbox_location(pEntity, HitBoxID);
			//		lol = hitbox_location(pEntity, 0);
			float dmg = 0.f;
			//		std::vector<dropdownboxitem> factor = options::menu.aimbot_tab.prefer_head.items;

					/*
					if (factor[2].GetSelected)
					{
						if (backup_awall->can_hit(lol, &dmg))
							return 0;
						else if (backup_awall->can_hit(Point, &dmg))
						{
							return HitBoxID;
						}
					}
					else
					{
					*/
			if (backup_awall->can_hit(Point, &dmg))
				//	if (new_autowall->CanHit(Point))
			{
				return HitBoxID;
			}
			//	}
		}
		return -1;
	}
}

void CAimbot::DoNoRecoil(CUserCmd *pCmd)
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
void CAimbot::aimAtPlayer(CUserCmd *pCmd, IClientEntity* pLocal)
{
	//	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->range;

	IClientEntity* target = nullptr;

	for (int i = 0; i <= 65; i++)
	{
		IClientEntity *pEntity = interfaces::ent_list->get_client_entity(i);
		if (TargetMeetsRequirements(pEntity, pLocal))
		{
			if (TargetID != -1)
				target = interfaces::ent_list->get_client_entity(TargetID);
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

bool CAimbot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
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
	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, interfaces::ent_list->get_client_entity(TargetID), 0);
	Vector AddAngs = angles - LastAimstepAngle;
	if (fovLeft > 29.0f && options::menu.MiscTab.OtherSafeMode.GetIndex() == 1)
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
	//	if (Options::Menu.aimbot_tab.AimbotSilentAim.GetState())
	//	{
	pCmd->viewangles = angles;
	//	}
	//	if (!Options::Menu.aimbot_tab.AimbotSilentAim.GetState())
	//	{
	//		Interfaces::Engine->SetViewAngles(angles);
	//	}

	if (options::menu.aimbot_tab.AimbotSilentAim.GetState())
	{
		Vector oViewangles = pCmd->viewangles;
		float oForwardmove = pCmd->forwardmove;
		float oSidemove = pCmd->sidemove;

		static auto choked = 0.f;

		//So we dont kill ourselfs
		if (choked < 1 && !c_fakelag->shot)
		{
			bSendPacket = false;//this will overwrite fakelag values when psilent is on
			choked++;
		}
		else
			bSendPacket = true;

		if (bSendPacket)
		{

			pCmd->viewangles = oViewangles;
			pCmd->forwardmove = oForwardmove;
			pCmd->sidemove = oSidemove;
			choked++;
		}
	}

	return ReturnValue;
}

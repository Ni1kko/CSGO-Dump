#include "RageBot.h"
#include "../../options.hpp"
#include "../prediction/prediction.hpp"
#include "../../config.hpp"
#include "AutoWall.h"
#include "../../helpers/utils.hpp"


/*int Globals::TargetID;
C_AutoWall AutoWall;

namespace Aimbot
{
	bool Autoshoot;
	bool Autoscope;
	bool Automm;
	float fov;
	bool Autowall;
	bool Silent;
	int Hitscans = 2;
}


template<class T, class U>
T clamp2(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}


#define max(a,b)            (((a) > (b)) ? (a) : (b))

float LagFix()
{
	//pAyy fakelag fix bro

	float updaterate = g_CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar* minupdate = g_CVar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = g_CVar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetFloat();

	float ratio = g_CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_CVar->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = g_CVar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = g_CVar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clamp2(ratio, cmin->GetFloat(), cmax->GetFloat());


	return max(lerp, ratio / updaterate);

}

bool C_IWAimbot::GetHiboxPosition(C_BasePlayer* pPlayer, Vector& vPos, int iHitBox)
{
	vPos = pPlayer->GetHitboxPos(iHitBox);
	return true;
}
bool C_IWAimbot::IsVisible(C_BasePlayer* TargetEntity, C_BasePlayer* LocalPlayer, const Vector& TargetPosition, const Vector& EyePos)
{
	trace_t Trace;
	Ray_t Ray;

	Ray.Init(EyePos, TargetPosition);
	CTraceFilter filter;

	filter.pSkip = LocalPlayer;
	//filter.pSkip2 = TargetEntity;

	//Interface->EngineTrace()->GetVTable<IEngineTrace>()->TraceRay(Ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_HITBOX, &filter, &Trace);
	g_EngineTrace->TraceRay(Ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_HITBOX, &filter, &Trace);
	//Albo LocalPlayer->CanSeePlayer(TargetEnitu);
	return (Trace.fraction > 0.97f);
}



bool C_IWAimbot::TraceTargetHitpoint(C_BasePlayer* pLocalEntity, C_BaseCombatWeapon* pLocalWeapon, C_BasePlayer* pBaseEntity, Vector& vecHitPoint, CUserCmd* pUserCmd)
{
	
	Hitboxes hitbox = Hitboxes::HITBOX_HEAD;

	int Choose[] = { (hitbox), 11, 5, 12, 1, 9, 10, 13, 14, 16, 17, 18, 8, 7, 6, 4, 3, 2, 15 };

	Choose[0] = hitbox;

	for (int i = 18; i >= 0; i--)
	{
	if (Choose[i] == hitbox)
	{
	Choose[i] = 0;
	}
	}

	int Choose[] = { (hitbox), 11, 5, 12, 1, 9, 10, 13, 14, 16, 17, 18, 8, 7, 6, 4, 3, 2, 15 };

	Choose[0] = hitbox;

	for (int i = 18; i >= 0; i--)
	{
		if (Choose[i] == hitbox)
		{
			Choose[i] = 0;
		}
	}

	auto flMinDamage = options.auto_wall_mindamange;
	auto vecCurPos = pLocalEntity->GetEyePos();

	Vector vecHit;
	auto bDefaultHitbox = GetHiboxPosition(pBaseEntity, vecHit, hitbox); //Aimbothit box (8 - head)
	if (bDefaultHitbox)
	{
		auto bIsVisible = IsVisible(pBaseEntity, pLocalEntity, vecHit, vecCurPos);
		if (bIsVisible)
		{
			vecHitPoint = vecHit;
			return true;
		}

		auto flDamage = 0.f;
		//				Aimbot::AutoWall
		//					|
		//					V
		auto bPenetrate = (options.auto_wall && AutoWall.PenetrateWall(pLocalEntity, pLocalWeapon, vecHit, flDamage));

		if (bPenetrate && flDamage >= flMinDamage)
		{
			vecHitPoint = vecHit;
			return true;
		}
	}
	//Aimbot::Hitscans
	if (Aimbot::Hitscans) 
	{
		if (Aimbot::Hitscans <= 2)
		{
			static int increase = 0;
			if (increase >= 18)
			{
				increase = 0;
			}
			else
			{
				increase++;
			}
			GetHiboxPosition(pBaseEntity, vecHit, increase);
		}
		else if (Aimbot::Hitscans == 3)
		{
			auto flDamage = 0.f;
			for (int m_iHitbox = 18; m_iHitbox >= 0; m_iHitbox--)
			{
				GetHiboxPosition(pBaseEntity, vecHit, Choose[m_iHitbox]);
				if (options.auto_wall && AutoWall.PenetrateWall(pLocalEntity, pLocalWeapon, vecHit, flDamage) && flDamage >= flMinDamage) //&&  !pLocalWeapon->GetCSWeaponData()->iWeaponID == WEAPON_AWP)
				{
					vecHitPoint = vecHit;
					return true;
				}
			}
		}

		auto flDamage = 0.f;
		if (IsVisible(pBaseEntity, pLocalEntity, vecHit, vecCurPos))// && pLocalWeapon->GetID() != WEAPON_AWP) Nie penetruj scian na przy AWP bo i tak pewnie nie trafisz 
		{
			vecHitPoint = vecHit;
			return true;
		}
		else if (Aimbot::Hitscans == 2 && options.auto_wall && AutoWall.PenetrateWall(pLocalEntity, pLocalWeapon, vecHit, flDamage) && flDamage >= flMinDamage)//&& pLocalWeapon->GetID() != WEAPON_AWP)
		{
			vecHitPoint = vecHit;
			return true;
		}
	}

	return false;
}



void C_IWAimbot::CalcAngle(Vector& vSource, Vector& vDestination, QAngle& qAngle)
{
	Vector vDelta = vSource - vDestination;

	float fHyp = (vDelta.x * vDelta.x) + (vDelta.y * vDelta.y);

	float fRoot;

	__asm
	{
		sqrtss xmm0, fHyp
		movss fRoot, xmm0
	}

	qAngle.pitch = RAD2DEG(atan(vDelta.z / fRoot));
	qAngle.yaw = RAD2DEG(atan(vDelta.y / vDelta.x));

	if (vDelta.x >= 0.0f)
		qAngle.yaw += 180.0f;

	Math::FixAngles(qAngle);
}



float GetFov(Vector vLocalOrigin, Vector vPosition, Vector vForward)
{
	Vector vLocal;

	vPosition -= vLocalOrigin;
	vLocal = vPosition;

	vLocal.NormalizeInPlace();

	float fValue = vForward.Dot(vLocal);

	//np for kolo's math skills
	if (fValue < -1.0f)
		fValue = -1.0f;

	if (fValue > 1.0f)
		fValue = 1.0f;

	return RAD2DEG(acos(fValue));
}

bool Hitchance(float hitchance, C_BaseCombatWeapon* pWeapon)
{

	if (hitchance >= pWeapon->GetInaccuracy())
		return true;
	else return false;
}
void ProcessSimulation(CUserCmd* pCmd, float flTime)
{
	if (flTime <= 0.0f)
		return;
	int target_Tick_men = TIME_TO_TICKS(flTime);
	if ((pCmd->tick_count - target_Tick_men) > 8 || (pCmd->tick_count - target_Tick_men) < -10)
		return;

	pCmd->tick_count = target_Tick_men + TIME_TO_TICKS(0.031f);
}


Vector BestPoint(C_BasePlayer* targetPlayer, Vector & final)
{
	C_BasePlayer* pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

Vector TickPrediction(Vector AimPoint, C_BasePlayer* pTarget)
{
	return AimPoint + (pTarget->m_vecVelocity() * g_GlobalVars->interval_per_tick);
}



void C_IWAimbot::legitaa(CUserCmd* cmd, bool& bSendPacket) {
	

	static bool real = false;
	real = !real;
	QAngle angle = cmd->viewangles;
	if (real) angle -= 90.0f;
	bSendPacket = !real;
	cmd->viewangles = angle;
	Math::FixAngles(cmd->viewangles);
}

void C_IWAimbot::Main(CUserCmd* pUserCmd, C_BasePlayer* pLocal, bool& bSendPacket)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pLocal->m_hActiveWeapon();
	int iMyTeam = pLocal->m_iTeamNum();

	if (!pWeapon) return;

	if (pWeapon->IsGrenade()) {
		return;
	}

	static int wait = 0;

	int tickold = pUserCmd->tick_count;

	Vector vTarget;
	float m_flTime;
	Vector vEyePos = pLocal->GetEyePos();

	static QAngle vClientViewAngles;
	g_EngineClient->GetViewAngles(vClientViewAngles);

	static Vector vAngle;
	 Math::AngleVectors(vClientViewAngles, vAngle);

	//Reset();// xddddd i o co tu kmn nwm nie pomoge xd
	Vector vFinal;
	Vector predictions;
	int iTarget = -1;
	for (INT ax = 1; ax <= g_EngineClient->GetMaxClients(); ax++)
	{
		//MessageBox(NULL, "BreakPoint: pBeaseEntity,", "FadddWare", NULL);
		//C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(ax)->GetBaseEntity();
		auto pBaseEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(ax));

		//MessageBox(NULL, "BreakPoint: valid player to aim at,", "FadddWare", NULL);
		if (!pBaseEntity
			|| pBaseEntity == pLocal
			|| !pLocal->IsAlive()
			|| pBaseEntity->IsDormant()
			|| !pBaseEntity->IsAlive()
			//|| PlayerList[pBaseEntity->EntIndex()].bFriend //pomin jezeli jest oznaczony jako przyjaciel
			|| !TraceTargetHitpoint(pLocal, pWeapon, pBaseEntity, vTarget, pUserCmd))
			continue;

		if (options.aim_friendly_fire)
		{
			if (!pBaseEntity->m_iTeamNum() == iMyTeam)
				continue;
		}
		else
		{
			if (pBaseEntity->m_iTeamNum() == iMyTeam)
				continue;
		}

		//MessageBox(NULL, "Refund!,", "FadddWare", NULL);

		float fCurrFOV = GetFov(vEyePos, vTarget, vAngle);

		if (fCurrFOV > options.aim_fov)	
			continue;

		//	if(g_CVARS.CvarList[TargetSelection] == 1)//targetselection
		float fBestTarget = (Vector(vTarget - vEyePos).Length());

		if (fBestTarget < fCurrFOV)
			continue;

		fBestTarget = fCurrFOV;

		vFinal = vTarget;

		//this is our target which is closest to crosshair
		iTarget = ax;
		//m_flTime = pBaseEntity->m_flSimulationTime();

		if (options.multipointer)
		{
			predictions = BestPoint(pBaseEntity, vFinal);
		}

		if (options.vecvelocityprediction)
		{
			predictions = TickPrediction(vFinal, pBaseEntity);
		}

		Globals::TargetID = ax;

	}

	if (iTarget > -1)
	{
		if (options.multipointer || options.vecvelocityprediction)
		{
			CalcAngle(vEyePos, predictions, pUserCmd->viewangles); //Silent + predictions 
		}
		else
		{
			CalcAngle(vEyePos, vFinal, pUserCmd->viewangles); //Silent 
		}
		if (!options.aim_silent_mode) //C
			g_EngineClient->SetViewAngles(pUserCmd->viewangles); //Silent & ViewAngle 


		C_BasePlayer* pEntity;

		ProcessSimulation(pUserCmd, m_flTime);
		if (true) //AutoMM
		{
			if (pWeapon->m_iClip1() > 0)
			{
				if (!pWeapon->IsReloading())
				{
					if (options.auto_duck)
					{
						if (!(pUserCmd->buttons & IN_DUCK))
						{
							if (bSendPacket)
							{
								bSendPacket = false;
							}
							pUserCmd->buttons |= IN_DUCK;
							pUserCmd->forwardmove = 0.0f;
							pUserCmd->sidemove = 0.0f;
						}
					}
					if (options.auto_scope)
					{
						if (!pLocal->m_bIsScoped() && pWeapon->IsSniper())
						{
							if (bSendPacket)
							{
								bSendPacket = false;
							}
							pUserCmd->buttons |= IN_ATTACK2;
						}
					}
					//Auto shoot
					if (g_Options.aim_auto_shoot && pWeapon->IsSniper())
					{
						if (pLocal->m_vecVelocity().Length() < 60.0f)
						{
							if (g_Options.hit_chance)
							{
								if (Hitchance(g_Options.hit_chanceamount, pWeapon)) // lol nie wiem czemu ale to MUSI byc wieksze od 1
								{
									pUserCmd->buttons |= IN_ATTACK;
									//Utils::ConsolePrint("Hitchance working! ");
								}
							}
							else
							{
								pUserCmd->buttons |= IN_ATTACK;
							}
						}
					}
					//Auto shoot to
					else if (g_Options.aim_auto_shoot)
					{
						if (Hitchance(0.010f, pWeapon))
						{
							pUserCmd->buttons |= IN_ATTACK;
						}
						else
						{
							pUserCmd->buttons |= IN_ATTACK;
						}
					}

					static bool WasFiring = false;
					CCSWeaponInfo* WeaponInfo = pWeapon->GetCSWeaponData();
					if (!WeaponInfo->m_nFullAuto)
					{
						if (pUserCmd->buttons & IN_ATTACK)
						{
							if (WasFiring)
							{
								pUserCmd->buttons &= ~IN_ATTACK;
							}
							//Vector AimPunch = pLocal->m_aimPunchAngle() * 2.f;
							//Vector delta = pUserCmd->viewangles - AimPunch;
							//Math::NormalizeAngles(delta);
							//pUserCmd->viewangles = delta;
						}

						WasFiring = pUserCmd->buttons & IN_ATTACK ? true : false;
					}
				}
			}
			else
			{
				pUserCmd->buttons |= IN_RELOAD;
			}
		}
		//AutoShoot
		else if (options.aim_auto_shoot)
		{
			if (pWeapon->m_iClip1() == 0)
			{
				pUserCmd->buttons |= IN_RELOAD;
			}
			else
			{
				//hitchance
				if (true)
				{
					if (Hitchance(0.05f, pWeapon))
					{
						pUserCmd->buttons |= IN_ATTACK;
					}
				}
				else
				{
					pUserCmd->buttons |= IN_ATTACK;
				}
			}
		}
	}
	else
	{
		
	}

}*/

#include "..\SDK\CEntity.h"
#include "AutoWall.h"

class KnifeBot
{

	static float GetDamage(Vector Angle)
	{
		g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		FireBulletData Bullet_Data = FireBulletData(g::pLocalEntity->GetEyeOrigin());
		Bullet_Data.filter.pSkip = g::pLocalEntity;
		Utils::AngleVectors(Angle, &Bullet_Data.direction);
		
		C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();
		if (autowall->SimulateFireBullet2(g::pLocalEntity, pWeapon, Bullet_Data))
			return Bullet_Data.current_damage;
		return 0;
	}
public:
	static void Run(C_BaseEntity* pLocal, CUserCmd* pCmd)
	{
		C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();

		//if (!Vars.Misc.Knifebot)
		//	return;
		if (!pWeapon)
			return;
		if (!pWeapon->is_knife())
			return;

		for (int y = 0; y <= 360; y += 360.f / 6.f)
		{
			for (int x = -89; x <= 89; x += 179.f / 6.f)
			{
				Vector ang = Vector(x, y, 0);
				Vector dir;
				Utils::AngleVectors(ang, &dir);
				trace_t trace;
				autowall->UTIL_TraceLine(pLocal->GetEyeOrigin(), pLocal->GetEyeOrigin() + (dir * 64), MASK_SHOT, pLocal, &trace);

				if (trace.m_pEnt == nullptr)
					continue;
				if (trace.m_pEnt == pLocal)
					continue;
				if (!trace.m_pEnt->IsAlive())
					continue;
				if (!(trace.m_pEnt->GetHealth() > 0))
					continue;
				if (trace.m_pEnt->GetTeam() == pLocal->GetTeam())
					continue;
				if (trace.m_pEnt->IsDormant())
					continue;
				if (trace.m_pEnt->IsImmune())
					continue;
				if (!aimbot->CanShoot())
					return;

				PlayerInfo_t info;
				if (!(g_pEngine->GetPlayerInfo(trace.m_pEnt->EntIndex(), &info)))
					continue;
				pCmd->viewangles = Vector(x, y, 0);
				pCmd->buttons |= IN_ATTACK2;
				return;
			}
		}
	}
};

class ZeusBot
{
public:
	static void Run()
	{

	}
};
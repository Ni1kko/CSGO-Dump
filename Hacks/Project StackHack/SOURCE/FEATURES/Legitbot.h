#pragma once
#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Aimbot.h"
#include "../HOOKS/hooks.h"

/*class CLegitBot
{
public:
	void Run(SDK::CUserCmd* cmd);
	bool EntityIsValid(int i);
	void Triggerbot(SDK::CUserCmd* cmd);
private:
	void GoToTarget(SDK::CUserCmd* cmd);
	void FindTarget();
	void DropTarget();/	void RCS(SDK::CUserCmd* cmd);*/
//
//	int m_fbestfov;
//	int m_ibesttarget;
//};
//
//extern CLegitBot* legitbot;

class CLegitBot
{
public:
	//CLegitBot();

	void weapon_settings(SDK::CBaseWeapon* weapon);
	void do_aimbot(SDK::CBaseEntity *local, SDK::CBaseWeapon* weapon, SDK::CUserCmd* cmd);
	Vector get_randomized_recoil(SDK::CBaseEntity* local);
	Vector get_randomized_angles(SDK::CBaseEntity* local);
	void triggerbot(SDK::CUserCmd* cmd);
	float FovToPlayer(Vector ViewOffSet, Vector View, SDK::CBaseEntity * pEntity, int aHitBox);
	bool AimAtPoint(SDK::CBaseEntity * pLocal, Vector point, SDK::CUserCmd * pCmd);
	int GetEntUnderCrosshair();
	int ClosestBone(SDK::CBaseEntity * Entity);
	void Aimbot(SDK::CUserCmd * cmd);
	void RCS(SDK::CUserCmd * cmd);
	bool entity_is_valid(int i);
	int bestHitbox;
	int FoV;
	bool IsLocked;
	float Speed;
	int TargetID;
	float RecoilControl = 1.65;

private:

	bool get_hitbox(SDK::CBaseEntity* local, SDK::CBaseEntity* entity, Vector& destination);
	int get_target(SDK::CBaseEntity* local, SDK::CBaseWeapon* weapon, SDK::CUserCmd* cmd, Vector& destination);

private:
	int best_target;
	bool shoot;
	Vector view_angle, aim_angle,
		delta_angle, final_angle, hitbox_position;

	float
		aim_smooth, aim_fov,
		randomized_smooth, randomized_angle,
		recoil_min, recoil_max;
};

class LegitAimbot
{
public:
	void lmaobox(SDK::CUserCmd * cmd, SDK::CBaseEntity* entity);
	void Run(SDK::CBaseEntity * pLocal, SDK::CUserCmd * pCommand);
	void Do(SDK::CUserCmd* cmd);
	void Update(SDK::CUserCmd* cmd);

	void MakeVector(Vector angle, Vector & vector);

	float flGetDistance(Vector vOrigin, Vector vLocalOrigin);

	float GetFOV(Vector angle, Vector src, Vector dst);

	int GetBestTarget(SDK::CBaseEntity * pLocal, SDK::CUserCmd * pCommand);

	bool IsVisible(void * pLocal, void * pEntity, Vector vStart, Vector vEnd);

	int GetBestHitbox(SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);

private:
	float duration_spent_clicking = 0.f;
	Vector last_viewangles;
};

extern LegitAimbot legit_aimbot;

extern CLegitBot* legitbot;
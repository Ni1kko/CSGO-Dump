#pragma once

#include "../NewBacktracking.h"
#include "../includes.h"

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
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Movement.h"
#include "../HOOKS/hooks.h"
#include "../NewAimbot.h"
#include "../NewBacktracking.h"

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class CBaseWeapon;
	struct mstudiobbox_t;
}

struct AimbotData_t
{
	AimbotData_t(SDK::CBaseEntity* player, const int& idx)
	{
		this->pPlayer = player;
		this->index = idx;
	}

	SDK::CBaseEntity*	pPlayer;
	int					index;
};

class CAimbot
{
public:
	bool ProcessCmd(int iTargetIndex, SDK::CUserCmd * pCmd);
	SDK::mstudiobbox_t * GetHitbox(SDK::CBaseEntity * entity, int hitbox_index);
	bool is_slow_walking(SDK::CBaseEntity * entity);
	Vector GetHitboxPosition(SDK::CBaseEntity * entity, int hitbox_index);
	void shoot_enemy(SDK::CUserCmd* cmd);
	bool good_backtrack_tick(int tick);
	void run_aimbot(SDK::CUserCmd * cmd);
	float GetLatency1();
	void SelectTarget();
	void lby_backtrack(SDK::CUserCmd * pCmd, SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);
	float accepted_inaccuracy(SDK::CBaseWeapon * weapon);
	bool can_shoot();
	int tick;
	int extrapolted_ticks;
	void rotate_movement(float yaw, SDK::CUserCmd * cmd);
	Vector get_hitbox_pos(SDK::CBaseEntity* entity, int hitbox_id);
	Vector multipoint(SDK::CBaseEntity* entity, int option);
	Vector point(SDK::CBaseEntity* entity, int option);
	Vector LastAngle;
	bool IsAimStep;
	void auto_revolver(SDK::CUserCmd* cmd);

	bool Backtrack[65];
	bool ShotBacktrack[65];

	std::vector<AimbotData_t>	Entities;
private:

	std::vector<Vector> GetMultiplePointsForHitbox(SDK::CBaseEntity * local, SDK::CBaseEntity * entity, int iHitbox, VMatrix BoneMatrix[128]);
	int get_damage(Vector position);
	int GetBestHitbox(SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);
};

extern CAimbot* aimbot;
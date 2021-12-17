#pragma once
#include "../../singleton.h"
#include "../../Configuration/Config.h"
#include "../../Hooking/KeyHandler.h"
#include "..\\..\\..\\Frontend\Rendering\Render.h"
#include "../../SDK/CBaseEntity.h"

#include "CAutoWall.h"
#include "c_backtrack.h"

#define VECTOR_MAX Vector(FLT_MAX, FLT_MAX, FLT_MAX)


class c_ragebot : public singleton<c_ragebot> {
public:
	void create_move();
	CBaseEntity* tempTarget = nullptr;
	int TargetID = 0;
	bool IsViable(CBaseEntity* 
	
	
	
	);
	int GetTargetCycle();
	int GetTargetFOV();
	bool hitchance(Vector point);
private:
	bool Backtrack[65];
	bool ShotBacktrack[65];
	int GetTargetDistance();
	int GetTargetHealth();
	int GetTargetPing();
	bool aim_at_point(Vector point,int idx);
	bool is_able_to_shoot();

	Vector hitscan(int* estimated_damage = nullptr);
	void multipoints(int hitbox, matrix3x4_t bones[128], std::vector<Vector>& points);

	void quick_stop();

	CBaseEntity* target = nullptr;

	Vector oldViewangle = Vector(0, 0, 0);
	bool shouldRevertViewangle = false;
	bool shouldForceDistanceSelection = false;

	Vector oldPoint[65];
};
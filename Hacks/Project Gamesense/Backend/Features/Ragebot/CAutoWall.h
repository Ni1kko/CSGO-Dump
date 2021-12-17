#pragma once
#include "../../singleton.h"

#include "../../SDK/CBaseEntity.h"
#include "../../SDK/CTrace.h"

struct returninfo {
	int damage = -1;
	int hitgroup = -1;
	int walls = 4;
	bool did_penetrate_wall = false;
	float thickness = 1.f;

	CBaseEntity* ent = nullptr;
	Vector end = Vector();
};

struct fbdata {
	Vector start = Vector();
	Vector end = Vector();
	Vector pos = Vector();
	Vector dir = Vector();

	ITraceFilter* filter = nullptr;
	trace_t trace;

	float thickness = 1.f;
	float damage = 1.f;
	int walls = 4;
};

class c_autowall : public singleton<c_autowall> {
public:
	float get_estimated_point_damage(Vector point);
	bool CanHitFloatingPoint(const Vector &point, const Vector &source, CBaseEntity* pEnt);
	float get_point_damage(Vector point, CBaseEntity* e);
	bool IsArmored(CBaseEntity *player, int armorVal, int hitgroup);
	returninfo autowall(Vector start, Vector end, CBaseEntity* from_ent = nullptr, CBaseEntity* to_ent = nullptr, int hitgroup = -1);

private:
	void clip_trace_to_player(Vector& start, Vector& end, CBaseEntity* ent, unsigned int mask, ITraceFilter* filter, trace_t* trace);
	void scale_damage(CBaseEntity* ent, CSWeaponInfo * weapon_info, int& hitgroup, float& damage);
	bool handle_bullet_penetration(CSWeaponInfo* inf, fbdata& bullet);
	bool trace_to_exit(trace_t* enter_trace, Vector& start, Vector& dir, trace_t * exit_trace);
	bool is_breakable(CBaseEntity* e);
};
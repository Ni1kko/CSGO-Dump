#pragma once

#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1
#define DAMAGE_YES		2
#define DAMAGE_AIM		3
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

struct ReturnInfo_t
{
	int m_damage;
	int m_hitgroup;
	int m_penetration_count;
	bool m_did_penetrate_wall;
	float m_thickness;
	Vector m_end;
	IBasePlayer* m_hit_entity;

	ReturnInfo_t(int damage, int hitgroup, int penetration_count, bool did_penetrate_wall, float thickness, IBasePlayer* hit_entity)
	{
		m_damage = damage;
		m_hitgroup = hitgroup;
		m_penetration_count = penetration_count;
		m_did_penetrate_wall = did_penetrate_wall;
		m_thickness = thickness;
		m_hit_entity = hit_entity;
	}
};

class CAutoWall
{
private:
	struct FireBulletData_t
	{
		Vector m_start;
		Vector m_end;
		Vector m_current_position;
		Vector m_direction;

		ITraceFilter* m_filter;
		trace_t m_enter_trace;

		float m_thickness;
		float m_current_damage;
		int m_penetration_count;
	};

	void ScaleDamage(IBasePlayer* e, CCSWeaponInfo* weapon_info, int hitgroup, float& current_damage);
	bool HandleBulletPenetration(CCSWeaponInfo* info, FireBulletData_t& data, bool extracheck = false, Vector point = Vector(0, 0, 0));
	bool TraceToExit(trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace);
	void TraceLine(Vector& start, Vector& end, unsigned int mask, IBasePlayer* ignore, trace_t* trace);
	void ClipTrace(Vector& start, Vector& end, IBasePlayer* e, unsigned int mask, ITraceFilter* filter, trace_t* old_trace);
	bool IsBreakableEntity(IBasePlayer* e);

	float HitgroupDamage(int iHitGroup);

public:
	std::vector<float> scanned_damage;
	std::vector<Vector> scanned_points;
	void reset() {
		scanned_damage.clear();
		scanned_points.clear();
	}
	bool CanHitFloatingPoint(const Vector& point, const Vector& source);
	float CanHit(Vector& point);
	ReturnInfo_t Think(Vector pos, IBasePlayer* target, int specific_hitgroup = -1);
};

extern CAutoWall g_AutoWall;
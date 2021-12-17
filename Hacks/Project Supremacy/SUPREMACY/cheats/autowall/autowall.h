#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class autowall : public singleton< autowall > {
public:
	struct returninfo_t {
		int m_damage;
		int m_hitgroup;
		int m_penetration_count;
		bool m_did_penetrate_wall;
		float m_thickness;
		vec3_t m_end;
		IClientEntity * m_hit_entity;

		returninfo_t(int damage, int hitgroup, int penetration_count, bool did_penetrate_wall, float thickness, IClientEntity * hit_entity) {
			m_damage = damage;
			m_hitgroup = hitgroup;
			m_penetration_count = penetration_count;
			m_did_penetrate_wall = did_penetrate_wall;
			m_thickness = thickness;
			m_hit_entity = hit_entity;
		}
	};

	struct firebulletdata_t {
		vec3_t m_start;
		vec3_t m_end;
		vec3_t m_current_position;
		vec3_t m_direction;

		ITraceFilter * m_filter;
		trace_t m_enter_trace;

		float m_thickness;
		float m_current_damage;
		int m_penetration_count;
	};

	returninfo_t calculate_return_info(vec3_t start, vec3_t end, IClientEntity * from_entity = nullptr, IClientEntity * to_entity = nullptr, int specific_hitgroup = -1);
	void scale_damage(player_t * e, csweaponinfo_t * weapon_info, int hitgroup, float & current_damage);
	bool handle_bullet_penetration(csweaponinfo_t * info, firebulletdata_t & data);
	bool trace_to_exit(trace_t * enter_trace, vec3_t start, vec3_t dir, trace_t * exit_trace);

	void trace_line(vec3_t & start, vec3_t & end, unsigned int mask, IClientEntity * ignore, trace_t * trace);
	void clip_trace_to_player(vec3_t & start, vec3_t & end, player_t * e, unsigned int mask, ITraceFilter * filter, trace_t * old_trace);
	bool is_breakable_entity(IClientEntity * e);
	bool CanHitFloatingPoint(const Vector & point, const Vector & source);
};
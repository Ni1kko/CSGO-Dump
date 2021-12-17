#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

#include "..\lagcompensation\lagcompensation.h"

class aimbot : public singleton< aimbot > {
private:
	struct aimbotconfig_t {
		bool m_autoshoot;
		bool m_autowall;
		bool m_autoscope;
		bool m_hitchance;

		int m_minimum_damage;
		int m_hitchance_amount;

		std::vector< MultiDropdownItem_t > m_hitscan;
		std::vector< MultiDropdownItem_t > m_hitscan_history;
		std::vector< MultiDropdownItem_t > m_multipoint;
		std::vector< MultiDropdownItem_t > m_baim;

		float m_pointscale;

		bool m_quickstop;
	}config;

	weapon_t * m_local_weapon;
	vec3_t m_engine_angles;
	float m_recoil_scale;
	player_t * m_target;

	std::deque< tickrecord_t > m_track;
public:
	void create_move( );

	void update_config( );
};
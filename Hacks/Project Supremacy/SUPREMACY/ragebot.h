#pragma once
#include "includes.hpp"
#include "includes.hpp"
#include "sdk\structs.hpp"
#include "cheats/lagcompensation/lagcompensation.h"

class manual_aa : public singleton< manual_aa > {
private:
	enum {
		SIDE_NONE,
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_BACK
	};
	int side = SIDE_NONE;
public:
	void update();
	bool run();
};

class antiaim : public singleton< antiaim > {
public:
	bool nade_out(CUserCmd * pUserCmd);
	void create_move();
	void update_lowerbody_breaker();
	float at_target();
	bool DesyncDirection(float jitter, int direction);
	void DesyncAnimations(int type);

	float freestanding();

	float get_pitch();
	float get_fake();
	void desync();
	float getreal();
	float get_real();

	bool freeze_check = false;

	void get_unpredicted_velocity();
	player_t * m_e;
	vec3_t unpredicted_velocity;

	bool m_bBreakLowerBody = false;
	float m_flLastLby = 0.0f;
	float m_flNextBodyUpdate = 0.f;
	bool m_bBreakBalance = false;
	float m_flLastMoveTime;
};
class aimbot : public singleton< aimbot > {
private:
	struct target_t {
		float best_distance = 1000000.0f;
		bool backtrack = false;
		int tid = -1;
		vec3_t aimspot;
		float simtime = 0.f;
		tickrecord_t best_record;

		void reset() {
			best_distance = INT_MAX;
			tid = -1;
			simtime = 0.f;
			aimspot.Init();

			backtrack = false;
			best_record;
		}
	};

	weapon_t * local_weapon;
	vec3_t engine_angles;
	float recoil_scale;
public:
	target_t target_data;
public:
	void create_move();

	void iterate_players();

	void zeus_run();

	bool can_zeus(Vector & bestpoint);

	void aim();

	vec3_t best_point(player_t * e, matrix3x4_t bones[128]);

	struct bt_point_return_t {
		tickrecord_t record;
		vec3_t point;


	};

	aimbot::bt_point_return_t best_point_backtrack(player_t * e, player_record_t & player);
	void getpoints(player_t * e, int hitbox_id, matrix3x4_t bones[128], std::vector< vec3_t > & points);

	void quickstop();

	float GetLerpTime();

	void autostop(player_t * target, bool doinAimbot);

	void owowalk(player_t * target, bool doinAimbot);

	bool hitchance(vec3_t angles, player_t * m_entity, float chance);

	void get_target(player_t * e, float fov, vec3_t location, float simulation_time, bool backtrack, tickrecord_t lag_record);

	bool head_check(player_t * e, int m_fFlags, vec3_t m_vecVelocity);

	std::vector< int > hitboxes_from_vector(player_t * e, int m_fFlags, vec3_t m_vecVelocity, std::vector< MultiDropdownItem_t > arr);

	void update_config();
};

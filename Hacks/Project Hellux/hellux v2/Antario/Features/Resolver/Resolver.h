#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>

struct Info
{
	Info() {}


	Vector last_lby, inverse, inverse_right, inverse_left, lby, back, left, right, backtrack;
	float stored_simtime, last_move_time, pre_anim_lby;
	int last_ammo;
	bool breaking_lby, reset_state, could_be_faking;
	std::vector<float> unresolved_yaw, lby_deltas;

	bool lby_changed;
	bool could_be_slowmo;
	bool is_moving;
	bool is_jumping;
	bool is_crouching;
	bool lby_updated;
	bool using_fake_angles;
	float last_moving_lby;
	float stored_lby;
	float next_lby_update_time;
	int stored_missed;

};
struct resolver_infos
{
	AnimationLayer animation;

	bool jitter_desync = false, high_delta = false, low_delta = false, static_desync = false, lby_changed = false;

	float last_lby = FLT_MAX, lby_delta = FLT_MAX, last_eyes = FLT_MAX, eye_delta = FLT_MAX, eye_lby_delta = FLT_MAX, eyes = FLT_MAX;

};
struct resolver_record
{
	std::deque< resolver_infos > tick_records;

	resolver_infos current_tick;
	resolver_infos previous_tick;

};
class Resolver
{
public:
	Info player_info[65];
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];

	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
	bool enemy_slowwalk;
	bool enemy_is_slow_walking(C_BaseEntity* entity);
	float get_gun(C_BaseCombatWeapon* weapon);
	bool has_desync;
	bool enemy_fake[65];
	void animation_fix(C_BaseEntity* entity);
	void resolve(C_BaseEntity* player);
	void resolveOldMethod(C_BaseEntity* entity);
	void constructor(C_BaseEntity* entity);
	void setup(C_BaseEntity* entity);

private:
	void AnimationFix(C_BaseEntity* pEnt);
	resolver_record _player[64];

};
extern Resolver g_Resolver;
#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"
#include <deque>
#include <algorithm>
struct LagRecord
{
	LagRecord()
	{
	}

	void SaveRecord(player_t *player);
	float_t m_flSimulationTime;
	int32_t m_nFlags;
	int32_t m_iPriority;
};

class CMBacktracking : public singleton<CMBacktracking>
{
public:
	std::deque<LagRecord> m_LagRecord[64]; // All records
	std::pair<LagRecord, LagRecord> m_RestoreLagRecord[64]; // Used to restore/change
	std::deque<LagRecord> backtrack_records; // Needed to select records based on menu option.
	LagRecord current_record[64]; // Used for various other actions where we need data of the current lag compensated player

	inline void ClearHistory()
	{
		for (int i = 0; i < 64; i++)
			m_LagRecord[i].clear();
	}
	int lastincomingsequencenumber;
	int GetPriorityLevel(player_t * player, LagRecord * lag_record);

	float GetLerpTime();

	bool FindViableRecord(player_t *player, LagRecord* record);
	bool StartLagCompensation(player_t * player);
	bool IsTickValid(int tick);
	bool IsPlayerValid(player_t * player);

	void AnimationFix(player_t * player);
	void FrameUpdatePostEntityThink();
	void RemoveBadRecords(int Idx, std::deque<LagRecord>& records);
	void FinishLagCompensation(player_t *player);

	template<class T, class U>
	T clamp(T in, U low, U high);
};
extern CMBacktracking * cm_backtrack;
struct tickrecord_t {
	vec3_t m_vec_origin;
	vec3_t m_eye_angles;
	vec3_t m_abs_angles;
	vec3_t m_render_angles;
	vec3_t m_bbmin;
	vec3_t m_bbmax;
	vec3_t m_velocity;

	vec3_t m_previous_origin;
	float m_previous_simulation_time;
	float m_previous_curtime;

	float m_simulation_time;
	float m_duck_amount;
	float m_duck_speed;
	float m_lowerbody;
	float m_curtime;
	int m_flags;

	AnimationLayer m_anim_layers[15];
	matrix3x4_t m_bone_matrix[128];
	float m_pose_params[24];

	float m_anim_time;
	c_baseplayeranimationstate * m_anim_state;

	void save(player_t * e) {
		m_vec_origin = e->m_vecOrigin();
		m_abs_angles = e->abs_angles();
		m_eye_angles = e->m_angEyeAngles();
		m_render_angles = e->GetRenderAngles();
		m_velocity = e->m_vecVelocity();
		m_simulation_time = e->m_flSimulationTime();
		m_duck_amount = e->m_flDuckAmount();
		m_duck_speed = e->m_flDuckSpeed();
		m_lowerbody = e->m_flLowerBodyYawTarget();
		m_flags = e->m_fFlags();
		m_curtime = g_csgo.m_globals()->m_curtime;

		for (int i = 0; i < 15; i++)
			m_anim_layers[i] = e->get_animlayer(i);

		memcpy(m_pose_params, e->m_flPoseParameter(), sizeof(float) * 24);

		e->SetupBones(m_bone_matrix, 128, BONE_USED_BY_ANYTHING, 0.f);

		auto collideable = e->GetCollideable();

		if (!collideable)
			return;

		m_bbmin = collideable->OBBMins();
		m_bbmax = collideable->OBBMaxs();
	}
};

struct player_record_t {
	player_t * m_e;

	void log();

	std::deque< tickrecord_t > get_valid_track();

	std::deque< tickrecord_t > m_track;
};

struct incoming_sequence_t {
	incoming_sequence_t::incoming_sequence_t(int instate, int outstate, int seqnr, float time) {
		m_in_reliable_state = instate;
		m_out_reliable_state = outstate;
		m_sequence_nr = seqnr;
		m_current_time = time;
	}

	int m_in_reliable_state;
	int m_out_reliable_state;
	int m_sequence_nr;
	float m_current_time;
};

enum {
	disable_interpolation,
	enable_interpolation
};

class lagcompensation : public singleton< lagcompensation > {
	int latest_tick;
	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}
public:
	void fsn(ClientFrameStage_t stage);
	bool IsTickValid(int tick);
	void create_move();
	bool continue_loop(int id, player_t * e, ClientFrameStage_t stage);

	void apply_interpolation_flags(player_t * e, int flag);

	void animation_fix(player_t * e);
	player_record_t players[65];
	std::deque< vec3_t > last_eye_positions;
public:
	std::deque< incoming_sequence_t > sequences;
	int m_last_incoming_sequence_number;

	void update_sequence();
	void clear_sequence();
	void add_latency(INetChannel * net_channel);
};

static bool complete_restore = false;
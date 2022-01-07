#pragma once

#include "Hacks.h"
#include "Entities.h"

#include <array>
#include <deque>

#pragma once

#define LAG_COMPENSATION_TICKS 12
#define DISABLE_INTERPOLATION 0
#define ENABLE_INTERPOLATION 1

template<typename T>
class singleton
{
protected:
	singleton() {}
	~singleton() {}

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
public:
	static T& instance()
	{
		static T inst{};
		return inst;
	}
};

class C_Tick_Record
{
public:
	explicit C_Tick_Record() : sequence(0), entity_flags(0), simulation_time(0), lower_body_yaw(0), cycle(0)
	{
	}

	~C_Tick_Record()
	{
	}

	/*
	* reset
	* Resets the tick record
	*/
	Vector origin;
	Vector abs_origin;
	Vector velocity;
	Vector object_mins;
	Vector object_maxs;
	Vector hitbox_positon;

	Vector eye_angles;
	Vector abs_eye_angles;

	int sequence;
	int entity_flags;

	float simulation_time;
	float lower_body_yaw;
	float cycle;

	std::array<float, 24> pose_paramaters;
	std::array<float, 24> rag_positions;


	bool data_filled = false;
	void reset()
	{
		if (data_filled)
			return;

		origin.clearVekt();
		velocity.clearVekt();
		object_mins.clearVekt();
		object_maxs.clearVekt();
		hitbox_positon.clearVekt();

		eye_angles.clearVekt();
		abs_eye_angles.clearVekt();

		sequence = 0;
		entity_flags = 0;

		simulation_time = 0.f;
		lower_body_yaw = 0.f;
		cycle = 0.f;

		fill(begin(pose_paramaters), end(pose_paramaters), 0.f);
		fill(begin(rag_positions), end(rag_positions), 0.f);

		data_filled = false;
	}


};

/*
* C_Simulation_Data
* Used when simulating ticks for a player (useful for storing temp data)
*/
class C_Simulation_Data
{
public:
	C_Simulation_Data() : entity(nullptr), on_ground(false)
	{
	}

	~C_Simulation_Data()
	{
	}

	IClientEntity* entity;

	Vector origin;
	Vector velocity;

	bool on_ground;

	bool data_filled = false;
};

/*
* C_Player_Record
* Used to store ticks from a player and also information about the player
*/
class C_Player_Record
{
public:
	C_Player_Record() : entity(nullptr), tick_count(0), being_lag_compensated(false), lower_body_yaw(0), last_lower_body_yaw_last_update(0)
	{
	}

	~C_Player_Record()
	{
	}

	/*
	* reset
	* Resets the player
	*/
	void reset()
	{
		entity = nullptr;
		tick_count = -1;
		hitbox_position.clearVekt();
		eye_angles.clearVekt();
		being_lag_compensated = false;
		lower_body_yaw_resolved = false;
		lower_body_yaw = 0.f;
		last_lower_body_yaw_last_update = 0.f;
			
		restore_record.reset();

		if (!records->empty())
			records->clear();
	}

	IClientEntity* entity;
	C_Tick_Record restore_record;
	int tick_count;
	Vector hitbox_position;
	Vector eye_angles;
	bool being_lag_compensated;
	bool lower_body_yaw_resolved = false;
	float lower_body_yaw_resolved_yaw;
	float lower_body_yaw, last_lower_body_yaw_last_update;

	std::deque<C_Tick_Record> records[LAG_COMPENSATION_TICKS];
};

/*
* C_LagCompensation
* Used for rolling back entities and correcting players positions sent from the server
*/
class C_LagCompensation : public singleton<C_LagCompensation>
{
public:
	void frame_net_update_end() const;
	
	static bool should_lag_compensate(IClientEntity* entity);
	static void set_interpolation_flags(IClientEntity* entity, int flag);
	static float get_interpolation();
	bool is_time_delta_too_large(C_Tick_Record wish_record) const;
	void update_player_record_data(IClientEntity* entity) const;
	static void store_record_data(IClientEntity* entity, C_Tick_Record* record_data);
	static void apply_record_data(IClientEntity* entity, C_Tick_Record* record_data);
	static void simulate_movement(C_Simulation_Data* data);
	void predict_player(IClientEntity* entity, C_Tick_Record* current_record, C_Tick_Record* next_record) const;
	bool get_lowerbodyyaw_update_tick(IClientEntity* entity, C_Tick_Record* tick_record, int* record_index) const;
	int start_lag_compensation(IClientEntity* entity, int wish_tick, C_Tick_Record* output_record = nullptr) const;
	void start_position_adjustment() const;
	void start_position_adjustment(IClientEntity* entity) const;
	void finish_position_adjustment() const;
	static void finish_position_adjustment(IClientEntity* entity);
	static void reset();
	C_Player_Record player_lag_records[64];


};

extern C_LagCompensation * lagcomp;

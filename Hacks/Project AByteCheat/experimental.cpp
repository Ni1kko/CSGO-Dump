
#include "experimental.h"
#include "RageBot.h"
#include "Hooks.h"
C_LagCompensation * lagcomp = new C_LagCompensation();

#define TIME_TO_TICKS( dt ) ( (int)( 0.5f + (float)(dt) /interfaces::globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )  ( interfaces::globals->interval_per_tick * ( t ) )
#define ROUND_TO_TICKS( t ) ( interfaces::globals->interval_per_tick * TIME_TO_TICKS( t ) )

void C_LagCompensation::frame_net_update_end() const
{
	if (hackManager.pLocal() == nullptr || !hackManager.pLocal()->IsAlive() || hackManager.pLocal()->GetWeapon2() == nullptr)
	{
		reset();
		return;
	}

	for (auto index = 1; index < interfaces::globals->maxClients; index++)
	{
		auto entity = interfaces::ent_list->get_client_entity(index);

		if (!entity)
			continue;

		if (!should_lag_compensate(entity))
		{
			C_LagCompensation::instance().player_lag_records[index].records->clear();
			continue;
		}

		set_interpolation_flags(entity, DISABLE_INTERPOLATION);
		update_player_record_data(entity);
	}
}

bool C_LagCompensation::should_lag_compensate(IClientEntity* entity)
{

	if (!hackManager.pLocal()->isValidPlayer())
		return false;

	if (!entity->isValidPlayer())
		return false;

	if (entity->team() == hackManager.pLocal()->team())
		return false;

	return true;
}

/*
* set_interpolation_flags
* Sets the interpolation flags of the player
*/
void C_LagCompensation::set_interpolation_flags(IClientEntity* entity, int flag)
{
	auto var_map = reinterpret_cast<uintptr_t>(entity) + 36; // tf2 = 20
	auto var_map_list_count = *reinterpret_cast<int*>(var_map + 20);

	for (auto index = 0; index < var_map_list_count; index++)
		*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = flag;
}

/*
* get_interpolation
* Returns the interpolation per tick
*/

template<class T, class U>
T std_clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	if (in >= high)
		return high;
	return in;
}

float C_LagCompensation::get_interpolation()
{
	static auto cl_interp = interfaces::cvar->FindVar("cl_interp");
	static auto cl_interp_ratio = interfaces::cvar->FindVar("cl_interp_ratio");
	static auto sv_client_min_interp_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	static auto sv_client_max_interp_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
	static auto cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
	static auto sv_minupdaterate = interfaces::cvar->FindVar("sv_minupdaterate");
	static auto sv_maxupdaterate = interfaces::cvar->FindVar("sv_maxupdaterate");

	auto updaterate = std_clamp(cl_updaterate->GetFloat(), sv_minupdaterate->GetFloat(), sv_maxupdaterate->GetFloat());
	auto lerp_ratio = std_clamp(cl_interp_ratio->GetFloat(), sv_client_min_interp_ratio->GetFloat(), sv_client_max_interp_ratio->GetFloat());
	return std_clamp(lerp_ratio / updaterate, cl_interp->GetFloat(), 1.0f);
}

/*
* is_time_delta_too_large
* Returns if the time delta between now and wish tick is too out
*/
bool C_LagCompensation::is_time_delta_too_large(C_Tick_Record wish_record) const
{
	auto predicted_arrival_tick = TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetAvgLatency(0) + interfaces::engine->GetNetChannelInfo()->GetAvgLatency(1) + (ragebot->cmd->tick_count + interfaces::client_state->chokedcommands + 1));

	auto lerp_time = get_interpolation();
	auto correct = std_clamp(interfaces::engine->GetNetChannelInfo()->GetAvgLatency(0) + lerp_time, 0.f, 1.f);
	auto lag_delta = correct - TICKS_TO_TIME(predicted_arrival_tick + TIME_TO_TICKS(lerp_time - wish_record.simulation_time + lerp_time));

	return abs(lag_delta) > static_cast<float>(200.f) / 1000.f;
}

/*
* update_player_record_data
* Updates a players record data
*/
void C_LagCompensation::update_player_record_data(IClientEntity* entity) const
{
	if (entity == nullptr)
		return;
	if (!entity->isValidPlayer())
		return;
	if (!should_lag_compensate(entity))
		return;

	auto player_index = entity->GetIndex();
	auto player_record = &C_LagCompensation::instance().player_lag_records[player_index];

	if (entity->GetSimulationTime() > 0.f && (player_record->records->empty() || player_record->records->size() > 0 && player_record->records->at(0).simulation_time != entity->GetSimulationTime()))
	{
		C_Tick_Record new_record;
		store_record_data(entity, &new_record);

		if (new_record.data_filled)
		{
			player_record->records->push_front(new_record);

			while (player_record->records->size() > LAG_COMPENSATION_TICKS)
				player_record->records->pop_back();
		}
	}
}

/*
* store_record_data
* Stores a players data into a tick record
*/
void C_LagCompensation::store_record_data(IClientEntity* entity, C_Tick_Record* record_data)
{
	if (entity == nullptr)
		return;

	if (record_data == nullptr)
		return;

	record_data->origin = entity->GetOrigin();
	record_data->abs_origin = entity->GetAbsOrigin();
	record_data->velocity = entity->GetVelocity();
	record_data->object_mins = entity->get_obbmins();
	record_data->object_maxs = entity->get_obbmaxs();
	record_data->eye_angles = entity->GetEyeAngles();
	record_data->abs_eye_angles = entity->get_abs_eye_angles();
	record_data->sequence = entity->get_sequence();
	record_data->entity_flags = entity->GetFlags();
	record_data->simulation_time = entity->get_simulation_time();
	record_data->lower_body_yaw = entity->get_lower_body_yaw();
	record_data->cycle = entity->GetCycle2();
	//	record_data->pose_paramaters = entity->get_pose_paramaters();
	//	record_data->rag_positions = entity->get_ragdoll_pos();

	record_data->data_filled = true;
}

void C_LagCompensation::apply_record_data(IClientEntity* entity, C_Tick_Record* record_data)
{
	if (entity == nullptr || !record_data->data_filled)
		return;

	entity->GetOrigin() = record_data->origin;
	entity->GetVelocity() = record_data->velocity;
	entity->get_obbmins() = record_data->object_mins;
	entity->get_obbmaxs() = record_data->object_maxs;
	entity->GetEyeAngles() = record_data->eye_angles;
	entity->get_abs_eye_angles() = record_data->abs_eye_angles;
	entity->get_sequence() = record_data->sequence;
	entity->getFlags() = record_data->entity_flags;
	entity->get_simulation_time() = record_data->simulation_time;
	entity->get_lower_body_yaw() = record_data->lower_body_yaw;
	entity->GetCycle2() = record_data->cycle;
	//	entity->get_pose_paramaters() = record_data->pose_paramaters;
	//	entity->get_ragdoll_pos() = record_data->rag_positions;
	entity->SetAbsAngles(record_data->abs_eye_angles);
	entity->SetAbsOriginal(record_data->abs_origin);
}

/*
* simulate_movement
* Extrapolates the player forwards a tick
*/
void C_LagCompensation::simulate_movement(C_Simulation_Data* data)
{
	trace_t trace;
	CTraceFilter filter;
	filter.pSkip = hackManager.pLocal();

	auto sv_gravity = interfaces::cvar->FindVar("sv_gravity")->GetInt();
	auto sv_jump_impulse = interfaces::cvar->FindVar("sv_jump_impulse")->GetFloat(); // math.sqrt(91200) = 301.1
	auto gravity_per_tick = sv_gravity * interfaces::globals->interval_per_tick;
	auto predicted_origin = data->origin;

	if (data->on_ground)
		data->velocity.z -= gravity_per_tick;

	predicted_origin += data->velocity * interfaces::globals->interval_per_tick;
	Ray_t ray;
	ray.Init(data->origin, predicted_origin, data->entity->get_obbmins(), data->entity->get_obbmaxs());
	interfaces::trace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

	if (trace.fraction == 1.f)
		data->origin = predicted_origin;
	Ray_t ray2;
	ray.Init(data->origin, data->origin - Vector(0.f, 0.f, 2.f), data->entity->get_obbmins(), data->entity->get_obbmaxs());
	interfaces::trace->TraceRay(ray2, CONTENTS_SOLID, &filter, &trace);

	data->on_ground = trace.fraction == 0.f;
}

/*
* predict_player
* Attempts to predict the player when they have broken lag compensation
*/
constexpr float rad_2_deg(float radian)
{
	return radian * (180.f / PI);
}

/*
* deg_2_rad
* Converts a degree to a radian
*/
constexpr float deg_2_rad(float degree)
{
	return degree * (PI / 180.f);
}

float normalize_angle(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}

	return angle;
}

void C_LagCompensation::predict_player(IClientEntity* entity, C_Tick_Record* current_record, C_Tick_Record* next_record) const
{
	if (!should_lag_compensate(entity))
		return;

	// Create Simulation Data
	C_Simulation_Data simulation_data;

	simulation_data.entity = entity;
	simulation_data.origin = current_record->origin;
	simulation_data.velocity = current_record->velocity;
	simulation_data.on_ground = current_record->entity_flags & FL_ONGROUND;
	simulation_data.data_filled = true;

	// Calculate Delta's
	auto simulation_time_delta = current_record->simulation_time - next_record->simulation_time;
	auto simulation_tick_delta = std_clamp(TIME_TO_TICKS(simulation_time_delta), 1, 15);
	auto delta_ticks = (std_clamp(TIME_TO_TICKS(interfaces::engine->GetNetChannelInfo()->GetAvgLatency(1) + interfaces::engine->GetNetChannelInfo()->GetAvgLatency(0)) + interfaces::globals->tickcount - TIME_TO_TICKS(current_record->simulation_time + get_interpolation()), 0, 100)) - simulation_tick_delta;

	// Calculate movement delta
	auto current_velocity_angle = rad_2_deg(atan2(current_record->velocity.y, current_record->velocity.x));
	auto next_velocity_angle = rad_2_deg(atan2(next_record->velocity.y, next_record->velocity.x));
	auto velocity_angle_delta = normalize_angle(current_velocity_angle - next_velocity_angle);
	auto velocity_movement_delta = velocity_angle_delta / simulation_time_delta;

	if (delta_ticks > 0 && simulation_data.data_filled)
	{
		for (; delta_ticks >= 0; delta_ticks -= simulation_tick_delta)
		{
			auto ticks_left = simulation_tick_delta;
			do
			{
				simulate_movement(&simulation_data);
				current_record->simulation_time += interfaces::globals->interval_per_tick;

				--ticks_left;
			} while (ticks_left);
		}

		current_record->origin = simulation_data.origin;
		current_record->abs_origin = simulation_data.origin;
	}
}


bool C_LagCompensation::get_lowerbodyyaw_update_tick(IClientEntity* entity, C_Tick_Record* tick_record, int* record_index) const
{
	if (!should_lag_compensate(entity))
		return false;

	auto player_index = entity->GetIndex();
	auto player_record = &C_LagCompensation::instance().player_lag_records[player_index];

	for (auto index = 0; index < 8; index++)
	{
		if (index + 1 > player_record->records->size() - 1)
			return false;

		auto current_record = player_record->records->at(index);

		if (current_record.data_filled && !is_time_delta_too_large(current_record) && player_record->last_lower_body_yaw_last_update == current_record.simulation_time)
		{
			*tick_record = current_record;
			*record_index = index;
			return true;
		}
	}

	return false;
}

/*
* start_lag_compensation
* Attempts to roll the player back
*/
int C_LagCompensation::start_lag_compensation(IClientEntity* entity, int wish_tick, C_Tick_Record* output_record) const
{
	if (!should_lag_compensate(entity))
		return -1;

	auto player_index = entity->GetIndex();
	auto player_record = C_LagCompensation::instance().player_lag_records[player_index];

	if (player_record.records->empty() || wish_tick + 1 > player_record.records->size() - 1)
		return -1;

	auto current_record = player_record.records->at(wish_tick);
	auto next_record = player_record.records->at(wish_tick + 1);

	if (!current_record.data_filled || !next_record.data_filled || wish_tick > 0 && is_time_delta_too_large(current_record))
		return -1;

	if (wish_tick == 0 && (current_record.origin - next_record.origin).LengthSqr() > 4096.f)
		predict_player(entity, &current_record, &next_record);

	if (output_record != nullptr && current_record.data_filled)
		*output_record = current_record;

	return TIME_TO_TICKS(current_record.simulation_time + get_interpolation());
}

/*
* start_position_adjustment
* Function where position adjustment is called on everyone
*/
void C_LagCompensation::start_position_adjustment() const
{

	for (auto index = 1; index < interfaces::globals->maxClients; index++)
	{
		auto entity = interfaces::ent_list->get_client_entity(index);

		if (!should_lag_compensate(entity))
			continue;

		auto player_record = &C_LagCompensation::instance().player_lag_records[entity->GetIndex()];

		if (player_record->records->size() <= 2)
			return;

		player_record->being_lag_compensated = true;
		start_position_adjustment(entity);
		player_record->being_lag_compensated = false;
	}
}

/*
* start_position_adjustment
* Starts position adjustment on the entity given
*/
void C_LagCompensation::start_position_adjustment(IClientEntity* entity) const
{
	if (!should_lag_compensate(entity))
		return;

	/*
	* test_and_apply_record
	*/
	auto test_and_apply_record = [](IClientEntity* _entity, C_Player_Record* player_record, C_Tick_Record* restore_record, C_Tick_Record* corrected_record, int tick_count, bool lby_resolved) -> bool
	{
		apply_record_data(_entity, corrected_record);
		corrected_record->hitbox_positon = GetHitboxPosition(_entity, 0);
		apply_record_data(_entity, restore_record);

		if (corrected_record->hitbox_positon.empty())
			return false;

		player_record->tick_count = tick_count;
		player_record->eye_angles = corrected_record->eye_angles;
		player_record->hitbox_position = corrected_record->hitbox_positon;
		player_record->lower_body_yaw_resolved = lby_resolved;
		player_record->lower_body_yaw_resolved_yaw = corrected_record->lower_body_yaw;

		return true;
	};

	auto player_index = entity->GetIndex();
	auto player_record = &C_LagCompensation::instance().player_lag_records[player_index];

	if (player_record->records->empty())
		return;

	player_record->tick_count = -1;
	player_record->hitbox_position.clearVekt();
	player_record->eye_angles.clearVekt();
	player_record->lower_body_yaw_resolved = false;
	player_record->lower_body_yaw_resolved_yaw = 0.f;

	store_record_data(entity, &player_record->restore_record);

	C_Tick_Record lby_record;
	int lby_index;

	// Just normally loop as we would
	for (auto index = 0; index < player_record->records->size(); index++)
	{
		if (index + 1 > player_record->records->size() - 1 && index > 0)
			break;

		C_Tick_Record corrected_record;
		auto current_record = &player_record->records->at(index);

		auto tick_count = start_lag_compensation(entity, index, &corrected_record);

		if (tick_count == -1 || !corrected_record.data_filled)
			break;

		if (test_and_apply_record(entity, player_record, &player_record->restore_record, &corrected_record, tick_count, false))
			return;
	}
}

/*
* finish_position_adjustment
* Function where position adjustment is called on everyone
*/
void C_LagCompensation::finish_position_adjustment() const
{

	for (auto index = 1; index < interfaces::globals->maxClients; index++)
	{
		auto entity = interfaces::ent_list->get_client_entity(index);

		if (!should_lag_compensate(entity))
			continue;

		finish_position_adjustment(entity);
	}
}

/*
* finish_position_adjustment
* Finishes and cleans up position adjustment
*/
void C_LagCompensation::finish_position_adjustment(IClientEntity* entity)
{
	if (!should_lag_compensate(entity))
		return;

	auto player_index = entity->GetIndex();
	auto player_record = &C_LagCompensation::instance().player_lag_records[player_index];

	if (!player_record->restore_record.data_filled)
		return;

	apply_record_data(entity, &player_record->restore_record);

	player_record->restore_record.data_filled = false; // Set to false so that we dont apply this record again if its not set next time
}

/*
* reset
*/
void C_LagCompensation::reset()
{
	for (auto i = 0; i < 64; i++)
		C_LagCompensation::instance().player_lag_records[i].reset();
}


























































































































































































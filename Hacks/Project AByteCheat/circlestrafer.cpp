#include "circlestrafer.h"
#define M_PI 3.14159265358979323846
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
float get_move_angle(float speed)
{
	auto move_angle = RAD2DEG(asin(15.f / speed));
	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;
	return move_angle;
}
bool get_closest_plane(Vector* plane)
{
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return false;
	trace_t trace; CTraceWorldOnly filter; Ray_t ray;
	auto start = local_player->m_VecORIGIN(), mins = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();
	Vector planes;
	auto count = 0;
	for (auto step = 0.f; step <= M_PI * 2.f; step += M_PI / 10.f)
	{
		auto end = start;
		end.x += cos(step) * 64.f;
		end.y += sin(step) * 64.f;
		ray.Init(start, end, mins, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		if (trace.fraction < 1.f)
		{
			planes += trace.plane.normal;
			count++;
		}
	}
	planes /= count;
	if (planes.z < 0.1f) { *plane = planes; return true; }
	return false;
}
bool will_hit_obstacle_in_future(float predict_time, float step)
{
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return false;
	static auto sv_gravity = interfaces::cvar->FindVar("sv_gravity");
	static auto sv_jump_impulse = interfaces::cvar->FindVar("sv_jump_impulse");
	bool ground = local_player->GetFlags() & FL_ONGROUND;
	auto gravity_per_tick = sv_gravity->GetFloat() * interfaces::globals->interval_per_tick;
	auto start = local_player->m_VecORIGIN(), end = start;
	auto velocity = local_player->GetVelocity();
	auto min = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();
	trace_t trace;
	CTraceWorldOnly filter;
	Ray_t ray;
	auto predicted_ticks_needed = TIME_TO_TICKS(predict_time);
	auto velocity_rotation_angle = RAD2DEG(atan2(velocity.y, velocity.x));
	auto ticks_done = 0;
	if (predicted_ticks_needed <= 0)
		return false;
	while (true)
	{
		auto rotation_angle = velocity_rotation_angle + step;
		velocity.x = cos(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.y = sin(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.z = ground ? sv_jump_impulse->GetFloat() : velocity.z - gravity_per_tick;
		end += velocity * interfaces::globals->interval_per_tick;
		ray.Init(start, end, min, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		if (trace.fraction != 1.f && trace.plane.normal.z <= 0.9f || trace.startsolid || trace.allsolid)
			break;
		end = trace.endpos;
		end.z -= 2.f;
		ray.Init(trace.endpos, end, min, maxs);
		interfaces::trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		ground = (trace.fraction < 1.f || trace.allsolid || trace.startsolid) && trace.plane.normal.z >= 0.7f;
		if (++ticks_done >= predicted_ticks_needed)
			return false;
		velocity_rotation_angle = rotation_angle;
	}
	return true;
}
float NormalizeAngle(float angle)
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

// NO WORK, FIX ME UP 
void circlestrafe(CUserCmd* cmd, float* circle_yaw)
{
	if (!options::menu.MiscTab.CircleStrafe.GetState())
		return;
	auto local_player = interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	if (!local_player) return;
	const auto min_step = 2.25f;
	const auto max_step = 5.f;
	auto velocity_2d = local_player->GetVelocity().Length2D();
	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto ideal_strafe = clamp(get_move_angle(velocity_2d) * 2.f, min_step, max_step);
	auto predict_time = clamp(295.f / velocity_2d, 0.25f, 1.15f);
	//auto predict_time = clamp(circlevel / velocity_2d, circlemin, circlemax);
	auto step = ideal_strafe;
	while (true)
	{
		if (!will_hit_obstacle_in_future(predict_time, step) || step > max_step)
			break;
		step += 0.2f;
	}
	if (step > max_step)
	{
		step = ideal_strafe;
		while (true)
		{
			if (!will_hit_obstacle_in_future(predict_time, step) || step < -min_step)
				break;
			step -= 0.2f;
		}
		if (step < -min_step)
		{
			Vector plane;
			if (get_closest_plane(&plane))
				//step = -MATH::NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * circlemulti;
				step = -NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * 0.03f;
		}
		else
			step -= 0.2f;
	}
	else
		step += 0.2f;
	cmd->viewangles.y = *circle_yaw = NormalizeAngle(*circle_yaw + step);
	cmd->sidemove = copysign(450.f, -step);
}


























































































































































































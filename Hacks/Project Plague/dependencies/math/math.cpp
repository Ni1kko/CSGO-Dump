#include "../common_includes.hpp"

//aimtux
void math::correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove) {
	float yaw_delta = cmd->viewangles.y - old_angles.y;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	cmd->forwardmove = cos(DEG2RAD(yaw_delta)) * old_forwardmove + cos(DEG2RAD(yaw_delta + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(yaw_delta)) * old_forwardmove + sin(DEG2RAD(yaw_delta + 90.f)) * old_sidemove;
}

vec3_t math::calculate_angle(const vec3_t& src, const vec3_t& dst) {
	/*vec3_t angles;
	vec3_t delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;*/
	vec3_t angle;
	vector_angles(dst - src, angle);

	return angle;
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b[0]) + b[0][3];
	out.y = a.dot(b[1]) + b[1][3];
	out.z = a.dot(b[2]) + b[2][3];
}


vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::screen_transform(const vec3_t & point, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	screen.x = matrix.m[0][0] * point.x + matrix.m[0][1] * point.y + matrix.m[0][2] * point.z + matrix.m[0][3];
	screen.y = matrix.m[1][0] * point.x + matrix.m[1][1] * point.y + matrix.m[1][2] * point.z + matrix.m[1][3];
	screen.z = 0.0f;

	float w = matrix.m[3][0] * point.x + matrix.m[3][1] * point.y + matrix.m[3][2] * point.z + matrix.m[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return false;
	}

	screen.x /= w;
	screen.y /= w;

	return true;
}

bool math::world_to_screen(const vec3_t & origin, vec3_t & screen) {
	if (screen_transform(origin, screen)) {
		int w, h;
		interfaces::engine->get_screen_size(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}
	return false;
}

float math::distance_point_to_line(vec3_t Point, vec3_t LineOrigin, vec3_t Dir) {
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).length();
}

#include "directxmath.h"
void math::angle_vectors(const vec3_t& angles, vec3_t& forward) {
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles.x));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles.y));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void math::angle_vectors(const vec3_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up) {
	float sr, sp, sy, cr, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
	DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

void math::vector_angles(const vec3_t& forward, vec3_t& angles) {
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0) {
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else {
		yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

float math::get_fov(const vec3_t& view_angle, const vec3_t& aim_angle)
{
	vec3_t ang, aim;

	angle_vectors(view_angle, aim);
	angle_vectors(aim_angle, ang);

	return RAD2DEG(acos(aim.dot(ang) / aim.length_sqr()));
}
void math::normalize_angle(vec3_t& angles) {
	for (auto i = 0; i < 3; i++) {
		while (angles[i] < -180.0f) angles[i] += 360.0f;
		while (angles[i] > 180.0f) angles[i] -= 360.0f;
	}
}
void math::clamp_angle(vec3_t& angles) {
	angles.x = std::clamp(angles.x, -89.0f, 89.0f);
	angles.y = std::clamp(angles.y, -180.0f, 180.0f);
	angles.z = 0.0f;
}
bool math::sanitize_angle(vec3_t& angles)
{
	vec3_t temp = angles;
	normalize_angle(temp);
	clamp_angle(temp);

	if (!isfinite(temp.x) ||
		!isfinite(temp.y) ||
		!isfinite(temp.z))
		return false;

	angles = temp;

	return true;
}
float math::angle_diff(float dest_ang, float src_ang) {
	float delta;

	delta = fmodf(dest_ang - src_ang, 360.0f);
	if (dest_ang > src_ang)
	{
		if (delta >= 180)
			delta -= 360;
	}
	else
	{
		if (delta <= -180)
			delta += 360;
	}
	return delta;

}
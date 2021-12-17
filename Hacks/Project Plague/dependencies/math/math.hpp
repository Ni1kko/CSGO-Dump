#pragma once
constexpr double pi = 3.14159265358979323846;

namespace math {
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	vec3_t calculate_angle(const vec3_t& src, const vec3_t& dst);
	void sin_cos(float r, float* s, float* c);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void angle_vectors(const vec3_t& angles, vec3_t& forward);
	void angle_vectors(const vec3_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up);
	void vector_angles(const vec3_t& forward, vec3_t& angles);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t& origin, vec3_t& screen);
	float distance_point_to_line(vec3_t Point, vec3_t LineOrigin, vec3_t Dir);
	float get_fov(const vec3_t& view_angle, const vec3_t& aim_angle);
	void normalize_angle(vec3_t& angles);
	void clamp_angle(vec3_t& angles);
	bool sanitize_angle(vec3_t& angles);
	float angle_diff(float dest_ang, float src_ang);
};
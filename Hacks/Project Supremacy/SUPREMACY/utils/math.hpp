#pragma once

#include "..\includes.hpp"
#include "..\sdk\math\VMatrix.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

namespace math {
	void normalize_angles(Vector & angles);
	float angle_distance(float firstangle, float secondangle);
	float lowerbody_rotate(float lby, float yaw);
	void normalize_angle(QAngle & angles);
	void normalize_vector(vec3_t & vector);
	float normalize_yaw(float f);
	float normalize_pitch(float pitch);
	float vector_normalize(Vector& v);
	void clamp_angles(Vector & angles);
	void vector_transform(const Vector& in1, const matrix3x4_t & in2, Vector & out);
	vec3_t calculate_angle(vec3_t src, vec3_t dst);
	float get_fov(vec3_t viewangles, vec3_t aim_angle);
	void AngleVectors(const Vector &angles, Vector *forward);
	void angle_vectors(const Vector & angles, Vector & forward);
	void angle_vectors(const Vector &angles, Vector *forward, Vector *right, Vector* up);
	void angle_vectors(const QAngle & angles, Vector & forward);
	vec3_t angle_vectors(vec3_t & angle);
	void vector_angles(const Vector & forward, Vector & angles);
	void vector_angles(const Vector & forward, QAngle & angles);
	void vector_angles(const Vector& forward, Vector& up, Vector& angles);
	Vector cross_product(const Vector & a, const Vector & b);
	float dot_product(const Vector & a, const Vector & b);
	bool world_to_screen(const Vector &origin, Vector &screen);
	void rotate_movement(float yaw);

	float random_float(float min, float max);
	void random_seed(int seed);
	template<class T, class U>
	static T clamp(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	inline float clamp_yaw(float yaw)
	{
		while (yaw > 180.f)
			yaw -= 360.f;
		while (yaw < -180.f)
			yaw += 360.f;
		return yaw;
	}
}
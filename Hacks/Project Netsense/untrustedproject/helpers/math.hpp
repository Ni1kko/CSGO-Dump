#pragma once

#include "../valve_sdk/sdk.hpp"
#include <Windows.h>
#include <DirectXMath.h>
#include <array>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

#define M_PI 3.14159265358979323846
#define PI_F	((float)(M_PI)) 
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TICKS_TO_TIME(t) (g_GlobalVars->interval_per_tick * (t) )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

#ifndef max
#define max(a ,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

namespace Math
{
	inline float FASTSQRT(float x)
	{
		unsigned int i = *(unsigned int*)&x;

		i += 127 << 23;
		// approximation of square root
		i >>= 1;
		return *(float*)&i;
	}
	float VectorDistance(const Vector& v1, const Vector& v2);
	float AngleNormalize(float angle);
	float ApproachAngle(float target, float value, float speed);
	QAngle CalcAngle(const Vector& src, const Vector& dst);
	float RandomFloat(float min, float max);
	void VectorAnglesHitchance(const Vector& forward, Vector& up, QAngle& angles);
	float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle);
	template<class T>
	void Normalize3(T& vec)
	{
		for (auto i = 0; i < 2; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] >  180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
    void ClampAngles(QAngle& angles);
    void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
    void AngleVectors(const QAngle &angles, Vector& forward);
    void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
    void VectorAngles(const Vector& forward, QAngle& angles);
    bool WorldToScreen(const Vector& in, Vector& out);
	void FixAngles(QAngle& angles);
	float NormalizeYaw(float value);
	template < typename T >
	T Clamp(T in, T low, T high) {
		return min(max(in, low), high);
	}
	void RotateTriangle(std::array<ImVec2, 3>& points, float rotation);
	void MovementFix(CUserCmd* cmd, QAngle wish_angle, QAngle old_angles);
	float AngleDiff(float destAngle, float srcAngle);
}
#pragma once
#include "../valve_sdk/sdk.hpp"
#include <DirectXMath.h>
#define PI 3.14159265358979323846f
#define PI_F	((float)(PI)) 
#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
namespace Math
{
	void NormalizeAngles(QAngle& angles);
	void NormalizeVector(Vector& vec);
	void ClampAngles(QAngle& angles);
	void FixAngles(QAngle& angles);
	QAngle CalcAngle(Vector src, Vector dst);
	float GetFov(const QAngle &viewAngle, const QAngle &aimAngle);
	float GetDistance(Vector src, Vector dst);
	void SmoothAngle(QAngle src, QAngle &dst, float factor);
	void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void AngleVectors(const QAngle &angles, Vector& forward);
	void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
	void VectorAngles(const Vector& forward, QAngle& angles);
	void VectorAngles(const Vector& forward, Vector& up, QAngle& angles);
	bool WorldToScreen(const Vector& in, Vector& out);
	float DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along = nullptr, Vector *pointOnRay = nullptr);
	inline float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
	{
		auto PointDir = Point - LineOrigin;
		auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
		if (TempOffset < 0.000001f)
			return FLT_MAX;
		auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);
		return (Point - PerpendicularPoint).Length();
	}
	inline float ClampYaw(float yaw)
	{
		while (yaw > 180.f)
			yaw -= 360.f;
		while (yaw < -180.f)
			yaw += 360.f;
		return yaw;
	}
}
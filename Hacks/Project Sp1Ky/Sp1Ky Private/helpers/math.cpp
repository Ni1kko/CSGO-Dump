#include "math.hpp"
namespace Math
{
	//--------------------------------------------------------------------------------
	void FixAngles(QAngle& angles)
	{
		NormalizeAngles(angles);
		ClampAngles(angles);
	}
	float GetFov(const QAngle &viewAngle, const QAngle &aimAngle)
	{
		Vector ang, aim;
		AngleVectors(viewAngle, aim);
		AngleVectors(aimAngle, ang);
		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}
	float GetDistance(Vector src, Vector dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2) + pow(src.z - dst.z, 2));
	}
	QAngle CalcAngle(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = src - dst;
		VectorAngles(delta, angles);
		angles.Normalize();
		return angles;
	}
	void SmoothAngle(QAngle src, QAngle &dst, float factor)
	{
		QAngle delta = dst - src;
		NormalizeAngles(delta);
		dst = src + delta / factor;
	}
	//--------------------------------------------------------------------------------
	void NormalizeAngles(QAngle& angles)
	{
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}
	void NormalizeVector(Vector& vec)
	{
		for (auto i = 0; i < 3; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
	float DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along, Vector *pointOnRay)
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.Normalize();

		float rangeAlong = dir.Dot(to);
		if (along)
			*along = rangeAlong;

		float range;

		if (rangeAlong < 0.0f)
		{
			range = -(pos - rayStart).Length();

			if (pointOnRay)
				*pointOnRay = rayStart;
		}
		else if (rangeAlong > length)
		{
			range = -(pos - rayEnd).Length();

			if (pointOnRay)
				*pointOnRay = rayEnd;
		}
		else
		{
			Vector onRay = rayStart + rangeAlong * dir;
			range = (pos - onRay).Length();

			if (pointOnRay)
				*pointOnRay = onRay;
		}

		return range;
	}
	//--------------------------------------------------------------------------------
	void ClampAngles(QAngle& angles)
	{
		if (angles.pitch > 89.0f) angles.pitch = 89.0f;
		else if (angles.pitch < -89.0f) angles.pitch = -89.0f;
		if (angles.yaw > 180.0f) angles.yaw = 180.0f;
		else if (angles.yaw < -180.0f) angles.yaw = -180.0f;
		angles.roll = 0;
	}
	//--------------------------------------------------------------------------------
	void SinCos(float a, float* s, float*c)
	{
		*s = sin(a);
		*c = cos(a);
	}
	//--------------------------------------------------------------------------------
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	void AngleVectors(const QAngle &angles, Vector& forward)
	{
		float	sp, sy, cp, cy;
		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	//--------------------------------------------------------------------------------
	void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);
		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = (cr * cp);
	}
	//--------------------------------------------------------------------------------
	float AngleNormalize(float angle)
	{
		angle = fmodf(angle, 360.0f);
		if (angle > 180)
		{
			angle -= 360;
		}
		if (angle < -180)
		{
			angle += 360;
		}
		return angle;
	}
	void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
	{
		float deltaView;
		float f1;
		float f2;
		if (vOldAngles.yaw < 0.f)
			f1 = 360.0f + vOldAngles.yaw;
		else
			f1 = vOldAngles.yaw;
		if (pCmd->viewangles.yaw < 0.0f)
			f2 = 360.0f + pCmd->viewangles.yaw;
		else
			f2 = pCmd->viewangles.yaw;
		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;
		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	}
	//--------------------------------------------------------------------------------
	void VectorAngles(const Vector& forward, QAngle& angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;
	}

	Vector CrossProduct(const Vector &a, const Vector &b)
	{
		return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}

	void VectorAngles(const Vector& forward, Vector& up, QAngle& angles)
	{
		Vector left = CrossProduct(up, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.pitch = atan2f(-forward.z, forwardDist) * 180 / PI_F;
			angles.yaw = atan2f(forward.y, forward.x) * 180 / PI_F;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.roll = atan2f(left.z, upZ) * 180 / PI_F;
		}
		else
		{
			angles.pitch = atan2f(-forward.z, forwardDist) * 180 / PI_F;
			angles.yaw = atan2f(-left.x, left.y) * 180 / PI_F;
			angles.roll = 0;
		}
	}
	//--------------------------------------------------------------------------------
	static bool screen_transform(const Vector& in, Vector& out)
	{
		static auto& w2sMatrix = g_EngineClient->WorldToScreenMatrix();
		out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
		out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
		out.z = 0.0f;
		float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];
		if (w < 0.001f) {
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}
		out.x /= w;
		out.y /= w;
		return true;
	}
	//--------------------------------------------------------------------------------
	bool WorldToScreen(const Vector& in, Vector& out)
	{
		if (screen_transform(in, out)) {
			int w, h;
			g_EngineClient->GetScreenSize(w, h);
			out.x = (w / 2.0f) + (out.x * w) / 2.0f;
			out.y = (h / 2.0f) - (out.y * h) / 2.0f;
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------
}
float QAngle::RealYawDifference(QAngle dst, QAngle add)
{
	dst += add;
	dst.pitch = 0.f;
	Vector ang, aim;
	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);
	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
float QAngle::Difference(QAngle dst, QAngle add)
{
	dst += add;
	Vector ang, aim;
	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);
	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}




















































































































































































































































































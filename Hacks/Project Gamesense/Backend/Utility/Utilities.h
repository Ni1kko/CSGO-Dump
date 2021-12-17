#pragma once
#include "../Include/GlobalIncludes.h"
#include "../Hashing/xorstr.h"
#include "../SDK/vector3d.h"
#include "../SDK/vmatrix.h"
#include "../SDK/vector2d.h"
#include "../SDK/CTrace.h"

#define W2S(x, n) auto ws = std::wstring(x); auto n = std::string(ws.begin(), ws.end()).c_str()

namespace Cheat
{
	class CUtilities
	{
		public:
			enum ConsoleColor
			{
				BLACK = 0,
				DARKBLUE = FOREGROUND_BLUE,
				DARKGREEN = FOREGROUND_GREEN,
				DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
				DARKRED = FOREGROUND_RED,
				DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
				DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
				DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
				GRAY = FOREGROUND_INTENSITY,
				BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
				GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
				CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
				RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
				MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
				YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
				WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
			};

			void Console_Create( std::string title = xorstr_("") );
			void Console_Close();
			void Console_Log( const char* text, ... );
			void Console_LogColored( const char* text, ConsoleColor color, ... );
			void Console_SetTitle( std::string title );
			void Console_SetColor( ConsoleColor color );

			DWORD Memory_PatternScan( std::string moduleName, std::string pattern );

			template<class T>
			constexpr const T &Util_Clamp(const T &v, const T &lo, const T &hi)
			{
				return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
			}

			float Game_DistanceScreen( Vector2D vDstPos, Vector2D vSrcPos )
			{
				return ( sqrt( pow( vSrcPos.x - vDstPos.x, 2 ) + pow( vSrcPos.y - vDstPos.y, 2 ) ) );
			}

			bool Game_WorldToScreen( const Vector &origin, Vector &screen );
			bool Game_ScreenTransform( const Vector &point, Vector &screen );
			void Game_VectorTransform( const float* in1, const VMatrix &in2, float* out );
			void Game_VectorTransform( const Vector &in1, const VMatrix &in2, Vector &out );

			void Game_Msg( const char* msg, ... );

			Vector Math_CalcAngle( Vector src, Vector dst );
			float DistanceToRay( const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr );
			void Math_SinCos( float a, float* s, float* c );
			void CUtilities::Math_Normalize3(Vector& vec);
			void CUtilities::Math_ClampAngles(Vector& angles);
			void CUtilities::Math_FixAngles(Vector& angles);
			void Math_AngleVectors( const Vector &angles, Vector &forward );
			void Math_AngleVectors( const Vector &angles, Vector* forward, Vector* right, Vector* up );
			void Math_AngleVectors( const Vector &angles, Vector &forward, Vector &right, Vector &up );
			float Game_RandomFloat( float min, float max );
			void Math_VectorAngles( const Vector &forward, Vector &up, Vector &angles );
			void Math_VectorAngles2( const Vector &forward, Vector &angles );
			float Math_Normalizefloat( float angle );
			Vector Math_CrossProduct( const Vector &a, const Vector &b );
			void Math_VectorAngles( const Vector &forward, Vector &angles );
			bool Math_IsNearEqual( float v1, float v2, float Tolerance );

			float Math_NormalizeYawInPlace( float f );
			float Math_NormalizeYaw2(float yaw);
			void Math_NormalizeYaw(float & f);
			void Math_Normalize(Vector & f);
			void Math_VectorTransform( const Vector &in1, const matrix3x4_t &in2, Vector &out );
			bool Math_VectorIsNear( const Vector &bounds, const Vector &vec1, const Vector &vec2 );
			bool IsPointVisible( const Vector &start, const Vector &end, CBaseEntity* player );
			float Math_GetFov(Vector qAngles, Vector vecSource);
			void Game_TraceLine( Vector &start, Vector &end, unsigned int mask, CBaseEntity* ignore, trace_t* tr );
			float Math_GetFov2(Vector ViewOffSet, Vector View, CBaseEntity* pEntity, int aHitBox);
			float Distance(Vector2D point1, Vector2D point2);

			float GRD_TO_BOG(float GRD);

			inline float Math_DotProduct( const float* a, const float* b )
			{
				return ( a[0] * b[0] + a[1] * b[1] + a[2] * b[2] );
			}

		private:
			FILE* m_pOutputFile = nullptr;
			HANDLE m_hOldOutput = NULL, m_hOutput = NULL;
	};

	extern CUtilities* Utilities;

	inline Vector CUtilities::Math_CalcAngle(Vector src, Vector dst)
	{
		auto ret = Vector();
		auto delta = src - dst;
		double hyp = delta.Length2D();
		ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.x = (atan(delta.z / hyp) * 57.295779513082f);
		ret[2] = 0.00;

		if (delta.x >= 0.0)
			ret.y += 180.0f;

		return ret;
	}
	inline float DotProduct(const float* a, const float* b)
	{
		return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
	}

	inline void CUtilities::Game_VectorTransform(const float* in1, const VMatrix &in2, float* out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}

	inline void CUtilities::Game_VectorTransform(const Vector &in1, const VMatrix &in2, Vector &out)
	{
		Game_VectorTransform(&in1.x, in2, &out.x);
	}

	inline float  CUtilities::GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}
	inline bool CUtilities::Math_IsNearEqual(float v1, float v2, float Tolerance)
	{
		return std::abs(v1 - v2) <= std::abs(Tolerance);
	}

	inline float CUtilities::Game_RandomFloat(float min, float max)
	{
		typedef float(*rndf)(float, float);
		static rndf m_RandomFloat = nullptr;
		if (!m_RandomFloat)
			m_RandomFloat = (rndf)GetProcAddress(GetModuleHandle(xorstr_("vstdlib.dll")), xorstr_("RandomFloat"));

		return m_RandomFloat(min, max);
	}

	inline void CUtilities::Math_VectorAngles(const Vector &forward, Vector &up, Vector &angles)
	{
		Vector left = Math_CrossProduct(up, forward);
		left.NormalizeInPlace();
		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
			angles.y = atan2f(forward.y, forward.x) * 180 / M_PI;
			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / M_PI;
		}

		else
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
			angles.y = atan2f(-left.x, left.y) * 180 / M_PI;
			angles.z = 0;
		}
	}
	inline void CUtilities::Math_VectorAngles2(const Vector &forward, Vector &angles)
	{
		if (forward.x == 0.f && forward.y == 0.f)
		{
			angles.x = forward.z > 0.f ? -90.f : 90.f;
			angles.y = 0.f;
		}

		else
		{
			angles.x = RAD2DEG(atan2(-forward.z, forward.Length2D()));
			angles.y = RAD2DEG(atan2(forward.y, forward.x));
		}

		angles.z = 0.f;
	}


	inline float CUtilities::Math_Normalizefloat(float angle)
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

	inline Vector CUtilities::Math_CrossProduct(const Vector &a, const Vector &b)
	{
		return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}


	inline float CUtilities::DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float* along, Vector* pointOnRay)
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.Normalize44();
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

		else
			if (rangeAlong > length)
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
	inline void CUtilities::Math_SinCos(float a, float* s, float* c)
	{
		*s = sin(a);
		*c = cos(a);
	}
	inline void CUtilities::Math_Normalize3(Vector& vec)
	{
		for (auto i = 0; i < 2; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
	inline void CUtilities::Math_ClampAngles(Vector& angles)
	{
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}
	inline void CUtilities::Math_FixAngles(Vector& angles)
	{
		Math_Normalize3(angles);
		Math_ClampAngles(angles);
	}
	inline void CUtilities::Math_AngleVectors(const Vector &angles, Vector &forward)
	{
		float sp, sy, cp, cy;
		Math_SinCos(DEG2RAD(angles[1]), &sy, &cy);
		Math_SinCos(DEG2RAD(angles[0]), &sp, &cp);
		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	inline void CUtilities::Math_AngleVectors(const Vector &angles, Vector* forward, Vector* right, Vector* up)
	{
		auto sincos = [](float radians, float * sine, float * cosine)
		{
			*sine = sin(radians);
			*cosine = cos(radians);
		};
		float sr, sp, sy, cr, cp, cy;
		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);
		sincos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}
	inline void CUtilities::Math_AngleVectors(const Vector &angles, Vector &forward, Vector &right, Vector &up)
	{
		float sr, sp, sy, cr, cp, cy;
		Math_SinCos(DEG2RAD(angles[1]), &sy, &cy);
		Math_SinCos(DEG2RAD(angles[0]), &sp, &cp);
		Math_SinCos(DEG2RAD(angles[2]), &sr, &cr);
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

	inline void CUtilities::Math_VectorAngles(const Vector &forward, Vector &angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;

			if (forward[2] > 0)
				pitch = 270;

			else
				pitch = 90;
		}

		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);

			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);

			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	inline float CUtilities::Math_NormalizeYawInPlace(float f)
	{
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;

		while (f < -180.0f)
			f += 360.0f;

		while (f > 180.0f)
			f -= 360.0f;

		return f;
	}

	inline float CUtilities::Math_NormalizeYaw2(float yaw)
	{
		if (yaw > 180)
			yaw -= (round(yaw / 360) * 360.f);
		else if (yaw < -180)
			yaw += (round(yaw / 360) * -360.f);

		return yaw;
	}
	inline void CUtilities::Math_NormalizeYaw(float &f)
	{
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;

		while (f < -180.0f) f += 360.0f;
		while (f > 180.0f) f -= 360.0f;
	}
	inline void CUtilities::Math_Normalize(Vector &f)
	{
		while (f.y <= -180) f.y += 360;
		while (f.y > 180) f.y -= 360;
		while (f.x <= -180) f.x += 360;
		while (f.x > 180) f.x -= 360;


		if (f.x > 89.0) f.x = 89;
		if (f.x < -89.0) f.x = -89;
		if (f.y < -180) f.y = -179.999;
		if (f.y > 180) f.y = 179.999;
	}


	inline void CUtilities::Math_VectorTransform(const Vector &in1, const matrix3x4_t &in2, Vector &out)
	{
		out.x = in1.x * in2[0][0] + in1.y * in2[0][1] + in1.z * in2[0][2] + in2[0][3];
		out.y = in1.x * in2[1][0] + in1.y * in2[1][1] + in1.z * in2[1][2] + in2[1][3];
		out.z = in1.x * in2[2][0] + in1.y * in2[2][1] + in1.z * in2[2][2] + in2[2][3];
	}

	inline bool CUtilities::Math_VectorIsNear(const Vector &bounds, const Vector &vec1, const Vector &vec2)
	{
		if (abs(vec2.x - vec1.x) <= bounds.x || abs(vec2.y - vec1.y) <= bounds.y || abs(vec2.z - vec1.z) <= bounds.y)
			return true;

		return false;
	}
}

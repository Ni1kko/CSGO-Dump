#pragma once
#include <Psapi.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CGlobalVarsBase.h"
#include "..\Utils\Utils.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )

class Utils
{
public:
	template<unsigned int IIdx, typename TRet, typename ... TArgs>
	static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
	{
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	template <typename t>
	static t GetVFunc(void* class_pointer, size_t index) {
		return (*(t**)class_pointer)[index];
	}

	static uintptr_t FindSignature(const char* szModule, const char* szSignature)
	{
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(szModule);
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;
			}
			else
			{
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return 0u;
	}

	// basefunct
	static std::string SetupStringParams(std::string szBasicString)
	{
		return szBasicString;
	}

	// Replace % with a desired string / value represented after semicolons. Works kinda like printf.
	template <typename T, typename... Targs>
	static std::string SetupStringParams(std::string szBasicString, T arg, Targs&& ...args)
	{
		const auto found = szBasicString.find_first_of('%');
		if (found != std::string::npos)
		{
			std::stringstream tmp;
			tmp << arg;
			szBasicString.replace(found, 1, tmp.str());
			szBasicString = SetupStringParams(szBasicString, std::forward<Targs>(args)...);
		}
		return szBasicString;
	}


	template <typename ... Args>
	static void Log(const std::string& str, Args ...arguments)
	{
		Utils::Log(Utils::SetupStringParams(str.c_str(), arguments...));
	}


	/**
	*   GetCurrentSystemTime - Gets actual system time
	*   @timeInfo: Reference to your own tm variable, gets modified.
	*/
	static void GetCurrentSystemTime(tm& timeInfo)
	{
		const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
		localtime_s(&timeInfo, &now_c); // using localtime_s as std::localtime is not thread-safe.
	};


	/**
	*   Log - Logs specified message to debug console if in debug mode.
	*   Format: [HH:MM:SS] str
	*   @str: Debug message to be shown.
	*/
	static void Log(const std::string& str)
	{
#ifdef _DEBUG
		tm timeInfo{ };
		Utils::GetCurrentSystemTime(timeInfo);

		std::stringstream ssTime; // Temp stringstream to keep things clean
		ssTime << "[" << std::put_time(&timeInfo, "%T") << "] " << str << std::endl;

		std::cout << ssTime.str();
#endif // _DEBUG
	};



	/**
	*   Log(HRESULT) - Sets up an error message when you pass HRESULT
	*   Message contains error code only, no specific information
	*/
	static void Log(const HRESULT hr)
	{
#ifdef _DEBUG
		std::stringstream strFormatted;
		strFormatted << "Operation failed, error code = 0x" << std::hex << hr;

		Utils::Log(strFormatted.str());
#endif // _DEBUG
	};



	/**
	*   WorldToScreen - Converts game coordinates to screen
	*   @origin: 3D coordinates to be converted
	*   @screen: Viewport coordinates to which we will convert
	*/
	static bool WorldToScreen(const Vector &origin, Vector &screen)
	{
		const auto screenTransform = [&origin, &screen]() -> bool
		{
			static std::uintptr_t pViewMatrix;
			if (!pViewMatrix)
			{
				pViewMatrix = static_cast<std::uintptr_t>(Utils::FindSignature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
				pViewMatrix += 3;
				pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
				pViewMatrix += 176;
			}

			const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
			screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
			screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];
			screen.z = 0.0f;

			float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

			if (w < 0.001f)
			{
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if (!screenTransform())
		{
			int iScreenWidth, iScreenHeight;
			g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

			screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
			screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

			return true;
		}
		return false;
	}

	static void VectorAngles(const Vector& forward, Vector& angles)
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
			yaw = (atan2(forward[1], forward[0]) * 180) / M_PI;
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180) / M_PI;
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	static void VectorTransform(const float *in1, const matrix3x4_t& in2, float *out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}
	static Vector VectorTransform(Vector vector, matrix3x4_t matrix)
	{
		return Vector(vector.Dot2(matrix[0]) + matrix[0][3], vector.Dot2(matrix[1]) + matrix[1][3], vector.Dot2(matrix[2]) + matrix[2][3]);
	}

	static void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		VectorTransform(&in1.x, in2, &out.x);
	}

	static Vector NormalizeAngle2(Vector yaw)
	{
		if (yaw.y > 180)
		{
			yaw.y -= (round(yaw.y / 360) * 360.f);
		}
		else if (yaw.y < -180)
		{
			yaw.y += (round(yaw.y / 360) * -360.f);
		}
		return yaw;
	}

	
	static float NormalizeYaw(float yaw)
	{
		if (yaw > 180)
		{
			yaw -= (round(yaw / 360) * 360.f);
		}
		else if (yaw < -180)
		{
			yaw += (round(yaw / 360) * -360.f);
		}
		return yaw;
	}
	static float NormalizePitch2(float pitch)
	{
		while (pitch > 89.f)
			pitch -= 180.f;
		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}

	static Vector NormalizeAngle(Vector angle)
	{
		angle.x = NormalizePitch2(angle.x);
		angle.y = NormalizeYaw(angle.y);

		if ((angle.z > 50) || (angle.z < 50))
			angle.z = 0;

		return angle;
	}
	static float YawDistance(float firstangle, float secondangle)
	{
		if (firstangle == secondangle)
			return 0.f;

		bool oppositeSides = false;

		if (firstangle > 0 && secondangle < 0)
			oppositeSides = true;
		else if (firstangle < 0 && secondangle > 0)
			oppositeSides = true;

		if (!oppositeSides)
			return fabs(firstangle - secondangle);

		bool past90 = false;

		if (firstangle > 90 && secondangle < -90)
		{
			firstangle -= (firstangle - 90);
			secondangle += (secondangle + 90);
		}
		else if (firstangle < -90 && secondangle > 90)
		{
			firstangle += (firstangle + 90);
			secondangle -= (secondangle - 90);
		}

		float oneTwo;

		oneTwo = fabs(firstangle - secondangle);

		return oneTwo;
	}

	inline float NormalizePitch(float pitch)
	{
		while (pitch > 89.f)
			pitch -= 180.f;
		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}

	inline Vector NormalizeAngleBame(Vector angle)
	{
		angle.x = NormalizePitch(angle.x);
		angle.y = NormalizeYaw(angle.y);

		if ((angle.z > 50) || (angle.z < 50))
			angle.z = 0;

		return angle;
	}

	static void Normalize(Vector &vIn, Vector &vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}

	static Vector CalcAngle(const Vector& src, const Vector& dst)
	{
		auto ret = Vector();
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.x = (atan(delta.z / hyp) * 57.295779513082f);
		ret.z = 0.f;

		if (delta.x >= 0.f)
			ret.y += 180.f;
		return ret;
	}

	static Vector FastCalcAngle(Vector vecOrigin, Vector vecOther)
	{
		auto ret = Vector();
		Vector delta = vecOrigin - vecOther;
		double hyp = delta.Length2D();
		ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.x = (atan(delta.z / hyp) * 57.295779513082f);
		ret.z = 0.f;

		if (delta.x >= 0.f)
			ret.y += 180.f;
		return ret;
	}

	static Vector CalcAngle2(const Vector& vecSource, const Vector& vecDestination)
	{
		auto ret = Vector();
		Vector delta = vecSource - vecDestination;
		double hyp = delta.Length2D();
		ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.x = (atan(delta.z / hyp) * 57.295779513082f);
		ret.z = 0.f;

		if (delta.x >= 0.f)
			ret.y += 180.f;
		return ret;
	}


	static void NormaliseViewAngle(Vector &angle)
	{

		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	}
	static void CalcAngle(Vector src, Vector dst, Vector &angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D(); //delta.Length
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		angles[2] = 0.00;

		if (delta.x >= 0.0)
			angles.y += 180.0f;
	}

	static void inline SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);

	}

	static void AngleVectors(const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	static void AngleVectorsbeta(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

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

	static void NormalizeLemon(Vector& angle)
	{
		while (angle.x > 89.0f) {
			angle.x -= 180.f;
		}
		while (angle.x < -89.0f) {
			angle.x += 180.f;
		}
		while (angle.y > 180.f) {
			angle.y -= 360.f;
		}
		while (angle.y < -180.f) {
			angle.y += 360.f;
		}
	}

	static void ClampAnglesLemon(Vector &angles)
	{
		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		angles.z = 0;
	}

	static bool ClampLemon(Vector &angles)
	{
		Vector a = angles;
		NormalizeLemon(a);
		ClampAnglesLemon(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}
};

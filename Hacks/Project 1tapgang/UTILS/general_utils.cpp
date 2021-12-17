#include "../includes.h"

#include "general_utils.h"

#include "interfaces.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CUserCmd.h"
#include "../UTILS/render.h"
#include "../SDK/CTrace.h"

#include <corecrt.h>
#include <iosfwd>
namespace UTILS
{
	HANDLE _out = NULL;
	
	bool ConsolePrint(bool logToFile, const char *fmt, ...)
	{
		char buf[1024] = { 0 };
		va_list va;

		va_start(va, fmt);
		_vsnprintf_s(buf, 1024, fmt, va);
		va_end(va);

		std::string outRes = buf;

		OutputDebugString(outRes.c_str());

		

		if (!_out)
			return false;

		return !!WriteConsoleA(_out, outRes.c_str(), static_cast<DWORD>(strlen(outRes.c_str())), nullptr, nullptr);
	}
	const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y | %d | %X", &tstruct);

		return buf;
	}
	Vector TraceToEnd(Vector start, Vector end)
	{
		SDK::trace_t trace;
		SDK::CTraceWorldOnly filter;
		SDK::Ray_t ray;

		ray.Init(start, end);
		INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

		return trace.end;
	}

	const char* GetHitgroupName(int hitgroup)
	{
		switch (hitgroup)
		{
		case 1:
			return "Head";
		case 2:
			return "Chest";
		case 3:
			return "Pelvis";
		case 4:
			return "Left Arm";
		case 5:
			return "Right Arm";
		case 6:
			return "Left Leg";
		case 7:
			return "Right Leg";
		default:
			return "";
		}
	}
	bool is_point_in_range(Vector2D point, Vector2D position, Vector2D size)
	{
		return (point.x >= position.x && point.x <= position.x + size.x)
			&& (point.y >= position.y && point.y <= position.y + size.y);
	}
	float GetFraction(float value, float min, float max)
	{
		float ratio = 1.f / (max - min);
		return ratio * (value - min);
	}
	inline auto get_min_sys_addr() -> void*
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return si.lpMinimumApplicationAddress;
	}
	inline auto is_bad_ptr(void* ptr) -> bool
	{
		static auto min_sys_addr = get_min_sys_addr();
		return (ptr <= min_sys_addr);
	}
	float RandomFloat(float min, float max)
	{
		static auto ranFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
		if (!is_bad_ptr(ranFloat))
			return ranFloat(min, max);
		else
			return 0.f;
	}
	float GetValueFromFraction(float fraction, float min, float max)
	{
		return ((max - min) * fraction) + min;
	}
	unsigned int GetNumberOfDigits(int i)
	{
		i = i < 0 ? -(i * 10) : i;
		return i > 0 ? (int)log10((double)i) + 1 : 1;
	}
	float RoundToDecimalPlaces(float value, int decimals)
	{
		float yeet = pow(10.f, decimals);
		return roundf(value * yeet) / yeet;
	}
	std::string FloatToString(float value, int decimals) // this shit is ugly as fOK
	{
		std::string ret = std::to_string(value);

		ret.erase(ret.begin() + clamp<unsigned int>(GetNumberOfDigits(value)
			+ decimals + 1, 0, ret.size()), ret.end());

		if (decimals <= 0)
			ret.pop_back();

		return ret;
	}
	Vector CalcAngle(Vector src, Vector dst)
	{
		Vector ret;
		MATH::VectorAngles(dst - src, ret);
		return ret;
	}
	void calculate_angle(Vector src, Vector dst, Vector &angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		angles[2] = 0.0f;
		if (delta.x >= 0.0) angles.y += 180.0f;
	}
	bool IsOnScreen(Vector origin, Vector& screen)
	{
		if (!RENDER::WorldToScreen(origin, screen)) return false;
		int iScreenWidth, iScreenHeight;
		INTERFACES::Engine->GetScreenSize(iScreenWidth, iScreenHeight);
		bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
		return xOk && yOk;
	}
	float GetCurtime()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return -1;

		return static_cast<float>(local_player->GetTickBase()) * INTERFACES::Globals->interval_per_tick;
	}
	float Lerp(float fraction, float min, float max)
	{
		return min + (fraction * (max - min));
	}
	DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
	{
		if (!dwAddress || !dwLength || !szPattern)
			return 0;

		const char* pat = szPattern;
		DWORD firstMatch = NULL;

		for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else pat += 2;
			}
			else
			{
				pat = szPattern;
				firstMatch = 0;
			}
		}

		return 0;
	}
	DWORD find_signature_new(const char* szModuleName, const char* PatternName, char* szPattern) //new
	{
		HMODULE hModule = GetModuleHandleA(szModuleName);
		PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

		DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

		return ret;
	}
	DWORD signature(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}
	DWORD FindPatternV2(std::string moduleName, std::string pattern)
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
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
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}
	uint64_t FindSignature(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;    //one ?
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}
	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}
	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		std::cout << "FAIL LMAO" << std::endl;
		return 0;
	}

	void NormalizeLemon(Vector& angle)
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

	void ClampAnglesLemon(Vector &angles)
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

	bool ClampLemon(Vector &angles)
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
}
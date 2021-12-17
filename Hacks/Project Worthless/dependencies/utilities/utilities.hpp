#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"
#define M_PI 3.14159265358979323846

using ServerRankRevealAll2 = char(__cdecl*)(int*);
namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual(void* ppClass, int index) {
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
	namespace math {
		template <typename t> t clamp_value(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
	}

	extern ServerRankRevealAll2 ServerRankRevealAllEx;
	std::uint8_t* pattern_scan(void* module, const char* signature);
	void load_named_sky(const char* sky_name);
	void apply_clan_tag(const char* name);
	extern void ServerRankRevealAll();
}

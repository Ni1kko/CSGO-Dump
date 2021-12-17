#pragma once
#include "includes.hpp"

#define square( x ) ( x * x )
namespace mikrowelle {
	inline float FastSqrt(float x)
	{
		unsigned int i = *(unsigned int*)&x;
		i += 127 << 23;
		i >>= 1;
		return *(float*)&i;
	}
}

class pwalk : public singleton<pwalk>
{
public:
	void CreateMovementwithBlackPeopleThrowingCoalIntoMyOven(CUserCmd* get_cmd);
};

#pragma once

#include "..\..\includes.hpp"
static bool memereview = false;
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low) return low;
	else if (in >= high) return high;
	else return in;
}

class fakelag : public singleton< fakelag > {
public:
	int fakelag_amountstand, fakelag_amountair, fakelag_amountshoot, fakelag_amountmove;
	bool moving, inair, standing, shooting;
	void create_move(CUserCmd *pCmd);
	int BreakLagCompensation();
};
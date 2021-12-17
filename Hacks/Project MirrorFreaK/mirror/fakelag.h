#pragma once
#include "Hacks.h"

class fakelag
{
public:

	int break_lagcomp();

	int break_lagcomp_mm();

	void Fakelag(CUserCmd * pCmd, bool & bSendPacket);

};

extern fakelag * c_fakelag;
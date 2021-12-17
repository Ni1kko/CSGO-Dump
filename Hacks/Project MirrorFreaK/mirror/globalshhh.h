#pragma once
#pragma once

class Globalsh
{
public:

	int prevChoked;
	int ChokeAmount;
	int NextBreak;
	bool fakewalk;
	bool bSendPaket;
	float OldSimulationTime[65];
};

extern Globalsh globalsh;
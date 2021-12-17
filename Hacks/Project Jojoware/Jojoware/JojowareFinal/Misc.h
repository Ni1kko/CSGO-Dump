#pragma once
#include "Memory.h"
#include "CheatManager.h"
#include "Includes.h"
class Misc
{
public:
	void Bhop();
	void RadarHack();
	void  RankReveal();
	void RenderChange();
	void NoFlash();
	void ThirdPerson();
	void Hitmarker();
private:
	struct CLRRGB
	{
		float r, g, b;
	};
	CLRRGB clr = { 255,0,0 };
	void ChangeYellowCLR()
	{
		clr.r = 255;
		clr.g = 255;
		clr.b = 0;
	}
	void ChangeGreenCLR()
	{
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
	}
	void ChangeRedCLR()
	{
		clr.r = 255;
		clr.g = 0;
		clr.b = 0;
	}
	const char* CompRanks[19] =
	{
			"Unranked",
			"Silver I",
			"Silver II",
			"Silver III",
			"Silver IV",
			"Silver Elite",
			"Silver Elite Master",
			"Gold Nova I",
			"Gold Nova II",
			"Gold Nova III",
			"Gold Nova Master",
			"Master Guardian I",
			"Master Guardian II",
			"Master Guardian Elite",
			"Distinguished Master Guardian",
			"Legendary Eagle",
			"Legendary Eagle Master",
			"Supreme Master First Class",
			"The Global Elite"
	};
}; extern Misc Mi;
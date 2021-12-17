#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>

class Resolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void OnCreateMove();
	void Yaw(C_BaseEntity * ent);
	void FrameStage(ClientFrameStage_t stage);
private:
	void AnimationFix(C_BaseEntity* pEnt);
};
extern Resolver g_Resolver;
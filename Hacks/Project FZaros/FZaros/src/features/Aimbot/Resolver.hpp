#pragma once
#include "../../utilities/includes.h"
#include "..\..\sdk\baseClient.hpp"
#include "..\..\sdk\baseEntity.hpp"
#include <deque>

class Resolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void OnCreateMove();
	void Yaw(CBaseEntity* ent);
	void FrameStage(clientFrameStage_t stage);
private:
	void AnimationFix(CBaseEntity* pEnt);
};
extern Resolver g_Resolver;
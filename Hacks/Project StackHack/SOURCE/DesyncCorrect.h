#pragma once
#include <map>
#include "includes.h"



class DesyncCorrect
{
public:
	void Resolve(SDK::CBaseEntity * pEnt);
	void AnimationFix(SDK::CBaseEntity* pEnt);
	void HandleHits(SDK::CBaseEntity* pEnt);

	float desync_delta(SDK::CBaseEntity * entity);

	int actual_yaw, opposite_yaw;
	bool CanJitter[65];
	float NormalizeYaw180(float yaw);

	void DesyncDetection();

	float Yoinks(SDK::CBaseEntity * pEnt, float yaw);

	bool lby_prediction(SDK::CBaseEntity * entity);

	void Main(SDK::CBaseEntity * pPlayerEntity);

	float resolvedYaw[65];
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];
	bool           Shot[65];
	bool           Hit[65];
	int            MissedShots[65];
};

extern DesyncCorrect* desync;
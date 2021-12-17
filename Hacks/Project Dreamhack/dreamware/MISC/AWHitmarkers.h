#pragma once

#include "../MISC/includes.h"
#include "../SDK/GameEvents.h"

struct pImpactInfo {

	float x, y, z;
	long long time;
};

struct pHitmarkerInfo {

	pImpactInfo pImpact;
	int alpha;
};

class Hitmarker : SDK::IGameEventListener {

public:
	void Initialize();
	void Paint();

private:
	void FireGameEvent(SDK::IGameEvent* pEvent) override;
	int GetEventDebugID(void) override;
	void PlayerHurt(SDK::IGameEvent* pEvent);
	void BulletImpact(SDK::IGameEvent* pEvent);
	SDK::CBaseEntity* GetPlayer(int userid);
	std::vector<pImpactInfo> pImpacts;
	std::vector<pHitmarkerInfo> pHitmarkers;
};

extern Hitmarker* pHitmarker;
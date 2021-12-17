#pragma once

#include "../SDK/GameEvents.h"

#define CREATE_EVENT_LISTENER(class_name)\
class class_name : public SDK::IGameEventListener\
{\
public:\
	~class_name() { INTERFACES::GameEventManager->RemoveListener(this); }\
\
	virtual void FireGameEvent(SDK::IGameEvent* game_event);\
};\



struct ImpactInfo {
	float x, y, z;
	long long time;
};

struct HitmarkerInfo {
	ImpactInfo impact;
	float alpha;
};
class HitmarkerStuff
{
public:
	std::vector<ImpactInfo> impacts;
	std::vector<HitmarkerInfo> hitmarkers;
};
extern HitmarkerStuff* hitmarkerstuff;

namespace CGameEvents //fuck namespaces, fuck ur style bolbi
{
	void InitializeEventListeners();
	void RemoveEventListeners();
	bool GoodestWeapon(const char * weapon);

	CREATE_EVENT_LISTENER(ItemPurchaseListener);
	CREATE_EVENT_LISTENER(PlayerHurtListener);
	CREATE_EVENT_LISTENER(BulletImpactListener);
	CREATE_EVENT_LISTENER(PlayerDeathListener);
	CREATE_EVENT_LISTENER(AntiAimDisableListener);
	CREATE_EVENT_LISTENER(RoundStartListener, "round_start");
	CREATE_EVENT_LISTENER(WeaponFiredListener);

};
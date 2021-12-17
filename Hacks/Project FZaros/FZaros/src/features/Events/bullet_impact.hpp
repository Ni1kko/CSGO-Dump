#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

struct BulletImpactInfo
{
	float m_flExpTime;
	Vector3 m_vecHitPos;
};

class bullet_impact : public IGameEventListener2, public Singleton<bullet_impact>
{
public:
	void fireGameEvent(IGameEvent *event);
	int  getEventDebugID(void);

	void registerSelf();
	void unRegisterSelf();

	void paint(void);

private:
	std::vector<BulletImpactInfo> bulletImpactInfo;
};
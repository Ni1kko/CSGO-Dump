#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

struct HitMarkerInfo
{
	float m_flExpTime;
	int m_iDmg;
};

class player_hurt : public IGameEventListener2, public Singleton<player_hurt>
{
public:
	void fireGameEvent(IGameEvent *event);
	int  getEventDebugID(void);

	void registerSelf();
	void unRegisterSelf();

	void paint(void);

private:
	std::vector<HitMarkerInfo> hitMarkerInfo;
	float _flHurtTime;

};
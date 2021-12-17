#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

class item_purchase : public IGameEventListener2, public Singleton<item_purchase>
{
public:
	void fireGameEvent(IGameEvent* event);
	int  getEventDebugID(void);

	void registerSelf();
	void unRegisterSelf();
};
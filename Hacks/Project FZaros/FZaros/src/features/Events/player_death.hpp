#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

class player_death : public IGameEventListener2, public Singleton<player_death>
{
public:
	void fireGameEvent( IGameEvent *event );
	int  getEventDebugID( void );

	void registerSelf( );
	void unRegisterSelf( );
};
#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

class weapon_fire : public IGameEventListener2, public Singleton<weapon_fire>
{
public:
	void fireGameEvent( IGameEvent *event );
	int  getEventDebugID( void );

	void registerSelf( );
	void unRegisterSelf( );
};
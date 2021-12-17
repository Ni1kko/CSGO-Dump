#pragma once

#include "..\..\utilities\includes.h"
#include "..\..\utilities\Singleton.hpp"

#include <vector>

class round_start : public IGameEventListener2, public Singleton<round_start>
{
public:
	void fireGameEvent( IGameEvent *event );
	int  getEventDebugID( void );

	void registerSelf( );
	void unRegisterSelf( );
};
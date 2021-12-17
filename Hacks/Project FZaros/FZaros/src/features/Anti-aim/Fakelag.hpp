#pragma once
#include "..\..\utilities\includes.h"


class CFakeLag
{
private:
	bool handleActivation();
	void fakeDuck( );
public:
	void createMove();
}; extern CFakeLag* g_FakeLag;
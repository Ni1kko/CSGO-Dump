#pragma once

#include "main.h"

class CSkinChanger
{
public:
	void ChangePattern( CBaseEntity* pLocal );

	void ChangeGloves( CBaseEntity * pLocal );
};

extern CSkinChanger g_SkinChanger;
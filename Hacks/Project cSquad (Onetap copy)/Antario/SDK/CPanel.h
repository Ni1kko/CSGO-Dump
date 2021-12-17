#pragma once
#include "..\Utils\Utils.h"

class CPanel
{
public:
	const char* GetName(unsigned int Panel)
	{
		return Utils::CallVFunc<36, char*>(this, Panel);
	}

	void SetMouseInputEnabled(unsigned int iPanel, bool bState)
	{
		return Utils::CallVFunc<32, void>(this, iPanel, bState);
	}
};

extern CPanel* g_pPanel;
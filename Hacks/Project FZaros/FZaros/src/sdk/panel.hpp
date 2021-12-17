#pragma once
#include "../utilities/interfaceUtilities.hpp"

class CPanel
{
public:
	const char *getPanelName(unsigned int panel)
	{
		using OriginalFn = VFUNC_SIGNATURE(const char *, unsigned int);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[36])(this, panel);
	}

};
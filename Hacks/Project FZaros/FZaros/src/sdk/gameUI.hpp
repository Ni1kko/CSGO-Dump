#pragma once
#include "../utilities/interfaceUtilities.hpp"

class CGameUI
{
public:
	void messageBox(const char *title, const char *text)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, const char *, const char *, bool, bool, const char *, const char *,
			const char *, const char *, const char *);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[20])(this, title, text, true, false, "", "", "", "", "");
	}

};
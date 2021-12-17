#include "DLL_MAIN.h"
class IVPanel
{
public:
	const char* GetName(int iIndex)
	{
		typedef const char*(__thiscall* OriginalFn)(void*, int);
		return getvfunc< OriginalFn >(this, 36)(this, iIndex);
	}
	void SetMouseInputEnabled(unsigned int panel, bool state)
	{
		return getvfunc<void(__thiscall *)(PVOID, int, bool)>(this, 32)(this, panel, state);
	}
};

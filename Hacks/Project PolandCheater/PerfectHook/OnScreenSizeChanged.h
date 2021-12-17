#pragma once
#include "HookIncludes.h"

typedef void(__thiscall *OnScreenSizeChangedFn)(ISurface *, int, int);
OnScreenSizeChangedFn oOnScreenSizeChanged;
void __stdcall OnScreenSizeChanged(int oldWidth, int oldHeight);

void __stdcall OnScreenSizeChanged(int oldWidth, int oldHeight)
{
	oOnScreenSizeChanged(g_Surface, oldWidth, oldHeight);

    g_Render->SetupFonts();
}

#pragma once
#include "..\Utils\Utils.h"

class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		return Utils::CallVFunc<14, void>(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		return Utils::CallVFunc<15, void>(this, r, g, b, a);
	}

	void UnlockCursor()
	{
		return Utils::CallVFunc<66, void>(this);
	}

	void IPlaySound(const char* fileName)
	{
		return Utils::CallVFunc<82, void>(this, fileName);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		return Utils::CallVFunc<19, void>(this, x0, y0, x1, y1);
	}
};
extern ISurface* g_pSurface;
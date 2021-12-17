#pragma once
#include "vmatrix.h"

class CViewSetup
{
	public:
		int   x;
		int   x_old;
		int   y;
		int   y_old;
		int   width;
		int   width_old;
		int   height;
		int   height_old;
		char      pad_0x0020[0x90];
		float     fov;
		float     viewmodel_fov;
		Vector    origin;
		Vector    angles;
		char      pad_0x00D0[0x7C];
};

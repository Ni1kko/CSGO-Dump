#pragma once
#include "../Include/GlobalIncludes.h"
#include "vfunc.hpp"

class CUserCmd;

class IClientMode
{
	public:
		bool CreateMove( float flInputSampleTime, CUserCmd* usercmd )
		{
			typedef bool ( __thiscall * OriginalFn )( float, CUserCmd* );

			if ( this )
				return vfunc<OriginalFn>( this, 24 )( flInputSampleTime, usercmd );
		}
};

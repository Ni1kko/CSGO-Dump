#pragma once
#include "color.h"
#include "vfunc.hpp"

class CRenderView
{
	public:
		void SetBlend( float blend )
		{

			typedef void( __thiscall * Fn )( void*, float );
			vfunc<Fn>( this, 4 )( this, blend );
		}
		float GetBlend()
		{
			typedef float( __thiscall * Fn )( void* );
			return vfunc<Fn>( this, 5 )( this );
		}
		void SetColorModulation( CColor color )
		{
			float blend[3] = { color.RGBA[0], color.RGBA[1], color.RGBA[2] };
			blend[0] /= 255;
			blend[1] /= 255;
			blend[2] /= 255;
			typedef void( __thiscall * Fn )( void*, float* );
			vfunc<Fn>( this, 6 )( this, blend );
		}
		void SetColorModulation( float const* colors )
		{
			typedef void( __thiscall * Fn )( PVOID, float const* );
			return vfunc< Fn >( this, 6 )( this, colors );
		}
		void GetColorModulation( float* blend )
		{
			typedef void( __thiscall * Fn )( void*, float* );
			vfunc<Fn>( this, 7 )( this, blend );
		}
};

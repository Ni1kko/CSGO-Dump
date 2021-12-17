#pragma once
#include "../materialSystem.hpp"

class CStudioRender
{
public:
	void ForceMaterialOverride( IMaterial* material, int type = 0, int b0 = 0 ) {
		using original_fn = void( __thiscall* )( CStudioRender*, IMaterial*, int, int );
		return ( *( original_fn** )this )[ 33 ]( this, material, type, b0 );
	}
};
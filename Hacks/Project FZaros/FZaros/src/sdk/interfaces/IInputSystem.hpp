#pragma once
#include "../../utilities/interfaceUtilities.hpp"

class IInputSystem
{
public:
	void enableInput( bool bEnable )
	{
		using OriginalFn = VFUNC_SIGNATURE( void, bool );
		return ( ( OriginalFn )g_InterfaceUtils->getVfuncTable( this )[ 11 ] )( this, bEnable );
	}
};
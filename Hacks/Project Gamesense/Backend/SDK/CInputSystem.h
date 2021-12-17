#pragma once
#include "vfunc.hpp"

class CInputSystem
{
	public:
		void EnableInput( bool enabled )
		{
			typedef void( __thiscall * Fn )( void*, bool );

			if ( this )
				return ( ( Fn )vfunc<Fn>( this, 11 ) )( this, enabled );
		}
};

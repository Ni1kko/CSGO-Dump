#pragma once
class Vector;

class Collideable
{
	public:
		Vector& VecMins()
		{
			if ( this )
				return *reinterpret_cast<Vector*>( ( DWORD )this + 0x8 );
		}
		Vector& VecMaxs()
		{
			if ( this )
				return *reinterpret_cast<Vector*>( ( DWORD )this + 0x14 );
		}
};

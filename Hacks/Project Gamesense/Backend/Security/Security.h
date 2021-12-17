#pragma once
#include "../Include/GlobalIncludes.h"

namespace Cheat
{
	class CSecurity
	{
		public:
			void ErasePEHeader( HANDLE hModule );
			void UnlinkModule( const char* szModuleName );
	};

	extern CSecurity* Security;
}

#include "SDK.h"

namespace SDK
{
	bool KeyValues::LoadFromBuffer( KeyValues *pThis , const char *pszFirst , const char *pszSecond , PVOID pSomething , PVOID pAnother , PVOID pLast )
	{
		typedef bool( __thiscall *_LoadFromBuffer )( KeyValues* , const char* , const char* , PVOID , PVOID , PVOID );

		static _LoadFromBuffer LoadFromBufferFn = 0;
		static bool SearchFunction = false;

		if ( !SearchFunction )
		{
			DWORD dwFunctionAddress = CSX::Memory::FindPattern( CLIENT_DLL , "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" , 0 );

			if ( dwFunctionAddress )
			{
				LoadFromBufferFn = (_LoadFromBuffer)dwFunctionAddress;
				SearchFunction = true;
			}
		}

		if ( LoadFromBufferFn && SearchFunction )
		{
			return LoadFromBufferFn( pThis , pszFirst , pszSecond , pSomething , pAnother , pLast );
		}

		return false;
	}
}











































































































































































#pragma once

#include "main.h"

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* ( *CreateInterfaceFn )( const char *pName, int *pReturnCode );

class CInterfaces
{
public:
	HMODULE GetModuleHandleSafe( const char* pszModuleName );
	void* GetInterfaces( char* szBaseName, const char* interfaceName, const char* ptrName, CreateInterface_t pInterface );
	void InitInterfaces();
};

extern CInterfaces g_pInterfaces;
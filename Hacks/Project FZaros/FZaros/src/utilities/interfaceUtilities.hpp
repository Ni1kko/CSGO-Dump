#pragma once
#include <string>

#define VFUNC_SIGNATURE(type, ...) type(__thiscall *)(void *, __VA_ARGS__)

class CInterfaceUtils
{
public:
	void *grabModule(const std::string &moduleName);
	void *grabInterface(const std::string &moduleName, const std::string &interfaceName);
	void *bruteforceInterface( void *modulePtr, const std::string &interfaceName );
	void *bruteforceInterface( const char * mod, const char * iface );

	uintptr_t *getVfuncTable(void *interfacePtr);

	template<unsigned int IIdx, typename TRet, typename ... TArgs>
    static auto getVFunc(void* thisptr, TArgs ... argList) -> TRet
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

private:
	void *grabModuleTimeout(const std::string &moduleName, int timeout = 20000);
	void *grabInterface2(void *modulePtr, const std::string &interfaceName);

};

extern CInterfaceUtils *g_InterfaceUtils;
#include <cctype>
#include <chrono>
#include <windows.h>

#include "interfaceUtilities.hpp"

using CreateInterfaceFn = void* (*)(const char*, int*);

void* CInterfaceUtils::grabModuleTimeout(const std::string &moduleName, int timeout)
{
	using namespace std::chrono;

	void* result = nullptr;
	auto beginTime = system_clock::now();

	while (duration_cast<seconds>(system_clock::now() - beginTime).count() <= timeout)
	{
		result = GetModuleHandleA(moduleName.c_str());

		if (result != nullptr)
			break;
	}

	return result;
}

void* CInterfaceUtils::bruteforceInterface(void* modulePtr, const std::string &interfaceName)
{
	void* result = nullptr;
	auto createInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)modulePtr, "CreateInterface");

	if (createInterface == nullptr)
		return nullptr;

	for (int i = 0; i < 1000; i++)
	{
		char nameBuffer[128];
		sprintf_s(nameBuffer, "%s%03i", interfaceName.c_str(), i);

		result = createInterface(nameBuffer, nullptr);

		if (result != nullptr)
			break;
	}

	return nullptr;
}

void *CInterfaceUtils::bruteforceInterface( const char *mod, const char *iface ) {
	// LoL
	auto CaptureInterface = [ ] ( const char* mod, const char* iface, bool print_if_nullptr ) -> void*
	{
		using fn_capture_iface_t = void*( *)( const char*, int* );
		auto fn_addr = ( fn_capture_iface_t )GetProcAddress( GetModuleHandleA( mod ), "CreateInterface" );

		auto iface_addr = fn_addr( iface, nullptr );

		if ( iface_addr != nullptr || print_if_nullptr )
			printf( "found %s at 0x%p\n", iface, fn_addr( iface, nullptr ) );

		return iface_addr;
	};
		
	for ( int i = 0; i < 1000; i++ ) {
		char buffer[ 128 ];
		sprintf_s( buffer, "%s%03i", iface, i );
		void *result = CaptureInterface( mod, buffer, i == 999 );
		if ( result != nullptr )
			return result;
	}
	return nullptr;
}


void* CInterfaceUtils::grabInterface2(void* modulePtr, const std::string & interfaceName)
{
	if (std::isdigit(interfaceName[interfaceName.length() - 1]) &&
		std::isdigit(interfaceName[interfaceName.length() - 2]) &&
		std::isdigit(interfaceName[interfaceName.length() - 3]))
	{
		auto createInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)modulePtr, "CreateInterface");

		if (createInterface == nullptr)
			return nullptr;

		return createInterface(interfaceName.c_str(), nullptr);
	}
	else
	{
		return this->bruteforceInterface(modulePtr, interfaceName);
	}
}

void* CInterfaceUtils::grabModule(const std::string &moduleName)
{
	return this->grabModuleTimeout(std::move(moduleName));
}

void* CInterfaceUtils::grabInterface(const std::string &moduleName, const std::string &interfaceName)
{
	return this->grabInterface2(this->grabModule(std::move(moduleName)), std::move(interfaceName));
}

uintptr_t* CInterfaceUtils::getVfuncTable(void *interfacePtr)
{
	return *reinterpret_cast<uintptr_t**>(interfacePtr);
}

CInterfaceUtils* g_InterfaceUtils = new CInterfaceUtils();
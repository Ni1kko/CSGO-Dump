#pragma once
#include <Windows.h>

#include <tuple>
#include <unordered_map>

#define BLACKBONE_API

namespace blackbone
{

	enum call_order
	{
		HookFirst,      // Hook called before original function
		HookLast,       // Hook called after original function
		NoOriginal,     // Original function doesn't get called
	};

	enum return_method
	{
		UseNew,         // Return value returned by hook
		UseOriginal     // Return original function value
	};

class DetourBase
{
    using mapIdx = std::unordered_map<DWORD, int>;

public:
    BLACKBONE_API DetourBase();
    BLACKBONE_API ~DetourBase();

protected:
    BLACKBONE_API bool AllocateBuffer( uint8_t* nearest );
    BLACKBONE_API bool DisableHook();
    BLACKBONE_API bool EnableHook();

    BLACKBONE_API static LONG NTAPI VectoredHandler ( PEXCEPTION_POINTERS excpt );
    BLACKBONE_API static LONG NTAPI Int3Handler     ( PEXCEPTION_POINTERS excpt );
    BLACKBONE_API static LONG NTAPI AVHandler       ( PEXCEPTION_POINTERS excpt );
    BLACKBONE_API static LONG NTAPI StepHandler     ( PEXCEPTION_POINTERS excpt );

protected:
    bool _hooked = false;               // Hook is installed

    void* _callback = nullptr;          // User supplied hook function
    void* _callbackClass = nullptr;     // Class pointer for user hook
    void* _original = nullptr;          // Original function address
    void* _internalHandler = nullptr;   // Pointer to hook handler
    void* _callOriginal = nullptr;      // Pointer to original function 
    
    mapIdx   _hwbpIdx;                  // Thread HWBP index
    size_t   _origSize = 0;             // Original code size
    uint8_t* _buf = nullptr;            // Trampoline buffer
    uint8_t* _origCode = nullptr;       // Original function bytes
    uint8_t* _origThunk = nullptr;      // Original bytes adjusted for relocation
    uint8_t* _newCode = nullptr;        // Trampoline bytes
    
    call_order _order = call_order::HookFirst;
    return_method _retType = return_method::UseOriginal;

    // Global hook instances relationship
    BLACKBONE_API static std::unordered_map<void*, DetourBase*> _breakpoints;

    // Exception handler
    BLACKBONE_API static void* _vecHandler;
};

}
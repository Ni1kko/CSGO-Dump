#pragma once

#include "HookHandlers.h"

namespace blackbone
{

template<typename Fn, class C = NoClass>
class Detour: public HookHandler<Fn, C>
{
public:
    using type    = typename HookHandler<Fn, C>::type;
    using hktype  = typename HookHandler<Fn, C>::hktype;
    using hktypeC = typename HookHandler<Fn, C>::hktypeC;

public:  
    Detour()
    {
        this->_internalHandler = &HookHandler<Fn, C>::Handler;
    }

    ~Detour()
    {
        Restore();
    }

    bool Hook(type ptr, hktype hkPtr, call_order order = call_order::HookFirst, return_method retType = return_method::UseOriginal)
    { 
        if (this->_hooked)
            return false;

        this->_order = order;
        this->_retType = retType;
        this->_callOriginal = this->_original = ptr;
        this->_callback = hkPtr;

        if (!DetourBase::AllocateBuffer( reinterpret_cast<uint8_t*>(ptr) ))
            return false;

		return HookInt3();
    }

    bool Hook(type Ptr, hktypeC hkPtr, C* pClass, call_order order = call_order::HookFirst, return_method retType = return_method::UseOriginal)
    {
        this->_callbackClass = pClass;

        return Hook( Ptr, brutal_cast<hktype>(hkPtr), type, order, retType );
    }

    bool Restore()
    {
        if (!this->_hooked)
            return false;
        
		DWORD flOld = 0;
		if (!VirtualProtect(this->_original, this->_origSize, PAGE_EXECUTE_READWRITE, &flOld))
			return false;

		memcpy(this->_original, this->_origCode, this->_origSize);
		VirtualProtect(this->_original, this->_origSize, flOld, &flOld);

        this->_hooked = false;
        return true;
    }

private:
    bool HookInt3()
    {
        this->_newCode[0] = 0xCC;
        this->_origSize = sizeof( this->_newCode[0] );

        // Setup handler
        if (this->_vecHandler == nullptr)
            this->_vecHandler = AddVectoredExceptionHandler( 1, &DetourBase::VectoredHandler );

        if (!this->_vecHandler)
            return false;

        this->_breakpoints.insert( std::make_pair( this->_original, (DetourBase*)this ) );

        // Save original code
        memcpy( this->_origCode, this->_original, this->_origSize );

        // Write break instruction
        DWORD flOld = 0;
        if (!VirtualProtect(this->_original, this->_origSize, PAGE_EXECUTE_READWRITE, &flOld))
			return false;
        memcpy( this->_original, this->_newCode, this->_origSize );
        VirtualProtect( this->_original, this->_origSize, flOld, &flOld );

        return this->_hooked = TRUE;
    }
};
}

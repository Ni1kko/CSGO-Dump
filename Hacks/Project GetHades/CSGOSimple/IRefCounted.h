#pragma once
#include "valve_sdk/sdk.hpp"
class IRefCounted
{
private:
    volatile long ref_count_{};

public:
    virtual ~IRefCounted() = default;
    virtual void destructor ( char bDelete ) = 0;
    virtual bool on_final_release() = 0;

    void dereference()
    {
        if ( InterlockedDecrement ( &ref_count_ ) == 0 && on_final_release() )
            destructor ( 1 );
    }
};
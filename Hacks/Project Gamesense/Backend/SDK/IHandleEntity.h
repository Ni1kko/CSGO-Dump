#pragma once
class CBaseHandle;

class IHandleEntity
{
public:
    virtual            ~IHandleEntity() {}
    virtual void    set_ref_ehandle(const CBaseHandle& handle) = 0;
    virtual const    CBaseHandle& get_ref_ehandle() const = 0;
};

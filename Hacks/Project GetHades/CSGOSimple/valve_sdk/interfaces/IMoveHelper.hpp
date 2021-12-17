#pragma once

class IClientEntity;

class IMoveHelper
{
public:
    virtual void _vpad ( ) = 0;
    virtual void set_host ( IClientEntity* host ) = 0;
};

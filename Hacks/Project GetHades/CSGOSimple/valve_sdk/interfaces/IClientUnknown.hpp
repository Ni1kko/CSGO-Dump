#pragma once

#include "../misc/IHandleEntity.hpp"
#include "ICollideable.hpp"

//class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class IClientUnknown : public IHandleEntity
{
public:
    virtual ICollideable* get_collideable ( ) = 0;
    virtual IClientNetworkable* get_client_networkable ( ) = 0;
    virtual IClientRenderable* get_client_renderable ( ) = 0;
    virtual IClientEntity* get_iclient_entity ( ) = 0;
    virtual C_BaseEntity* get_base_entity ( ) = 0;
    virtual IClientThinkable* get_client_thinkable ( ) = 0;
    //virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
    virtual IClientAlphaProperty* get_client_alpha_property ( ) = 0;
};

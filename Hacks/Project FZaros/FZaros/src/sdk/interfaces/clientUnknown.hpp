#pragma once
#include "clientThinkable.hpp"
#include "clientNetworkable.hpp"
#include "../vector.hpp"
#include "clientRenderable.hpp"
#include "../baseHandle.hpp"


class IClientAlphaProperty;
class CBaseEntity;
class IClientEntity;

class ICollideable
{
public:
	virtual void pad0();
	virtual Vector3&				OBBMins() const = 0;
	virtual Vector3&				OBBMaxs() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable*              GetCollideable() = 0;
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual IClientEntity*             GetIClientEntity() = 0;
	virtual CBaseEntity*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};
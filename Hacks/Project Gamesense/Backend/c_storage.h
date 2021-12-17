#pragma once
#include "singleton.h"

#include "SDK/CBaseEntity.h"

class c_storage : public singleton<c_storage> {
public:
	float fl_fraction = 0.f;
};
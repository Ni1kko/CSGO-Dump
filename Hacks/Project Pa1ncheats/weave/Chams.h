#pragma once
#include "Hooks.h"

class CChams
{
public:
	void Draw(void* ecx, void* results, const DrawModelInfo_t& info,
		matrix* bone_to_world, float* flex_weights, float* flex_delayed_weights, const Vector& model_origin, int flags);
};

extern CChams* Chams;
#pragma once
// all includes
#include "material_helper.h"

class c_helpers
{
public:
    c_material_helper material_helper;
    static void trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr );
};

extern c_helpers g_helpers;

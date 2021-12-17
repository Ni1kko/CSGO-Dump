#pragma once

#include "../../valve_sdk/csgostructs.hpp"

class c_movement
{
public:
    void fix_movement ( usercmd_t* cmd, qangle_t angles ) const;
};

#pragma once
#include "../valve_sdk/csgostructs.hpp"

namespace rebuild
{

    bool setup_bones( C_BasePlayer* player, void* bones_to_world, int bone_mask, int max_bones );
};

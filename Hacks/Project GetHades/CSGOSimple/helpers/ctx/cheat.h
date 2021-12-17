#pragma once
namespace ctx
{
    struct cheat_t
    {
        bool should_update_animations = false;
        vector_t unpredicted_velocity;
    };

    extern cheat_t cheat;
}

#pragma once

class C_BasePlayer;
class usercmd_t;

namespace ctx
{
    struct client_t
    {
        C_BasePlayer* local = nullptr;
        usercmd_t* cmd = nullptr;
        bool should_choke = false;
        bool should_setup_local_bones = false;
    };

    extern client_t client;
}

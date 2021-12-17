#pragma once
#include "../../../valve_sdk/csgostructs.hpp"

enum class fakelag_modes : int
{
    factor,
    adaptiv,
    peak
};

class c_fakelag
{
public:
    void on_create_move ( usercmd_t* cmd, bool& send_packet );
    static int get_ticks_to_choke ( usercmd_t* cmd );
};

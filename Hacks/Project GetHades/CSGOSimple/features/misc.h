#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include "misc/movement.h"

class c_misc
{
public:
    static void bhop ( usercmd_t* cmd );
    static void auto_strafe ( usercmd_t* cmd, QAngle vangles );
    static void nightmode ( );
    static void disable_nightmode ( );
    static void radar ( );
    void clantag_changer ( );
    static void airduck ( usercmd_t* cmd );

    c_movement movement;
};

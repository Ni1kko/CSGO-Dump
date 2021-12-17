#pragma once
#include "../valve_sdk/csgostructs.hpp"

// include all features
#include "misc/misc.hpp"
#include "ragebot/ragebot.hpp"
#include "chams.h"
#include "debug_overlay.h"
#include "keybind_system.h"
#include "misc.h"
#include "visuals.hpp"
#include "legitbot/legitbot.h"
#include "visuals/visuals.hpp"

class c_features
{
public:
    c_features ( )
    {
        /* this is only here so compile tells us errors in the files and not here */
    };
    c_anti_aim anti_aim;
    c_autowall autowall;
    c_chams chams;
    c_debug_overlay debug_overlay;
    c_engine_prediction engine_prediction{ };
    c_event_logger event_logger;
    c_fakelag fakelag;
    c_keybind_system keybind_system;
    c_misc misc;
    //c_movement_fix movement_fix;
    c_rage_backtrack rage_backtrack;
    c_ragebot ragebot;
    c_resolver resolver;
    c_visuals visuals;
    c_legitbot legitbot;
    c_animations animations;
};

extern c_features g_features;

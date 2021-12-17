#pragma once
// ReSharper disable CppUnusedIncludeDirective
#include "../valve_sdk/math/Vector.hpp"
// ReSharper restore CppUnusedIncludeDirective
#include "../valve_sdk/math/QAngle.hpp"

struct hitmarker_data
{
    float time = 0.f;
    float dmg = 0.f;
    bool did_kill = false;
};

class matrix3x4_t;

class c_globals
{
public:
    /* create_move pre-save */
    float old_forwardmove = 0;
    float old_sidemove = 0;
    //QAngle old_viewangles;

    float curtime = 0.f;

    /* in functions */
    bool in_slowwalk = false;
    //bool in_lby_break = false;
    bool rbot_force_slowwalk = false;

    bool set_curtime_after_prediction = false;

    float fake_yaw_ = 0.f;
    float real_yaw = 0.f;
    float real_pitch = 0.f;

    hitmarker_data hitmarker;

    float max_desync_delta = 0.f;

    bool in_fakeduck = false;
    bool fackeduck_send_packet_overwrite = true;

    bool breaking_lc = false;

    bool overwrite_setupbones_matrix = false;
    int overwrite_setupbones_index = -1;

    int prev_supressed_tick = 0;
    //float prev_real_yaw = 0.f;
    //float prev_real_pitch = 0.f;
    bool last_pre_supress2 = false;

    QAngle old_viewpunch = QAngle ( 0, 0, 0 );
    QAngle old_aimpunch = QAngle ( 0, 0, 0 );

    bool lby_broken = false;
    //float fake_yaw = 0.f;

    bool in_lby_break = false;

    QAngle fsn_real_ang{ };

    bool anim_did_update = false;

    //QAngle backup_abs_ang[64] = {};
    float current_real_rotation_rendered = 0.f;
};

extern c_globals globals;

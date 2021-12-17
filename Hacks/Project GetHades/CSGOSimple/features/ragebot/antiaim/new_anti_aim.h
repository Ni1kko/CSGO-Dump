#pragma once
#include "../../../valve_sdk/csgostructs.hpp"

enum class yaw_antiaim_t : int
{
    none,
    at_target,
    spinbot,
    random,
    freestand
};

enum class pitch_antiaim_t : int
{
    none,
    up,
    zero,
    down
    // add fake pitches, etc. here
};

enum class desync_antiaim_t : int
{
    none,
    directional_static,
    static_fake,
    jitter
};

enum class direction_t : int
{
    mid,
    left,
    right
};

class new_anti_aim
{
public:
    void on_create_move ( );

private: // antiaims
    void run_pitch_antiaim ( );
    void run_yaw_antiaim ( );
    void run_desync_antiaim ( );

private: // pitch
    void pitch_down();
    void pitch_up();
    void pitch_zero();

private: // yaw
    void yaw_at_target();
    void yaw_spinbot();
    void yaw_random();
    void yaw_freestand();

private: // desync
    void desync_static();
    void desync_switch_real();
    void desync_jitter();

private:
    direction_t get_direction_for_desync();
    C_BasePlayer* get_low_fov_player();

private:
    void run_lby_predication ( );

    float m_next_lby_update = -1;

private:
    bool should_skip_antiaim ( );
};

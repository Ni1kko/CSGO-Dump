#pragma once

#include "../../valve_sdk/csgostructs.hpp"

struct player_animation_t
{
    float m_last_simtime = 0.f;
    float m_spawntime = 0.f;
    bool m_init = false;
    std::array< animation_layer_t, 13 > m_layers{ };
    std::array< float, 20 > m_poses{ };
    CCSGOPlayerAnimState* m_state = nullptr;
    bool m_got_matrix = false;
    matrix3x4_t m_matrix[128] {};
    bool m_should_recalculate_matrix = false;
};

class c_animations
{
public:
    void on_create_move ( bool send_packet );

private:
    void fake_animation ( );
    void real_animation ( const bool send_packet );

public:
    void player_animations ( );

public: // fake state
    bool m_should_update_fake = false;
    std::array< animation_layer_t, 13 > m_fake_layers;
    std::array< float, 20 > m_fake_poses;
    CCSGOPlayerAnimState* m_fake_state = nullptr;
    float m_fake_rotation = 0.f;
    bool init_fake_anim = false;
    float m_fake_spawntime = 0.f;
    float m_fake_delta = 0.f;
    matrix3x4_t m_fake_matrix[128];
    matrix3x4_t m_fake_position_matrix[128];
    //std::array< matrix3x4_t, 128 > m_fake_matrix;
    //std::array< matrix3x4_t, 128 > m_fake_position_matrix;
    bool m_got_fake_matrix = false;
    float m_real_yaw_ang = 0.f;

public: // real state
    bool m_should_update_real = false;
    float m_real_spawntime = 0.f;
    bool init_real_anim = false;
    std::array< animation_layer_t, 13 > m_real_layers;
    std::array< float, 20 > m_real_poses;
    CCSGOPlayerAnimState* m_real_state = nullptr;
    bool m_got_real_matrix = false;
    matrix3x4_t m_real_matrix[128];
    float m_server_abs_rotation = 0.f;

public:
    //float m_last_fake_yaw = 0.f;
    //float last_pre_yaw = 0.f;
    //float m_real_yaw_jitter = 0.f;
    qangle_t m_current_real_angle = qangle_t ( 0.f, 0.f, 0.f );
    qangle_t m_current_fake_angle = qangle_t ( 0.f, 0.f, 0.f );

public:
    bool m_should_update_entity_animstate = true;

    std::array< player_animation_t, 64 > m_player_states;
};

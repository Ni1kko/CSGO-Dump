#pragma once
#include "../../../valve_sdk/csgostructs.hpp"
#include <map>

class c_anti_aim
{
public:
    /**
     * \brief Sets all antiaims up.
     */
    void setup ( );

    /**
     * \brief Predicts lby update.
     */
    void run_lby_prediction ( );

    /**
     * \brief Runs the user selected antiaim.
     * \param cmd current command
     * \param send_packet current send-packet
     */
    void run_antiaim ( usercmd_t* cmd, bool& send_packet );

    /* other functions */
    void slowwalk ( usercmd_t* cmd );
    void fake_crouch ( usercmd_t* cmd, bool& send_packet );
    /* should be run inside engine prediction, breaks lby for good usage of desync */
    void lby_breaker ( usercmd_t* cmd, bool& send_packet );

    void finish_antiaim ( usercmd_t* cmd, bool& send_packet );

    //QAngle thirdperson_angle = QAngle ( 0, 0, 0 );
    [[nodiscard]] bool did_shot_this_fakelag ( ) const;

    float m_last_fake_yaw = 0.f;

    float last_pre_yaw = 0.f;

    float m_real_yaw_jitter = 0.f;

    //qangle_t m_current_real_angle = qangle_t ( 0.f, 0.f, 0.f );
    //qangle_t m_current_fake_angle = qangle_t ( 0.f, 0.f, 0.f );

    void register_real_base_antiaim ( const std::string& name, const std::function< void  ( std::string, usercmd_t*, bool ) >& callback );
    void register_real_add_antiaim ( const std::string& name, const std::function< void  ( std::string, usercmd_t*, bool ) >& callback );
    void register_desync_antiaim ( const std::string& name, std::function< void  ( std::string, usercmd_t*, bool&, float ) > callback );
    void register_pitch_antiaim ( const std::string& name, std::function< void  ( std::string, usercmd_t*, bool ) > callback );

    // vectors for faster access from menu
    std::vector< std::string > real_base_antiaim_names;
    std::vector< std::string > real_add_antiaim_names;
    std::vector< std::string > desync_antiaim_names;
    std::vector< std::string > pitch_antiaim_names;
    std::unordered_map< std::string, std::function< void  ( std::string name, usercmd_t* cmd, bool& send_packet ) > > real_base_antiaims;
    std::unordered_map< std::string, std::function< void  ( std::string name, usercmd_t* cmd, bool& send_packet ) > > real_add_antiaims;
    std::unordered_map< std::string, std::function< void  ( std::string name, usercmd_t* cmd, bool& send_packet, float max_rotation ) > > desync_antiaims;
    std::unordered_map< std::string, std::function< void  ( std::string name, usercmd_t* cmd, bool& send_packet ) > > pitch_antiaims;
private:
    float m_next_lby_update_time = 0.f;
    float m_overwrite_real = 0.f;
    bool m_should_overwrite_real = false;

    struct
    {
        QAngle last_ang{ };
        bool did_shot_last_tick = false;
    } saved_data;

    int get_automatic_desync_dir ( );

    //usercmd_t* current_cmd = nullptr;
    //bool current_send_packet = false;

    /* yaw methods */
    static bool manual_aa ( usercmd_t* cmd );
    //float yaw_freestanding ( usercmd_t* cmd ) const;

    bool did_shot_in_this_fakelag_ = false;
    QAngle did_shot_in_this_fakelag_angle_ = QAngle ( 0, 0, 0 );

    bool last_tick_in_shot_ = false;
    float last_tick_yaw_ = 0.f;

    /* misc functions */

    /* misc data */
    QAngle last_angle_ = QAngle ( 0, 0, 0 );

    bool desync_rotate ( float rotation, int direction, usercmd_t* cmd, bool& send_packet );

    int m_rotate = 0;
    int m_rotate_iteration = 0;

    float m_goal_feet_yaw = 0.f;
    float m_current_feet_yaw = 0.0f;
    float m_previous_feet_yaw = 0.0f;
};

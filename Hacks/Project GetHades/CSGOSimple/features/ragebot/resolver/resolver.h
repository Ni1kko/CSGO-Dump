#pragma once
#include "../../../valve_sdk/csgostructs.hpp"

enum class resolver_flags_t
{
    none = 0 << 0,
    desyncing = 1 << 0,
    tick_shifting = 1 << 1,
    in_air = 1 << 2,
    slow_walking = 1 << 3,
    fast_speed = 1 << 4,
    in_fakelag = 1 << 5
};

struct resolverdata_t
{
public:
    bool update ( C_BasePlayer* player );

public:
    resolver_flags_t resolver_flags = resolver_flags_t::none;
    float m_goal_feet_yaw = 0.f;

private:
    bool m_init = false;
private:
    float m_last_simtime = -0.f;
};

class c_resolver
{
public:
    enum class resolver_angle_overwrites_t
    {
        none,
        plus,
        minus,
        max
    };

    enum class resolver_modes_t
    {
        legit,
        damage,
        brutforce,
        max
    };

    struct resolver_last_hit_head_t
    {
        bool did_hit = false;
        resolver_angle_overwrites_t addang = resolver_angle_overwrites_t::none;
    };

    float server_goal_feet_yaw ( C_BasePlayer* entity );

    class resolver_data_t
    {
    public:
        bool get_979_playing_state ( ) const
        {
            return interfaces::global_vars->curtime - this->times.last_979 < 2.f;
        }

        bool setup_player ( C_BasePlayer* player )
        {
            if ( !player || !player->is_alive( ) || !player->is_enemy( ) )
                return false;

            if ( player->is_dormant( ) )
            {
                this->times.last_dormant = interfaces::global_vars->curtime;
                this->was_last_dormant = true;
                return false;
            }

            this->player = player;

            //auto current_length = 0.f;

            //if ( std::round ( this->last_angle.pitch ) ==  std::round ( player->eye_angles(  ).pitch ) )
            //{
            //    current_length = (g_local->)
            //}

            if ( this->is_979_playing( ) )
                this->times.last_979 = player->simulation_time( );

            if ( player->simulation_time( ) == this->times.last_simtime )
                this->times.last_choke = player->simulation_time( );

            this->times.last_simtime = player->simulation_time( );

            this->in_shot = this->is_playing_shot_anim( ) && player->eye_angles( ).pitch > - 60.f;

            this->desyncing = !was_last_dormant && this->is_desyncing( );

            was_last_dormant = false;

            this->server_send_ang = player->eye_angles( );

            this->original_angle = player->eye_angles( );

            return true;
        }

        void count_shot ( const c_resolver::resolver_data_t rdata )
        {
            ++this->overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ];
            ++this->shots;
            ++this->overall_overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ];
            ++this->overall_shots;
        }

        void remove_shot ( const c_resolver::resolver_data_t rdata )
        {
            --this->shots;
            --this->overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ];
            --this->overall_overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ];
            --this->overall_shots;

            if ( this->shots < 0 )
                this->shots = 0;

            if ( this->overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ] < 0 )
                this->overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ] = 0;

            if ( this->overall_overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ] < 0 )
                this->overall_overwrites_shots[ static_cast< int > ( rdata.current_overwrite ) ] = 0;

            if ( this->overall_shots < 0 )
                this->overall_shots = 0;
        }

        void reset_shots ( )
        {
            this->shots = 0;

            for ( auto& iter : this->overwrites_shots )
                iter = 0;
        }

        resolver_angle_overwrites_t get_brutforce_overwrite ( );

        resolver_modes_t get_best_resolver_mode ( ) const;

        void set_hit_head ( )
        {
            last_hit_head.addang = this->current_overwrite;
            last_hit_head.did_hit = true;
        }

        void set_did_not_hit ( )
        {
            last_hit_head.did_hit = false;
        }

        qangle_t server_send_ang = { 0, 0, 0 };
        float goal_feet_yaw = 0.f;
        bool should_resolve = false;

        struct
        {
            resolver_angle_overwrites_t overwrite = resolver_angle_overwrites_t::max;
            float simtime = 0.f;
        } last_damage_data;

        struct
        {
            resolver_angle_overwrites_t last_direction = resolver_angle_overwrites_t::max;
            float last_simtime = 0.f;
        } last_can_hit_head_points;

        bool is_resolving = false;

        float last_abs_yaw = 0.f;
        float resolver_abs_yaw = 0.f;

        bool resolved = true;
        bool desyncing = false;
        bool tick_shifting = false;
        qangle_t original_angle = { };
        int shots = 0;
        int overwrites_shots[static_cast< int > ( resolver_angle_overwrites_t::max )] = { };
        int overall_shots = 0;
        int overall_overwrites_shots[static_cast< int > ( resolver_angle_overwrites_t::max )] = { };
        resolver_angle_overwrites_t current_overwrite = resolver_angle_overwrites_t::none;
        bool in_shot = false;
        float desync_add = 0.f;
        resolver_last_hit_head_t last_hit_head;

        qangle_t last_angle = qangle_t ( 0.f, 0.f, 0.f );
        qangle_t last_resolved_angle = qangle_t ( 0.f, 0.f, 0.f );

        float last_update_simtime = 0.f;
        int last_tickcount = -1;

        float detection_last_simtime = -1.f;
        float detection_pre_last_simtime = -1.f;
    private:
        bool was_last_dormant = false;
        bool is_playing_shot_anim ( );

        C_BasePlayer* player = nullptr;

        bool is_979_playing ( );
        bool is_desyncing ( );

        animation_layer_t previous_layer = { };

        struct
        {
            float last_dormant = 0.f;
            float last_979 = 0.f;
            float last_choke = 0.f;
            float last_simtime = 0.f;
        } times;
    };

    void on_create_move ( );
    void on_frame_stage_notify ( );

    bool get_is_desyncing ( int idx );
    bool get_is_tick_shifting ( int idx );

    void reset_resolver_data ( );
    void reset_player_resolver_data ( int idx );
    void count_player_shot ( int idx, const c_resolver::resolver_data_t rdata );
    void remove_player_shot ( int idx, const c_resolver::resolver_data_t rdata );

    std::array< resolver_data_t, 64 > rdata;

private:
    void resolve_yaw ( C_BasePlayer* player );
    void resolve_pitch ( C_BasePlayer* player );

    float get_resolved_yaw_by_damage ( C_BasePlayer* player );
    float get_resolved_yaw_by_brutforce ( C_BasePlayer* player ) const;
    float get_resolved_yaw_by_anti_freestand ( C_BasePlayer* player );
};

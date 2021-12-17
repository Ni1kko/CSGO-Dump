//#pragma once
//#include "../valve_sdk/csgostructs.hpp"
//#include "rage_backtrack.h"
//
//#include <deque>
//
//struct log_data
//{
//    std::string msg = "";
//    float time = 0.f;
//};
//
//struct event_data
//{
//    Vector pos = Vector ( 0, 0, 0 );
//    bool got_pos = false;
//    bool got_hurt = false;
//    float time = 0.f;
//    int hurt_player = -1;
//    int userid = -1;
//    bool hurt = false;
//    int damage = 0;
//    bool died = false;
//    int hitbox = hitbox_max;
//};
//
//struct player_event_logger_time
//{
//    int index = -1;
//    float shot_time = 0.f;
//    Vector eyepos = {0, 0, 0};
//    QAngle aimang = {0, 0, 0};
//    resolver_data_struct_t rdata;
//    player_backup_data bdata;
//    bool did_bullet_impact = false;
//    bool did_player_hurt = false;
//    bool did_weapon_fire = false;
//};
//
//class c_event_logger
//{
//public:
//    void on_fire_event ( IGameEvent* event );
//    void on_create_move();
//    void on_draw();
//
//    void set_rbot_data ( C_BasePlayer* player, int index, QAngle angle );
//    void set_rbot_data_backtrack ( C_BasePlayer* player, int index, QAngle angle, backtrack_all_data data );
//private:
//    std::vector<player_event_logger_time> rbot_shot_data;
//    //int last_rbot_entity = -1;
//    //float last_rbot_shot_time = 0.f;
//    //player_backup_data last_shot_pdata;
//    //Vector last_rbot_shot_eyepos = Vector ( 0, 0, 0 );
//    //QAngle last_rbot_shot_angle = QAngle ( 0, 0, 0 );
//    //resolver_data_struct_t last_rbot_resolver_data;
//
//    enum class miss_type : int
//    {
//        none,
//        spread,
//        resolver,
//        custom
//    };
//
//    std::deque<event_data> last_events;
//    std::deque<log_data> logs;
//    void log_damage ( C_BasePlayer* player, int damage, int hitgroup );
//    void draw_beam ( Vector startpos, Color color, Vector pos ) const;
//    void draw_footstep_beam ( C_BasePlayer* player ) const;
//    template <typename T>
//    std::string to_string_with_precision ( const T a_value, const int n = 6 )
//    {
//        std::ostringstream out;
//        out.precision ( n );
//        out << std::fixed << a_value;
//        return out.str();
//    }
//};
//

#pragma once
#include "../../valve_sdk/csgostructs.hpp"

#include <deque>
#include "../ragebot/aimbot/rage_backtrack.h"

struct log_data_t
{
    std::string msg = "";
    float time = 0.f;
};

struct event_data_t
{
    Vector pos = Vector ( 0, 0, 0 );
    bool got_pos = false;
    bool got_hurt = false;
    float time = 0.f;
    int hurt_player = -1;
    int userid = -1;
    bool hurt = false;
    int damage = 0;
    bool died = false;
    int hitbox = hitbox_max;
};

class c_event_logger
{
public:
    void on_fire_event ( IGameEvent* event );
    void on_create_move ( );
    void on_draw ( );

    void set_rbot_data ( C_BasePlayer* player, int index, QAngle angle );
    void set_rbot_data_backtrack ( C_BasePlayer* player, int index, QAngle angle, backtrack_all_data data );
private:
    int last_rbot_entity = -1;
    float last_rbot_shot_time = 0.f;
    player_backup_data_t last_shot_pdata;
    Vector last_rbot_shot_eyepos = Vector ( 0, 0, 0 );
    QAngle last_rbot_shot_angle = QAngle ( 0, 0, 0 );
    c_resolver::resolver_data_t last_rbot_resolver_data;

    enum class miss_type_t : int
    {
        none,
        spread,
        resolver,
        custom
    };

    std::deque< event_data_t > last_events;
    std::deque< log_data_t > logs;
    void log_damage ( C_BasePlayer* player, int damage, int hitgroup );
    void draw_beam ( Vector startpos, Color color, Vector pos ) const;
    void draw_footstep_beam ( C_BasePlayer* player ) const;

    template < typename t >
    static std::string to_string_with_precision ( const t a_value, const int n = 6 )
    {
        std::ostringstream out;
        out.precision ( n );
        out << std::fixed << a_value;
        return out.str( );
    }
};

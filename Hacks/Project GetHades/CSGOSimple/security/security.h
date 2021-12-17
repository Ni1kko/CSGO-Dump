#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include <chrono>
#include <ctime>
#include <deque>
#include "../include/json.h"

struct resolver_shot_data_t
{
    int shots = 0;
    bool resolved = false;
    bool did_hit = false;
    bool missed_due_to_spread = false;
    int damage = 0;
    bool head = false;
    float yaw = 0.f;
    std::string mode_name;
    //animation_layer_t layers[15];
    //c_base_player_anim_state state;
};

class c_security
{
public:
    c_security ( )
    {
        this->last_run_time = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now( ) );
        this->authkey = xor_str ( "" );
    }

    void run ( );
    void run_heartbeat ( );
    bool still_valid ( ) const;
    void init ( );

    std::deque< resolver_shot_data_t > info_data;
private:
    nlohmann::json inform_about_security_issue ( std::string action, nlohmann::json j );
    float last_run_time = 0.f;
    std::string authkey = "";
    std::string hwid = "";
    std::string url = xor_str ( "https://api.gethad.es/action" );
    std::string useragent = xor_str ( "GeG50d07jW6PzBUNsKBj" );

    void check_for_analysis_process ( );

    void send_resolver_data ( );

    bool is_inside_vmware ( ) const;
    bool is_inside_vpc ( ) const;

    std::string get_string ( std::wstring wst );
};

extern c_security security;

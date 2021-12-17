#pragma once
#include "../../../valve_sdk/csgostructs.hpp"
#include "rage_backtrack.h"
#include <deque>

struct point_scan_struct_t
{
    Vector pos;
    bool center = false;
};

enum class delay_mode_t : int
{
    none,
    shot,
    unlag
};

enum class rbot_baim_modes_t : int
{
    none,
    baim,
    force_baim
};

enum class user_baim_modes_t : int
{
    none,
    adaptive,
    desync,
    prefer
};

enum class autostop_modes_t : int
{
    none,
    slowwalk,
    faststop,
    stop
};

enum class shooting_modes_t : int
{
    normal,
    instant,
    client_silent,
    antiaim,
    supress
};

enum class backtrack_modes_t : int
{
    none,
    best,
    all,
    last
};

struct aimbot_bone_data_struct_t
{
    matrix3x4_t matrix[maxstudiobones];
    float time = 0.f;
    bool got_bone = false;
    int index = -1;
};

struct rbot_hitbox_t
{
    int hitbox = -1;
    float pointscale = 0.f;
};

struct ragebot_autowall_data_t
{
    bool init = false;
    Vector local_eyepos = Vector ( 0, 0, 0 );
    QAngle last_enemy_eyeang = QAngle ( 0, 0, 0 );
    Vector last_enemy_pos = Vector ( 0, 0, 0 );
    bool can_hit[hitbox_max]{ };
    std::deque< float > damage[hitbox_max];
};

class c_ragebot
{
public:
    struct can_hit_struct_t
    {
        bool can_hit = false;
        float damage = 0.f;
        Vector pos;
        int hitbox = 0;
        bool using_backtrack = false;
        backtrack_all_data bdata;
    };

    // enums
    enum class target_select_modes_t : int
    {
        damage,
        fov,
        nearest,
        loop
    };

    void on_create_move ( usercmd_t* cmd, bool& send_packet );
    bool did_shoot_this_tick ( ) const;

    float next_shot_time = 0.f;
    void set_shooting ( C_BaseCombatWeapon* weapon );
    bool can_shoot ( ) const;

    std::deque< aimbot_bone_data_struct_t > aimbot_shots_data;
    bool should_supress = false;
private:
    bool did_shoot = false;

    ragebot_autowall_data_t last_autowall_data[64];
    std::vector< rbot_hitbox_t > current_hitboxes;
    C_BasePlayer* player = nullptr;

    struct
    {
        float last_simtime = 0.f;
        bool did_unlag = false;
        bool breaking_lc = false;
    } saved_player_data[64];

    can_hit_struct_t can_hit_body;

    void create_hitbox_list ( );
    void find_target ( C_BasePlayer* local, C_BaseCombatWeapon* weapon );
    static bool is_baim_hitbox ( int hitbox );

    /**
     * \brief
     * \return whether shooting at entity should be delayed
     */
    bool should_delay ( );

    /**
     * \brief
     * \return whether enemy is breaking lag compensation
     */
    bool is_breaking_lc ( );

    /**
     * \brief
     * \return the baimmode to use
     */
    rbot_baim_modes_t get_baimmode ( ) const;

    /**
     * \brief
     * \param local local player entity
     * \param weapon local player weapon entity
     * \param damage_out predicted damage output
     * \param pos_out position output
     * \param hitbox_out hitbox id output
     * \param using_backtrack_out is using backtrack output
     * \param backtrackdata_out backtrack record output
     * \return whether it can hit the entity or not
     */
    bool calculate_target_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, float& damage_out, Vector& pos_out, int& hitbox_out,
                                   bool& using_backtrack_out, backtrack_all_data& backtrackdata_out );

    /**
     * \brief
     * \param local local local player entity
     * \param weapon local player weapon entity
     * \param record backtrack record
     * \param damage_out predicted damage output
     * \param pos_out position output
     * \param hitbox_out hitbox id output
     * \return whether it can hit the entity or not
     */
    bool calculate_target_backtrack_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, backtrack_all_data& record, float& damage_out, Vector& pos_out,
                                             int& hitbox_out );

    /* target selection modes */
    bool get_target_by_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, float& damage, Vector& hitbox_pos_export,
                                int& entity_out, bool& is_baiming_out, int& hitbox_out, backtrack_all_data& bdata_out, bool& using_backtrack_out );

    bool process_backtrack_by_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, C_BasePlayer* entity, float& damage_out,
                                       Vector& pos_out, backtrack_all_data& backtrackrecord_out, bool& should_overwrite_out, int& hitbox_out );

    bool backtrack_get_best_hitbox_by_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, C_BasePlayer* entity, int i, int backtrack_idx,
                                               float& damage, Vector& out_pos, bool& should_overwrite );

    /* hitbox shit */
    bool prepare_hitboxes ( C_BasePlayer* player );
    studiohdr_t* cached_studio_hdr = nullptr;
    matrix3x4_t cached_matrix[maxstudiobones];
    mstudiohitboxset_t* cached_studiohitboxset = nullptr;
    mstudiobbox_t* cached_studio_box = nullptr;
    bool get_hitbox ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos );
    bool get_hitbox_backtrack ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos, backtrack_all_data data );
    std::vector< point_scan_struct_t > get_points_for_hitscan ( C_BasePlayer* player, int hitbox, float pointscale );

    /* prediction */
    bool in_prediction = false;
    bool start_prediction ( C_BasePlayer* player );
    void end_prediction ( C_BasePlayer* player );
    matrix3x4_t prediction_matrix[maxstudiobones];
    player_backup_data_t pre_prediction_data;
    bool get_predicted_hitbox ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos, int& new_tickcount ) const;

    //std::deque<bullet_impact_data> bullet_impacts;

    /* features */
    static void auto_scope ( usercmd_t* cmd );
    static void auto_stop ( usercmd_t* cmd );
    void zeusbot ( usercmd_t* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon, bool& send_packet );
    void knifebot ( usercmd_t* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon );

    /* extra info */
    bool in_fakelag ( C_BasePlayer* player );
    float simtimes[64]{ };

    //usercmd_t* current_cmd = nullptr;

    /**
     * \brief calculates a chance to hit an player
     * \param ang aim angle
     * \param player player entity
     * \param hitchance hitchance that is needed
     * \return true when hitchance is higher than given input hitchance
     */
    bool hit_chance ( QAngle ang, C_BasePlayer* player, float hitchance ) const;
};

#pragma once
#include "../../../valve_sdk/csgostructs.hpp"
#include <deque>
//#include "../../../features/resolver.h"
#include "../resolver/resolver.h"

struct player_backtrack_t
{
public:
    bool backup ( C_BasePlayer* player, matrix3x4_t* matrix );
    bool restore ( C_BasePlayer* player );

    matrix3x4_t m_matrix[maxstudiobones];
private:
    matrix3x4_t m_bone_cache[256];
    int m_last_bone_count = -1;
};

struct player_backup_data_t
{
    player_backup_data_t ( );

    bool create ( C_BasePlayer* player )
    {
        if ( !player->setup_bones ( matrix, maxstudiobones, bone_used_by_anything, player->simulation_time( ) ) )
            return false;

        vec_velocity = player->vec_velocity( );
        lower_body_yaw = player->lower_body_yaw_target( );
        flags = player->flags( );
        //min = player->get
        min = player->get_collideable( )->obb_mins( );
        max = player->get_collideable( )->obb_maxs( );

        auto renderable = player->get_client_renderable( );
        uintptr_t* bonecache = *reinterpret_cast< uintptr_t** > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x290C );
        const uint32_t count = *reinterpret_cast< uintptr_t* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x2918 );
        max_bones_count = count;
        memcpy ( bone_matrix_cache, bonecache, sizeof ( matrix3x4_t ) * ( count < 256 ? count : 256 ) );

        heav_armor = player->has_heavy_armor( );
        armor = player->armor_value( );
        origin = player->vec_origin( );

        init = true;

        return true;
    }

    bool restore ( C_BasePlayer* player ) const;

    bool init = false;
    /* matrix and hitbox data */
    studiohdr_t* studio_hdr = nullptr;
    matrix3x4_t matrix[maxstudiobones];
    matrix3x4_t bone_matrix_cache[256];
    mstudiohitboxset_t* studiohitboxset = nullptr;
    /* general data */
    Vector min;
    Vector max;
    Vector origin;
    bool got = false;
    Vector vec_velocity;
    float lower_body_yaw{ };
    int flags;
    bool heav_armor = false;
    int armor = 0;
    int max_bones_count = 0;
};

struct backtrack_optimize_data
{
    Vector local_pos = Vector ( 0, 0, 0 );
    bool did_run_autowall[hitbox_max]{ };
    float autowall_dmg[hitbox_max]{ };
    Vector hitbox_positions[hitbox_max];
};

struct backtrack_all_data
{
    bool operator== ( const backtrack_all_data& d ) const
    {
        return simtime == d.simtime && creation_time == d.creation_time;
    }

    bool operator!= ( const backtrack_all_data& d ) const
    {
        return simtime != d.simtime || creation_time != d.creation_time;
    }

    player_backup_data_t ndata;
    c_resolver::resolver_data_t rdata;
    float simtime = 0.f;
    backtrack_optimize_data adata;
    Vector last_local_eye_awall_pos = Vector ( 0, 0, 0 );
    float creation_time = 0.f;
};

class c_rage_backtrack
{
public:
    void on_create_move ( );
    float get_lerp_time ( ) const;
    bool tick_valid ( int tick ) const;
    player_backup_data_t get_normal_data ( int i ) const;
    backtrack_all_data get_last_backtrack_data ( int i );
    std::deque< backtrack_all_data > saved_data[64];
    //matrix3x4_t overwrite_matrix[maxstudiobones];
private:
    void add_record ( );
};

#pragma once
#include "../../valve_sdk/csgostructs.hpp"

struct lbot_player_data
{
    matrix3x4_t matrix[maxstudiobones];
    // more shit
};

class c_legitbot
{
public:
    void on_create_move ( usercmd_t* cmd );
private:
    /* main functions */
    void do_aimbot ( usercmd_t* cmd );
    std::vector< int > get_hitboxes_to_scan ( );

    void do_rcs ( usercmd_t* cmd );
    void reset_rcs ( usercmd_t* cmd );

    /* local data */
    C_BaseCombatWeapon* weapon = nullptr;
    C_BasePlayer* local = nullptr;
    QAngle viewangles = { };
    CCSWeaponInfo* weapon_data = nullptr;

    /* scanned player data */
    matrix3x4_t scan_matrix[maxstudiobones];
    mstudiohitboxset_t* scan_hitboxset = nullptr;
    studiohdr_t* scan_hdr = nullptr;

    /* helper functions */
    QAngle smooth_angle ( QAngle input, QAngle new_angle );
    bool prepare_player ( C_BasePlayer* player );
    bool get_hitbox_pos ( int hitbox, Vector& pos ) const;

    /* vars */
    struct rcs_data
    {
        rcs_data ( )
        {
        }

        rcs_data ( float p, float y )
        {
            this->pitch = p;
            this->yaw = y;
            this->time = interfaces::global_vars->realtime;
        }

        rcs_data ( C_BasePlayer* player )
        {
            *this = rcs_data ( globals.old_aimpunch.pitch, globals.old_aimpunch.yaw );
        }

        float pitch = 0.f;
        float yaw = 0.f;
        float time = 0.f;
    };

    rcs_data last_rcs_data;
};

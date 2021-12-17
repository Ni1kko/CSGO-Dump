#pragma once
#include "../../../valve_sdk/csgostructs.hpp"

struct fire_bullet_data
{
    Vector src;
    CGameTrace enter_trace;
    Vector direction;
    CTraceFilter filter;
    float trace_length{ };
    float trace_length_remaining{ };
    float current_damage{ };
    int penetrate_count{ };
};

class c_autowall
{
public:
    float get_thickness ( Vector& start, Vector& end, float distance ) const;
    bool can_hit_point ( const Vector& point, const Vector& source );
    bool can_hit_point ( const Vector& point, const Vector& source, C_BaseEntity* skip );
    float can_hit_point_dmg ( const Vector& point, const Vector& source );
    float can_hit_point_dmg ( const Vector& point, C_BasePlayer* attacker );
    float damage ( const Vector& point );
    void overwrite_hitbox ( int hitbox );
    void reset_overwrite ( );

private:
    bool trace_to_exit_short ( Vector& point, Vector& dir, const float step_size, float max_distance ) const;
    static void trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr );
    static bool vectorto_vector_visible ( Vector src, Vector point );
    float get_hitgroup_damage_mult ( int iHitGroup );
    bool handle_bullet_penetration ( CCSWeaponInfo* wpn_data, fire_bullet_data& data, bool extracheck, Vector point );
    void scale_damage ( int hitgroup, C_BasePlayer* enemy, float weapon_armor_ratio, float& current_damage );
    void util_clip_trace_to_players ( const Vector& vec_abs_start, const Vector& vec_abs_end, unsigned int mask, ITraceFilter* filter,
                                      CGameTrace* tr ) const;
    bool simulate_fire_bullet ( C_BaseEntity* local, C_BaseCombatWeapon* weapon, fire_bullet_data& data );
    bool trace_to_exitalt ( Vector& end, CGameTrace& tr, Vector start, Vector v_end, CGameTrace* trace ) const;

    bool hitbox_overwritten_ = false;
    int hitbox_ = -1;
};

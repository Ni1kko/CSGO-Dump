#pragma once
#include "../../../valve_sdk/csgostructs.hpp"

struct hitpoint_t
{
    hitpoint_t ( ) = default;

    explicit hitpoint_t ( vector_t position, const bool center = false ) : position ( position ), center ( center )
    {
    };
    vector_t position{ };
    bool center = false;
};

class new_ragebot
{
public:
    enum hitscan_t
    {
        damage,
        distance,
        automatic
    };

    struct can_hit_t
    {
        can_hit_t ( ) = default;
        /// <summary>creates a object of the current struct</summary>
        /// <param name="damage">the calculated damage</param>
        /// <param name="position">the position</param>
        can_hit_t ( float damage, vector_t position );

        float damage = -1.f;
        vector_t position{ };
    };

public:
    /// <summary>runs the ragebot and fires a shot</summary>
    void run ( ) noexcept;

    /// <summary>Calculates the time till the next shot can be fired, etc...</summary>
    void finish_run ( ) noexcept;

    /// <summary>Rests the ragebot's timers</summary>
    void reset_ragebot ( ) noexcept;

private:
    /// <summary>compensates the weapons recoil</summary>
    void compensate_recoil ( ) noexcept;

private:
    float m_next_shot_time = 0.f;
    int m_last_player_index = 0;

private:
    /// <summary>Builds the list of hitbox's from the user selected options.</summary>
    void build_hitbox_list ( );

    std::vector< int > m_hitboxes;

private:
    /// <summary>
    /// Runs the user selected hitscan mode
    /// </summary>
    void run_hitscan ( );

    /// <summary></summary>
    void run_damage_hitscan ( );

    /// <summary></summary>
    void run_distance_hitscan ( );

    /// <summary></summary>
    void run_automatic_hitscan ( );

    /// <summary>Returns the point from a player that hurts him the most.</summary>
    /// <param name="damage_out">the damage we would give him</param>
    /// <param name="point_out">the point where we hurt him the most</param>
    /// <returns>whether the scan was successful or not</returns>
    bool get_best_hitpoint ( float& damage_out, vector_t& point_out );

    /// <summary>returns a vector with hitbox positions (center, multi-point)</summary>
    /// <param name="hitbox">the hitbox id</param>
    /// <returns>a vector with the positions</returns>
    std::vector< hitpoint_t > get_hitbox_points ( int hitbox ) noexcept;

private:
    /// <summary>prepares the ragebot for hit scanning the player</summary>
    /// <param name="player">the player entity</param>
    /// <returns>whether the caching of matrix, studiohdr, ... was successful or not</returns>
    bool prepare_player ( C_BasePlayer* player );

    studiohdr_t* m_tmp_studio_hdr = nullptr;
    mstudiohitboxset_t* m_tmp_studio_hitboxset = nullptr;
    std::array< matrix3x4_t, 128 > m_tmp_bone_matrix;

private:
    bool m_found_hittable_player = false;
    C_BasePlayer* m_player_found = nullptr;
    vector_t m_hitpos_found = { 0, 0, 0 };
    bool m_using_backtrack = false;
    // todo when backtrack is redone use this here
};

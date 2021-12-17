#pragma once
#include "sdk.hpp"
#include <array>
#include "../IRefCounted.h"
#include "../helpers/utils.hpp"
#include "../hooks/hooks.hpp"
#define netvar(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = g_netvarsys.GetOffset(table, netvar);     \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + _##name);                 \
    }

#define pnetvar(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = g_netvarsys.GetOffset(table, netvar);     \
        return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + _##name);                 \
    }

#define netprop(name, table, netvar) static RecvProp* name() \
{ \
	static auto prop_ptr = g_netvarsys.GetNetvarProp(table, netvar); \
	return prop_ptr; \
}

#define offset(type, varname, amount) type& varname##() { \
    return *reinterpret_cast<type*> (reinterpret_cast<uintptr_t>(this) + amount); \
    }

#define pred_map_prop(type, varname, name) type& varname##() { \
        static unsigned int _static = g_utils.find_in_data_map ( get_pred_desc_map(), name ); \
        return *reinterpret_cast<type*> (reinterpret_cast<uintptr_t>(this) + _static); \
    }

namespace vfindex
{ 
#define add_idx(name, idx) constexpr int name##  = idx

    namespace ccsplayer
    {
        // 55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36
        add_idx ( update_client_side_animations, 221 );
        add_idx ( is_player, 155 );
    }

    namespace ccscombat_weapon
    {
        add_idx ( is_weapon, 163 );
        add_idx ( get_cs_weapon_data, 454 );
        add_idx ( get_inaccuracy, 476 );
        add_idx ( get_spread, 446 );
        add_idx ( update_accuracy_penalty, 477 );
    }

    namespace cattributable_item
    {
        add_idx ( set_glove_model_index, 75 ); // prob outdated
        add_idx ( send_view_model_matching_sequence, 241 ); // prob outdated
    }
}

//float_t& surface_friction()
//{
//    //g_utils.find_in_data_map()
//    static unsigned int _m_surfaceFriction = g_utils.find_in_data_map ( get_pred_desc_map(), xor_str ( "m_surfaceFriction" ) );
//    return * ( float_t* ) ( ( uintptr_t ) this + _m_surfaceFriction );
//}

struct datamap_t;
class animation_layer_t;
class c_base_player_anim_state;
class CCSPlayerAnimState;
class CCSGOPlayerAnimState;
class C_BaseEntity;

class VarMapEntry_t
{
public:
    unsigned short type;
    unsigned short needs_to_interpolate; // Set to false when this var doesn't need Interpolate() called on it anymore.
    void* data;
    void* watcher;
};

struct VarMapping_t
{
    CUtlVector< VarMapEntry_t > entries;
    int interpolated_entries;
    float last_interpolation_time;
};

enum cs_weapon_type_t
{
    weapontype_knife = 0,
    weapontype_pistol,
    weapontype_submachinegun,
    weapontype_rifle,
    weapontype_shotgun,
    weapontype_sniper_rifle,
    weapontype_machinegun,
    weapontype_c4,
    weapontype_placeholder,
    weapontype_grenade,
    weapontype_unknown
};

class C_BaseEntity;

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
    char short_name[64]; //0x0000
    char texture_file[64]; //0x0040
    bool render_using_font; //0x0080
    bool precached; //0x0081
    int8_t character_in_font; //0x0082
    uint8_t pad_0083[1]; //0x0083
    uint32_t font; //0x0084
    int32_t texture_id; //0x0088
    float tex_coords[4]; //0x008C
    uint8_t pad_009_c[16]; //0x009C
};

class C_EconItemView
{
private:
    using str_32 = char[32];
public:
    netvar ( int32_t, initialized, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_bInitialized" ) );
    netvar ( short, item_definition_index, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iItemDefinitionIndex" ) );
    netvar ( int32_t, entity_level, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iEntityLevel" ) );
    netvar ( int32_t, account_id, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iAccountID" ) );
    netvar ( int32_t, item_id_low, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iItemIDLow" ) );
    netvar ( int32_t, item_id_high, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iItemIDHigh" ) );
    netvar ( int32_t, entity_quality, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_iEntityQuality" ) );
    netvar ( str_32, custom_name, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_szCustomName" ) );

    CUtlVector< IRefCounted* >& custom_materials ( );
    CUtlVector< IRefCounted* >& visuals_data_processors ( );
};

class C_BaseEntity : public IClientEntity
{
public:

    datamap_t* get_data_desc_map ( )
    {
        typedef datamap_t* ( __thiscall * o_GetPredDescMap ) ( void* );
        return CallVFunction< o_GetPredDescMap > ( this, 15 ) ( this );
    }

    datamap_t* get_pred_desc_map ( )
    {
        typedef datamap_t* ( __thiscall * o_GetPredDescMap ) ( void* );
        return CallVFunction< o_GetPredDescMap > ( this, 17 ) ( this );
    }

    static __forceinline C_BaseEntity* get_entity_by_index ( int index )
    {
        return dynamic_cast< C_BaseEntity* > ( interfaces::entity_list->get_client_entity ( index ) );
    }

    static __forceinline C_BaseEntity* get_entity_from_handle ( CBaseHandle h )
    {
        return dynamic_cast< C_BaseEntity* > ( interfaces::entity_list->get_client_entity_from_handle ( h ) );
    }

    netvar ( int32_t, model_index, xor_str ( "DT_BaseEntity" ), xor_str ( "m_nModelIndex" ) );
    netvar ( int32_t, team_num, xor_str ( "DT_BaseEntity" ), xor_str ( "m_iTeamNum" ) );
    netvar ( Vector, vec_origin, xor_str ( "DT_BaseEntity" ), xor_str ( "m_vecOrigin" ) );
    netvar ( Vector, vec_angles, xor_str ( "DT_BaseEntity" ), xor_str ( "m_vecAngles" ) );
    netvar ( bool, should_glow, xor_str ( "DT_DynamicProp" ), xor_str ( "m_bShouldGlow" ) );
    netvar ( CHandle<C_BasePlayer>, owner_entity, xor_str ( "DT_BaseEntity" ), xor_str ( "m_hOwnerEntity" ) );
    netvar ( bool, spotted, xor_str ( "DT_BaseEntity" ), xor_str ( "m_bSpotted" ) );
    netvar ( float_t, c4_blow, xor_str ( "DT_PlantedC4" ), xor_str ( "m_flC4Blow" ) );

    const matrix3x4_t& rgfl_coordinate_frame ( )
    {
        static auto m_rgfl_coordinate_frame = g_netvarsys.GetOffset ( xor_str ( "DT_BaseEntity" ), xor_str ( "m_CollisionGroup" ) ) - 0x30;
        return * reinterpret_cast< matrix3x4_t* > ( reinterpret_cast< uintptr_t > ( this ) + m_rgfl_coordinate_frame );
    }

    bool is_player ( );
    Vector get_bone_pos ( int i );
    Vector get_head_pos ( );
    bool is_loot ( );
    bool is_weapon ( );
    bool is_planted_c4 ( );
    bool is_defuse_kit ( );
    //bool isSpotted();

    int get_sequence ( )
    {
        return *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( this ) + 0x28B8 );
        // can be found in setup bones --> first check after bonemask | 0x80000
    }

    int prev_bone_mask ( )
    {
        return *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( this ) + 0x2698 );
        // can be found in setup bones --> second check after bonemask | 0x80000
    }

    //entity + 0x26B0
    DWORD bone_thread_id ( )
    {
        return *reinterpret_cast< DWORD* > ( reinterpret_cast< uintptr_t > ( this ) + 0x26B0 );
        // can be found in setup bones --> second check after bonemask | 0x80000
    }

    void standard_blending_rules ( studiohdr_t* hdr, Vector* pos, quaternion_t* q, float curtime );
};

class C_EnvTonemapController : public C_BaseEntity
{
public:
    netvar ( bool, use_custom_auto_exposure_min, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_bUseCustomAutoExposureMin" ) );
    netvar ( bool, use_custom_auto_exposure_max, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_bUseCustomAutoExposureMax" ) );
    netvar ( bool, use_custom_bloom_scale, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_bUseCustomBloomScale" ) );
    netvar ( float, custom_auto_exposure_min, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flCustomAutoExposureMin" ) );
    netvar ( float, custom_auto_exposure_max, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flCustomAutoExposureMax" ) );
    netvar ( float, custom_bloom_scale, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flCustomBloomScale" ) );
    netvar ( float, custom_bloom_scale_minimum, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flCustomBloomScaleMinimum" ) );
    netvar ( float, bloom_exponent, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flBloomExponent" ) );
    netvar ( float, bloom_saturation, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flBloomSaturation" ) );
    netvar ( float, tonemap_percent_target, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flTonemapPercentTarget" ) );
    netvar ( float, tonemap_percent_bright_pixels, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flTonemapPercentBrightPixels" ) );
    netvar ( float, tonemap_min_avg_lum, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flTonemapMinAvgLum" ) );
    netvar ( float, tonemap_rate, xor_str ( "DT_EnvTonemapController" ), xor_str ( "m_flTonemapRate" ) );
};

class C_PlantedC4
{
public:
    netvar ( bool, bomb_ticking, xor_str ( "DT_PlantedC4" ), xor_str ( "m_bBombTicking" ) );
    netvar ( bool, bomb_defused, xor_str ( "DT_PlantedC4" ), xor_str ( "m_bBombDefused" ) );
    netvar ( float, c4_blow, xor_str ( "DT_PlantedC4" ), xor_str ( "m_flC4Blow" ) );
    netvar ( float, timer_length, xor_str ( "DT_PlantedC4" ), xor_str ( "m_flTimerLength" ) );
    netvar ( float, defuse_length, xor_str ( "DT_PlantedC4" ), xor_str ( "m_flDefuseLength" ) );
    netvar ( float, defuse_count_down, xor_str ( "DT_PlantedC4" ), xor_str ( "m_flDefuseCountDown" ) );
    netvar ( CHandle<C_BasePlayer>, bomb_defuser, xor_str ( "DT_PlantedC4" ), xor_str ( "m_hBombDefuser" ) );
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    netvar ( uint64_t, original_owner_xuid, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_OriginalOwnerXuidLow" ) );
    netvar ( int32_t, original_owner_xuid_low, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_OriginalOwnerXuidLow" ) );
    netvar ( int32_t, original_owner_xuid_high, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_OriginalOwnerXuidHigh" ) );
    netvar ( int32_t, fallback_stat_trak, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_nFallbackStatTrak" ) );
    netvar ( int32_t, fallback_paint_kit, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_nFallbackPaintKit" ) );
    netvar ( int32_t, fallback_seed, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_nFallbackSeed" ) );
    netvar ( float_t, fallback_wear, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_flFallbackWear" ) );

    netvar ( C_EconItemView, item2, xor_str ( "DT_BaseAttributableItem" ), xor_str ( "m_Item" ) );

    C_EconItemView& item ( )
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return * reinterpret_cast< C_EconItemView* > ( this );
    }

    void set_glove_model_index ( int model_index );
};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
    netvar ( int32_t, model_index, xor_str ( "DT_BaseWeaponWorldModel" ), xor_str ( "m_nModelIndex" ) );
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
    netvar ( float_t, next_primary_attack, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_flNextPrimaryAttack" ) );
    netvar ( float_t, next_secondary_attack, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_flNextSecondaryAttack" ) );
    netvar ( int32_t, clip1, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iClip1" ) );
    netvar ( int, primary_reserve_ammo_count, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iPrimaryReserveAmmoCount" ) );
    netvar ( int32_t, clip2, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iClip2" ) );
    netvar ( float_t, recoil_index, xor_str ( "DT_WeaponCSBase" ), xor_str ( "m_flRecoilIndex" ) );
    netvar ( int32_t, view_model_index, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iViewModelIndex" ) );
    netvar ( int32_t, world_model_index, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iWorldModelIndex" ) );
    netvar ( int32_t, world_dropped_model_index, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_iWorldDroppedModelIndex" ) );
    netvar ( bool, pin_pulled, xor_str ( "DT_BaseCSGrenade" ), xor_str ( "m_bPinPulled" ) );
    netvar ( float_t, throw_time, xor_str ( "DT_BaseCSGrenade" ), xor_str ( "m_fThrowTime" ) );
    netvar ( float_t, postpone_fire_ready_time, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_flPostponeFireReadyTime" ) );
    netvar ( CHandle<C_BaseWeaponWorldModel>, weapon_world_wodel, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_hWeaponWorldModel" ) );
    netvar ( float, time_weapon_idle, xor_str ( "DT_BaseCombatWeapon" ), xor_str ( "m_flTimeWeaponIdle" ) );
    netvar ( float, last_shot_time, "DT_WeaponCSBase", "m_fLastShotTime" );

    CCSWeaponInfo* get_cs_weapon_data ( );
    bool has_bullets ( );
    bool can_fire ( );
    bool is_grenade ( );
    bool is_knife ( );
    bool is_reloading ( );
    bool is_rifle ( );
    bool is_pistol ( );
    bool is_sniper ( );
    bool is_auto ( );
    bool is_scout ( );
    bool is_r8 ( );
    bool is_deagle ( );
    bool is_lmg ( );
    bool is_smg ( );
    bool is_gun ( );
    bool is_zeus ( );
    float get_inaccuracy ( );
    float get_spread ( );
    void update_accuracy_penalty ( );
    CUtlVector< IRefCounted* >& custom_materials ( );
    bool* custom_material_initialized ( );
};

class C_BasePlayer : public C_BaseEntity
{
public:
    static __forceinline C_BasePlayer* get_player_by_user_id ( int id )
    {
        return dynamic_cast< C_BasePlayer* > ( get_entity_by_index ( interfaces::engine_client->get_player_for_userid ( id ) ) );
    }

    static __forceinline C_BasePlayer* get_player_by_index ( int i )
    {
        return dynamic_cast< C_BasePlayer* > ( get_entity_by_index ( i ) );
    }

    netvar ( int, set_observer_mode, xor_str ( "DT_BasePlayer" ), xor_str ( "m_iObserverMode" ) );
    netvar ( bool, has_defuser, xor_str ( "DT_CSPlayer" ), xor_str ( "m_bHasDefuser" ) );
    netvar ( bool, gun_game_immunity, xor_str ( "DT_CSPlayer" ), xor_str ( "m_bGunGameImmunity" ) );
    netvar ( int32_t, shots_fired, xor_str ( "DT_CSPlayer" ), xor_str ( "m_iShotsFired" ) );
    netvar ( QAngle, eye_angles, xor_str ( "DT_CSPlayer" ), xor_str ( "m_angEyeAngles[0]" ) );
    netvar ( int, armor_value, xor_str ( "DT_CSPlayer" ), xor_str ( "m_ArmorValue" ) );
    netvar ( bool, has_heavy_armor, xor_str ( "DT_CSPlayer" ), xor_str ( "m_bHasHeavyArmor" ) );
    netvar ( bool, has_helmet, xor_str ( "DT_CSPlayer" ), xor_str ( "m_bHasHelmet" ) );
    netvar ( bool, is_scoped, xor_str ( "DT_CSPlayer" ), xor_str ( "m_bIsScoped" ) );
    netvar ( float, lower_body_yaw_target, xor_str ( "DT_CSPlayer" ), xor_str ( "m_flLowerBodyYawTarget" ) );
    netvar ( int32_t, health, xor_str ( "DT_BasePlayer" ), xor_str ( "m_iHealth" ) );
    netvar ( int32_t, e_state, xor_str ( "DT_BasePlayer" ), xor_str ( "m_eState" ) );
    netvar ( int32_t, flags, xor_str ( "DT_BasePlayer" ), xor_str ( "m_fFlags" ) );
    netvar ( int32_t, tick_base, xor_str ( "DT_BasePlayer" ), xor_str ( "m_nTickBase" ) );
    netvar ( Vector, vec_view_offset, xor_str ( "DT_BasePlayer" ), xor_str ( "m_vecViewOffset[0]" ) );
    netvar ( QAngle, view_punch_angle, xor_str ( "DT_BasePlayer" ), xor_str ( "m_viewPunchAngle" ) );
    netvar ( QAngle, aim_punch_angle, xor_str ( "DT_BasePlayer" ), xor_str ( "m_aimPunchAngle" ) );
    netvar ( CHandle<C_BaseViewModel>, view_model, xor_str ( "DT_BasePlayer" ), xor_str ( "m_hViewModel[0]" ) );
    netvar ( Vector, vec_velocity, xor_str ( "DT_BasePlayer" ), xor_str ( "m_vecVelocity[0]" ) );
    netvar ( float, maxspeed, xor_str ( "DT_BasePlayer" ), xor_str ( "m_flMaxspeed" ) );
    netvar ( CHandle<C_BasePlayer>, observer_target, xor_str ( "DT_BasePlayer" ), xor_str ( "m_hObserverTarget" ) );
    netvar ( float, flash_max_alpha, xor_str ( "DT_CSPlayer" ), xor_str ( "m_flFlashMaxAlpha" ) );
    netvar ( int32_t, hitbox_set, xor_str ( "DT_BaseAnimating" ), xor_str ( "m_nHitboxSet" ) );
    netvar ( CHandle<C_BaseCombatWeapon>, active_weapon, xor_str ( "DT_BaseCombatCharacter" ), xor_str ( "m_hActiveWeapon" ) );
    netvar ( int32_t, account, xor_str ( "DT_CSPlayer" ), xor_str ( "m_iAccount" ) );
    netvar ( float, flash_duration, xor_str ( "DT_CSPlayer" ), xor_str ( "m_flFlashDuration" ) );
    netvar ( float, simulation_time, xor_str ( "DT_BaseEntity" ), xor_str ( "m_flSimulationTime" ) );
    netvar ( float, cycle, xor_str ( "DT_ServerAnimationData" ), xor_str ( "m_flCycle" ) );
    netvar ( int, sequence, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_nSequence" ) );
    pnetvar ( char, last_place_name, xor_str ( "DT_BasePlayer" ), xor_str ( "m_szLastPlaceName" ) );
    netprop ( lower_body_yaw_target_prop, xor_str ( "DT_CSPlayer" ), xor_str ( "m_flLowerBodyYawTarget" ) );
    netprop ( ping_prop, xor_str ( "CPlayerResource" ), xor_str ( "m_iPing" ) );
    netvar ( bool, client_side_animation, xor_str ( "DT_BaseAnimating" ), xor_str ( "m_bClientSideAnimation" ) );
    netvar ( Vector, rag_pos, xor_str ( "DT_Ragdoll" ), xor_str ( "m_ragPos" ) );
    netvar ( Vector, vec_force, xor_str ( "DT_CSRagdoll" ), xor_str ( "m_vecForce" ) );
    netvar ( Vector, vec_ragdoll_velocity, xor_str ( "DT_CSRagdoll" ), xor_str ( "m_vecRagdollVelocity" ) );
    //NETVAR ( float, duck_speed, "DT_BasePlayer", "m_flDuckSpeed" );
    netvar ( Vector, view_offset, xor_str ( "DT_BasePlayer" ), xor_str ( "m_vecViewOffset" ) );
    netvar ( bool, skybox_fog, xor_str ( "DT_BasePlayer" ), xor_str ( "m_skybox3d.fog.enable" ) );
    netvar ( short, ping, xor_str ( "DT_PlayerResource" ), xor_str ( "m_iPing" ) );
    netvar ( int, effects, xor_str ( "DT_BaseEntity" ), xor_str ( "m_fEffects" ) );
    netvar ( CHandle<C_BaseEntity>, ground_entity, xor_str ( "DT_BasePlayer" ), xor_str ( "m_hGroundEntity" ) );

    //NETVAR(int, m_iAccount, "DT_CSPlayer", "m_iAccount");

    int& eflag ( )
    {
        return *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( this ) + 0xE8 ); // is not working
    }

    void set_abs_angles ( const QAngle& angles )
    {
        using SetAbsAnglesFn = void ( __thiscall*) ( void*, const QAngle& angles );
        static SetAbsAnglesFn SetAbsAngles = ( SetAbsAnglesFn )g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                      xor_str ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );

        SetAbsAngles ( this, angles );
    }

    void set_abs_original ( Vector origin )
    {
        using SetAbsOriginFn = void ( __thiscall*) ( void*, const Vector& origin );
        static SetAbsOriginFn SetAbsOrigin;

        if ( !SetAbsOrigin )
            SetAbsOrigin = ( SetAbsOriginFn )( ( DWORD )g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                               xor_str ( "55 8B EC 83 E4 ? 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D ?" ) ) );

        SetAbsOrigin ( this, origin );
    }

    bool is_local ( )
    {
        return this == g_local;
    }

    float get_max_desync_delta ( );
    float get_max_desync_delta_inverted ( );
    float get_yaw_feet_delta ( );

    VarMapping_t* var_mapping ( )
    {
        return reinterpret_cast< VarMapping_t* > ( reinterpret_cast< DWORD > ( this ) + 0x24 );
    }

    void modify_eye_pos ( c_base_player_anim_state* animstate, vector_t* pos );
    vector_t get_eye_pos ( );
    vector_t get_eye_pos ( void* animstate );

    //void set_vangle ( QAngle angles )
    //{
    //    static auto deadflag = g_netvarsys.GetOffset ( xor_str ( "DT_BasePlayer" ), xor_str ( "deadflag" ) );
    //    * reinterpret_cast< QAngle* > ( reinterpret_cast< DWORD > ( this ) + deadflag + 0x4 ) = angles;
    //}

    //NETVAR ( QAngle, get_abs_angles, "DT_BaseEntity", "m_angAbsAngles" );
    //NETVAR ( Vector, get_abs_origin, "DT_BaseEntity", "m_angAbsOrigin" );
    netvar ( float, duck_speed, xor_str ( "DT_BasePlayer" ), xor_str ( "m_flDuckSpeed" ) );
    netvar ( float, duck_amount, xor_str ( "DT_BasePlayer" ), xor_str ( "m_flDuckAmount" ) );
    //NETVAR(std::array<float, 24>,			 m_flPoseParameter,		 "DT_BaseAnimating", "m_flPoseParameter");
    std::array< float, 24 > pose_parameter ( )
    {
        static int _m_flPoseParameter = g_netvarsys.GetOffset ( xor_str ( "DT_BaseAnimating" ), xor_str ( "m_flPoseParameter" ) );
        return * reinterpret_cast< std::array< float, 24 >* > ( reinterpret_cast< uintptr_t > ( this ) + _m_flPoseParameter );
    }

    pnetvar ( CHandle<C_BaseCombatWeapon>, my_weapons, xor_str ( "DT_BaseCombatCharacter" ), xor_str ( "m_hMyWeapons" ) );
    pnetvar ( CHandle<C_BaseAttributableItem>, my_wearables, xor_str ( "DT_BaseCombatCharacter" ), xor_str ( "m_hMyWearables" ) );

    netvar ( int, survival_team, xor_str ( "DT_CSPlayer" ), xor_str ( "m_nSurvivalTeam" ) );

    usercmd_t*& current_command ( );

    bool in_dangerzone ( ) const
    {
        static auto game_type = interfaces::cvar->find_var ( xor_str ( "game_type" ) );
        return game_type->get_int( ) == 6;
    }

    bool is_enemy ( ) const
    {
        if ( !g_local )
            return false;

        if ( in_dangerzone( ) )
            return this->survival_team( ) != g_local->survival_team( ) || g_local->survival_team( ) == -1;
        else
            return this->team_num( ) != g_local->team_num( );
    }

    const Vector world_space_center ( )
    {
        Vector vecOrigin = vec_origin( );

        Vector min = this->get_collideable( )->obb_mins( ) + vecOrigin;
        Vector max = this->get_collideable( )->obb_maxs( ) + vecOrigin;

        Vector size = max - min;
        size /= 2.f;
        size += min;

        return size;
    }

    /*gladiator v2*/
    void invalidate_bone_cache ( );
    int get_num_anim_overlays ( );
    animation_layer_t* get_anim_overlays ( );
    animation_layer_t* get_anim_overlay ( int i );
    int get_sequence_activity ( int sequence );
    c_base_player_anim_state* get_base_player_anim_state ( );
    CCSPlayerAnimState* get_player_anim_state ( );

    //bool setup_bones ( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );

    static void update_animation_state ( CCSGOPlayerAnimState* state, QAngle angle );
    static void reset_animation_state ( CCSGOPlayerAnimState* state );
    void create_animation_state ( CCSGOPlayerAnimState* state );

    pred_map_prop ( float, surface_friction, xor_str ( "m_surfaceFriction" ) );
    pred_map_prop ( Vector, vec_base_velocity, xor_str ( "m_vecBaseVelocity" ) );
    pred_map_prop ( float, maxspeed, xor_str ( "m_flMaxspeed" ) );

    /*QAngle &m_angAbsRotation()
    {
    return *(QAngle*)((DWORD)&m_angRotation() - 12);
    }*/

    //Vector        get_eye_pos();
    player_info_t get_player_info ( ) const;
    bool is_alive ( ) const;
    bool is_flashed ( );
    bool has_c4 ( );
    Vector get_hitbox_pos ( int hitbox_id );
    mstudiobbox_t* get_hitbox ( int hitbox_id );
    bool get_hitbox_pos ( int hitbox, Vector& output );
    Vector get_bone_pos ( int bone );
    bool can_see_player ( C_BasePlayer* player, int hitbox );
    bool can_see_player ( C_BasePlayer* player, const Vector& pos );
    void update_client_side_animation ( );
    void set_angle2 ( QAngle wantedang );

    offset ( int, move_type, 0x25C );
    //int move_type();
    QAngle* get_vangles ( );
    float get_flash_bang_time ( );

    //float_t spawn_time();
    offset ( float, spawn_time, 0xA350 );
};

class C_BaseViewModel : public C_BaseEntity
{
public:
    netvar ( int32_t, model_index, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_nModelIndex" ) );
    netvar ( int32_t, view_model_index, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_nViewModelIndex" ) );
    netvar ( CHandle<C_BaseCombatWeapon>, weapon, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_hWeapon" ) );
    netvar ( CHandle<C_BasePlayer>, owner, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_hOwner" ) );
    netprop ( sequence, xor_str ( "DT_BaseViewModel" ), xor_str ( "m_nSequence" ) );
    void send_view_model_matching_sequence ( int sequence );
    //float sequence_duration ( int s );
};

class animation_layer_t
{
public:
    char pad_0000[20];
    // These should also be present in the padding, don't see the use for it though
    //float	m_flLayerAnimtime;
    //float	m_flLayerFadeOuttime;
    uint32_t order; //0x0014
    uint32_t sequence; //0x0018
    float_t prev_cycle; //0x001C
    float_t weight; //0x0020
    float_t weight_delta_rate; //0x0024
    float_t playback_rate; //0x0028
    float_t cycle; //0x002C
    void* owner; //0x0030 // player's thisptr
    char pad_0038[4]; //0x0034
}; //Size: 0x0038

class c_base_player_anim_state
{
public:
    char pad[4];
    char bUnknown; //0x4 || triggers new animation like when switching weapons
    float framecount_thing; // ref DoProceduralFootPlant
    float someTime;
    char pad2[83];
    void* pBaseEntity; //0x60
    void* pActiveWeapon; //0x64
    void* pLastActiveWeapon; //0x68
    float m_flLastClientSideAnimationUpdateTime; //0x6C
    int m_iLastClientSideAnimationUpdateFramecount; //0x70
    float m_flFrameTime; //0x74 (Curtime - m_flLastClientSideAnimationUpdateTime)
    float m_flYaw; //0x78
    float m_flPitch; //0x7C
    float m_flGoalFeetYaw; //0x80
    float m_flCurrentFeetYaw; //0x84
    float m_flCurrentTorsoYaw; //0x88
    float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
    float m_flLeanAmount; //0x90
    char pad4[4]; //NaN
    float m_flFeetCycle; //0x98 0 to 1
    float m_flFeetYawRate; //0x9C 0 to 1
    float m_fUnknown2;
    float m_fDuckAmount; //0xA4
    float m_fLandingDuckAdditiveSomething; //0xA8
    float m_fUnknown3; //0xAC
    Vector m_vOrigin; //0xB0, 0xB4, 0xB8
    Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
    //Vector velocity; // 0xC8, 0xCC, 0xD0
    float m_vVelocityX; //0xC8
    float m_vVelocityY; //0xCC
    float m_vVelocityZ; //0xD0
    float m_flUnknownFloat1; //0xD4 Affected by movement and direction
    char pad6[8];
    float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
    float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
    float not_speed_fraction; //0xE8 //possible speed fraction
    float speed_2d; //0xEC
    float flUpVelocity; //0xF0
    float m_flSpeedNormalized; //0xF4 //from 0 to 1
    float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
    float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
    float m_flTimeSinceStartedMoving; //0x100
    float m_flTimeSinceStoppedMoving; //0x104
    bool m_bOnGround; //0x108
    bool m_bInHitGroundAnimation; //0x109
    char pad7[10];
    float m_flLastOriginZ; //0x114
    float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
    float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
    char pad8[4]; //NaN
    float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
    char pad9[4]; //NaN
    float m_flUnknown3;
    char pad10[528];

public:
    float max_rotation ( );
};

class CCSPlayerAnimState
{
public:
    virtual ~CCSPlayerAnimState ( ) = 0;
    virtual void update ( float_t eyeYaw, float_t eyePitch ) = 0;
};

class CCSGOPlayerAnimState
{
public:

    Vector get_vec_velocity ( )
    {
        // Only on ground velocity
        return * reinterpret_cast< Vector* > ( reinterpret_cast< uintptr_t > ( this ) + 0xC8 );
    }

    float get_velocity ( )
    {
        return * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( this ) + 0xEC );
    }

    char pad_0x0000[0x344]; //0x0000
}; //Size=0x344

class DT_CSPlayerResource
{
public:
    pnetvar ( int32_t, active_coin_rank, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nActiveCoinRank" ) );
    pnetvar ( int32_t, music_id, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nMusicID" ) );
    pnetvar ( int32_t, persona_data_public_level, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nPersonaDataPublicLevel" ) );
    pnetvar ( int32_t, ersona_data_public_commends_leader, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nPersonaDataPublicCommendsLeader" ) );
    pnetvar ( int32_t, persona_data_public_commends_teacher, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nPersonaDataPublicCommendsTeacher" ) );
    pnetvar ( int32_t, persona_data_public_commends_friendly, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_nPersonaDataPublicCommendsFriendly" ) );
    pnetvar ( int32_t, competitive_ranking, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_iCompetitiveRanking" ) );
    pnetvar ( int32_t, competitive_wins, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_iCompetitiveWins" ) );
    pnetvar ( int32_t, player_vip, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_iPlayerVIP" ) );
    pnetvar ( int32_t, mvps, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_iMVPs" ) );
    pnetvar ( int32_t, score, xor_str ( "DT_CSPlayerResource" ), xor_str ( "m_iScore" ) );
};

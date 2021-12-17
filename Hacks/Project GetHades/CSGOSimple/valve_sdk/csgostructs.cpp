#include "csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"

bool C_BaseEntity::is_player ( )
{
    //index: 152
    //ref: "effects/nightvision"
    //sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
    return CallVFunction< bool ( __thiscall*) ( C_BaseEntity* ) > ( this, vfindex::ccsplayer::is_player ) ( this );
}

Vector C_BaseEntity::get_bone_pos ( const int i )
{
    matrix3x4_t bone_matrix[128];

    if ( this->setup_bones ( bone_matrix, 128, bone_used_by_hitbox, 0.f ) )
        return Vector ( bone_matrix[ i ][ 0 ][ 3 ], bone_matrix[ i ][ 1 ][ 3 ], bone_matrix[ i ][ 2 ][ 3 ] );

    return Vector ( 0, 0, 0 );
}

Vector C_BaseEntity::get_head_pos ( )
{
    return this->get_bone_pos ( 6 );
}

bool C_BaseEntity::is_loot ( )
{
    return ( get_client_class( )->class_id == CPhysPropAmmoBox ||
        get_client_class( )->class_id == CPhysPropLootCrate ||
        get_client_class( )->class_id == CPhysPropRadarJammer ||
        get_client_class( )->class_id == CPhysPropWeaponUpgrade ||
        get_client_class( )->class_id == CDrone ||
        get_client_class( )->class_id == CDronegun ||
        get_client_class( )->class_id == CItem_Healthshot ||
        get_client_class( )->class_id == CItemCash );
}

bool C_BaseEntity::is_weapon ( )
{
    //index: 160
    //ref: "CNewParticleEffect::DrawModel"
    //sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
    return CallVFunction< bool ( __thiscall*) ( C_BaseEntity* ) > ( this, vfindex::ccscombat_weapon::is_weapon ) ( this );
}

bool C_BaseEntity::is_planted_c4 ( )
{
    return get_client_class( )->class_id == CPlantedC4;
}

bool C_BaseEntity::is_defuse_kit ( )
{
    return get_client_class( )->class_id == CBaseAnimating;
}

void C_BaseEntity::standard_blending_rules ( studiohdr_t* hdr, Vector* pos, quaternion_t* q, float curtime )
{
    using fn = void ( __fastcall*) ( studiohdr_t*, Vector*, quaternion_t*, float );
    reinterpret_cast< fn > ( reinterpret_cast< uintptr_t > ( this->get_client_renderable( ) ) + 804 ) ( hdr, pos, q, curtime );
}

CCSWeaponInfo* C_BaseCombatWeapon::get_cs_weapon_data ( )
{
    return CallVFunction< CCSWeaponInfo* ( __thiscall*) ( void* ) > ( this, vfindex::ccscombat_weapon::get_cs_weapon_data ) ( this );
    /*
    static auto fnGetWpnData
    = reinterpret_cast<CCSWeaponInfo*(__thiscall*)(void*)>(
    g_utils.PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B")
    );
    return fnGetWpnData(this);*/
}

bool C_BaseCombatWeapon::has_bullets ( )
{
    return !is_reloading( ) && clip1( ) > 0;
}

bool C_BaseCombatWeapon::can_fire ( )
{
    static decltype ( this ) stored_weapon = nullptr;
    static auto stored_tick = 0;
    static bool switched_weapons = false;
    static float switch_idle_time = 0.f;

    if ( stored_weapon != this || stored_tick >= g_local->tick_base( ) )
    {
        stored_weapon = this;
        stored_tick = g_local->tick_base( );
        switched_weapons = true;
        switch_idle_time = this->time_weapon_idle( );
        return false; //cannot shoot first tick after switch
    }

    if ( switched_weapons && this->time_weapon_idle( ) == switch_idle_time )
        return false;
    else
        switched_weapons = false;

    if ( is_reloading( ) || clip1( ) <= 0 || !g_local )
        return false;

    //auto flServerTime = g_local->tick_base() * interfaces::global_vars->interval_per_tick;

    return next_primary_attack( ) <= interfaces::global_vars->curtime;
}

bool C_BaseCombatWeapon::is_grenade ( )
{
    return get_cs_weapon_data( )->weapon_type == weapontype_grenade;
}

bool C_BaseCombatWeapon::is_gun ( )
{
    switch ( get_cs_weapon_data( )->weapon_type )
    {
    case weapontype_c4:
        return false;

    case weapontype_grenade:
        return false;

    case weapontype_knife:
        return false;

    case weapontype_unknown:
        return false;

    default:
        return true;
    }
}

bool C_BaseCombatWeapon::is_zeus ( )
{
    return this->item( ).item_definition_index( ) == WEAPON_ZEUS;
}

bool C_BaseCombatWeapon::is_knife ( )
{
    if ( this->item( ).item_definition_index( ) == WEAPON_ZEUS )
        return false;

    return get_cs_weapon_data( )->weapon_type == weapontype_knife;
}

bool C_BaseCombatWeapon::is_rifle ( )
{
    switch ( get_cs_weapon_data( )->weapon_type )
    {
    case weapontype_rifle:
        return true;

    case weapontype_submachinegun:
        return true;

    case weapontype_shotgun:
        return true;

    case weapontype_machinegun:
        return true;

    default:
        return false;
    }
}

bool C_BaseCombatWeapon::is_pistol ( )
{
    switch ( get_cs_weapon_data( )->weapon_type )
    {
    case weapontype_pistol:
        return true;

    default:
        return false;
    }
}

bool C_BaseCombatWeapon::is_sniper ( )
{
    switch ( get_cs_weapon_data( )->weapon_type )
    {
    case weapontype_sniper_rifle:
        return true;

    default:
        return false;
    }
}

bool C_BaseCombatWeapon::is_auto ( )
{
    switch ( this->item( ).item_definition_index( ) )
    {
    case WEAPON_SCAR20:
    case WEAPON_G3SG1:
        return true;

    default:
        return false;
    }
}

bool C_BaseCombatWeapon::is_scout ( )
{
    return this->item( ).item_definition_index( ) == WEAPON_SSG08;
}

bool C_BaseCombatWeapon::is_r8 ( )
{
    return this->item( ).item_definition_index( ) == WEAPON_REVOLVER;
}

bool C_BaseCombatWeapon::is_deagle ( )
{
    return this->item( ).item_definition_index( ) == WEAPON_DEAGLE;
}

bool C_BaseCombatWeapon::is_lmg ( )
{
    return this->get_cs_weapon_data( )->weapon_type == weapontype_machinegun;
}

bool C_BaseCombatWeapon::is_smg ( )
{
    return this->get_cs_weapon_data( )->weapon_type == weapontype_submachinegun;
}

bool C_BaseCombatWeapon::is_reloading ( )
{
    //GetModuleHandleA ( xor_str ( "test" ) );
    static auto in_reload = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                     xor_str ( "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90" ) ) + 2 );
    return * reinterpret_cast< bool* > ( reinterpret_cast< uintptr_t > ( this ) + in_reload );
}

float C_BaseCombatWeapon::get_inaccuracy ( )
{
    return CallVFunction< float ( __thiscall*) ( void* ) > ( this, vfindex::ccscombat_weapon::get_inaccuracy ) ( this );
}

float C_BaseCombatWeapon::get_spread ( )
{
    return CallVFunction< float ( __thiscall*) ( void* ) > ( this, vfindex::ccscombat_weapon::get_spread ) ( this );
}

void C_BaseCombatWeapon::update_accuracy_penalty ( )
{
    CallVFunction< void ( __thiscall*) ( void* ) > ( this, vfindex::ccscombat_weapon::update_accuracy_penalty ) ( this );
}

CUtlVector< IRefCounted* >& C_BaseCombatWeapon::custom_materials ( )
{
    static auto custom_materials = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                            xor_str ( "83 BE ? ? ? ? ? 7F 67" ) ) + 2 ) - 12;
    return * reinterpret_cast< CUtlVector< IRefCounted* >* > ( reinterpret_cast< uintptr_t > ( this ) + custom_materials );
}

bool* C_BaseCombatWeapon::custom_material_initialized ( )
{
    static auto custom_material_initialized = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                                       xor_str ( "C6 86 ? ? ? ? ? FF 50 04" ) ) + 2
    );
    return reinterpret_cast< bool* > ( reinterpret_cast< uintptr_t > ( this ) + custom_material_initialized );
}

usercmd_t*& C_BasePlayer::current_command ( )
{
    static auto current_command = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                           xor_str ( "89 BE ? ? ? ? E8 ? ? ? ? 85 FF" ) ) + 2 );
    return * reinterpret_cast< usercmd_t** > ( reinterpret_cast< uintptr_t > ( this ) + current_command );
}

int C_BasePlayer::get_num_anim_overlays ( )
{
    return * reinterpret_cast< int* > ( reinterpret_cast< DWORD > ( this ) + 0x297C );
}

animation_layer_t* C_BasePlayer::get_anim_overlays ( )
{
    // to find offset: use 9/12/17 dll
    // sig: 55 8B EC 51 53 8B 5D 08 33 C0
    return * reinterpret_cast< animation_layer_t** > ( reinterpret_cast< DWORD > ( this ) + 0x2990 );
}

animation_layer_t* C_BasePlayer::get_anim_overlay ( int i )
{
    if ( i < 15 )
        return &get_anim_overlays( )[ i ];

    return nullptr;
}

int C_BasePlayer::get_sequence_activity ( int sequence )
{
    const auto hdr = interfaces::mdl_info->get_studiomodel ( this->get_model( ) );

    if ( !hdr )
        return -1;

    // sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
    // sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
    // c_csplayer vfunc 242, follow calls to find the function.
    // #STR: "No model!", "Not Found!"
    static auto get_sequence_activity_call = reinterpret_cast< int ( __fastcall*) ( void*, studiohdr_t*, int ) > ( g_utils.pattern_scan (
        xor_str ( "client_panorama.dll" ), xor_str ( "55 8B EC 53 8B 5D 08 56 8B F1 83" ) ) );

    return get_sequence_activity_call ( this, hdr, sequence );
}

c_base_player_anim_state* C_BasePlayer::get_base_player_anim_state ( )
{
    return * reinterpret_cast< c_base_player_anim_state** > ( reinterpret_cast< DWORD > ( this ) + 0x3900 );
}

CCSPlayerAnimState* C_BasePlayer::get_player_anim_state ( )
{
    return * reinterpret_cast< CCSPlayerAnimState** > ( reinterpret_cast< DWORD > ( this ) + 0x3884 );
}

/*
bool C_BasePlayer::setup_bones ( matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
    static auto host_timescale = interfaces::cvar->find_var ( ( "host_timescale" ) );
    float OldCurTime = interfaces::global_vars->curtime;
    float OldRealTime = interfaces::global_vars->realtime;
    float OldFrameTime = interfaces::global_vars->frametime;
    float OldAbsFrameTime = interfaces::global_vars->absoluteframetime;
    float OldAbsFrameTimeStart = interfaces::global_vars->absoluteframestarttimestddev;
    float OldInterpAmount = interfaces::global_vars->interpolation_amount;
    int OldFrameCount = interfaces::global_vars->framecount;
    int OldTickCount = interfaces::global_vars->tickcount;

    interfaces::global_vars->curtime = this->simulation_time();
    interfaces::global_vars->realtime = this->simulation_time();
    interfaces::global_vars->frametime = interfaces::global_vars->interval_per_tick * host_timescale->get_float();
    interfaces::global_vars->absoluteframetime = interfaces::global_vars->interval_per_tick * host_timescale->get_float();
    interfaces::global_vars->absoluteframestarttimestddev = this->simulation_time() - interfaces::global_vars->interval_per_tick * host_timescale->get_float();
    interfaces::global_vars->interpolation_amount = 0;
    interfaces::global_vars->framecount = time_to_ticks ( this->simulation_time() );
    interfaces::global_vars->tickcount = time_to_ticks ( this->simulation_time() );

    * ( int* ) ( ( int )this + 236 ) |= 8; // IsNoInterpolationFrame
    bool returnVal = this->SetupBones ( pBoneToWorldOut, nMaxBones, boneMask, currentTime );
    * ( int* ) ( ( int )this + 236 ) &= ~8; // (1 << 3)

    interfaces::global_vars->curtime = OldCurTime;
    interfaces::global_vars->realtime = OldRealTime;
    interfaces::global_vars->frametime = OldFrameTime;
    interfaces::global_vars->absoluteframetime = OldAbsFrameTime;
    interfaces::global_vars->absoluteframestarttimestddev = OldAbsFrameTimeStart;
    interfaces::global_vars->interpolation_amount = OldInterpAmount;
    interfaces::global_vars->framecount = OldFrameCount;
    interfaces::global_vars->tickcount = OldTickCount;

    return returnVal;
}
*/

void C_BasePlayer::update_animation_state ( CCSGOPlayerAnimState* state, QAngle angle )
{
    static auto update_anim_state = g_utils.pattern_scan (
        xor_str ( "client_panorama.dll" ), xor_str ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) );

    /*static auto UpdateAnimState = g_utils.PatternScan(
    GetModuleHandle(L"client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
    */
    if ( !update_anim_state )
        return;

    __asm
    {
        push 0
    }

    __asm
    {
        mov ecx, state

        movss xmm1, dword ptr[angle + 4]
        movss xmm2, dword ptr[angle]

        call update_anim_state
    }
}

void C_BasePlayer::reset_animation_state ( CCSGOPlayerAnimState* state )
{
    using reset_anim_state_t = void ( __thiscall*) ( CCSGOPlayerAnimState* );
    static auto reset_anim_state = reinterpret_cast< reset_anim_state_t > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                                   xor_str ( "56 6A 01 68 ? ? ? ? 8B F1" ) ) );

    if ( !reset_anim_state )
        return;

    reset_anim_state ( state );
}

void C_BasePlayer::create_animation_state ( CCSGOPlayerAnimState* state )
{
    using create_anim_state_t = void ( __thiscall*) ( CCSGOPlayerAnimState*, C_BasePlayer* );
    static auto create_anim_state = reinterpret_cast< create_anim_state_t > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                                     xor_str ( "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) ) );

    if ( !create_anim_state )
        return;

    create_anim_state ( state, this );
}

void C_BasePlayer::modify_eye_pos ( c_base_player_anim_state* animstate, vector_t* pos )
{
    /// Unneeded but whatever
    if ( interfaces::engine_client->is_hltv( ) || interfaces::engine_client->is_playing_demo( ) )
        return;

    static auto lookup_bone_fn = reinterpret_cast< int ( __thiscall*) ( void*, const char* ) > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                                                        xor_str (
                                                                                                                            "55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14" ) )
    );

    if ( *reinterpret_cast< bool* > ( uintptr_t ( animstate ) + 0x109 ) /// in hitground
        && animstate->m_fDuckAmount != 0.f /// unk
        && animstate->pBaseEntity
        && reinterpret_cast< C_BasePlayer* > ( animstate->pBaseEntity )->ground_entity( )
        && reinterpret_cast< C_BasePlayer* > ( animstate->pBaseEntity )->ground_entity( ).get( )
    )
    {
        auto base_entity = animstate->pBaseEntity;

        auto bone_pos = get_bone_pos ( lookup_bone_fn ( base_entity, xor_str ( "head_0" ) ) );

        bone_pos.z += 1.7f;

        if ( ( *pos ).z > bone_pos.z )
        {
            float some_factor = 0.f;

            float delta = ( *pos ).z - bone_pos.z;

            float some_offset = ( delta - 4.f ) / 6.f;

            if ( some_offset >= 0.f )
                some_factor = std::fminf ( some_offset, 1.f );

            ( *pos ).z += ( ( bone_pos.z - ( *pos ).z ) * ( ( ( some_factor * some_factor ) * 3.f ) - ( ( ( some_factor * some_factor ) * 2.f ) * some_factor
            ) ) );
        }
    }
}

Vector C_BasePlayer::get_eye_pos ( )
{
    //static auto weapon_shoot_position = reinterpret_cast<float* ( __thiscall* ) ( void*, Vector* )> (
    //                                        g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "55 8B EC 56 8B 75 ? 57 8B F9 56 8B 07 FF 90" ) )
    //                                    );

    //Vector output;
    //weapon_shoot_position ( this, &output );

    //return output;

    auto pos = vector_t{ };
    CallVFunction< void ( __thiscall*) ( void*, vector_t* ) > ( this, 166 ) ( this, &pos );

    if ( *reinterpret_cast< int32_t* > ( uintptr_t ( this ) + 0x3AB0 ) )
    {
        auto anim_state = *reinterpret_cast< void** > ( uintptr_t ( this ) + 0x3900 );

        if ( anim_state )
            modify_eye_pos ( reinterpret_cast< c_base_player_anim_state* > ( anim_state ), &pos );
    }

    return pos;
}

vector_t C_BasePlayer::get_eye_pos ( void* animstate )
{
    auto pos = vector_t{ };
    CallVFunction< void ( __thiscall*) ( void*, vector_t* ) > ( this, 166 ) ( this, &pos );

    if ( animstate )
        modify_eye_pos ( reinterpret_cast< c_base_player_anim_state* > ( animstate ), &pos );

    return pos;
}

player_info_t C_BasePlayer::get_player_info ( ) const
{
    player_info_t info;
    interfaces::engine_client->get_player_info ( ent_index( ), &info );
    return info;
}

bool C_BasePlayer::is_alive ( ) const
{
    return e_state( ) == E_ALIVE || this->health( ) > 0;
}

bool C_BasePlayer::is_flashed ( )
{
    static auto flash_max_alpha = g_netvarsys.GetOffset ( xor_str ( "DT_CSPlayer" ), xor_str ( "m_flFlashMaxAlpha" ) );
    return * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( this ) + flash_max_alpha - 0x8 ) > 200.0;
}

bool C_BasePlayer::has_c4 ( )
{
    static auto fn_has_c4
        = reinterpret_cast< bool ( __thiscall*) ( void* ) > (
            g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "56 8B F1 85 F6 74 31" ) )
            //56 8B F1 85 F6 74 ? 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 ? 8A 86 ? ? ? ? 84 C0 74 ? 8B 06 8B CE 6A ?
        );

    return fn_has_c4 ( this );
}

Vector C_BasePlayer::get_hitbox_pos ( const int hitbox_id )
{
    matrix3x4_t bone_matrix[maxstudiobones];

    if ( setup_bones ( bone_matrix, maxstudiobones, bone_used_by_hitbox, 0.0f ) )
    {
        auto studio_model = interfaces::mdl_info->get_studiomodel ( get_model( ) );

        if ( studio_model )
        {
            const auto hitbox = studio_model->get_hitbox_set ( 0 )->get_hitbox ( hitbox_id );

            if ( hitbox )
            {
                auto
                    min = Vector{ },
                    max = Vector{ };

                g_math.vector_transform ( hitbox->bbmin, bone_matrix[ hitbox->bone ], min );
                g_math.vector_transform ( hitbox->bbmax, bone_matrix[ hitbox->bone ], max );

                return ( min + max ) / 2.0f;
            }
        }
    }

    return Vector{ };
}

mstudiobbox_t* C_BasePlayer::get_hitbox ( const int hitbox_id )
{
    matrix3x4_t bone_matrix[maxstudiobones];

    if ( setup_bones ( bone_matrix, maxstudiobones, bone_used_by_hitbox, 0.0f ) )
    {
        auto studio_model = interfaces::mdl_info->get_studiomodel ( get_model( ) );

        if ( studio_model )
        {
            const auto hitbox = studio_model->get_hitbox_set ( 0 )->get_hitbox ( hitbox_id );

            if ( hitbox )
            {
                return hitbox;
                //auto
                //	min = Vector{},
                //	max = Vector{};

                //g_math.VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
                //g_math.VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

                //return (min + max) / 2.0f;
            }
        }
    }

    return nullptr;
}

bool C_BasePlayer::get_hitbox_pos ( int hitbox, Vector& output )
{
    if ( hitbox >= hitbox_max )
        return false;

    const auto model = this->get_model( );

    if ( !model )
        return false;

    auto studio_hdr = interfaces::mdl_info->get_studiomodel ( model );

    if ( !studio_hdr )
        return false;

    matrix3x4_t matrix[maxstudiobones];

    if ( !this->setup_bones ( matrix, maxstudiobones, 0x100, 0 ) )
        return false;

    const auto studio_box = studio_hdr->get_hitbox_set ( 0 )->get_hitbox ( hitbox );

    if ( !studio_box )
        return false;

    Vector min, max;

    g_math.vector_transform ( studio_box->bbmin, matrix[ studio_box->bone ], min );
    g_math.vector_transform ( studio_box->bbmax, matrix[ studio_box->bone ], max );

    output = ( min + max ) * 0.5f;

    return true;
}

Vector C_BasePlayer::get_bone_pos ( int bone )
{
    matrix3x4_t bone_matrix[maxstudiobones];

    if ( setup_bones ( bone_matrix, maxstudiobones, bone_used_by_anything, 0.0f ) )
        return bone_matrix[ bone ].at ( 3 );

    return Vector{ };
}

bool C_BasePlayer::can_see_player ( C_BasePlayer* player, int hitbox )
{
    if ( !interfaces::engine_trace )
        return false;

    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    const auto endpos = player->get_hitbox_pos ( hitbox );

    ray.Init ( get_eye_pos( ), endpos );
    interfaces::engine_trace->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

    return tr.hit_entity == player || tr.fraction > 0.97f;
}

bool C_BasePlayer::can_see_player ( C_BasePlayer* player, const Vector& pos )
{
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    //auto start = GetEyePos();
    //auto dir = (pos - start).Normalized();

    ray.Init ( get_eye_pos( ), pos );
    interfaces::engine_trace->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

    return tr.hit_entity == player || tr.fraction > 0.97f;
}

void C_BasePlayer::update_client_side_animation ( )
{
    //static auto addr = reinterpret_cast<uintptr_t> ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ), xor_str ( "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36" ) ) );
    //return void (__thiscall*)
    return CallVFunction< void ( __thiscall*) ( void* ) > ( this, vfindex::ccsplayer::update_client_side_animations ) ( this );
}

void C_BasePlayer::set_angle2 ( const QAngle wantedang )
{
    typedef void ( __thiscall * set_angle_fn ) ( void*, const QAngle& );
    static set_angle_fn set_angle2 = reinterpret_cast< set_angle_fn > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                               xor_str ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) ) );
    set_angle2 ( this, wantedang );
}

void C_BasePlayer::invalidate_bone_cache ( )
{
    static auto invalidate_bone_cache_fn = g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                  xor_str ( "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81" ) );
    reinterpret_cast< void ( __fastcall*) ( void* ) > ( invalidate_bone_cache_fn ) ( this );
}

//int C_BasePlayer::move_type()
//{
//    return * reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( this ) + 0x25C );
//}

QAngle* C_BasePlayer::get_vangles ( )
{
    static auto deadflag = g_netvarsys.GetOffset ( xor_str ( "DT_BasePlayer" ), xor_str ( "deadflag" ) );
    return reinterpret_cast< QAngle* > ( reinterpret_cast< uintptr_t > ( this ) + deadflag + 0x4 );
}

void C_BaseAttributableItem::set_glove_model_index ( const int model_index )
{
    return CallVFunction< void ( __thiscall*) ( void*, int ) > ( this, 75 ) ( this, model_index );
}

float C_BasePlayer::get_flash_bang_time ( )
{
    static uint32_t get_flash_bang_time = * reinterpret_cast< uint32_t* > ( reinterpret_cast< uint32_t > ( g_utils.pattern_scan (
        xor_str ( "client_panorama.dll" ),
        xor_str ( "F3 0F 10 86 ?? ?? ?? ?? 0F 2F 40 10 76 30" ) ) ) + 4 );
    return * reinterpret_cast< float* > ( this + get_flash_bang_time );
    //return *(float*)((uintptr_t)this + 0xa308);
}

void C_BaseViewModel::send_view_model_matching_sequence ( int sequence )
{
    return CallVFunction< void ( __thiscall*) ( void*, int ) > ( this, 241 ) ( this, sequence );
}

float c_base_player_anim_state::max_rotation ( )
{
    const auto animstate = reinterpret_cast< uintptr_t > ( this );

    if ( !animstate )
        return 58.f;

    const auto duckammount = * reinterpret_cast< float* > ( animstate + 164 );
    const auto speedfraction_clamped = std::max ( 0.f, std::min ( *reinterpret_cast< float* > ( animstate + 248 ), 1.f ) );

    const auto speedfactor = std::max ( 0.f, std::min ( 1.f, *reinterpret_cast< float* > ( animstate + 252 ) ) );

    float unk1 = ( ( ( *reinterpret_cast< float* > ( animstate + 284 ) * -0.30000001 ) - 0.19999999 ) * speedfraction_clamped ) + 1.f;

    if ( duckammount > 0 )
        unk1 += ( ( duckammount * speedfactor ) * ( 0.5f - unk1 ) );

    return *reinterpret_cast< float* > ( animstate + 820 ) * unk1;
}

/*
float C_BaseViewModel::sequence_duration ( int s )
{
    static auto sequence_duration
        = reinterpret_cast<float ( __thiscall* ) ( void*, int )> (
              g_utils.pattern_scan ( GetModuleHandleA ( xor_str ( "client_panorama.dll" ) ),
                                     xor_str ( "55 8B EC 56 8B F1 83 BE ? ? ? ? ? 75 ? 8B 46 ? 8D 4E ? FF 50 ? 85 C0 74 ? 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 ? 83 39 ? 74 ? 8B 41 ?" )
                                   )
          );
    return sequence_duration ( this, s );
    //55 8B EC 56 8B F1 83 BE ? ? ? ? ? 75 ? 8B 46 ? 8D 4E ? FF 50 ? 85 C0 74 ? 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 ? 83 39 ? 74 ? 8B 41 ?
    //return 0.0f;
}
*/

CUtlVector< IRefCounted* >& C_EconItemView::custom_materials ( )
{
    return * reinterpret_cast< CUtlVector< IRefCounted* >* > ( reinterpret_cast< uintptr_t > ( this ) + 0x14 );
}

CUtlVector< IRefCounted* >& C_EconItemView::visuals_data_processors ( )
{
    static auto in_reload = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                     xor_str ( "81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C 24 0C" ) ) + 2 );
    return * reinterpret_cast< CUtlVector< IRefCounted* >* > ( reinterpret_cast< uintptr_t > ( this ) + in_reload );
}

float C_BasePlayer::get_max_desync_delta_inverted ( )
{
    const auto animstate = reinterpret_cast< uintptr_t > ( this->get_base_player_anim_state( ) );

    if ( !animstate )
        return 0.f;

    const auto duckammount = * reinterpret_cast< float* > ( animstate + 0xA4 );
    const auto speedfraction_clamped = std::max ( 0.f, std::min ( *reinterpret_cast< float* > ( animstate + 0xF8 ), 1.f ) );

    const auto speedfactor = std::max ( 0.f, std::min ( 1.f, *reinterpret_cast< float* > ( animstate + 0xFC ) ) );

    auto unk1 = ( ( ( *reinterpret_cast< float* > ( animstate + 0x11C ) * -0.30000001 ) - 0.19999999 ) * speedfraction_clamped ) + 1.f;

    if ( duckammount > 0 )
        unk1 += ( ( duckammount * speedfactor ) * ( 0.5f - unk1 ) );

    return *reinterpret_cast< float* > ( animstate + 0x330 ) * unk1;
}

float C_BasePlayer::get_max_desync_delta ( )
{
    const auto animstate = reinterpret_cast< uintptr_t > ( this->get_base_player_anim_state( ) );

    if ( !animstate )
        return 58.f;

    const auto duckammount = * reinterpret_cast< float* > ( animstate + 164 );
    const auto speedfraction_clamped = std::max ( 0.f, std::min ( *reinterpret_cast< float* > ( animstate + 248 ), 1.f ) );

    const auto speedfactor = std::max ( 0.f, std::min ( 1.f, *reinterpret_cast< float* > ( animstate + 252 ) ) );

    float unk1 = ( ( ( *reinterpret_cast< float* > ( animstate + 284 ) * -0.30000001 ) - 0.19999999 ) * speedfraction_clamped ) + 1.f;

    if ( duckammount > 0 )
        unk1 += ( ( duckammount * speedfactor ) * ( 0.5f - unk1 ) );

    return *reinterpret_cast< float* > ( animstate + 820 ) * unk1;
}

float C_BasePlayer::get_yaw_feet_delta ( )
{
    auto animstate = this->get_base_player_anim_state( );

    if ( !animstate )
        return -1.f;

    return g_math.angle_diff ( *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( animstate ) + 0x78 ), animstate->m_flGoalFeetYaw );
}

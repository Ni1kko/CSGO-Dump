#include "autowall.h"
#include "../../../helpers/math.hpp"
#include "../../features.h"

bool CGameTrace::did_hit_world ( ) const
{
    return hit_entity == interfaces::entity_list->get_client_entity ( 0 );
}

bool CGameTrace::did_hit_non_world_entity ( ) const
{
    return hit_entity != nullptr && !did_hit_world( );
}

bool c_autowall::trace_to_exit_short ( Vector& point, Vector& dir, const float step_size, float max_distance ) const
{
    float distance = 0;

    while ( distance <= max_distance )
    {
        distance += step_size;

        point += dir * distance;
        const auto point_contents = interfaces::engine_trace->get_point_contents ( point, MASK_SHOT_HULL );

        if ( ! ( point_contents & MASK_SHOT_HULL ) )
        {
            // found first free point
            return true;
        }
    }

    return false;
}

void c_autowall::trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr )
{
    CTraceFilter filter;
    filter.pSkip = ignore;
    Ray_t ray;
    ray.Init ( vec_abs_start, vec_abs_end );
    interfaces::engine_trace->trace_ray ( ray, mask, &filter, ptr );
}

bool c_autowall::vectorto_vector_visible ( Vector src, Vector point )
{
    CGameTrace trace_init;
    trace_line ( src, point, MASK_SOLID, g_local, &trace_init );
    CGameTrace trace;
    trace_line ( src, point, MASK_SOLID, reinterpret_cast< C_BasePlayer* > ( trace_init.hit_entity ), &trace );

    return trace.fraction == 1.0f || trace_init.fraction == 1.0f;
}

float c_autowall::get_hitgroup_damage_mult ( int iHitGroup )
{
    switch ( iHitGroup )
    {
    case hitgroup_head:
        return 4.f;

    case hitgroup_stomach:
        return 1.25f;

    case hitgroup_leftleg:
    case hitgroup_rightleg:
        return 0.75f;

    default:
        break;
    }

    return 1.0f;
}

void c_autowall::scale_damage ( int hitgroup, C_BasePlayer* enemy, float weapon_armor_ratio, float& current_damage )
{
    if ( hitbox_overwritten_ )
    {
        switch ( this->hitbox_ )
        {
        case hitbox_head:
            hitgroup = hitgroup_head;
            break;

        case hitbox_neck:
            hitgroup = hitgroup_generic;
            break;

        case hitbox_chest:
        case hitbox_lower_chest:
        case hitbox_upper_chest:
            hitgroup = hitgroup_chest;
            break;

        case hitbox_stomach:
        case hitbox_pelvis:
            hitgroup = hitgroup_stomach;
            break;

        case hitbox_left_calf:
        case hitbox_left_foot:
            hitgroup = hitgroup_leftleg;
            break;

        case hitbox_right_calf:
        case hitbox_right_foot:
            hitgroup = hitgroup_rightleg;
            break;

        case hitbox_right_upper_arm:
        case hitbox_right_forearm:
            hitgroup = hitgroup_rightarm;
            break;

        case hitbox_left_upper_arm:
        case hitbox_left_forearm:
            hitgroup = hitgroup_leftarm;
            break;

        default:
            hitgroup = hitgroup_generic;
        }
    }

    current_damage *= get_hitgroup_damage_mult ( hitgroup );

    if ( enemy->armor_value( ) > 0.0f && hitgroup < hitgroup_leftleg )
    {
        if ( hitgroup == hitgroup_head && !enemy->has_helmet( ) )
            return;

        float armorscaled = weapon_armor_ratio * 0.5f * current_damage;

        if ( ( current_damage - armorscaled ) * 0.5f > enemy->armor_value( ) )
            armorscaled = current_damage - enemy->armor_value( ) * 2.0f;

        current_damage = armorscaled;
    }
}

static auto util_cliptrace_address = reinterpret_cast< uintptr_t > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                            xor_str (
                                                                                                "53 8b dc 83 ec 08 83 e4 f0 83 c4 04 55 8b 6b 04 89 6c 24 04 8b ec 81 ec ? ? ? ? 8b 43 10"
                                                                                            ) ) );

void c_autowall::util_clip_trace_to_players ( const Vector& vec_abs_start, const Vector& vec_abs_end, unsigned int mask, ITraceFilter* filter,
                                              CGameTrace* tr ) const
{
    if ( !util_cliptrace_address )
        return;

    _asm
    {
        MOV EAX, filter
        LEA ECX, tr
        PUSH ECX
        PUSH EAX
        PUSH mask
        LEA EDX, vec_abs_end
        LEA ECX, vec_abs_start
        CALL util_cliptrace_address
        ADD ESP, 0xC
    }
}

bool c_autowall::simulate_fire_bullet ( C_BaseEntity* local, C_BaseCombatWeapon* weapon, fire_bullet_data& data )
{
    data.penetrate_count = 4;
    data.trace_length = 0.0f;
    auto* wpn_data = weapon->get_cs_weapon_data( );
    data.current_damage = static_cast< float > ( wpn_data->damage );

    while ( data.penetrate_count > 0 && data.current_damage >= 1.0f )
    {
        data.trace_length_remaining = wpn_data->range - data.trace_length;
        auto end_point = data.src + data.direction * data.trace_length_remaining;
        trace_line ( data.src, end_point, 0x4600400B, local, &data.enter_trace );
        util_clip_trace_to_players ( data.src, end_point * 40.f, 0x4600400B, &data.filter, &data.enter_trace );

        if ( data.enter_trace.fraction == 1.0f )
            break;

        if ( data.enter_trace.hitgroup <= 7 && data.enter_trace.hitgroup > 0 && local->team_num( ) != reinterpret_cast< C_BasePlayer* > ( data
                                                                                                                                          .enter_trace.
                                                                                                                                          hit_entity )->
            team_num( ) )
        {
            data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
            data.current_damage *= pow ( wpn_data->range_modifier, data.trace_length * 0.002 );
            scale_damage ( data.enter_trace.hitgroup, reinterpret_cast< C_BasePlayer* > ( data.enter_trace.hit_entity ), wpn_data->armor_ratio,
                           data.current_damage );
            return true;
        }

        if ( !handle_bullet_penetration ( wpn_data, data, false, Vector ( 0, 0, 0 ) ) )
            break;
    }

    return false;
}

static auto o_trace_to_exit = reinterpret_cast< uintptr_t > ( g_utils.pattern_scan (
    xor_str ( "client_panorama.dll" ),
    xor_str ( "55 8B EC 83 EC 30 F3 0F 10 75" ) ) );

bool c_autowall::trace_to_exitalt ( Vector& end, CGameTrace& tr, Vector start, Vector v_end, CGameTrace* trace ) const
{
    //typedef bool ( __fastcall * trace_to_exit_fn ) ( Vector&, CGameTrace&, float, float, float, float, float, float, CGameTrace* );

    if ( !o_trace_to_exit )
        return false;

    //return oTraceToExit ( end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace );
    //float x = start.x, y = start.y, z = start.z;
    //float x2 = vEnd.x, y2 = vEnd.y, z2 = vEnd.z;
    _asm
    {
        push trace
        push v_end.z
        push v_end.y
        push v_end.x
        push start.z
        push start.y
        push start.x
        mov edx, tr
        mov ecx, end
        call o_trace_to_exit
        add esp, 0x1C
    }
}

bool c_autowall::handle_bullet_penetration ( CCSWeaponInfo* wpn_data, fire_bullet_data& data, bool extracheck, Vector point )
{
    const auto enter_surface_data = interfaces::phys_surface->get_surface_data ( data.enter_trace.surface.surfaceProps );
    const int enter_material = enter_surface_data->game.material;
    const auto enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
    data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
    data.current_damage *= pow ( wpn_data->range_modifier, data.trace_length * 0.002 );

    if ( data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f )
        data.penetrate_count = 0;

    if ( data.penetrate_count <= 0 )
        return false;

    Vector dummy;
    CGameTrace trace_exit;

    if ( !trace_to_exitalt ( dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit ) )
        return false;

    const auto exit_surface_data = interfaces::phys_surface->get_surface_data ( trace_exit.surface.surfaceProps );
    const auto exit_material = exit_surface_data->game.material;
    const auto exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
    auto final_damage_modifier = 0.16f;
    float combined_penetration_modifier;

    if ( ( data.enter_trace.contents & CONTENTS_GRATE ) != 0 || enter_material == 89 || enter_material == 71 )
    {
        combined_penetration_modifier = 3.0f;
        final_damage_modifier = 0.05f;
    }
    else
        combined_penetration_modifier = ( enter_surf_penetration_mod + exit_surf_penetration_mod ) * 0.5f;

    if ( enter_material == exit_material )
    {
        if ( exit_material == 87 || exit_material == 85 )
            combined_penetration_modifier = 3.0f;
        else if ( exit_material == 76 )
            combined_penetration_modifier = 2.0f;
    }

    const auto v34 = fmaxf ( 0.f, 1.0f / combined_penetration_modifier );
    const auto v35 = data.current_damage * final_damage_modifier + v34 * 3.0f * fmaxf ( 0.0f, 3.0f / wpn_data->penetration * 1.25f );
    auto thickness = ( trace_exit.endpos - data.enter_trace.endpos ).length( );

    if ( extracheck )
        if ( !vectorto_vector_visible ( trace_exit.endpos, point ) )
            return false;

    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;
    const auto lost_damage = fmaxf ( 0.0f, v35 + thickness );

    if ( lost_damage > data.current_damage )
        return false;

    if ( lost_damage >= 0.0f )
        data.current_damage -= lost_damage;

    if ( data.current_damage < 1.0f )
        return false;

    data.src = trace_exit.endpos;
    data.penetrate_count--;

    return true;
}

float c_autowall::damage ( const Vector& point )
{
    fire_bullet_data data;
    data.filter.pSkip = g_local;
    data.src = g_local->get_eye_pos ( g_features.animations.m_real_state );

    const auto angles = g_math.calc_angle ( data.src, point );
    g_math.angle_vectors ( angles, data.direction );
    // ReSharper disable CppExpressionWithoutSideEffects
    data.direction.normalize_in_place( );

    if ( simulate_fire_bullet ( g_local, g_local->active_weapon( ), data ) )
        return data.current_damage;

    return -1.f;
}

void c_autowall::overwrite_hitbox ( int hitbox )
{
    hitbox_overwritten_ = true;
    this->hitbox_ = hitbox;
}

void c_autowall::reset_overwrite ( )
{
    this->hitbox_overwritten_ = false;
}

float c_autowall::get_thickness ( Vector& start, Vector& end, const float distance ) const
{
    auto dir = end - start;
    auto step = start;

    if ( dir.length( ) > distance && distance != -1.f )
        return -1.f;

    dir.normalize_in_place( );
    CTraceFilterWorldOnly filter;
    trace_t trace;
    Ray_t ray;
    float thickness = 0;

    while ( true )
    {
        ray.Init ( step, end );
        //TraceRay ( ray, MASK_SHOT_HULL, &filter, &trace );
        interfaces::engine_trace->trace_ray ( ray, MASK_SHOT_HULL, &filter, &trace );

        if ( !trace.DidHit( ) )
            return thickness;

        const auto last_step = trace.endpos;
        step = trace.endpos;

        if ( ( end - start ).length( ) <= ( step - start ).length( ) )
            break;

        if ( !trace_to_exit_short ( step, dir, 5, 150.f ) )
            return FLT_MAX;

        thickness += ( step - last_step ).length( );
    }

    return thickness;
}

bool c_autowall::can_hit_point ( const Vector& point, const Vector& source )
{
    if ( !g_local )
        return false;

    fire_bullet_data data; // = FireBulletData ( source, g::pLocalEntity );
    data.src = source;
    data.filter.pSkip = g_local;

    const auto angles = g_math.calc_angle ( data.src, point );
    g_math.angle_vectors ( angles, data.direction );
    // ReSharper disable CppExpressionWithoutSideEffects
    data.direction.normalize_in_place( );
    // ReSharper restore CppExpressionWithoutSideEffects

    C_BaseCombatWeapon* weapon = static_cast< C_BaseCombatWeapon* > ( g_local->active_weapon( ) );

    if ( !weapon )
        return false;

    data.penetrate_count = 1;
    data.trace_length = 0.0f;

    const auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return false;

    data.current_damage = static_cast< float > ( weapon_data->damage );
    data.trace_length_remaining = weapon_data->range - data.trace_length;
    Vector end = data.src + data.direction * data.trace_length_remaining;
    trace_line ( data.src, end, MASK_SHOT | CONTENTS_HITBOX, g_local, &data.enter_trace );

    return vectorto_vector_visible ( data.src, point ) || handle_bullet_penetration ( weapon_data, data, true, point );
}

bool c_autowall::can_hit_point ( const Vector& point, const Vector& source, C_BaseEntity* skip )
{
    if ( !g_local || !skip )
        return false;

    fire_bullet_data data; // = FireBulletData ( source, g::pLocalEntity );
    data.src = source;
    data.filter.pSkip = skip;

    CTraceFilterSkipTwoEntities filter ( skip, g_local );
    const auto angles = g_math.calc_angle ( data.src, point );
    g_math.angle_vectors ( angles, data.direction );
    // ReSharper disable CppExpressionWithoutSideEffects
    data.direction.normalize_in_place( );
    // ReSharper restore CppExpressionWithoutSideEffects

    C_BaseCombatWeapon* weapon = static_cast< C_BaseCombatWeapon* > ( g_local->active_weapon( ) );

    if ( !weapon )
        return false;

    data.penetrate_count = 1;
    data.trace_length = 0.0f;

    const auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return false;

    data.current_damage = static_cast< float > ( weapon_data->damage );
    data.trace_length_remaining = weapon_data->range - data.trace_length;
    Vector end = data.src + data.direction * data.trace_length_remaining;
    trace_line ( data.src, end, MASK_SHOT | CONTENTS_HITBOX, g_local, &data.enter_trace );

    return vectorto_vector_visible ( data.src, point ) || handle_bullet_penetration ( weapon_data, data, true, point );
}

float c_autowall::can_hit_point_dmg ( const Vector& point, const Vector& source )
{
    if ( !g_local )
        return false;

    fire_bullet_data data; // = FireBulletData ( source, g::pLocalEntity );
    data.src = source;
    data.filter.pSkip = g_local;

    const auto angles = g_math.calc_angle ( data.src, point );
    g_math.angle_vectors ( angles, data.direction );
    // ReSharper disable once CppExpressionWithoutSideEffects
    data.direction.normalize_in_place( );

    C_BaseCombatWeapon* weapon = static_cast< C_BaseCombatWeapon* > ( g_local->active_weapon( ) );

    if ( !weapon )
        return false;

    data.penetrate_count = 1;
    data.trace_length = 0.0f;

    CCSWeaponInfo* weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return false;

    data.current_damage = static_cast< float > ( weapon_data->damage );
    data.trace_length_remaining = weapon_data->range - data.trace_length;
    Vector end = data.src + data.direction * data.trace_length_remaining;
    trace_line ( data.src, end, MASK_SHOT | CONTENTS_HITBOX, g_local, &data.enter_trace );

    if ( simulate_fire_bullet ( g_local, g_local->active_weapon( ), data ) )
        return data.current_damage;

    return -1.f;
}

float c_autowall::can_hit_point_dmg ( const Vector& point, C_BasePlayer* attacker )
{
    if ( !attacker )
        return -1.f;

    fire_bullet_data data; // = FireBulletData ( source, g::pLocalEntity );
    data.src = attacker->get_eye_pos( );
    data.filter.pSkip = attacker;

    const auto angles = g_math.calc_angle ( data.src, point );
    g_math.angle_vectors ( angles, data.direction );
    // ReSharper disable CppExpressionWithoutSideEffects
    data.direction.normalize_in_place( );
    // ReSharper restore CppExpressionWithoutSideEffects

    C_BaseCombatWeapon* weapon = static_cast< C_BaseCombatWeapon* > ( attacker->active_weapon( ) );

    if ( !weapon )
        return -1.f;

    data.penetrate_count = 1;
    data.trace_length = 0.0f;

    const auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return -1.f;

    data.current_damage = static_cast< float > ( weapon_data->damage );
    data.trace_length_remaining = weapon_data->range - data.trace_length;
    Vector end = data.src + data.direction * data.trace_length_remaining;
    trace_line ( data.src, end, MASK_SHOT | CONTENTS_HITBOX, attacker, &data.enter_trace );

    return handle_bullet_penetration ( weapon_data, data, true, point ) ? data.current_damage : -1.f;
}

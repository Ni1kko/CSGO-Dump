//#include "event_logger.h"
//#include "../helpers/debug_console.h"
//#include "../renderer/vgui_render.h"
//#include "../helpers/math.hpp"
//#include "../helpers/console.h"
//#include "../options.h"
//#include "../security/security.h"
//#include "features.h"
//
//struct shotdata
//{
//    int shots = 0;
//};
////shotdata shots_data[64];
//
//void c_event_logger::on_fire_event ( IGameEvent* event )
//{
//    if ( !strcmp ( event->get_name(), xor_str ( "bullet_impact" ) ) )
//    {
//        const auto pos = Vector ( event->get_float ( xor_str ( "x" ) ), event->get_float ( xor_str ( "y" ) ), event->get_float ( xor_str ( "z" ) ) );
//        const auto userid = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//
//        if ( userid == interfaces::engine_client->get_localplayer() && g_local )
//        {
//            static auto last_shot_from = g_local->get_eye_pos();
//            static auto last_curtime = interfaces::global_vars->curtime;
//
//            if ( last_curtime != interfaces::global_vars->curtime )
//            {
//                last_shot_from = g_local->get_eye_pos();
//                last_curtime = interfaces::global_vars->curtime;
//            }
//
//            draw_beam ( last_shot_from, Color::Green, pos );
//        }
//        else
//        {
//            C_BasePlayer* pla = dynamic_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( userid ) );
//
//            if ( pla && pla->is_alive() && !pla->is_dormant() )
//            {
//                if ( pla->is_enemy() )
//                {
//                    static auto last_shot_from = pla->get_eye_pos();
//                    static auto last_curtime = interfaces::global_vars->curtime;
//
//                    if ( last_curtime != interfaces::global_vars->curtime )
//                    {
//                        last_shot_from = pla->get_eye_pos();
//                        last_curtime = interfaces::global_vars->curtime;
//                    }
//
//                    draw_beam ( last_shot_from, Color::Red, pos );
//                }
//                else
//                {
//                    static auto last_shot_from = pla->get_eye_pos();
//                    static auto last_curtime = interfaces::global_vars->curtime;
//
//                    if ( last_curtime != interfaces::global_vars->curtime )
//                    {
//                        last_shot_from = pla->get_eye_pos();
//                        last_curtime = interfaces::global_vars->curtime;
//                    }
//
//                    draw_beam ( last_shot_from, Color::Blue, pos );
//                }
//            }
//        }
//
//        if ( rbot_shot_data.size() == 0 )
//            return;
//
//        const auto size = last_events.size();
//
//        for ( auto& iter : rbot_shot_data )
//        {
//            if ( iter.did_bullet_impact )
//                continue;
//
//            if ( size == 0 )
//            {
//                event_data d;
//                d.hurt_player = interfaces::global_vars->curtime - iter.shot_time < 2.f ? iter.index : -1;
//                d.pos = pos;
//                d.userid = userid;
//                d.hurt = false;
//                d.time = interfaces::global_vars->curtime;
//                d.got_pos = true;
//                last_events.push_back ( d );
//            }
//            else
//            {
//                if ( last_events[size - 1].time == interfaces::global_vars->curtime && last_events[size - 1].hurt_player == iter.index )
//                    last_events[size - 1].pos = pos;
//                else
//                {
//                    event_data d;
//                    d.hurt_player = interfaces::global_vars->curtime - iter.shot_time < 2.f ? iter.index : -1;
//                    d.pos = pos;
//                    d.userid = userid;
//                    d.hurt = false;
//                    d.time = interfaces::global_vars->curtime;
//                    last_events.push_back ( d );
//                }
//            }
//
//            iter.did_bullet_impact = true;
//            return;
//        }
//    }
//
//    static auto last_hurt_curtime = 0.f;
//    static auto last_hurt_attacker = -1;
//    static auto last_hurt_userid = -1;
//    static auto last_hurt_damage = -1;
//    static auto last_hurt_health = -1;
//    static auto last_hurt_hitgroup = 0;
//
//    if ( !strcmp ( event->get_name(), xor_str ( "player_hurt" ) ) )
//    {
//        const auto attacker = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "attacker" ) ) );
//        const auto hurt = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//        const auto health = event->get_int ( xor_str ( "health" ) );
//        const auto dmg_health = event->get_int ( xor_str ( "dmg_health" ) );
//        const auto hitgroup = event->get_int ( xor_str ( "hitgroup" ) );
//
//        if ( last_hurt_curtime == interfaces::global_vars->curtime && last_hurt_attacker == attacker
//                && last_hurt_userid == hurt && last_hurt_damage == dmg_health
//                && last_hurt_health == health && last_hurt_hitgroup == hitgroup )
//            return;
//
//        last_hurt_curtime = interfaces::global_vars->curtime;
//        last_hurt_attacker = attacker;
//        last_hurt_userid = hurt;
//        last_hurt_damage = dmg_health;
//        last_hurt_health = health;
//        last_hurt_hitgroup = hitgroup;
//
//        if ( attacker == interfaces::engine_client->get_localplayer() )
//        {
//            globals.hitmarker.time = interfaces::global_vars->realtime;
//            globals.hitmarker.dmg = dmg_health;
//            globals.hitmarker.did_kill = health <= 0;
//            interfaces::vgui_surface->play_sound_ ( xor_str ( "buttons\\arena_switch_press_02.wav" ) );
//
//            const auto player_hurt = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( hurt ) );
//
//            if ( !player_hurt )
//                return;
//
//            log_damage ( player_hurt, dmg_health, hitgroup );
//        }
//
//        const auto weapon = event->get_string ( xor_str ( "weapon" ) );
//
//        if ( weapon == xor_str ( "inferno" ) || weapon == xor_str ( "hegrenade" ) || weapon == xor_str ( "decoy" ) )
//            return;
//
//        for ( auto& last_event : last_events )
//        {
//            if ( last_event.userid == attacker && ( last_event.hurt_player == hurt || last_event.hurt_player == -1 ) )
//            {
//                last_event.hurt = true;
//                last_event.damage = dmg_health;
//                last_event.died = health <= 0;
//                last_event.got_hurt = true;
//                last_event.hitbox = hitgroup;
//            }
//        }
//    }
//
//    // fix time shit
//    if ( !strcmp ( event->get_name(), xor_str ( "weapon_fire" ) ) )
//    {
//        const auto userid = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//        const auto player = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( userid ) );
//
//        if ( !player || !player->is_alive() )
//            return;
//
//        auto weapon = player->active_weapon().get();
//
//        if ( !weapon || weapon->is_knife() || weapon->is_grenade() )
//            return;
//
//        #ifdef _DEBUG
//
//        if ( player == g_local )
//        {
//            for ( auto& iter : rbot_shot_data )
//            {
//                if ( iter.did_weapon_fire )
//                    continue;
//
//                if ( fabs ( interfaces::global_vars->curtime - iter.shot_time ) < 1.5f )
//                    g_features.resolver.count_shot ( iter.index, iter.rdata );
//
//                debug_console::debug ( xor_str ( "[ DEBUG ] event_logger => weapon_fire " ) );
//
//                iter.did_weapon_fire = true;
//                return;
//            }
//        }
//
//        #endif // _DEBUG
//
//        draw_footstep_beam ( player );
//    }
//
//    if ( !strcmp ( event->get_name(), xor_str ( "player_footstep" ) ) )
//    {
//        const auto userid = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//        auto player = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( userid ) );
//
//        if ( !player || !player->is_alive() )
//            return;
//
//        draw_footstep_beam ( player );
//    }
//
//    if ( !strcmp ( event->get_name(), xor_str ( "player_jump" ) ) )
//    {
//        const auto userid = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//        auto player = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( userid ) );
//
//        if ( !player || !player->is_alive() )
//            return;
//
//        draw_footstep_beam ( player );
//    }
//
//    if ( !strcmp ( event->get_name(), xor_str ( "round_start" ) ) )
//    {
//        for ( auto i = 0; i < interfaces::global_vars->max_clients; i++ )
//            c_resolver::player_dead ( i );
//    }
//
//    if ( !strcmp ( event->get_name(), xor_str ( "player_death" ) ) )
//    {
//        const auto attacker = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "attacker" ) ) );
//        const auto hurt = interfaces::engine_client->get_player_for_userid ( event->get_int ( xor_str ( "userid" ) ) );
//
//        c_resolver::player_dead ( hurt );
//    }
//
//    if ( !strcmp ( event->get_name(), xor_str ( "cs_game_disconnected" ) ) )
//    {
//        for ( auto& i : g_features.resolver.resolver_data )
//            i = resolver_data_struct_t();
//    }
//}
//
//void c_event_logger::on_create_move()
//{
//    if ( !g_local )
//    {
//        last_events = std::deque<event_data>();
//        return;
//    }
//
//    int count = 0;
//
//    auto ba = rbot_shot_data;
//
//    for ( auto& last_event : last_events )
//    {
//        if ( !last_event.got_pos )
//            continue;
//
//        auto player = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( last_event.userid ) );
//
//        if ( !player )
//            continue;
//
//        if ( ba.size() == 0 )
//            return;
//
//        if ( rbot_shot_data.size() != 0 )
//            rbot_shot_data.erase ( rbot_shot_data.begin() );
//
//        if ( player == g_local )
//        {
//            count++;
//
//            auto current_idx = count - 1;
//
//            if ( current_idx >= ba.size() )
//                current_idx = ba.size() - 1;
//
//            if ( last_event.hurt_player == -1 )
//                last_event.hurt_player = ba[current_idx].index;
//
//            auto player_hurt = reinterpret_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( last_event.hurt_player ) );
//
//
//            auto type = miss_type::none;
//
//            if ( player_hurt && player_hurt->is_alive() && last_event.hurt_player != -1 )
//            {
//
//                player_backup_data bdata;
//                bdata.create ( player_hurt );
//
//                ba[current_idx].bdata.restore ( player_hurt );
//                globals.overwrite_setupbones_matrix = true;
//                globals.overwrite_setupbones_index = player_hurt->ent_index();
//
//                //uintptr_t* bonecache = * ( uintptr_t** ) ( ( uintptr_t )player_hurt + 0x290C );
//                //uint32_t count = * ( uintptr_t* ) ( ( uintptr_t )player_hurt + 0x2918 );
//                //matrix3x4_t backup_matrix[maxstudiobones];
//
//                //if ( !player_hurt->setup_bones ( backup_matrix, 128, bone_used_by_anything, 0.f ) )
//                //    break;
//
//                //memcpy ( bonecache, last_rbot_matrix, sizeof ( matrix3x4_t ) * ( count < 256 ? count : 256 ) );
//
//                //float dmg = autowall::get_damage_vec ( last_events[i].pos, last_rbot_shot_eyepos, player_hurt, hitbox_chest );
//
//                //trace_t tr;
//                //Ray_t ray;
//
//                //ray.Init ( last_rbot_shot_eyepos, last_event.pos );
//                //interfaces::engine_trace->clip_ray_to_entity ( ray, MASK_SHOT | CONTENTS_GRATE, player, &tr );
//
//                //auto trace_hit_player = false;
//
//                //if ( tr.hit_entity == player_hurt )
//                //{
//                //    if ( tr.hit_entity )
//                //        console::write_line ( "tr.hit_entity: " + std::to_string ( tr.hit_entity->ent_index() ) + ", player_hurt: " + std::to_string ( player_hurt->ent_index() ) );
//
//                //    trace_hit_player = true;
//                //}
//
//                auto trace_hit_player = false;
//
//                trace_t trace;
//                Ray_t ray;
//                Vector view;
//                const auto angles = g_math.calc_angle ( ba[current_idx].eyepos, last_event.pos );
//                g_math.angle_vectors ( angles, view );
//                view.normalize_in_place ( );
//
//                view = ba[current_idx].eyepos + view * 4096.f;
//
//                ray.Init ( ba[current_idx].eyepos, view );
//                CTraceFilterPlayersOnlySkipOne filter ( g_local );
//                filter.ShouldHitEntity ( player_hurt, 0 );
//
//                interfaces::engine_trace->trace_ray ( ray, MASK_ALL | CONTENTS_GRATE, &filter, &trace );
//
//                if ( trace.hit_entity )
//                    trace_hit_player = true;
//
//                if ( !last_event.hurt && interfaces::global_vars->curtime - ba[current_idx].shot_time < 2.f )
//                {
//                    const auto fov = g_math.get_fov ( ba[current_idx].aimang, g_math.calc_angle ( ba[current_idx].eyepos, last_event.pos ) );
//
//                    if ( !trace_hit_player && fov >= 0.01f && fov < 30.f )
//                    {
//                        type = miss_type::spread;
//
//                        if ( g_options.misc_eventlog )
//                        {
//                            debug_console::print ( xor_str ( "missed shot due to spread by " ) + to_string_with_precision ( fov, 2 ) + xor_str ( " degrees" ) );
//                            logs.push_back ( { xor_str ( "missed shot due to spread by " )  + to_string_with_precision ( fov, 2 ) + xor_str ( " degrees" ), interfaces::global_vars->realtime } );
//                        }
//                    }
//                    else
//                    {
//                        type = miss_type::resolver;
//
//                        if ( g_options.misc_eventlog )
//                        {
//                            debug_console::print ( xor_str ( "missed shot due to bad resolve" ) );
//                            logs.push_back ( { xor_str ( "missed shot due to bad resolve" ), interfaces::global_vars->realtime } );
//                        }
//                    }
//                }
//
//                //memcpy ( backup_matrix, bonecache, sizeof ( matrix3x4_t ) * ( count < 256 ? count : 256 ) );
//
//                bdata.restore ( player_hurt );
//
//                globals.overwrite_setupbones_matrix = false;
//                globals.overwrite_setupbones_index = -1;
//
//                //bdata.restore ( player_hurt );
//            }
//            else
//            {
//                if ( last_event.died )
//                    type = miss_type::none;
//                else
//                    type = miss_type::custom;
//            }
//
//            /*
//                int shots = 0;
//            bool resolved = false;
//            bool did_hit = false;
//            animation_layer_t layers[15];
//            c_base_player_anim_state state;
//            */
//            static auto last_spread_misses = 0;
//            static auto last_ent_index = -1;
//            static auto last_spawntime = 0.f;
//
//            if ( last_ent_index != ba[current_idx].index || ( player_hurt != nullptr && last_spawntime != player_hurt->spawn_time() ) )
//                last_spread_misses = 0;
//
//            //if ( type == miss_type::spread )
//            //{
//            //    last_spawntime = player_hurt->spawn_time();
//            //    last_ent_index = last_rbot_entity;
//            //    last_spread_misses++;
//
//            //    if ( last_spread_misses > 2 )
//            //        type = miss_type::resolver;
//            //}
//            //else
//            //    last_spread_misses = 0;
//
//            if ( type != miss_type::resolver && type != miss_type::custom && ba[current_idx].index != -1 )
//            {
//                g_features.resolver.remove_shot ( ba[current_idx].index, ba[current_idx].rdata );
//
//                ////shots_data[last_rbot_entity].shots--;
//                //g_features.resolver.resolver_data[last_rbot_entity].shots--;
//
//                ////if ( shots_data[last_rbot_entity].shots < 0 )
//                ////    shots_data[last_rbot_entity].shots = 0;
//
//                ////g_features.resolver.resolver_data[last_rbot_entity].shots = shots_data[last_rbot_entity].shots;
//
//                ////if ( !last_rbot_resolver_was_desync )
//                //if ( did_was_last_non_desync )
//                //{
//                //    g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver--;
//
//                //    if ( g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver < 0 )
//                //        g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver = 0;
//
//                //    did_was_last_non_desync = false;
//                //}
//
//                //g_features.resolver.resolver_data[last_rbot_entity].overall_shots--;
//
//                //if ( g_features.resolver.resolver_data[last_rbot_entity].overall_shots < 0 )
//                //    g_features.resolver.resolver_data[last_rbot_entity].overall_shots = 0;
//
//                //c_resolver::remove_shot_at_current_mode ( last_rbot_entity );
//                ////g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots--;
//
//                //if ( g_features.resolver.resolver_data[last_rbot_entity].shots < 0 )
//                //    g_features.resolver.resolver_data[last_rbot_entity].shots = 0;
//
//
//                //g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )]--;
//                //g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )]--;
//
//                //if ( g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] < 0 )
//                //    g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] = 0;
//
//                //if ( g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] < 0 )
//                //    g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] = 0;
//
//                ////if ( g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots < 0 )
//                ////    g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots = 0;
//
//                ////g_features.resolver.resolver_data[last_rbot_entity].shots = g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots;
//            }
//
//            if ( !ba[current_idx].rdata.in_shot )
//            {
//                if ( ( last_event.hitbox == hitbox_head || last_event.hitbox == hitbox_neck ) && type == miss_type::none && ba[current_idx].index != -1 && interfaces::global_vars->curtime - ba[current_idx].shot_time < 1.f )
//                {
//                    g_features.resolver.resolver_data[ba[current_idx].index].has_hit_head_yet = true;
//                    g_features.resolver.resolver_data[ba[current_idx].index].last_hit_mode.shots_missed = 0;
//                    g_features.resolver.resolver_data[ba[current_idx].index].last_hit_mode.mode = ba[current_idx].rdata.current_mode;
//                    g_features.resolver.resolver_data[ba[current_idx].index].last_hit_mode.method = ba[current_idx].rdata.current_method;
//                }
//                else if ( type == miss_type::resolver && ba[current_idx].index != -1 && interfaces::global_vars->curtime - ba[current_idx].shot_time < 2.f )
//                    g_features.resolver.resolver_data[ba[current_idx].index].last_hit_mode.shots_missed++;
//            }
//
//            std::string tmp_mode_name;
//
//            switch ( ba[current_idx].rdata.current_mode )
//            {
//            case resolver_modes_t::plus:
//                tmp_mode_name = xor_str ( "plus" );
//                break;
//
//            case resolver_modes_t::minus:
//                tmp_mode_name = xor_str ( "minus" );
//                break;
//
//            case resolver_modes_t::plus_120:
//                tmp_mode_name = xor_str ( "plus_120" );
//                break;
//
//            case resolver_modes_t::minus_120:
//                tmp_mode_name = xor_str ( "minus_120" );
//                break;
//
//            case resolver_modes_t::none:
//                tmp_mode_name = xor_str ( "none" );
//                break;
//
//            case resolver_modes_t::shot:
//                tmp_mode_name = xor_str ( "shot" );
//                break;
//
//            default:
//                tmp_mode_name = xor_str ( "-1" );
//                break;
//            }
//
//            resolver_shot_data tmpdata = {                g_features.resolver.resolver_data[ba[current_idx].index].shots,
//                                                          g_features.resolver.resolver_data[ba[current_idx].index].resolved,
//                                                          type == miss_type::none,
//                                                          type == miss_type::spread,
//                                                          last_event.damage
//                                         };
//            tmpdata.mode_name = tmp_mode_name;
//
//            security.info_data.push_back ( tmpdata );
//
//        }
//    }
//
//    last_events = std::deque<event_data>();
//}
//
//void c_event_logger::on_draw()
//{
//    if ( !g_options.misc_eventlog )
//        return;
//
//    if ( logs.size() == 0 )
//        return;
//
//    auto draw_height = 15;
//    //auto did_min = false;
//    const auto add_log = [&] ( std::string text, float delta ) -> void
//    {
//        auto time_delta_2 = delta - 2.f;
//
//        if ( time_delta_2 < 0.f )
//            time_delta_2 = 0.f;
//
//        const auto percent = time_delta_2 / 1.f;
//        auto alpha = 255 - static_cast<int> ( percent * 255.f );
//        const auto clr = Color ( 255, 255, 255, alpha );
//        const auto tmp = static_cast<int> ( 12 * percent );
//
//        draw_height -= tmp;
//
//        g_render.text ( g_fonts.normal, text, 5, draw_height, clr );
//        draw_height += 12;
//    };
//
//    for ( auto _log = logs.begin(); _log != logs.end(); _log++ )
//    {
//        const auto t = interfaces::global_vars->realtime - _log->time;
//
//        if ( t < 3.f )
//        {
//            add_log ( _log->msg, t );
//            /*
//            auto t2 = t - 2.f;
//
//            if ( t2 < 0.f )
//                t2 = 0.f;
//
//            const auto percent = ( t2 / 1.f );
//            auto alpha = 255 - ( static_cast<int> ( percent * 255.f ) );
//
//            const auto clr = Color ( 255, 255, 255, alpha );
//
//            h -= static_cast<int> ( ( percent * 12.f ) );
//
//            g_render.text ( g_fonts.normal, _log->msg, 5, h, clr );
//
//            h += 12;
//            */
//        }
//        else
//        {
//            logs.erase ( _log );
//
//            if ( !logs.empty() )
//                _log = logs.begin();
//            else
//                return;
//        }
//    }
//
//    /*
//    for ( auto log : logs )
//    {
//        if ( ( interfaces::global_vars->realtime - log.time ) < 1.f )
//        {
//            g_render.text ( g_fonts.normal, "", 5, h, Color::White );
//            h += 13;
//        }
//    }
//    */
//}
//
//void c_event_logger::set_rbot_data ( C_BasePlayer* player, int index, QAngle angle )
//{
//    if ( !player )
//        return;
//
//    player_event_logger_time tmp;
//
//    if ( tmp.bdata.create ( player ) )
//    {
//        tmp.index = index;
//        tmp.shot_time = interfaces::global_vars->curtime;
//        tmp.eyepos = g_local->get_eye_pos();
//        tmp.aimang = angle;
//        tmp.rdata = g_features.resolver.resolver_data[index];
//        //last_rbot_resolver_was_desync = g_features.resolver.resolver_data[index].desyncing;
//        //last_rbot_got_matrix = player->setup_bones ( last_rbot_matrix, 128, bone_used_by_anything, 0.f );
//    }
//
//    this->rbot_shot_data.push_back ( tmp );
//}
//
//void c_event_logger::set_rbot_data_backtrack ( C_BasePlayer* player, int index, QAngle angle, backtrack_all_data data )
//{
//    if ( !player )
//        return;
//
//    player_event_logger_time tmp;
//
//    tmp.bdata = data.ndata;
//
//    tmp.index = index;
//    tmp.shot_time = interfaces::global_vars->curtime;
//    tmp.eyepos = g_local->get_eye_pos();
//    tmp.aimang = angle;
//    tmp.rdata = data.rdata;
//
//    this->rbot_shot_data.push_back ( tmp );
//    //last_rbot_resolver_was_desync = g_features.resolver.resolver_data[index].desyncing;
//    //last_rbot_got_matrix = player->setup_bones ( last_rbot_matrix, 128, bone_used_by_anything, 0.f );
//}
//
//void c_event_logger::log_damage ( C_BasePlayer* player, int damage, int hitgroup )
//{
//    if ( !g_options.misc_eventlog )
//        return;
//
//    std::string hitgroup_str;
//
//    switch ( hitgroup )
//    {
//    case hitbox_head:
//    case hitbox_neck:
//        hitgroup_str = xor_str ( "HEAD" );
//        break;
//
//    case hitbox_pelvis:
//    case hitbox_stomach:
//    case hitbox_lower_chest:
//    case hitbox_chest:
//    case hitbox_upper_chest:
//        hitgroup_str = xor_str ( "BODY" );
//        break;
//
//    case hitbox_right_thigh:
//    case hitbox_left_thigh:
//    case hitbox_right_calf:
//    case hitbox_left_calf:
//    case hitbox_right_foot:
//    case hitbox_left_foot:
//        hitgroup_str = xor_str ( "FOOT" );
//        break;
//
//    case hitbox_right_hand:
//    case hitbox_left_hand:
//    case hitbox_right_upper_arm:
//    case hitbox_right_forearm:
//    case hitbox_left_upper_arm:
//    case hitbox_left_forearm:
//        hitgroup_str = xor_str ( "ARM" );
//        break;
//    }
//
//    debug_console::print ( xor_str ( "hit player \"" ) + std::string ( player->get_player_info().sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) + xor_str ( " damage in the " ) + hitgroup_str );
//    logs.push_back ( { xor_str ( "hit player \"" ) + std::string ( player->get_player_info().sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) + xor_str ( " damage in the " ) + hitgroup_str, interfaces::global_vars->realtime } );
//    //logs.push_back ( { xor_str ( "hit player \"" ) + std::string ( player->get_player_info().sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) + xor_str ( " damage in the " ) + hitgroup_str, interfaces::global_vars->realtime } );
//
//
//
//}
//
//void c_event_logger::draw_beam ( Vector startpos, Color color, Vector pos ) const
//{
//    if ( !g_options.visuals_bullet_tracers )
//        return;
//
//    const auto colorbeams = g_options.visuals_beams_bullet_tracer;
//    BeamInfo_t beam_info;
//    beam_info.m_nType = TE_BEAMPOINTS;
//    beam_info.m_pszModelName = xor_str ( "sprites/physbeam.vmt" );
//    beam_info.m_nModelIndex = interfaces::mdl_info->get_model_index ( xor_str ( "materials/sprites/physbeam.vmt" ) );
//    beam_info.m_flHaloScale = 0.f;
//    beam_info.m_fle = 4.f;
//    beam_info.m_flWidth = 2.f;
//    beam_info.m_flEndWidth = 2.f;
//    beam_info.m_flFadeLength = 0.1f;
//    beam_info.m_flAmplitude = 2.f;
//    beam_info.m_flBrightness = color.a();
//    beam_info.m_flSpeed = 0.2f;
//    beam_info.m_nStartFrame = 0;
//    beam_info.m_flFrameRate = 0.f;
//    beam_info.m_flRed = colorbeams.r();
//    beam_info.m_flGreen = colorbeams.g();
//    beam_info.m_flBlue = colorbeams.b();
//    beam_info.m_flBrightness = colorbeams.a();
//    beam_info.m_nSegments = 2;
//    beam_info.m_bRenderable = true;
//    beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
//
//    static auto last_r = Vector ( g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ) );
//    static auto last_time = interfaces::global_vars->curtime;
//
//    if ( last_time != interfaces::global_vars->curtime )
//    {
//        last_r = Vector ( g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ) );
//        last_time = interfaces::global_vars->curtime;
//    }
//
//    beam_info.m_vecStart = startpos + last_r;
//    beam_info.m_vecEnd = pos;
//
//    auto beam = interfaces::render_beams->CreateBeamPoints ( beam_info );
//
//    if ( beam )
//        interfaces::render_beams->DrawBeam ( beam );
//}
//
//void c_event_logger::draw_footstep_beam ( C_BasePlayer* player ) const
//{
//    if ( !g_options.visuals_footstep_esp )
//        return;
//
//    auto local = static_cast<C_BasePlayer*> ( g_local );
//
//    if ( !local || !player || player->is_dormant() || local == player || !player->is_enemy() )
//        return;
//
//    const auto color = g_options.visuals_footstep_esp_color;
//
//    BeamInfo_t beam_info;
//    beam_info.m_nType = TE_BEAMRINGPOINT;
//    beam_info.m_pszModelName = xor_str ( "sprites/physbeam.vmt" );
//    beam_info.m_nModelIndex = interfaces::mdl_info->get_model_index ( xor_str ( "sprites/physbeam.vmt" ) );
//    beam_info.m_nHaloIndex = -1;
//    beam_info.m_flHaloScale = 3;
//    beam_info.m_fle = 2.f;
//    beam_info.m_flWidth = 4.f;
//    beam_info.m_flFadeLength = 1.f;
//    beam_info.m_flAmplitude = 0.f;
//    beam_info.m_flRed = color.r();
//    beam_info.m_flGreen = color.g();
//    beam_info.m_flBlue = color.b();
//    beam_info.m_flBrightness = color.a();
//    beam_info.m_flSpeed = 0.f;
//    beam_info.m_nStartFrame = 0.f;
//    beam_info.m_flFrameRate = 60.f;
//    beam_info.m_nSegments = -1;
//    beam_info.m_nFlags = FBEAM_FADEOUT;
//    beam_info.m_vecCenter = player->vec_origin() + Vector ( 0, 0, 5 );
//    beam_info.m_flStartRadius = 5.f;
//    beam_info.m_flEndRadius = g_options.visuals_beam_ring_size;
//
//    auto beam = interfaces::render_beams->CreateBeamRingPoint ( beam_info );
//
//    if ( beam )
//        interfaces::render_beams->DrawBeam ( beam );
//}

#include "event_logger.h"
#include "../../helpers/debug_console.h"
#include "../../renderer/vgui_render.h"
#include "../../helpers/math.hpp"
#include "../../helpers/console.h"
#include "../../options.h"
#include "../../security/security.h"
#include "../features.h"

struct shotdata
{
    int shots = 0;
};

//shotdata shots_data[64];

void c_event_logger::on_fire_event ( IGameEvent* event )
{
    if ( !strcmp ( event->GetName( ), xor_str ( "bullet_impact" ) ) )
    {
        const auto pos = Vector ( event->GetFloat ( xor_str ( "x" ) ), event->GetFloat ( xor_str ( "y" ) ), event->GetFloat ( xor_str ( "z" ) ) );
        const auto userid = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );

        if ( userid == interfaces::engine_client->get_localplayer( ) && g_local )
        {
            static auto last_shot_from = g_local->get_eye_pos( );
            static auto last_curtime = interfaces::global_vars->curtime;

            if ( last_curtime != interfaces::global_vars->curtime )
            {
                last_shot_from = g_local->get_eye_pos( );
                last_curtime = interfaces::global_vars->curtime;
            }

            draw_beam ( last_shot_from, Color::green, pos );
        }
        else
        {
            C_BasePlayer* pla = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( userid ) );

            if ( pla && pla->is_alive( ) && !pla->is_dormant( ) )
            {
                if ( pla->is_enemy( ) )
                {
                    static auto last_shot_from = pla->get_eye_pos( );
                    static auto last_curtime = interfaces::global_vars->curtime;

                    if ( last_curtime != interfaces::global_vars->curtime )
                    {
                        last_shot_from = pla->get_eye_pos( );
                        last_curtime = interfaces::global_vars->curtime;
                    }

                    draw_beam ( last_shot_from, Color::red, pos );
                }
                else
                {
                    static auto last_shot_from = pla->get_eye_pos( );
                    static auto last_curtime = interfaces::global_vars->curtime;

                    if ( last_curtime != interfaces::global_vars->curtime )
                    {
                        last_shot_from = pla->get_eye_pos( );
                        last_curtime = interfaces::global_vars->curtime;
                    }

                    draw_beam ( last_shot_from, Color::blue, pos );
                }
            }
        }

        const auto size = last_events.size( );

        if ( size == 0 )
        {
            event_data_t d;
            d.hurt_player = interfaces::global_vars->curtime - last_rbot_shot_time < 2.f ? last_rbot_entity : -1;
            d.pos = pos;
            d.userid = userid;
            d.hurt = false;
            d.time = interfaces::global_vars->curtime;
            d.got_pos = true;
            last_events.push_back ( d );
        }
        else
        {
            if ( last_events[ size - 1 ].time == interfaces::global_vars->curtime && last_events[ size - 1 ].hurt_player == last_rbot_entity )
                last_events[ size - 1 ].pos = pos;
            else
            {
                event_data_t d;
                d.hurt_player = interfaces::global_vars->curtime - last_rbot_shot_time < 2.f ? last_rbot_entity : -1;
                d.pos = pos;
                d.userid = userid;
                d.hurt = false;
                d.time = interfaces::global_vars->curtime;
                last_events.push_back ( d );
            }
        }
    }

    static auto last_hurt_curtime = 0.f;
    static auto last_hurt_attacker = -1;
    static auto last_hurt_userid = -1;
    static auto last_hurt_damage = -1;
    static auto last_hurt_health = -1;
    static auto last_hurt_hitgroup = 0;

    if ( !strcmp ( event->GetName( ), xor_str ( "player_hurt" ) ) )
    {
        const auto attacker = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "attacker" ) ) );
        const auto hurt = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );
        const auto health = event->GetInt ( xor_str ( "health" ) );
        const auto dmg_health = event->GetInt ( xor_str ( "dmg_health" ) );
        const auto hitgroup = event->GetInt ( xor_str ( "hitgroup" ) );

        if ( last_hurt_curtime == interfaces::global_vars->curtime && last_hurt_attacker == attacker
            && last_hurt_userid == hurt && last_hurt_damage == dmg_health
            && last_hurt_health == health && last_hurt_hitgroup == hitgroup )
            return;

        last_hurt_curtime = interfaces::global_vars->curtime;
        last_hurt_attacker = attacker;
        last_hurt_userid = hurt;
        last_hurt_damage = dmg_health;
        last_hurt_health = health;
        last_hurt_hitgroup = hitgroup;

        if ( attacker == interfaces::engine_client->get_localplayer( ) )
        {
            globals.hitmarker.time = interfaces::global_vars->realtime;
            globals.hitmarker.dmg = dmg_health;
            globals.hitmarker.did_kill = health <= 0;
            interfaces::vgui_surface->play_sound_ ( xor_str ( "buttons\\arena_switch_press_02.wav" ) );

            const auto player_hurt = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( hurt ) );

            if ( !player_hurt )
                return;

            g_features.visuals.add_damage_data ( hurt, dmg_health, player_hurt->get_eye_pos( ), hitgroup == hitbox_neck );

            log_damage ( player_hurt, dmg_health, hitgroup );
        }

        const auto weapon = event->GetString ( xor_str ( "weapon" ) );

        if ( weapon == xor_str ( "inferno" ) || weapon == xor_str ( "hegrenade" ) || weapon == xor_str ( "decoy" ) )
            return;

        for ( auto& last_event : last_events )
        {
            if ( last_event.userid == attacker && ( last_event.hurt_player == hurt || last_event.hurt_player == -1 ) )
            {
                last_event.hurt = true;
                last_event.damage = dmg_health;
                last_event.died = health <= 0;
                last_event.got_hurt = true;
                last_event.hitbox = hitgroup;
            }
        }
    }

    // fix time shit
    if ( !strcmp ( event->GetName( ), xor_str ( "weapon_fire" ) ) )
    {
        const auto userid = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );
        const auto player = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( userid ) );

        if ( !player || !player->is_alive( ) )
            return;

        auto weapon = player->active_weapon( ).get( );

        if ( !weapon || weapon->is_knife( ) || weapon->is_grenade( ) )
            return;

#ifdef _DEBUG

        if ( player == g_local )
        {
            if ( fabs ( interfaces::global_vars->curtime - last_rbot_shot_time ) < 1.5f )
                g_features.resolver.count_player_shot ( last_rbot_entity, last_rbot_resolver_data );

            debug_console::print ( xor_str ( "[ DEBUG ] event_logger => weapon_fire " ) );
        }

#endif // _DEBUG

        draw_footstep_beam ( player );
    }

    if ( !strcmp ( event->GetName( ), xor_str ( "player_footstep" ) ) )
    {
        const auto userid = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );
        auto player = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( userid ) );

        if ( !player || !player->is_alive( ) )
            return;

        draw_footstep_beam ( player );
    }

    if ( !strcmp ( event->GetName( ), xor_str ( "player_jump" ) ) )
    {
        const auto userid = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );
        auto player = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( userid ) );

        if ( !player || !player->is_alive( ) )
            return;

        draw_footstep_beam ( player );
    }

    if ( !strcmp ( event->GetName( ), xor_str ( "round_start" ) ) )
    {
        for ( auto i = 0; i < interfaces::global_vars->max_clients; i++ )
            g_features.resolver.reset_player_resolver_data ( i );
    }

    if ( !strcmp ( event->GetName( ), xor_str ( "player_death" ) ) )
    {
        const auto attacker = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "attacker" ) ) );
        const auto hurt = interfaces::engine_client->get_player_for_userid ( event->GetInt ( xor_str ( "userid" ) ) );

        g_features.resolver.reset_player_resolver_data ( hurt );
    }

    if ( !strcmp ( event->GetName( ), xor_str ( "cs_game_disconnected" ) ) )
        g_features.resolver.reset_resolver_data( );
}

void c_event_logger::on_create_move ( )
{
    if ( !g_local )
    {
        last_events = std::deque< event_data_t >( );
        return;
    }

    for ( auto& last_event : last_events )
    {
        if ( !last_event.got_pos )
            continue;

        auto player = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( last_event.userid ) );

        if ( !player )
            continue;

        if ( last_event.hurt_player == -1 )
            last_event.hurt_player = last_rbot_entity;

        auto player_hurt = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( last_event.hurt_player ) );

        if ( player == g_local )
        {
            auto type = miss_type_t::none;

            if ( player_hurt && player_hurt->is_alive( ) && last_event.hurt_player != -1 )
            {
                player_backup_data_t bdata;
                bdata.create ( player_hurt );
                last_shot_pdata.restore ( player_hurt );
                globals.overwrite_setupbones_matrix = true;
                globals.overwrite_setupbones_index = player_hurt->ent_index( );

                //uintptr_t* bonecache = * ( uintptr_t** ) ( ( uintptr_t )player_hurt + 0x290C );
                //uint32_t count = * ( uintptr_t* ) ( ( uintptr_t )player_hurt + 0x2918 );
                //matrix3x4_t backup_matrix[maxstudiobones];

                //if ( !player_hurt->setup_bones ( backup_matrix, 128, bone_used_by_anything, 0.f ) )
                //    break;

                //memcpy ( bonecache, last_rbot_matrix, sizeof ( matrix3x4_t ) * ( count < 256 ? count : 256 ) );

                //float dmg = autowall::get_damage_vec ( last_events[i].pos, last_rbot_shot_eyepos, player_hurt, hitbox_chest );

                //trace_t tr;
                //Ray_t ray;

                //ray.Init ( last_rbot_shot_eyepos, last_event.pos );
                //interfaces::engine_trace->clip_ray_to_entity ( ray, MASK_SHOT | CONTENTS_GRATE, player, &tr );

                //auto trace_hit_player = false;

                //if ( tr.hit_entity == player_hurt )
                //{
                //    if ( tr.hit_entity )
                //        console::write_line ( "tr.hit_entity: " + std::to_string ( tr.hit_entity->ent_index() ) + ", player_hurt: " + std::to_string ( player_hurt->ent_index() ) );

                //    trace_hit_player = true;
                //}

                auto trace_hit_player = false;

                trace_t trace;
                Ray_t ray;
                Vector view;
                const auto angles = g_math.calc_angle ( last_rbot_shot_eyepos, last_event.pos );
                g_math.angle_vectors ( angles, view );
                view.normalize_in_place( );

                view = last_rbot_shot_eyepos + view * 4096.f;

                ray.Init ( last_rbot_shot_eyepos, view );
                CTraceFilterPlayersOnlySkipOne filter ( g_local );
                filter.ShouldHitEntity ( player_hurt, 0 );

                interfaces::engine_trace->trace_ray ( ray, MASK_ALL | CONTENTS_GRATE, &filter, &trace );

                if ( trace.hit_entity )
                    trace_hit_player = true;

                if ( last_event.hurt && last_event.hitbox == hitbox_neck )
                    g_features.resolver.rdata.at ( player_hurt->ent_index( ) ).set_hit_head( );

                if ( !last_event.hurt && interfaces::global_vars->curtime - last_rbot_shot_time < 2.f )
                {
                    const auto fov = g_math.get_fov ( last_rbot_shot_angle, g_math.calc_angle ( last_rbot_shot_eyepos, last_event.pos ) );

                    if ( !trace_hit_player && fov >= 0.01f && fov < 30.f )
                    {
                        type = miss_type_t::spread;

                        if ( g_options.misc_eventlog )
                        {
                            debug_console::print (
                                xor_str ( "missed shot due to spread by " ) + to_string_with_precision ( fov, 2 ) + xor_str ( " degrees" ) );
                            logs.push_back ( {
                                xor_str ( "missed shot due to spread by " ) + to_string_with_precision ( fov, 2 ) + xor_str ( " degrees" ),
                                interfaces::global_vars->realtime
                            } );
                        }
                    }
                    else
                    {
                        type = miss_type_t::resolver;

                        if ( g_options.misc_eventlog )
                        {
                            g_features.resolver.rdata.at ( player_hurt->ent_index( ) ).set_did_not_hit( );
                            debug_console::print ( xor_str ( "missed shot due to bad resolve" ) );
                            logs.push_back ( { xor_str ( "missed shot due to bad resolve" ), interfaces::global_vars->realtime } );
                        }
                    }
                }

                //memcpy ( backup_matrix, bonecache, sizeof ( matrix3x4_t ) * ( count < 256 ? count : 256 ) );

                bdata.restore ( player_hurt );

                globals.overwrite_setupbones_matrix = false;
                globals.overwrite_setupbones_index = -1;

                //bdata.restore ( player_hurt );
            }
            else
            {
                if ( last_event.died )
                    type = miss_type_t::none;
                else
                    type = miss_type_t::custom;
            }

            /*
                int shots = 0;
            bool resolved = false;
            bool did_hit = false;
            animation_layer_t layers[15];
            c_base_player_anim_state state;
            */
            static auto last_spread_misses = 0;
            static auto last_ent_index = -1;
            static auto last_spawntime = 0.f;

            if ( last_ent_index != last_rbot_entity || ( player_hurt != nullptr && last_spawntime != player_hurt->spawn_time( ) ) )
                last_spread_misses = 0;

            //if ( type == miss_type::spread )
            //{
            //    last_spawntime = player_hurt->spawn_time();
            //    last_ent_index = last_rbot_entity;
            //    last_spread_misses++;

            //    if ( last_spread_misses > 2 )
            //        type = miss_type::resolver;
            //}
            //else
            //    last_spread_misses = 0;

            if ( type != miss_type_t::resolver && type != miss_type_t::custom && last_rbot_entity != -1 )
            {
                g_features.resolver.remove_player_shot ( last_rbot_entity, last_rbot_resolver_data );

                ////shots_data[last_rbot_entity].shots--;
                //g_features.resolver.resolver_data[last_rbot_entity].shots--;

                ////if ( shots_data[last_rbot_entity].shots < 0 )
                ////    shots_data[last_rbot_entity].shots = 0;

                ////g_features.resolver.resolver_data[last_rbot_entity].shots = shots_data[last_rbot_entity].shots;

                ////if ( !last_rbot_resolver_was_desync )
                //if ( did_was_last_non_desync )
                //{
                //    g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver--;

                //    if ( g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver < 0 )
                //        g_features.resolver.resolver_data[last_rbot_entity].overall_shots_no_resolver = 0;

                //    did_was_last_non_desync = false;
                //}

                //g_features.resolver.resolver_data[last_rbot_entity].overall_shots--;

                //if ( g_features.resolver.resolver_data[last_rbot_entity].overall_shots < 0 )
                //    g_features.resolver.resolver_data[last_rbot_entity].overall_shots = 0;

                //c_resolver::remove_shot_at_current_mode ( last_rbot_entity );
                ////g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots--;

                //if ( g_features.resolver.resolver_data[last_rbot_entity].shots < 0 )
                //    g_features.resolver.resolver_data[last_rbot_entity].shots = 0;

                //g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )]--;
                //g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )]--;

                //if ( g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] < 0 )
                //    g_features.resolver.resolver_data[last_rbot_entity].method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] = 0;

                //if ( g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] < 0 )
                //    g_features.resolver.resolver_data[last_rbot_entity].overall_method_shots[static_cast<int> ( g_features.resolver.resolver_data[last_rbot_entity].current_method )] = 0;

                ////if ( g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots < 0 )
                ////    g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots = 0;

                ////g_features.resolver.resolver_data[last_rbot_entity].shots = g_features.resolver.resolver_data[last_rbot_entity].unpredicted_shots;
            }

            //if ( !last_rbot_resolver_data.in_shot )
            //{
            //    if ( ( last_event.hitbox == hitbox_head || last_event.hitbox == hitbox_neck ) && type == miss_type_t::none && last_rbot_entity != -1 && interfaces::global_vars->curtime - last_rbot_shot_time < 1.f )
            //    {
            //        g_features.resolver.rdata.at ( last_rbot_entity ).has_hit_head_yet = true;
            //        g_features.resolver.rdata.at ( last_rbot_entity ).last_hit_mode.shots_missed = 0;
            //        g_features.resolver.rdata.at ( last_rbot_entity ).last_hit_mode.mode = last_rbot_resolver_data.current_mode;
            //        g_features.resolver.rdata.at ( last_rbot_entity ).last_hit_mode.method = last_rbot_resolver_data.current_method;
            //    }
            //    else if ( type == miss_type_t::resolver && last_rbot_entity != -1 && interfaces::global_vars->curtime - last_rbot_shot_time < 2.f )
            //        g_features.resolver.resolver_data[last_rbot_entity].last_hit_mode.shots_missed++;
            //}

            //std::string tmp_mode_name;

            //switch ( last_rbot_resolver_data.current_mode )
            //{
            //case resolver_modes_t::plus:
            //    tmp_mode_name = xor_str ( "plus" );
            //    break;

            //case resolver_modes_t::minus:
            //    tmp_mode_name = xor_str ( "minus" );
            //    break;

            //case resolver_modes_t::plus_120:
            //    tmp_mode_name = xor_str ( "plus_120" );
            //    break;

            //case resolver_modes_t::minus_120:
            //    tmp_mode_name = xor_str ( "minus_120" );
            //    break;

            //case resolver_modes_t::none:
            //    tmp_mode_name = xor_str ( "none" );
            //    break;

            //case resolver_modes_t::shot:
            //    tmp_mode_name = xor_str ( "shot" );
            //    break;

            //default:
            //    tmp_mode_name = xor_str ( "-1" );
            //    break;
            //}

            //resolver_shot_data tmpdata = {                g_features.resolver.rdata.at ( last_rbot_entity ).shots,
            //                                              g_features.resolver.rdata.at ( last_rbot_entity ).get_resolved_state(  ),
            //                                              type == miss_type_t::none,
            //                                              type == miss_type_t::spread,
            //                                              last_event.damage
            //                             };
            ////tmpdata.mode_name = tmp_mode_name;

            //security.info_data.push_back ( tmpdata );
        }
    }

    last_events = std::deque< event_data_t >( );
}

void c_event_logger::on_draw ( )
{
    if ( !g_options.misc_eventlog )
        return;

    if ( logs.size( ) == 0 )
        return;

    auto draw_height = 15;
    //auto did_min = false;
    const auto add_log = [&] ( std::string text, float delta ) -> void
    {
        auto time_delta_2 = delta - 2.f;

        if ( time_delta_2 < 0.f )
            time_delta_2 = 0.f;

        const auto percent = time_delta_2 / 1.f;
        auto alpha = 255 - static_cast< int > ( percent * 255.f );
        const auto clr = Color ( 255, 255, 255, alpha );
        const auto tmp = static_cast< int > ( 12 * percent );

        draw_height -= tmp;

        g_render.text ( g_fonts.normal, text, 5, draw_height, clr );
        draw_height += 12;
    };

    for ( auto _log = logs.begin( ); _log != logs.end( ); _log++ )
    {
        const auto t = interfaces::global_vars->realtime - _log->time;

        if ( t < 3.f )
        {
            add_log ( _log->msg, t );
            /*
            auto t2 = t - 2.f;

            if ( t2 < 0.f )
                t2 = 0.f;

            const auto percent = ( t2 / 1.f );
            auto alpha = 255 - ( static_cast<int> ( percent * 255.f ) );

            const auto clr = Color ( 255, 255, 255, alpha );

            h -= static_cast<int> ( ( percent * 12.f ) );

            g_render.text ( g_fonts.normal, _log->msg, 5, h, clr );

            h += 12;
            */
        }
        else
        {
            logs.erase ( _log );

            if ( !logs.empty( ) )
                _log = logs.begin( );
            else
                return;
        }
    }

    /*
    for ( auto log : logs )
    {
        if ( ( interfaces::global_vars->realtime - log.time ) < 1.f )
        {
            g_render.text ( g_fonts.normal, "", 5, h, Color::White );
            h += 13;
        }
    }
    */
}

void c_event_logger::set_rbot_data ( C_BasePlayer* player, int index, QAngle angle )
{
    if ( !player )
        return;

    if ( last_shot_pdata.create ( player ) )
    {
        last_rbot_entity = index;
        last_rbot_shot_time = interfaces::global_vars->curtime;
        last_rbot_shot_eyepos = g_local->get_eye_pos( );
        last_rbot_shot_angle = angle;
        last_rbot_resolver_data = g_features.resolver.rdata.at ( index );
        //last_rbot_resolver_was_desync = g_features.resolver.resolver_data[index].desyncing;
        //last_rbot_got_matrix = player->setup_bones ( last_rbot_matrix, 128, bone_used_by_anything, 0.f );
    }
    else
    {
        last_rbot_entity = -1;
        last_rbot_shot_time = 0.f;
        last_rbot_shot_angle.Init( );
        last_rbot_resolver_data = { };
        //last_rbot_resolver_was_desync = false;
        //last_rbot_got_matrix = false;
    }
}

void c_event_logger::set_rbot_data_backtrack ( C_BasePlayer* player, int index, QAngle angle, backtrack_all_data data )
{
    if ( !player )
        return;

    last_shot_pdata = data.ndata;

    last_rbot_entity = index;
    last_rbot_shot_time = interfaces::global_vars->curtime;
    last_rbot_shot_eyepos = g_local->get_eye_pos( );
    last_rbot_shot_angle = angle;
    last_rbot_resolver_data = data.rdata;
    //last_rbot_resolver_was_desync = g_features.resolver.resolver_data[index].desyncing;
    //last_rbot_got_matrix = player->setup_bones ( last_rbot_matrix, 128, bone_used_by_anything, 0.f );
}

void c_event_logger::log_damage ( C_BasePlayer* player, int damage, int hitgroup )
{
    if ( !g_options.misc_eventlog )
        return;

    std::string hitgroup_str;

    switch ( hitgroup )
    {
    case hitbox_head:
    case hitbox_neck:
        hitgroup_str = xor_str ( "HEAD" );
        break;

    case hitbox_pelvis:
    case hitbox_stomach:
    case hitbox_lower_chest:
    case hitbox_chest:
    case hitbox_upper_chest:
        hitgroup_str = xor_str ( "BODY" );
        break;

    case hitbox_right_thigh:
    case hitbox_left_thigh:
    case hitbox_right_calf:
    case hitbox_left_calf:
    case hitbox_right_foot:
    case hitbox_left_foot:
        hitgroup_str = xor_str ( "FOOT" );
        break;

    case hitbox_right_hand:
    case hitbox_left_hand:
    case hitbox_right_upper_arm:
    case hitbox_right_forearm:
    case hitbox_left_upper_arm:
    case hitbox_left_forearm:
        hitgroup_str = xor_str ( "ARM" );
        break;
    }

    debug_console::print (
        xor_str ( "hit player \"" ) + std::string ( player->get_player_info( ).sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) +
        xor_str ( " damage in the " ) + hitgroup_str );
    logs.push_back ( {
        xor_str ( "hit player \"" ) + std::string ( player->get_player_info( ).sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) +
        xor_str ( " damage in the " ) + hitgroup_str,
        interfaces::global_vars->realtime
    } );
    //logs.push_back ( { xor_str ( "hit player \"" ) + std::string ( player->get_player_info().sz_name ) + xor_str ( "\" for " ) + std::to_string ( damage ) + xor_str ( " damage in the " ) + hitgroup_str, interfaces::global_vars->realtime } );
}

void c_event_logger::draw_beam ( Vector startpos, Color color, Vector pos ) const
{
    if ( !g_options.visuals_bullet_tracers )
        return;

    const auto colorbeams = g_options.visuals_beams_bullet_tracer;
    BeamInfo_t beam_info;
    beam_info.m_nType = TE_BEAMPOINTS;
    beam_info.m_pszModelName = xor_str ( "sprites/physbeam.vmt" );
    beam_info.m_nModelIndex = interfaces::mdl_info->get_model_index ( xor_str ( "materials/sprites/physbeam.vmt" ) );
    beam_info.m_flHaloScale = 0.f;
    beam_info.m_fle = 4.f;
    beam_info.m_flWidth = 2.f;
    beam_info.m_flEndWidth = 2.f;
    beam_info.m_flFadeLength = 0.1f;
    beam_info.m_flAmplitude = 2.f;
    beam_info.m_flBrightness = color.a( );
    beam_info.m_flSpeed = 0.2f;
    beam_info.m_nStartFrame = 0;
    beam_info.m_flFrameRate = 0.f;
    beam_info.m_flRed = colorbeams.r( );
    beam_info.m_flGreen = colorbeams.g( );
    beam_info.m_flBlue = colorbeams.b( );
    beam_info.m_flBrightness = colorbeams.a( );
    beam_info.m_nSegments = 2;
    beam_info.m_bRenderable = true;
    beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

    static auto last_r = Vector ( g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ) );
    static auto last_time = interfaces::global_vars->curtime;

    if ( last_time != interfaces::global_vars->curtime )
    {
        last_r = Vector ( g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ), g_math.random_float ( -0.5f, 0.5f ) );
        last_time = interfaces::global_vars->curtime;
    }

    beam_info.m_vecStart = startpos + last_r;
    beam_info.m_vecEnd = pos;

    auto beam = interfaces::render_beams->CreateBeamPoints ( beam_info );

    if ( beam )
        interfaces::render_beams->DrawBeam ( beam );
}

void c_event_logger::draw_footstep_beam ( C_BasePlayer* player ) const
{
    if ( !g_options.visuals_footstep_esp )
        return;

    auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( !local || !player || player->is_dormant( ) || local == player || !player->is_enemy( ) )
        return;

    const auto color = g_options.visuals_footstep_esp_color;

    BeamInfo_t beam_info;
    beam_info.m_nType = TE_BEAMRINGPOINT;
    beam_info.m_pszModelName = xor_str ( "sprites/physbeam.vmt" );
    beam_info.m_nModelIndex = interfaces::mdl_info->get_model_index ( xor_str ( "sprites/physbeam.vmt" ) );
    beam_info.m_nHaloIndex = -1;
    beam_info.m_flHaloScale = 3;
    beam_info.m_fle = 2.f;
    beam_info.m_flWidth = 4.f;
    beam_info.m_flFadeLength = 1.f;
    beam_info.m_flAmplitude = 0.f;
    beam_info.m_flRed = color.r( );
    beam_info.m_flGreen = color.g( );
    beam_info.m_flBlue = color.b( );
    beam_info.m_flBrightness = color.a( );
    beam_info.m_flSpeed = 0.f;
    beam_info.m_nStartFrame = 0.f;
    beam_info.m_flFrameRate = 60.f;
    beam_info.m_nSegments = -1;
    beam_info.m_nFlags = FBEAM_FADEOUT;
    beam_info.m_vecCenter = player->vec_origin( ) + Vector ( 0, 0, 5 );
    beam_info.m_flStartRadius = 5.f;
    beam_info.m_flEndRadius = g_options.visuals_beam_ring_size;

    auto beam = interfaces::render_beams->CreateBeamRingPoint ( beam_info );

    if ( beam )
        interfaces::render_beams->DrawBeam ( beam );
}

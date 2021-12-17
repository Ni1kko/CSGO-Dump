#include "engine_prediction.h"
#include "../../helpers/globals.h"

static char* cMoveData;

void c_engine_prediction::run_engine_pred ( usercmd_t* cmd )
{
    if ( !g_local )
        return;

    static usercmd_t* pLastCmd;

    // get random_seed as its 0 in clientmode->createmove
    //const auto get_random_seed = [] ( usercmd_t * p_cmd )
    //{
    //    using MD5_PseudoRandomFn = unsigned long ( __cdecl* ) ( std::uintptr_t );
    //    static auto offset = g_utils.pattern_scan ( GetModuleHandleA ( xor_str ( "client_panorama.dll" ) ), xor_str ( "55 8B EC 83 E4 F8 83 EC 70 6A 58" ) );
    //    static auto MD5_PseudoRandom = reinterpret_cast<MD5_PseudoRandomFn> ( offset );
    //    return MD5_PseudoRandom ( p_cmd->command_number ) & 0x7FFFFFFF;

    //};

    pLastCmd = cmd;
    flOldCurtime = interfaces::global_vars->curtime;
    flOldFrametime = interfaces::global_vars->frametime;
    ctx::cheat.unpredicted_velocity = g_local->vec_velocity( );

    //globals::random_seed = get_random_seed(cmd);
    globals.curtime = g_utils.get_estimate_server_time ( cmd );
    interfaces::global_vars->curtime = globals.curtime; //nTickBase * interfaces::global_vars->interval_per_tick;
    interfaces::global_vars->frametime = interfaces::global_vars->interval_per_tick;

    //interfaces::game_movement->start_track_prediction_errors ( g_local );

    //CMoveData data;
    //memset ( &data, 0, sizeof ( CMoveData ) );

    //interfaces::move_helper->set_host ( g_local );
    //interfaces::prediction->setup_move ( g_local, cmd, interfaces::move_helper, &data );
    //interfaces::game_movement->process_movement ( g_local, &data );
    //interfaces::prediction->finish_move ( g_local, cmd, &data );

    if ( !interfaces::engine_client->is_connected( ) && !interfaces::engine_client->is_in_game( ) )
        return;

    //Nullptr'ing is fun!
    if ( !interfaces::move_helper || !cmd || !g_local || !g_local->is_alive( ) )
        return;

    //We need to cast g_LocalPlayer to C_BasePlayer so we can use it in our code
    CMoveData C_MoveData;

    //Let's back things up for later use
    iFlagsBackup = g_local->flags( );
    iButtonsBackup = cmd->buttons;

    //Set m_pCurrentCommand to cmd. We'll set this to nullptr later.
    g_local->current_command( ) = cmd;

    //Let's get some signatures now
    if ( !m_pPredictionRandomSeed || !m_pSetPredictionPlayer )
    {
        m_pPredictionRandomSeed = *reinterpret_cast< int** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                      xor_str ( "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) ) + 2 );
        // A3 ? ? ? ? 66 0F 6E 86  / +1
        m_pSetPredictionPlayer = *reinterpret_cast< int** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                     xor_str ( "89 35 ? ? ? ? F3 0F 10 48 20" ) ) + 2 );
    }

    //We don't want to be calling this every tick, bad for framerate.
    //Thanks, zbe and Heep
    if ( !cMoveData )
        cMoveData = ( char* )( calloc ( 1, sizeof ( CMoveData ) ) );

    //Let's set it up so that it predicts ourselves
    interfaces::move_helper->set_host ( g_local );
    *m_pPredictionRandomSeed = MD5_PseudoRandom ( cmd->command_number ) & 0x7FFFFFFF; //crashing here!!!!!!! some nullptr
    *m_pSetPredictionPlayer = uintptr_t ( g_local );

    interfaces::global_vars->curtime = globals.curtime;
}

void c_engine_prediction::end_engine_pred ( ) const
{
    interfaces::game_movement->finish_track_prediction_errors ( g_local );
    interfaces::move_helper->set_host ( nullptr );

    interfaces::global_vars->curtime = flOldCurtime;
    interfaces::global_vars->frametime = flOldFrametime;
}

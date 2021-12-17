#include "aimbot.h"

void aimbot::create_move( ) {
	g_csgo.m_engine( )->GetViewAngles( m_engine_angles );
	m_local_weapon = g_ctx.m_local->m_hActiveWeapon( ).Get( );

	auto weapon_recoil_scale = g_csgo.m_cvar( )->FindVar( "weapon_recoil_scale" );
	m_recoil_scale = weapon_recoil_scale->GetFloat( );

	if ( !m_local_weapon )
		return;

	update_config( );

	if ( ( g_ctx.m_local->m_flNextAttack( ) > util::server_time( ) ) )
		return;

	if ( m_local_weapon->is_non_aim( ) || m_local_weapon->m_iClip1( ) < 1 )
		return;

	if ( !m_local_weapon->can_fire( ) )
		return;

	for ( int i = 1; i <= g_csgo.m_globals( )->m_maxclients; i++ ) {
		auto e = static_cast< player_t * >( g_csgo.m_entitylist( )->GetClientEntity( i ) );

		if ( !e->valid( true ) )
			continue;

		matrix3x4_t matrix[ 128 ];
	}
}

void aimbot::update_config( ) {
	auto id = m_local_weapon->m_iItemDefinitionIndex( );

	int element = -1;

	if ( m_local_weapon->is_pistol( ) && id != WEAPON_DEAGLE )
		element = 0;

	if ( m_local_weapon->m_iItemDefinitionIndex( ) == WEAPON_DEAGLE )
		element = 1;

	if ( m_local_weapon->is_smg( ) )
		element = 3;

	if ( id == WEAPON_AWP || id == WEAPON_SSG08 )
		element = 4;

	if ( id == WEAPON_SCAR20 || id == WEAPON_G3SG1 )
		element = 5;

	if ( m_local_weapon->is_shotgun( ) )
		element = 6;

	if ( element == -1 )
		element = 2;

	config.m_autoshoot = g_cfg.ragebot.autoshoot[ element ];
	config.m_autowall = g_cfg.ragebot.autowall[ element ];
	config.m_minimum_damage = g_cfg.ragebot.minimum_damage[ element ];
	config.m_hitscan = g_cfg.ragebot.hitscan[ element ];
	config.m_hitscan_history = g_cfg.ragebot.hitscan_history[ element ];
	config.m_multipoint = g_cfg.ragebot.multipoint_hitboxes[ element ];
	config.m_pointscale = g_cfg.ragebot.pointscale[ element ];
	config.m_hitchance = g_cfg.ragebot.hitchance[ element ];
	config.m_hitchance_amount = g_cfg.ragebot.hitchance_amount[ element ];
	config.m_baim = g_cfg.ragebot.baim_settings[ element ];
	config.m_quickstop = g_cfg.ragebot.quickstop[ element ];
		 
	config.m_autoscope = false;

	if ( element == 4 || element == 5 )
		config.m_autoscope = g_cfg.ragebot.autoscope[ element - 3 ];
	else if ( element == 2 && ( id == WEAPON_AUG || id == WEAPON_SG556 ) )
		config.m_autoscope = g_cfg.ragebot.autoscope[ 0 ];
}










































#include "config.hpp"
#include "options.hpp"
#include "helpers/math.hpp"
#include "helpers/utils.hpp"
#include "menu.hpp"
#include "helpers/input.hpp"

// For ints, chars, floats, and bools
void CConfig::SetupValue( int& value, int def, std::string category, std::string name ) { value = def; ints.push_back( new ConfigValue< int >( category, name, &value, def ) ); }
void CConfig::SetupValue( char* value, char* def, std::string category, std::string name ) { value = def; chars.push_back( new ConfigValue< char >( category, name, value, *def ) ); }
void CConfig::SetupValue( float& value, float def, std::string category, std::string name ) { value = def; floats.push_back( new ConfigValue< float >( category, name, &value, def ) ); }
void CConfig::SetupValue( bool& value, bool def, std::string category, std::string name ) { value = def; bools.push_back( new ConfigValue< bool >( category, name, &value, def ) ); }
void CConfig::SetupColor( float value[4], std::string name ) {
	SetupValue( value[0], value[0], ( "colors" ), name + "_r" );
	SetupValue( value[1], value[1], ( "colors" ), name + "_g" );
	SetupValue( value[2], value[2], ( "colors" ), name + "_b" );
	SetupValue( value[3], value[3], ( "colors" ), name + "_a" );
}

void CConfig::SetupLegit( ) {
	for( auto&[key, val] : weapon_names ) {
		SetupValue( g_Options.legitbot_items[key].enabled, false, ( val ), ( "enabled" ) );
		SetupValue( g_Options.legitbot_items[key].deathmatch, false, ( val ), ( "deathmatch" ) );
		SetupValue( g_Options.legitbot_items[key].autopistol, false, ( val ), ( "autopistol" ) );
		SetupValue( g_Options.legitbot_items[key].smoke_check, false, ( val ), ( "smoke_check" ) );
		SetupValue( g_Options.legitbot_items[key].flash_check, false, ( val ), ( "flash_check" ) );
		SetupValue( g_Options.legitbot_items[key].jump_check, false, ( val ), ( "jump_check" ) );
		SetupValue( g_Options.legitbot_items[key].autowall, false, ( val ), ( "autowall" ) );
		SetupValue( g_Options.legitbot_items[key].silent, false, ( val ), ( "silent" ) );
		SetupValue( g_Options.legitbot_items[key].autofire, false, (val), ("autofire") );
		SetupValue( g_Options.legitbot_items[key].on_key, true, (val), ("on_key") );
		SetupValue( g_Options.legitbot_items[key].rcs, false, ( val ), ( "rcs" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_fov_enabled, false, ( val ), ( "rcs_fov_enabled" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_smooth_enabled, false, ( val ), ( "rcs_smooth_enabled" ) );
		SetupValue( g_Options.legitbot_items[key].autostop, false, ( val ), ( "autostop" ) );
		SetupValue( g_Options.legitbot_items[key].backtrack_time, 0.0f, ( val ), ( "backtrack_time" ) );
		SetupValue( g_Options.legitbot_items[key].only_in_zoom, false, ( val ), ( "only_in_zoom" ) );
		SetupValue( g_Options.legitbot_items[key].autofire_key, 1, (val), ("autofire_key") );
		SetupValue( g_Options.legitbot_items[key].key, 1, (val), ("key") );
		SetupValue( g_Options.legitbot_items[key].aim_type, 1, ( val ), ( "aim_type" ) );
		SetupValue( g_Options.legitbot_items[key].priority, 0, ( val ), ( "priority" ) );
		SetupValue( g_Options.legitbot_items[key].fov_type, 0, ( val ), ( "fov_type" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_type, 0, ( val ), ( "rcs_type" ) );
		SetupValue( g_Options.legitbot_items[key].smooth_type, 0, ( val ), ( "smooth_type" ) );
		SetupValue( g_Options.legitbot_items[key].hitbox, 1, ( val ), ( "hitbox" ) );
		SetupValue( g_Options.legitbot_items[key].fov, 0.f, ( val ), ( "fov" ) );
		SetupValue( g_Options.legitbot_items[key].silent_fov, 0.f, ( val ), ( "silent_fov" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_fov, 0.f, ( val ), ( "rcs_fov" ) );
		SetupValue( g_Options.legitbot_items[key].smooth, 1, ( val ), ( "smooth" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_smooth, 1, ( val ), ( "rcs_smooth" ) );
		SetupValue( g_Options.legitbot_items[key].shot_delay, 0, ( val ), ( "shot_delay" ) );
		SetupValue( g_Options.legitbot_items[key].kill_delay, 0, ( val ), ( "kill_delay" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_x, 100, ( val ), ( "rcs_x" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_y, 100, ( val ), ( "rcs_y" ) );
		SetupValue( g_Options.legitbot_items[key].rcs_start, 1, ( val ), ( "rcs_start" ) );
		SetupValue( g_Options.legitbot_items[key].min_damage, 1, ( val ), ( "min_damage" ) );
	}
}

void CConfig::SetupVisuals( ) {
	SetupValue( g_Options.esp_enabled, false, ( "esp" ), ( "enabled" ) );
	SetupValue( g_Options.esp_enemies_only, false, ( "esp" ), ( "enemies_only" ) );
	SetupValue( g_Options.esp_player_skeleton, false, ( "esp" ), ( "player_skeleton" ) );
	SetupValue( g_Options.esp_player_boxes, false, ( "esp" ), ( "player_boxes" ) );
	SetupValue( g_Options.esp_player_boxes_type, 0, ( "esp" ), ( "player_boxes_type" ) );
	SetupValue( g_Options.esp_player_names, false, ( "esp" ), ( "player_names" ) );
	SetupValue( g_Options.esp_player_health, false, ( "esp" ), ( "player_health" ) );
	SetupValue( g_Options.esp_player_armour, false, ( "esp" ), ( "player_armour" ) );
	SetupValue( g_Options.esp_player_weapons, false, ( "esp" ), ( "player_weapons" ) );
	SetupValue( g_Options.esp_dropped_weapons, false, ( "esp" ), ( "dropped_weapons" ) );
	SetupValue( g_Options.esp_defuse_kit, false, ( "esp" ), ( "defuse_kit" ) );
	SetupValue( g_Options.esp_planted_c4, false, ( "esp" ), ( "planted_c4" ) );
	SetupValue( g_Options.esp_items, false, ( "esp" ), ( "items" ) );
	SetupValue( g_Options.esp_grenade_prediction, false, ( "esp" ), ( "grenade_prediction" ) );
	SetupValue( g_Options.esp_backtrack, false, ( "esp" ), ( "backtrack" ) );
	SetupValue( g_Options.esp_sounds, false, ( "esp" ), ( "sounds" ) );
	SetupValue( g_Options.esp_sounds_time, 0.5f, ( "esp" ), ( "sounds_time" ) );
	SetupValue( g_Options.esp_sounds_radius, 15.0f, ( "esp" ), ( "sounds_radius" ) );
	SetupValue( g_Options.esp_choke_indicator, false, ( "esp" ), ( "choke_indicator" ) );
	SetupValue( g_Options.esp_angle_lines, false, ( "esp" ), ( "angle_lines" ) );
	SetupValue( g_Options.esp_hitmarker, false, ( "esp" ), ( "hitmarker" ) );

	SetupValue( g_Options.glow_enabled, false, ( "glow" ), ( "enabled" ) );
	SetupValue( g_Options.glow_enemies_only, false, ( "glow" ), ( "enemies_only" ) );
	SetupValue( g_Options.glow_players, false, ( "glow" ), ( "players" ) );
	SetupValue( g_Options.glow_chickens, false, ( "glow" ), ( "chickens" ) );
	SetupValue( g_Options.glow_c4_carrier, false, ( "glow" ), ( "c4_carrier" ) );
	SetupValue( g_Options.glow_planted_c4, false, ( "glow" ), ( "planted_c4" ) );
	SetupValue( g_Options.glow_defuse_kits, false, ( "glow" ), ( "defuse_kits" ) );
	SetupValue( g_Options.glow_weapons, false, ( "glow" ), ( "weapons" ) );

	SetupValue( g_Options.chams_player_enabled, false, ( "chams" ), ( "player_enabled" ) );
	SetupValue( g_Options.chams_player_enemies_only, false, ( "chams" ), ( "player_enemies_only" ) );
	SetupValue( g_Options.chams_player_wireframe, false, ( "chams" ), ( "player_wireframe" ) );
	SetupValue( g_Options.chams_player_flat, false, ( "chams" ), ( "player_flat" ) );
	SetupValue( g_Options.chams_player_ignorez, false, ( "chams" ), ( "player_ignorez" ) );
	SetupValue( g_Options.chams_player_glass, false, ( "chams" ), ( "player_glass" ) );
	SetupValue( g_Options.chams_disable_occulusion, false, ( "chams" ), ( "disable_occulusion" ) );
	SetupValue( g_Options.chams_backtrack, 0, ( "chams" ), ( "backtrack" ) );
	SetupValue( g_Options.chams_backtrack_flat, false, ( "chams" ), ( "backtrack_flat" ) );
	SetupValue( g_Options.chams_desync, false, ( "chams" ), ( "desync" ) );

	SetupValue(g_Options.other_nightmode, false, ("other"), ("nightmode"));
	SetupValue(g_Options.other_nightmode_size, 0.2f, ("other"), ("nightmode_size"));
	SetupValue(g_Options.other_mat_ambient_light_r, false, ("other"), ("mat_ambient_light_r"));
	SetupValue(g_Options.other_mat_ambient_light_g, false, ("other"), ("mat_ambient_light_g"));
	SetupValue(g_Options.other_mat_ambient_light_b, false, ("other"), ("mat_ambient_light_b"));
	SetupValue(g_Options.other_mat_ambient_light_rainbow, false, ("other"), ("mat_ambient_light_rainbow"));

	SetupValue( g_Options.other_drawfov, false, ( "other" ), ( "drawfov" ) );
	SetupValue( g_Options.other_no_hands, false, ( "other" ), ( "no_hands" ) );
	SetupValue( g_Options.other_no_smoke, false, ( "other" ), ( "no_smoke" ) );
	SetupValue( g_Options.other_no_flash, false, ( "other" ), ( "no_flash" ) );
}

void CConfig::SetupMisc( ) {
	SetupValue( g_Options.misc_bhop, false, ( "misc" ), ( "bhop" ) );
	SetupValue( g_Options.misc_autostrafe, false, ( "misc" ), ( "autostrafe" ) );
	SetupValue( g_Options.misc_edgejump, false, ( "misc" ), ( "edgejump" ) );
	SetupValue( g_Options.misc_spectatorlist, false, ( "misc" ), ( "spectatorlist" ) );
	SetupValue( g_Options.misc_radar, false, ( "misc" ), ( "radar" ) );
	SetupValue( g_Options.misc_radar_enemyonly, false, ( "misc" ), ( "radar_enemyonly" ) );
	SetupValue( g_Options.misc_thirdperson, false, ( "misc" ), ( "thirdperson" ) );
	SetupValue( g_Options.misc_thirdperson_bind, 0, ( "misc" ), ( "thirdperson_bind" ) );
	SetupValue( g_Options.misc_showranks, false, ( "misc" ), ( "showranks" ) );
	SetupValue( g_Options.misc_clantag, false, ("misc"), ("clantag"));
	SetupValue( g_Options.misc_watermark, false, ( "misc" ), ( "watermark" ) );
	SetupValue( g_Options.misc_desync, 0, ( "misc" ), ( "desync" ) );
	SetupValue( g_Options.misc_desync_bind, 0, ( "misc" ), ( "desync_bind" ) );
	//SetupValue( g_Options.misc_fakeduck_bind, false, ( "misc" ), ( "fakeduck_bind" ) );
	//SetupValue( g_Options.misc_fakeduck_ticks, 0, ( "misc" ), ( "fakeduck_ticks" ) );
	//SetupValue( g_Options.misc_block_ducked_attack, false, ( "misc" ), ( "block_ducked_attack" ) );
	SetupValue( g_Options.misc_thirdperson_dist, 50.f, ( "misc" ), ( "thirdperson_dist" ) );
	SetupValue( g_Options.misc_override_fov, 90, ( "misc" ), ( "override_fov" ) );
	SetupValue( g_Options.misc_viewmodel_fov, 68, ( "misc" ), ( "viewmodel_fov" ) );
	SetupValue( g_Options.misc_retrack_speed, 2.f, ( "misc" ), ( "misc_retrack_speed" ) );

	SetupValue( g_Options.misc_slowwalk_bind, false, ( "misc" ), ( "slowwalk_bind" ) );
	SetupValue( g_Options.misc_slowwalk_speed, 1.0f, ( "misc" ), ( "slowwalk_speed" ) );

	SetupValue( g_Options.fakelag_enabled, false, ( "fakelag" ), ( "enabled" ) );
	SetupValue( g_Options.fakelag_standing, false, ( "fakelag" ), ( "standing" ) );
	SetupValue( g_Options.fakelag_moving, false, ( "fakelag" ), ( "moving" ) );
	SetupValue( g_Options.fakelag_unducking, false, ( "fakelag" ), ( "unducking" ) );
	SetupValue( g_Options.fakelag_mode, 0, ( "fakelag" ), ( "mode" ) );
	SetupValue( g_Options.fakelag_factor, 0, ( "fakelag" ), ( "factor" ) );
}

void CConfig::SetupSkins() {
	//for (auto&[key, val] : k_weapon_names) {
	for (auto& key : k_weapon_names) {
		SetupValue(g_Options.skins.m_items[key.definition_index].name, "Default",( key.name ), ("name"));
		SetupValue(g_Options.skins.m_items[key.definition_index].enabled, 0, (key.name), ("enabled"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_vector_index, 0, ( key.name ), ("definition_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_index, 1, (key.name), ("definition_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].paint_kit_vector_index, 0, (key.name), ("paint_kit_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].paint_kit_index, 0, (key.name), ("paint_kit_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_override_vector_index, 0, (key.name), ("definition_override_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_override_index, 0, (key.name), ("definition_override_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].seed, 0, (key.name), ("seed"));
		SetupValue(g_Options.skins.m_items[key.definition_index].stat_trak, 0, (key.name), ("stat_trak"));
		SetupValue(g_Options.skins.m_items[key.definition_index].wear, 0.0f, (key.name), ("wear"));
		//SetupValue(g_Options.skins.m_items[key.definition_index].custom_name, "", (key.name), ("custom_name"));
	}
}

void CConfig::SetupColors( ) {
	SetupColor( g_Options.color_esp_ally_visible, "color_esp_ally_visible" );
	SetupColor( g_Options.color_esp_enemy_visible, "color_esp_enemy_visible" );
	SetupColor( g_Options.color_esp_ally_occluded, "color_esp_ally_occluded" );
	SetupColor( g_Options.color_esp_enemy_occluded, "color_esp_enemy_occluded" );
	SetupColor( g_Options.color_esp_enemy_occluded, "color_esp_enemy_occluded" );
	SetupColor( g_Options.color_esp_weapons, "color_esp_weapons" );
	SetupColor( g_Options.color_esp_defuse, "color_esp_defuse" );
	SetupColor( g_Options.color_esp_c4, "color_esp_c4" );
	SetupColor( g_Options.color_esp_item, "color_esp_item" );
	SetupColor( g_Options.color_glow_ally, "color_glow_ally" );
	SetupColor( g_Options.color_glow_enemy, "color_glow_enemy" );
	SetupColor( g_Options.color_glow_chickens, "color_glow_chickens" );
	SetupColor( g_Options.color_glow_c4_carrier, "color_glow_c4_carrier" );
	SetupColor( g_Options.color_glow_planted_c4, "color_glow_planted_c4" );
	SetupColor( g_Options.color_glow_defuse, "color_glow_defuse" );
	SetupColor( g_Options.color_glow_weapons, "color_glow_weapons" );
	SetupColor( g_Options.color_chams_player_ally_visible, "color_chams_player_ally_visible" );
	SetupColor( g_Options.color_chams_player_ally_occluded, "color_chams_player_ally_occluded" );
	SetupColor( g_Options.color_chams_player_enemy_visible, "color_chams_player_enemy_visible" );
	SetupColor( g_Options.color_chams_player_enemy_occluded, "color_chams_player_enemy_occluded" );
	SetupColor( g_Options.color_chams_backtrack, "color_chams_backtrack" );
	SetupColor( g_Options.color_chams_desync, "color_chams_desync" );
	SetupColor( g_Options.color_grenade_prediction, "color_grenade_prediction" );
	SetupColor( g_Options.color_esp_sounds, "color_esp_sounds" );
}

void CConfig::Setup( ) {
	CConfig::SetupLegit( );
	CConfig::SetupVisuals( );
	CConfig::SetupMisc( );
	CConfig::SetupSkins();
	CConfig::SetupColors( );
}

#include "helpers/utils.hpp"
void CConfig::Save( const std::string& name ) {
	if( name.empty( ) )
		return;

	CreateDirectoryA( u8"C:\\Amnesia\\", NULL );
	std::string file = u8"C:\\Amnesia\\" + name;

	for( auto value : ints ) {
		WritePrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), std::to_string( *value->value ).c_str( ), file.c_str( ) );
	}

	for( auto value : floats ) WritePrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), std::to_string( *value->value ).c_str( ), file.c_str( ) );
	for( auto value : bools ) WritePrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), *value->value ? "true" : "false", file.c_str( ) );
}

void CConfig::Load( const std::string& name ) {
	if( name.empty( ) )
		return;

	g_ClientState->ForceFullUpdate();

	CreateDirectoryA( u8"C:\\Amnesia\\", NULL );
	std::string file = u8"C:\\Amnesia\\" + name;

	char value_l[32] = { '\0' };
	for( auto value : ints ) {
		GetPrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), "0", value_l, 32, file.c_str( ) ); *value->value = atoi( value_l );
	}

	for( auto value : floats ) {
		GetPrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), "0.0f", value_l, 32, file.c_str( ) ); *value->value = atof( value_l );
	}

	for( auto value : bools ) {
		GetPrivateProfileStringA( value->category.c_str( ), value->name.c_str( ), "false", value_l, 32, file.c_str( ) ); *value->value = !strcmp( value_l, "true" );
	}
}

CConfig* Config = new CConfig( );

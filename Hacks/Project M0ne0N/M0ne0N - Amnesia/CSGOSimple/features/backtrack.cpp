#include "backtrack.hpp"
#include "aimbot.hpp"
#include "../helpers/math.hpp"

void Backtrack::OnMove( CUserCmd *pCmd ) {
	if( !g_EngineClient->IsInGame( ) || !g_LocalPlayer || !g_LocalPlayer->IsAlive( ) ) {
		data.clear( );
		return;
	}

	auto pWeapon = g_LocalPlayer->m_hActiveWeapon( );
	if( !pWeapon || !pWeapon->IsGun( ) ) {
		data.clear( );
		return;
	}

	auto weaponData = pWeapon->GetCSWeaponData( );
	g_Aimbot.settings = g_Options.legitbot_items[pWeapon->m_Item( ).m_iItemDefinitionIndex( )];
	if( !g_Aimbot.settings.enabled ) {
		data.clear( );
		return;
	}

	static ConVar* sv_maxunlag = g_CVar->FindVar( "sv_maxunlag" );
	static ConVar* sv_minupdaterate = g_CVar->FindVar( "sv_minupdaterate" );
	static ConVar* sv_maxupdaterate = g_CVar->FindVar( "sv_maxupdaterate" );

	static ConVar* sv_client_min_interp_ratio = g_CVar->FindVar( "sv_client_min_interp_ratio" );
	static ConVar* sv_client_max_interp_ratio = g_CVar->FindVar( "sv_client_max_interp_ratio" );

	static ConVar* cl_interp_ratio = g_CVar->FindVar( "cl_interp_ratio" );
	static ConVar* cl_interp = g_CVar->FindVar( "cl_interp" );
	static ConVar* cl_updaterate = g_CVar->FindVar( "cl_updaterate" );

	float updaterate = cl_updaterate->GetFloat( );

	float minupdaterate = sv_minupdaterate->GetFloat( );
	float maxupdaterate = sv_maxupdaterate->GetFloat( );

	float min_interp = sv_client_min_interp_ratio->GetFloat( );
	float max_interp = sv_client_max_interp_ratio->GetFloat( );

	float flLerpAmount = cl_interp->GetFloat( );
	float flLerpRatio = cl_interp_ratio->GetFloat( );
	flLerpRatio = std::clamp( flLerpRatio, min_interp, max_interp );
	if( flLerpRatio == 0.0f )
		flLerpRatio = 1.0f;

	float updateRate = std::clamp( updaterate, minupdaterate, maxupdaterate );
	lerp_time = std::fmaxf( flLerpAmount, flLerpRatio / updateRate );
	latency = g_EngineClient->GetNetChannelInfo( )->GetLatency( FLOW_OUTGOING ) + g_EngineClient->GetNetChannelInfo( )->GetLatency( FLOW_INCOMING );
	correct_time = latency + lerp_time;

	// setup records
	C_BasePlayer* player;
	for( int i = 1; i <= g_GlobalVars->maxClients; ++i ) {
		player = C_BasePlayer::GetPlayerByIndex( i );
		if( player->IsNotTarget( ) ) {
			if( data.count( i ) > 0 )
				data.erase( i );
			continue;
		}

		if( !g_Aimbot.settings.deathmatch && player->m_iTeamNum( ) == g_LocalPlayer->m_iTeamNum( ) ) {
			if( data.count( i ) > 0 )
				data.erase( i );
			continue;
		}

		auto& cur_data = data[i];
		if( !cur_data.empty( ) ) {
			auto& front = cur_data.front( );
			if( front.simTime == player->m_flSimulationTime( ) )
				continue;

			while( !cur_data.empty( ) ) {
				auto& back = cur_data.back( );
				float deltaTime = correct_time - ( g_GlobalVars->curtime - back.simTime );
				if( std::fabsf( deltaTime ) <= 0.2f )
					break;

				cur_data.pop_back( );
			}
		}

		auto model = player->GetModel( );
		if( !model )
			continue;

		auto hdr = g_MdlInfo->GetStudiomodel( model );
		if( !hdr )
			continue;

		auto hitbox_set = hdr->GetHitboxSet( player->m_nHitboxSet( ) );
		auto hitbox_head = hitbox_set->GetHitbox( HITBOX_HEAD );
		auto hitbox_center = ( hitbox_head->bbmin + hitbox_head->bbmax ) * 0.5f;

		backtrack_data bd;
		bd.simTime = player->m_flSimulationTime( );

		*( Vector* )( ( uintptr_t )player + 0xA0 ) = player->m_vecOrigin();
		*( int* )( ( uintptr_t )player + 0xA68 ) = 0;
		*( int* )( ( uintptr_t )player + 0xA30 ) = 0;
		player->InvalidateBoneCache( );
		player->SetupBones( bd.boneMatrix, 128, BONE_USED_BY_ANYTHING, g_GlobalVars->curtime );

		Math::VectorTransform( hitbox_center, bd.boneMatrix[hitbox_head->bone], bd.hitboxPos );

		data[i].push_front( bd );
	}

	Vector localEyePos = g_LocalPlayer->GetEyePos( );
	QAngle angles;
	int tick_count = -1;
	float best_fov = 180.0f;
	for( auto& node : data ) {
		auto& cur_data = node.second;
		if( cur_data.empty( ) )
			continue;

		for( auto& bd : cur_data ) {
			float deltaTime = correct_time - ( g_GlobalVars->curtime - bd.simTime );
			if( std::fabsf( deltaTime ) > g_Aimbot.settings.backtrack_time )
				continue;

			Math::VectorAngles( bd.hitboxPos - localEyePos, angles );
			Math::FixAngles( angles );
			float fov = g_Aimbot.GetFovToPlayer( pCmd->viewangles, angles );
			if( best_fov > fov ) {
				best_fov = fov;
				tick_count = TIME_TO_TICKS( bd.simTime + lerp_time );
			}
		}
	}

	if( tick_count != -1 ) {
		pCmd->tick_count = tick_count;
	}
}

Backtrack g_Backtrack;

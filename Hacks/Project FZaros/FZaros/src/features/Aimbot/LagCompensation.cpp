#include "LagCompensation.hpp"

CLagCompensation_new g_LagCompensation;

template<class T, class U>
T clamp( T in, U low, U high )
{
	if ( in <= low )
		return low;

	if ( in >= high )
		return high;

	return in;
}

float CLagCompensation_new::estimateServerTime( ) //ty polak
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = ( INetChannelInfo* )g_Interfaces->gameEngine->netchannelInfo( );
	v2 = ( INetChannelInfo* )g_Interfaces->gameEngine->netchannelInfo( );

	v3 = v1->GetAvgLatency( INetChannelInfo::LOCALPLAYER );
	v4 = v2->GetAvgLatency( INetChannelInfo::GENERIC );

	return v3 + v4 + TICKS_TO_TIME( 1 ) + TICKS_TO_TIME( Globals::oCmd->tickCount );
}

float CLagCompensation_new::networkLatency( )
{
	INetChannelInfo *nci = g_Interfaces->gameEngine->netchannelInfo( );
	if ( nci )
	{
		float OutgoingLatency = nci->GetLatency( 0 );
		return OutgoingLatency;
	}
	return 0.0f;
}

void CLagCompensation_new::storeRecords( CBaseEntity* pEntity )
{
	int i = pEntity->EntIndex( );
	for ( int j = PlayerRecord[ i ].records.size( ) - 1; j >= 0; --j )
	{
		tickRecord rec = PlayerRecord[ i ].records.at( j );

		if ( rec.m_flSimulationTime < g_Interfaces->globalVars->curtime - 0.2f )
			PlayerRecord[ i ].records.erase( PlayerRecord[ i ].records.begin( ) + j );
	}

	static Vector3 oldOrigin[ 64 ];
	if ( PlayerRecord[ i ].records.size( ) > 0 && pEntity->simulationTime( ) == PlayerRecord[ i ].records.back( ).m_flSimulationTime ) //already got such a record
		return;

	for ( int j = PlayerRecord[ i ].records.size( ) - 1; j >= 0; j-- )
	{
		tickRecord rec = PlayerRecord[ i ].records.at( j );

		switch (config->get_int("rageBacktrackType"))
		{
		case 0:
			rec = PlayerRecord[ i ].records.at( 0 );
			break;
		case 1:
			rec = PlayerRecord[ i ].records.at( j );
			break;
		}

		if ( PlayerRecord[ i ].records.size( ) > 0 && rec.m_flSimulationTime > pEntity->simulationTime( ) ) //Invalid lag record, maybe from diffrent game?
		{
			PlayerRecord[ i ].records.clear( );
			return;
		}

	}

	Vector3 curOrigin = pEntity->vecOrigin( );
	Vector3 v = curOrigin - oldOrigin[ i ];
	bool breaks_lagcomp = v.LengthSqr( ) > 4096.f;
	oldOrigin[ i ] = curOrigin;
	tickRecord newRecord;

	newRecord.needs_extrapolation = breaks_lagcomp;
	static float OldLower[ 64 ];

	PlayerRecord[ i ].lowerBodyYawTarget = pEntity->lowerBodyYawTarget( );
	newRecord.m_angEyeAngles = pEntity->eyeAngles( );

	newRecord.m_nFlags = pEntity->flags( );
	newRecord.m_absangles = pEntity->GetAbsAngles( );

	newRecord.m_flSimulationTime = pEntity->simulationTime( );
	newRecord.m_vecAbsOrigin = pEntity->GetAbsOrigin( );
	newRecord.bLowerBodyYawUpdated = false;
	newRecord.m_vecOrigin = pEntity->vecOrigin( );
	newRecord.m_vecVelocity = pEntity->velocity( );
	newRecord.m_vecMins = pEntity->GetCollideable( )->OBBMins( );
	newRecord.m_vecMax = pEntity->GetCollideable( )->OBBMaxs( );

	if ( PlayerRecord[ i ].lowerBodyYawTarget != OldLower[ i ] )
		newRecord.bLowerBodyYawUpdated = true;

	for ( int i = 0; i < 24; i++ )
		newRecord.m_flPoseParameter[ i ] = *( float* )( ( DWORD )pEntity + 0x2774 + sizeof( float ) * i );

	pEntity->SetupBones( newRecord.boneMatrix, 128, 256, g_Interfaces->globalVars->curtime );

	OldLower[ i ] = PlayerRecord[ i ].lowerBodyYawTarget;
	PlayerRecord[ i ].records.push_back( newRecord );
}

void CLagCompensation_new::clearRecords( )
{
	if ( !g_Interfaces->gameEngine->connected( ) && !g_Interfaces->gameEngine->inGame( ) )
	{
		if ( PlayerRecord->records.size( ) > 0 )
			PlayerRecord->records.clear( );
	}
}

bool CLagCompensation_new::isTickValid( tickRecord tick )
{
	signed int entity_tickcount = int( ( ( tick.m_flSimulationTime + getLerpTime( ) ) / g_Interfaces->globalVars->intervalPerTick ) + 0.5f );

	INetChannelInfo *nci = g_Interfaces->gameEngine->netchannelInfo( );

	float inc_avg_latency = nci->GetAvgLatency( 1 ),
		out_avg_latency = nci->GetAvgLatency( 0 ),
		tick_interval = 1.f / g_Interfaces->globalVars->intervalPerTick;

	signed int lerp_ticks = int( ( tick_interval * getLerpTime( ) ) + 0.5f ), v10 = 0x3F800000;
	int predicted_cmd_arrival = int( ( ( inc_avg_latency + out_avg_latency ) * tick_interval ) + 0.5f ) + g_Interfaces->globalVars->tickcount + 1;

	float out_latency = nci->GetLatency( 0 ),
		v12 = ( lerp_ticks * g_Interfaces->globalVars->intervalPerTick ) + out_latency;

	if ( v12 > 1.f || ( v10 = 0, v12 < 0.f ) )
		v12 = float( v10 );

	return v12 - ( ( predicted_cmd_arrival + lerp_ticks - entity_tickcount )
		* g_Interfaces->globalVars->intervalPerTick ) <= 0.2f;
}

float CLagCompensation_new::getLerpTime( )
{
	int updaterate = g_Interfaces->cvar->FindVar( "cl_updaterate" )->GetInt( );
	ConVar* minupdate = g_Interfaces->cvar->FindVar( "sv_minupdaterate" );
	ConVar* maxupdate = g_Interfaces->cvar->FindVar( "sv_maxupdaterate" );

	if ( minupdate && maxupdate )
		updaterate = maxupdate->GetInt( );

	float ratio = g_Interfaces->cvar->FindVar( "cl_interp_ratio" )->GetFloat( );

	if ( ratio == 0 )
		ratio = 1.0f;

	float lerp = g_Interfaces->cvar->FindVar( "cl_interp" )->GetFloat( );
	ConVar* cmin = g_Interfaces->cvar->FindVar( "sv_client_min_interp_ratio" );
	ConVar* cmax = g_Interfaces->cvar->FindVar( "sv_client_max_interp_ratio" );

	if ( cmin && cmax && cmin->GetFloat( ) != 1 )
		ratio = clamp( ratio, cmin->GetFloat( ), cmax->GetFloat( ) );

	return max( lerp, ( ratio / updaterate ) );
}
#include "resolver_recode.h"
#include "..\lagcompensation\lagcompensation.h"
#include "../autowall/autowall.h"

void resolver::fsn( ) {
	if (!g_ctx.m_local->is_alive( ) )
		return;
	if (!g_cfg.ragebot.antiaim_correction2)
		return;
	for ( int i = 1; i <= g_csgo.m_globals( )->m_maxclients; i++ ) {
		auto e = static_cast< player_t * >( g_csgo.m_entitylist( )->GetClientEntity( i ) );

		if ( !e->valid( true ) ) {
			g_ctx.m_globals.missed_shots[ i ] = 0;

			record_t temp;
			m_resolver_records[ i ].m_current = temp;
			m_resolver_records[ i ].m_records.clear( );

			continue;
		}

		m_resolver_records[ i ].e = e;
		m_resolver_records[ i ].update( );

		m_resolver_records[ i ].m_records.push_front( m_resolver_records[ i ].m_current );

		if ( m_resolver_records[ i ].m_records.size( ) >= 16 )
			m_resolver_records[ i ].m_records.pop_back( );

		m_resolver_records[ i ].resolve( );

		m_resolver_records[ i ].m_previous = m_resolver_records[ i ].m_current;
		m_resolver_records[ i ].m_initiazlied_previous = true;
	}
}
enum {
	CURRENT_LAYER,
	PREVIOUS_LAYER
};

bool float_near( float v1, float v2, float tolerance ) {
	return std::abs( v1 - v2 ) <= std::abs( tolerance );
}

void resolver::info_t::update( ) {
	float server_time = util::server_time( );

	m_current.layer_count = e->animlayer_count( );
	for ( int i = 0; i < m_current.layer_count; i++ )
		m_current.layers[ i ] = e->get_animlayers( )[ i ];

	m_current.tickcount = g_csgo.m_globals( )->m_tickcount;
	m_current.flags = e->m_fFlags( );
	m_current.velocity = e->m_vecVelocity( ).Length( );
	m_current.lowerbody = e->m_flLowerBodyYawTarget( );
	m_current.eye_angle = e->m_angEyeAngles( ).y;
	m_current.lby180 = m_current.lowerbody + 180;
	m_current.bodyeye_delta = m_current.eye_angle - m_current.lowerbody;

	m_current.at_target_yaw = math::calculate_angle( e->m_vecOrigin( ),
		g_ctx.m_local->m_vecOrigin( ) ).y;

	m_current.fakewalk = m_current.fakewalk_check( e );
	m_current.in_air = !( m_current.flags & FL_ONGROUND );
	m_current.moving = m_current.velocity >= 0.1;

	m_current.lby_updated = false;
	if ( !m_current.in_air && m_current.moving && m_current.velocity < 450.0f && !m_current.fakewalk ) {
		m_current.lby_updated = true;
		m_current.moving_lby = m_current.lowerbody;
	}

	if ( m_current.last_lby != m_current.lowerbody ) {
		m_current.lby_updated = true;
		m_current.last_lby = m_current.lowerbody;
	}

	m_current.breaking_lby = false;
	m_current.breaking_lby_u120 = false;

	AnimationLayer layers[ 2 ];

	if ( m_current.adjusting_balance( e, &layers[ CURRENT_LAYER ] ) &&
		m_current.adjusting_balance( e, &layers[ PREVIOUS_LAYER ] ) ) {

		if ( ( layers[ PREVIOUS_LAYER ].m_flCycle != layers[ CURRENT_LAYER ].m_flCycle ) || layers[ CURRENT_LAYER ].m_flWeight == 1.f )
			m_current.breaking_lby = true;

		if ( layers[ CURRENT_LAYER ].m_flWeight == 0.f && ( layers[ PREVIOUS_LAYER ].m_flCycle > 0.92f && layers[ CURRENT_LAYER ].m_flCycle > 0.92f ) )
			m_current.breaking_lby_u120 = true;
	}

	m_current.moving_lby_valid = m_current.moving_lby != INT_MAX;
}

void resolver::info_t::pitch_resolve() {
		e->m_angEyeAngles().x = e->m_angEyeAngles().x;
		if (!g_cfg.misc.anti_untrusted)
		{
			int shots = g_ctx.m_globals.missed_shots[e->EntIndex()];
			constexpr int pitch[4] = { -89, 89, 0, -89 };
			e->m_angEyeAngles().y = m_current.at_target_yaw + pitch[shots % 4];
		}
}
static int GetSequenceActivity(IClientEntity* pEntity, int sequence)
{
	const model_t* pModel = pEntity->GetModel();
	if (!pModel)
		return 0;

	auto hdr = g_csgo.m_modelinfo()->GetStudiomodel(pEntity->GetModel());

	if (!hdr)
		return -1;

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return get_sequence_activity(pEntity, hdr, sequence);
}
void resolver::info_t::resolve() {
	auto weapon = e->m_hActiveWeapon().Get();
	if (weapon) {
		if (weapon->m_iClip1() != m_current.last_ammo) {
			m_current.last_ammo = weapon->m_iClip1();
			return;
		}
	}
	const auto sim = e->m_flSimulationTime();
	auto i = e->EntIndex();
	bool did_shoot[64] = { false };
	float delta6[64] = { 0.f };
	for (int s = 0; s < 14; s++)
	{
		auto anim_layer = e->get_animlayer(s);
		if (!anim_layer.m_pOwner)
			continue;
		auto anime = &e->get_animlayers()[1];
		auto activity = GetSequenceActivity(e, anime->m_nSequence);

		if (activity == -1)
			continue;
		if (sim - last_sim[i] >= 1)
		{
			if (sim - last_sim[i] == 1 && !m_current.adjusting_balance(e, anime))
			{
				didsync[i] = true;
				nofake[i] = true;
			}
			last_sim[i] = sim;
		}
		if (fabs(last_sim[i] - sim) > 2.0f && e->m_vecVelocity().Length2D() < 0.1f)
		{
			last_sim[i] = 0.f;
			last_forced_shot[i] = 0.0f;
		}
		if (fabsf(last_forced_shot[i] - sim) > 0.1f && e->m_vecVelocity().Length2D() < 0.1f)
		{
			last_forced_shot[i] = sim;
			did_shoot[i] = true;
		} // dasmax codenz <3
		else
		{
			did_shoot[i] = false;
		}
		if (!m_current.moving)
		{

			if (anim_layer.m_flWeight == 1.f && m_current.adjusting_balance(e, anime))
			{
				highdelta[i] = true;
				lowdelta[i] = false;
				prebreak[i] = false;
			}

			else if (anim_layer.m_flWeight == 1.f && m_current.adjusting_balance(e, anime))
			{
				highdelta[i] = false;
				lowdelta[i] = false;
				prebreak[i] = true;
			}

			else if (anim_layer.m_flWeight == 0.f && !m_current.adjusting_balance(e, anime))
			{
				highdelta[i] = false;
				lowdelta[i] = true;
				prebreak[i] = false;
			}

			else if ((anim_layer.m_flWeight == 0.f && m_current.adjusting_balance(e, anime)) || (change[i] && delta6[i] < 120 && m_current.adjusting_balance(e, anime)) || did_shoot[i])
			{
				highdelta[i] = false;
				lowdelta[i] = false;
				prebreak[i] = true;
			}

			else
			{
				highdelta[i] = false;
				lowdelta[i] = false;
				prebreak[i] = false;
			}
			if (m_current.last_lby != m_current.lowerbody)
			{
				if (m_current.adjusting_balance(e, anime) && fabsf(m_current.lowerbody - m_current.last_lby) < 120 && anim_layer.m_flWeight == 0.f && !m_current.moving)
				{
					delta6[i] = fabsf(m_records.front().lowerbody - m_current.last_lby) - 35;
				}

				else
				{
					delta6[i] = fabsf(m_records.front().lowerbody - m_current.last_lby);
				}
			}

		}
	}
	int shots = g_ctx.m_globals.missed_shots[e->EntIndex()];

	if (m_records.size() > 1 && m_initiazlied_previous) {
		m_current.lby_comparing_tick = get_lby_comparing_ticks();
		m_current.delta_comparing_tick = get_delta_comparing_ticks();
		m_current.lbydelta = m_current.eye_angle - m_records.front().lowerbody;
		auto & player = lagcompensation::get().players[e->EntIndex()];

		if (m_current.lby_updated) {
			m_resolver_mode = "LBY";
			e->m_angEyeAngles().y = m_current.lowerbody;
		}
		else {
			if (m_current.in_air) {
				if (!m_current.moving_lby_valid) {
					m_resolver_mode = "BRUTE TARGET";

					constexpr int angles[4] = { 90, -90, 180, 0 };
					e->m_angEyeAngles().y = m_current.at_target_yaw + angles[shots % 4];
				}
				else {
					m_resolver_mode = "MOVE LBY BRUTE";
					e->m_angEyeAngles().y = m_current.moving_lby + (shots % 8 * 45);
				}
			}
			else {
				if (m_current.fakewalk) {
					if (m_current.moving && !m_current.in_air) {
						if (delta_keeps_changing(10.f)) {
							m_resolver_mode = "DELTA CHANGE";
							e->m_angEyeAngles().y = e->m_angEyeAngles().y - m_current.delta_comparing_tick;
						}
						else if (lby_keeps_changing(10.f)) {
							m_resolver_mode = "LBY CHANGE";
							e->m_angEyeAngles().y = m_current.lby_comparing_tick;
						}
						else {
							m_resolver_mode = "BRUTE TARGET";

							constexpr int angles[4] = { 90, -90, 180, 0 };
							e->m_angEyeAngles().y = m_current.at_target_yaw + angles[shots % 4];
						}
					}
				}
				else {
					AnimationLayer layers[2];
					if (m_current.breaking_lby) {
						if (m_current.lbydelta < 35 && m_current.lbydelta > -35) {
							m_resolver_mode = "LBY CALCULATE";
							e->m_angEyeAngles().y = m_current.delta_comparing_tick + 180;
						}
						else if (fabsf(m_current.bodyeye_delta) < 35.f && fabsf(m_current.bodyeye_delta) > 0.f) {
							m_resolver_mode = "BODYEYE DELTA";
							e->m_angEyeAngles().y = m_records.front().lowerbody + m_current.bodyeye_delta;
						}
						else {
							m_resolver_mode = "LBY 180";
							e->m_angEyeAngles().y = m_records.front().lowerbody + 180;
						}
					}
					else if (m_current.breaking_lby_u120) {
						if (m_current.lbydelta < 35 && m_current.lbydelta > -35) {
							m_resolver_mode = "LBY CALCULATE";
							e->m_angEyeAngles().y = m_current.delta_comparing_tick + 180;
						}
						else if (fabsf(m_current.bodyeye_delta) < 35.f && fabsf(m_current.bodyeye_delta) > 0.f) {
							m_resolver_mode = "BODYEYE DELTA";
							e->m_angEyeAngles().y = m_records.front().lowerbody + m_current.bodyeye_delta;
						}
						else {
							int fired[65];
							fired[e->EntIndex()] = g_ctx.m_globals.shots[e->EntIndex()];
							if (prebreak[e->EntIndex()]) // if they did 979, but their delta < 120
							{
								if (fired[e->EntIndex()] <= 2)
								{
									e->m_angEyeAngles().y = m_records.front().lowerbody - (120 * fired[e->EntIndex()] + 1);
								}
								else
									e->m_angEyeAngles().y = m_records.front().lowerbody - delta6[e->EntIndex()] - (15 * fired[e->EntIndex()]);
								m_resolver_mode = "979 DELTA < 120";
							}

							else if (lowdelta[e->EntIndex()] && !change[e->EntIndex()]) // if we did not see their lby change, but are balancing weight.
							{
								if (fired[e->EntIndex()] < 2)
									e->m_angEyeAngles().y = m_records.front().lowerbody - (60 * fired[e->EntIndex()] + 1);
								else
								{
									e->m_angEyeAngles().y = m_records.front().lowerbody + (35 * fired[e->EntIndex()]);
								}
								m_resolver_mode = "MAGIC";
							}

							else if (lowdelta[e->EntIndex()] && change[e->EntIndex()]) // if we saw their lby change and can grab delta
							{
								if (fired[e->EntIndex()] < 2)
									e->m_angEyeAngles().y = m_records.front().lowerbody - delta6[e->EntIndex()];
								else
									e->m_angEyeAngles().y = m_records.front().lowerbody - (75 * g_ctx.m_globals.missed_shots[e->EntIndex()]);
								m_resolver_mode = "DELTA";
							}

							else if (highdelta[e->EntIndex()] && !change[e->EntIndex()])
							{
								if (fired[e->EntIndex()] < 1)
									e->m_angEyeAngles().y = m_records.front().lowerbody - 135;
								else
								{

									e->m_angEyeAngles().y = m_records.front().lowerbody - (120 * fired[e->EntIndex()]); // backwards
								}
								m_resolver_mode = "HIGH DELTA";
							}

							else if (highdelta[e->EntIndex()] && change[e->EntIndex()])
							{
								if (fired[e->EntIndex()] < 2)
									e->m_angEyeAngles().y = m_records.front().lowerbody - delta6[e->EntIndex()];
								else
								{
									e->m_angEyeAngles().y = m_records.front().lowerbody - (delta6[e->EntIndex()] * fired[e->EntIndex()]);
								}
								m_resolver_mode = "HIGH DELTA & CHANGED LBY";
							}

							else if (change[e->EntIndex()] && !highdelta[e->EntIndex()] && !lowdelta[e->EntIndex()] && !prebreak[e->EntIndex()]) // just be sure
							{
								if (fired[e->EntIndex()] < 2)
									e->m_angEyeAngles().y = m_records.front().lowerbody - delta6[e->EntIndex()];
								else
									e->m_angEyeAngles().y = m_records.front().lowerbody - delta6[e->EntIndex()] - (20 * fired[e->EntIndex()]); // so if the delta is 90, it will be 90 - (20 x2, then 3) which is 40 and 60.
								m_resolver_mode = "MORE DELTA BS";
							}

							else if (!highdelta[e->EntIndex()] && !change[e->EntIndex()] && !prebreak[e->EntIndex()] && !lowdelta[e->EntIndex()])
							{
								if (fired[e->EntIndex()] < 2)
									e->m_angEyeAngles().y = m_records.front().lowerbody;
								else
								{
									e->m_angEyeAngles().y = m_records.front().lowerbody - 120;
									m_resolver_mode = "LBY - 120";
								}
								m_resolver_mode = "LBY";
							}
							else
							{
								m_resolver_mode = "LBY";
								e->m_angEyeAngles().y = m_records.front().lowerbody;
							}
						}
					}
					else {
						if (has_static_real(5)) {//changed from 15 to 5
							m_resolver_mode = "STATIC YAW";
							e->m_angEyeAngles().y = (m_records.front().lowerbody) + (math::random_float(0.f, 1.f) > 0.f ? 10 : -10);
						}
						else if (has_static_difference(5)) {	//changed from 15 to 5
							m_resolver_mode = "STATIC YAW FAKE";
							e->m_angEyeAngles().y = m_current.eye_angle - m_current.lbydelta;
						}
						else if (has_steady_difference(5)) {//changed from 15 to 5
							m_resolver_mode = "STEADY YAW";

							float
								tickdif = static_cast<float>(m_records.front().tickcount - m_records.at(1).tickcount),
								lbydif = delta(m_records.front().lowerbody, m_records.at(1).lowerbody),
								ntickdif = static_cast<float>(g_csgo.m_globals()->m_tickcount - m_records.front().tickcount);

							e->m_angEyeAngles().y = (lbydif / tickdif) * ntickdif;
						}
						else if (delta_keeps_changing(4)) {//changed from 15 to 4
							m_resolver_mode = "DELTA CHANGE";
							e->m_angEyeAngles().y = m_current.eye_angle - delta_keeps_changing(4);//changed from 15 to 4
						}
						else if (lby_keeps_changing(5.f)) {//changed from 15.f to 5.f
							m_resolver_mode = "LBY CHANGE";
							e->m_angEyeAngles().y = m_current.lby_comparing_tick;
						}
						else {
							if (m_current.moving_lby_valid) {
								m_resolver_mode = "MOVE LBY";
								e->m_angEyeAngles().y = m_current.moving_lby;
							}
							else {
								m_resolver_mode = "BRUTE TARGET";
								constexpr int angles[4] = { 90, -90, 180, 0 };
								e->m_angEyeAngles().y = m_current.at_target_yaw + angles[shots % 4];
							}
						}
					}

				}
			}
		}
	}
	else {
		m_resolver_mode = "LBY";
		e->m_angEyeAngles().y = m_current.lowerbody;
	}

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.ragebot.override_key)) {
		m_resolver_mode = "OVERRIDE";

		vec3_t screen;
		if (math::world_to_screen(e->m_vecOrigin(), screen)) {
			POINT mouse; GetCursorPos(&mouse);
			float delta = mouse.x - screen.x;

			if (delta > 0) e->m_angEyeAngles().y = m_current.at_target_yaw - 90;
			else e->m_angEyeAngles().y = m_current.at_target_yaw + 90;
		}
	}

	if (!g_cfg.misc.anti_untrusted)
		e->m_angEyeAngles().x = shots % 4 ? 89.f : -89.f;
}
float resolver::info_t::delta( float a, float b ) {
	return abs( math::normalize_yaw( a - b ) );
}

bool resolver::info_t::different( float a, float b, float tolerance ) {
	return ( delta( a, b ) > tolerance );
}

int resolver::info_t::get_different_lbys( float tolerance ) {
	std::vector<float> vec;

	for ( auto var : m_records ) {
		float curyaw = var.lowerbody;
		bool add = true;
		for ( auto fl : vec ) {
			if ( !different( curyaw, fl, tolerance ) )
				add = false;
		}
		if ( add )
			vec.push_back( curyaw );
	}

	return vec.size( );
}

float resolver::info_t::get_lby_comparing_ticks( ) {
	int
		modulo = 1,
		difangles = get_different_lbys( 15 ),
		inc = modulo * difangles;

	for ( auto var : m_records ) {
		for ( int lasttick = var.tickcount; lasttick < g_csgo.m_globals( )->m_tickcount; lasttick += inc ) {
			if ( lasttick == g_csgo.m_globals( )->m_tickcount )
				return var.tickcount;
		}
	}

	return 0.f;
}

bool resolver::info_t::lby_keeps_changing( float tolerance ) {
	return ( get_different_lbys( tolerance ) > 180 / 2 );
}


int resolver::info_t::get_different_deltas( float tolerance ) {
	std::vector<float> vec;

	for ( auto var : m_records ) {
		float curdelta = var.eye_angle - var.lowerbody;
		bool add = true;
		for ( auto fl : vec ) {
			if ( !different( curdelta, fl, tolerance ) )
				add = false;
		}
		if ( add )
			vec.push_back( curdelta );
	}

	return vec.size( );
}

float resolver::info_t::get_delta_comparing_ticks( ) {
	int
		modulo = 1,
		difangles = get_different_deltas( 15 ),
		inc = modulo * difangles;

	for ( auto var : m_records ) {
		for ( int lasttick = var.tickcount; lasttick < g_csgo.m_globals( )->m_tickcount; lasttick += inc ) {
			if ( lasttick == g_csgo.m_globals( )->m_tickcount )
				return var.eye_angle - var.lowerbody;
		}
	}

	return 0.f;
}

bool resolver::info_t::delta_keeps_changing( float tolerance ) {
	return ( get_different_deltas( tolerance ) > 180 / 2 );
}

bool resolver::info_t::has_static_real( float tolerance ) {
	auto minmax = std::minmax_element( std::begin( m_records ), std::end( m_records ), [ ] ( const record_t t1, const record_t t2 ) { return t1.lowerbody < t2.lowerbody; } );
	return ( fabsf( ( *minmax.first ).lowerbody - ( *minmax.second ).lowerbody ) <= tolerance );
}

bool resolver::info_t::has_static_difference( float tolerance ) {
	auto lby_delta = [ ] ( const record_t record ) -> float {
		return record.eye_angle - record.lowerbody;
	};

	for ( auto i = m_records.begin( ); i < m_records.end( ) - 1;) {
		if ( delta( lby_delta( *i ), lby_delta( *++i ) ) > tolerance )
			return false;
	}

	return true;
}

bool resolver::info_t::has_steady_difference( float tolerance ) {
	size_t misses = 0;

	for ( size_t i = 0; i < m_records.size( ) - 1; i++ ) {
		float
			tickdif = static_cast< float >( m_records.at( i ).simtime - m_records.at( i + 1 ).tickcount ),
			lbydif = delta( m_records.at( i ).lowerbody, m_records.at( i + 1 ).lowerbody ),
			ntickdif = static_cast< float >( g_csgo.m_globals( )->m_tickcount - m_records.at( i ).tickcount );

		if ( ( ( lbydif / tickdif ) * ntickdif ) > tolerance ) misses++;
	}

	return ( misses <= ( m_records.size( ) / 3 ) );
}

bool resolver::record_t::adjusting_balance( player_t * e, AnimationLayer * set ) {
	const auto activity = e->sequence_activity( layers[ 3 ].m_nSequence );

	if ( activity == 979 ) {
		*set = layers[ 3 ];

		return true;
	}

	return false;
}

bool resolver::record_t::fakewalk_check( player_t * e ) {
	bool
		fakewalking = false,
		stage1 = false,			// stages needed cause we are iterating all layers, otherwise won't work :)
		stage2 = false,
		stage3 = false;

	for ( int i = 0; i < 14; i++ ) {
		AnimationLayer record = e->get_animlayers( )[ i ];

		if ( record.m_nSequence == 26 && record.m_flWeight < 0.4f )
			stage1 = true;
		if ( record.m_nSequence == 7 && record.m_flWeight > 0.001f )
			stage2 = true;
		if ( record.m_nSequence == 2 && record.m_flWeight == 0 )
			stage3 = true;
	}

	if ( stage1 && stage2 )
		if ( stage3 || ( e->m_fFlags( ) & FL_DUCKING ) ) // since weight from stage3 can be 0 aswell when crouching, we need this kind of check, cause you can fakewalk while crouching, thats why it's nested under stage1 and stage2
			fakewalking = true;
		else
			fakewalking = false;
	else
		fakewalking = false;

	return fakewalking;
}

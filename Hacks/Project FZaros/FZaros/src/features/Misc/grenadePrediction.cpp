#include "grenadePrediction.hpp" 

CGrenadePrediction g_GrenadePred;

void CGrenadePrediction::predict( CUserCmd *ucmd ) {
	//	readability 
	constexpr float restitution = 0.45f;
	constexpr float power[ ] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{}, grenade_act{ 1 };
	Vector3 pos, thrown_direction, start, eye_origin;
	Vector3 angles, thrown;

	//	first time setup 
	static auto *sv_gravity = g_Interfaces->cvar->FindVar( "sv_gravity" );

	//	calculate step and actual gravity value 
	gravity = sv_gravity->GetFloat( ) / 8.0f;
	step = g_Interfaces->globalVars->intervalPerTick;

	//	get local view and eye origin 
	eye_origin = Globals::localPlayer->eyePosition( );
	angles = ucmd->viewAngles;

	//	copy current angles and normalise pitch 
	thrown = angles;

	if ( thrown.x < 0 ) {
		thrown.x = -10 + thrown.x * ( ( 90 - 10 ) / 90.0f );
	}
	else {
		thrown.x = -10 + thrown.x * ( ( 90 + 10 ) / 90.0f );
	}

	//	find out how we're throwing the grenade 
	auto primary_attack = ucmd->buttons & IN_ATTACK;
	auto secondary_attack = ucmd->buttons & IN_ATTACK2;

	if ( primary_attack && secondary_attack ) {
		grenade_act = ACT_LOB;
	}
	else if ( secondary_attack ) {
		grenade_act = ACT_DROP;
	}

	//	apply 'magic' and modulate by velocity 
	unk01 = power[ grenade_act ];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	//	here's where the fun begins 
	g_Math.AngToVec( thrown, thrown_direction );

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = ( thrown_direction * new_velocity ) + Globals::localPlayer->velocity( );

	//	let's go ahead and predict 
	for ( auto time = 0.0f; index < 500; time += step ) {
		pos = start + thrown_direction * step;

		//	setup trace 
		CGameTrace trace;
		CTraceFilterSkipEntity filter( Globals::localPlayer );
		C_Ray ray;
		ray.Init( start, pos );

		g_Interfaces->trace->TraceRay( ray, MASK_SOLID, &filter, &trace );

		//	modify path if we have hit something 
		if ( trace.flFraction != 1.0f ) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.Dot( trace.plane.normal ) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += ( step * ( 1.0f - trace.flFraction ) );
		}

		//	check for detonation 
		auto detonate = detonated( Globals::localPlayer->activeWeapon( ), time, trace );

		//	emplace nade point 
		_points.at( index++ ) = CNadePoint( start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate );
		start = pos;

		//	apply gravity modifier 
		thrown_direction.z -= gravity * trace.flFraction * step;

		if ( detonate ) {
			break;
		}
	}

	//	invalidate all empty points and finish prediction 
	for ( auto n = index; n < 500; ++n ) {
		_points.at( n ).m_valid = false;
	}

	_predicted = true;
}

bool CGrenadePrediction::detonated( CBaseCombatWeapon *weapon, float time, CGameTrace &trace ) {
	if ( !weapon ) {
		return true;
	}

	//	get weapon item index 
	const auto index = weapon->itemDefenitionIndex( );

	switch ( index ) {
		//	flash and HE grenades only live up to 2.5s after thrown 
	case 43:
	case 44:
		if ( time > 2.5f ) {
			return true;
		}
		break;

		//	fire grenades detonate on ground hit, or 3.5s after thrown 
	case weapon_molotov:
	case 48:
		if ( trace.flFraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f ) {
			return true;
		}
		break;

		//	decoy and smoke grenades were buggy in prediction, so i used this ghetto work-around 
	case weapon_decoy:
	case 45:
		if ( time > 2.5f ) {
			return true;
		}
		break;
	}

	return false;
}

void CGrenadePrediction::think( CUserCmd *ucmd ) {
	if ( !config->get_bool( "espGrenadePred" ) )
		return;

	if ( !( ucmd->buttons & IN_ATTACK ) && !( ucmd->buttons & IN_ATTACK2 ) ) {
		_predicted = false;
		return;
	}

	const static std::vector< int > nades{
		weapon_flash,
		weapon_smoke,
		weapon_he,
		weapon_molotov,
		weapon_decoy,
		weapon_inc
	};

	//	grab local weapon 
	auto *weapon = Globals::localPlayer->activeWeapon( );

	if ( !weapon ) {
		return;
	}

	if ( std::find( nades.begin( ), nades.end( ), weapon->itemDefenitionIndex( ) ) != nades.end( ) ) {
		return predict( ucmd );
	}

	_predicted = false;
}

void CGrenadePrediction::draw( ) {	
	if ( !config->get_bool( "espGrenadePred" ) )
		return;

	if ( !g_Interfaces->gameEngine->inGame( ) || !Globals::localPlayer || !Globals::localPlayer->alive( ) )
		return;

	static auto alpha = 0.f;
	static auto _Switch = false;
	if ( alpha <= 0.f || alpha >= 255.f )
		_Switch = !_Switch;

	alpha += _Switch ? ( 255.f / 0.5f * g_Interfaces->globalVars->frametime ) : -( 255.f / 0.5f * g_Interfaces->globalVars->frametime );
	alpha = std::clamp<float>( alpha, 0.f, 255.f );

	auto draw_3d_dotted_circle = [ ] ( Vector3 position, float points, float radius ) {
		float step = PI * 2.0f / points;
		for ( float a = 0; a < PI * 2.0f; a += step ) {
			Vector3 start( radius * cosf( a ) + position.x, radius * sinf( a ) + position.y, position.z );

			Vector3 start2d;
			if ( g_Interfaces->debugOverlay->WorldToScreen( start, start2d ) )
				g_Renderer->line( start2d.x, start2d.y, start2d.x + 1, start2d.y + 1,
					Color(
						config->get_color("colorGrenadePred").r(),
						config->get_color("colorGrenadePred").g(),
						config->get_color("colorGrenadePred").b(),
						( int )alpha ) );
		}
	};

	Vector3 start, end;

	//	draw nade path 
	if ( _predicted ) {
		for ( auto &p : _points ) {
			if ( !p.m_valid ) {
				break;
			}

			if ( g_Interfaces->debugOverlay->WorldToScreen( p.m_start, start ) && g_Interfaces->debugOverlay->WorldToScreen( p.m_end, end ) ) {
				//	draw line 
				g_Renderer->line( start.x, start.y, end.x, end.y, config->get_color( "colorGrenadePred" ) );

				//	draw small box if detonated or hit a wall 
				if ( p.m_detonate || p.m_plane ) {
					g_Renderer->rectangle( start.x - 2, start.y - 2, 5, 5, p.m_detonate ? Color( 255, 0, 0 ) : Color::White );
				}

				if ( p.m_detonate )
					draw_3d_dotted_circle( p.m_end, 250, 150 );
			}
		}
	}
}
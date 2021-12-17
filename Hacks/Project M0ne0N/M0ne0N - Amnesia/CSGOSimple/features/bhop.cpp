#include "bhop.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../options.hpp"

void BunnyHop::OnCreateMove( CUserCmd* cmd ) {
	static bool jumped_last_tick = false;
	static bool should_fake_jump = false;

	if( !jumped_last_tick && should_fake_jump ) {
		should_fake_jump = false;
		cmd->buttons |= IN_JUMP;
	} else if( cmd->buttons & IN_JUMP ) {
		if( g_LocalPlayer->m_fFlags( ) & FL_ONGROUND ) {
			jumped_last_tick = true;
			should_fake_jump = true;
		} else {
			cmd->buttons &= ~IN_JUMP;
			jumped_last_tick = false;
		}
	} else {
		jumped_last_tick = false;
		should_fake_jump = false;
	}
}

void BunnyHop::AutoStrafe( CUserCmd* cmd, QAngle va ) {
	if( !g_LocalPlayer || !g_LocalPlayer->IsAlive( ) || g_LocalPlayer->m_nMoveType( ) != MOVETYPE_WALK )
		return;

	bool on_ground = ( g_LocalPlayer->m_fFlags( ) & FL_ONGROUND ) && !( cmd->buttons & IN_JUMP );
	if( on_ground ) {
		return;
	}

	static auto side = 1.0f;
	side = -side;

	auto velocity = g_LocalPlayer->m_vecVelocity( );
	velocity.z = 0.0f;

	QAngle wish_angle = cmd->viewangles;

	auto speed = velocity.Length2D( );
	auto ideal_strafe = std::clamp( RAD2DEG( atan( 15.f / speed ) ), 0.0f, 90.0f );

	if( cmd->forwardmove > 0.0f )
		cmd->forwardmove = 0.0f;

	static auto cl_sidespeed = g_CVar->FindVar( "cl_sidespeed" );

	static float old_yaw = 0.f;
	auto yaw_delta = std::remainderf( wish_angle.yaw - old_yaw, 360.0f );
	auto abs_angle_delta = abs( yaw_delta );
	old_yaw = wish_angle.yaw;

	if( abs_angle_delta <= ideal_strafe || abs_angle_delta >= 30.0f ) {
		QAngle velocity_direction;
		Math::VectorAngles( velocity, velocity_direction );
		auto velocity_delta = std::remainderf( wish_angle.yaw - velocity_direction.yaw, 360.0f );
		auto retrack = std::clamp( RAD2DEG( atan( 30.0f / speed ) ), 0.0f, 90.0f ) * g_Options.misc_retrack_speed;
		if( velocity_delta <= retrack || speed <= 15.0f ) {
			if( -( retrack ) <= velocity_delta || speed <= 15.0f ) {
				wish_angle.yaw += side * ideal_strafe;
				cmd->sidemove = cl_sidespeed->GetFloat( ) * side;
			} else {
				wish_angle.yaw = velocity_direction.yaw - retrack;
				cmd->sidemove = cl_sidespeed->GetFloat( );
			}
		} else {
			wish_angle.yaw = velocity_direction.yaw + retrack;
			cmd->sidemove = -cl_sidespeed->GetFloat( );
		}

		Math::MovementFix( cmd, wish_angle, cmd->viewangles );
	} else if( yaw_delta > 0.0f ) {
		cmd->sidemove = -cl_sidespeed->GetFloat( );
	} else if( yaw_delta < 0.0f ) {
		cmd->sidemove = cl_sidespeed->GetFloat( );
	}

#if 0
	static bool leftRight;

	bool inMove = cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT;

	if( cmd->buttons & IN_FORWARD && g_LocalPlayer->m_vecVelocity( ).Length( ) <= 50.0f )
		cmd->forwardmove = 250.0f;

	float yaw_change = 0.0f;

	if( g_LocalPlayer->m_vecVelocity( ).Length( ) > 50.f )
		yaw_change = 30.0f * fabsf( 30.0f / g_LocalPlayer->m_vecVelocity( ).Length( ) );

	C_BaseCombatWeapon* ActiveWeapon = g_LocalPlayer->m_hActiveWeapon( );

	if( ActiveWeapon && ActiveWeapon->CanFire( ) && cmd->buttons & IN_ATTACK )
		yaw_change = 0.0f;

	QAngle viewAngles = va;

	bool OnGround = ( g_LocalPlayer->m_fFlags( ) & FL_ONGROUND );
	if( !OnGround && !inMove ) {
		if( leftRight || cmd->mousedx > 1 ) {
			viewAngles.yaw += yaw_change;
			cmd->sidemove = 350.0f;
		} else if( !leftRight || cmd->mousedx < 1 ) {
			viewAngles.yaw -= yaw_change;
			cmd->sidemove = -350.0f;
		}

		leftRight = !leftRight;
	}
	viewAngles.Normalize( );
	Math::ClampAngles( viewAngles );
#endif
	//MovementFix::Get().Correct(viewAngles, cmd, cmd->forwardmove, cmd->sidemove);
}

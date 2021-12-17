#include "Misc.h"

#include "../../Include/GlobalIncludes.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/player_info_t.h"
#include "../../Utility/Utilities.h"
using namespace Features;
float XM_PI = 3.141592654f;
CMisc* Features::Misc = new CMisc();


Vector BoxCircleStrafe::boxCenter = Vector( 0, 0, 0 );
Vector BoxCircleStrafe::lastAngles1 = Vector( 0, 0, 0 );

bool BoxCircleStrafe::started = false;

float BoxCircleStrafe::strafeYaw = 0;
float BoxCircleStrafe::boxRadius = 0;
static auto trace_fraction_worldprops(Vector start, Vector end) -> float {
	Ray_t ray;
	trace_t tr;
	CTraceFilter filter;

	filter.pSkip1 = G::LocalPlayer;

	ray.Init(start, end);
	Interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &tr);

	return tr.fraction;
};
void BoxCircleStrafe::TraceWorld(Vector start, Vector end, trace_t* trace)
{
	CBaseEntity* pLocal = G::LocalPlayer;
	Ray_t ray;
	ray.Init( start, end );
	CTraceWorldOnly filter;
	Interfaces::Trace->TraceRay( ray, MASK_ALL, ( ITraceFilter* )&filter, trace );
}
float right_movement;
bool active;
float strafe_angle;
void BoxCircleStrafe::Start( CUserCmd* pCmd )
{
	/*CBaseEntity* pLocal = G::LocalPlayer;
	Vector angles = Vector( 0, pCmd->viewangles.y, 0 );

	if ( !started )
		strafeYaw = 0;

	Vector forward, left, right, up;
	Cheat::Utilities->Math_AngleVectors( angles, forward, right, up );
	left = right * -1;
	forward.NormalizeInPlace();
	left.NormalizeInPlace();
	right.NormalizeInPlace();
	up.NormalizeInPlace();
	trace_t traceBox;
	TraceWorld( pLocal->GetOrigin(), pLocal->GetOrigin() - Vector( 0, 0, 1000 ), &traceBox );
	GetBoxCenter( traceBox.end, forward, left, 64 * 15, boxCenter, boxRadius );
	started = true;*/
	/*strafe_angle = 0;
	active = true;

	Vector angles; Interfaces::Engine->GetViewAngles(&angles);
	angles.x = 0;

	Vector
		forward = angles.Direction(),
		right = forward.Cross(Vector(0, 0, 1)),
		left = Vector(-right.x, -right.y, right.z);

	float left_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + left * 250.0f + Vector(0, 0, 10));
	float right_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + right * 250.0f + Vector(0, 0, 10));

	if (left_path > right_path)
		right_movement = -1;
	else
		right_movement = 1;*/
}

void BoxCircleStrafe::Stop()
{
	Vector lastAngles = Vector( 0, 0, 0 );
	started = false;
	strafeYaw = 0;
}

void BoxCircleStrafe::Strafe( CUserCmd* pCmd )
{
	CBaseEntity* pLocal = G::LocalPlayer;
	Vector origin = pLocal->GetOrigin();
	Vector velocity = pLocal->GetVelocity();
	Vector angles = pCmd->viewangles;
	Vector velAngles;
	Cheat::Utilities->Math_VectorAngles( velocity, velAngles );
	velAngles.NormalizeAngles();
	float speed = velocity.Length();
	float tanAngle = Cheat::Utilities->Math_NormalizeYawInPlace( Cheat::Utilities->Math_CalcAngle( origin, boxCenter ).y - 90.0f );
	float airaccel = Interfaces::Var->FindVar( ( "sv_airaccelerate" ) )->GetFloat();
	float maxspeed = Interfaces::Var->FindVar( ( "sv_maxspeed" ) )->GetFloat();
	float maxDelta = ( airaccel * maxspeed ) * Interfaces::Globals->interval_per_tick;
	float delta = 0;

	if ( OutOfBox( origin ) )
		delta = -5.9f + Cheat::Utilities->Math_NormalizeYawInPlace( velAngles.y - tanAngle );

	else
	{
		float FinalPath = 1.0f;
		delta = ( 275.0f / speed ) * ( 2.0f / FinalPath ) * ( 128.0f / ( 1.7f / Interfaces::Globals->interval_per_tick ) ) * 5.9f;
	}

	if ( delta > maxDelta )
		delta = maxDelta;

	if ( delta < -maxDelta )
		delta = -maxDelta;

	strafeYaw += delta;

	if ( fabs( strafeYaw ) >= 360.0f )
	{
		started = false;
		strafeYaw = 0;
	}

	else
	{
		if ( lastAngles1 != Vector( 0, 0, 0 ) )
			strafeYaw -= Cheat::Utilities->Math_NormalizeYawInPlace( lastAngles1.y - angles.y );

		pCmd->forwardmove = min( 450.0f, cos( ( strafeYaw + 90 ) * ( M_PI / 180.0f ) ) * 450.0f );
		pCmd->sidemove = min( 450.0f, sin( ( strafeYaw + 90 ) * ( M_PI / 180.0f ) ) * 450.0f );
	}

	lastAngles1 = pCmd->viewangles;
	/*Vector velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	float speed = velocity.Length();
	if (speed < 45) speed = 45;
	if (speed > 800) speed = 800;

	float final_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + Vector(0, 0, 10) + velocity / 3.0f);
	float delta_angle = right_movement * fmax((275.0f / speed) * (2.0f / final_path) * (128.0f / (1.7f / Interfaces::Globals->interval_per_tick)) * 4.20f, 2.0f);
	strafe_angle += delta_angle;

	if (fabs(strafe_angle) >= 360.0f) {
		strafe_angle = 0.0f;
		active = false;
		right_movement = 0;
	}
	else {
		pCmd->forwardmove = cos((strafe_angle + 90 * right_movement) * (XM_PI / 180.0f)) * 450.0f;
		pCmd->sidemove = sin((strafe_angle + 90 * right_movement) * (XM_PI / 180.0f)) * 450.0f;
	}*/
}

bool BoxCircleStrafe::GetBoxCenter( Vector origin, Vector forward, Vector left, float maxSearch, Vector &center, float &radius )
{
	float minZ = origin.z;
	Vector endFront, endLeft, endBack, endRight;

	for ( float i = 0; i < maxSearch; i += 5.0f )
	{
		trace_t trace;
		Vector start = origin + forward * i + Vector( 0, 0, 1 );
		Vector end = origin + forward * i - Vector( 0, 0, 15 );
		TraceWorld( start, end, &trace );

		if ( fabsf( trace.end.z - minZ ) > 1.0f )
			break;

		endFront = trace.end;
	}

	for ( float i = 0; i < maxSearch; i += 5.0f )
	{
		trace_t trace;
		Vector start = origin - forward * i + Vector( 0, 0, 1 );
		Vector end = origin - forward * i - Vector( 0, 0, 15 );
		TraceWorld( start, end, &trace );

		if ( fabsf( trace.end.z - minZ ) > 1.0f )
			break;

		endBack = trace.end;
	}

	for ( float i = 0; i < maxSearch; i += 5.0f )
	{
		trace_t trace;
		Vector start = origin + left * i + Vector( 0, 0, 1 );
		Vector end = origin + left * i - Vector( 0, 0, 15 );
		TraceWorld( start, end, &trace );

		if ( fabsf( trace.end.z - minZ ) > 1.0f )
			break;

		endLeft = trace.end;
	}

	for ( float i = 0; i < maxSearch; i += 5.0f )
	{
		trace_t trace;
		Vector start = origin - left * i + Vector( 0, 0, 1 );
		Vector end = origin - left * i - Vector( 0, 0, 15 );
		TraceWorld( start, end, &trace );

		if ( fabsf( trace.end.z - minZ ) > 1.0f )
			break;

		endRight = trace.end;
	}

	center = ( ( ( endFront + endBack ) / 2.0f ) + ( ( endLeft + endRight ) / 2.0f ) ) / 2.0f;
	radius = ( center - endFront ).Length2D();
	return ( radius < maxSearch - 5.0f );
}

bool BoxCircleStrafe::OutOfBox( Vector origin )
{
	if ( ( origin - boxCenter ).Length2D() > boxRadius )
		return true;

	return false;
}

float CircleStrafe::RightMovement = -1;
bool CircleStrafe::IsActive = false;
float CircleStrafe::StrafeAngle = 0;

Vector CircleStrafe::CurrentAngles = Vector( 0, 0, 0 );
Vector CircleStrafe::LastAngles = Vector( 0, 0, 0 );

float CircleStrafe::GetTraceFractionWorldProps( Vector start, Vector end )
{
	Ray_t rayF, rayL, rayR;
	CTraceWorldOnly filter;
	trace_t traceF, traceL, traceR;
	Vector angle = Cheat::Utilities->Math_CalcAngle( start, end );
	Vector forward, left;
	Cheat::Utilities->Math_AngleVectors( angle, forward );
	Cheat::Utilities->Math_AngleVectors( angle + Vector( 0, 90, 0 ), forward );
	forward.NormalizeInPlace();
	left.NormalizeInPlace();
	float bodyOffset = 32;
	rayF.Init( start, end );
	rayL.Init( start + left * bodyOffset, end + left * bodyOffset );
	rayR.Init( start - left * bodyOffset, end - left * bodyOffset );
	float fractionF, fractionL, fractionR;
	Interfaces::Trace->TraceRay( rayF, MASK_ALL, ( ITraceFilter* )&filter, &traceF );
	Interfaces::Trace->TraceRay( rayL, MASK_ALL, ( ITraceFilter* )&filter, &traceL );
	Interfaces::Trace->TraceRay( rayR, MASK_ALL, ( ITraceFilter* )&filter, &traceR );
	fractionF = traceF.fraction;
	fractionL = traceL.fraction;
	fractionR = traceR.fraction;
	return min( fractionF, min( fractionL, fractionR ) );
}

void CircleStrafe::Rotate( CUserCmd* pCmd, float rotation )
{
	rotation = DEG2RAD( rotation );
	float cos_rot = cos( rotation );
	float sin_rot = sin( rotation );
	float new_forwardmove = ( cos_rot * pCmd->forwardmove ) - ( sin_rot * pCmd->sidemove );
	float new_sidemove = ( sin_rot * pCmd->forwardmove ) + ( cos_rot * pCmd->sidemove );
	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

void CircleStrafe::Start( CUserCmd* pCmd )
{
/*	auto pLocal = G::LocalPlayer;

	if ( !IsActive )
		StrafeAngle = 0;

	IsActive = true;
	CurrentAngles = pCmd->viewangles;

	if ( pLocal->GetVelocity().Length() > 100 )
		Cheat::Utilities->Math_VectorAngles( pLocal->GetVelocity(), CurrentAngles );

	Vector Forward, Right, Left, VelForward;
	Cheat::Utilities->Math_AngleVectors( CurrentAngles, Forward );
	Cheat::Utilities->Math_AngleVectors( CurrentAngles + Vector( 0, -90, 0 ), Right );
	Cheat::Utilities->Math_AngleVectors( CurrentAngles + Vector( 0, 90, 0 ), Left );
	Forward.NormalizeInPlace();
	Right.NormalizeInPlace();
	Left.NormalizeInPlace();
	float frac = GetTraceFractionWorldProps( pLocal->GetAbsOrigin(), pLocal->GetVelocity() * TICKS_TO_TIME( 5 ) );
	Vector origin = Vector( 0, 0, 5 ) + pLocal->GetAbsOrigin() + pLocal->GetVelocity().GetNormalized() * frac;
	float LeftPath = 0;
	float RightPath = 0;

	for ( float i = 5.0f; i < 90.0f; i += 5.0f )
	{
		Vector val, var;
		Cheat::Utilities->Math_AngleVectors( CurrentAngles + Vector( 0, i, 0 ), val );
		Cheat::Utilities->Math_AngleVectors( CurrentAngles + Vector( 0, -i, 0 ), var );
		LeftPath += min(
		        GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ), origin + val * 250.0f + Vector( 0, 0, 5 ) ),
		        min(
		            GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 + 32 ), origin + val * 250.0f + Vector( 0, 0, 5 + 32 ) ),
		            min(
		                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + Left * 50, origin + val * 250.0f + Vector( 0, 0, 5 ) + Left * 50 ),
		                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + Right * 50, origin + val * 250.0f + Vector( 0, 0, 5 ) + Right * 50 )
		            )
		        )
		    );
		RightPath += min(
		        GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ), origin + var * 250.0f + Vector( 0, 0, 5 ) ),
		        min(
		            GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 + 32 ), origin + var * 250.0f + Vector( 0, 0, 5 + 32 ) ),
		            min(
		                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + Left * 50, origin + var * 250.0f + Vector( 0, 0, 5 ) + Left * 50 ),
		                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + Right * 50, origin + var * 250.0f + Vector( 0, 0, 5 ) + Right * 50 )
		            )
		        )
		    );
	}

	if ( LeftPath > RightPath )
		RightMovement = -1;

	else
		RightMovement = 1;*/
	strafe_angle = 0;
	active = true;

	Vector angles; Interfaces::Engine->GetViewAngles(&angles);
	angles.x = 0;

	Vector
		forward = angles.Direction(),
		right = forward.Cross(Vector(0, 0, 1)),
		left = Vector(-right.x, -right.y, right.z);

	float left_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + left * 250.0f + Vector(0, 0, 10));
	float right_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + right * 250.0f + Vector(0, 0, 10));

	if (left_path > right_path)
		right_movement = -1;
	else
		right_movement = 1;
}

void CircleStrafe::Strafe( CUserCmd* pCmd )
{
	/*Vector Velocity = G::LocalPlayer->GetVelocity();
	Vector velFw = Velocity.GetNormalized();
	Vector VelLeft, VelRight, VelAng;
	Cheat::Utilities->Math_VectorAngles( Velocity, VelAng );
	Cheat::Utilities->Math_AngleVectors( Velocity + Vector( 0, -90, 0 ), VelRight );
	Cheat::Utilities->Math_AngleVectors( Velocity + Vector( 0, 90, 0 ), VelLeft );
	VelLeft.Normalize();
	VelRight.Normalize();
	float frac = GetTraceFractionWorldProps( G::LocalPlayer->GetAbsOrigin(), G::LocalPlayer->GetVelocity() * TICKS_TO_TIME( 5 ) );
	Vector origin = Vector( 0, 0, 5 ) + G::LocalPlayer->GetAbsOrigin() + G::LocalPlayer->GetVelocity().GetNormalized() * frac;
	Velocity.z = 0;
	float Speed = Velocity.Length();

	if ( Speed < 45 )
		Speed = 45;

	float retrack = 5.9f + ( Speed / 1500.0f ) * 5.0f;
	float FinalPath = min(
	        GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ), origin + Vector( 0, 0, 5 ) + Velocity / 5.9f ),
	        min(
	            GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 + 32 ), origin + Vector( 0, 0, 5 + 32 ) + Velocity / 5.9f ),
	            min(
	                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + VelLeft * 50, origin + Vector( 0, 0, 5 ) + Velocity / 5.9f + VelLeft * 50 ),
	                GetTraceFractionWorldProps( origin + Vector( 0, 0, 5 ) + VelRight * 50, origin + Vector( 0, 0, 5 ) + Velocity / 5.9f + VelRight * 50 )
	            )
	        )
	    );
	static int lastChangeDir = 0;
	float Delta = ( 275.0f / Speed ) * ( 2.0f / FinalPath ) * ( 128.0f / ( 1.7f / Interfaces::Globals->interval_per_tick ) ) * retrack;
	float DeltaAngle = RightMovement * min( Delta, 15.0f );
	StrafeAngle += DeltaAngle;

	if ( fabs( StrafeAngle ) >= 360.0f )
	{
		StrafeAngle = 0.0f;
		IsActive = false;
		LastAngles = Vector( 0, 0, 0 );
	}

	else
	{
		if ( LastAngles != Vector( 0, 0, 0 ) )
			StrafeAngle -= Cheat::Utilities->Math_NormalizeYawInPlace( LastAngles.y - pCmd->viewangles.y );

		pCmd->forwardmove = ( cos( ( StrafeAngle + 90 * RightMovement ) * ( M_PI / 180.0f ) ) * 450.0f );
		pCmd->sidemove = ( sin( ( StrafeAngle + 90 * RightMovement ) * ( M_PI / 180.0f ) ) * 450.0f );
	}

	LastAngles = pCmd->viewangles;*/
	Vector velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	float speed = velocity.Length();
	if (speed < 45) speed = 45;
	if (speed > 800) speed = 800;

	float final_path = trace_fraction_worldprops(G::LocalPlayer->GetOrigin() + Vector(0, 0, 10), G::LocalPlayer->GetOrigin() + Vector(0, 0, 10) + velocity / 3.0f);
	float delta_angle = right_movement * fmax((275.0f / speed) * (2.0f / final_path) * (128.0f / (1.7f / Interfaces::Globals->interval_per_tick)) * 4.20f, 2.0f);
	strafe_angle += delta_angle;

	if (fabs(strafe_angle) >= 360.0f) {
		strafe_angle = 0.0f;
		active = false;
		right_movement = 0;
	}
	else {
		pCmd->forwardmove = cos((strafe_angle + 90 * right_movement) * (XM_PI / 180.0f)) * 450.0f;
		pCmd->sidemove = sin((strafe_angle + 90 * right_movement) * (XM_PI / 180.0f)) * 450.0f;
	}
}
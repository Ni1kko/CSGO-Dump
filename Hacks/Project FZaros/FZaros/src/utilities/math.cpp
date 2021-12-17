#include "math.hpp"

CMath g_Math;

void CMath::vectorAngles( const Vector3 &vecForward, Vector3 &vecAngles )
{
	Vector3 vecView;
	if ( vecForward[ 1 ] == 0.f && vecForward[ 0 ] == 0.f )
	{
		vecView[ 0 ] = 0.f;
		vecView[ 1 ] = 0.f;
	}
	else
	{
		vecView[ 1 ] = atan2( vecForward[ 1 ], vecForward[ 0 ] ) * 180.f / 3.14159265358979323846f;

		if ( vecView[ 1 ] < 0.f )
			vecView[ 1 ] += 360.f;

		vecView[ 2 ] = sqrt( vecForward[ 0 ] * vecForward[ 0 ] + vecForward[ 1 ] * vecForward[ 1 ] );

		vecView[ 0 ] = atan2( vecForward[ 2 ], vecView[ 2 ] ) * 180.f / 3.14159265358979323846f;
	}

	vecAngles[ 0 ] = -vecView[ 0 ];
	vecAngles[ 1 ] = vecView[ 1 ];
	vecAngles[ 2 ] = 0.f;
}

void CMath::vectorAngles( const Vector3& forward, Vector3& up, Vector3& angles )
{
	Vector3 left = CrossProduct( up, forward );
	left.NormalizeInPlace( );

	float forwardDist = forward.Length2D( );

	if ( forwardDist > 0.001f )
	{
		angles.x = atan2f( -forward.z, forwardDist ) * 180 / M_PI;
		angles.y = atan2f( forward.y, forward.x ) * 180 / M_PI;

		float upZ = ( left.y * forward.x ) - ( left.x * forward.y );
		angles.z = atan2f( left.z, upZ ) * 180 / M_PI;
	}
	else
	{
		angles.x = atan2f( -forward.z, forwardDist ) * 180 / M_PI;
		angles.y = atan2f( -left.x, left.y ) * 180 / M_PI;
		angles.z = 0;
	}
}

void CMath::rotateMovement( float yaw, CUserCmd* pCmd )
{
	Vector3 ViewAngles;
	g_Interfaces->gameEngine->getViewAngles( ViewAngles );

	float rotation = DEG2RAD( ViewAngles.y - yaw );

	float cos_rot = cos( rotation );
	float sin_rot = sin( rotation );

	float new_forwardmove = ( cos_rot * pCmd->forwardmove ) - ( sin_rot * pCmd->sidemove );
	float new_sidemove = ( sin_rot * pCmd->forwardmove ) + ( cos_rot * pCmd->sidemove );

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

inline Vector3 operator*( float lhs, const Vector3 &rhs )
{
	return rhs * lhs;
}

float CMath::distanceToRay( const Vector3 &pos, const Vector3 &ray_start, const Vector3 &ray_end )
{
	Vector3 to = pos - ray_start;
	Vector3 dir = ray_end - ray_start;
	float length = Vector3Normalize( dir );

	float rangeAlong = dir.Dot( to );

	float range;

	if ( rangeAlong < 0.0f )
		range = -( pos - ray_start ).Length( );
	else if ( rangeAlong > length )
		range = -( pos - ray_end ).Length( );
	else
	{
		Vector3 onRay = ray_start + rangeAlong * dir;
		range = ( pos - onRay ).Length( );
	}

	return range;
}


Vector3 CMath::calcAngle( const Vector3& vecSource, const Vector3& vecDestination )
{
	Vector3 qAngles;
	Vector3 delta = Vector3( ( vecSource[ 0 ] - vecDestination[ 0 ] ), ( vecSource[ 1 ] - vecDestination[ 1 ] ), ( vecSource[ 2 ] - vecDestination[ 2 ] ) );
	double hyp = std::sqrtf( delta[ 0 ] * delta[ 0 ] + delta[ 1 ] * delta[ 1 ] );
	qAngles[ 0 ] = ( float ) ( std::atan( delta[ 2 ] / hyp ) * ( 180.0 / M_PI ) );
	qAngles[ 1 ] = ( float ) ( std::atan( delta[ 1 ] / delta[ 0 ] ) * ( 180.0 / M_PI ) );
	qAngles[ 2 ] = 0.f;
	if ( delta[ 0 ] >= 0.f )
		qAngles[ 1 ] += 180.f;

	return qAngles;

}

float CMath::GetFov(const Vector3& viewAngle, const Vector3& aimAngle)
{
	Vector3 ang, aim;

	angleVectors(viewAngle, &aim);
	angleVectors(aimAngle, &ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

void CMath::vectorTransform( const Vector3 in1, const matrix3x4_t in2, Vector3 &out )
{
	out[ 0 ] = DotProduct( in1, Vector3( in2[ 0 ][ 0 ], in2[ 0 ][ 1 ], in2[ 0 ][ 2 ] ) ) + in2[ 0 ][ 3 ];
	out[ 1 ] = DotProduct( in1, Vector3( in2[ 1 ][ 0 ], in2[ 1 ][ 1 ], in2[ 1 ][ 2 ] ) ) + in2[ 1 ][ 3 ];
	out[ 2 ] = DotProduct( in1, Vector3( in2[ 2 ][ 0 ], in2[ 2 ][ 1 ], in2[ 2 ][ 2 ] ) ) + in2[ 2 ][ 3 ];
}

vec_t CMath::vectorNormalize( Vector3& v )
{
	vec_t l = v.Length( );

	if ( l != 0.0f )
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}

void CMath::angleVectors( const Vector3 &angles, Vector3 *forward )
{
	Assert( s_bMathlibInitialized );
	Assert( forward );

	float	sp, sy, cp, cy;

	sy = sin( DEG2RAD( angles[ 1 ] ) );
	cy = cos( DEG2RAD( angles[ 1 ] ) );

	sp = sin( DEG2RAD( angles[ 0 ] ) );
	cp = cos( DEG2RAD( angles[ 0 ] ) );

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

float CMath::normalieYawInPlace( float f )
{
	if ( std::isnan( f ) || std::isinf( f ) )
		f = 0;

	if ( f > 9999999 )
		f = 0;

	if ( f < -9999999 )
		f = 0;

	while ( f < -180.0f ) f += 360.0f;
	while ( f > 180.0f ) f -= 360.0f;

	return f;
}

void CMath::normalizeAngles( Vector3& angles )
{
	for ( auto i = 0; i < 3; i++ )
	{
		while ( angles[ i ] < -180.0f ) angles[ i ] += 360.0f;
		while ( angles[ i ] > 180.0f ) angles[ i ] -= 360.0f;
	}
}

float CMath::normalizeYaw( float yaw )
{
	if ( yaw > 180 )
		yaw -= ( round( yaw / 360 ) * 360.f );
	else if ( yaw < -180 )
		yaw += ( round( yaw / 360 ) * -360.f );

	return yaw;
}

void sin_cos( float radian, float* sin, float* cos )
{
	*sin = std::sin( radian );
	*cos = std::cos( radian );
}

void CMath::angleVectors( const Vector3& angles, Vector3* forward, Vector3* right, Vector3* up )
{
	float sp, sy, sr, cp, cy, cr;

	sin_cos( GRD_TO_BOG( angles.x ), &sp, &cp );
	sin_cos( GRD_TO_BOG( angles.y ), &sy, &cy );
	sin_cos( GRD_TO_BOG( angles.z ), &sr, &cr );

	if ( forward != nullptr )
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if ( right != nullptr )
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if ( up != nullptr )
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void CMath::AngToVec( const Vector3 &angles, Vector3 &forward )
{
	float sp, sy, cp, cy;

	sin_cos( DEG2RAD( angles.y ), &sy, &cy );
	sin_cos( DEG2RAD( angles.x ), &sp, &cp );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void CMath::randomSeed( int seed )
{
	static auto random_seed = reinterpret_cast< void( *)( int ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" ) );

	random_seed( seed );
}

float CMath::randomFloat( float min, float max )
{
	static auto random_float = reinterpret_cast< float( *)( float, float ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloat" ) );

	return random_float( min, max );
}

void CMath::clampAngles( Vector3 &angles )
{
	if ( angles.y > 180.0f )
		angles.y = 180.0f;
	else if ( angles.y < -180.0f )
		angles.y = -180.0f;

	if ( angles.x > 89.0f )
		angles.x = 89.0f;
	else if ( angles.x < -89.0f )
		angles.x = -89.0f;

	angles.z = 0;
}

bool CMath::clamp( Vector3 &angles )
{
	Vector3 a = angles;
	normalizeAngles( a );
	clampAngles( a );

	if ( isnan( a.x ) || isinf( a.x ) ||
		isnan( a.y ) || isinf( a.y ) ||
		isnan( a.z ) || isinf( a.z ) )
	{
		return false;
	}
	else
	{
		angles = a;
		return true;
	}
}

float CMath::GRD_TO_BOG( float GRD )
{
	return ( M_PI / 180 ) * GRD;
}

float CMath::Distance( Vector2 point1, Vector2 point2 )
{
	float diffY = point1.y - point2.y;
	float diffX = point1.x - point2.x;
	return sqrt( ( diffY * diffY ) + ( diffX * diffX ) );
}

float CMath::normalizeAngle( float flAng )
{

	// rotate left by 180 degrees
	flAng = flAng + 180.0f;

	// normalize to -360..360 degrees
	flAng = fmod( flAng, 360.0f );
	if ( flAng != flAng ) // http://stackoverflow.com/a/570694
	{
		// edge case error handling
		flAng = 0.0f;
	}

	// rotate negative angles left by 360 degrees to get the positive equivalent
	if ( flAng < 0.0f )
	{
		flAng += 360.0f;
	}

	// rotate right by 180 degrees
	flAng -= 180.0f;

	return flAng;

}

float CMath::NormalizeFloat( float angle ) const
{
	if ( angle > 180.f || angle < -180.f )
	{
		auto revolutions = round( abs( angle / 360.f ) );

		if ( angle < 0.f )
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}

	return angle;
}

void CMath::angleMatrix( const Vector3 angles, matrix3x4_t& matrix )
{
	float sr, sp, sy, cr, cp, cy;

	sy = sin( DEG2RAD( angles[ 1 ] ) );
	cy = cos( DEG2RAD( angles[ 1 ] ) );

	sp = sin( DEG2RAD( angles[ 0 ] ) );
	cp = cos( DEG2RAD( angles[ 0 ] ) );

	sr = sin( DEG2RAD( angles[ 2 ] ) );
	cr = cos( DEG2RAD( angles[ 2 ] ) );

	//matrix = (YAW * PITCH) * ROLL
	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;
	matrix[ 2 ][ 0 ] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;

	matrix[ 0 ][ 1 ] = sp * srcy - crsy;
	matrix[ 1 ][ 1 ] = sp * srsy + crcy;
	matrix[ 2 ][ 1 ] = sr * cp;

	matrix[ 0 ][ 2 ] = ( sp * crcy + srsy );
	matrix[ 1 ][ 2 ] = ( sp * crsy - srcy );
	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.0f;
	matrix[ 1 ][ 3 ] = 0.0f;
	matrix[ 2 ][ 3 ] = 0.0f;
}

void CMath::matrixSetColumn( const Vector3 &in, int column, matrix3x4_t& out )
{
	out[ 0 ][ column ] = in.x;
	out[ 1 ][ column ] = in.y;
	out[ 2 ][ column ] = in.z;
}

void CMath::angleMatrix( const Vector3 &angles, const Vector3 &position, matrix3x4_t& matrix_out )
{
	angleMatrix( angles, matrix_out );
	matrixSetColumn( position, 3, matrix_out );
}

void CMath::matrixCopy( const matrix3x4_t& source, matrix3x4_t& target )
{
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			target[ i ][ j ] = source[ i ][ j ];
		}
	}
}

void CMath::matrixMultiply( matrix3x4_t& in1, const matrix3x4_t& in2 )
{
	matrix3x4_t out;
	if ( &in1 == &out )
	{
		matrix3x4_t in1b;
		matrixCopy( in1, in1b );
		matrixMultiply( in1b, in2 );
		return;
	}
	if ( &in2 == &out )
	{
		matrix3x4_t in2b;
		matrixCopy( in2, in2b );
		matrixMultiply( in1, in2b );
		return;
	}
	out[ 0 ][ 0 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 0 ][ 1 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 0 ][ 2 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 0 ][ 3 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 0 ][ 3 ];
	out[ 1 ][ 0 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 1 ][ 1 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 1 ][ 2 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 1 ][ 3 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 1 ][ 3 ];
	out[ 2 ][ 0 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 2 ][ 1 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 2 ][ 2 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 2 ][ 3 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 2 ][ 3 ];

	in1 = out;
}

FORCEINLINE float DotProduct( const float *v1, const float *v2 )
{
	return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
}

void CMath::vectorRotate( const float *in1, const matrix3x4_t& in2, float *out )
{
	out[ 0 ] = DotProduct( in1, in2[ 0 ] );
	out[ 1 ] = DotProduct( in1, in2[ 1 ] );
	out[ 2 ] = DotProduct( in1, in2[ 2 ] );
}

void CMath::vectorRotate( const Vector3& in1, const matrix3x4_t &in2, Vector3 &out )
{
	vectorRotate( &in1.x, in2, &out.x );
}

void CMath::vectorRotate( const Vector3 &in1, const Vector3 &in2, Vector3 &out )
{
	matrix3x4_t matRotate;
	angleMatrix( in2, matRotate );
	vectorRotate( in1, matRotate, out );
}
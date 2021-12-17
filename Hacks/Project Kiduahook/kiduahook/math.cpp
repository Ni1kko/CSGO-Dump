#include "math.h"

CMath g_Math;

void CMath::AngleMatrix(const Vector angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	sr = sin(DEG2RAD(angles[2]));
	cr = cos(DEG2RAD(angles[2]));

	//matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void CMath::AngleMatrix(QAngle vecAngles, Vector vecOrigin, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(vecAngles[1]), &sy, &cy);
	SinCos(DEG2RAD(vecAngles[0]), &sp, &cp);
	SinCos(DEG2RAD(vecAngles[2]), &sr, &cr);

	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;

	//MatrixSetColumn( position, 3, matrix );
	matrix[0][3] = vecOrigin.x;
	matrix[1][3] = vecOrigin.y;
	matrix[2][3] = vecOrigin.z;
}

void CMath::MatrixAngles(const matrix3x4_t& matrix, float *angles)
{
	float forward[3];
	float left[3];
	float up[3];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];
	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];
	up[2] = matrix[2][2];

	float xyDist = SSESqrt(forward[0] * forward[0] + forward[1] * forward[1]);

	// enough here to get angles?
	if (xyDist > 0.001f)
	{
		// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// (roll)	z = ATAN( left.z, up.z );
		angles[2] = RAD2DEG(atan2f(left[2], up[2]));
	}
	else	// forward is mostly Z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angles[2] = 0;
	}
}

void MatrixGetColumn(const matrix3x4_t &src, int nCol, Vector& pColumn)
{
	pColumn.x = src[0][nCol];
	pColumn.y = src[1][nCol];
	pColumn.z = src[2][nCol];
}

void MatrixPosition(const matrix3x4_t &matrix, Vector &position)
{
	MatrixGetColumn(matrix, 3, position);
}

void CMath::MatrixAngle(const matrix3x4_t &matrix, QAngle &angles, Vector &position)
{
	MatrixAngles(matrix, &angles.x);
	MatrixPosition(matrix, position);
}

void inline CMath::SinCos( float radians, float *sine, float *cosine )
{
	_asm
	{
		fld		DWORD PTR[ radians ]
		fsincos

		mov edx, DWORD PTR[ cosine ]
		mov eax, DWORD PTR[ sine ]

		fstp DWORD PTR[ edx ]
		fstp DWORD PTR[ eax ]
	}
}

void CMath::CalcAngle( Vector &vIn, const Vector &vDest, Vector &vAngles )
{
	Vector vDelta = Vector( ( vIn[ 0 ] - vDest[ 0 ] ), ( vIn[ 1 ] - vDest[ 1 ] ), ( vIn[ 2 ] - vDest[ 2 ] ) );
	double dHyp = sqrtf( vDelta[ 0 ] * vDelta[ 0 ] + vDelta[ 1 ] * vDelta[ 1 ] );

	vAngles[ 0 ] = atan( vDelta[ 2 ] / dHyp ) * ( 180.0 / M_PI );
	vAngles[ 1 ] = atan( vDelta[ 1 ] / vDelta[ 0 ] ) * ( 180.0 / M_PI );
	vAngles[ 2 ] = 0.0f;

	if( vDelta[ 0 ] >= 0.0 )
		vAngles[ 1 ] += 180.0f;
}

void CMath::VectorAngles( const Vector& forward, QAngle& angles )
{
	if( forward [ 1 ] == 0.0f && forward [ 0 ] == 0.0f )
	{
		angles [ 0 ] = ( forward [ 2 ] > 0.0f ) ? 270.0f : 90.0f;
		angles [ 1 ] = 0.0f;
	}
	else
	{
		angles [ 0 ] = atan2( -forward [ 2 ], forward.Length2D() ) * -180 / M_PI;
		angles [ 1 ] = atan2( forward [ 1 ], forward [ 0 ] ) * 180 / M_PI;

		if( angles [ 1 ] > 90 ) angles [ 1 ] -= 180;
		else if( angles [ 1 ] < 90 ) angles [ 1 ] += 180;
		else if( angles [ 1 ] == 90 ) angles [ 1 ] = 0;
	}

	angles [ 2 ] = 0.0f;
}

void CMath::VectorAngles( const Vector& forward, Vector& angles )
{
	if( forward[ 1 ] == 0.0f && forward[ 0 ] == 0.0f )
	{
		angles[ 0 ] = ( forward[ 2 ] > 0.0f ) ? 270.0f : 90.0f;
		angles[ 1 ] = 0.0f;
	}
	else
	{
		angles[ 0 ] = atan2( -forward[ 2 ], forward.Length2D() ) * -180 / M_PI;
		angles[ 1 ] = atan2( forward[ 1 ], forward[ 0 ] ) * 180 / M_PI;

		if( angles[ 1 ] > 90 ) angles[ 1 ] -= 180;
		else if( angles[ 1 ] < 90 ) angles[ 1 ] += 180;
		else if( angles[ 1 ] == 90 ) angles[ 1 ] = 0;
	}

	angles[ 2 ] = 0.0f;
}

QAngle CMath::CalcAngle( Vector src, Vector dst )
{
	QAngle angles;
	Vector delta = src - dst;

	VectorAngles( delta, angles );

	NormalizeQAngle( angles );

	return angles;
}

Vector CMath::CalcAngles( Vector src, Vector dst )
{
	QAngle angles;
	Vector delta = src - dst;

	VectorAngles( delta, angles );

	NormalizeQAngle( angles );

	return Vector( angles.x, angles.y, angles.z );
}

void CMath::AngleVector( Vector &vAngle, Vector &vForward )
{
	float sp, sy, cp, cy;
	SinCos( vAngle[ 1 ] * M_PI / 180.f, &sy, &cy );
	SinCos( vAngle[ 0 ] * M_PI / 180.f, &sp, &cp );

	vForward[ 0 ] = cp*cy;
	vForward[ 1 ] = cp*sy;
	vForward[ 2 ] = -sp;
}

void CMath::AngleVector( const Vector &angles, Vector *forward, Vector *right, Vector *up )
{
	float sr, sp, sy, cr, cp, cy;

	SinCos( DEG2RAD( angles[ 1 ] ), &sy, &cy );
	SinCos( DEG2RAD( angles[ 0 ] ), &sp, &cp );
	SinCos( DEG2RAD( angles[ 2 ] ), &sr, &cr );

	if( forward )
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if( right )
	{
		right->x = ( -1 * sr*sp*cy + -1 * cr*-sy );
		right->y = ( -1 * sr*sp*sy + -1 * cr*cy );
		right->z = -1 * sr*cp;
	}

	if( up )
	{
		up->x = ( cr*sp*cy + -sr*-sy );
		up->y = ( cr*sp*sy + -sr*cy );
		up->z = cr*cp;
	}
}

void CMath::AngleVector( const Vector angles, float *forward, float *right, float *up )
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[ 0 ] * ( M_PI / 180.f );
	sp = sin( angle );
	cp = cos( angle );

	angle = angles[ 1 ] * ( M_PI / 180.f );
	sy = sin( angle );
	cy = cos( angle );

	if( forward )
	{
		forward[ 0 ] = cp * cy;
		forward[ 1 ] = cp * sy;
		forward[ 2 ] = -sp;
	}

	if( right || up )
	{
		static float sr, cr;

		angle = angles[ 2 ] * ( M_PI / 180.f );
		sr = sin( angle );
		cr = cos( angle );

		if( right )
		{
			right[ 0 ] = -1 * sr * sp * cy + -1 * cr * -sy;
			right[ 1 ] = -1 * sr * sp * sy + -1 * cr *cy;
			right[ 2 ] = -1 * sr * cp;
		}

		if( up )
		{
			up[ 0 ] = cr * sp *cy + -sr * -sy;
			up[ 1 ] = cr * sp *sy + -sr * cy;
			up[ 2 ] = cr * cp;
		}
	}
}

float CMath::DotProduct( const float *v1, const float *v2 )
{
	return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
}

void CMath::FloatTransform( const float *in1, const matrix3x4_t& in2, float *out )
{
	Assert( in1 != out );
	out[ 0 ] = DotProduct( in1, in2[ 0 ] ) + in2[ 0 ][ 3 ];
	out[ 1 ] = DotProduct( in1, in2[ 1 ] ) + in2[ 1 ][ 3 ];
	out[ 2 ] = DotProduct( in1, in2[ 2 ] ) + in2[ 2 ][ 3 ];
}

void CMath::VectorTransform( const Vector& in1, const matrix3x4_t& in2, Vector& out )
{
	FloatTransform( &in1.x, in2, &out.x );
}

Vector CMath::GetHitspot( CBaseEntity* pEntity, int iHitBox )
{
	matrix3x4_t matrix[ 128 ];

	if( !pEntity->SetupBones( matrix, 128, 0x00000100, 0 ) )
		return Vector( 0, 0, 0 );

	auto hdr = g_pModel->GetStudiomodel( pEntity->GetModel() );

	if( !hdr )
		return Vector( 0, 0, 0 );

	auto set = hdr->pHitboxSet( 0 );

	if( !set )
		return Vector( 0, 0, 0 );

	auto hitbox = set->pHitbox( iHitBox );

	if( !hitbox )
		return Vector( 0, 0, 0 );

	Vector vMin, vMax;
	VectorTransform( hitbox->bbmin, matrix[ hitbox->bone ], vMin );
	VectorTransform( hitbox->bbmax, matrix[ hitbox->bone ], vMax );

	Vector vCenter = ( vMin + vMax ) * 0.5f;

	return vCenter;
}

Vector CMath::GetHitspot( CBaseEntity* pEntity, int iHitBox, matrix3x4_t *matrix )
{
	if( !pEntity->SetupBones( matrix, 128, 256, g_pEngine->GetLastTimeStamp() ) )
		return Vector( 0, 0, 0 );

	auto hdr = g_pModel->GetStudiomodel( pEntity->GetModel() );

	if( !hdr )
		return Vector( 0, 0, 0 );

	auto set = hdr->pHitboxSet( 0 );

	if( !set )
		return Vector( 0, 0, 0 );

	auto hitbox = set->pHitbox( iHitBox );

	if( !hitbox )
		return Vector( 0, 0, 0 );

	Vector vMin, vMax;
	VectorTransform( hitbox->bbmin, matrix [ hitbox->bone ], vMin );
	VectorTransform( hitbox->bbmax, matrix [ hitbox->bone ], vMax );

	Vector vCenter = ( vMin + vMax ) * 0.5f;

	return vCenter;
}

float CMath::AngleDiff(float destAngle, float srcAngle)
{
	float delta;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle)
	{
		if (delta >= 180)
			delta -= 360;
	}
	else
	{
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}

void CMath::NormalizeQAngle( QAngle& vec )
{
	for( int i = 0; i < 2; ++i )
	{
		while( vec[ i ] > 180.f )
			vec[ i ] -= 360.f;

		while( vec[ i ] < -180.f )
			vec[ i ] += 360.f;
	}

	vec[ 2 ] = 0.f;
}

void CMath::NormalizeVector( Vector& vec )
{
	for( int i = 0; i < 2; ++i )
	{
		while( vec[ i ] > 180.f )
			vec[ i ] -= 360.f;

		while( vec[ i ] < -180.f )
			vec[ i ] += 360.f;
	}

	vec[ 2 ] = 0.f;
}

void CMath::ClampAngles( Vector& Vectors )
{
	if( Vectors[ 0 ] > 89.f )
		Vectors[ 0 ] = 89.f;
	if( Vectors[ 0 ] < -89.f )
		Vectors[ 0 ] = -89.f;
	if( Vectors[ 1 ] > 180.f )
		Vectors[ 1 ] = 180.f;
	if( Vectors[ 1 ] < -180.f )
		Vectors[ 1 ] = -180.f;

	Vectors[ 2 ] = 0.f;
}

void CMath::MakeVector( Vector vecIn, Vector& vecOut )
{
	float pitch;
	float yaw;
	float tmp;

	pitch = ( float ) ( vecIn[ 0 ] * M_PI / 180 );
	yaw = ( float ) ( vecIn[ 1 ] * M_PI / 180 );
	tmp = ( float ) cos( pitch );

	vecOut[ 0 ] = ( float ) ( -tmp * -cos( yaw ) );
	vecOut[ 1 ] = ( float ) ( sin( yaw )*tmp );
	vecOut[ 2 ] = ( float ) -sin( pitch );
}

float CMath::GetRageFoV( Vector angle, Vector src, Vector dst )
{
	Vector ang, aim;
	float fov;

	CalcAngle( src, dst, ang );
	MakeVector( angle, aim );
	MakeVector( ang, ang );

	float mag_s = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) );
	float mag_d = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) );
	float u_dot_v = aim[ 0 ] * ang[ 0 ] + aim[ 1 ] * ang[ 1 ] + aim[ 2 ] * ang[ 2 ];

	fov = acos( u_dot_v / ( mag_s*mag_d ) ) * ( 180.0 / M_PI );

	float xDist = abs( src[ 0 ] - dst[ 0 ] );
	float yDist = abs( src[ 1 ] - dst[ 1 ] );
	float Distance = sqrt( ( xDist * xDist ) + ( yDist * yDist ) );

	return fov;
}

float CMath::GetFoV( Vector angle, Vector src, Vector dst )
{
	Vector ang, aim;
	float fov;

	CalcAngle( src, dst, ang );
	MakeVector( angle, aim );
	MakeVector( ang, ang );

	float mag_s = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) );
	float mag_d = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) );
	float u_dot_v = aim[ 0 ] * ang[ 0 ] + aim[ 1 ] * ang[ 1 ] + aim[ 2 ] * ang[ 2 ];

	fov = acos( u_dot_v / ( mag_s*mag_d ) ) * ( 180.0 / M_PI );

	float xDist = abs( src[ 0 ] - dst[ 0 ] );
	float yDist = abs( src[ 1 ] - dst[ 1 ] );
	float Distance = sqrt( ( xDist * xDist ) + ( yDist * yDist ) );

	Distance /= 650;

	if( Distance < 0.7f )
		Distance = 0.7f;

	if( Distance > 6.5 )
		Distance = 6.5;

	fov *= Distance;

	return fov;
}

void CMath::BulletLocation( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, Vector& vViewAngles, Vector& vBullet )
{
	typedef void( __cdecl* RandomSeed_t )( int );
	static RandomSeed_t pRandomSeed = 0;

	typedef float( __cdecl* RandomFloat_t )( float, float );
	static RandomFloat_t pRandomFloat = 0;

	if( !pRandomSeed )
	{
		pRandomSeed = ( RandomSeed_t ) ( GetProcAddress( GetModuleHandleA( "vstdlib" ), "RandomSeed" ) );

		if( !pRandomSeed )
			return;
	}

	if( !pRandomFloat )
	{
		pRandomFloat = ( RandomFloat_t ) ( GetProcAddress( GetModuleHandleA( "vstdlib" ), "RandomFloat" ) );

		if( !pRandomFloat )
			return;
	}

	float flA = pRandomFloat( 0.0f, 1.0f );
	float flB = pRandomFloat( 0.0f, 6.283185f );
	float flC = pRandomFloat( 0.0f, 1.0f );
	float flD = pRandomFloat( 0.0f, 6.283185f );

	if( pWeapon->GetWeaponID() == WEAPON_R8REVOLVER )
	{
		if( UserCmd::pCmd && UserCmd::pCmd->buttons & IN_ATTACK2 )
		{
			flA = 1.0 - ( flA * flA );
			flC = 1.0 - ( flC * flC );
		}
	}
	else
		if( pWeapon->GetWeaponID() == WEAPON_NEGEV && pWeapon->GetRecoilIndex() < 3.f )
		{
			for( int i = 3; i > pWeapon->GetRecoilIndex(); --i )
			{
				flA *= flA;
				flC *= flC;
			}

			flA = 1.f - flA;
			flC = 1.f - flC;
		}

	float flForward[ 3 ], flRight[ 3 ], flUp[ 3 ];
	Vector vSpread;
	Vector vDirection;
	Vector vView;
	Vector vStart;

	vStart = pLocal->GetEyePosition();

	vSpread.x = ( cos( flB ) * ( flA * pWeapon->GetInaccuracy() ) ) + ( cos( flD ) * ( flC * pWeapon->GetSpread() ) );
	vSpread.y = ( sin( flB ) * ( flA * pWeapon->GetInaccuracy() ) ) + ( sin( flD ) * ( flC * pWeapon->GetSpread() ) );

	AngleVector( vViewAngles, flForward, flRight, flUp );

	vDirection.x = flForward[ 0 ] + vSpread.x * flRight[ 0 ] + vSpread.y * flUp[ 0 ];
	vDirection.y = flForward[ 1 ] + vSpread.x * flRight[ 1 ] + vSpread.y * flUp[ 1 ];
	vDirection.z = flForward[ 2 ] + vSpread.x * flRight[ 2 ] + vSpread.y * flUp[ 2 ];

	vView.x = 8192.0f * vDirection.x;
	vView.y = 8192.0f * vDirection.y;
	vView.z = 8192.0f * vDirection.z;

	vBullet = vStart + vView;
}

void CMath::FixMovement( const Vector& old_view, CUserCmd *cmd, CBaseEntity* local )
{
	Vector ViewForward, ViewRight, ViewUp, cmdForward, cmdRight, cmdUp;
	Vector cmdView = cmd->viewangles;

	NormalizeVector( cmdView );

	AngleVector( old_view, &ViewForward, &ViewRight, &ViewUp );
	AngleVector( cmdView, &cmdForward, &cmdRight, &cmdUp );

	float v8 = sqrt( ( ViewForward.x * ViewForward.x ) + ( ViewForward.y * ViewForward.y ) );
	float v10 = sqrt( ( ViewRight.x * ViewRight.x ) + ( ViewRight.y * ViewRight.y ) );
	float v12 = sqrt( ViewUp.z * ViewUp.z );

	Vector	NormalizedViewForward( ( 1.0 / v8 ) * ViewForward.x, ( 1.0 / v8 ) * ViewForward.y, ( 1.0 / v8 ) * 0.0 ),
		NormalizedViewRight( ( 1.0 / v10 ) * ViewRight.x, ( 1.0 / v10 ) * ViewRight.y, ( 1.0 / v10 ) * 0.0 ),
		NormalizedViewUp( ( 1.0 / v12 ) * 0.0, ( 1.0 / v12 ) * 0.0, ( 1.0 / v12 ) * ViewUp.z );

	float v14 = sqrt( ( cmdForward.x * cmdForward.x ) + ( cmdForward.y * cmdForward.y ) );
	float v16 = sqrt( ( cmdRight.x * cmdRight.x ) + ( cmdRight.y * cmdRight.y ) );
	float v18 = sqrt( cmdUp.z * cmdUp.z );

	Vector	NormalizedCmdForward( ( 1.0 / v14 ) * cmdForward.x, ( 1.0 / v14 ) * cmdForward.y, ( 1.0 / v14 ) * 0.0 ),
		NormalizedCmdRight( ( 1.0 / v16 ) * cmdRight.x, ( 1.0 / v16 ) * cmdRight.y, ( 1.0 / v16 ) * 0.0 ),
		NormalizedCmdUp( ( 1.0 / v18 ) * 0.0, ( 1.0 / v18 ) * 0.0, ( 1.0 / v18 ) * cmdUp.z );

	float v22 = NormalizedViewForward.x * cmd->forwardmove;
	float v26 = NormalizedViewForward.y * cmd->forwardmove;
	float v28 = NormalizedViewForward.z * cmd->forwardmove;
	float v24 = NormalizedViewRight.x * cmd->sidemove;
	float v23 = NormalizedViewRight.y * cmd->sidemove;
	float v25 = NormalizedViewRight.z * cmd->sidemove;
	float v30 = NormalizedViewUp.x * cmd->upmove;
	float v27 = NormalizedViewUp.z * cmd->upmove;
	float v29 = NormalizedViewUp.y * cmd->upmove;

	cmd->forwardmove = ( ( ( ( NormalizedCmdForward.x * v24 ) + ( NormalizedCmdForward.y * v23 ) ) + ( NormalizedCmdForward.z * v25 ) )
		+ ( ( ( NormalizedCmdForward.x * v22 ) + ( NormalizedCmdForward.y * v26 ) ) + ( NormalizedCmdForward.z * v28 ) ) )
		+ ( ( ( NormalizedCmdForward.y * v30 ) + ( NormalizedCmdForward.x * v29 ) ) + ( NormalizedCmdForward.z * v27 ) );
	cmd->sidemove = ( ( ( ( NormalizedCmdRight.x * v24 ) + ( NormalizedCmdRight.y * v23 ) ) + ( NormalizedCmdRight.z * v25 ) )
		+ ( ( ( NormalizedCmdRight.x * v22 ) + ( NormalizedCmdRight.y * v26 ) ) + ( NormalizedCmdRight.z * v28 ) ) )
		+ ( ( ( NormalizedCmdRight.x * v29 ) + ( NormalizedCmdRight.y * v30 ) ) + ( NormalizedCmdRight.z * v27 ) );
	cmd->upmove = ( ( ( ( NormalizedCmdUp.x * v23 ) + ( NormalizedCmdUp.y * v24 ) ) + ( NormalizedCmdUp.z * v25 ) )
		+ ( ( ( NormalizedCmdUp.x * v26 ) + ( NormalizedCmdUp.y * v22 ) ) + ( NormalizedCmdUp.z * v28 ) ) )
		+ ( ( ( NormalizedCmdUp.x * v30 ) + ( NormalizedCmdUp.y * v29 ) ) + ( NormalizedCmdUp.z * v27 ) );

	//Better safe than sorry
	clamp( cmd->forwardmove, -450.f, 450.f );
	clamp( cmd->sidemove, -450.f, 450.f );
	clamp( cmd->upmove, -320.f, 320.f );

	//Fix Sliding
	cmd->buttons &= ~IN_MOVERIGHT;
	cmd->buttons &= ~IN_MOVELEFT;
	cmd->buttons &= ~IN_LEFT;
	cmd->buttons &= ~IN_RIGHT;
	cmd->buttons &= ~IN_FORWARD;
	cmd->buttons &= ~IN_BACK;

	if( g_Var.Misc.Slide && local->GetMoveType() != 8 && local->GetMoveType() != 9)
	{
		if (cmd->forwardmove < 0.f)
			cmd->buttons |= IN_FORWARD;
		else
			if (cmd->forwardmove > 0.f)
				cmd->buttons |= IN_BACK;

		if (cmd->sidemove < 0.f)
		{
			cmd->buttons |= IN_MOVERIGHT;
			cmd->buttons |= IN_RIGHT;
		}
		else
			if (cmd->sidemove > 0.f)
			{
				cmd->buttons |= IN_MOVELEFT;
				cmd->buttons |= IN_LEFT;
			}
	}
	else
	{
		if( cmd->forwardmove > 0.f )
			cmd->buttons |= IN_FORWARD;
		else
			if( cmd->forwardmove < 0.f )
				cmd->buttons |= IN_BACK;

		if( cmd->sidemove > 0.f )
		{
			cmd->buttons |= IN_MOVERIGHT;
			cmd->buttons |= IN_RIGHT;
		}
		else
			if( cmd->sidemove < 0.f )
			{
				cmd->buttons |= IN_MOVELEFT;
				cmd->buttons |= IN_LEFT;
			}
	}
}

void VecAngles( const Vector &vecForward, Vector &vecAngles )
{
	Vector vecView;
	if( vecForward[ 1 ] == 0.f && vecForward[ 0 ] == 0.f )
	{
		vecView[ 0 ] = 0.f;
		vecView[ 1 ] = 0.f;
	}
	else
	{
		vecView[ 1 ] = atan2( vecForward[ 1 ], vecForward[ 0 ] ) * 180.f / M_PI;

		if( vecView[ 1 ] < 0.f )
			vecView[ 1 ] += 360.f;

		vecView[ 2 ] = sqrt( vecForward[ 0 ] * vecForward[ 0 ] + vecForward[ 1 ] * vecForward[ 1 ] );

		vecView[ 0 ] = atan2( vecForward[ 2 ], vecView[ 2 ] ) * 180.f / M_PI;
	}

	vecAngles[ 0 ] = -vecView[ 0 ];
	vecAngles[ 1 ] = vecView[ 1 ];
	vecAngles[ 2 ] = 0.f;
}

Vector CMath::RotateMatrix(CBaseEntity* pLocal, int bone, float rotation)
{
	Vector bone_pos;
	Vector origin;
	Vector bone_offset;

	bone_pos = GetHitspot(pLocal, bone);
	origin = pLocal->GetOrigin();
	bone_offset = bone_pos - origin;

	float radius = bone_offset.Length2D();
	float radian = rotation * M_PI;

	bone_offset.x = cos(radian) * radius;
	bone_offset.y = sin(radian) * radius;

	bone_offset += origin;

	return bone_offset;
}

void CMath::VectorRotate(const float *in1, const matrix3x4_t& in2, float *out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}

void CMath::VectorRotate(const Vector& in1, const matrix3x4_t &in2, Vector &out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

void CMath::VectorRotate(const Vector &in1, const Vector &in2, Vector &out)
{
	matrix3x4_t matRotate;
	AngleMatrix(in2, matRotate);
	VectorRotate(in1, matRotate, out);
}

void MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			target[i][j] = source[i][j];
		}
	}
}

void CMath::MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2)
{
	matrix3x4_t out;
	if (&in1 == &out)
	{
		matrix3x4_t in1b;
		MatrixCopy(in1, in1b);
		MatrixMultiply(in1b, in2);
		return;
	}

	if (&in2 == &out)
	{
		matrix3x4_t in2b;
		MatrixCopy(in2, in2b);
		MatrixMultiply(in1, in2b);
		return;
	}

	// 1  0
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	in1 = out;
}

matrix3x4_t CMath::MultiplyMatrix(matrix3x4_t& in1, const matrix3x4_t& in2)
{
	matrix3x4_t out;
	if (&in1 == &out)
	{
		matrix3x4_t in1b;
		MatrixCopy(in1, in1b);
		MatrixMultiply(in1b, in2);
		return out;
	}

	if (&in2 == &out)
	{
		matrix3x4_t in2b;
		MatrixCopy(in2, in2b);
		MatrixMultiply(in1, in2b);
		return out;
	}

	// 1  0
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	return out;
}

void CMath::RotateMovement( CUserCmd* pCmd, float flYaw ) 
{
	Vector vView;
	g_pEngine->GetViewAngles( vView );

	float flRotation = DEG2RAD( vView.y - flYaw );

	float flCos = cos( flRotation );
	float flSin = sin( flRotation );

	float new_forwardmove = ( flCos * pCmd->forwardmove ) - ( flSin * pCmd->sidemove );
	float new_sidemove = ( flSin * pCmd->forwardmove ) + ( flCos * pCmd->sidemove );

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

void CMath::GetSpreadFix( CBaseEntity* pLocal, UINT seed, Vector& pflInAngles, CUserCmd *pCmd )
{
	pflInAngles.x = g_Math.NormalizeYaw( pflInAngles.x );
	pflInAngles.y = g_Math.NormalizeYaw( pflInAngles.y );

	typedef void( __cdecl* RandomSeed_t )( int );
	static RandomSeed_t pRandomSeed = 0;

	typedef float( __cdecl* RandomFloat_t )( float, float );
	static RandomFloat_t pRandomFloat = 0;

	if( !pRandomSeed )
	{
		pRandomSeed = ( RandomSeed_t ) ( GetProcAddress( GetModuleHandleA( "vstdlib" ), "RandomSeed" ) );

		if( !pRandomSeed )
			return;
	}

	if( !pRandomFloat )
	{
		pRandomFloat = ( RandomFloat_t ) ( GetProcAddress( GetModuleHandleA( "vstdlib" ), "RandomFloat" ) );

		if( !pRandomFloat )
			return;
	}

	CBaseCombatWeapon * pWeapon = pLocal->GetWeapon();

	if( !pLocal || !pWeapon )
		return;

	pWeapon->UpdateAccuracyPenalty();

	pRandomSeed( seed ); //FX_FireBullets in sdk_fx_shared.cpp

/*	auto fRand1 = pRandomFloat( 0.0f, 1.0f );
	const auto fRandPi1 = pRandomFloat( 0.0f, 2.0f * M_PI );
	auto fRand2 = pRandomFloat( 0.0f, 1.f );
	const auto fRandPi2 = pRandomFloat( 0.0f, 2.0f * M_PI );

	if( pWeapon->GetWeaponID() == WEAPON_R8REVOLVER && ( pCmd->buttons & IN_ATTACK2 ) )
	{
		fRand1 = 1.f - fRand1 * fRand1;
		fRand2 = 1.f - fRand2 * fRand2;
	}
	else 
		if( pWeapon->GetWeaponID() == WEAPON_NEGEV && pWeapon->GetRecoilIndex() < 3.f )
	{
		for( auto i = 3; i > pWeapon->GetRecoilIndex(); i-- )
		{
			fRand1 *= fRand1;
			fRand2 *= fRand2;
		}

		fRand1 = 1.f - fRand1;
		fRand2 = 1.f - fRand2;
	}

	const auto flRandInaccuracy = fRand1 * pWeapon->GetInaccuracy();
	const auto flRandSpread = fRand2 * pWeapon->GetSpread();

	const auto flSpreadX = cos( fRandPi1 ) * flRandInaccuracy + cos( fRandPi2 ) * flRandSpread;
	const auto flSpreadY = sin( fRandPi1 ) * flRandInaccuracy + sin( fRandPi2 ) * flRandSpread;

	pCmd->viewangles.x += RAD2DEG( atan( sqrt( flSpreadX * flSpreadX + flSpreadY * flSpreadY ) ) );
	pCmd->viewangles.z = -RAD2DEG( atan2( flSpreadX, flSpreadY ) );*/

	float flA = pRandomFloat( 0.0f, 1.0f );
	float flB = pRandomFloat( 0.0f, 6.283185f );
	float flC = pRandomFloat( 0.0f, 1.0f );
	float flD = pRandomFloat( 0.0f, 6.283185f );

	if( pWeapon->GetWeaponID() == WEAPON_R8REVOLVER && ( pCmd->buttons & IN_ATTACK2 ) )
	{
		flA = 1.0 - ( flA * flA );
		flC = 1.0 - ( flC * flC );
	}
	else
		if( pWeapon->GetWeaponID() == WEAPON_NEGEV && pWeapon->GetRecoilIndex() < 3.f )
		{
			for( int i = 3; i > pWeapon->GetRecoilIndex(); --i )
			{
				flA *= flA;
				flC *= flC;
			}

			flA = 1.f - flA;
			flC = 1.f - flC;
		}

	float vForward[ 3 ], vRight[ 3 ], vUp[ 3 ];
	Vector vDir;
	Vector vView, vSpread;

	vSpread.x = ( cos( flB ) * ( flA * pWeapon->GetInaccuracy() ) ) + ( cos( flD ) * ( flC * pWeapon->GetSpread() ) );
	vSpread.y = ( sin( flB ) * ( flA * pWeapon->GetInaccuracy() ) ) + ( sin( flD ) * ( flC * pWeapon->GetSpread() ) );

	Vector vAng = pflInAngles;

	Vector qDifference;
	Vector EstimatedViewAngles;

	float fDiff = 0.0f;
	float fDiffOld = 180.0f;

	for( ;; )
	{
		AngleVector( pflInAngles, vForward, vRight, vUp );

		vDir.x = vForward[ 0 ] + vSpread.x * vRight[ 0 ] + vSpread.y * vUp[ 0 ];
		vDir.y = vForward[ 1 ] + vSpread.x * vRight[ 1 ] + vSpread.y * vUp[ 1 ];
		vDir.z = vForward[ 2 ] + vSpread.x * vRight[ 2 ] + vSpread.y * vUp[ 2 ];

		vView.x = 8192.0f * vDir.x;
		vView.y = 8192.0f * vDir.y;
		vView.z = 8192.0f * vDir.z;

		VecAngles( vView, EstimatedViewAngles );

		NormalizeVector( EstimatedViewAngles );

		qDifference = vAng - EstimatedViewAngles;

		NormalizeVector( qDifference );

		fDiff = sqrt( ( qDifference.x * qDifference.x ) + ( qDifference.y * qDifference.y ) );

		if( ( fDiff <= 0.001f ) || ( fDiff >= fDiffOld ) )
			break;

		fDiffOld = fDiff;

		pflInAngles += qDifference;

		g_Math.NormalizeVector( qDifference );
	}
}

/*  if ( sub_108BDB60(a2, "weapon_bone") )
		goto LABEL_18;
	  if ( sub_108BDB60(a2, "Bip01_Head") )
	  {
		v5 = sub_1017B5F0(v2);
		return sub_1070CA90(v5, "head_0");
	  }
	  if ( sub_108BDB60(a2, "L_Hand") )
	  {
		v7 = sub_1017B5F0(v2);
		return sub_1070CA90(v7, "hand_L");
	  }
	  if ( sub_108BDB60(a2, "R_Hand") )
	  {
LABEL_18:
		v8 = sub_1017B5F0(v2);
		v4 = sub_1070CA90(v8, "hand_R");*/

/*void AdjustEyePos(void* animstate, Vector* pos)
{
//55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14
	static auto lookup_bone_fn = (int(__thiscall*)(void*, const char*))g_Pattern.PatternSearchIDA("client.dll", "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 0F 84 ? ? ? ?");

	if (*reinterpret_cast<bool*>(uintptr_t(animstate) + 0x109) /// in hitground
		&& *reinterpret_cast<float*>(uintptr_t(animstate) + 0x29) != 0.f) /// unk
	{
		auto base_entity = *reinterpret_cast<void**>(uintptr_t(animstate) + 0x60);

		auto bone_pos = get_bone_pos(reinterpret_cast<CBaseEntity*>(base_entity), lookup_bone_fn(base_entity, "head_0"));

		bone_pos.z += 1.7f;

		if ((*pos).z > bone_pos.z)
		{
			float some_factor = 0.f;

			float delta = (*pos).z - bone_pos.z;

			float some_offset = (delta - 4.f) / 6.f;
			if (some_offset >= 0.f)
				some_factor = std::fminf(some_offset, 1.f);

			(*pos).z += ((bone_pos.z - (*pos).z) * (((some_factor * some_factor) * 3.f) - (((some_factor * some_factor) * 2.f) * some_factor)));
		}
	}
}

Vector GetFixedEyePos()
{
	Vector vPos;

	eye_pos(&vPos);

	if (*reinterpret_cast<int32_t*>(uintptr_t(this) + 0x3AB0))
	{
		auto anim_state = *reinterpret_cast<void**>(uintptr_t(this) + 0x3900);
		if (anim_state)
			modify_eye_pos(anim_state, &vPos);
	}

	return vPos;
}*/
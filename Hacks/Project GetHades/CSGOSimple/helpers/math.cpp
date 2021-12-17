#include "math.hpp"

c_math g_math;

//--------------------------------------------------------------------------------
QAngle c_math::calc_angle ( const Vector& src, const Vector& dst ) const
{
    QAngle angles;
    Vector delta = src - dst;

    vector_angles ( delta, angles );

    angles.Normalize( );

    return angles;
}

//--------------------------------------------------------------------------------
float c_math::get_fov ( const QAngle& view_angle, const QAngle& aim_angle ) const
{
    Vector ang, aim;

    angle_vectors ( view_angle, aim );
    angle_vectors ( aim_angle, ang );

    return rad2deg ( acos ( aim.dot ( ang ) / aim.length_sqr() ) );
}

//--------------------------------------------------------------------------------
void c_math::clamp_angles ( QAngle& angles ) const
{
    if ( angles.pitch > 89.0f )
        angles.pitch = 89.0f;
    else if ( angles.pitch < -89.0f )
        angles.pitch = -89.0f;

    if ( angles.yaw > 180.0f )
        angles.yaw = 180.0f;
    else if ( angles.yaw < -180.0f )
        angles.yaw = -180.0f;

    angles.roll = 0;
}

void c_math::clamp_yaw ( float& yaw ) const
{
    if ( yaw > 180.0f )
        yaw = 180.0f;
    else if ( yaw < -180.0f )
        yaw = -180.0f;
}

void c_math::normalize_angles ( QAngle& angles ) const
{
    for ( auto i = 0; i < 3; i++ )
    {
        while ( angles[ i ] < -180.0f )
            angles[ i ] += 360.0f;

        while ( angles[ i ] > 180.0f )
            angles[ i ] -= 360.0f;
    }
}

float c_math::angle_normalize ( float angle ) const
{
    angle = fmodf ( angle, 360.0f );

    if ( angle > 180 )
        angle -= 360;

    if ( angle < -180 )
        angle += 360;

    return angle;
}

void c_math::normalize_yaw ( float& angles ) const
{
    while ( angles < -180.0f )
        angles += 360.0f;

    while ( angles > 180.0f )
        angles -= 360.0f;
}

//--------------------------------------------------------------------------------
void c_math::vector_transform ( const Vector& in1, const matrix3x4_t& in2, Vector& out ) const
{
    out[ 0 ] = in1.dot ( in2[ 0 ] ) + in2[ 0 ][ 3 ];
    out[ 1 ] = in1.dot ( in2[ 1 ] ) + in2[ 1 ][ 3 ];
    out[ 2 ] = in1.dot ( in2[ 2 ] ) + in2[ 2 ][ 3 ];
}

//--------------------------------------------------------------------------------
void c_math::angle_vectors ( const QAngle& angles, Vector& forward ) const
{
    float sp, sy, cp, cy;

    DirectX::XMScalarSinCos ( &sp, &cp, deg2rad ( angles[0] ) );
    DirectX::XMScalarSinCos ( &sy, &cy, deg2rad ( angles[1] ) );

    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
}

//--------------------------------------------------------------------------------
void c_math::angle_vectors ( const QAngle& angles, Vector& forward, Vector& right, Vector& up ) const
{
    float sr, sp, sy, cr, cp, cy;

    DirectX::XMScalarSinCos ( &sp, &cp, deg2rad ( angles[0] ) );
    DirectX::XMScalarSinCos ( &sy, &cy, deg2rad ( angles[1] ) );
    DirectX::XMScalarSinCos ( &sr, &cr, deg2rad ( angles[2] ) );

    forward.x = ( cp * cy );
    forward.y = ( cp * sy );
    forward.z = ( -sp );
    right.x = ( -1 * sr * sp * cy + -1 * cr * -sy );
    right.y = ( -1 * sr * sp * sy + -1 * cr * cy );
    right.z = ( -1 * sr * cp );
    up.x = ( cr * sp * cy + -sr * -sy );
    up.y = ( cr * sp * sy + -sr * cy );
    up.z = ( cr * cp );
}

//--------------------------------------------------------------------------------

float c_math::degrees_to_radians ( const float deg ) const
{
    return deg * ( m_pi / 180.f );
}

float c_math::radians_to_degrees ( const float rad ) const
{
    return rad * m_radpi;
}

void c_math::rotate_triangle ( std::array< Vector2D, 3 >& points, float rotation )
{
    const auto points_center = ( points.at ( 0 ) + points.at ( 1 ) + points.at ( 2 ) ) / 3;

    for ( auto& point : points )
    {
        point -= points_center;

        const auto temp_x = point.x;
        const auto temp_y = point.y;

        const auto theta = degrees_to_radians ( rotation );
        const auto c = cosf ( theta );
        const auto s = sinf ( theta );

        point.x = temp_x * c - temp_y * s;
        point.y = temp_x * s + temp_y * c;

        point += points_center;
    }
}

float c_math::fl_approach_angle ( float fl_target, float fl_value, float fl_speed )
{
    auto fl_angle_mod = [] ( float fl_angle ) -> float
    {
        return ( ( 360.0f / 65536.0f ) * ( ( int32_t )( fl_angle * ( 65536.0f / 360.0f ) ) & 65535 ) );
    };

    float fl_adjusted_speed = fl_speed;

    if ( fl_adjusted_speed < 0.0f )
        fl_adjusted_speed *= -1.0f;

    float fl_angle_mod_target = fl_angle_mod ( fl_target );
    float fl_angle_mod_value = fl_angle_mod ( fl_value );

    float fl_delta = ( fl_angle_mod_target - fl_angle_mod_value );

    if ( fl_delta >= -180.0f )
    {
        if ( fl_delta >= 180.0f )
            fl_delta -= 360.0f;
    }
    else
    {
        if ( fl_delta <= -180.0f )
            fl_delta += 360.0f;
    }

    float fl_return;

    if ( fl_delta <= fl_adjusted_speed )
    {
        if ( ( fl_adjusted_speed * -1.0f ) <= fl_delta )
            fl_return = fl_angle_mod_target;
        else
            fl_return = ( fl_angle_mod_value - fl_adjusted_speed );
    }
    else
        fl_return = ( fl_angle_mod_value + fl_adjusted_speed );

    return fl_return;
}

void c_math::vector_angles ( const Vector& forward, QAngle& angles ) const
{
    if ( forward[ 1 ] == 0.0f && forward[ 0 ] == 0.0f )
    {
        angles[ 0 ] = ( forward[ 2 ] > 0.0f ) ? 270.0f : 90.0f;
        angles[ 1 ] = 0.0f;
    }
    else
    {
        angles[ 0 ] = atan2 ( -forward[ 2 ], forward.length2d( ) ) * -180 / m_pi;
        angles[ 1 ] = atan2 ( forward[ 1 ], forward[ 0 ] ) * 180 / m_pi;

        if ( angles[ 1 ] > 90 )
            angles[ 1 ] -= 180;
        else if ( angles[ 1 ] < 90 )
            angles[ 1 ] += 180;
        else
            if ( angles[ 1 ] == 90 )
                angles[ 1 ] = 0;
    }

    angles[ 2 ] = 0.0f;
}

//--------------------------------------------------------------------------------
bool c_math::screen_transform ( const Vector& in, Vector& out ) const
{
    static auto& w2sMatrix = interfaces::engine_client->world_to_screen_matrix( );

    out.x = w2sMatrix.m[ 0 ][ 0 ] * in.x + w2sMatrix.m[ 0 ][ 1 ] * in.y + w2sMatrix.m[ 0 ][ 2 ] * in.z + w2sMatrix.m[ 0 ][ 3 ];
    out.y = w2sMatrix.m[ 1 ][ 0 ] * in.x + w2sMatrix.m[ 1 ][ 1 ] * in.y + w2sMatrix.m[ 1 ][ 2 ] * in.z + w2sMatrix.m[ 1 ][ 3 ];
    out.z = 0.0f;

    float w = w2sMatrix.m[ 3 ][ 0 ] * in.x + w2sMatrix.m[ 3 ][ 1 ] * in.y + w2sMatrix.m[ 3 ][ 2 ] * in.z + w2sMatrix.m[ 3 ][ 3 ];

    if ( w < 0.001f )
    {
        out.x *= 100000;
        out.y *= 100000;
        return false;
    }

    out.x /= w;
    out.y /= w;

    return true;
}

//--------------------------------------------------------------------------------
bool c_math::world_to_screen ( const Vector& in, Vector& out ) const
{
    if ( screen_transform ( in, out ) )
    {
        int w, h;
        interfaces::engine_client->get_screen_size ( w, h );

        out.x = ( w / 2.0f ) + ( out.x * w ) / 2.0f;
        out.y = ( h / 2.0f ) - ( out.y * h ) / 2.0f;

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------
float c_math::random_float ( float min, float max ) const
{
    static auto ranFloat = reinterpret_cast< float ( *) ( float, float ) > ( GetProcAddress ( GetModuleHandleW ( L"vstdlib.dll" ), "RandomFloat" ) );

    if ( ranFloat )
        return ranFloat ( min, max );
    else
        return 0.f;
}

//--------------------------------------------------------------------------------
Vector c_math::cross_product2 ( const Vector& a, const Vector& b ) const
{
    return Vector ( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

//--------------------------------------------------------------------------------
void c_math::vector_angles ( const Vector& forward, Vector& up, QAngle& angles ) const
{
    Vector left = cross_product2 ( up, forward );
    left.normalize_in_place( );

    float forwardDist = forward.length2d( );

    if ( forwardDist > 0.001f )
    {
        angles.pitch = atan2f ( -forward.z, forwardDist ) * 180 / pi_f;
        angles.yaw = atan2f ( forward.y, forward.x ) * 180 / pi_f;

        float upZ = ( left.y * forward.x ) - ( left.x * forward.y );
        angles.roll = atan2f ( left.z, upZ ) * 180 / pi_f;
    }
    else
    {
        angles.pitch = atan2f ( -forward.z, forwardDist ) * 180 / pi_f;
        angles.yaw = atan2f ( -left.x, left.y ) * 180 / pi_f;
        angles.roll = 0;
    }
}

float c_math::distance_to_ray ( const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along, Vector* pointOnRay ) const
{
    Vector to = pos - rayStart;
    Vector dir = rayEnd - rayStart;
    float length = dir.normalize( );

    float rangeAlong = dir.dot ( to );

    if ( along )
        *along = rangeAlong;

    float range;

    if ( rangeAlong < 0.0f )
    {
        range = - ( pos - rayStart ).length( );

        if ( pointOnRay )
            *pointOnRay = rayStart;
    }
    else if ( rangeAlong > length )
    {
        range = - ( pos - rayEnd ).length( );

        if ( pointOnRay )
            *pointOnRay = rayEnd;
    }
    else
    {
        Vector onRay = rayStart + rangeAlong * dir;
        range = ( pos - onRay ).length( );

        if ( pointOnRay )
            *pointOnRay = onRay;
    }

    return range;
}

float c_math::angle_diff ( const float dest_angle, const float src_angle ) const
{
    auto delta = fmodf ( dest_angle - src_angle, 360.0f );

    if ( dest_angle > src_angle )
    {
        if ( delta >= 180 )
            delta -= 360;
    }
    else
    {
        if ( delta <= -180 )
            delta += 360;
    }

    return delta;
}

void c_math::rotate_matrix ( QAngle current_ang, QAngle new_ang, Vector origin, matrix3x4_t* matrix_in, matrix3x4_t* matrix_out )
{
    if ( !matrix_in )
        return;

    Vector OutPos;
    matrix3x4_t BoneMatrix[maxstudiobones];

    float ang = current_ang.yaw - new_ang.yaw;

    for ( int i = 0; i < maxstudiobones; i++ )
    {
        angle_matrix ( QAngle ( 0.f, ang, 0.f ), BoneMatrix[ i ] );
        matrix_multiply ( BoneMatrix[ i ], matrix_in[ i ] );
        Vector BonePos = Vector ( matrix_in[ i ][ 0 ][ 3 ], matrix_in[ i ][ 1 ][ 3 ], matrix_in[ i ][ 2 ][ 3 ] ) - origin;
        vector_rotate ( BonePos, QAngle ( 0.f, ang, 0.f ), OutPos );
        OutPos += origin;
        BoneMatrix[ i ][ 0 ][ 3 ] = OutPos.x;
        BoneMatrix[ i ][ 1 ][ 3 ] = OutPos.y;
        BoneMatrix[ i ][ 2 ][ 3 ] = OutPos.z;
    }

    matrix_out = BoneMatrix;
}

//--------------------------------------------------------------------------------
void c_math::angle_matrix ( const QAngle angles, matrix3x4_t& matrix ) const
{
    if ( !&matrix )
        return;

    float sy = sin ( deg2rad ( angles[1] ) );
    float cy = cos ( deg2rad ( angles[1] ) );

    float sp = sin ( deg2rad ( angles[0] ) );
    float cp = cos ( deg2rad ( angles[0] ) );

    float sr = sin ( deg2rad ( angles[2] ) );
    float cr = cos ( deg2rad ( angles[2] ) );

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

void c_math::matrix_set_column ( const Vector& in, int column, matrix3x4_t& out ) const
{
    out[ 0 ][ column ] = in.x;
    out[ 1 ][ column ] = in.y;
    out[ 2 ][ column ] = in.z;
}

void c_math::angle_matrix ( const QAngle& angles, const Vector& position, matrix3x4_t& matrix_out ) const
{
    angle_matrix ( angles, matrix_out );
    matrix_set_column ( position, 3, matrix_out );
}

void c_math::matrix_copy ( const matrix3x4_t& source, matrix3x4_t& target ) const
{
    if ( !&target || !&source || !target[ 0 ] || !target[ 1 ] || !target[ 2 ] || &target[ 2 ][ 0 ] == nullptr || &target[ 2 ][ 1 ] == nullptr || &target[ 2 ]
        [ 2 ] == nullptr )
        return;

    for ( int i = 0; i < 3; i++ )
    {
        for ( int j = 0; j < 4; j++ )
            target[ i ][ j ] = source[ i ][ j ];
    }
}

void c_math::matrix_multiply ( matrix3x4_t& in1, const matrix3x4_t& in2 ) const
{
    if ( !&in2 || !&in1 || !in2[ 0 ] || !in2[ 1 ] || !in2[ 2 ] || &in2[ 2 ][ 0 ] == nullptr || &in2[ 2 ][ 1 ] == nullptr )
        return;

    matrix3x4_t out;

    if ( &in1 == &out )
    {
        matrix3x4_t in1b;
        matrix_copy ( in1, in1b );
        matrix_multiply ( in1b, in2 );
        return;
    }

    if ( &in2 == &out )
    {
        matrix3x4_t in2b;
        matrix_copy ( in2, in2b );
        matrix_multiply ( in1, in2b );
        return;
    }

    if ( !&in2 || !&in1 )
        return;

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

void c_math::vector_rotate ( const float* in1, const matrix3x4_t& in2, float* out )
{
    out[ 0 ] = dot_product ( in1, in2[ 0 ] );
    out[ 1 ] = dot_product ( in1, in2[ 1 ] );
    out[ 2 ] = dot_product ( in1, in2[ 2 ] );
}

void c_math::vector_rotate ( const Vector& in1, const matrix3x4_t& in2, Vector& out )
{
    vector_rotate ( &in1.x, in2, &out.x );
}

void c_math::vector_rotate ( const Vector& in1, const QAngle& in2, Vector& out )
{
    matrix3x4_t matRotate;
    angle_matrix ( in2, matRotate );
    vector_rotate ( in1, matRotate, out );
}

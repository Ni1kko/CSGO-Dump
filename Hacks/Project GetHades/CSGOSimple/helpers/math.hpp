#pragma once

#include "../valve_sdk/sdk.hpp"
#include <cmath>
#include <DirectXMath.h>
#include <array>

#define rad2deg(x) DirectX::XMConvertToDegrees(x)
#define deg2rad(x) DirectX::XMConvertToRadians(x)
#define m_pi 3.14159265358979323846
#define m_radpi 57.295779513082f
#define pi_f	(static_cast<float>(m_pi))

#define tick_interval			(interfaces::global_vars->interval_per_tick)
#define ticks_to_time(t) (interfaces::global_vars->interval_per_tick * (t) )
#define time_to_ticks( dt )		static_cast<int>( lround( lround( 0.5f + (float)(dt) / tick_interval ) ) )

class c_math
{
public:
    float angle_diff ( float dest_angle, float src_angle ) const;

    inline void sin_cos ( float a, float* s, float* c ) const
    {
        *s = sin ( a );
        *c = cos ( a );
    }

    /// does not work rn
    void rotate_matrix ( QAngle current_ang, QAngle new_ang, Vector origin, matrix3x4_t* matrix_in, matrix3x4_t* matrix_out );

    FORCEINLINE float dot_product ( const float* v1, const float* v2 ) const
    {
        return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
    }

    void angle_matrix ( const QAngle angles, matrix3x4_t& matrix ) const;
    void matrix_set_column ( const Vector& in, int column, matrix3x4_t& out ) const;
    void angle_matrix ( const QAngle& angles, const Vector& position, matrix3x4_t& matrix_out ) const;
    void matrix_copy ( const matrix3x4_t& source, matrix3x4_t& target ) const;
    void matrix_multiply ( matrix3x4_t& in1, const matrix3x4_t& in2 ) const;
    void vector_rotate ( const float* in1, const matrix3x4_t& in2, float* out );
    void vector_rotate ( const Vector& in1, const matrix3x4_t& in2, Vector& out );
    void vector_rotate ( const Vector& in1, const QAngle& in2, Vector& out );
    void rotate_triangle ( std::array< Vector2D, 3 >& points, float rotation );
    float fl_approach_angle ( float fl_target, float fl_value, float fl_speed );

    inline float fastsqrt ( float x ) const
    {
        unsigned int i = * ( unsigned int* )&x;

        i += 127 << 23;
        // approximation of square root
        i >>= 1;
        return * ( float* )&i;
    }

    float radians_to_degrees ( const float rad ) const;
    float degrees_to_radians ( const float deg ) const;
    QAngle calc_angle ( const Vector& src, const Vector& dst ) const;
    float get_fov ( const QAngle& view_angle, const QAngle& aim_angle ) const;
    template < class T >
    void clamp_angles ( QAngle& angles );
    void normalize_angles ( QAngle& angles ) const;
    void normalize_yaw ( float& angles ) const;
    void vector_transform ( const Vector& in1, const matrix3x4_t& in2, Vector& out ) const;
    void angle_vectors ( const QAngle& angles, Vector& forward ) const;
    void angle_vectors ( const QAngle& angles, Vector& forward, Vector& right, Vector& up ) const;
    void vector_angles ( const Vector& forward, QAngle& angles ) const;
    bool world_to_screen ( const Vector& in, Vector& out ) const;
    float random_float ( float min, float max ) const;
    void vector_angles ( const Vector& forward, Vector& up, QAngle& angles ) const;
    float distance_to_ray ( const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr ) const;
    void clamp_angles ( QAngle& angles ) const;
    void clamp_yaw ( float& yaw ) const;
    float angle_normalize ( float angle ) const;

private:
    Vector cross_product2 ( const Vector& a, const Vector& b ) const;
    bool screen_transform ( const Vector& in, Vector& out ) const;
};

extern c_math g_math;

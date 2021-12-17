#pragma once

#include <sstream>

class Vector
{
public:
    Vector ( void )
    {
        invalidate( );
    }

    Vector ( float X, float Y, float Z )
    {
        x = X;
        y = Y;
        z = Z;
    }

    Vector ( const float* clr )
    {
        x = clr[ 0 ];
        y = clr[ 1 ];
        z = clr[ 2 ];
    }

    /*
    void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)
    {
    	result.x = a.y*b.z - a.z*b.y;
    	result.y = a.z*b.x - a.x*b.z;
    	result.z = a.x*b.y - a.y*b.x;
    }
    */
    void vector_cross_product ( const Vector& a, const Vector& b, Vector& result ) const;

    Vector cross ( const Vector& vOther ) const
    {
        Vector res;
        vector_cross_product ( *this, vOther, res );
        return res;
    }

    [[nodiscard]] float normalize ( ) const
    {
        auto res = *this;
        auto l = res.length( );

        if ( l != 0.0f )
            res /= l;
        else
            res.x = res.y = res.z = 0.0f;

        return l;
    }

    void init ( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f )
    {
        x = ix;
        y = iy;
        z = iz;
    }

    bool is_valid ( ) const
    {
        return std::isfinite ( x ) && std::isfinite ( y ) && std::isfinite ( z );
    }

    void invalidate ( )
    {
        x = y = z = std::numeric_limits< float >::infinity( );
    }

    float& operator[] ( int i )
    {
        return ( ( float* )this )[ i ];
    }

    float operator[] ( int i ) const
    {
        return ( ( float* )this )[ i ];
    }

    void zero ( )
    {
        x = y = z = 0.0f;
    }

    bool operator== ( const Vector& src ) const
    {
        return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
    }

    bool operator!= ( const Vector& src ) const
    {
        return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
    }

    Vector& operator+= ( const Vector& v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator-= ( const Vector& v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector& operator*= ( float fl )
    {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }

    Vector& operator*= ( const Vector& v )
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vector& operator/= ( const Vector& v )
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    Vector& operator+= ( float fl )
    {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }

    Vector& operator/= ( float fl )
    {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }

    Vector& operator-= ( float fl )
    {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }

    void normalize_in_place ( )
    {
        *this = normalized( );
    }

    Vector normalized ( ) const
    {
        auto res = *this;
        auto l = res.length( );

        if ( l != 0.0f )
            res /= l;

        else
            res.x = res.y = res.z = 0.0f;

        return res;
    }

    float dist_to ( const Vector& vOther ) const
    {
        Vector delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.length( );
    }

    float dist_to_sqr ( const Vector& vOther ) const
    {
        Vector delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.length_sqr( );
    }

    float dot ( const Vector& vOther ) const
    {
        return ( x * vOther.x + y * vOther.y + z * vOther.z );
    }

    float length ( ) const
    {
        return sqrt ( x * x + y * y + z * z );
    }

    float length_sqr ( void ) const
    {
        return ( x * x + y * y + z * z );
    }

    float length2d ( ) const
    {
        return sqrt ( x * x + y * y );
    }

    Vector& operator= ( const Vector& vOther )
    {
        x = vOther.x;
        y = vOther.y;
        z = vOther.z;
        return *this;
    }

    Vector Vector::operator- ( void ) const
    {
        return Vector ( -x, -y, -z );
    }

    Vector Vector::operator+ ( const Vector& v ) const
    {
        return Vector ( x + v.x, y + v.y, z + v.z );
    }

    Vector Vector::operator- ( const Vector& v ) const
    {
        return Vector ( x - v.x, y - v.y, z - v.z );
    }

    Vector Vector::operator* ( float fl ) const
    {
        return Vector ( x * fl, y * fl, z * fl );
    }

    Vector Vector::operator* ( const Vector& v ) const
    {
        return Vector ( x * v.x, y * v.y, z * v.z );
    }

    Vector Vector::operator/ ( float fl ) const
    {
        return Vector ( x / fl, y / fl, z / fl );
    }

    Vector Vector::operator/ ( const Vector& v ) const
    {
        return Vector ( x / v.x, y / v.y, z / v.z );
    }

    float x, y, z;
};

inline Vector operator* ( float lhs, const Vector& rhs )
{
    return rhs * lhs;
}

inline Vector operator/ ( float lhs, const Vector& rhs )
{
    return rhs / lhs;
}

class __declspec ( align ( 16 ) ) vector_aligned : public Vector
{
public:
    inline vector_aligned ( void )
    {
    };

    inline vector_aligned ( float X, float Y, float Z )
    {
        init ( X, Y, Z );
    }

public:
    explicit vector_aligned ( const Vector& vOther )
    {
        init ( vOther.x, vOther.y, vOther.z );
    }

    vector_aligned& operator= ( const Vector& vOther )
    {
        init ( vOther.x, vOther.y, vOther.z );
        return *this;
    }

    vector_aligned& operator= ( const vector_aligned& vOther )
    {
        init ( vOther.x, vOther.y, vOther.z );
        return *this;
    }

    float w{ };
};

typedef Vector vector_t;

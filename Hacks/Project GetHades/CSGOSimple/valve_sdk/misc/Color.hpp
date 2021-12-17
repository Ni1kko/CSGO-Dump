#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Color
{
public:
    Color ( );
    Color ( int _r, int _g, int _b );
    Color ( int _r, int _g, int _b, int _a );

    Color ( float _r, float _g, float _b ) : Color ( _r, _g, _b, 1.0f )
    {
    }

    Color ( float _r, float _g, float _b, float _a )
        : Color (
            static_cast< int > ( _r * 255.0f ),
            static_cast< int > ( _g * 255.0f ),
            static_cast< int > ( _b * 255.0f ),
            static_cast< int > ( _a * 255.0f ) )
    {
    }

    explicit Color ( float* rgb ) : Color ( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ], 1.0f )
    {
    }

    explicit Color ( unsigned long argb )
    {
        _CColor[ 2 ] = static_cast< unsigned char > ( ( argb & 0x000000FF ) >> ( 0 * 8 ) );
        _CColor[ 1 ] = static_cast< unsigned char > ( ( argb & 0x0000FF00 ) >> ( 1 * 8 ) );
        _CColor[ 0 ] = static_cast< unsigned char > ( ( argb & 0x00FF0000 ) >> ( 2 * 8 ) );
        _CColor[ 3 ] = static_cast< unsigned char > ( ( argb & 0xFF000000 ) >> ( 3 * 8 ) );
    }

    void SetRawColor ( int color32 );
    int GetRawColor ( ) const;
    void SetColor ( int _r, int _g, int _b, int _a = 0 );
    void SetColor ( float _r, float _g, float _b, float _a = 0 );
    void GetColor ( int& _r, int& _g, int& _b, int& _a ) const;

    int at ( const int idx ) const
    {
        return _CColor[ idx ];
    }

    std::string GetNormalnijHexColor ( ) const;

    int r ( ) const
    {
        return _CColor[ 0 ];
    }

    int g ( ) const
    {
        return _CColor[ 1 ];
    }

    int b ( ) const
    {
        return _CColor[ 2 ];
    }

    int a ( ) const
    {
        return _CColor[ 3 ];
    }

    unsigned char& operator[] ( const int index )
    {
        return _CColor[ index ];
    }

    const unsigned char& operator[] ( const int index ) const
    {
        return _CColor[ index ];
    }

    bool operator== ( const Color& rhs ) const;
    bool operator!= ( const Color& rhs ) const;
    Color& operator= ( const Color& rhs );

    static Color FromHSB ( const float hue, const float saturation, const float brightness )
    {
        const auto h = hue == 1.0f ? 0 : hue * 6.0f;
        const auto f = h - static_cast< int > ( h );
        const auto p = brightness * ( 1.0f - saturation );
        const auto q = brightness * ( 1.0f - saturation * f );
        const auto t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

        if ( h < 1 )
        {
            return Color (
                static_cast< unsigned char > ( brightness * 255 ),
                static_cast< unsigned char > ( t * 255 ),
                static_cast< unsigned char > ( p * 255 )
            );
        }
        else if ( h < 2 )
        {
            return Color (
                static_cast< unsigned char > ( q * 255 ),
                static_cast< unsigned char > ( brightness * 255 ),
                static_cast< unsigned char > ( p * 255 )
            );
        }
        else if ( h < 3 )
        {
            return Color (
                static_cast< unsigned char > ( p * 255 ),
                static_cast< unsigned char > ( brightness * 255 ),
                static_cast< unsigned char > ( t * 255 )
            );
        }
        else if ( h < 4 )
        {
            return Color (
                static_cast< unsigned char > ( p * 255 ),
                static_cast< unsigned char > ( q * 255 ),
                static_cast< unsigned char > ( brightness * 255 )
            );
        }
        else if ( h < 5 )
        {
            return Color (
                static_cast< unsigned char > ( t * 255 ),
                static_cast< unsigned char > ( p * 255 ),
                static_cast< unsigned char > ( brightness * 255 )
            );
        }
        else
        {
            return Color (
                static_cast< unsigned char > ( brightness * 255 ),
                static_cast< unsigned char > ( p * 255 ),
                static_cast< unsigned char > ( q * 255 )
            );
        }
    }

    static Color black;
    static Color white;
    static Color red;
    static Color green;
    static Color blue;

private:
    unsigned char _CColor[4];
};

typedef Color color_t;

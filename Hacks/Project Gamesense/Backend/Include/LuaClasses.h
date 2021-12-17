#pragma once
#include <math.h>
#include <minmax.h>
#include <algorithm>

struct LuaColor
{
	unsigned char RGBA[4];

	LuaColor()
	{
		RGBA[0] = 255;
		RGBA[1] = 255;
		RGBA[2] = 255;
		RGBA[3] = 255;
	}

	LuaColor( int r, int g, int b, int a = 255 )
	{
		RGBA[0] = r;
		RGBA[1] = g;
		RGBA[2] = b;
		RGBA[3] = a;
	}

	void set( int r, int g, int b, int a = 255 )
	{
		RGBA[0] = r;
		RGBA[1] = g;
		RGBA[2] = b;
		RGBA[3] = a;
	}

	inline int r() const
	{
		return RGBA[0];
	}
	inline int g() const
	{
		return RGBA[1];
	}
	inline int b() const
	{
		return RGBA[2];
	}
	inline int a() const
	{
		return RGBA[3];
	}

	bool operator!=( LuaColor color )
	{
		return RGBA[0] != color.RGBA[0] || RGBA[1] != color.RGBA[1] || RGBA[2] != color.RGBA[2] || RGBA[3] != color.RGBA[3];
	}
	bool operator==( LuaColor color )
	{
		return RGBA[0] == color.RGBA[0] && RGBA[1] == color.RGBA[1] && RGBA[2] == color.RGBA[2] && RGBA[3] == color.RGBA[3];
	}

	bool notEquals( LuaColor color )
	{
		return RGBA[0] != color.RGBA[0] || RGBA[1] != color.RGBA[1] || RGBA[2] != color.RGBA[2] || RGBA[3] != color.RGBA[3];
	}
	bool equals( LuaColor color )
	{
		return RGBA[0] == color.RGBA[0] && RGBA[1] == color.RGBA[1] && RGBA[2] == color.RGBA[2] && RGBA[3] == color.RGBA[3];
	}

	static float Base( const unsigned char col )
	{
		return col / 255.f;
	}

	static LuaColor Inverse( const LuaColor color )
	{
		return LuaColor( 255 - color.RGBA[0], 255 - color.RGBA[1], 255 - color.RGBA[2] );
	}

	float Difference( const LuaColor color ) const // from 0.f - 1.f with 1.f being the most different
	{
		float red_diff = fabs( Base( RGBA[0] ) - Base( color.RGBA[0] ) );
		float green_diff = fabs( Base( RGBA[1] ) - Base( color.RGBA[1] ) );
		float blue_diff = fabs( Base( RGBA[2] ) - Base( color.RGBA[2] ) );
		float alpha_diff = fabs( Base( RGBA[3] ) - Base( color.RGBA[3] ) );
		return ( red_diff + green_diff + blue_diff + alpha_diff ) * 0.25f;
	}

	float Hue() const
	{
		float R = Base( RGBA[0] );
		float G = Base( RGBA[1] );
		float B = Base( RGBA[2] );
		float mx = max( R, max( G, B ) );
		float mn = min( R, min( G, B ) );

		if ( mx == mn )
			return 0.f;

		float delta = mx - mn;
		float hue = 0.f;

		if ( mx == R )
			hue = ( G - B ) / delta;

		else
			if ( mx == G )
				hue = 2.f + ( B - R ) / delta;

			else
				hue = 4.f + ( R - G ) / delta;

		hue *= 60.f;

		if ( hue < 0.f )
			hue += 360.f;

		return hue / 360.f;
	}

	float Saturation() const
	{
		float R = Base( RGBA[0] );
		float G = Base( RGBA[1] );
		float B = Base( RGBA[2] );
		float mx = max( R, max( G, B ) );
		float mn = min( R, min( G, B ) );
		float delta = mx - mn;

		if ( mx == 0.f )
			return delta;

		return delta / mx;
	}

	float Brightness() const
	{
		float R = Base( RGBA[0] );
		float G = Base( RGBA[1] );
		float B = Base( RGBA[2] );
		return max( R, max( G, B ) );
	}

	static LuaColor FromHSB( float hue, float saturation, float brightness, int alpha = 255 )
	{
		hue = std::clamp<float>( hue, 0.f, 1.f );
		saturation = std::clamp<float>( saturation, 0.f, 1.f );
		brightness = std::clamp<float>( brightness, 0.f, 1.f );
		float h = ( hue == 1.f ) ? 0.f : ( hue * 6.f );
		float f = h - static_cast<int>( h );
		float p = brightness * ( 1.f - saturation );
		float q = brightness * ( 1.f - saturation * f );
		float t = brightness * ( 1.f - ( saturation * ( 1.f - f ) ) );

		if ( h < 1.f )
			return LuaColor( brightness * 255, t * 255, p * 255, alpha );

		else
			if ( h < 2.f )
				return LuaColor( q * 255, brightness * 255, p * 255, alpha );

			else
				if ( h < 3.f )
					return LuaColor( p * 255, brightness * 255, t * 255, alpha );

				else
					if ( h < 4 )
						return LuaColor( p * 255, q * 255, brightness * 255, alpha );

					else
						if ( h < 5 )
							return LuaColor( t * 255, p * 255, brightness * 255, alpha );

						else
							return LuaColor( brightness * 255, p * 255, q * 255, alpha );
	}
};

struct LuaGuiColor
{
	float RGBA[4];

	LuaGuiColor( float* col )
	{
		RGBA[0] = col[0];
		RGBA[1] = col[1];
		RGBA[2] = col[2];
		RGBA[3] = col[3];
	}

	LuaGuiColor( LuaColor col )
	{
		RGBA[0] = ( float )col.r() / 255.f;
		RGBA[1] = ( float )col.g() / 255.f;
		RGBA[2] = ( float )col.b() / 255.f;
		RGBA[3] = ( float )col.a() / 255.f;
	}

	LuaColor toColor()
	{
		return LuaColor( RGBA[0] * 255.f, RGBA[1] * 255.f, RGBA[2] * 255.f, RGBA[3] * 255.f );
	}
};

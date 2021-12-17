#include "color.hpp"

struct c_rgbb { double r, g, b; }; // a fraction between 0 and 1
struct c_hsvv { double h, s, v; }; // a fraction between 0 and 1, but hue is in degrees (0 to 360)

c_hsvv rgb_to_hsvv( const c_rgbb &in ) {
	c_hsvv result;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	result.v = max;
	delta = max - min;

	if ( delta < 0.0001 ) {
		result.s = 0;
		result.h = 0;

		return result;
	}

	if ( max > 0 ) {
		result.s = ( delta / max );
	}
	else {
		result.s = 0;
		result.h = 0;

		return result;
	}

	if ( in.r >= max ) {
		result.h = ( in.g - in.b ) / delta;
	}
	else {
		if ( in.g >= max ) {
			result.h = 2 + ( in.b - in.r ) / delta;
		}
		else {
			result.h = 4 + ( in.r - in.g ) / delta;
		}
	}

	result.h *= 60;

	if ( result.h < 0 )
		result.h += 360;

	return result;
}

Color Color::Black( 0, 0, 0, 255 );
Color Color::White( 255, 255, 255, 255 );
Color Color::Red( 255, 0, 0, 255 );
Color Color::Green( 70, 255, 70, 255 );
Color Color::Blue( 0, 0, 255, 255 );
Color Color::MenuMain( 225, 0, 255, 255 );

Color::Color( )
{
	*( ( int * )this ) = 0;
}

Color::Color( int _r, int _g, int _b )
{
	setColor( _r, _g, _b, 255 );
}

Color::Color( int _r, int _g, int _b, int _a )
{
	setColor( _r, _g, _b, _a );
}

void Color::setRawColor( int color32 )
{
	*( ( int * )this ) = color32;
}

int Color::getRawColor( ) const
{
	return *( ( int * )this );
}

__inline void Color::setColor( int _r, int _g, int _b, int _a )
{
	_CColor[ 0 ] = ( unsigned char )_r;
	_CColor[ 1 ] = ( unsigned char )_g;
	_CColor[ 2 ] = ( unsigned char )_b;
	_CColor[ 3 ] = ( unsigned char )_a;

	auto meme = rgb_to_hsvv( { ( float )_r / 255.f, ( float )_g / 255.f, ( float )_b / 255.f } );
	h = meme.h;
	s = meme.s;
	v = meme.v;
}

__inline void Color::setColor( float _r, float _g, float _b, float _a )
{
	_CColor[ 0 ] = static_cast< unsigned char >( _r * 255.0f );
	_CColor[ 1 ] = static_cast< unsigned char >( _g * 255.0f );
	_CColor[ 2 ] = static_cast< unsigned char >( _b * 255.0f );
	_CColor[ 3 ] = static_cast< unsigned char >( _a * 255.0f );

	auto meme = rgb_to_hsvv( { _r, _g, _b } );
	h = meme.h;
	s = meme.s;
	v = meme.v;
}

void Color::setColor( float* color )
{
	if ( !color )
		return;

	_CColor[ 0 ] = ( unsigned char )( color[ 0 ] * 255.f );
	_CColor[ 1 ] = ( unsigned char )( color[ 1 ] * 255.f );
	_CColor[ 2 ] = ( unsigned char )( color[ 2 ] * 255.f );
	_CColor[ 3 ] = ( unsigned char )( color[ 3 ] * 255.f );

	auto meme = rgb_to_hsvv( { color[ 0 ], color[ 1 ], color[ 2 ] } );
	h = meme.h;
	s = meme.s;
	v = meme.v;
}

void Color::getColor( int &_r, int &_g, int &_b, int &_a ) const
{
	_r = _CColor[ 0 ];
	_g = _CColor[ 1 ];
	_b = _CColor[ 2 ];
	_a = _CColor[ 3 ];
}

bool Color::operator== ( const Color &rhs ) const
{
	return ( *( ( int * )this ) == *( ( int * )&rhs ) );
}

bool Color::operator!= ( const Color &rhs ) const
{
	return !( operator==( rhs ) );
}

Color& Color::operator=( const Color &rhs )
{
	setRawColor( rhs.getRawColor( ) );
	return *this;
}
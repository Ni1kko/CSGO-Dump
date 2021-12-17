#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

#include "../sdk/color.hpp"
#include "../sdk/vector.hpp"

struct vertice_t
{
	float x, y, z, rhw;
	int c;
};

enum font_flags_d3d
{
	font_centered = ( 1 << 1 ),
	font_dropshadow = ( 1 << 2 ),
	font_outline = ( 1 << 3 ),
};

class c_font
{
private:
	void draw( std::string text, Vector2 pos, Color c, int flags )
	{
		RECT rect;
		SetRect( &rect, pos.x, pos.y, pos.x, pos.y );
		p_font->DrawTextA( NULL, text.c_str( ), -1, &rect, ( ( flags & font_flags_d3d::font_centered ) ? DT_CENTER : DT_LEFT ) | DT_NOCLIP, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) );
	}
	std::string name;
	int height;
	unsigned int weight;
	bool anti_aliased = false;
public:
	c_font( std::string name, int height, int weight, bool anti_aliased )
	{
		this->name = name;
		this->height = height;
		this->weight = weight;
		this->anti_aliased = anti_aliased;
	}
	c_font( std::string name, int height, int weight )
	{
		this->name = name;
		this->height = height;
		this->weight = weight;
	}

	IDirect3DDevice9* device;
	LPD3DXFONT p_font;
	void setup( IDirect3DDevice9* p_device )
	{
		this->device = p_device;
	}
	void init( )
	{
		D3DXCreateFontA( device, height, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, anti_aliased ? DRAFT_QUALITY : ANTIALIASED_QUALITY, DEFAULT_PITCH, name.c_str( ), &p_font );
	}
	void invalidate( )
	{
		p_font->Release( );
	}
	void draw_text( std::string text, Vector2 pos, Color c, int flags )
	{
		if ( flags & font_flags_d3d::font_dropshadow )
		{
			draw( text, pos + Vector2( 1, 1 ), Color( 0, 0, 0, c.a( ) ), flags );
		}

		if ( flags & font_flags_d3d::font_outline )
		{
			draw( text, Vector2( pos.x - 1, pos.y ), Color( 0, 0, 0, c.a( ) ), flags );
			draw( text, Vector2( pos.x + 1, pos.y - 1 ), Color( 0, 0, 0, c.a( ) ), flags );
			draw( text, Vector2( pos.x, pos.y - 1 ), Color( 0, 0, 0, c.a( ) ), flags );
			draw( text, Vector2( pos.x, pos.y + 1 ), Color( 0, 0, 0, c.a( ) ), flags );
		}

		draw( text, pos, c, flags );
	}
	Vector2 measure_size( std::string in )
	{
		auto rect = RECT( );
		this->p_font->DrawTextA( 0, in.c_str( ), strlen( in.c_str( ) ), &rect, DT_CALCRECT, D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
		return { float( rect.right - rect.left ), float( rect.bottom - rect.top ) };
	}

};

class c_renderer_d3d
{
	// sub calls
private:
	IDirect3DStateBlock9* state_block;
	IDirect3DVertexDeclaration9* vert_dec;
	IDirect3DVertexShader9* vert_shader;
	std::vector<c_font*> font_arr;

public:
	IDirect3DDevice9* device;

	c_font *menu_bold;
	c_font *menu_small;
	c_font *menu;
	c_font *indicator;

	bool initialized = false;

	void set_render_states( )
	{
		device->SetVertexShader( nullptr );
		device->SetPixelShader( nullptr );
		device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
		device->SetRenderState( D3DRS_LIGHTING, FALSE );
		device->SetRenderState( D3DRS_FOGENABLE, FALSE );
		device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		device->SetRenderState( D3DRS_ZENABLE, FALSE );
		device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

		device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
		device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );

		device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
		device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
		device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

		device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
		device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	}

	void init( IDirect3DDevice9* p_device )
	{
		device = p_device;

		menu_bold = new c_font( "Tahoma", 12, 700 );
		menu_bold->setup( device );
		menu_bold->init( );

		menu_small = new c_font( "Small Fonts", 9, 400 );
		menu_small->setup( device );
		menu_small->init( );

		menu = new c_font( "Tahoma", 12, 400 );
		menu->setup( device );
		menu->init( );

		indicator = new c_font( "Verdana", 26, 800, true );
		indicator->setup( device );
		indicator->init( );

		initialized = true;
	}

	void invalidate( )
	{
		menu_bold->invalidate( );
		menu_small->invalidate( );
		menu->invalidate( );
		indicator->invalidate( );
	}

	void restore( IDirect3DDevice9* p_device )
	{
		device = p_device;

		menu_bold->setup( device );
		menu_bold->init( );

		menu_small->setup( device );
		menu_small->init( );

		menu->setup( device );
		menu->init( );

		indicator->setup( device );
		indicator->init( );
	}

	void set_viewport( D3DVIEWPORT9 vp )
	{
		device->SetViewport( &vp );
	}

	D3DVIEWPORT9 get_viewport( )
	{
		D3DVIEWPORT9 vp;
		device->GetViewport( &vp );
		return vp;
	}

	// drawing functions

	void rect( Vector2 a, Vector2 b, Color c )
	{
		b += a;

		b.x -= 1;
		b.y -= 1;

		vertice_t verts[ 5 ] = {
			{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ float( b.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ float( b.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ float( a.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) }
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &verts, 20 );
	}

	void rect( float x, float y, float w, float h, Color c )
	{
		rect( { x,y }, { w,h }, c );
	}

	void rect_fill( Vector2 a, Vector2 b, Color c )
	{
		b += a;

		vertice_t verts[ 4 ] = {
			{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )},
			{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )},
			{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )},
			{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )}
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
	}

	void draw_filled_triangle( Vector2 p1, Vector2 p2, Vector2 p3, Color c )
	{
		vertice_t verts[ ] = {
			{ p1.x, p1.y, 0.00f, 0.00f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ p2.x, p2.y, 0.00f, 0.00f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) },
			{ p3.x, p3.y, 0.00f, 0.00f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) }
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 1, &verts, 20 );
	}

	void rect_fill( float x, float y, float w, float h, Color c )
	{
		rect_fill( { x, y }, { w, h }, c );
	}

	void gradient_v( Vector2 a, Vector2 b, Color c_a, Color c_b )
	{
		b += a;

		vertice_t verts[ 4 ] = {
			{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) )},
			{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) )},
			{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) )},
			{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) )}
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
	}

	void gradient_v( float x, float y, float w, float h, Color c_a, Color c_b )
	{
		gradient_v( { x,y }, { w, h }, c_a, c_b );
	}

	void gradient_h( Vector2 a, Vector2 b, Color c_a, Color c_b )
	{
		b += a;

		vertice_t verts[ 4 ] = {
		{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) )},
		{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) )},
		{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r( ), c_a.g( ), c_a.b( ), c_a.a( ) )},
		{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r( ), c_b.g( ), c_b.b( ), c_b.a( ) )}
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
	}

	void gradient_h( float x, float y, float w, float h, Color c_a, Color c_b )
	{
		gradient_h( { x,y }, { w, h }, c_a, c_b );
	}

	void line( Vector2 a, Vector2 b, Color c )
	{
		vertice_t verts[ 2 ] = {
			{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )},
			{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) )}
		};

		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_LINELIST, 1, &verts, 20 );
	}

	void line( float x, float y, float x1, float x2, Color c )
	{
		line( { x, y }, { x1, x2 }, c );
	}

	struct VERTEX
	{
		float x, y, z;
		float rhw;
		DWORD color;
	};

	void clircleFilledDualColour( float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device )
	{
		LPDIRECT3DVERTEXBUFFER9 g_pVB2;

		std::vector<VERTEX> circle( resolution + 2 );

		float angle = rotate * D3DX_PI / 180;

		circle[ 0 ].x = x;
		circle[ 0 ].y = y;
		circle[ 0 ].z = 0;
		circle[ 0 ].rhw = 1;
		circle[ 0 ].color = color2;

		for ( int i = 1; i < resolution + 2; i++ )
		{
			circle[ i ].x = ( float )( x - rad * cos( D3DX_PI*( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
			circle[ i ].y = ( float )( y - rad * sin( D3DX_PI*( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
			circle[ i ].z = 0;
			circle[ i ].rhw = 1;
			circle[ i ].color = color;
		}

		int _res = resolution + 2;
		for ( int i = 0; i < _res; i++ )
		{
			circle[ i ].x = x + cos( angle )*( circle[ i ].x - x ) - sin( angle )*( circle[ i ].y - y );
			circle[ i ].y = y + sin( angle )*( circle[ i ].x - x ) + cos( angle )*( circle[ i ].y - y );
		}

		m_device->CreateVertexBuffer( ( resolution + 2 ) * sizeof( VERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL );

		VOID* pVertices;
		g_pVB2->Lock( 0, ( resolution + 2 ) * sizeof( VERTEX ), ( void** )&pVertices, 0 );
		memcpy( pVertices, &circle[ 0 ], ( resolution + 2 ) * sizeof( VERTEX ) );
		g_pVB2->Unlock( );

		m_device->SetTexture( 0, NULL );
		m_device->SetPixelShader( NULL );
		m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		m_device->SetStreamSource( 0, g_pVB2, 0, sizeof( VERTEX ) );
		m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
		m_device->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, resolution );

		if ( g_pVB2 != NULL )
			g_pVB2->Release( );
	}


	void circle( Vector2 a, int radius, int percent, Color c )
	{
		vertice_t* vertex = new vertice_t[ percent + 1 ];

		int x = a.x;
		int y = a.y;
		int r = radius;
		int p = percent;

		float pi = D3DX_PI * ( ( float )percent / 100.f );

		for ( int i = 0; i <= percent; i++ )
		{
			vertex[ i ] = { x + r * cos( pi * ( i / ( p / 2.0f ) ) ), y + r * sin( pi * ( i / ( p / 2.0f ) ) ), 0.0f, 1.0f, D3DCOLOR_RGBA( c.r( ), c.g( ), c.b( ), c.a( ) ) };
		}


		device->SetTexture( 0, nullptr );
		device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, percent, vertex, 20 );
	}

	void draw_text( int x, int y, c_font* font, const std::string &str, const Color &color, int flags )
	{
		font->draw_text( str, { x, y }, color, flags ? 4 : 4 );
	}

	void begin( )
	{
		device->CreateStateBlock( D3DSBT_PIXELSTATE, &state_block );

		device->GetVertexDeclaration( &vert_dec );
		device->GetVertexShader( &vert_shader );

		set_render_states( );
	}

	void end( )
	{
		state_block->Apply( );
		state_block->Release( );

		device->SetVertexDeclaration( vert_dec );
		device->SetVertexShader( vert_shader );
	}

};
inline c_renderer_d3d *g_renderer_d3d = new c_renderer_d3d( );

#pragma once

#include "sdk.h"

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,		// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,		// compiled bitmap font - no fallbacks
};

class VGUI_Surface
{
public:
	void DrawSetColor( int r, int g, int b, int a )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 15 )( this, r, g, b, a );
	}

	void DrawFilledRect( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 16 )( this, x0, y0, x1, y1 );
	}

	void DrawOutlinedRect( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* OriginalFn )( void*, int x0, int y0, int x1, int y1 );
		getvfunc<OriginalFn>( this, 18 )( this, x0, y0, x1, y1 );
	}

	void DrawLine( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 19 )( this, x0, y0, x1, y1 );
	}

	void DrawSetTextFont( unsigned long font )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, unsigned long );
		getvfunc<OriginalFn>( this, 23 )( this, font );
	}

	void DrawSetTextColor( int r, int g, int b, int a )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 25 )( this, r, g, b, a );
	}

	void DrawSetTextPos( int x, int y )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int );
		getvfunc<OriginalFn>( this, 26 )( this, x, y );
	}

	void DrawPrintText( const wchar_t *text, int textLen )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const wchar_t *, int, int );
		return getvfunc<OriginalFn>( this, 28 )( this, text, textLen, 0 );
	}

	void DrawSetTextureRGBA( int id, const Color *rgba, int wide, int tall )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, const Color *, int, int );
		return getvfunc<OriginalFn>( this, 37 )( this, id, rgba, wide, tall );
	}

	void DrawSetTextureRGBA( int id, const unsigned char *rgba, int wide, int tall )
	{
		typedef void( __thiscall* OriginalFn )( void*, int, const unsigned char*, int, int );
		return getvfunc<OriginalFn>( this, 37 )( this, id, rgba, wide, tall );
	}

	void DrawSetTexture( int iTextureID )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int);
		return getvfunc<OriginalFn>( this, 38 )( this, iTextureID );
	}

	void DrawTexturedRect( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		return getvfunc<OriginalFn>( this, 41 )( this, x0, y0, x1, y1 );
	}

	bool IsTextureIDValid( int id )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 42 )( this, id );
	}

	int CreateNewTextureID( bool procedural = false )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, bool );
		return getvfunc<OriginalFn>( this, 43 )( this, procedural );
	}

	void UnlockCursor()
	{
		typedef void( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 66 )( this );
	}

	void LockCursor()
	{
		typedef void( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 67 )( this );
	}

	unsigned long CreateFont()
	{
		typedef unsigned int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 71 )( this );
	}

	void SetFontGlyphSet( unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, unsigned long, const char*, int, int, int, int, int, int, int );
		getvfunc<OriginalFn>( this, 72 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0 );
	}

	bool AddCustomFontFile( const char* fontName )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID, const char* );
		return getvfunc<OriginalFn>( this, 73 )( this, fontName );
	}

	void GetTextSize( unsigned long font, const wchar_t *text, int &wide, int &tall )
	{
		typedef void( __thiscall* OriginalFn )( void*, unsigned long font, const wchar_t *text, int &wide, int &tall );
		getvfunc<OriginalFn>( this, 79 )( this, font, text, wide, tall );
	}

	void PlaySoundFile( const char* fileName ) 
	{
		typedef void( __thiscall* OriginalFn )( void*, const char* );
		return getvfunc<OriginalFn>( this, 82 )( this, fileName );
	}

	void SurfaceGetCursorPos( int &x, int &y )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int &, int & );
		getvfunc<OriginalFn>( this, 100 )( this, x, y );
	}

	void SurfaceSetCursorPos( int x, int y )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int );
		getvfunc<OriginalFn>( this, 101 )( this, x, y );
	}

	void DrawOutlinedCircle( int x, int y, int radius, int segments )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int );
		getvfunc<OriginalFn>( this, 103 )( this, x, y, radius, segments );
	}

	void DrawTexturedPolygon( int n, Vertex_t *pVertice, bool bClipVertices = true )
	{
		typedef void( __thiscall* OriginalFn )( void*, int, Vertex_t*, bool );
		return getvfunc<OriginalFn>( this, 106 )( this, n, pVertice, bClipVertices );
	}

	void DrawWordBubble( int x0, int y0, int x1, int y1, int nBorderThickness, Color rgbaBackground, Color rgbaBorder,
		bool bPointer = false, int nPointerX = 0, int nPointerY = 0, int nPointerBaseThickness = 16 )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int, int, int, int, int, Color, Color, bool, int, int, int );
		getvfunc<OriginalFn>( this, 110 )( this, x0, y0, x1, y1, nBorderThickness, rgbaBackground, rgbaBorder, bPointer, nPointerX, nPointerY, nPointerBaseThickness );
	}

	const char* GetFontName( unsigned long font )
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned long );
		return getvfunc<OriginalFn>( this, 134 )( this, font );
	}
};

extern VGUI_Surface* g_pSurface;
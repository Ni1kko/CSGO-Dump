#include "drawing.h"

CDraw g_Draw;

void CDraw::InitFonts()
{
	visualsfont = g_pSurface->CreateFontA();
	keyfont = g_pSurface->CreateFontA();
	tabfont = g_pSurface->CreateFontA();
	espfont = g_pSurface->CreateFontA();
	iconfont = g_pSurface->CreateFontA();
	iconmenu = g_pSurface->CreateFontA();
	indicatorfont = g_pSurface->CreateFontA();

	g_pSurface->SetFontGlyphSet( iconmenu, "undefeated", 22, 500, 0, 0, FONTFLAG_ANTIALIAS );
	g_pSurface->SetFontGlyphSet( iconfont, "undefeated", 11, 200, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS );
	g_pSurface->SetFontGlyphSet( visualsfont, "Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE );
	g_pSurface->SetFontGlyphSet( keyfont, "Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS );
	g_pSurface->SetFontGlyphSet( tabfont, "Tahoma", 14, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW );
	g_pSurface->SetFontGlyphSet( espfont, "Verdana", 11, 500, 0, 0, FONTFLAG_ANTIALIAS );
	g_pSurface->SetFontGlyphSet( indicatorfont, "Verdana", 21, 1000, 0, 0, FONTFLAG_ANTIALIAS );
}

void CDraw::DrawOutlineRect( int x, int y, int w, int h, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawOutlinedRect( x, y, x + w, y + h );
}

void CDraw::FillRGBA( int x, int y, int w, int h, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawFilledRect( x, y, x + w, y + h );
}

void CDraw::LineRGBA( int x1, int y1, int x2, int y2, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawLine( x1, y1, x2, y2 );
}

void CDraw::DrawRect( int x1, int y1, int x2, int y2, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawFilledRect( x1, y1, x2, y2 );
}

void CDraw::DrawCircle( int x, int y, int radius, int segments, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawOutlinedCircle( x, y, radius, segments );
}

void CDraw::Draw3DCube( Vector* points, int r, int g, int b, int a )
{
	Vector startPos, endPos;

	for( int i = 0; i < 3; i++ )
	{
		if( g_pDebugOverlay->ScreenPosition( points[ i ], startPos ) != 1 )
			if( g_pDebugOverlay->ScreenPosition( points[ i + 1 ], endPos ) != 1 )
				LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );
	}
	if( g_pDebugOverlay->ScreenPosition( points[ 0 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 3 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );

	for( int i = 4; i < 7; i++ )
	{
		if( g_pDebugOverlay->ScreenPosition( points[ i ], startPos ) != 1 )
			if( g_pDebugOverlay->ScreenPosition( points[ i + 1 ], endPos ) != 1 )
				LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );
	}
	if( g_pDebugOverlay->ScreenPosition( points[ 4 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 7 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );

	if( g_pDebugOverlay->ScreenPosition( points[ 0 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 6 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );

	if( g_pDebugOverlay->ScreenPosition( points[ 1 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 5 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );

	if( g_pDebugOverlay->ScreenPosition( points[ 2 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 4 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );

	if( g_pDebugOverlay->ScreenPosition( points[ 3 ], startPos ) != 1 )
		if( g_pDebugOverlay->ScreenPosition( points[ 7 ], endPos ) != 1 )
			LineRGBA( startPos.x, startPos.y, endPos.x, endPos.y, r, g, b, a );
}

void CDraw::Draw3DCube( float scalar, Vector angles, Vector middle_origin, int r, int g, int b, int a )
{
	Vector forward, right, up;
	g_Math.AngleVector( angles, &forward, &right, &up );

	Vector points [ 8 ];
	points [ 0 ] = middle_origin - ( right * scalar ) + ( up * scalar ) - ( forward * scalar ); // BLT
	points [ 1 ] = middle_origin + ( right * scalar ) + ( up * scalar ) - ( forward * scalar ); // BRT
	points [ 2 ] = middle_origin - ( right * scalar ) - ( up * scalar ) - ( forward * scalar ); // BLB
	points [ 3 ] = middle_origin + ( right * scalar ) - ( up * scalar ) - ( forward * scalar ); // BRB

	points [ 4 ] = middle_origin - ( right * scalar ) + ( up * scalar ) + ( forward * scalar ); // FLT
	points [ 5 ] = middle_origin + ( right * scalar ) + ( up * scalar ) + ( forward * scalar ); // FRT
	points [ 6 ] = middle_origin - ( right * scalar ) - ( up * scalar ) + ( forward * scalar ); // FLB
	points [ 7 ] = middle_origin + ( right * scalar ) - ( up * scalar ) + ( forward * scalar ); // FRB

	Vector points_screen [ 8 ];
	for( int i = 0; i < 8; i++ )
		if( !g_pDebugOverlay->ScreenPosition( points [ i ], points_screen [ i ] ) != 1 )
			return;

	g_pSurface->DrawSetColor( r, g, b, a );

	// Back frame
	g_pSurface->DrawLine( points_screen [ 0 ].x, points_screen [ 0 ].y, points_screen [ 1 ].x, points_screen [ 1 ].y );
	g_pSurface->DrawLine( points_screen [ 0 ].x, points_screen [ 0 ].y, points_screen [ 2 ].x, points_screen [ 2 ].y );
	g_pSurface->DrawLine( points_screen [ 3 ].x, points_screen [ 3 ].y, points_screen [ 1 ].x, points_screen [ 1 ].y );
	g_pSurface->DrawLine( points_screen [ 3 ].x, points_screen [ 3 ].y, points_screen [ 2 ].x, points_screen [ 2 ].y );

	// Frame connector
	g_pSurface->DrawLine( points_screen [ 0 ].x, points_screen [ 0 ].y, points_screen [ 4 ].x, points_screen [ 4 ].y );
	g_pSurface->DrawLine( points_screen [ 1 ].x, points_screen [ 1 ].y, points_screen [ 5 ].x, points_screen [ 5 ].y );
	g_pSurface->DrawLine( points_screen [ 2 ].x, points_screen [ 2 ].y, points_screen [ 6 ].x, points_screen [ 6 ].y );
	g_pSurface->DrawLine( points_screen [ 3 ].x, points_screen [ 3 ].y, points_screen [ 7 ].x, points_screen [ 7 ].y );

	// Front frame
	g_pSurface->DrawLine( points_screen [ 4 ].x, points_screen [ 4 ].y, points_screen [ 5 ].x, points_screen [ 5 ].y );
	g_pSurface->DrawLine( points_screen [ 4 ].x, points_screen [ 4 ].y, points_screen [ 6 ].x, points_screen [ 6 ].y );
	g_pSurface->DrawLine( points_screen [ 7 ].x, points_screen [ 7 ].y, points_screen [ 5 ].x, points_screen [ 5 ].y );
	g_pSurface->DrawLine( points_screen [ 7 ].x, points_screen [ 7 ].y, points_screen [ 6 ].x, points_screen [ 6 ].y );
}

void DrawTexturedPoly( int n, Vertex_t* vertice, Color col )
{
	static int texture_id = g_pSurface->CreateNewTextureID( true );
	static unsigned char buf[ 4 ] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA( texture_id, buf, 1, 1 );
	g_pSurface->DrawSetColor( col.r(), col.g(), col.b(), col.a() );
	g_pSurface->DrawSetTexture( texture_id );
	g_pSurface->DrawTexturedPolygon( n, vertice );
}

void DrawTexturedPoly( int n, VertexCol_t* vertice )
{
	static int texture_id = g_pSurface->CreateNewTextureID( true );
	static unsigned char buf[ 4 ] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA( texture_id, buf, 1, 1 );
	g_pSurface->DrawSetColor( vertice->m_Col.r(), vertice->m_Col.g(), vertice->m_Col.b(), 255 );
	g_pSurface->DrawSetTexture( texture_id );
	g_pSurface->DrawTexturedPolygon( n, &vertice->m_Vertex );
}

void CDraw::DrawFilledTriangle( Vector2D one, Vector2D two, Vector2D three, int r, int g, int b, int a )
{
	static Vertex_t MouseVt [ 3 ];

	MouseVt [ 0 ].Init( one );
	MouseVt [ 1 ].Init( two );
	MouseVt [ 2 ].Init( three );

	DrawTexturedPoly( 3, MouseVt, Color( r, g, b, a ) );
}

void CDraw::DrawFilledArrow( Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, int red, int green, int blue, int alpha )
{
	static Vertex_t MouseVt[ 7 ];

	MouseVt[ 0 ].Init( a );
	MouseVt[ 1 ].Init( b );
	MouseVt[ 2 ].Init( c );
	MouseVt[ 3 ].Init( d );
	MouseVt[ 4 ].Init( e );
	MouseVt[ 5 ].Init( f );
	MouseVt[ 6 ].Init( g );

	DrawTexturedPoly( 7, MouseVt, Color( red, green, blue, alpha ) );
}

void CDraw::DrawFilledCircle( Vector2D center, int r, int g, int b, int a, float radius, float points )
{
	std::vector<Vertex_t> vertices;
	float step = ( float ) M_PI * 2.0f / points;

	for( float a = 0; a < ( M_PI * 2.0f ); a += step )
		vertices.push_back( Vertex_t( Vector2D( radius * cosf( a ) + center.x, radius * sinf( a ) + center.y ) ) );

	DrawTexturedPoly( ( int ) points, vertices.data(), Color( r, g, b, a ) );
}

void CDraw::DrawGradient( int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2 )
{
	FillRGBA( x, y, w, h, r1, g1, b1, a1 );

	int r = r2;
	int g = g2;
	int b = b2;

	for( int i = 0; i < h; i++ )
	{
		float flIndex = i, flHeight = h;
		float flAlpha = flIndex / flHeight;

		int iAlpha = flAlpha * 255;

		FillRGBA( x, y + i, w, 1, r, g, b, iAlpha );
	}
}

void CDraw::DrawGradientH( int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2 )
{
	FillRGBA( x, y, w, h, r1, g1, b1, a1 );

	int r = r2;
	int g = g2;
	int b = b2;

	for( int i = 0; i < w; i++ )
	{
		float flIndex = i, flWidth = w;
		float flAlpha = flIndex / flWidth;

		int iAlpha = flAlpha * 255;

		FillRGBA( x + i, y, 2, h, r, g, b, iAlpha );
	}
}

void CDraw::DrawBackGround( int x, int y, int w, int h, int r, int g, int b, int a, int r1, int g1, int b1 )
{
	static int GradientTexture = 0;
	static std::unique_ptr<Color[]> Gradient = nullptr;

	if( !Gradient )
	{
		Gradient = std::make_unique<Color[]>( w * h );

		for( int i = 0; i < w; i++ ) 
		{
			switch( i % 4 ) 
			{
			case 0:
				for( int k = 0; k < h; k++ ) 
					*( Color* )( Gradient.get() + i + k * w ) = Color( r, g, b );
				break;
			case 1:
				for( int k = 0; k < h; k++ ) 
				{
					switch( k % 4 ) 
					{
					case 1:
						*( Color* )( Gradient.get() + i + k * w ) = Color( r, g, b ); 
						break;
					default:
						*( Color* )( Gradient.get() + i + k * w ) = Color( r1, g1, b1 ); 
						break;
					}
				}
				break;
			case 2:
				for( int k = 0; k < h; k++ ) 
					*( Color* )( Gradient.get() + i + k * w ) = Color( r, g, b );
				break;
			case 3:
				for( int k = 0; k < h; k++ ) 
				{
					switch( k % 4 ) 
					{
					case 3:
						*( Color* )( Gradient.get() + i + k * w ) = Color( r, g, b ); 
						break;
					default:
						*( Color* )( Gradient.get() + i + k * w ) = Color( r1, g1, b1 ); 
						break;
					}
				}
				break;
			}
		}

		GradientTexture = g_pSurface->CreateNewTextureID( true );
		g_pSurface->DrawSetTextureRGBA( GradientTexture, Gradient.get(), w, h );
	}

	g_pSurface->DrawSetColor( 255, 255, 255, a );
	g_pSurface->DrawSetTexture( GradientTexture );
	g_pSurface->DrawTexturedRect( x, y, x + w, y + h );
}

void CDraw::DrawColorPalette( int x, int y, int w, int h )
{
	static int GradientTexture = 0;
	static std::unique_ptr<Color[ ]> Gradient = nullptr;

	if( !Gradient )
	{
		Gradient = std::make_unique<Color[ ]>( w * h );

		for( int i = 0; i < w; i++ )
		{
			int div = w / 6;
			int phase = i / div;
			float t = ( i % div ) / ( float ) div;
			int r, g, b;

			switch( phase )
			{
			case( 0 ):
				r = 255;
				g = 255 * t;
				b = 0;
				break;
			case( 1 ):
				r = 255 * ( 1.f - t );
				g = 255;
				b = 0;
				break;
			case( 2 ):
				r = 0;
				g = 255;
				b = 255 * t;
				break;
			case( 3 ):
				r = 0;
				g = 255 * ( 1.f - t );
				b = 255;
				break;
			case( 4 ):
				r = 255 * t;
				g = 0;
				b = 255;
				break;
			case( 5 ):
				r = 255;
				g = 0;
				b = 255 * ( 1.f - t );
				break;
			}

			for( int k = 0; k < h; k++ )
			{
				float sat = k / ( float ) h;
				int _r = r + sat * ( 255 - r );
				int _g = g + sat * ( 255 - g );
				int _b = b + sat * ( 255 - b );

				*reinterpret_cast< Color* >( Gradient.get() + i + k * w ) = Color( _r, _g, _b );
			}
		}

		GradientTexture = g_pSurface->CreateNewTextureID( true );
		g_pSurface->DrawSetTextureRGBA( GradientTexture, Gradient.get(), w, h );
	}

	g_pSurface->DrawSetColor( 255, 255, 255, 255 );
	g_pSurface->DrawSetTexture( GradientTexture );
	g_pSurface->DrawTexturedRect( x, y, x + w, y + h );
}

void CDraw::DrawHealthBar( int x, int y, int w, int h, int r, int g, int b, int a, int health )
{
	FillRGBA( x, y, w, h, 0, 0, 0, clamp( a, 0, 150 ) );

	FillRGBA( x + 1, y + h - h * clamp( health, 0, 100 ) / 100 + 1, w - 2, h * clamp( health, 0, 100 ) / 100 - 2, r, g, b, a );

	if( health > 0 && health < 100 )
		DrawString( g_Draw.espfont, false, x - w * 2 - 5, y + h - h * health / 100 - 6, 255, 255, 255, a, "%i", health );
}

void CDraw::DrawKevlarBar( int x, int y, int w, int h, int r, int g, int b, int a, float kev )
{
	FillRGBA( x, y, w, h, 0, 0, 0, clamp( a, 0, 150 ) );

	FillRGBA( x + 1, y + 1, w * kev / 100 - 2, h - 2, r, g, b, a );
}

void CDraw::DrawArmorBar( int x, int y, int w, int h, int r, int g, int b, int a, float ammo, int maxammo )
{
	FillRGBA( x, y, w, h, 0, 0, 0, clamp( a, 0, 150 ) );

	if( ammo == 0 )
		FillRGBA( x + 1, y + 1, w - 2, h - 2, 255, 50, 0, a );

	FillRGBA( x + 1, y + 1, w * ammo / maxammo - 2, h - 2, r, g, b, a );
}

void CDraw::DrawLBYBar( int x, int y, int w, int h, int r, int g, int b, int a, float time )
{
	FillRGBA( x, y, w, h, 0, 0, 0, clamp( a, 0, 150 ) );

	if( time > 0.0 )
		FillRGBA( x + 1, y + 1, w * time / 1.1 - 2, h - 2, r, g, b, a );
	else
		FillRGBA( x + 1, y + 1, w - 2, h - 2, 255, 50, 0, a );
}

void CDraw::DrawFeatureBox( int x, int y, int w, int h, int r, int g, int b, int a, int af, int fr, int fg, int fb, int fa, const char* input, unsigned long font, bool bFill )
{
	CHAR szBuffer[ MAX_PATH ];

	if( !input )
		return;

	vsprintf( szBuffer, input, ( char* ) &input + _INTSIZEOF( input ) );

	g_pSurface->DrawSetTextColor( fr, fg, fb, fa );
	g_pSurface->DrawSetTextFont( font );
	g_pSurface->DrawSetTextPos( x + 15, y - 7 );

	std::wstring wide = stringToWide( std::string( szBuffer ) );

	LineRGBA( x, y, x + 11, y, 70, 70, 70, af );
	LineRGBA( x - 1, y - 1, x + 10, y - 1, 30, 30, 30, a );

	LineRGBA( x, y, x, y + h, 70, 70, 70, af );
	LineRGBA( x - 1, y, x - 1, y + h + 1, 30, 30, 30, a );

	LineRGBA( x + 19 + getWidth( font, szBuffer ), y, x + w, y, 70, 70, 70, af );
	LineRGBA( x + 20 + getWidth( font, szBuffer ), y - 1, x + w + 1, y - 1, 30, 30, 30, a );

	LineRGBA( x, y + h, x + w + 1, y + h, 70, 70, 70, af );
	LineRGBA( x, y + h + 1, x + w + 2, y + h + 1, 30, 30, 30, a );

	LineRGBA( x + w, y, x + w, y + h, 70, 70, 70, af );
	LineRGBA( x + w + 1, y - 1, x + w + 1, y + h + 1, 30, 30, 30, a );

	if( bFill )
		FillRGBA( x + 2, y + 2, w - 3, h - 3, r, g, b, a );

	g_pSurface->DrawPrintText( wide.c_str(), wide.length() );
}

int CDraw::getWidth( unsigned long font, const char* input, ... )
{
	INT iWide = 0;
	INT iTall = 0;
	INT iBufSize = MultiByteToWideChar( CP_UTF8, 0x0, input, -1, NULL, 0 );

	wchar_t* pszUnicode = new wchar_t[ iBufSize ];

	MultiByteToWideChar( CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize );

	g_pSurface->GetTextSize( font, pszUnicode, iWide, iTall );

	delete[ ] pszUnicode;

	return iWide;
}

std::wstring CDraw::stringToWide( const std::string& text )
{
	std::wstring wide( text.length(), L' ' );
	std::copy( text.begin(), text.end(), wide.begin() );

	return wide;
}

void CDraw::DrawStringUnicode( unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const wchar_t* text, ... )
{
	if( text == NULL )
		return;

	if( center )
	{
		int iWide = 0;
		int iTall = 0;
		g_pSurface->GetTextSize( font, text, iWide, iTall );
		x -= iWide / 2;
	}

	g_pSurface->DrawSetTextPos( x, y );
	g_pSurface->DrawSetTextFont( font );
	g_pSurface->DrawSetTextColor( r, g, b, a );
	g_pSurface->DrawPrintText( text, wcslen( text ) );
}

void CDraw::DrawString( unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ... )
{
	CHAR szBuffer[ MAX_PATH ];

	if( !input )
		return;

	vsprintf( szBuffer, input, ( char* )&input + _INTSIZEOF( input ) );


	if( center )
		x -= getWidth( Font, szBuffer ) / 2;

	g_pSurface->DrawSetTextColor( r, g, b, a );
	g_pSurface->DrawSetTextFont( Font );
	g_pSurface->DrawSetTextPos( x, y );

	std::wstring wide = stringToWide( std::string( szBuffer ) );
	g_pSurface->DrawPrintText( wide.c_str(), wide.length() );
}

void CDraw::DrawTile( int x, int y, int w, int h, int r, int g, int b, int a, bool back, int rback, int gback, int bback, int aback )
{
	if( back )
		FillRGBA( x, y, w, h, rback, gback, bback, aback );

	int x2 = x - w, y2 = y - h;

	while( x2 <= x )
	{
		FillRGBA( x2 + w, y, 1, h, r, g, b, a );
		x2 += 8;
	}

	while( y2 <= y )
	{
		FillRGBA( x, y2 + h, w, 1, r, g, b, a );
		y2 += 8;
	}
}

void CDraw::DrawHitMarker( int x, int y, int w, int h, int space, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );

	g_pSurface->DrawLine( x + space, y + space, x + w, y + h );
	g_pSurface->DrawLine( x + space, y - space, x + w, y - h );

	g_pSurface->DrawLine( x - space, y + space, x - w, y + h );
	g_pSurface->DrawLine( x - space, y - space, x - w, y - h );
}

void CDraw::DrawHexagon( int x, int y, int size, float angle, int r, int g, int b, int a )
{
	float flAng = angle;
	float flScale = 2.0 * M_PI / 6;

	int x1 = size * cos( flAng ) + x / 2, y1 = size * sin( flAng ) + y / 2;

	for( int i = 0; i < 6; i++ )
	{
		int x2 = x1, y2 = y1;
		flAng += flScale;
		x1 = size * cos( flAng ) + x / 2;
		y1 = size * sin( flAng ) + y / 2;
		LineRGBA( x2, y2, x1, y1, r, g, b, a );
	}
}

void CDraw::NewMenu( int x, int y, int w, int h, int a )
{
	FillRGBA( x, y, w, h, 40, 40, 40, a );

	FillRGBA( x + 1, y + 1, 3, h - 2, 20, 20, 20, a );
	FillRGBA( x + w - 4, y + 1, 3, h - 2, 20, 20, 20, a );
	FillRGBA( x + 1, y + h - 4, w - 2, 3, 20, 20, 20, a );
	FillRGBA( x + 1, y + 1, w - 2, 20, 20, 20, 20, a );

	DrawOutlineRect( x + 3, y + 20, w - 6, h - 23, 70, 70, 70, clamp( a, 0, 150 ) );
	DrawOutlineRect( x, y, w, h, 30, 30, 30, clamp( a, 0, 150 ) );

	VEngineClient::player_info_t pInfo;

	if( g_pEngine->GetPlayerInfo( g_pEngine->GetLocalPlayer(), &pInfo ) )
		DrawString( visualsfont, true, x + w / 2, y + 3, 200, 200, 200, clamp( a, 0, 150 ), "god %s", pInfo.name );
	else
		DrawString( visualsfont, true, x + w / 2, y + 3, 200, 200, 200, clamp( a, 0, 150 ), "menu name" );

	for( int i = 0; i < 20; i++ )
	{
		FillRGBA( x + i + 5, y + i + 21, 120, 5 / 5, 50, 50, 50, a ); //1
		FillRGBA( x + i + 128, y + i + 21, 140, 5 / 5, 50, 50, 50, a ); //2
		FillRGBA( x + i + 271, y + i + 21, 140, 5 / 5, 50, 50, 50, a ); //3
		FillRGBA( x + i + 414, y + i + 21, 115, 5 / 5, 50, 50, 50, a ); //4
	}

	FillRGBA( x + 5, y + 21, 120, 20, 50, 50, 50, a ); //1
	FillRGBA( x + 434, y + 21, 118, 20, 50, 50, 50, a ); //4

//	DrawBackGround( x + 5, y + 41, w - 10, h - 45, 30, 30, 30, a, 50, 50, 50 );

	LineRGBA( x + 5, y + 21, x + 125, y + 21, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 4, y + 21, x + 4, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 5, y + 41, x + 144, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 124, y + 21, x + 144, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );

	LineRGBA( x + 128, y + 21, x + 267, y + 21, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 127, y + 21, x + 147, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 147, y + 41, x + 286, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 267, y + 21, x + 287, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );

	LineRGBA( x + 271, y + 21, x + 410, y + 21, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 270, y + 21, x + 290, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 290, y + 41, x + 429, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 410, y + 21, x + 430, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );

	LineRGBA( x + 414, y + 21, x + 551, y + 21, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 413, y + 21, x + 433, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 433, y + 41, x + 551, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
	LineRGBA( x + 551, y + 21, x + 551, y + 41, 30, 30, 30, clamp( a, 0, 200 ) );
}

void CrossProductSDK( const Vector& a, const Vector& b, Vector& result )
{
	CHECK_VALID( a );
	CHECK_VALID( b );
	Assert( &a != &result );
	Assert( &b != &result );
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}

FORCEINLINE float DotProduct( const Vector& a, const Vector& b )
{
	CHECK_VALID( a );
	CHECK_VALID( b );
	return( a.x*b.x + a.y*b.y + a.z*b.z );
}

void Triangle( const Vector &p1, const Vector &p2, const Vector &p3, int r, int g, int b, int a, bool noDepthTest, float duration )
{
	CBaseEntity *player = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );
	if( !player )
		return;

	// Clip triangles that are far away
	Vector to1 = p1 - player->GetAbsOrigin();
	Vector to2 = p2 - player->GetAbsOrigin();
	Vector to3 = p3 - player->GetAbsOrigin();

	if( ( to1.LengthSqr() > 90000000 ) &&
		( to2.LengthSqr() > 90000000 ) &&
		( to3.LengthSqr() > 90000000 ) )
	{
		return;
	}

	// Clip triangles that are behind the client 
	Vector clientForward = player->GetEyePosition();

	float  dot1 = DotProduct( clientForward, to1 );
	float  dot2 = DotProduct( clientForward, to2 );
	float  dot3 = DotProduct( clientForward, to3 );

	if( dot1 < 0 && dot2 < 0 && dot3 < 0 )
		return;

	g_pDebugOverlay->AddTriangleOverlay( p1, p2, p3, r, g, b, a, noDepthTest, duration );
}

void HorzArrow( const Vector &startPos, const Vector &endPos, float width, int r, int g, int b, int a, bool noDepthTest, float flDuration )
{
	Vector	lineDir = ( endPos - startPos );
	VectorNormalize( lineDir );
	Vector  upVec = Vector( 0, 0, 1 );
	Vector	sideDir;
	float   radius = width / 2.0;

	CrossProductSDK( lineDir, upVec, sideDir );

	Vector p1 = startPos - sideDir * radius;
	Vector p2 = endPos - lineDir * width - sideDir * radius;
	Vector p3 = endPos - lineDir * width - sideDir * width;
	Vector p4 = endPos;
	Vector p5 = endPos - lineDir * width + sideDir * width;
	Vector p6 = endPos - lineDir * width + sideDir * radius;
	Vector p7 = startPos + sideDir * radius;

	// Outline the arrow
	g_pDebugOverlay->AddLineOverlay( p1, p2, r, g, b, noDepthTest, flDuration );
	g_pDebugOverlay->AddLineOverlay( p2, p3, r, g, b, noDepthTest, flDuration );
	g_pDebugOverlay->AddLineOverlay( p3, p4, r, g, b, noDepthTest, flDuration );
	g_pDebugOverlay->AddLineOverlay( p4, p5, r, g, b, noDepthTest, flDuration );
	g_pDebugOverlay->AddLineOverlay( p5, p6, r, g, b, noDepthTest, flDuration );
	g_pDebugOverlay->AddLineOverlay( p6, p7, r, g, b, noDepthTest, flDuration );

	if( a > 0 )
	{
		// Fill us in with triangles
		Triangle( p5, p4, p3, r, g, b, a, noDepthTest, flDuration ); // Tip
		Triangle( p1, p7, p6, r, g, b, a, noDepthTest, flDuration ); // Shaft
		Triangle( p6, p2, p1, r, g, b, a, noDepthTest, flDuration );

		// And backfaces
		Triangle( p3, p4, p5, r, g, b, a, noDepthTest, flDuration ); // Tip
		Triangle( p6, p7, p1, r, g, b, a, noDepthTest, flDuration ); // Shaft
		Triangle( p1, p2, p6, r, g, b, a, noDepthTest, flDuration );
	}
}

Vector UTIL_YawToVector( float yaw )
{
	Vector ret;

	ret.z = 0;
	float angle = DEG2RAD( yaw );
	g_Math.SinCos( angle, &ret.y, &ret.x );

	return ret;
}

void CDraw::YawArrow( const Vector &startPos, float yaw, float length, float width, int r, int g, int b, int a, bool noDepthTest, float flDuration )
{
	Vector forward = UTIL_YawToVector( yaw );

	HorzArrow( startPos, startPos + forward * length, width, r, g, b, a, noDepthTest, flDuration );
}

void CDraw::DirectionTriangle( Vector2D center, float flYaw, float flDistCenter, float flLength, int r, int g, int b, int a )
{
	Vector2D vSlope = { sin( DEG2RAD( -flYaw ) ), -cos( DEG2RAD( flYaw ) ) };
	Vector2D vDirection = vSlope * flLength;
	Vector2D vStart = center + ( vSlope * flDistCenter );
	Vector2D vEnd = vStart + ( vDirection );

	float flLeft = flYaw - 1;
	Vector2D vLSlope = { sin( DEG2RAD( -flLeft ) ), -cos( DEG2RAD( flLeft ) ) };
	Vector2D vLStart = center + ( vLSlope * flDistCenter );

	float flRight = flYaw + 1;
	Vector2D vRSlope = { sin( DEG2RAD( -flRight ) ), -cos( DEG2RAD( flRight ) ) };
	Vector2D vRStart = center + ( vRSlope * flDistCenter );

	g_Draw.DrawFilledTriangle( vLStart, vRStart, vEnd, r, g, b, a );
}

/*std::vector <int> Resolution( std::string szName )
{
	std::ifstream in( szName );
	unsigned int uiWidth, uiHeight;

	in.seekg( 16 );
	in.read( ( char* )&uiWidth, 4 );
	in.read( ( char* )&uiHeight, 4 );

	uiWidth = ntohl( uiWidth );
	uiHeight = ntohl( uiHeight );

	std::vector <int> vReturn;

	vReturn.push_back( uiWidth );
	vReturn.push_back( uiHeight );

	return vReturn;
}

unsigned char* ImageID( const char* cFile, int w, int h )
{
	std::vector <unsigned char> cImage;
	unsigned char* cSize = new unsigned char[ cImage.size() ];

	std::copy( cImage.begin(), cImage.end(), cSize );
	cImage.clear();

	return cSize;
}

void ConvertToChar( std::string& szStr, char cArray[ ], size_t tSize )
{
	strcpy_s( cArray, tSize, szStr.c_str() );
	return;
}

CTexture::CTexture( std::string szName )
{
	std::vector <int> iResolution = Resolution( szName );
	
	iWidth = iResolution[ 0 ];
	iHeight = iResolution[ 1 ];

	char cName[ MAX_PATH ];
	ConvertToChar( szName, cName, MAX_PATH );

	cImageID = ImageID( cName, iWidth, iHeight );
	iLast = 0;
}

void CTexture::Draw( int x, int y, int x1, int y1 )
{
	bool bValid = false;
	unsigned int iTexture;

	if( iLast == 0 || g_pSurface->IsTextureIDValid( iLast ) )
	{
		iTexture = g_pSurface->CreateNewTextureID( true );

		if( iTexture )
			return;

		g_pSurface->DrawSetTextureRGBA( iTexture, cImageID, iWidth, iHeight );

		if( g_pSurface->IsTextureIDValid( iTexture ) )
			bValid = true;

		iLast = iTexture;
	}
	else
		iTexture = iLast;

	g_pSurface->DrawSetColor( 255, 255, 255, 255 );

	g_pSurface->DrawSetTexture( iTexture );

	g_pSurface->DrawTexturedRect( x, y, x1, y1 );
}*/

#pragma once

#include "main.h"

class CDraw
{
public:
	void InitFonts();

	void DrawOutlineRect( int x, int y, int w, int h, int r, int g, int b, int a );
	void FillRGBA( int x, int y, int w, int h, int r, int g, int b, int a );
	void LineRGBA( int x1, int y1, int x2, int y2, int r, int g, int b, int a );
	void DrawRect( int x1, int y1, int x2, int y2, int r, int g, int b, int a );
	void DrawCircle( int x, int y, int radius, int segments, int r, int g, int b, int a );
	void Draw3DCube( Vector * points, int r, int g, int b, int a );
	void Draw3DCube( float scalar, Vector angles, Vector middle_origin, int r, int g, int b, int a );
	void DrawHealthBar( int x, int y, int w, int h, int r, int g, int b, int a, int health );
	void DrawKevlarBar( int x, int y, int w, int h, int r, int g, int b, int a, float kev );
	void DrawArmorBar( int x, int y, int w, int h, int r, int g, int b, int a, float ammo, int maxammo );
	void DrawLBYBar( int x, int y, int w, int h, int r, int g, int b, int a, float time );
	void DrawFilledTriangle( Vector2D one, Vector2D two, Vector2D three, int r, int g, int b, int a );
	void DrawFilledArrow( Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, int red, int green, int blue, int alpha );
	void DrawFilledCircle( Vector2D center, int r, int g, int b, int a, float radius, float points );
	void DrawGradient( int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2 );
	void DrawGradientH( int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2 );
	void DrawBackGround( int x, int y, int w, int h, int r, int g, int b, int a, int r1, int g1, int b1 );
	void DrawColorPalette( int x, int y, int w, int h );
	void DrawFeatureBox( int x, int y, int w, int h, int r, int g, int b, int a, int af, int fr, int fg, int fb, int fa, const char* input, unsigned long font, bool bFill );

	void CDraw::DrawStringUnicode( unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const wchar_t* text, ... );
	void DrawString( unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ... );
	int getWidth( unsigned long font, const char* input, ... );
	std::wstring stringToWide( const std::string& text );

	void DrawTile( int x, int y, int w, int h, int r, int g, int b, int a, bool back = false, int rback = 0, int gback = 0, int bback = 0, int aback = 255 );

	void DrawHitMarker( int x, int y, int w, int h, int space, int r, int g, int b, int a );

	void DrawHexagon( int x, int y, int size, float angle, int r, int g, int b, int a );

	void NewMenu( int x, int y, int w, int h, int a );

	void YawArrow( const Vector &startPos, float yaw, float length, float width, int r, int g, int b, int a, bool noDepthTest, float flDuration );

	void DirectionTriangle( Vector2D center, float flYaw, float flDistCenter, float flLength, int r, int g, int b, int a );

	unsigned long visualsfont, keyfont, tabfont, iconfont, espfont, iconmenu, indicatorfont;
	unsigned long ulCreateFont;
};

extern CDraw g_Draw;
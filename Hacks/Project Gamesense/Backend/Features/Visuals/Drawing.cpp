#include "Drawing.h"
#include "../../Configuration/Config.h"
using namespace Graphics;
using namespace Interfaces;

CDrawing* Graphics::Drawing = new CDrawing();
bool CDrawing::m_init = false;
void CDrawing::Initialize()
{
	static int oldw = 0;
	static int oldh = 0;

	int w = 0;
	int h = 0;
	Interfaces::Engine->GetScreenSize(w, h);

	if ( !m_init || oldw != w || oldh != h )
	{
		fonts.esp_main = Surface->SCreateFont();
		fonts.esp_name = Surface->SCreateFont();
		fonts.esp_indicator = Surface->SCreateFont();
		fonts.esp_side = Surface->SCreateFont();
		fonts.esp_icons = Surface->SCreateFont();
		fonts.eventlogs = Surface->SCreateFont();
		fonts.weapon_icons = Surface->SCreateFont();
		Surface->SetFontGlyphSet(fonts.esp_main, "Verdana", 12, 0, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		Surface->SetFontGlyphSet(fonts.esp_name, "Verdana", 12, 600, 0, 0, FONTFLAG_DROPSHADOW);
		Surface->SetFontGlyphSet(fonts.esp_indicator, "Calibri", 29, 611, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		Surface->SetFontGlyphSet(fonts.esp_side, "Small Fonts", 8, 500, 0, 0, FONTFLAG_OUTLINE);
		Surface->SetFontGlyphSet(fonts.esp_icons, "Small Fonts", 8, 500, 0, 0, FONTFLAG_OUTLINE);
		Surface->SetFontGlyphSet(fonts.eventlogs, "Lucida console", 10, 400, 0, 0, FONTFLAG_DROPSHADOW);
		Surface->SetFontGlyphSet(fonts.weapon_icons, "undefeated", 16, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

		m_init = true;
		oldw = w;
		oldh = h;
	}
}

void CDrawing::Rect( int x, int y, int w, int h, CColor color )
{
	Surface->DrawSetColor( color );
	Surface->DrawFilledRect( x, y, x + w, y + h );
}

void CDrawing::RectP( int x, int y, int w, int h, CColor color )
{
	Surface->DrawSetColor( color );
	Surface->DrawFilledRect( x, y, w, h );
}

void CDrawing::RectOutlined( int x, int y, int w, int h, CColor color, CColor outline )
{
	Rect( x, y, w, h, color );
	Outline( x, y, w, h, outline );
}

void CDrawing::Outline( int x, int y, int w, int h, CColor color )
{
	Surface->DrawSetColor( color );
	Surface->DrawOutlinedRect( x, y, x + w, y + h );
}

void CDrawing::Line( int x, int y, int x2, int y2, CColor color )
{
	Surface->DrawSetColor( color );
	Surface->DrawLine( x, y, x2, y2 );
}
void CDrawing::rect_filled(int x, int y, int w, int h, CColor color) {
	//color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	Surface->DrawSetColor(color);
	Surface->DrawFilledRect(x, y, x + w, y + h);
}

void CDrawing::PolyLine( int* x, int* y, int count, CColor color )
{
	Surface->DrawSetColor( color );
	Surface->DrawPolyLine( x, y, count );
}

void CDrawing::Polygon( int count, Vertex_t* vertexs, CColor color )
{
	static auto texture = Surface->CreateNewTextureID( true );
	Surface->DrawSetColor( color );
	Surface->DrawSetTexture( texture );
	Surface->DrawTexturedPolygon( count, vertexs );
}

void CDrawing::PolygonOutlined( int count, Vertex_t* vertexs, CColor color, CColor outline )
{
	static int x[128];
	static int y[128];
	Polygon( count, vertexs, color );

	for ( int i = 0; i < count; i++ )
	{
		x[i] = vertexs[i].m_Position.x;
		y[i] = vertexs[i].m_Position.y;
	}

	PolyLine( x, y, count, outline );
	delete[] x;
	delete[] y;
}

void CDrawing::GradientVertical( int x, int y, int w, int h, CColor c1, CColor c2 )
{
	Surface->SetDrawColor( c1 );
	Surface->DrawFilledRectFade( x, y, x + w, y + h, c1.a(), 0, false );
	Surface->SetDrawColor( c2 );
	Surface->DrawFilledRectFade( x, y, x + w, y + h, 0, c2.a(), false );
}

void CDrawing::DrawCircle( int x, int y, int radius, CColor c )
{
	Surface->SetDrawColor( c );
	Surface->DrawOutlinedCircle( x, y, radius, 2000 );
}

void CDrawing::GradientHorizontal( int x, int y, int w, int h, CColor c1, CColor c2 )
{
	Surface->SetDrawColor( c1 );
	Surface->DrawFilledRectFade( x, y, x + w, y + h, c1.a(), 0, true );
	Surface->SetDrawColor( c2 );
	Surface->DrawFilledRectFade( x, y, x + w, y + h, 0, c2.a(), true );
}

void CDrawing::Text(int x, int y, const char* text, int font, CColor color) {
	if (!text)
		return;

	size_t Size = strlen(text) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	//mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, text, -1, WideBuffer, Size);

	Surface->DrawSetTextColor(color);
	Surface->DrawSetTextFont(font);
	Surface->DrawSetTextPos(x, y);
	Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));

	delete[] WideBuffer;
}

RECT CDrawing::TextSize( const char* text, int font )
{
	int apple = 0;
	char Buffer[2048] = { '\0' };

	size_t Size = strlen(text) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	//mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, text, -1, WideBuffer, Size);
	int Width = 0, Height = 0;
	Surface->GetTextSize( font, WideBuffer, Width, Height );
	RECT outcome = { 0, 0, Width, Height };
	delete[] WideBuffer;
	return outcome;
}

void CDrawing::Text(int x, int y, const wchar_t* text, int font, CColor color) {
	if (!text)
		return;

	Surface->DrawSetTextColor(color);
	Surface->DrawSetTextFont(font);
	Surface->DrawSetTextPos(x, y);
	Surface->DrawPrintText(text, wcslen(text));
}

RECT CDrawing::TextSize(const wchar_t* text, int font) {
	int w = 0, h = 0;
	Surface->GetTextSize(font, text, w, h);

	return { 0, 0, w, h };
}

void CDrawing::Draw3DCube( float scalar, Vector angles, Vector middle_origin, CColor outline )
{
	Vector forward, right, up;
	Cheat::Utilities->Math_AngleVectors( angles, forward, right, up );
	Vector points[8];
	points[0] = middle_origin - ( right * scalar ) + ( up * scalar ) - ( forward * scalar );
	points[1] = middle_origin + ( right * scalar ) + ( up * scalar ) - ( forward * scalar );
	points[2] = middle_origin - ( right * scalar ) - ( up * scalar ) - ( forward * scalar );
	points[3] = middle_origin + ( right * scalar ) - ( up * scalar ) - ( forward * scalar );
	points[4] = middle_origin - ( right * scalar ) + ( up * scalar ) + ( forward * scalar );
	points[5] = middle_origin + ( right * scalar ) + ( up * scalar ) + ( forward * scalar );
	points[6] = middle_origin - ( right * scalar ) - ( up * scalar ) + ( forward * scalar );
	points[7] = middle_origin + ( right * scalar ) - ( up * scalar ) + ( forward * scalar );
	Vector points_screen[8];

	for ( int i = 0; i < 8; i++ )
		if ( !Cheat::Utilities->Game_WorldToScreen( points[i], points_screen[i] ) )
			return;

	Interfaces::Surface->DrawSetColor( outline );
	Interfaces::Surface->DrawLine( points_screen[0].x, points_screen[0].y, points_screen[1].x, points_screen[1].y );
	Interfaces::Surface->DrawLine( points_screen[0].x, points_screen[0].y, points_screen[2].x, points_screen[2].y );
	Interfaces::Surface->DrawLine( points_screen[3].x, points_screen[3].y, points_screen[1].x, points_screen[1].y );
	Interfaces::Surface->DrawLine( points_screen[3].x, points_screen[3].y, points_screen[2].x, points_screen[2].y );
	Interfaces::Surface->DrawLine( points_screen[0].x, points_screen[0].y, points_screen[4].x, points_screen[4].y );
	Interfaces::Surface->DrawLine( points_screen[1].x, points_screen[1].y, points_screen[5].x, points_screen[5].y );
	Interfaces::Surface->DrawLine( points_screen[2].x, points_screen[2].y, points_screen[6].x, points_screen[6].y );
	Interfaces::Surface->DrawLine( points_screen[3].x, points_screen[3].y, points_screen[7].x, points_screen[7].y );
	Interfaces::Surface->DrawLine( points_screen[4].x, points_screen[4].y, points_screen[5].x, points_screen[5].y );
	Interfaces::Surface->DrawLine( points_screen[4].x, points_screen[4].y, points_screen[6].x, points_screen[6].y );
	Interfaces::Surface->DrawLine( points_screen[7].x, points_screen[7].y, points_screen[5].x, points_screen[5].y );
	Interfaces::Surface->DrawLine( points_screen[7].x, points_screen[7].y, points_screen[6].x, points_screen[6].y );
}
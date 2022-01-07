#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


namespace Render
{
	void Initialise();

	void Clear(int x, int y, int w, int h, Color color);
//	void Outline(int x, int y, int w, int h, Color color);
	void Outline(float x, float y, float w, float h, Color color);
	void OutlinedRect(int x, int y, int w, int h, Color color_out, Color color_in);
	void drawRECT(int x1, int y1, int x2, int y2, Color clr);
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float & flRainbow,float alpha);
	void Line(int x, int y, int x2, int y2, Color color);
	void Line_3(Vector2D start_pos, Vector2D end_pos, Color col);
	void PolyLine(int *x, int *y, int count, Color color);
	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void DrawRect(int x, int y, int w, int h, Color col);
	void DrawFilledRect(int x1, int y1, int x2, int y2, Color color);
	void DrawEmptyRect(int x1, int y1, int x2, int y2, Color color, unsigned char ignore_flags);
	void rect(int x, int y, int w, int h, Color color);
	void outlineyeti(int x, int y, int w, int h, Color color);
	void gradient_verticle(int x, int y, int w, int h, Color c1, Color c2);
	void nonamegey(int x, int y, const char * _Input, int font, Color color);
	void textT(int x, int y, int font, Color color, const char * _Input, ...);
	void text_yeti(int x, int y, const char * _Input, int font, Color color);
	void color_spectrum(int x, int y, int w, int h);
	void outlined_rectyeti(int x, int y, int w, int h, Color color_out, Color color_in);
	Color color_spectrum_pen(int x, int y, int w, int h, Vector stx);
	void gradient_horizontal(int x, int y, int w, int h, Color c1, Color c2);
	void GradientB(int x, int y, int w, int h, Color color1, Color color2, int variation);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);

	void Color_spectrum(int x, int y, int w, int h);

	Color Color_spectrum_pen(int x, int y, int w, int h, Vector stx);

	void DrawTexturedPoly(int n, Vertex_t * vertice, Color col);

	void DrawFilledCircle(Vector2D center, Color color, float radius, float points);

	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void DrawCircle(float x, float y, float r, float segments, Color color);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);
	void GradientSideways(int x, int y, int w, int h, Color color1, Color color2, int variation);

	std::uint8_t * load_image(unsigned short id, int width, int height, const std::string & type);

	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD MenuBold;
		extern DWORD esp;
		extern DWORD MenuText;
		extern DWORD Icon;
		extern DWORD MenuTabs;
		extern DWORD smallassfont;
		extern DWORD Text;
		extern DWORD nameaiz;
		extern DWORD Slider;
		extern DWORD Clock;
		extern DWORD LBY;
		extern DWORD xd;
		extern DWORD nnbruda;
		extern DWORD Tabs;
		extern DWORD niggerbomb;
		extern DWORD BOMB;
		extern DWORD CheckBox;
		extern DWORD IconESP;
		extern DWORD slickESP;
		extern DWORD MenuSymbols;
		extern DWORD LBYIndicator;
		extern DWORD WeaponIcon;
		extern DWORD gay;
		extern DWORD GroupBox;
		extern DWORD NameESP;
	};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Text2(int x, int y, const char * _Input, int font, Color color);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	void TEXTUNICODE(int x, int y, const char * _Input, int font, Color color);
	RECT GetTextSize(DWORD font, const char* text);

	RECT GetTextSize2(const char * _Input, int font);

	bool TransformScreen(const Vector & in, Vector & out);

	bool WorldToScreen(const Vector& in, Vector& out);
	
	RECT GetViewport();
};


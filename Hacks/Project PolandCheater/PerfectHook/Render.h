
#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


class Render
{
public:
	void SetupFonts();

	// Normal Drawing functions
	void Clear(int x, int y, int w, int h, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void Outline(int x, int y, int w, int h, Color color);
	void DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...);
	void OutlineRainbow(int x, int y, int w, int h, float flSpeed, float &flRainbow);
	void Line(int x, int y, int x2, int y2, Color color);
	void PolyLine(int *x, int *y, int count, Color color);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
	void OutlineCircle(int x, int y, int r, int seg, Color color);
	void Pixel(int x, int y, Color col);

	// Gradient Functions
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	// Text functions


	void Text(int x, int y, Color color, DWORD font, const char* text, ...);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	void Text(int x, int y, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);

	// Other rendering functions
	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();
    class Fonts
    {
    public:
        DWORD Default;
        DWORD ESP;
        DWORD Guns;
        DWORD Defuse;
        DWORD Time;
        DWORD DroppedGuns;
    } font;


};

extern Render* g_Render;
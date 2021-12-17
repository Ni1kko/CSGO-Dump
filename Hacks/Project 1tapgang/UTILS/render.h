#pragma once
#include "../SDK/ISurface.h"

namespace SDK
{
	class IMaterial;
}
namespace RENDER
{
	void old_text(int x, int y, const wchar_t* _Input, int font, CColor color);
	unsigned int CreateF(std::string font_name, int size, int weight, int blur, int scanlines, int flags);
	void Clear(int x, int y, int w, int h, CColor color);
	RECT get_text_size(DWORD font, const char * text);
	void Outline(int x, int y, int w, int h, CColor color);
	void Color_spectrum(int x, int y, int w, int h);
	void OutlinedRect(int x, int y, int w, int h, CColor color_out, CColor color_in);
	void rect(int x, int y, int w, int h, CColor color);
	void DrawOutlinedRect(int x, int y, int w, int h, CColor col);
	void Text2(int x, int y, const char * _Input, int font, CColor color);
	CColor Color_spectrum_pen(int x, int y, int w, int h, Vector stx);
	void GradientV(int x, int y, int w, int h, CColor c1, CColor c2);
	void GradientH(int x, int y, int w, int h, CColor c1, CColor c2);
	RECT GetViewport();
	void Text(int x, int y, CColor color, DWORD font, const char * text, ...);
	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, CColor Color, std::string Input);
	void DrawWF(int X, int Y, unsigned int Font, CColor Color, const wchar_t* Input);
	Vector2D GetTextSize(unsigned int Font, std::string Input);

	void DrawLine(int x1, int y1, int x2, int y2, CColor color);

	void DrawEmptyRect(int x1, int y1, int x2, int y2, CColor color, unsigned char = 0); // the flags are for which sides to ignore in clockwise, 0b1 is top, 0b10 is right, etc.
	void DrawFilledRect(int x1, int y1, int x2, int y2, CColor color);
	void FillRectangle(int x1, int y2, int width, int height, CColor color);
	void DrawFilledRectOutline(int x1, int y1, int x2, int y2, CColor color);
	void DrawFilledRectArray(SDK::IntRect* rects, int rect_amount, CColor color);
	void DrawCornerRect(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const CColor& color, const CColor& outlined_color);
	void DrawEdges(float topX, float topY, float bottomX, float bottomY, float length, CColor color);

	void DrawCircle(int x, int y, int radius, int segments, CColor color);
	void DrawTexturedPoly(int n, SDK::Vertex_t * vertice, CColor col);
	void DrawFilledCircle(int x, int y, int radius, int segments, CColor color);
	//void DrawFilledCircle(Vector2D center, CColor color, CColor outline, float radius, float points);

	void TexturedPolygon(int n, std::vector<SDK::Vertex_t> vertice, CColor color);
	void DrawFilledCircle(Vector2D center, CColor color, float radius, float points);
	void DrawSomething();
	void DrawFilled3DBox(Vector origin, int width, int height, CColor outline, CColor filling);
	bool WorldToScreen(Vector world, Vector &screen);
}
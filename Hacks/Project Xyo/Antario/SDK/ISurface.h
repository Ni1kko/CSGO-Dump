#pragma once
#include "..\Utils\Color.h"
#include "..\Utils\Utils.h"
#include "..\SDK\vector2d.h"
#include <array>
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
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};


struct Vertex_t
{
	Vector2D	mPosition;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
};
struct vertex_t {
	vertex_t() { }
	vertex_t(const vec2_t &pos, const vec2_t &coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}
	void Init(const vec2_t &pos, const vec2_t &coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}

	vec2_t	position;
	vec2_t	m_TexCoord;
};


typedef  Vertex_t FontVertex_t;

class ISurface
{
public:
	void UnLockCursor()
	{
		typedef void(__thiscall* OriginalFn)(void*);
		Utils::GetVFunc< OriginalFn >(this, 66)(this);
	}

	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* oDrawSetColor)(PVOID, Color);
		return Utils::GetVFunc< oDrawSetColor >(this, 14)(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 19)(this, x0, y0, x1, y1);
	}
	void DrawPolyLine(int* x, int* y, int count)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, int*, int*, int);
		Utils::GetVFunc< OriginalFn >(this, 19 + 1)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		Utils::GetVFunc< OriginalFn >(this, 23)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return Utils::GetVFunc< oDrawSetTextColor >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		Utils::GetVFunc< OriginalFn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		Utils::GetVFunc< OriginalFn >(this, 28)(this, text, textLen, 0);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
		return Utils::GetVFunc< oDrawSetTextColor >(this, 38)(this, textureID);
	}

	inline void DrawTexturedRect(int x, int y, int w, int h) {
		typedef void(__thiscall *OrigFn)(void *, int, int, int, int);
		Utils::GetVFunc<OrigFn>(this, 41)(this, x, y, w, h);
	}

	bool IsTextureIDValid(int TextureID)
	{
		typedef bool(__thiscall* OriginalFn)(void*, int);
		return Utils::GetVFunc<OriginalFn>(this, 42)(this, TextureID);
	}

	void GetClipRect(int& x, int& y, int& x2, int& y2) {
		typedef void(__thiscall * OrigFn)(void*, int&, int&, int&, int&);
		Utils::GetVFunc<OrigFn>(this, 146)(this, x, y, x2, y2);

	}
	void clip(const vec2_t& pos, const vec2_t& size, bool override)
	{
		RECT rec;
		rec.left = static_cast<LONG>(pos.x);
		rec.top = static_cast<LONG>(pos.y);
		rec.right = static_cast<LONG>(pos.x + size.x + 1);
		rec.bottom = static_cast<LONG>(pos.y + size.y + 1);

		SetClipRect(rec.left, rec.top, rec.right, rec.bottom);
	}
	void SetClippingEnabled(bool value)
	{
		*reinterpret_cast<bool*>(uintptr_t(this) + 0x280) = value;
	}

	void SetClipRect(int x, int y, int x2, int y2) {
		typedef void(__thiscall * OrigFn)(void*, int, int, int, int);
		Utils::GetVFunc<OrigFn>(this, 147)(this, x, y, x2, y2);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char  const* colors, int w, int h)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, unsigned char  const*, int, int);
		return Utils::GetVFunc< oDrawSetTextColor >(this, 37)(this, textureID, colors, w, h);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
		return Utils::GetVFunc< oDrawSetTextColor >(this, 43)(this, procedural);
	}

	int create_new_texture_id(bool procedural = false) {
		using original_fn = int(__thiscall*)(ISurface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}

	void set_drawing_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}
	void set_texture(int id) {
		using original_fn = void(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[38](this, id);
	}
	void draw_polygon(int n, vertex_t *vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(ISurface*, int, vertex_t*, bool);
		return (*(original_fn**)this)[106](this, n, vertice, clip_vertices);
	}
	void add_textured_polygon(int n, vertex_t* vertice, int r, int g, int b, int a) {
		static int texture_id = create_new_texture_id(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		set_drawing_color(r, g, b, a);
		set_texture(texture_id);
		draw_polygon(n, vertice);
	}


	void PolygonOutline(int count, std::array< vec2_t, 3 >points, Color colorLine)
	{
		static int x[128];
		static int y[128];
		std::array< vertex_t, 3 >vertices{ vertex_t(points.at(0)), vertex_t(points.at(1)), vertex_t(points.at(2)) };

		for (int i = 0; i < count; i++)
		{
			x[i] = vertices[i].position.x;
			y[i] = vertices[i].position.y;
		}

		PolyLine(x, y, count, colorLine);
	}
	void PolyLine(int* x, int* y, int count, Color color)
	{
		DrawSetColor(color);
		DrawPolyLine(x, y, count);
	}
	void draw_filled_triangle(std::array< vec2_t, 3 >points, Color colour) {
		std::array< vertex_t, 3 >vertices{ vertex_t(points.at(0)), vertex_t(points.at(1)), vertex_t(points.at(2)) };
		add_textured_polygon(3, vertices.data(), colour.red, colour.green, colour.blue, colour.alpha);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, FontVertex_t*, bool);
		return Utils::GetVFunc< oDrawSetTextColor >(this, 106)(this, vtxCount, vtx, bClipVertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return Utils::GetVFunc< OriginalFn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		Utils::GetVFunc< OriginalFn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void PlaySoundA(const char *filename)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const char*);
		Utils::GetVFunc< OriginalFn >(this, 82)(this, filename);
	}

	void ISurface::GetTextSize(DWORD font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, DWORD, const wchar_t*, int&, int&);
		return Utils::GetVFunc<OriginalFn>(this, 79)(this, font, text, wide, tall);
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
		return Utils::GetVFunc< oDrawOutlinedCircle >(this, 103)(this, x, y, r, seg);
	}

	void OutlineCircle(int x, int y, int r, int seg, Color color)
	{
		DrawSetColor(color);
		DrawOutlinedCircle(x, y, r, seg);
	}

	void SurfaceGetCursorPos(int &x, int &y)
	{
		typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
		return Utils::GetVFunc< oSurfaceGetCursorPos >(this, 66)(this, x, y);
	}

	void FilledRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawFilledRect(x, y, x + w, y + h);
	}

	void RoundedFilledRect(int x, int y, int width, int height, float radius, Color col) { // UC https://www.unknowncheats.me/forum/1498179-post4.html
	// TODO: make the quality not hardcoded -green
	// don't you have to give it quality in the formula you wrote? 8 + 4 * ( quality ) ? -dex
		constexpr int quality = 24;

		static Vertex_t verts[quality];

		Vector2D add = { 0, 0 };

		for (int i = 0; i < quality; i++) {
			float angle = (static_cast <float> (i) / -quality) * 6.28f - (6.28f / 16.f);

			verts[i].mPosition.x = radius + x + add.x + (radius * sin(angle));
			verts[i].mPosition.y = height - radius + y + add.y + (radius * cos(angle));

			if (i == 4) {
				add.y = -height + (radius * 2);
			}
			else if (i == 10) {
				add.x = width - (radius * 2);
			}
			else if (i == 16) {
				add.y = 0;
			}
			else if (i == 22) {
				add.x = 0;
			}
		}
		DrawSetColor(col.red, col.green, col.blue, col.alpha);
		DrawTexturedPolygon(quality, verts);
	}

	void FilledRectManual(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawFilledRect(x, y, w, h);
	}

	void OutlinedRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawOutlinedRect(x, y, x + w, y + h);
	}

	void OutlinedVecRect(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawLine(x, y, x2, y);
		DrawLine(x2, y, x2, y2);
		DrawLine(x2, y2, x, y2);
		DrawLine(x, y2, x, y);
	}

	void Line(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color.red, color.green, color.blue, color.alpha);
		DrawLine(x, y, x2, y2);
	}

	void DrawTexturedPoly(int n, Vertex_t* vertice, Color col) {
		static int texture_id = CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		DrawSetTextureRGBA(texture_id, buf, 1, 1);
		DrawSetColor(col);
		DrawSetTexture(texture_id);
		DrawTexturedPolygon(n, vertice);
	}

	void DrawFilledCircle(Vector2D center, Color color, float radius, float points) {
		std::vector<Vertex_t> vertices;
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

		DrawTexturedPoly((int)points, vertices.data(), color);
	}

	void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		/* set up buffer */
		char Buffer[2048] = { '\0' };

		/* set up varargs*/
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;

		/* set up widebuffer*/
		wchar_t* WideBuffer = new wchar_t[Size];

		/* char -> wchar */
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		/* check center */
		int Width = 0, Height = 0;

		if (Center)
		{
			GetTextSize(Font, WideBuffer, Width, Height);
		}

		/* call and draw*/
		DrawSetTextColor(Color.red, Color.green, Color.blue, Color.alpha);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));

		return;
	}

	RECT GetTextSizeRect(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	Vector2D GetMousePosition()
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool MouseInRegion(int x, int y, int x2, int y2) {
		if (GetMousePosition().x > x && GetMousePosition().y > y && GetMousePosition().x < x2 + x && GetMousePosition().y < y2 + y)
			return true;
		return false;
	}
};

extern ISurface* g_pSurface;
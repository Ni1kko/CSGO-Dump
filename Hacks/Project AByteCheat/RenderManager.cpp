#pragma once

#include "RenderManager.h"

#define _CRT_SECURE_NO_WARNINGS

#define M_PI 3.14159265358979323846

namespace Render
{
	namespace Fonts
	{
		DWORD Default;
		DWORD Menu;
		DWORD MenuBold;
		DWORD esp;
		DWORD MenuText;
		DWORD Icon;
		DWORD MenuTabs;
		DWORD Text;
		DWORD IconESP;
		DWORD Slider;
		DWORD smallassfont;
		DWORD Clock;
		DWORD slickESP;
		DWORD LBY;
		DWORD Tabs;
		DWORD CheckBox;
		DWORD BOMB;
		DWORD MenuSymbols;
		DWORD nameaiz;
		DWORD LBYIndicator;
		DWORD WeaponIcon;
		DWORD nnbruda;
		DWORD niggerbomb;
		DWORD xd;
		DWORD gay;
		DWORD GroupBox;
		DWORD NameESP;
	};
};


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

void Render::Initialise()
{
	Fonts::Default = 0x1D;
	Fonts::Menu = interfaces::surface->FontCreate();
	Fonts::MenuBold = interfaces::surface->FontCreate();
	Fonts::esp = interfaces::surface->FontCreate();
	Fonts::MenuText = interfaces::surface->FontCreate();
	Fonts::Icon = interfaces::surface->FontCreate();
	Fonts::MenuTabs = interfaces::surface->FontCreate();
	Fonts::Slider = interfaces::surface->FontCreate();
	Fonts::Clock = interfaces::surface->FontCreate();
	Fonts::nameaiz = interfaces::surface->FontCreate();
	Fonts::BOMB = interfaces::surface->FontCreate();
	Fonts::nnbruda = interfaces::surface->FontCreate();
	Fonts::LBY = interfaces::surface->FontCreate();
	Fonts::Tabs = interfaces::surface->FontCreate();
	Fonts::CheckBox = interfaces::surface->FontCreate();
	Fonts::MenuSymbols = interfaces::surface->FontCreate();
	Fonts::LBYIndicator = interfaces::surface->FontCreate();
	Fonts::niggerbomb = interfaces::surface->FontCreate();
	Fonts::WeaponIcon = interfaces::surface->FontCreate();
	Fonts::smallassfont = interfaces::surface->FontCreate();
	Fonts::xd = interfaces::surface->FontCreate();
	Fonts::slickESP = interfaces::surface->FontCreate();
	Fonts::IconESP = interfaces::surface->FontCreate();
	Fonts::gay = interfaces::surface->FontCreate();
	Fonts::GroupBox = interfaces::surface->FontCreate();//NameESP
	Fonts::NameESP = interfaces::surface->FontCreate();//NameESP


	interfaces::surface->SetFontGlyphSet(Fonts::nameaiz, "Tahoma", 12, 300, 0, 0, FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::BOMB, "Verdana", 45, 1000, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::Menu, "Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::Text, "DINPro-Regular", 30, 500, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::MenuBold, "Tahoma", 12, 700, 0, 0, FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::esp, "Smallest Pixel-7", 10, 100, 0, 0, FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::smallassfont, "Smallest Pixel-7", 8, 100, 0, 0, FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::slickESP, "Smallest Pixel-7", 13, 700, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::MenuText, "Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::MenuTabs, "MyScriptFont", 18, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::Slider, "Smallest Pixel-7", 17, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::Clock, "Arial", 22, 575, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::Tabs, "menu_font", 50, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::IconESP, "icomoon", 14, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::nnbruda, "Verdana", 12, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::LBY, "Verdana", 36, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::niggerbomb, "Verdana", 28, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::CheckBox, "eagle", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::Icon, "undefeated", 10, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::MenuSymbols, "Tahoma", 16, 500, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::LBYIndicator, "Smallest Pixel-7", 13, 700, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::WeaponIcon, "AstriumWep", 12, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::gay, "Smallest Pixel-7", 11, 250, 0, 0, FONTFLAG_ANTIALIAS);
	interfaces::surface->SetFontGlyphSet(Fonts::xd, "Smallest Pixel-7", 11, 120, 0, 0, FONTFLAG_OUTLINE);
	interfaces::surface->SetFontGlyphSet(Fonts::GroupBox, "Verdana", 12, FW_EXTRABOLD, 0, 0, FONTFLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(Fonts::NameESP, "Verdana", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);

	Utilities::Log("Render System Ready");
}

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	interfaces::engine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::Outline(float x, float y, float w, float h, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}
void Render::OutlinedRect(int x, int y, int w, int h, Color color_out, Color color_in)
{
	interfaces::surface->DrawSetColor(color_in);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);

	interfaces::surface->DrawSetColor(color_out);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}
void Render::drawRECT(int x1, int y1, int x2, int y2, Color clr)
{
	interfaces::surface->DrawSetColor(clr);
	interfaces::surface->DrawFilledRect(x1, y1, x2, y2);
}

void Render::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow,float alpha)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		colRainbow.SetAlpha(alpha);
		Render::DrawRect(x + i, y, 1, height, colRainbow);
	}
}
void Render::Line(int x, int y, int x2, int y2, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawLine(x, y, x2, y2);
}

void Render::Line_3(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Render::Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawPolyLine(x, y, count);
}

void Render::TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color)
{
	static int texture_id = interfaces::surface->CreateNewTextureID(true); // 
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	interfaces::surface->DrawSetTextureRGBA(texture_id, buf, 1, 1); //
	interfaces::surface->DrawSetColor(color); //
	interfaces::surface->DrawSetTexture(texture_id); //
	interfaces::surface->DrawTexturedPolygon(n, vertice.data()); //
}


void Render::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	interfaces::surface->DrawSetColor(col);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	interfaces::surface->DrawSetColor(col);
	interfaces::surface->DrawLine(x0, y0, x1, y1);
}
void Render::DrawRect(int x, int y, int w, int h, Color col)
{
	interfaces::surface->DrawSetColor(col);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::DrawFilledRect(int x1, int y1, int x2, int y2, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawFilledRect(x1, y1, x2, y2);
}

void Render::DrawEmptyRect(int x1, int y1, int x2, int y2, Color color, unsigned char ignore_flags)
{
	interfaces::surface->DrawSetColor(color);

	if (!(ignore_flags & 0b1))
		interfaces::surface->DrawLine(x1, y1, x2, y1);

	if (!(ignore_flags & 0b10))
		interfaces::surface->DrawLine(x2, y1, x2, y2);

	if (!(ignore_flags & 0b100))
		interfaces::surface->DrawLine(x2, y2, x1, y2);

	if (!(ignore_flags & 0b1000))
		interfaces::surface->DrawLine(x1, y2, x1, y1);
}



void Render::rect(int x, int y, int w, int h, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);
}
void Render::outlineyeti(int x, int y, int w, int h, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::gradient_verticle(int x, int y, int w, int h, Color c1, Color c2)
{
	Render::rect(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		Render::rect(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::nonamegey(int x, int y, const char* _Input, int font, Color color)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}

void Render::textT(int x, int y, int font, Color color, const char* _Input, ...)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}

void Render::text_yeti(int x, int y, const char* _Input, int font, Color color)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}

void Render::color_spectrum(int x, int y, int w, int h)
{
	static int GradientTexture = 0;
	static std::unique_ptr<Color[]> Gradient = nullptr;
	if (!Gradient)
	{
		Gradient = std::make_unique<Color[]>(w * h);

		for (int i = 0; i < w; i++)
		{
			int div = w / 6;
			int phase = i / div;
			float t = (i % div) / (float)div;
			int r, g, b;

			switch (phase)
			{
			case(0):
				r = 255;
				g = 255 * t;
				b = 0;
				break;
			case(1):
				r = 255 * (1.f - t);
				g = 255;
				b = 0;
				break;
			case(2):
				r = 0;
				g = 255;
				b = 255 * t;
				break;
			case(3):
				r = 0;
				g = 255 * (1.f - t);
				b = 255;
				break;
			case(4):
				r = 255 * t;
				g = 0;
				b = 255;
				break;
			case(5):
				r = 255;
				g = 0;
				b = 255 * (1.f - t);
				break;
			case 6:
				r = 255;
				g = 255;
				b = 255;
				break;
			}

			for (int k = 0; k < h; k++)
			{
				float sat = k / (float)h;
				int _r = r + sat * (128 - r);
				int _g = g + sat * (128 - g);
				int _b = b + sat * (128 - b);

				*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
			}
		}

		GradientTexture = interfaces::surface->CreateNewTextureID(true);
		interfaces::surface->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
	}
	interfaces::surface->DrawSetColor(Color(255, 255, 255, 255));
	interfaces::surface->DrawSetTexture(GradientTexture);
	interfaces::surface->DrawTexturedRect(x, y, x + w, y + h);
}

void Render::outlined_rectyeti(int x, int y, int w, int h, Color color_out, Color color_in)
{
	interfaces::surface->DrawSetColor(color_in);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);

	interfaces::surface->DrawSetColor(color_out);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}
bool Render::TransformScreen(const Vector& in, Vector& out)
{
	static ptrdiff_t ptrViewMatrix;
	if (!ptrViewMatrix)
	{//                                                          
		ptrViewMatrix = static_cast<ptrdiff_t>(Utilities::Memory::FindPatternV2("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
		ptrViewMatrix += 0x3;
		ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
		ptrViewMatrix += 176;
	}
	const matrix3x4& worldToScreen = interfaces::engine->WorldToScreenMatrix(); // matrix



	int ScrW, ScrH;

	interfaces::engine->GetScreenSize(ScrW, ScrH);

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0; // 0 poniewaz z nie jest nam potrzebne | uzywamy tylko wysokosci i szerokosci (x,y)
	if (w > 0.01)
	{
		float inverseWidth = 1 / w; // inverse na 1 pozycje ekranu
		out.x = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
		out.y = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
		return true;
	}
	return false;
}
bool Render::WorldToScreen(const Vector& in, Vector& out)
{
	if (Render::TransformScreen(in, out)) {
		int w, h;
		interfaces::engine->GetScreenSize(w, h);
		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;
		return true;
	}
	return false;
}

void Render::Text(int x, int y, Color color, DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	interfaces::surface->DrawSetTextFont(font);

	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(wcstring, wcslen(wcstring));
	return;
}
void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(text, wcslen(text));
}

void Render::TEXTUNICODE(int x, int y, const char* _Input, int font, Color color)
{
	wchar_t buffer[36];
	if (MultiByteToWideChar(CP_UTF8, 0, _Input, -1, buffer, 36) > 0)
	{
		interfaces::surface->DrawSetTextColor(color);
		interfaces::surface->DrawSetTextFont(font);
		interfaces::surface->DrawSetTextPos(x, y);
		interfaces::surface->DrawPrintText(buffer, wcslen(buffer));
	}
}

void Render::Text2(int x, int y, const char* _Input, int font, Color color)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextPos(x, y);
	interfaces::surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}
void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Render::GetTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	interfaces::surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}	
RECT Render::GetTextSize2(const char* _Input, int font)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
	int Width = 0, Height = 0;

	interfaces::surface->GetTextSize(font, WideBuffer, Width, Height);

	RECT outcome = { 0, 0, Width, Height };
	return outcome;
}

void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::DrawCircle(float x, float y, float r, float segments, Color color)
{
	interfaces::surface->DrawSetColor(color);
	interfaces::surface->DrawOutlinedCircle(x, y, r, segments);
}

int TweakColor(int c1, int c2, int variation)
{
	if (c1 == c2)
		return c1;
	else if (c1 < c2)
		c1 += variation;
	else
		c1 -= variation;
	return c1;
}

Color Render::color_spectrum_pen(int x, int y, int w, int h, Vector stx)
{
	int div = w / 6;
	int phase = stx.x / div;
	float t = ((int)stx.x % div) / (float)div;
	int r, g, b;

	switch (phase)
	{
	case(0):
		r = 255;
		g = 255 * t;
		b = 0;
		break;
	case(1):
		r = 255 * (1.f - t);
		g = 255;
		b = 0;
		break;
	case(2):
		r = 0;
		g = 255;
		b = 255 * t;
		break;
	case(3):
		r = 0;
		g = 255 * (1.f - t);
		b = 255;
		break;
	case(4):
		r = 255 * t;
		g = 0;
		b = 255;
		break;
	case(5):
		r = 255;
		g = 0;
		b = 255 * (1.f - t);
		break;
	}

	float sat = stx.y / h;
	return Color(r + sat * (128 - r), g + sat * (128 - g), b + sat * (128 - b), 255);
}

void Render::gradient_horizontal(int x, int y, int w, int h, Color c1, Color c2)
{
	Render::rect(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		Render::rect(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::GradientB(int x, int y, int w, int h, Color color1, Color color2, int variation)
{
	int r1 = color1.r();
	int g1 = color1.g();
	int b1 = color1.b();
	int a1 = color1.a();

	int r2 = color2.r();
	int g2 = color2.g();
	int b2 = color2.b();
	int a2 = color2.a();

	for (int i = 0; i <= w; i++)
	{
		Render::DrawRect(x + i, y, 1, h, Color(r1, g1, b1, a1));
		r1 = TweakColor(r1, r2, variation);
		g1 = TweakColor(g1, g2, variation);
		b1 = TweakColor(b1, b2, variation);
		a1 = TweakColor(a1, a2, variation);
	}
}

void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = interfaces::surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	interfaces::surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	interfaces::surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	interfaces::surface->DrawSetTexture(Texture); // bind texture

	interfaces::surface->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Render::Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}
void Render::Color_spectrum(int x, int y, int w, int h)
{
	static int GradientTexture = 0;
	static std::unique_ptr<Color[]> Gradient = nullptr;
	if (!Gradient)
	{
		Gradient = std::make_unique<Color[]>(w * h);

		for (int i = 0; i < w; i++)
		{
			int div = w / 6;
			int phase = i / div;
			float t = (i % div) / (float)div;
			int r, g, b;

			switch (phase)
			{
			case(0):
				r = 255;
				g = 255 * t;
				b = 0;
				break;
			case(1):
				r = 255 * (1.f - t);
				g = 255;
				b = 0;
				break;
			case(2):
				r = 0;
				g = 255;
				b = 255 * t;
				break;
			case(3):
				r = 0;
				g = 255 * (1.f - t);
				b = 255;
				break;
			case(4):
				r = 255 * t;
				g = 0;
				b = 255;
				break;
			case(5):
				r = 255;
				g = 0;
				b = 255 * (1.f - t);
				break;
			}

			for (int k = 0; k < h; k++)
			{
				float sat = k / (float)h;
				int _r = r + sat * (128 - r);
				int _g = g + sat * (128 - g);
				int _b = b + sat * (128 - b);

				*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
			}
		}

		GradientTexture = interfaces::surface->CreateNewTextureID(true);
		interfaces::surface->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
	}
	interfaces::surface->DrawSetColor(Color(255, 255, 255, 255));
	interfaces::surface->DrawSetTexture(GradientTexture);
	interfaces::surface->DrawTexturedRect(x, y, x + w, y + h);
}
Color Render::Color_spectrum_pen(int x, int y, int w, int h, Vector stx)
{
	int div = w / 6;
	int phase = stx.x / div;
	float t = ((int)stx.x % div) / (float)div;
	float r, g, b;

	switch (phase)
	{
	case(0):
		r = 255;
		g = 255 * t;
		b = 0;
		break;
	case(1):
		r = 255 * (1.f - t);
		g = 255;
		b = 0;
		break;
	case(2):
		r = 0;
		g = 255;
		b = 255 * t;
		break;
	case(3):
		r = 0;
		g = 255 * (1.f - t);
		b = 255;
		break;
	case(4):
		r = 255 * t;
		g = 0;
		b = 255;
		break;
	case(5):
		r = 255;
		g = 0;
		b = 255 * (1.f - t);
		break;
	}

	float sat = stx.y / h;
	return Color(r + sat * (128 - r), g + sat * (128 - g), b + sat * (128 - b), 255);
}


void Render::DrawTexturedPoly(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = interfaces::surface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	interfaces::surface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	interfaces::surface->DrawSetColor(col);
	interfaces::surface->DrawSetTexture(texture_id);
	interfaces::surface->DrawTexturedPolygon(n, vertice);
}

void Render::DrawFilledCircle(Vector2D center, Color color, float radius, float points)
{
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

	DrawTexturedPoly((int)points, vertices.data(), color);
}

void Render::GradientSideways(int x, int y, int w, int h, Color color1, Color color2, int variation)
{
	int r1 = color1.r();
	int g1 = color1.g();
	int b1 = color1.b();
	int a1 = color1.a();

	int r2 = color2.r();
	int g2 = color2.g();
	int b2 = color2.b();
	int a2 = color2.a();

	for (int i = 0; i <= w; i++)
	{
		Render::DrawRect(x + i, y, 1, h, Color(r1, g1, b1, a1));
		r1 = TweakColor(r1, r2, variation);
		g1 = TweakColor(g1, g2, variation);
		b1 = TweakColor(b1, b2, variation);
		a1 = TweakColor(a1, a2, variation);
	}
}
























































































































































































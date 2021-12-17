#include "../includes.h"
#include "../SDK/IEngine.h"
#include "render.h"
#include "interfaces.h"

#include "../SDK/IMaterial.h"
#include "../SDK/IVDebugOverlay.h"


namespace RENDER
{
	unsigned int CreateF(std::string font_name, int size, int weight, int blur, int scanlines, int flags)
	{
		auto font = INTERFACES::Surface->SCreateFont();
		INTERFACES::Surface->SetFontGlyphSet(font, font_name.c_str(), size, weight, blur, scanlines, flags);

		return font;
	}
	void Clear(int x, int y, int w, int h, CColor color)
	{
		INTERFACES::Surface->DrawFilledRect(x, y, x + w, y + h);
	}
	RECT get_text_size(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		INTERFACES::Surface->GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}
	void Outline(int x, int y, int w, int h, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}
	void Color_spectrum(int x, int y, int w, int h)
	{
		static int GradientTexture = 0;
		static std::unique_ptr<CColor[]> Gradient = nullptr;
		if (!Gradient)
		{
			Gradient = std::make_unique<CColor[]>(w * h);

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

					*reinterpret_cast<CColor*>(Gradient.get() + i + k * w) = CColor(_r, _g, _b);
				}
			}

			GradientTexture = INTERFACES::Surface->CreateNewTextureID(true);
			INTERFACES::Surface->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
		}
		INTERFACES::Surface->DrawSetColor(CColor(255, 255, 255, 255));
		INTERFACES::Surface->DrawSetTexture(GradientTexture);
		INTERFACES::Surface->DrawTexturedRect(x, y, x + w, y + h);
	}
	void OutlinedRect(int x, int y, int w, int h, CColor color_out, CColor color_in)
	{
		INTERFACES::Surface->DrawSetColor(color_in);
		INTERFACES::Surface->DrawFilledRect(x, y, x + w, y + h);

		INTERFACES::Surface->DrawSetColor(color_out);
		INTERFACES::Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}
	void rect(int x, int y, int w, int h, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawFilledRect(x, y, x + w, y + h);
	}
	void DrawOutlinedRect(int x, int y, int w, int h, CColor col)
	{
		INTERFACES::Surface->DrawSetColor(col);
		INTERFACES::Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}
	void Text2(int x, int y, const char* _Input, int font, CColor color)
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

		INTERFACES::Surface->DrawSetTextColor(color);
		INTERFACES::Surface->DrawSetTextFont(font);
		INTERFACES::Surface->DrawSetTextPos(x, y);
		INTERFACES::Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}
	CColor Color_spectrum_pen(int x, int y, int w, int h, Vector stx)
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
		return CColor(r + sat * (128 - r), g + sat * (128 - g), b + sat * (128 - b), 255);
	}
	void GradientV(int x, int y, int w, int h, CColor c1, CColor c2)
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
			Clear(x, y + i, w, 1, CColor(first, second, third, ia));
		}
	}
	void GradientH(int x, int y, int w, int h, CColor c1, CColor c2)
	{
		Clear(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < w; i++)
		{
			float fi = i, fw = w;
			float a = fi / fw;
			DWORD ia = a * 255;
			Clear(x + i, y, 1, h, CColor(first, second, third, ia));
		}
	}
	RECT GetViewport()
	{
		RECT Viewport = { 0, 0, 0, 0 };
		int w, h;
		INTERFACES::Engine->GetScreenSize(w, h);
		Viewport.right = w; Viewport.bottom = h;
		return Viewport;
	}
	void Text(int x, int y, CColor color, DWORD font, const char* text, ...)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		INTERFACES::Surface->DrawSetTextFont(font);

		INTERFACES::Surface->DrawSetTextColor(color);
		INTERFACES::Surface->DrawSetTextPos(x, y);
		INTERFACES::Surface->DrawPrintText(wcstring, wcslen(wcstring));
	}
	void old_text(int x, int y, const wchar_t* _Input, int font, CColor color)
	{
		INTERFACES::Surface->DrawSetTextFont(font);
		INTERFACES::Surface->DrawSetTextColor(color);
		INTERFACES::Surface->DrawSetTextPos(x, y);

		INTERFACES::Surface->DrawPrintText(_Input, wcslen(_Input));
	}
	void Text(int x, int y, CColor color, DWORD font, const wchar_t* text)
	{
		INTERFACES::Surface->DrawSetTextFont(font);
		INTERFACES::Surface->DrawSetTextColor(color);
		INTERFACES::Surface->DrawSetTextPos(x, y);
		INTERFACES::Surface->DrawPrintText(text, wcslen(text));
	}
	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, CColor Color, std::string Input)
	{
		/* char -> wchar */
		size_t size = Input.size() + 1;
		auto wide_buffer = std::make_unique<wchar_t[]>(size);
		mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

		/* check center */
		int width = 0, height = 0;
		INTERFACES::Surface->GetTextSize(Font, wide_buffer.get(), width, height);
		if (!center_width)
			width = 0;
		if (!center_height)
			height = 0;

		/* call and draw*/
		INTERFACES::Surface->DrawSetTextColor(Color);
		INTERFACES::Surface->DrawSetTextFont(Font);
		INTERFACES::Surface->DrawSetTextPos(X - (width * .5), Y - (height * .5));
		INTERFACES::Surface->DrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()), 0);
	}
	void DrawWF(int X, int Y, unsigned int Font, CColor Color, const wchar_t* Input) //std::string for the nn's
	{
		/* call and draw*/
		INTERFACES::Surface->DrawSetTextColor(Color);
		INTERFACES::Surface->DrawSetTextFont(Font);
		INTERFACES::Surface->DrawSetTextPos(X, Y);
		INTERFACES::Surface->DrawPrintText(Input, wcslen(Input), 0);
	}
	Vector2D GetTextSize(unsigned int Font, std::string Input)
	{
		/* char -> wchar */
		size_t size = Input.size() + 1;
		auto wide_buffer = std::make_unique<wchar_t[]>(size);
		mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

		int width, height;
		INTERFACES::Surface->GetTextSize(Font, wide_buffer.get(), width, height);

		return Vector2D(width, height);
	}
	void FillRectangle(int x1, int y2, int width, int height, CColor color) {
		INTERFACES::Surface->DrawSetTextColor(color);
		INTERFACES::Surface->DrawFilledRect(x1, y2, x1 + width, y2 + height);
	}
	void DrawLine(int x1, int y1, int x2, int y2, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawLine(x1, y1, x2, y2);
	}

	void DrawEmptyRect(int x1, int y1, int x2, int y2, CColor color, unsigned char ignore_flags)
	{
		INTERFACES::Surface->DrawSetColor(color);
		if (!(ignore_flags & 0b1))
			INTERFACES::Surface->DrawLine(x1, y1, x2, y1);
		if (!(ignore_flags & 0b10))
			INTERFACES::Surface->DrawLine(x2, y1, x2, y2);
		if (!(ignore_flags & 0b100))
			INTERFACES::Surface->DrawLine(x2, y2, x1, y2);
		if (!(ignore_flags & 0b1000))
			INTERFACES::Surface->DrawLine(x1, y2, x1, y1);
	}
	void DrawCornerRect(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const CColor& color, const CColor& outlined_color)
	{
		auto corner = [&](const int32_t _x, const int32_t _y, const int32_t width, const int32_t height, const bool right_side, const bool down, const CColor& _color, const bool _outlined, const CColor& _outlined_color) -> void
		{
			const auto corner_x = right_side ? _x - width : _x;
			const auto corner_y = down ? _y - height : _y;
			const auto corner_w = down && right_side ? width + 1 : width;

			DrawEmptyRect(corner_x, _y, corner_w, 1, _color);
			DrawEmptyRect(_x, corner_y, 1, height, _color);

			if (_outlined) {
				DrawEmptyRect(corner_x, down ? _y + 1 : _y - 1, !down && right_side ? corner_w + 1 : corner_w, 1, _outlined_color);
				DrawEmptyRect(right_side ? _x + 1 : _x - 1, down ? corner_y : corner_y - 1, 1, down ? height + 2 : height + 1, _outlined_color);
			}
		};

		corner(x - w, y, w / 2, w / 2, false, false, color, outlined, outlined_color);
		corner(x - w, y + h, w / 2, w / 2, false, true, color, outlined, outlined_color);
		corner(x + w, y, w / 2, w / 2, true, false, color, outlined, outlined_color);
		corner(x + w, y + h, w / 2, w / 2, true, true, color, outlined, outlined_color);
	}
	void DrawEdges(float topX, float topY, float bottomX, float bottomY, float length, CColor color)
	{
		float scale = (bottomY - topY) / 5.0f;
		DrawLine(topX - scale, topX - scale + length, topY, topY, color); //  --- Top left
		DrawLine(topX - scale, topX - scale, topY, topY + length, color); // | Top left
		DrawLine(topX + scale, topX + scale + length, topY, topY, color); // --- Top right
		DrawLine(topX + scale + length, topX + scale + length, topY, topY + length, color); // | Top right
		DrawLine(bottomX - scale, topX - scale + length, bottomY, bottomY, color); // --- Bottom left
		DrawLine(bottomX - scale, topX - scale, bottomY, bottomY - length, color); // | Bottom left
		DrawLine(bottomX + scale, topX + scale + length, bottomY, bottomY, color); // --- Bottom right
		DrawLine(bottomX + scale + length, topX + scale + length, bottomY, bottomY - length, color); // | Bottom right
	}
	void DrawFilledRect(int x1, int y1, int x2, int y2, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawFilledRect(x1, y1, x2, y2);
	}
	void DrawFilledRectOutline(int x1, int y1, int x2, int y2, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawFilledRect(x1, y1, x2, y2);
		DrawEmptyRect(x1 - 1, y1 - 1, x2, y2, CColor(0, 0, 0, 100));
	}
	void DrawFilledRectArray(SDK::IntRect* rects, int rect_amount, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawFilledRectArray(rects, rect_amount);
	}

	void DrawCircle(int x, int y, int radius, int segments, CColor color)
	{
		INTERFACES::Surface->DrawSetColor(color);
		INTERFACES::Surface->DrawOutlinedCircle(x, y, radius, segments);
	}
	void DrawTexturedPoly(int n, SDK::Vertex_t* vertice, CColor col)
	{

		static int texture_id = INTERFACES::Surface->CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };

		INTERFACES::Surface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
		INTERFACES::Surface->DrawSetColor(col);

		INTERFACES::Surface->DrawSetTexture(texture_id);
		INTERFACES::Surface->DrawTexturedPolygon(n, vertice);
	}
	void TexturedPolygon(int n, std::vector<SDK::Vertex_t> vertice, CColor color) {
		static int texture_id = INTERFACES::Surface->CreateNewTextureID(true); // 
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		INTERFACES::Surface->DrawSetTextureRGBA(texture_id, buf, 1, 1, 1, 1); //
		INTERFACES::Surface->DrawSetColor(color); //
		INTERFACES::Surface->DrawSetTexture(texture_id); //
		INTERFACES::Surface->DrawTexturedPolygon(n, vertice.data()); //
	}
	void DrawFilledCircle(Vector2D center, CColor color, float radius, float points) {
		std::vector<SDK::Vertex_t> vertices;
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(SDK::Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

		DrawTexturedPoly((int)points, vertices.data(), color);
	}
	void RoundedBoxStatic(int x, int y, int w, int h, int radius, CColor col)
	{
		SDK::Vertex_t round[64];

		for (int i = 0; i < 4; i++)
		{
			int _x = x + ((i < 2) ? (w - radius) : radius);
			int _y = y + ((i % 3) ? (h - radius) : radius);

			float a = 90.f * i;

			for (int j = 0; j < 16; j++)
			{
				float _a = DEG2RAD(a + j * 6.f);

				round[(i * 16) + j] = SDK::Vertex_t(Vector2D(_x + radius * sin(_a), _y - radius * cos(_a)));
			}
		}

		INTERFACES::Surface->DrawSetColor(col);
		INTERFACES::Surface->DrawTexturedPolygon(64, round);
	}
	/*void DrawFilledCircle(Vector2D center, CColor color, CColor outline, float radius, float points)
	{
		static bool once = true;

		static std::vector<float> temppointsx;
		static std::vector<float> temppointsy;

		if (once)
		{
			float step = (float)M_PI * 2.0f / points;
			for (float a = 0; a < (M_PI * 2.0f); a += step)
			{
				float x = cosf(a);
				float y = sinf(a);
				temppointsx.push_back(x);
				temppointsy.push_back(y);
			}
			once = false;
		}

		std::vector<int> pointsx;
		std::vector<int> pointsy;
		std::vector<SDK::Vertex_t> vertices;

		for (int i = 0; i < temppointsx.size(); i++)
		{
			float x = radius * temppointsx[i] + center.x;
			float y = radius * temppointsy[i] + center.y;
			pointsx.push_back(x);
			pointsy.push_back(y);

			vertices.push_back(SDK::Vertex_t(Vector2D(x, y)));
		}

		TexturedPolygon(vertices.size(), vertices, color);
		INTERFACES::Surface->DrawSetColor(outline);
		INTERFACES::Surface->DrawPolyLine(pointsx.data(), pointsy.data(), points);
	}*/
	void DrawSomething()
	{

	}
	bool WorldToScreen(Vector world, Vector& screen)
	{
		return (INTERFACES::DebugOverlay->ScreenPosition(world, screen) != 1);
	}
	void DrawFilled3DBox(Vector origin, int width, int height, CColor outline, CColor filling)
	{
		float difw = float(width / 2);
		float difh = float(height / 2);
		Vector boxVectors[8] =
		{
			Vector(origin.x - difw, origin.y - difh, origin.z - difw),
			Vector(origin.x - difw, origin.y - difh, origin.z + difw),
			Vector(origin.x + difw, origin.y - difh, origin.z + difw),
			Vector(origin.x + difw, origin.y - difh, origin.z - difw),
			Vector(origin.x - difw, origin.y + difh, origin.z - difw),
			Vector(origin.x - difw, origin.y + difh, origin.z + difw),
			Vector(origin.x + difw, origin.y + difh, origin.z + difw),
			Vector(origin.x + difw, origin.y + difh, origin.z - difw),
		};

		static Vector vec0, vec1, vec2, vec3,
			vec4, vec5, vec6, vec7;

		if (WorldToScreen(boxVectors[0], vec0) &&
			WorldToScreen(boxVectors[1], vec1) &&
			WorldToScreen(boxVectors[2], vec2) &&
			WorldToScreen(boxVectors[3], vec3) &&
			WorldToScreen(boxVectors[4], vec4) &&
			WorldToScreen(boxVectors[5], vec5) &&
			WorldToScreen(boxVectors[6], vec6) &&
			WorldToScreen(boxVectors[7], vec7))
		{
			Vector2D lines[12][2];
			lines[0][0] = vec0;
			lines[0][1] = vec1;
			lines[1][0] = vec1;
			lines[1][1] = vec2;
			lines[2][0] = vec2;
			lines[2][1] = vec3;
			lines[3][0] = vec3;
			lines[3][1] = vec0;

			// top of box
			lines[4][0] = vec4;
			lines[4][1] = vec5;
			lines[5][0] = vec5;
			lines[5][1] = vec6;
			lines[6][0] = vec6;
			lines[6][1] = vec7;
			lines[7][0] = vec7;
			lines[7][1] = vec4;

			lines[8][0] = vec0;
			lines[8][1] = vec4;

			lines[9][0] = vec1;
			lines[9][1] = vec5;

			lines[10][0] = vec2;
			lines[10][1] = vec6;

			lines[11][0] = vec3;
			lines[11][1] = vec7;

			for (int i = 0; i < 12; i++)
				DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, outline);
		}
	}
}
#pragma once
#include <cstdint>
#include <windows.h>
#include <cmath>

class Color
{
public:
	Color();
	Color(int _r, int _g, int _b);
	Color(int _r, int _g, int _b, int _a);
	Color(float _r, float _g, float _b) : Color(_r, _g, _b, 1.0f) {}
	Color(float _r, float _g, float _b, float _a)
		: Color(
			static_cast<int>(_r * 255.0f),
			static_cast<int>(_g * 255.0f),
			static_cast<int>(_b * 255.0f),
			static_cast<int>(_a * 255.0f))
	{
	}
	explicit Color(float* rgb) : Color(rgb[0], rgb[1], rgb[2], 1.0f) {}
	explicit Color(unsigned long argb)
	{
		_CColor[2] = (unsigned char)((argb & 0x000000FF) >> (0 * 8));
		_CColor[1] = (unsigned char)((argb & 0x0000FF00) >> (1 * 8));
		_CColor[0] = (unsigned char)((argb & 0x00FF0000) >> (2 * 8));
		_CColor[3] = (unsigned char)((argb & 0xFF000000) >> (3 * 8));
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	void    SetRawColor(int color32);
	int     GetRawColor() const;
	void    SetColor(int _r, int _g, int _b, int _a = 0);
	void    SetColor(float _r, float _g, float _b, float _a = 0);
	void	SetColor(float* color);
	void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

	int r() const { return _CColor[0]; }
	int g() const { return _CColor[1]; }
	int b() const { return _CColor[2]; }
	int a() const { return _CColor[3]; }

	void SetAlpha(int alpha) { _CColor[3] = alpha; }

	unsigned char &operator[](int index)
	{
		return _CColor[index];
	}
	const unsigned char &operator[](int index) const
	{
		return _CColor[index];
	}

	bool operator==(const Color &rhs) const;
	bool operator!=(const Color &rhs) const;
	Color &operator=(const Color &rhs);

	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Grey;
	static Color Purple;
	static Color Gray_v2;
	static Color Gray_v3;

	unsigned char _CColor[4];
};

class color
{
public:
	unsigned char RGBA[4];

	color()
	{
		RGBA[0] = 255;
		RGBA[1] = 255;
		RGBA[2] = 255;
		RGBA[3] = 255;
	}

	color(int r, int g, int b, int a = 255)
	{
		RGBA[0] = static_cast<unsigned char>(r);
		RGBA[1] = static_cast<unsigned char>(g);
		RGBA[2] = static_cast<unsigned char>(b);
		RGBA[3] = static_cast<unsigned char>(a);
	}

	bool operator!=(color color)
	{
		return RGBA[0] != color.RGBA[0] || RGBA[1] != color.RGBA[1] || RGBA[2] != color.RGBA[2] || RGBA[3] != color.RGBA[3];
	}

	bool operator==(color color)
	{
		return RGBA[0] == color.RGBA[0] && RGBA[1] == color.RGBA[1] && RGBA[2] == color.RGBA[2] && RGBA[3] == color.RGBA[3];
	}

	// returns the color from 0.f - 1.f
	static float Base(const unsigned char col)
	{
		return col / 255.f;
	}

	static color Inverse(const color clr)
	{
		return color(255 - clr.RGBA[0], 255 - clr.RGBA[1], 255 - clr.RGBA[2]);
	}

	float Difference(const color color) const // from 0.f - 1.f with 1.f being the most different
	{
		float red_diff = fabs(Base(RGBA[0]) - Base(color.RGBA[0]));
		float green_diff = fabs(Base(RGBA[1]) - Base(color.RGBA[1]));
		float blue_diff = fabs(Base(RGBA[2]) - Base(color.RGBA[2]));
		float alpha_diff = fabs(Base(RGBA[3]) - Base(color.RGBA[3]));

		return (red_diff + green_diff + blue_diff + alpha_diff) * 0.25f;
	}

	// RGB -> HSB
	static float Hue(const color color)
	{
		float R = Base(color.RGBA[0]);
		float G = Base(color.RGBA[1]);
		float B = Base(color.RGBA[2]);

		float mx = max(R, max(G, B));
		float mn = min(R, min(G, B));
		if (mx == mn)
			return 0.f;

		float delta = mx - mn;

		float hue = 0.f;
		if (mx == R)
			hue = (G - B) / delta;
		else if (mx == G)
			hue = 2.f + (B - R) / delta;
		else
			hue = 4.f + (R - G) / delta;

		hue *= 60.f;
		if (hue < 0.f)
			hue += 360.f;

		return hue / 360.f;
	}

	static float Saturation(const color color)
	{
		float R = Base(color.RGBA[0]);
		float G = Base(color.RGBA[1]);
		float B = Base(color.RGBA[2]);

		float mx = max(R, max(G, B));
		float mn = min(R, min(G, B));

		float delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	static float Brightness(const color color)
	{
		float R = Base(color.RGBA[0]);
		float G = Base(color.RGBA[1]);
		float B = Base(color.RGBA[2]);

		return max(R, max(G, B));
	}

	float Hue() const
	{
		return Hue(*this);
	}

	float Saturation() const
	{
		return Saturation(*this);
	}

	float Brightness() const
	{
		return Brightness(*this);
	}
};
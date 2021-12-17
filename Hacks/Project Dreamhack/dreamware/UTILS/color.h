#pragma once

#define NOCOLOR CColor(0, 0, 0, 0)
#define WHITE CColor(255, 255, 255, 255)
#define PURPLE CColor(132, 0, 218)
#define BLACK CColor(0, 0, 0, 255)
#define RED CColor(255, 0, 0, 255)
#define LIGHTRED CColor(255, 100, 100, 255)
#define WHITERED CColor(255, 65, 65, 255);
#define DARKRED CColor(120, 0, 0, 255)
#define GREEN CColor(0, 255, 0, 255)
#define ORANGE CColor(255, 100, 0, 255)
#define BLUE CColor(0, 0, 255, 255)
#define LIGHTBLUE CColor(0, 140, 255, 255)
#define DARKGREY CColor(55, 55, 55, 255)
#define GREY CColor(70, 70, 70, 255)
#define LIGHTGREY CColor(150, 150, 150, 255)
#define HOTPINK CColor(255, 20, 147, 255)
#define CYAN CColor(0, 255, 255, 255)
#define YELLOW CColor(255, 255, 0, 255)


class CColor
{
public:

	unsigned char RGBA[4];

	CColor()
	{
		*((int *)this) = 0;
	}

	CColor(float _rgb[3])
	{
		SetColor((int)(_rgb[0] * 255.0f), (int)(_rgb[1] * 255.0f), (int)(_rgb[2] * 255.0f), (int)(_rgb[3] * 255.0f));
	}

	CColor(int color32)
	{
		*((int *)this) = color32;
	}

	CColor(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 255);
	}

	CColor(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	void SetColor(int _r, int _g, int _b, int _a = 255)
	{
		RGBA[0] = (unsigned char)_r;
		RGBA[1] = (unsigned char)_g;
		RGBA[2] = (unsigned char)_b;
		RGBA[3] = (unsigned char)_a;
	}

	void GetColor(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = RGBA[0];
		_g = RGBA[1];
		_b = RGBA[2];
		_a = RGBA[3];
	}

	void SetRawColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int*)this);
	}

	int GetD3DColor() const
	{
		return ((int)((((RGBA[3]) & 0xff) << 24) | (((RGBA[0]) & 0xff) << 16) | (((RGBA[1]) & 0xff) << 8) | ((RGBA[2]) & 0xff)));
	}

	inline int r() const
	{
		return RGBA[0];
	}

	inline int g() const
	{
		return RGBA[1];
	}

	inline int b() const
	{
		return RGBA[2];
	}

	inline int a() const
	{
		return RGBA[3];
	}

	inline float rBase() const
	{
		return RGBA[0] / 255.0f;
	}

	inline float gBase() const
	{
		return RGBA[1] / 255.0f;
	}

	inline float bBase() const
	{
		return RGBA[2] / 255.0f;
	}

	inline float aBase() const
	{
		return RGBA[3] / 255.0f;
	}

	unsigned char& operator[](int index)
	{
		return RGBA[index];
	}

	const unsigned char& operator[](int index) const
	{
		return RGBA[index];
	}

	bool operator ==(const CColor& rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator !=(const CColor& rhs) const
	{
		return !(operator==(rhs));
	}

	CColor& operator=(const CColor& rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

	float* Base()
	{
		float clr[3];

		clr[0] = RGBA[0] / 255.0f;
		clr[1] = RGBA[1] / 255.0f;
		clr[2] = RGBA[2] / 255.0f;

		return &clr[0];
	}

	float* BaseAlpha()
	{
		float clr[4];

		clr[0] = RGBA[0] / 255.0f;
		clr[1] = RGBA[1] / 255.0f;
		clr[2] = RGBA[2] / 255.0f;
		clr[3] = RGBA[3] / 255.0f;

		return &clr[0];
	}

	float Hue() const
	{
		if (RGBA[0] == RGBA[1] && RGBA[1] == RGBA[2])
		{
			return 0.0f;
		}

		float r = RGBA[0] / 255.0f;
		float g = RGBA[1] / 255.0f;
		float b = RGBA[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float delta = max - min;
		float hue = 0.0f;

		if (r == max)
		{
			hue = (g - b) / delta;
		}
		else if (g == max)
		{
			hue = 2 + (b - r) / delta;
		}
		else if (b == max)
		{
			hue = 4 + (r - g) / delta;
		}
		hue *= 60;

		if (hue < 0.0f)
		{
			hue += 360.0f;
		}
		return hue;
	}

	static CColor HSBtoRGB(float hue /* 0.f - 1.f*/,
		float saturation /* 0.f - 1.f */,
		float brightness /* 0.f - 1.f */,
		int alpha = 255)
	{
		hue = UTILS::clamp(hue, 0.f, 1.f);
		saturation = UTILS::clamp(saturation, 0.f, 1.f);
		brightness = UTILS::clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int>(h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f)
			return CColor(brightness * 255, t * 255, p * 255, alpha);
		else if (h < 2.f)
			return CColor(q * 255, brightness * 255, p * 255, alpha);
		else if (h < 3.f)
			return CColor(p * 255, brightness * 255, t * 255, alpha);
		else if (h < 4)
			return CColor(p * 255, q * 255, brightness * 255, alpha);
		else if (h < 5)
			return CColor(t * 255, p * 255, brightness * 255, alpha);
		else
			return CColor(brightness * 255, p * 255, q * 255, alpha);

	}

	float Saturation() const
	{
		float r = RGBA[0] / 255.0f;
		float g = RGBA[1] / 255.0f;
		float b = RGBA[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float l, s = 0;

		if (max != min)
		{
			l = (max + min) / 2;
			if (l <= 0.5f)
				s = (max - min) / (max + min);
			else
				s = (max - min) / (2 - max - min);
		}
		return s;
	}

	float Brightness() const
	{
		float r = RGBA[0] / 255.0f;
		float g = RGBA[1] / 255.0f;
		float b = RGBA[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		return (max + min) / 2;
	}

	static CColor FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return CColor(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return CColor(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return CColor(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return CColor(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return CColor(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return CColor(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	static CColor Red()
	{
		return CColor(255, 0, 0);
	}

	static CColor Green()
	{
		return CColor(0, 255, 0);
	}

	static CColor Blue()
	{
		return CColor(0, 0, 255);
	}

	static CColor LightBlue()
	{
		return CColor(100, 100, 255);
	}

	static CColor Grey()
	{
		return CColor(128, 128, 128);
	}

	static CColor DarkGrey()
	{
		return CColor(45, 45, 45);
	}

	static CColor Black()
	{
		return CColor(0, 0, 0);
	}

	static CColor White()
	{
		return CColor(255, 255, 255);
	}

	static CColor Purple()
	{
		return CColor(220, 0, 220);
	}

	//Menu
	static CColor Background()
	{
		return CColor(55, 55, 55);
	}

	static CColor FrameBorder()
	{
		return CColor(80, 80, 80);
	}

	static CColor MainText()
	{
		return CColor(230, 230, 230);
	}

	static CColor HeaderText()
	{
		return CColor(49, 124, 230);
	}

	static CColor CurrentTab()
	{
		return CColor(55, 55, 55);
	}

	static CColor Tabs()
	{
		return CColor(23, 23, 23);
	}

	static CColor Highlight()
	{
		return CColor(49, 124, 230);
	}

	static CColor ElementBorder()
	{
		return CColor(0, 0, 0);
	}

	static CColor SliderScroll()
	{
		return CColor(78, 143, 230);
	}

private:

};

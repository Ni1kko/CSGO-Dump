#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../../imgui/imgui.h"
/*#include <optional>
#include <d3d9.h>*/

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
	Color(Color clr, int alpha) {
		this->SetColor(clr.r(), clr.g(), clr.b(), alpha);
	}

	void    SetRawColor(int color32);
	int     GetRawColor() const;
	void    SetColor(int _r, int _g, int _b, int _a = 0);
	void    SetColor(float _r, float _g, float _b, float _a = 0);
	void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

	std::string GetNormalnijHexColor() const;

	int r() const { return _CColor[0]; }
	int g() const { return _CColor[1]; }
	int b() const { return _CColor[2]; }
	int a() const { return _CColor[3]; }

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
	//D3DCOLOR direct() const;

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

	void UpdateRainbow(float saturation, float brightness) {
		this->frainbow += 0.001093f;
		if (this->frainbow > 1.f) this->frainbow = 0.f;

		auto col_done = FromHSB(this->frainbow, saturation, brightness);
		this->SetColor(col_done.r(), col_done.g(), col_done.b(), this->a());
	}

	operator ImColor() {
		return ImColor((float)r(), (float)g(), (float)b(), (float)a());
	}
	
	operator ImVec4() {
		return ImVec4((float)r(), (float)g(), (float)b(), (float)a());
	}

	operator ImU32() {
		return (((unsigned char)a() & 0xff) << 24) + (((unsigned char)b() & 0xff) << 16) + (((unsigned char)g() & 0xff) << 8) + ((unsigned char)r() & 0xff);
	}

	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;
public:
	bool  rainbow  = false;
	float frainbow = 0.f;
private:
	unsigned char _CColor[4];
/*private:
	static std::optional<int> alpha_override;*/
};
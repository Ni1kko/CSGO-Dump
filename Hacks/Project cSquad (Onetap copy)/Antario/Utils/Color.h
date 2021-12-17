#pragma once

// Colors
class Color2
{
public:
	// constructors
	Color2()
	{
		*((int *)this) = 0;
	}
	Color2(int r, int g, int b)
	{
		SetColor(r, g, b, 0);
	}
	Color2(int r, int g, int b, int a)
	{
		SetColor(r, g, b, a);
	}
	void SetColor(int r, int g, int b, int a = 0)
	{
		_color[0] = (unsigned char)r;
		_color[1] = (unsigned char)g;
		_color[2] = (unsigned char)b;
		_color[3] = (unsigned char)a;
	}

	void GetColor(int &r, int &g, int &b, int &a) const
	{
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	inline float rBase() const { return _color[0] / 255.0f; }
	inline float gBase() const { return _color[1] / 255.0f; }
	inline float bBase() const { return _color[2] / 255.0f; }
	inline float aBase() const { return _color[3] / 255.0f; }

	void SetAlpha(int a) { _color[0] = (unsigned char)a; }
	int  GetAlpha() { return _color[0]; }


	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color2 &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color2 &rhs) const
	{
		return !(operator==(rhs));
	}

	Color2 &operator=(const Color2 &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}
	static Color2 FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color2(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color2(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color2(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color2(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color2(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color2(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	auto ToHSV(float& h, float& s, float& v)
	{
		float col[3] = { r() / 255.f, g() / 255.f, b() / 255.f };

		float K = 0.f;
		if (col[1] < col[2])
		{
			swap(col[1], col[2]);
			K = -1.f;
		}
		if (col[0] < col[1])
		{
			swap(col[0], col[1]);
			K = -2.f / 6.f - K;
		}

		const float chroma = col[0] - (col[1] < col[2] ? col[1] : col[2]);
		h = colfabs(K + (col[1] - col[2]) / (6.f * chroma + 1e-20f));
		s = chroma / (col[0] + 1e-20f);
		v = col[1];
	}



private:
	unsigned char _color[4];
	constexpr void  swap(float& a, float& b) { float tmp = a; a = b; b = tmp; }
	constexpr float colfabs(const float& x) { return x < 0 ? x * -1 : x; }
};

struct Color
{
    int red, green, blue, alpha;

    constexpr Color() : red(0), green(0), blue(0), alpha(255) { }

    constexpr Color(int r, int g, int b, int a = 255)
        : red{ r }, green{ g }, blue{ b }, alpha{ a } { }
		
	void SetColor(int _r, int _g, int _b, int _a = 255)
	{
		this->red = (unsigned char)_r;
		this->green = (unsigned char)_g;
		this->blue = (unsigned char)_b;
		this->alpha = (unsigned char)_a;
	}


	Color(float _rgb[3], float _alpha)
	{
		SetColor((int)(_rgb[0] * 255.0f), (int)(_rgb[1] * 255.0f), (int)(_rgb[2] * 255.0f), _alpha);
	}


    constexpr Color& operator *=(const float coeff)
    {
        this->red   = static_cast<int>(this->red * coeff);
        this->green = static_cast<int>(this->green * coeff);
        this->blue  = static_cast<int>(this->blue * coeff);
        return *this;
    }

    constexpr Color operator ()(const int a) const
    {
        return Color(red, green, blue, a);
    }

    constexpr Color& operator /=(const float div)
    {
        const auto flDiv = 1.f / div;
        *this *= flDiv;
        return *this;
    }

    constexpr Color& operator *(const float coeff) const
    {
        auto color    = *this;
        return color *= coeff;
    }


    constexpr Color& FromHSV(float h, float s, float v)
    {
        float colOut[3]{ };
        if (s == 0.0f)
        {
            red = green = blue = static_cast<int>(v * 255);
            return *this;
        }

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
        int   i = static_cast<int>(h);
        float f = h - static_cast<float>(i);
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (i)
        {
        case 0:
            colOut[0] = v;
            colOut[1] = t;
            colOut[2] = p;
            break;
        case 1:
            colOut[0] = q;
            colOut[1] = v;
            colOut[2] = p;
            break;
        case 2:
            colOut[0] = p;
            colOut[1] = v;
            colOut[2] = t;
            break;
        case 3:
            colOut[0] = p;
            colOut[1] = q;
            colOut[2] = v;
            break;
        case 4:
            colOut[0] = t;
            colOut[1] = p;
            colOut[2] = v;
            break;
        case 5: default:
            colOut[0] = v;
            colOut[1] = p;
            colOut[2] = q;
            break;
        }

        red   = static_cast<int>(colOut[0] * 255);
        green = static_cast<int>(colOut[1] * 255);
        blue  = static_cast<int>(colOut[2] * 255);
        return *this;
    }

    constexpr auto ToHSV(float& h, float& s, float& v)
    {
        float col[3]    = { red / 255.f, green / 255.f, blue / 255.f };

        float K = 0.f;
        if (col[1] < col[2])
        {
            swap(col[1], col[2]);
            K = -1.f;
        }
        if (col[0] < col[1])
        {
            swap(col[0], col[1]);
            K = -2.f / 6.f - K;
        }

        const float chroma = col[0] - (col[1] < col[2] ? col[1] : col[2]);
        h = colfabs(K + (col[1] - col[2]) / (6.f * chroma + 1e-20f));
        s = chroma / (col[0] + 1e-20f);
        v = col[1];
    }




    static constexpr Color Black(int a = 255) { return { 0, 0, 0, a }; }
    static constexpr Color Grey(int  a = 255) { return { 127, 127, 127, a }; }
    static constexpr Color White(int a = 255) { return { 255, 255, 255, a }; }
    static constexpr Color Red(int   a = 255) { return { 255, 0, 0, a }; }
    static constexpr Color Green(int a = 255) { return { 0, 255, 0, a }; }
    static constexpr Color Blue(int  a = 255) { return { 0, 0, 255, a }; }

private:
    constexpr void  swap(float& a, float& b) { float tmp = a; a = b; b = tmp; }
    constexpr float colfabs(const float& x)  { return x < 0 ? x * -1 : x; }

};
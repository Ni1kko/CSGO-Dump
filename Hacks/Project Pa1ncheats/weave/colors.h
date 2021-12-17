#pragma once
#include <algorithm>

class color_t {
public:
	color_t()
	{
		*((int *)this) = 0;
	}

	color_t(int color32)
	{
		*((int *)this) = color32;
	}

	color_t(int _r, int _g, int _b)
	{
		set(_r, _g, _b, 255);
	}

	color_t(int _r, int _g, int _b, int _a)
	{
		set(_r, _g, _b, _a);
	}

	void set(int _r, int _g, int _b, int _a = 255)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void get(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	int get_raw() const
	{
		return *((int*)this);
	}

	unsigned char* get_ptr_r() {
		return &_color[0];
	}
	unsigned char* get_ptr_g() {
		return &_color[1];
	}
	unsigned char* get_ptr_b() {
		return &_color[2];
	}
	unsigned char* get_ptr_a() {
		return &_color[3];
	}


	inline int get_red() const
	{
		return _color[0];
	}
	inline int get_green() const
	{
		return _color[1];
	}
	inline int get_blue() const
	{
		return _color[2];
	}
	inline int get_alpha() const
	{
		return _color[3];
	}

	inline void set_red(int value) {
		_color[0] = value;
	}
	inline void set_green(int value) {
		_color[1] = value;
	}
	inline void set_blue(int value) {
		_color[2] = value;
	}
	inline void set_alpha(int value) {
		_color[3] = value;
	}
	unsigned char& operator[](int index)
	{
		return _color[index];
	}

	const unsigned char& operator[](int index) const
	{
		return _color[index];
	}

	bool operator ==(const color_t& rhs) const
	{
		return rhs.get_red() == this->get_red()
			&& rhs.get_green() == this->get_green()
			&& rhs.get_blue() == this->get_blue()
			&& rhs.get_alpha() == this->get_alpha();
	}

	bool operator !=(const color_t& rhs) const
	{
		return !(operator==(rhs));
	}

	color_t& operator=(const color_t& rhs)
	{
		color_t clr = rhs;
		set_u32(clr.u32());
		return *this;
	}
	void set_u32(unsigned long color32)
	{
		*((unsigned long*)_color) = color32;
	}
	inline unsigned long u32()
	{
		return *((unsigned long*)_color);
	}
	double get_hue()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));
		if (mx == mn)
			return 0.f;

		double delta = mx - mn;

		double hue = 0.f;
		if (mx == r)
			hue = (g - b) / delta;
		else if (mx == g)
			hue = 2.f + (b - r) / delta;
		else
			hue = 4.f + (r - g) / delta;

		hue *= 60.f;
		if (hue < 0.f)
			hue += 360.f;

		return hue / 360.f;
	}

	double get_saturation()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));

		double delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	double get_brightness()
	{
		double r = _color[0] / 255.f;
		double g = _color[1] / 255.f;
		double b = _color[2] / 255.f;

		return max(r, max(g, b));
	}

	static color_t hsb(float hue, float saturation, float brightness)
	{
		auto clamp = [](float value, float min, float max) {
			if (value > max) return max;
			else if (value < min) return min;
			return value;
		};
		hue = clamp(hue, 0.f, 1.f);
		saturation = clamp(saturation, 0.f, 1.f);
		brightness = clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int>(h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f)
			return color_t((int)(brightness * 255), (int)(t * 255), (int)(p * 255));
		else if (h < 2.f)
			return color_t((int)(q * 255), (int)(brightness * 255), (int)(p * 255));
		else if (h < 3.f)
			return color_t((int)(p * 255), (int)(brightness * 255), (int)(t * 255));
		else if (h < 4)
			return color_t((int)(p * 255), (int)(q * 255), (int)(brightness * 255));
		else if (h < 5)
			return color_t((int)(t * 255), (int)(p * 255), (int)(brightness * 255));
		else
			return color_t((int)(brightness * 255), (int)(p * 255), (int)(q * 255));
	}
	color_t manage_alpha(int alpha) {
		return color_t((*this)[0], (*this)[1], (*this)[2], alpha);
	}
	static color_t increase(color_t clr, int value, bool consider_alpha = false) {
		return color_t(clr.get_red() + value, clr.get_green() + value, clr.get_blue() + value, clr.get_alpha() + consider_alpha * value);
	}
private:
	unsigned char _color[4];
};
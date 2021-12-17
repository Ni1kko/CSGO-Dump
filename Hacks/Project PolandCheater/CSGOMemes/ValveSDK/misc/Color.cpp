#include "Color.hpp"

Color Color::Black(0, 0, 0, 255);
Color Color::White(255, 255, 255, 255);
Color Color::Red(255, 0, 0, 255);
Color Color::Green(0, 128, 0, 255);
Color Color::Blue(0, 0, 255, 255);

Color::Color()
{
    *((int *)this) = 0;
}
Color::Color(int _r, int _g, int _b)
{
    SetColor(_r, _g, _b, 0);
}
Color::Color(int _r, int _g, int _b, int _a)
{
    SetColor(_r, _g, _b, _a);
}
void Color::SetRawColor(int color32)
{
    *((int *)this) = color32;
}
int Color::GetRawColor() const
{
    return *((int *)this);
}
void Color::SetColor(int _r, int _g, int _b, int _a)
{
    _CColor[0] = (unsigned char)_r;
    _CColor[1] = (unsigned char)_g;
    _CColor[2] = (unsigned char)_b;
    _CColor[3] = (unsigned char)_a;
}
void Color::GetColor(int &_r, int &_g, int &_b, int &_a) const
{
    _r = _CColor[0];
    _g = _CColor[1];
    _b = _CColor[2];
    _a = _CColor[3];
}
bool Color::operator== (const Color &rhs) const
{
    return (*((int *)this) == *((int *)&rhs));
}
bool Color::operator!= (const Color &rhs) const
{
    return !(operator==(rhs));
}
Color& Color::operator=(const Color &rhs)
{
    SetRawColor(rhs.GetRawColor());
    return *this;
}

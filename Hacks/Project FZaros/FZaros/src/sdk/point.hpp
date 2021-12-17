#pragma once

class CPoint
{
public:
	int x, y;

	CPoint()
	{
		this->initialize(0, 0);
	}
	CPoint(int component)
	{
		this->initialize(component, component);
	}
	CPoint(int x, int y)
	{
		this->initialize(x, y);
	}

	void initialize(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	CPoint operator + (const CPoint &other)
	{
		return CPoint(this->x + other.x, this->y + other.y);
	}
	CPoint operator + (int component)
	{
		return CPoint(this->x + component, this->y + component);
	}
	CPoint operator - (const CPoint &other)
	{
		return CPoint(this->x - other.x, this->y - other.y);
	}
	CPoint operator - (int component)
	{
		return CPoint(this->x - component, this->y - component);
	}
	CPoint operator / (const CPoint &other)
	{
		return CPoint(this->x / other.x, this->y / other.y);
	}
	CPoint operator / (int component)
	{
		return CPoint(this->x / component, this->y / component);
	}
	CPoint operator * (const CPoint &other)
	{
		return CPoint(this->x * other.x, this->y * other.y);
	}
	CPoint operator * (int component)
	{
		return CPoint(this->x * component, this->y * component);
	}

	CPoint &operator += (const CPoint &other)
	{
		this->x += other.x;
		this->y += other.y;

		return *this;
	}
	CPoint &operator += (int component)
	{
		this->x += component;
		this->y += component;

		return *this;
	}
	CPoint &operator -= (const CPoint &other)
	{
		this->x -= other.x;
		this->y -= other.y;

		return *this;
	}
	CPoint &operator -= (int component)
	{
		this->x -= component;
		this->y -= component;

		return *this;
	}
	CPoint &operator /= (const CPoint &other)
	{
		this->x /= other.x;
		this->y /= other.y;

		return *this;
	}
	CPoint &operator /= (int component)
	{
		this->x /= component;
		this->y /= component;

		return *this;
	}
	CPoint &operator *= (const CPoint &other)
	{
		this->x *= other.x;
		this->y *= other.y;

		return *this;
	}
	CPoint &operator *= (int component)
	{
		this->x *= component;
		this->y *= component;

		return *this;
	}

};
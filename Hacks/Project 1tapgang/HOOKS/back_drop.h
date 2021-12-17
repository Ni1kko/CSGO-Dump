#pragma once

#include "../includes.h"
#include "../UTILS/render.h"

class Dot;
extern std::vector<Dot*> Dots;

class Dot
{
public:
	Dot(Vector p, Vector v)
	{
		RelativePosition = p;
		Velocity = v;
	}

	void Draw()
	{
		RENDER::DrawFilledRect(RelativePosition.x - 2, RelativePosition.y - 2, RelativePosition.x + 2, RelativePosition.y + 2, CColor(255, 255, 255, 255)); // замените на вашу функцию рендера прямоугольника
		auto t = std::find(Dots.begin(), Dots.end(), this);
		if (t == Dots.end()) return;
		for (auto i = t; i != Dots.end(); i++)
		{
			if ((*i) == this) continue;
			Vector Pos = RelativePosition;
			float Dist = Pos.DistTo((*i)->RelativePosition);
			if (Dist < 100)
			{
				Vector Dir = ((*i)->RelativePosition - RelativePosition).Normalize();

				RENDER::DrawLine(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, CColor(255, 255, 255, 255));
			}
		}
	}
	Vector RelativePosition = Vector(0, 0, 0);
	float Rotation = 0;
	float Size = 0;
	Vector Velocity;
};

namespace BackDrop
{
	void DrawBackDrop();
}
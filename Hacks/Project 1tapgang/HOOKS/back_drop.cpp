#include "back_drop.h"
#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../imgui/imgui.h"

std::vector<Dot*> Dots;

void BackDrop::DrawBackDrop()
{
	if (!SETTINGS::settings.menu_open)
		return;

	int width, height;
	INTERFACES::Engine->GetScreenSize(width, height);

	RENDER::DrawFilledRect(0, 0, width, height, CColor(0.f, 0.f, 0.f, 0.70f));

	int s = rand() % 24;

	if (s == 0)
		Dots.push_back(new Dot(Vector(rand() % (int)width, -16, 0), Vector((rand() % 3) - 3, rand() % 3 + 1, 0)));

	for (auto i = Dots.begin(); i < Dots.end();)
	{
		if ((*i)->RelativePosition.y < -20 || (*i)->RelativePosition.y > height + 20 || (*i)->RelativePosition.x < -20 || (*i)->RelativePosition.x > width + 20)
		{
			delete (*i);
			i = Dots.erase(i);
		}
		else
		{
			(*i)->RelativePosition = (*i)->RelativePosition + (*i)->Velocity * (0.3);
			i++;
		}
	}
	for (auto i = Dots.begin(); i < Dots.end(); i++)
		(*i)->Draw();
}
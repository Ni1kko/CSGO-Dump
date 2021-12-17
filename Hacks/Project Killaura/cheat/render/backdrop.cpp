#include "backdrop.h"
#include "../menu/menu.hpp"
#include "../../sdk/structs.hpp"

std::vector<Dot*> Dots;

void backdrop::render() {
	if (!Menu::Get().IsVisible()) return;

	static int width, height;
	g_EngineClient->GetScreenSize(width, height);

	int s = rand() % 130;

	if (s == 0) Dots.push_back(new Dot(Vector2D(rand() % (int)width, -16), Vector2D((rand() % 7) - 3, rand() % 3 + 1)));
	else if (s == 1) Dots.push_back(new Dot(Vector2D(rand() % (int)width, (int)height + 16), Vector2D((rand() % 7) - 3, -1 * (rand() % 3 + 1))));
	else if (s == 2) Dots.push_back(new Dot(Vector2D(-16, rand() % (int)height), Vector2D(rand() % 3 + 1, (rand() % 7) - 3)));
	else if (s == 3) Dots.push_back(new Dot(Vector2D((int)width + 16, rand() % (int)height), Vector2D(-1 * (rand() % 3 + 1), (rand() % 7) - 3)));

	for (auto i = Dots.begin(); i < Dots.end();) {
		if ((*i)->relative_position.y < -20 || (*i)->relative_position.y > height + 20 || (*i)->relative_position.x < -20 || (*i)->relative_position.x > width + 20) {
			delete (*i);
			i = Dots.erase(i);
		}
		else {
			(*i)->relative_position = (*i)->relative_position + (*i)->velocity * (0.3f);
			i++;
		}
	}
	for (auto i = Dots.begin(); i < Dots.end(); i++) (*i)->draw();
}
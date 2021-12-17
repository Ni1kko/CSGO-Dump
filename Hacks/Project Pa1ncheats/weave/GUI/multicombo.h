#pragma once
#include "element.h"

class c_multicombo : public c_element {
private:
	string label;
	void* value;
	float animation;
	float open_animation;
	float* animations;
	bool open;
	std::vector<string> elements;
	Vector2D pos;
	bool(*should_render)();
public:
	c_multicombo(string label, void* val, std::vector<string> el, bool(*should_render)() = nullptr) {
		this->label = label;
		this->value = val;
		this->elements = el;
		this->type = c_elementtype::combobox;
		this->animations = new float[elements.size()];
		for (int i = 0; i < elements.size(); i++)
			animations[i] = 0.f;
		this->open_animation = 0.f;
		this->should_render = should_render;
		this->open = false;
		animation = 0.f;
		open_animation = 0.f;
	}
	void change_pointer(void* ptr) { value = ptr; };
	bool hovered();
	bool update();
	void render();
	int get_total_offset();
};
#pragma once
#include "element.h"

class c_listbox : public c_element {
private:
	std::string label;
	float animation, press_animation;
	float height;
	bool(*should_render)();
	void* value;
	float scroll;
	float* animations;
	vector<string> elements;
	bool should_draw_scroll;
	float total_elements_size;
public:
	c_listbox(string str, void* value, vector<string> elements, float height, bool(*should_render)() = nullptr) {
		label = str;
		this->type = listbox;
		this->height = height;
		this->value = value;
		this->elements = elements;
		this->animation = 0.f;
		this->press_animation = 0.f;
		this->scroll = 0.f;
		this->animations = new float[elements.size()];
		for (int i = 0; i < elements.size(); i++)
			animations[i] = 0.f;
		this->should_render = should_render;
		should_draw_scroll = false;
		total_elements_size = elements.size() * 20.f;
	}
	void change_pointer(void* ptr) { };
	bool update();
	bool hovered();
	void render();
	void special_render() {};
	int get_total_offset() {
		if (should_render)
			if (!should_render())
				return 0;
		return height + 5 + (label.size() > 0) * after_text_offset;
	};
};
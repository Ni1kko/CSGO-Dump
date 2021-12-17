#pragma once
#include "element.h"

class c_checkbox : public c_element {
private:
	string label;
	float animation, press_animation;
	void* value;
	bool (*should_render)();
public:
	c_bind* bind;
	c_checkbox(string label, void* val, bool (*should_render)() = nullptr, c_bind* bind = nullptr) {
		this->label = label;
		this->value = val;
		this->type = c_elementtype::checkbox;
		this->binder.open = false;
		if (bind) {
			this->bind = bind;
			this->bind->key = 0;
			this->bind->type = 0;
		}
		else
			this->bind = nullptr;
		this->binder.active = false;
		this->should_render = should_render;
		for (int i = 0; i < 4; i++)
			binder.animations[i] = 0.f;
		press_animation = 0.f;
		animation = 0.f;
	}
	struct {
		bool open, active;
		float animations[4];
		float animation;
		vector<string> elements = { "disable bind", "hold", "toggle", "release" };
	} binder;
	void* get_ptr() { return value; };
	bool update();
	bool hovered();
	void render();
	void change_pointer(void* ptr) { value = ptr; };
	void special_render() {};
	int get_total_offset() {
		if (should_render)
			if (!should_render())
				return 0;
		return 15;
	};
};

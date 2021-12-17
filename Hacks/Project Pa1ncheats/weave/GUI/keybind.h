#pragma once
#include "element.h"
struct c_bind;
class c_keybind : public c_element {
private:
	string label;
	float animation;
	float reading_animation;
	
	bool(*should_render)();
public:
	c_bind* bind;
	c_keybind(string label, c_bind* bind, bool(*should_render)() = nullptr) {
		this->label = label;
		this->bind = bind;
		this->type = c_elementtype::keybind;
		this->binder.open = false;
		this->bind->key = 0;
		this->bind->type = 0;
		this->binder.active = false;
		this->should_render = should_render;
		for (int i = 0; i < 4; i++)
			binder.animations[i] = 0.f;
		animation = 0.f;
	}
	struct {
		bool open, active;
		float animations[4];
		float animation;
		vector<string> elements = { "always off", "hold", "toggle", "release", "always on" };
	} binder;
	bool update();
	bool hovered();
	void render();
	void change_pointer(void* ptr) { };
	void special_render() {};
	int get_total_offset() {
		if (should_render)
			if (!should_render())
				return 0;
		return 23;
	};
};

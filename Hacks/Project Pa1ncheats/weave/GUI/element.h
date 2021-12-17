#pragma once
#include "../DLL_MAIN.h"
#include "../render.h"

//using namespace std;

#define g_size 150
#define after_text_offset 15

extern float animation_speed;

//const int main_color_r = 255;
//const int main_color_g = 150;
//const int main_color_b = 0;
extern color_t main_color;

extern Vector2D g_mouse;

enum c_elementtype {
	window,
	child,
	text,
	dynamic_text,
	button,
	checkbox,
	slider,
	combobox,
	multi_combobox,
	colorpicker,
	keybind,
	input_text,
	listbox,
	bar,
};

class c_element {
public:
	int tab;
	c_elementtype type;
	c_element* child;
	virtual int get_total_offset() = 0;
	virtual void render() = 0;
	virtual bool update() = 0;
	virtual bool hovered() = 0;
};

//extern c_element* g_hovered_element;
//extern c_element* g_active_element;
//extern Vector2D g_active_element_pos;

extern const vector<string> KeyStrings;
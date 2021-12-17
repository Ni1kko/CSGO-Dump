#pragma once

#include "..\includes.hpp"

#include "..\sdk\misc\Color.hpp"
#include "..\sdk\interfaces\IInputSystem.hpp"

#include <functional>
#include <windows.h>
#include <Lmcons.h>

class C_GroupBox;
class C_Tab;

// ----------------------------------------------------------------------------------------

struct rect_t {
	int x, y, w, h;

	rect_t() { }

	rect_t(int n_x, int n_y, int n_w, int n_h) {
		x = n_x;
		y = n_y;
		w = n_w;
		h = n_h;
	}

	bool contains_point(POINT pnt) {
		return
			pnt.x > x &&
			pnt.y > y &&
			pnt.x < x + w &&
			pnt.y < y + h;
	}
};

// ----------------------------------------------------------------------------------------

enum ControlFlagTypes {
	CONTROLFL_KEYBIND,
	CONTROLFL_COLORSELECTOR
};

class C_ControlFlag {
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void click() = 0;

	C_GroupBox * parent;

	rect_t area;
};

// ----------------------------------------------------------------------------------------

enum {
	TAB_ANIMATION_UP,
	TAB_ANIMATION_DOWN
};

class menu {
private:
	struct Mouse_t {
		POINT cursor;
		void paint();
	};
	Mouse_t c_mouse;

public:
	void render();
	void update();

	menu();

	static menu & get() {
		static menu instance;
		return instance;
	}
public:
	bool
		open = false,
		mouse_enable,
		dragging;

	void toggle() {
		open = !open;
	}

	bool active() {
		return open;
	}

	int
		drag_x,
		drag_y;

	int selected_tab = 0;
	rect_t area;

	rect_t m_pos = rect_t(
		area.x - 74,
		area.y,
		area.w + 74, 1
	);

	std::vector< C_Tab* > tabs;

	bool
		animating,
		animate_direction;

	int
		animation_offset,
		animation_destination;

	void add_tab(C_Tab * tab) {
		tabs.push_back(tab);
	}

	bool m_keystate[256] = { };
	bool m_oldstate[256] = { };

	bool key_press(int key) {
		return m_keystate[key] && !m_oldstate[key];
	}

	POINT cursor() {
		return c_mouse.cursor;
	}
public:
	void poll_mouse();
	void poll_keyboard();
};

// ----------------------------------------------------------------------------------------

class C_Control {
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void click() = 0;

	std::vector< C_ControlFlag * > flags;

	C_GroupBox * parent;

	rect_t area;
	int height_offset = 0;
	int sub = -1;

	bool * override_function;
	bool last_override_state;

	void set_override(bool * n_override);
};

// ----------------------------------------------------------------------------------------

class C_Tab {
public:
	std::string name, icon;
	int index;
	std::vector< C_GroupBox * > group_boxes;
	menu * parent;

	C_Control * overriding_control;
	C_ControlFlag * overriding_flag;
	bool block_input, reset_inputblock, blocked_control_flag;

	struct subtab_t {
		char * name;
		int index;
		vgui::HFont font;
		bool chuj;

		subtab_t(char * _name, int _index);
		subtab_t(char * _name, int _index, vgui::HFont _font);
	};

	enum {
		SUBTAB_ANIMATION_LEFT,
		SUBTAB_ANIMATION_RIGHT
	};

	bool
		animating,
		animation_direction;

	float
		animation_offset,
		animation_destination;

	int selected_sub;
	std::vector< subtab_t > subtabs;

	void paint_subtabs();
	void update_subtabs();
public:
	void paint();
	void update();
	void add_group(C_GroupBox * gb) { group_boxes.push_back(gb); }

	C_Tab(menu * n_parent, int n_index, std::string n_icon, std::string n_name, std::vector< subtab_t > n_subtabs = { });
};

// ----------------------------------------------------------------------------------------

enum GroupRow {
	GROUP_LEFT,
	GROUP_RIGHT,
};

class C_GroupBox {
private:
	C_Tab * parent;
	float last_pos = 8;
	std::vector< C_Control * > controls;
	rect_t area;
	std::string name;

	struct SubData_t {
		int
			selected_sub = -1,
			last_sub;
		std::vector< int > sub_tabs;
	}subdata;

	int parent_sub;
public:
	int get_parent_sub() {
		return parent_sub;
	}

	C_Tab * get_tab() {
		return parent;
	}

	void reset_block() {
		parent->reset_inputblock = true;
	}

	void focus_control(C_Control * pointer) {
		parent->block_input = true;
		parent->reset_inputblock = false;
		parent->blocked_control_flag = false;
		parent->overriding_flag = nullptr;
		parent->overriding_control = pointer;
	}

	void focus_subcontrol(C_ControlFlag * pointer) {
		parent->block_input = true;
		parent->reset_inputblock = false;
		parent->blocked_control_flag = false;
		parent->overriding_flag = pointer;
		parent->overriding_control = nullptr;
	}
public:
	void render();
	void update(rect_t mparent_area);
	void update_subs();

	void reset_control_positions();
	void add_control(C_Control * new_control, bool first = true);

	C_GroupBox(GroupRow row, int y, std::string n_name, int sub_tab_count = 0);
};

// ----------------------------------------------------------------------------------------

class C_ColorSelector : public C_ControlFlag {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	Color color;
	Color * preview_color;
	float brightness = 1.0f;
	bool
		open,
		dragging;

	C_ColorSelector(Color * n_color, std::string n_label = "");
};

// ----------------------------------------------------------------------------------------

class C_CheckBox : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	bool * state;

	C_CheckBox(std::string n_label, bool * n_state, std::vector< C_ControlFlag * > n_flags = { });
};

// ----------------------------------------------------------------------------------------

class C_SliderInt : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	float min, max;
	int * value;
	std::string follower;
	bool dragging;

	C_SliderInt(std::string n_label, int * n_value, int n_min, int n_max, std::string n_follower, std::vector< C_ControlFlag * > n_flags = { });
};

// ----------------------------------------------------------------------------------------

class C_SliderFloat : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	float min, max;
	float * value;
	std::string follower;
	bool dragging;

	C_SliderFloat(std::string n_label, float * n_value, float n_min, float n_max, std::string n_follower, std::vector< C_ControlFlag * > n_flags = { });
};

// ----------------------------------------------------------------------------------------

class C_Dropdown : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	int * selected;
	std::vector< std::string > items;
	std::vector< std::string > * items_pointer;
	bool open = false, using_pointer;

	C_Dropdown(std::string n_label, int * n_selected, std::vector< std::string > n_items, std::vector< C_ControlFlag * > n_flags = { });
	C_Dropdown(std::string n_label, int * n_selected, std::vector< std::string > * n_items, std::vector< C_ControlFlag * > n_flags = { });
};

// ----------------------------------------------------------------------------------------

struct MultiDropdownItem_t {
	bool enabled;
	std::string name;

	MultiDropdownItem_t(bool n_enabled, std::string n_name) {
		enabled = n_enabled;
		name = n_name;
	}
};

class C_MultiDropdown : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	std::vector< MultiDropdownItem_t > * items_ptr;
	bool open = false;

	C_MultiDropdown(std::string n_label, std::vector< MultiDropdownItem_t > * n_items, std::vector< C_ControlFlag * > n_flags = { });
};

// ----------------------------------------------------------------------------------------

class C_Button : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	std::function< void() > func;
	C_Button(std::string n_label, std::function< void() > n_func = { });
};

// ----------------------------------------------------------------------------------------

class C_TextField : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	std::string * input;
	bool taking_input = false;
	float next_blink;

	C_TextField(std::string n_label, std::string * n_input);
};

// ----------------------------------------------------------------------------------------

class C_KeyBind : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	ButtonCode_t * key;
	bool taking_input;

	C_KeyBind(std::string n_label, ButtonCode_t * n_key);
};

// ----------------------------------------------------------------------------------------

enum TextSeperatorType {
	SEPERATOR_NORMAL,
	SEPERATOR_BOLD
};

class C_TextSeperator : public C_Control {
public:
	void draw();
	void update();
	void click();
public:
	std::string label;
	TextSeperatorType type;

	C_TextSeperator(std::string n_label, TextSeperatorType type, std::vector< C_ControlFlag * > n_flags = { });
};

void setup_main_menu();
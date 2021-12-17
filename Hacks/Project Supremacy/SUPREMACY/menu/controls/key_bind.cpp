#include "..\cmenu.hpp"
C_KeyBind::C_KeyBind(std::string n_label, ButtonCode_t * n_key) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	key = n_key;
	area.h = 3;
	if (n_label != "")
	{
		height_offset = 14;
		area.h += 14;
	}

	parent->add_control(this);
}
void C_KeyBind::draw()
{
	rect_t n_area = rect_t
	(
		area.x,
		area.y + height_offset,
		area.w,
		20
	);

	if (label != "") render::get().text(fonts[TAHOMA12], area.x, area.y, Color::Purple, HFONT_CENTERED_NONE, label.c_str());


	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(n_area.x, n_area.y - 15, n_area.w, 20);

	if (item_region.contains_point(mouse))
	{
		render::get().rect_filled(n_area.x, n_area.y - 15, n_area.w, 20, Color{ 65, 65, 65, 0 });
	}
	else render::get().rect_filled(n_area.x, n_area.y - 15, n_area.w, 20, Color{ 55, 55, 55, 0 });

	render::get().rect(n_area.x, n_area.y - 15, n_area.w, 20, Color{ 55, 55, 55, 0 });

	std::string to_render = g_csgo.m_inputsys()->ButtonCodeToString(*key);

	std::transform(to_render.begin(), to_render.end(), to_render.begin(), ::tolower);
	if (*key <= BUTTON_CODE_NONE)
	{
		to_render = " -- ";
	}
	if (taking_input)
	{
		to_render = " key ";
	}
	std::stringstream ss;

	ss << "[ " << to_render.c_str() << " ]";
	Color text_color = taking_input ? g_cfg.menu.menu_theme[0] : Color(153, 153, 153);

	render::get().text(fonts[TAHOMA12], n_area.x + 280, n_area.y + 3, text_color, HFONT_CENTERED_Y, ss.str().c_str());

}
void C_KeyBind::update() {
	if (!taking_input) {
		return;
	}
	parent->focus_control(this);
	for (int i = 0; i < 255; i++) {
		if (menu::get().key_press(i)) {
			*key = g_csgo.m_inputsys()->VirtualKeyToButtonCode(i);
			switch (i) {
			case 1: *key = MOUSE_LEFT; break;
			case 2: *key = MOUSE_RIGHT; break;
			case 4: *key = MOUSE_MIDDLE; break;
			case 5: *key = MOUSE_4; break;
			case 6: *key = MOUSE_5; break;
			}
			if (*key == KEY_ESCAPE) {
				*key = BUTTON_CODE_NONE;
				taking_input = false;
				parent->reset_block(); return;
			}
			taking_input = false;
			parent->reset_block(); return;
		}
	}
}
void C_KeyBind::click() {
	if (!taking_input) {
		taking_input = true;
	}
}
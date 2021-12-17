#include "..\cmenu.hpp"
C_Button::C_Button(std::string n_label, std::function< void() > n_func) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	func = n_func;
	area.h = 22;
	parent->add_control(this);
}
void C_Button::draw()
{
	POINT mouse;
	GetCursorPos(&mouse);

	rect_t item_region = rect_t(area.x, area.y, area.w, 22);
	render::get().rect_filled(area.x, area.y, area.w, 22, Color{ 55, 55, 55, 255 });
	if (item_region.contains_point(mouse))
	{
		render::get().rect_filled(area.x, area.y, area.w, 22, Color{ 105, 105, 105, 255 });
	}

	render::get().rect(area.x, area.y, area.w, 22, Color::Black);
	render::get().text(fonts[TAHOMA12], area.x + 100, area.y + 11, Color::Purple, HFONT_CENTERED_Y, label.c_str());
}
void C_Button::update() {
}
void C_Button::click() {
	func();
}
#include "..\cmenu.hpp"
C_CheckBox::C_CheckBox(std::string n_label, bool * n_state, std::vector< C_ControlFlag * > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	state = n_state;
	flags = n_flags;
	area.h = 9;
	parent->add_control(this);
}
void C_CheckBox::draw()
{
	static float rainbow;
	rainbow += g_cfg.misc.rainbowspeed * 0.0001;
	if (rainbow > 1.f)
		rainbow = 0.f;

	render::get().rect_filled(area.x, area.y, 9, 9, Color{ 62, 62, 62, 255 });
	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(area.x, area.y, 9, 9);
	if (item_region.contains_point(mouse)) {
		render::get().rect_filled(area.x, area.y, 9, 9, Color{ 72, 72, 72, 255 });
	}
	if (*state)
	{
		//render::get().text(fonts[SLIDERFONT], area.x + 1, area.y + 3, g_cfg.menu.menu_theme[0], HFONT_CENTERED_Y, u8"✓");
		render::get().rect_filled(area.x, area.y, 9, 9, g_cfg.menu.check_box_color);
	}
	render::get().rect(area.x, area.y, 9, 9, Color::Black);
	if (g_cfg.misc.RainbowBar)
	{
		render::get().text(fonts[TAHOMA12], area.x + 15, area.y + 5, Color::FromHSB(rainbow, 1.f, 1.f), HFONT_CENTERED_Y, label.c_str());
	}
	else
	{
		render::get().text(fonts[TAHOMA12], area.x + 15, area.y + 5, Color::Purple, HFONT_CENTERED_Y, label.c_str());
	}
}
void C_CheckBox::update() {
}
void C_CheckBox::click() {
	*state = !(*state);
}
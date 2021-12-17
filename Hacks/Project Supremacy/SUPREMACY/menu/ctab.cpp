#include "cmenu.hpp"
C_Tab::C_Tab(menu * n_parent, int n_index, std::string n_icon, std::string n_name, std::vector< subtab_t > n_subtabs) {
	parent = n_parent;
	index = n_index;
	icon = n_icon;
	name = n_name;
	selected_sub = -1;

	if (n_subtabs.size()) 
	{
		subtabs = n_subtabs;
		selected_sub = subtabs[0].index;
	}

	n_parent->add_tab(this);

	g_cfg.menu.tab_sub = -1;
	g_cfg.menu.last_tab = this;
}
C_Tab::subtab_t::subtab_t(char * _name, int _index) {
	name = _name;
	index = _index;
	font = fonts[esp];
}
C_Tab::subtab_t::subtab_t(char * _name, int _index, vgui::HFont _font) {
	name = _name;
	index = _index;
	font = _font;
}
void C_Tab::paint_subtabs()
{
	if (subtabs.size())
	{
		rect_t area = rect_t(
			menu::get().area.x + 8,
			menu::get().area.y + 8,
			menu::get().area.w - 16,
			15
		);
		float sub_width = area.w / subtabs.size();

		render::get().rect_filled(area.x, area.y, area.w, 26, Color(35, 35, 35, 255));
		//render::get().rect(area.x, area.y, area.w, area.h, Color::Black);
		render::get().rect_filled(area.x, area.y + 26, area.w, 4, Color(35, 35, 35, 255));

		render::get().rect_filled(area.x + (sub_width * selected_sub) + 1, area.y + 1, sub_width - 2, 25, Color(100, 100, 100));
		//render::get().rect_filled(area.x + animation_offset, area.y + 26, sub_width, 4, g_cfg.menu.menu_theme[0]);

		for (int i = 0; i < subtabs.size(); i++)
		{
			int text_x = area.x + (sub_width * i) + (sub_width / 2);

			int add = 0;

			if (subtabs[i].font == fonts[esp])
				add += 3;

			render::get().text(subtabs[i].font, text_x, area.y + 13 + add, selected_sub == subtabs[i].index ? Color::White : Color(153, 153, 153), HFONT_CENTERED_X | HFONT_CENTERED_Y, subtabs[i].name);
		}
	}
}
void C_Tab::update_subtabs() {
	if (subtabs.size()) {
		rect_t area = rect_t(
			menu::get().area.x + 8,
			menu::get().area.y + 8,
			menu::get().area.w - 16,
			30
		);
		float sub_width = area.w / subtabs.size();
		for (int i = 0; i < subtabs.size(); i++) {
			rect_t tab_area = rect_t(
				area.x + (sub_width * i),
				area.y, sub_width, 26
			);
			if (menu::get().key_press(VK_LBUTTON) && tab_area.contains_point(menu::get().cursor())) {
				if (!animating && selected_sub != subtabs[i].index) {
					animating = true;
					animation_direction = (subtabs[i].index < selected_sub) ? SUBTAB_ANIMATION_LEFT : SUBTAB_ANIMATION_RIGHT;
					animation_destination = tab_area.x;
					selected_sub = subtabs[i].index;
					reset_inputblock = true;
				}
			}
		}
		for (int i = 0; i < 9; i++) {
			if (!animating) break;
			if (animation_direction == SUBTAB_ANIMATION_RIGHT) {
				if ((area.x + animation_offset) < animation_destination) {
					animation_offset += 1;
					math::clamp(animation_offset, 0.0f, area.x - animation_destination);
				}
				else {
					animating = false;
				}
			}
			else {
				if ((area.x + animation_offset) > animation_destination) {
					animation_offset -= 1;	
					math::clamp(animation_offset, area.x - animation_destination, (float)area.w);
				}
				else {
					animating = false;
				}
			}
		}
	}
}
void C_Tab::paint()
{
	rect_t tab_area = rect_t
	(
		menu::get().area.x - 75,
		menu::get().area.y + (index * 15),
		75,
		15
	);
	rect_t tab_bar = rect_t
	(
		menu::get().area.x - 10,
		menu::get().area.y + menu::get().animation_offset,
		10,
		15
	);
	rect_t top2bottom = rect_t
	(
		menu::get().area.x,
		menu::get().area.y,
		1,
		menu::get().area.h
	);
	render::get().text(fonts[TAHOMA12], tab_area.x + 12, tab_area.y + 8, Color::Grey, HFONT_CENTERED_Y, name.c_str());
	if (index == menu::get().selected_tab)
	{
		static float rainbow;
		rainbow += 0.0005f;
		if (rainbow > 1.f)
			rainbow = 0.f;
		render::get().rect(top2bottom.x, top2bottom.y, top2bottom.w, top2bottom.h + 2, Color(48, 48, 48));
		render::get().rect(top2bottom.x - 1, top2bottom.y, top2bottom.w, top2bottom.h, Color::Black);

		render::get().rect_filled(tab_area.x, tab_area.y, tab_area.w, tab_area.h + 2, Color(35, 35, 35));
		render::get().text(fonts[TAHOMA12], tab_area.x + 12, tab_area.y + 8, g_cfg.menu.color_text_tab, HFONT_CENTERED_Y, name.c_str());


		paint_subtabs();
	}
}
void C_Tab::update()
{
	rect_t main_tab_area = rect_t(
		menu::get().area.x - 75, menu::get().area.y,
		75, menu::get().area.h
	);
	rect_t tab_area = rect_t(
		main_tab_area.x,
		main_tab_area.y + (index * 15),
		75, 15
	);
	if (tab_area.contains_point(menu::get().cursor()) && menu::get().key_press(VK_LBUTTON)) {
		if (!menu::get().animating && index != menu::get().selected_tab) {
			menu::get().animating = true;
			menu::get().animate_direction = (index > menu::get().selected_tab) ? TAB_ANIMATION_DOWN : TAB_ANIMATION_UP;
			menu::get().animation_destination = tab_area.y;
			menu::get().selected_tab = index;
		}
	}
	if (index == menu::get().selected_tab) {
		update_subtabs();
	}
	if (index == menu::get().selected_tab) {
		for (int c = group_boxes.size() - 1; c >= 0; c--) {
			if (group_boxes[c]->get_parent_sub() == selected_sub)
				group_boxes[c]->update(menu::get().area);
		}
	}
}
#include "..\cmenu.hpp"
#define gg 20
C_MultiDropdown::C_MultiDropdown(std::string n_label, std::vector< MultiDropdownItem_t > * n_items, std::vector< C_ControlFlag * > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	items_ptr = n_items;
	flags = n_flags;
	area.h = gg;
	if (n_label != "") {
		height_offset = 14;
		area.h += 20;
	}
	parent->add_control(this);
}
void C_MultiDropdown::draw() {
	std::vector< MultiDropdownItem_t > items = *items_ptr;
	rect_t n_area = rect_t(
		area.x,
		area.y + height_offset,
		area.w,
		gg
	);
	if (label != "") render::get().text(fonts[TAHOMA12], area.x, area.y, Color::Purple, HFONT_CENTERED_NONE, label.c_str());
	else render::get().text(fonts[TAHOMA12], area.x, area.y, Color::Purple, HFONT_CENTERED_NONE, "none");
	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(n_area.x, n_area.y, n_area.w, gg);
	if (item_region.contains_point(mouse)) {
		render::get().rect_filled(n_area.x, n_area.y, n_area.w, gg, Color{ 65, 65, 65, 255 });
	}
	else render::get().rect_filled(n_area.x, n_area.y, n_area.w, gg, Color{ 55, 55, 55, 255 });
	render::get().rect(n_area.x, n_area.y, n_area.w, gg, Color::Black);
	auto arrow = [](int x, int y) {
		for (auto i = 5; i >= 2; --i) {
			auto offset = 5 - i;
			render::get().line(x + offset, y + offset, x + offset + math::clamp(i - offset, 0, 5), y + offset, Color(153, 153, 153));
		}
	};
	arrow(n_area.x + n_area.w - 10, n_area.y + 9);
	if (!items.size()) { return; }
	std::string ss;
	for (auto n = 0; n < items.size(); ++n) {
		auto format_len = ss.length() < 13;
		auto first = ss.length() <= 0;
		if ((items[n].enabled && format_len)) {
			if (!first) {
				ss.append(", ");
			}
			ss.append(items[n].name);
		}
		else if (!format_len) {
			ss.append("...");
			break;
		}
	}
	if (ss.length() <= 0) {
		ss += "none";
	}
	render::get().text(fonts[esp], n_area.x + 10, n_area.y + 10, Color(153, 153, 153), HFONT_CENTERED_Y, "%s", ss.c_str());
	if (!open) {
		return;
	}
	n_area.y += 21;
	n_area.h -= gg;
	render::get().rect_filled(n_area.x, n_area.y, area.w, items.size() * gg, Color{ 55, 55, 55, 255 });
	for (int i = 0; i < items.size(); i++) {
		auto & current = items.at(i);
		rect_t item_region = rect_t(n_area.x, n_area.y + (gg * i), n_area.w, gg);
		if (item_region.contains_point(mouse)) {
			render::get().rect_filled(n_area.x, n_area.y + (gg * i), n_area.w, gg, Color{ 40, 40, 40, 255 });
		}
		if (current.enabled) {
			render::get().text(fonts[TAHOMA12], area.x + 10, n_area.y + (gg * i) + 10, g_cfg.menu.menu_theme[0], HFONT_CENTERED_Y, current.name.c_str());
		}
		else {
			render::get().text(fonts[TAHOMA12], area.x + 10, n_area.y + (gg * i) + 10, Color(153, 153, 153), HFONT_CENTERED_Y, current.name.c_str());
		}
	}
	render::get().rect(n_area.x, n_area.y, area.w, items.size() * gg, Color::Black);
}
void C_MultiDropdown::update() {
	std::vector< MultiDropdownItem_t > items = *items_ptr;
	if (!open || !items.size()) return;
	POINT mouse; GetCursorPos(&mouse);
	parent->focus_control(this);
	for (int i = 0; i < items.size(); i++) {
		rect_t item_region = rect_t(area.x, area.y + gg + (height_offset)+(i * gg), area.w, gg);
		if (menu::get().key_press(VK_LBUTTON) && item_region.contains_point(mouse)) { items[i].enabled = !items[i].enabled;
		}
	}
	rect_t full_area = rect_t(area.x, area.y + height_offset, area.w, area.h + (items.size() * gg));
	if (menu::get().key_press(VK_LBUTTON) && !full_area.contains_point(mouse)) {
		open = false;
		parent->reset_block();
	}
	*items_ptr = items;
}
void C_MultiDropdown::click() {
	if (open) parent->reset_block();
	open = !open;
}
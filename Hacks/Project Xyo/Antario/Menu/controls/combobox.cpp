#include "../menu.h"

void c_menu::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, 163, 20) && !click_rest)
	{
		name_selected = name;
		pressed = true;
		click_rest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, 163, 20))
		click_rest = false;

	if (pressed)
	{
		if (!rest)
			selected_opened = !selected_opened;
		rest = true;
	}
	else
		rest = false;

	if (name_selected == name)
		open = selected_opened;

	render::get().draw_text(x_offset + 25, y_offset - 3, render::get().menu_font, name.c_str(), false, color(255, 255, 255, 255));
	render::get().draw_filled_rect(x_offset + 25, y_offset + 14, 163, 20, color(95, 95, 95, 255));
	render::get().draw_outline(x_offset + 25, y_offset + 14, 163, 20, color(0, 0, 0, 255));

	if (open)
	{
		render::get().draw_line(x_offset + 25 + 163 - 6, y_offset + 14 + 11, x_offset + 25 + 163 - 11, y_offset + 14 + 11, color(179, 179, 179, 255));
		render::get().draw_line(x_offset + 25 + 163 - 7, y_offset + 14 + 10, x_offset + 25 + 163 - 10, y_offset + 14 + 10, color(179, 179, 179, 255));
		render::get().draw_line(x_offset + 25 + 163 - 8, y_offset + 14 + 9, x_offset + 25 + 163 - 9, y_offset + 14 + 9, color(179, 179, 179, 255));

		render::get().draw_filled_rect(x_offset + 25, y_offset + 14 + 21, 163, (size * 16), color(95, 95, 95, 255));
		render::get().draw_outline(x_offset + 25, y_offset + 14 + 21, 163, (size * 16), color(0, 0, 0, 255));

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 34 + i * 16, 163, 16))
				*item = i;

			if (i == *item)
				render::get().draw_text(x_offset + 25 + 10, y_offset + 14 + 23 + i * 16, render::get().menu_font, itemname[i].c_str(), false, color(255, 255, 255, 255));
			else
				render::get().draw_text(x_offset + 25 + 10, y_offset + 14 + 23 + i * 16, render::get().menu_font, itemname[i].c_str(), false, color(185, 185, 185, 255));
		}
	}
	else
	{
		render::get().draw_line(x_offset + 25 + 163 - 6, y_offset + 14 + 9, x_offset + 25 + 163 - 11, y_offset + 14 + 9, color(179, 179, 179, 255));
		render::get().draw_line(x_offset + 25 + 163 - 7, y_offset + 14 + 10, x_offset + 25 + 163 - 10, y_offset + 14 + 10, color(179, 179, 179, 255));
		render::get().draw_line(x_offset + 25 + 163 - 8, y_offset + 14 + 11, x_offset + 25 + 163 - 9, y_offset + 14 + 11, color(179, 179, 179, 255));
	}

	render::get().draw_text(x_offset + 35, y_offset + 19, render::get().menu_font, itemname[*item].c_str(), false, color(255, 255, 255, 255));

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
		y_offset += 42 + (size * 16);
	else
		y_offset += 41;
}
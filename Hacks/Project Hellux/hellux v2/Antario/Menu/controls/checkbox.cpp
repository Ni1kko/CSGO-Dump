#include "../menu.h"

void c_menu::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	static bool pressed = false;

	auto text_size = render::get().GetTextSize2(render::get().menu_font, name.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, 9 + text_size.right, 9))
	{
		if (pressed)
			*item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, 9 + text_size.right, 9) && !pressed)
		pressed = true;

	if (*item == true)
		render::get().draw_filled_rect(x_offset + 8, y_offset, 9, 9, color(85, 224, 111, 255));
	else
		render::get().draw_filled_rect(x_offset + 8, y_offset, 9, 9, color(95, 95, 95, 255));

	render::get().draw_outline(x_offset + 8, y_offset, 9, 9, color(0, 0, 0, 255));

	render::get().draw_text(x_offset + 25, y_offset - 3, render::get().menu_font, name.c_str(), false, color(255, 255, 255, 255));

	y_offset += 16;
}
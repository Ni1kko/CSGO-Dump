#include "../menu.h"

void c_menu::slider(int max, std::string name, int* item, std::string suffix, int step, bool color_slider, int color_r, int color_g, int color_b)
{
	if (groupbox_bottom <= y_offset + 30)
		return;

	float SliderSize = 75; // 163

	float _pixel_value = max / SliderSize;

	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(groupbox_width- 128, y_offset, SliderSize + 1, 13)) {
			*item = abs(mouse_pos().x - (groupbox_width- 128)) * _pixel_value;
		
		}

		static bool pressed_subtract = false;
		static bool pressed_add = false;

		/*
		subtract
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(groupbox_width - 140, y_offset, 13, 13))
		{
			if (pressed_subtract)
				*item -= step;
			pressed_subtract = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(groupbox_width - 140, y_offset, 13, 13) && !pressed_subtract)
			pressed_subtract = true;

		/*
		add
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(groupbox_width - 54, y_offset, 13, 13))
		{
			if (pressed_add)
				*item += step;
			pressed_add = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(groupbox_width - 54, y_offset, 13, 13) && !pressed_add)
			pressed_add = true;

		if (*item > max) {
			*item = max;
		}

		if (*item < 0) {
			*item = 0;
		}

	}

		std::string namedisplay;

		namedisplay += name.c_str();

		render::get().draw_text(x_offset + 7, y_offset - 3, render::get().menu_font, namedisplay.c_str(), false, color(255, 255, 255, MenuAlpha_Text));

		render::get().draw_filled_rect(groupbox_width - 128, y_offset, SliderSize, 13 / 2, color(50, 50, 50, MenuAlpha_Main));
		render::get().draw_filled_rect(groupbox_width - 128, y_offset + 13 / 2, SliderSize, 13 / 2, color(40, 40, 40, MenuAlpha_Main));
		if (*item)
		{
			if (*item == max) {
				render::get().draw_filled_rect(groupbox_width - 128, y_offset, SliderSize, 13 / 2, color_slider ? color(color_r, color_g, color_b, MenuAlpha_Main) : color(57, 79, 110, MenuAlpha_Main));
				render::get().draw_filled_rect(groupbox_width - 128, y_offset + 13 / 2, SliderSize, 13 / 2, color_slider ? color(color_r, color_g, color_b, MenuAlpha_Main) : color(48, 69, 100, MenuAlpha_Main));
			}
			else {
				render::get().draw_filled_rect(groupbox_width - 128, y_offset, (*item / _pixel_value), 13 / 2, color_slider ? color(color_r, color_g, color_b, MenuAlpha_Main) : color(57, 79, 110, MenuAlpha_Main));
				render::get().draw_filled_rect(groupbox_width - 128, y_offset + 13 / 2, (*item / _pixel_value), 13 / 2, color_slider ? color(color_r, color_g, color_b, MenuAlpha_Main) : color(48, 69, 100, MenuAlpha_Main));
			}
		}

		render::get().draw_filled_rect(groupbox_width - 140, y_offset, 13, 13 / 2, color(50, 50, 50, MenuAlpha_Main));
		render::get().draw_filled_rect(groupbox_width - 140, y_offset + 13 / 2, 13, 13 / 2, color(40, 40, 40, MenuAlpha_Main));
		render::get().draw_outline(groupbox_width - 140, y_offset, 13, 13, color(0, 0, 0, MenuAlpha_Main));
		render::get().draw_text(groupbox_width - 140 + (13 / 2) - 1, y_offset - 3, render::get().menu_font, "-", false, color(255, 255, 255, MenuAlpha_Text));

		render::get().draw_filled_rect(groupbox_width - 54, y_offset, 13, 13 / 2, color(50, 50, 50, MenuAlpha_Main));
		render::get().draw_filled_rect(groupbox_width - 54, y_offset + 13 / 2, 13, 13 / 2, color(40, 40, 40, MenuAlpha_Main));
		render::get().draw_outline(groupbox_width - 54, y_offset, 13, 13, color(0, 0, 0, MenuAlpha_Main));
		render::get().draw_text(groupbox_width - 54 - (14 / 2) + 9, y_offset - 2, render::get().menu_font, "+", false, color(255, 255, 255, MenuAlpha_Text));


		render::get().draw_filled_rect(groupbox_width - 34, y_offset, 30, 13 / 2, color(50, 50, 50, MenuAlpha_Main));
		render::get().draw_filled_rect(groupbox_width - 34, y_offset + 13 / 2, 30, 13 / 2, color(40, 40, 40, MenuAlpha_Main));
		render::get().draw_outline(groupbox_width - 34, y_offset, 30, 13, color(0, 0, 0, MenuAlpha_Main));

		render::get().draw_text(groupbox_width - 32, y_offset, render::get().tab_font, std::to_string(*item), false, color(255, 255, 255, MenuAlpha_Text));

		render::get().draw_outline(groupbox_width- 128, y_offset, SliderSize, 13, color(0, 0, 0, MenuAlpha_Main));

		//render::get().draw_text(x_offset + 22, y_offset + 10, render::get().menu_font, "-", false, color(255, 255, 255, MenuAlpha_Text));
		//render::get().draw_text(x_offset + comboboxsize + SliderSize + 12, y_offset + 11, render::get().menu_font, "+", false, color(255, 255, 255, MenuAlpha_Text));

		y_offset += 30;
}
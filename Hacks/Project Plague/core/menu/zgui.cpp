
// #===================================================================================#
// +                                                                                   +
// +  _   _   ____  ________ _____                                        (_)          +
// + | \ | | / __ \|__   __||  ____|  DO NOT edit    ____   __ _   _   _   _           +
// + |  \| || |  | |  | |   | |__    this file, it  |_  /  / _` | | | | | | |          +
// + | . ` || |  | |  | |   |  __|   was created by: / /  | (_| | | |_| | | |          +
// + | |\  || |__| |  | |   | |____                 /___|  \__, |  \__,_| |_|          +
// + |_| \_| \____/   |_|   |______|                        __/ |                      +
// +                                                       |___/    zgui_packer        +
// +                                                             by danielkrupinski    + 
// +   (https://github.com/zxvnme/zgui/blob/master/packer.py)       and zxvnme         +      
// #===================================================================================#

#include "zgui.hpp"

using namespace zgui::globals;
zgui::functions_t zgui::functions;
zgui::gui_window_context_t zgui::globals::window_ctx;

// ========================================================================
bool zgui::button(const char *id, const vec2 size) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    bool result = false;
    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2 + 1}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {size.x - 2, size.y - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", size});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y + global_config.item_spacing});

    utils::misc::push_font(font);

    return result;
}
// ========================================================================

// ========================================================================
bool zgui::tab_button(const char* id, const vec2 size, const bool toggled) {
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };

	const bool active = window_ctx.blocking == utils::hash::hash(id);

	bool result = false;
	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
		window_ctx.blocking = utils::hash::hash(id);
	}
	else if (active && !utils::input::key_down(VK_LBUTTON)) {
		window_ctx.blocking = 0;
		result = hovered;
	}

	int text_width, text_height;
	functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    if (toggled)
    {
        window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + size.y - 2}, zgui_render_type::zgui_filled_rect, global_colors.control_active_or_clicked, "", {size.x - 2, 1} });
        window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + size.y - 3}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {size.x - 2, 2} });
    }
    window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {size.x - 2, size.y - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", size });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y + global_config.item_spacing });

	utils::misc::push_font(font);

	return result;
}
// ========================================================================

// ========================================================================
void zgui::checkbox(const char *id, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    float control_height = text_height - 1;
    float control_width = text_height - 1;

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width + text_width + 4, control_height); !active && hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + control_width + 4, draw_pos.y - 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::combobox(const char *id, std::vector<std::string> items, int &value, const int size) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    int control_width = size;
    const int control_height = 15;

    value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 4, draw_pos.y + 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 3, draw_pos.y + 1 }, zgui_render_type::zgui_text, global_colors.color_text, items.at(value), vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {float(control_width) - 2.f, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {float(control_width), control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing});


    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height * (items.size() + 1)); !hovered && utils::input::key_pressed(VK_LBUTTON))
    {
        if (window_ctx.blocking == utils::hash::hash(id))
            window_ctx.blocking = 0;
    }
    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON)) {
		if (window_ctx.blocking == 0)
			window_ctx.blocking = utils::hash::hash(id);
		else if (window_ctx.blocking == utils::hash::hash(id))
			window_ctx.blocking = 0;
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                value = i - 1;
            }

            bool selected = value == i - 1;
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 3, draw_pos.y + (control_height - 1) * i + 1}, zgui_render_type::zgui_text, global_colors.color_text, items.at(i - 1), vec2{0, 0}, font});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + ((control_height - 1) * i) + 1}, zgui_render_type::zgui_filled_rect, hovered ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {float(control_width) - 2.f, control_height - 2}});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y + (control_height - 1) * i}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {float(control_width), control_height}});
        }
    }

    utils::misc::push_font(font);
}
// ========================================================================


// ========================================================================
void zgui::multi_combobox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 11;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 4, draw_pos.y + 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    std::string value_str;
    int text_width, text_height;

    for (auto &item_t : items) {
        if (*item_t.value) {
            if (value_str.length() > 0)
                value_str += ", ";

            value_str += item_t.name;
        }
    }

    functions.get_text_size(font, value_str.c_str(), text_width, text_height);
    if (text_width > control_width - 18) {
        value_str.resize(control_width / 10);
        value_str += " ...";
    }
    if (!value_str.length())
        value_str += "None";

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 3, draw_pos.y + 1 }, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height} });

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing});

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height * (items.size() + 1)); !hovered && utils::input::key_pressed(VK_LBUTTON))
    {
        if (window_ctx.blocking == utils::hash::hash(id))
            window_ctx.blocking = 0;
    }
    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON)) {
		if (window_ctx.blocking == 0)
			window_ctx.blocking = utils::hash::hash(id);
		else if (window_ctx.blocking == utils::hash::hash(id))
			window_ctx.blocking = 0;
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);
            const bool outofbounds = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1), control_width, control_height * i);
            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                *items[i - 1].value = !*items[i - 1].value;
            }
            bool selected = *items[i - 1].value >= 1;
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 3, draw_pos.y + (control_height - 1) * i + 1 }, zgui_render_type::zgui_text, global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + ((control_height - 1) * i) + 1}, zgui_render_type::zgui_filled_rect, (hovered || selected) ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y + (control_height - 1) * i}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});
        }
    }

    utils::misc::push_font(font);
}
// ========================================================================


// ========================================================================
void zgui::colorpicker(const char* id, float* value, const float pusher)
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 20;
	const int control_height = 12;
	int push_height = control_height - 20;

	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ window_ctx.position.x + cursor_pos.x + pusher, window_ctx.position.y + cursor_pos.y - 20 };

	color pickercol = { int(value[0] * 255.f),int(value[1] * 255.f),int(value[2] * 255.f),255 };

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, pickercol, "", {control_width - 2, control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height} });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + push_height + global_config.item_spacing });
	utils::misc::push_font(font);

    vec2 control_pos{ window_ctx.position.x + cursor_pos.x - 4, draw_pos.y + 20 };
    if (const bool hovered = utils::input::mouse_in_region(control_pos.x, control_pos.y, 108, 65); !hovered && utils::input::key_pressed(VK_LBUTTON)) {
        if (window_ctx.blocking == utils::hash::hash(id))
            window_ctx.blocking = 0;
    }
	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON)) {
		if (window_ctx.blocking == 0)
			window_ctx.blocking = utils::hash::hash(id);
		else if (window_ctx.blocking == utils::hash::hash(id))
			window_ctx.blocking = 0;
	}
	else if (window_ctx.blocking == utils::hash::hash(id)) {
		zgui::picker_slider(std::string("Red#" + id_split[0]).c_str(), 0.f, 1.f, value[0], value);
		zgui::picker_slider(std::string("Green#" + id_split[0]).c_str(), 0.f, 1.f, value[1], value);
		zgui::picker_slider(std::string("Blue#" + id_split[0]).c_str(), 0.f, 1.f, value[2], value);
		/*if (!utils::input::mouse_in_region(draw_pos.x - 4, draw_pos.y, control_width + 54, 68) && utils::input::key_pressed(VK_LBUTTON))
		{
			window_ctx.blocking = 0;
		}*/
	}
}
// ========================================================================


// ========================================================================
void zgui::begin_groupbox(std::string_view title, const vec2 size) {
	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };

	int text_width{}, text_height{};

	if (!title.empty())
		functions.get_text_size(font, title.data(), text_width, text_height);

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.blackOutline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, size.x - 2, size.y - 2, global_colors.color_groupbox_bg);

	if (!title.empty()) {
        functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 16, size.x - 2, 1, global_colors.blackOutline);
		functions.draw_text(draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + 2, global_colors.color_text, font, false, title.data());
	}


	window_ctx.next_cursor_pos = vec2{ cursor_pos.x, cursor_pos.y + size.y + 4 };

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + 5, cursor_pos.y + 25 });

	utils::misc::push_font(font);
}

// ========================================================================
void zgui::end_groupbox() {
    utils::misc::push_cursor_pos(window_ctx.next_cursor_pos);
    window_ctx.next_cursor_pos = {};
}
// ========================================================================


// ========================================================================
void zgui::key_bind(const char *id, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 78;
    const int control_height = 14;

    value = std::clamp(value, 0, 255);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    /*const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }*/

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1 + control_width + 7, draw_pos.y + 2 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        for (int i = 0; i < 256; i++) {
            if (utils::input::key_pressed(i)) {
				if (keys_list[i] != "None")
					value = i;
				else
					value = 0;

                window_ctx.blocking = 0;
            }
        }
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 1}, zgui_render_type::zgui_text, global_colors.color_text, active ? "..." : keys_list[value].data(), vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width, control_height}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width + 2, control_height + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing + 2 });

    utils::misc::push_font(font);
}
// ========================================================================



// ========================================================================
void zgui::listbox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 120;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    for (int i = 1; i <= items.size(); i++) {
        const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * (i - 1), control_width, control_height);

        if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
            window_ctx.blocking = 0;
            *items[i - 1].value = !*items[i - 1].value;
        }
        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * (i - 1) + 4}, zgui_render_type::zgui_text, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
    }


    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, static_cast<float>(control_height * items.size() - 2)}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, static_cast<float>(control_height * items.size())}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) + control_height * (items.size() - 1)});

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::slider_int(const char *id, const int min, const int max, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const int control_width = 100;
    const int control_height = 11;

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 4, draw_pos.y - 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    float dynamic_width = std::clamp((float(value) - min) / (max - min) * control_width, 0.f, float(control_width) - 2.f);

    int text_width, text_height;
    std::string value_str = std::to_string(value);
    functions.get_text_size(font, value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 3)
        text_x = 3;

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + text_x, draw_pos.y + 1}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font});

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_active_or_clicked, "", {dynamic_width, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});


    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height  + global_config.item_spacing});

    utils::misc::push_font(font);
}
void zgui::picker_slider(const char* id, const int min, const int max, float& value, float* slidercol) {
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const unsigned long font = utils::misc::pop_font();

	const int control_width = 100;
	const int control_height = 11;

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 4, draw_pos.y - 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });
	const bool hovered = utils::input::mouse_in_region(draw_pos.x - 2, draw_pos.y, control_width + 4, control_height);
	if (utils::input::key_down(VK_LBUTTON) && hovered) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width))* (max - min) + min);

        value = value_mapped;
	}

    float dynamic_width = std::clamp((float(value) - min) / (max - min) * control_width, 0.f, float(control_width) - 2.f);

	int text_width, text_height; 
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string value_str = ss.str();
	functions.get_text_size(font, value_str.c_str(), text_width, text_height);

	int text_x = dynamic_width - text_width;

    if (text_x < 3)
        text_x = 3;

    window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + text_x, draw_pos.y + 1}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font });

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, {int(slidercol[0]*255.f),int(slidercol[1]*255.f),int(slidercol[2]*255.f),255}, "", {static_cast<float>(dynamic_width), control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height} });


	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing });

	utils::misc::push_font(font);
}
// ========================================================================


// ========================================================================
void zgui::slider_float(const char *id, const float min, const float max, float &value, const int decimales) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 11;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 4, draw_pos.y - 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    float dynamic_width = std::clamp((float(value) - min) / (max - min) * control_width, 0.f, float(control_width) - 2.f);

    int text_width, text_height;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(decimales) << value;
    std::string value_str = ss.str();
    functions.get_text_size(font, value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 3)
        text_x = 3;

    window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + text_x, draw_pos.y + 1}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font });

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_active_or_clicked, "", {static_cast<float>(dynamic_width), control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height} });


	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing });


    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool zgui::clickable_text(const char *id, const bool toggled) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 1, window_ctx.position.y + cursor_pos.y - 1};

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, (toggled || window_ctx.blocking == utils::hash::hash(id)) ? global_colors.control_active_or_clicked : global_colors.color_text, id_split[0], vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + 7 + global_config.item_spacing});

    bool result = false;

    if (!active && hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    utils::misc::push_font(font);

    return result;
}

// ========================================================================
void zgui::text(const char *text) {
    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 1, window_ctx.position.y + cursor_pos.y - 1};

    int text_width, text_height;
    functions.get_text_size(font, text, text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, global_colors.color_text, text, vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + 7 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::text_input(const char *id, std::string &value, const int max_length, const int flags) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 150;
    const int control_height = 14;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    /*const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }*/

	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + control_width + 7, draw_pos.y + 1 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

    if (hovered && utils::input::key_pressed(VK_LBUTTON) && !active && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        if (utils::input::key_pressed(VK_ESCAPE) || utils::input::key_pressed(VK_RETURN) || (!hovered && utils::input::key_pressed(VK_LBUTTON))) {
            window_ctx.blocking = 0;
        }
        else if (utils::input::key_pressed(VK_BACK) && !value.empty()) {
            value.pop_back();
        }
        else if (value.length() < max_length) {
            for (int i = 32; i <= 222; i++) {
                if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
                    continue;

                if (i > 57 && i <= 90) {
                    if (utils::input::key_pressed(i))
                        value += utils::input::key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
                }
                else {
                    if (utils::input::key_pressed(i)) {
                        for (int j = 0; j < sizeof(special_characters); j++) {
                            if (special_characters[j].vk == i)
                                value += utils::input::key_down(VK_SHIFT) ? special_characters[j].shift : special_characters[j].regular;
                        }
                    }
                }
            }
        }
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 1}, zgui_render_type::zgui_text, global_colors.color_text, flags & zgui_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str(), vec2{0, 0}, font});
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width, control_height} });
	window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width + 2, control_height + 2} });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height + global_config.item_spacing + 2 });

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::toggle_button(const char *id, const vec2 size, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", size});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {size.x + 2, size.y + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags) {
	if (!input_loop_started)
		throw std::exception("Input loop didnt start or didnt start properly.");;

	if (!(flags & zgui_window_flags_always_open)) {
		if (utils::input::key_pressed(global_config.menu_toggle_key))
			window_ctx.opened = !window_ctx.opened;
	}
	else
		window_ctx.opened = true;

	if (const int prev_alpha = window_ctx.alpha; !(flags & zgui_window_flags_no_ontoggle_animation)) {
		const int fade_factor = static_cast<int>(1.0f / 0.15f * functions.get_frametime() * 255);
		window_ctx.alpha = std::clamp(window_ctx.alpha + (window_ctx.opened ? fade_factor : -fade_factor), 0, 255);

		if (window_ctx.alpha != prev_alpha) {
			global_colors.window_border_fill.a = window_ctx.alpha;
			global_colors.window_border_color.a = window_ctx.alpha;
			global_colors.window_background.a = window_ctx.alpha;

			global_colors.color_text.a = window_ctx.alpha;
			global_colors.color_text_dimmer.a = window_ctx.alpha;

			global_colors.control_outline.a = window_ctx.alpha;
			global_colors.control_active_or_clicked.a = window_ctx.alpha;
			global_colors.control_idle.a = window_ctx.alpha;

			global_colors.color_groupbox_bg.a = window_ctx.alpha;
			global_colors.blackOutline.a = window_ctx.alpha;
		}
	}

	if (window_ctx.opened || window_ctx.alpha > 0) {
		if (!(flags & zgui_window_flags_no_move)) {
			if (utils::input::mouse_in_region(window_ctx.position.x + 1, window_ctx.position.y + 1, window_ctx.size.x - 2, 15)
				&& utils::input::key_pressed(VK_LBUTTON) && !window_ctx.dragging) {
				window_ctx.dragging = true;
			}
			else if (utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
				const vec2 mouse_delta{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y };
				const vec2 new_position{ window_ctx.position.x + mouse_delta.x, window_ctx.position.y + mouse_delta.y };

				window_ctx.position = new_position;
			}
			else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
				window_ctx.dragging = false;
			}
		}

		if (window_ctx.size.x < 1 && window_ctx.size.y < 1)
			window_ctx.size = default_size;

        functions.draw_filled_rect(window_ctx.position.x, window_ctx.position.y, window_ctx.size.x, window_ctx.size.y, global_colors.window_border_color);
        functions.draw_filled_rect(window_ctx.position.x + 1, window_ctx.position.y + 1, window_ctx.size.x - 2, window_ctx.size.y - 2, global_colors.window_border_fill);
       
        functions.draw_text(window_ctx.position.x + 5, window_ctx.position.y + 2, global_colors.color_text, font, false, title.data());

        functions.draw_filled_rect(window_ctx.position.x + 5, window_ctx.position.y + 16, window_ctx.size.x - 10, window_ctx.size.y - 21, global_colors.window_border_color);
        functions.draw_filled_rect(window_ctx.position.x + 6, window_ctx.position.y + 17, window_ctx.size.x - 12, window_ctx.size.y - 23, global_colors.window_background);

		utils::misc::push_font(font);
		utils::misc::push_cursor_pos(global_config.base_pos);
	}

	return window_ctx.opened || window_ctx.alpha > 0;
}

// ========================================================================
void zgui::end_window() {
    for (int i = window_ctx.render.size() - 1; i >= 0; i--) {
        switch (window_ctx.render[i].render_type) {
            case zgui_render_type::zgui_line:
                functions.draw_line(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_rect:
                functions.draw_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_filled_rect:
                functions.draw_filled_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_text:
                functions.draw_text(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].color, window_ctx.render[i].font, false, window_ctx.render[i].text.c_str());
                break;
        }
    }

    window_ctx.render.clear();

    while (!window_ctx.cursor_pos.empty())
        window_ctx.cursor_pos.pop();
}
// ========================================================================

// ========================================================================
void zgui::next_column(const int pusher_x, const int pusher_y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{ global_config.base_pos.x + pusher_x, global_config.base_pos.y + pusher_y};

    utils::misc::push_cursor_pos(new_cursor_pos);
}
void zgui::fix_spacing(const int pusher_y) {
	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + pusher_y });
}
// ========================================================================
std::vector<std::string> zgui::utils::hash::split_str(const char *str, const char separator) {
    std::vector<std::string> output;
    std::string substring;
    std::istringstream stream{str};

    while (std::getline(stream, substring, separator))
        output.push_back(substring);

    return output;
}

// ========================================================================
uint32_t zgui::utils::hash::hash(const char *str, const uint32_t value) {
    return *str ? hash(str + 1, (value ^ *str) * 0x1000193ull) : value;
}
// ========================================================================

// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(std::string_view window_name) {
    if (window_name.empty())
        throw std::exception("No window from where input should be read from specified in function parameter.");

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = GetAsyncKeyState(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(FindWindowA(nullptr, window_name.data()), &p_mouse_pos);
    previous_mouse_pos = mouse_pos;
    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(HWND hwnd) {
    if (!hwnd)
        throw std::exception("No window from where input should be read from specified in function parameter.");

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = GetAsyncKeyState(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(hwnd, &p_mouse_pos);
    previous_mouse_pos = mouse_pos;

    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
bool zgui::utils::input::key_pressed(const int key) {
    return key_state[key] && !prev_key_state[key];
}

// ========================================================================
bool zgui::utils::input::key_down(const int key) {
    return key_state[key];
}

// ========================================================================
bool zgui::utils::input::key_released(const int key) {
    return !key_state[key] && prev_key_state[key];
}

// ========================================================================
// Check if mouse is hovered over specified region.
bool zgui::utils::input::mouse_in_region(const int x, const int y, const int w, const int h) {
    return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}
// ========================================================================

// ========================================================================
// Push cursor position to the stack defined in window context.
void zgui::utils::misc::push_cursor_pos(const vec2 pos) {
    window_ctx.cursor_pos.push(pos);
}

// ========================================================================
// Pop cursor position from the stack defined in window context.
zgui::vec2 zgui::utils::misc::pop_cursor_pos() {
    const vec2 pos = window_ctx.cursor_pos.top();
    window_ctx.cursor_pos.pop();
    return pos;
}

// ========================================================================
// Pop font from the stack defined in window context.
void zgui::utils::misc::push_font(const unsigned long font) {
    window_ctx.fonts.push(font);
}

// ========================================================================
// Push font to the stack defined in window context.
unsigned long zgui::utils::misc::pop_font() {
    const unsigned long font = window_ctx.fonts.top();
    window_ctx.fonts.pop();
    return font;
}
// ========================================================================
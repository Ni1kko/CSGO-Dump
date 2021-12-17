#include "child.h"
#include "window.h"
#include "../menu.h"

c_element* c_child::get_parent() {
	return this->child;
}
bool c_child::hovered() {
	auto pos = ((c_window*)child)->get_cursor_position() + get_position();
	auto size = this->get_size();
	bool m = scroll.value < total_elements_size - size.y + 20.f;
	return g_mouse.x > pos.x && g_mouse.y > pos.y + (scroll.value > 0) * 20.f
		&& g_mouse.x < pos.x + size.x&& g_mouse.y < pos.y + size.y - m * 20.f;
}

void c_child::initialize_elements()
{
	for (auto &e : elements) {
		e->child = this;
		e->tab = this->tab;
	}
}

void c_child::lock_bounds() {
	auto pos = ((c_window*)child)->get_cursor_position() + get_position();
	auto size = this->get_size();
	auto m = Vector2D(pos.x + size.x, pos.y + size.y);
	g_Render->_drawList->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(m.x, m.y), true);
}
void c_child::unlock_bounds() {
	g_Render->_drawList->PopClipRect();
}
void c_child::render() {
	c_window* wnd = (c_window*)child;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return;
	auto pos = wnd->get_cursor_position() + get_position();
	auto size = get_size();
	int alpha = (int)(wnd->get_transparency() * 2.55f);
	
	auto clr = color_t(60, 60, 60, alpha);
	
	g_Render->FilledRect(pos.x, pos.y, size.x, size.y, color_t(22, 22, 22, (int)(wnd->get_transparency() * 2.55f)));

	Vector2D backup_cursor_pos = Vector2D(pos.x + 45, pos.y + 15);
	
	if (should_draw_scroll)
		backup_cursor_pos.y -= scroll.value;
	
	set_cursor_position(backup_cursor_pos);
	if (!scroll.dragging)
		update_elements();

	set_cursor_position(backup_cursor_pos);
	lock_bounds();
	render_elements();
	unlock_bounds();
	int max_scroll_value = total_elements_size - size.y + 20.f;
	if (should_draw_scroll) {

		//g_Render->FilledRect(pos.x + size.x - 9, pos.y + 1, 8, size.y - 2, color_t(32, 32, 32, alpha));
		if (scroll.value > 0) {
			g_Render->filled_rect_gradient(pos.x, pos.y, size.x, 20,
				color_t(25, 25, 25, alpha), color_t(25, 25, 25, alpha),
				color_t(25, 25, 25, 0), color_t(25, 25, 25, 0));
			Vector2D base = Vector2D(pos.x + size.x - 10, pos.y + 6);
			auto new_alpha = alpha * (scroll.value / max_scroll_value);
			g_Render->_drawList->AddTriangleFilled(
				ImVec2(base.x, base.y - 3), ImVec2(base.x - 3, base.y + 3.5f), ImVec2(base.x + 3, base.y + 3.5f),
				color_t(60, 60, 60, new_alpha).u32());
		}
		if (scroll.value < max_scroll_value) {
			g_Render->filled_rect_gradient(pos.x, pos.y + size.y - 20, size.x, 20,
				color_t(25, 25, 25, 0), color_t(25, 25, 25, 0),
				color_t(25, 25, 25, alpha), color_t(25, 25, 25, alpha));

			Vector2D base = Vector2D(pos.x + size.x - 10, pos.y + size.y - 6);
			auto new_alpha = alpha * (1.f - (scroll.value / max_scroll_value));
			g_Render->_drawList->AddTriangleFilled(
				ImVec2(base.x, base.y + 3.5f), ImVec2(base.x - 3.5f, base.y - 3.5f), ImVec2(base.x + 3.5f, base.y - 3.5f),
				color_t(60, 60, 60, new_alpha).u32());
		}

		auto scroll_pos = scroll.value / total_elements_size * (size.y - 40.f);
		auto scroll_pos_max = max_scroll_value / (float)total_elements_size * (size.y - 40.f);

		g_Render->FilledRect(pos.x + size.x - 5, pos.y + scroll_pos, 3, size.y - scroll_pos_max,
			color_t(60 + scroll.hovering_animation * 25.f, 60 + scroll.hovering_animation * 25.f, 60 + scroll.hovering_animation * 25.f, alpha));
	}

	g_Render->Rect(pos.x, pos.y, size.x, size.y, clr);
	g_Render->DrawString(pos.x + 10, pos.y, color_t(255, 255, 255, alpha),
		render::centered_y | render::centered_x, fonts::menu_main, get_title().c_str(), total_elements_size, scroll);
}
int c_child::get_total_offset() { return (int)get_size().y + 10; }
bool c_child::update()
{
	c_window* wnd = (c_window*)child;
	if (g_active_element) {
		if (g_active_element != this)
			return false;
	}
	auto pos = wnd->get_cursor_position() + get_position();
	auto size = get_size();
	bool hvrd = g_mouse.x > pos.x && g_mouse.y > pos.y
		&& g_mouse.x < pos.x + size.x&& g_mouse.y < pos.y + size.y;
	should_draw_scroll = total_elements_size > size.y - 20.f;
	bool hovering_listbox = wnd->g_hovered_element && wnd->g_hovered_element->type == c_elementtype::listbox;
	if (hvrd && !wnd->g_active_element && should_draw_scroll && !hovering_listbox) {
		int accelerator = 0;
		while (csgo->scroll_amount > 0) {
			scroll.value -= 15 + accelerator;
			csgo->scroll_amount--;
			accelerator++;
		}
		accelerator = 0;
		while (csgo->scroll_amount < 0) {
			scroll.value += 15 + accelerator;
			csgo->scroll_amount++;
			accelerator++;
		}
	}

	if (should_draw_scroll && !hovering_listbox) {
		/*if (scroll.animation > 0) {
			scroll.value += int(animation_speed * 75.f);
			scroll.animation -= int(animation_speed * 75.f);
		}
		else if (scroll.animation < 0) {
			scroll.value -= int(animation_speed * 75.f);
			scroll.animation += int(animation_speed * 75.f);
		}*/
		scroll.value = clamp(scroll.value, 0.f, clamp(total_elements_size - size.y + 20.f, 0.f, FLT_MAX));

		int max_scroll_value = total_elements_size - size.y + 20.f;
		auto scroll_pos = scroll.value / total_elements_size * (size.y - 40.f);
		auto scroll_pos_max = max_scroll_value / (float)total_elements_size * (size.y - 40.f);

		bool h_scroll = g_mouse.x > pos.x + size.x - 8 && g_mouse.y > pos.y + scroll_pos
			&& g_mouse.x < pos.x + size.x - 2 && g_mouse.y < pos.y + scroll_pos + size.y - scroll_pos_max;
		if (h_scroll) {
			if (scroll.hovering_animation < 1.f)
				scroll.hovering_animation += animation_speed;
			wnd->g_hovered_element = this;


			if (wnd->is_holding()) {
				scroll.dragging = true;
				
			}
			if (scroll.dragging && wnd->is_click()) {
				scroll.clicked_pos_y = ImGui::GetIO().MouseClickedPos[0].y;
			}
			if (!wnd->is_holding())
				scroll.dragging = false;
		}
		else {
			if (wnd->is_holding() && scroll.dragging)
				scroll.dragging = true;
			else {
				if (!wnd->is_holding())
					scroll.dragging = false;
				if (wnd->g_hovered_element == this)
					wnd->g_hovered_element = nullptr;
				if (scroll.hovering_animation > 0.f)
					scroll.hovering_animation -= animation_speed;
			}
		}
		if (scroll.dragging) {
			scroll.value += ImGui::GetIO().MouseDelta.y * (total_elements_size / (size.y - 40.f));
			wnd->g_active_element = this;
		}
		else {
			if (wnd->g_active_element == this)
				wnd->g_active_element = nullptr;
		}

		scroll.value = clamp(scroll.value, 0.f, clamp(total_elements_size - size.y + 20.f, 0.f, FLT_MAX));
	}
	else
		scroll.value = 0.f;
	scroll.hovering_animation = clamp(scroll.hovering_animation, 0.f, 1.f);
	set_transparency(wnd->get_transparency());
	return false;
}
void c_child::update_elements()
{
	total_elements_size = 0;
	bool b = false;
	for (auto e : elements) {
		if (e->tab != ((c_window*)child)->get_active_tab_index())
			continue;
		if (!b)
			e->update();
		int offset = e->get_total_offset();
		if (offset > 0) {
			apply_element(offset + 3);
			total_elements_size += offset + 3;
		}

		if (e == ((c_window*)child)->g_active_element)
			b = true;
	}
}
void c_child::render_elements()
{
	auto wnd = (c_window*)child;
	for (auto e : elements) {
		if (e->tab != wnd->get_active_tab_index())
			continue;
		if (e == ((c_window*)child)->g_active_element) {
			((c_window*)child)->g_active_element_pos = get_cursor_position();
			((c_window*)child)->active_element_parent = this;
			int offset = e->get_total_offset();
			if (offset > 0)
				apply_element(offset + 3);
		}
		else {
			auto cursor = get_cursor_position();
			auto chld_pos = wnd->get_cursor_position() + get_position();
			auto size_y = e->get_total_offset();
			if (cursor.y >= chld_pos.y - size_y
				&& cursor.y <= chld_pos.y + get_size().y)
				e->render();
			int offset = e->get_total_offset();
			if (offset > 0)
				apply_element(offset + 3);
		}
	}
};
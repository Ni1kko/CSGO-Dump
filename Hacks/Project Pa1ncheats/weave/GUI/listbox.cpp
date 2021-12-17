#include "listbox.h"
#include "window.h"
#include "child.h"
#include "../menu.h"

bool c_listbox::update()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c) return false;
	auto wnd = (c_window*)c->get_parent();
	if (!wnd)
		return false;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return false;
	if (wnd->g_active_element != this && wnd->g_active_element != nullptr) return false;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	if (label.size() > 0) pos.y += after_text_offset;
	auto alpha = (int)(c->get_transparency() * 2.55f);
	auto size = Vector2D(g_size, height);
	bool h = hovered();
	should_draw_scroll = total_elements_size > size.y;
	if (h) {
		wnd->g_hovered_element = this;
		while (csgo->scroll_amount > 0) {
			scroll -= 10;
			csgo->scroll_amount--;
		}
		while (csgo->scroll_amount < 0) {
			scroll += 10;
			csgo->scroll_amount++;
		}
	}
	
	if (should_draw_scroll) {
		scroll = clamp(scroll, 0.f, clamp(total_elements_size - size.y, 0.f, FLT_MAX));
	}
	else
		scroll = 0.f;
	if (should_draw_scroll)
		pos.y -= scroll;
	size = Vector2D(g_size, 20.f);
	for (int i = 0; i < elements.size(); i++) {
			
		bool hvrd = g_mouse.x > pos.x && g_mouse.y >= pos.y
			&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y;
		if (hvrd) {
			if (animations[i] < 1.f) animations[i] += animation_speed;
		}
		else {
			if (animations[i] > 0.f) animations[i] -= animation_speed;
		}
		if (hvrd && c->hovered() && wnd->is_click()) {
			*(unsigned int*)value = i;
			return true;
		}
		animations[i] = clamp(animations[i], 0.f, 1.f);
		pos.y += 20;

	}

	


	if (press_animation > 0.f)
		press_animation -= animation_speed;
	if (h) {
		if (animation < 1.f) animation += animation_speed;
	}
	else {
		if (animation > 0.f) animation -= animation_speed;
	}
	
	animation = clamp(animation, 0.f, 1.f);
	press_animation = clamp(press_animation, 0.f, 1.f);
	return false;
}
bool c_listbox::hovered()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c->hovered())
		return false;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	if (label.size() > 0) pos.y += after_text_offset;
	auto size = Vector2D(g_size, height);
	return g_mouse.x > pos.x && g_mouse.y > pos.y
		&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y;
}
void c_listbox::render() {
	if (should_render)
		if (!should_render())
			return;
	c_child* c = (c_child*)child;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto size = Vector2D(g_size, height);
	auto wnd = (c_window*)c->get_parent();
	if (!wnd)
		return;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return;

	auto alpha = (int)(wnd->get_transparency() * 2.55f);
	auto clr = color_t(50 + animation * 10.f, 50 + animation * 10.f, 50 + animation * 10.f, alpha * (1 - press_animation));
	auto clr2 = color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, alpha * (1 - press_animation));
	if (label.size() > 0) {
		g_Render->DrawString(pos.x, pos.y, color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha),
			 render::none, fonts::menu_desc, label.c_str());
		pos.y += after_text_offset;
	}
	g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, size.y + 2, color_t(0, 0, 0, alpha));
	g_Render->_drawList->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), true);
	g_Render->FilledRect(pos.x, pos.y, size.x, size.y,
		color_t(50, 50, 50, alpha));
	if (should_draw_scroll)
		pos.y -= scroll;
	for (int i = 0; i < elements.size(); i++) {
		g_Render->FilledRect(pos.x, pos.y, size.x, 20,
			color_t(50 + 10.f * animations[i], 50 + 10.f * animations[i], 50 + 10.f * animations[i], alpha));

		g_Render->DrawString(pos.x + 10, pos.y + 10,
			*(unsigned int*)value == i ? color_t(100 + 55.f * animations[i], 100 + 50.f * animations[i], 255.f - 25.f * animations[i], alpha) : 
			color_t(200 + 55 * animations[i], 200 + 55 * animations[i], 200 + 55 * animations[i], alpha), render::centered_y,

			fonts::menu_desc, elements[i].c_str());
		pos.y += 20;
	}
	if (should_draw_scroll)
		pos.y += scroll;
	pos.y -= 20 * (elements.size());
	if (scroll > 0)
		g_Render->filled_rect_gradient(pos.x, pos.y, size.x, 15,
			color_t(35, 35, 35, alpha), color_t(35, 35, 35, alpha),
			color_t(35, 35, 35, 0), color_t(35, 35, 35, 0));
	if (scroll < total_elements_size - size.y) {
		g_Render->filled_rect_gradient(pos.x, pos.y + size.y - 15, size.x, 15,
			color_t(35, 35, 35, 0), color_t(35, 35, 35, 0),
			color_t(35, 35, 35, alpha), color_t(35, 35, 35, alpha));
	}
	g_Render->_drawList->PopClipRect();

}
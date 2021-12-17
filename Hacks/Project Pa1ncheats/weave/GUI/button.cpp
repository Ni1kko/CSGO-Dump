#include "button.h"
#include "window.h"
#include "child.h"

string c_button::get_label() {
	return this->label;
}
void c_button::set_label(string label) {
	this->label = label;
}
bool c_button::update()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c) return false;
	
	bool inactive = active;
	if (inactive) inactive = !active();
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto size = Vector2D(g_size, 20);
	bool h = hovered();
	auto wnd = (c_window*)c->get_parent();
	if (!wnd)
		return false;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return false;
	if (wnd->g_active_element != this && wnd->g_active_element != nullptr) return false;
	if (h) {
		wnd->g_hovered_element = this;
		if (c->hovered() && wnd->is_click()) {
			if (fn && !inactive) {
				
				fn();
				press_animation = 1.f;
				wnd->reset_mouse();
			}
			else {
				ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
			}
			return true;
		}
	}
	if (fn && !inactive) {
		if (press_animation > 0.f)
			press_animation -= animation_speed;
		if (h) {
			if (animation < 1.f) animation += animation_speed;
		}
		else {
			if (animation > 0.f) animation -= animation_speed;
		}
	}
	animation = clamp(animation, 0.f, 1.f);
	press_animation = clamp(press_animation, 0.f, 1.f);
	return false;
}
bool c_button::hovered()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c->hovered())
		return false;
	auto wnd = (c_window*)c->get_parent();
	if (!wnd)
		return false;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto size = Vector2D(g_size, 20);
	return g_mouse.x > pos.x && g_mouse.y > pos.y
		&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y && !wnd->g_active_element;
}
void c_button::render() {
	if (should_render)
		if (!should_render())
			return;
	bool inactive = active;
	if (inactive) inactive = !active();
	c_child* c = (c_child*)child;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	
	auto size = Vector2D(g_size, 20);

	auto wnd = (c_window*)c->get_parent();
	if (!wnd) return;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return;
	auto alpha = (int)(wnd->get_transparency() * 2.55f);
	auto clr = color_t(50 + animation * 10.f, 50 + animation * 10.f, 50 + animation * 10.f, alpha * (1 - press_animation));
	auto clr2 = color_t(80 - animation * 10.f, 80 - animation * 10.f, 80 - animation * 10.f, alpha * (1 - press_animation));

	g_Render->filled_rect_gradient(pos.x, pos.y, size.x, size.y,
		clr2, clr2,
		clr, clr);
	g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, size.y + 2, 
		color_t(0, 0, 0, alpha));
	g_Render->DrawString(pos.x + size.x / 2, pos.y + size.y / 2, (fn && !inactive) ?
		color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha) :
		color_t(127, 127, 127, alpha),
		render::centered_x | render::centered_y , fonts::menu_desc, label.c_str());
}
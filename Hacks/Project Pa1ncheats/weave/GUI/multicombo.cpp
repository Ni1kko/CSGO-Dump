#include "multicombo.h"
#include "child.h"

bool c_multicombo::hovered()
{
	if (should_render)
		if (!should_render())
			return false;
	auto pos = ((c_child*)child)->get_cursor_position();
	if (!((c_child*)child)->hovered())
		return false;
	auto size = Vector2D(g_size, 20);
	if (label.size() > 0)
		pos.y += after_text_offset;
	return g_mouse.x >= pos.x && g_mouse.y >= pos.y
		&& g_mouse.x <= pos.x + size.x && g_mouse.y <= pos.y + size.y;
}
bool c_multicombo::update()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c) return false;
	
	auto wnd = (c_window*)c->get_parent();
	if (!wnd) return false;
	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return false;
	if (wnd->g_active_element != this && wnd->g_active_element != nullptr) return false;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto size = Vector2D(g_size, 20);

	bool h = hovered();
	if (h) wnd->g_hovered_element = this;
	
	if (h && c->hovered() && wnd->is_click()) {
		if (open) {
			open = false;
			wnd->g_active_element = nullptr;
		}
		else
		{
			open = true;
			wnd->g_active_element = this;
		}
		return true;
	}
	if (open) {
		if (open_animation < 1.f)
			open_animation += animation_speed;
		if (label.size() > 0)
			pos.y += after_text_offset;
		h = g_mouse.x >= pos.x && g_mouse.y >= pos.y
			&& g_mouse.x <= pos.x + size.x && g_mouse.y <= pos.y + size.y * (elements.size() + 1);
		if (wnd->is_click() && !h) {
			open = false;
			wnd->g_active_element = nullptr;
			wnd->reset_mouse();
			return true;
		}
		if (open_animation == 1.f) {
			for (size_t i = 0; i < elements.size(); i++) {
				pos.y += 20;
				h = g_mouse.x >= pos.x && g_mouse.y >= pos.y
					&& g_mouse.x <= pos.x + size.x && g_mouse.y < pos.y + size.y;
				if (h) {
					if (animations[i] < 1.f) animations[i] += animation_speed;
					if (h) wnd->g_hovered_element = this;
				}
				else
				{
					if (animations[i] > 0.f) animations[i] -= animation_speed;
				}
				if (animations[i] > 1.f) animations[i] = 1.f;
				else if (animations[i] < 0.f) animations[i] = 0.f;

				if (wnd->is_click() && h) {
					*(unsigned int*)value ^= 1 << i;
					return false;
				}
			}
		}
		wnd->g_active_element = this;
	}
	else
	{
		if (wnd->g_active_element == this)
			wnd->g_active_element = nullptr;
		if (open_animation > 0.f)
			open_animation -= animation_speed;
	}
	// bullshit animations
	if (h || open) {
		if (animation < 1.f) animation += animation_speed;
	}
	else {
		if (animation > 0.f) animation -= animation_speed;
	}
	animation = clamp(animation, 0.f, 1.f);
	
	if (open_animation > 1.f) open_animation = 1.f;
	else if (open_animation < 0.f) open_animation = 0.f;
	return false;
}
void c_multicombo::render() {
	if (should_render)
		if (!should_render())
			return;
	c_child* c = (c_child*)child;
	if (!c)
		return;
	auto wnd = (c_window*)c->get_parent();
	if (!wnd)
		return;
	if (wnd->get_active_tab_index() != tab && wnd->get_tabs().size() > 0)
		return;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto alpha = (int)(wnd->get_transparency() * 2.55f);
	auto size = Vector2D(g_size, 20);

	bool h = hovered();

	auto clr = color_t(50 + animation * 10.f, 50 + animation * 10.f, 50 + animation * 10.f, alpha);
	if (label.size() > 0) {
		if (this == wnd->g_active_element)
			c->lock_bounds();
		g_Render->DrawString(pos.x, pos.y, color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha),
			render::none, fonts::menu_desc, label.c_str());
		pos.y += after_text_offset;
		if (this == wnd->g_active_element)
			c->unlock_bounds();
	}

	if (open) {
		auto new_alpha = clamp(alpha * open_animation, 0.f, 255.f);
		g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, size.y * (elements.size() + 1), color_t(0, 0, 0, new_alpha));
		g_Render->FilledRect(pos.x, pos.y, size.x, size.y,
			color_t(50, 50, 50, alpha));
		string format;
		int t = 0;
		for (int i = 0; i < elements.size(); i++) {
			if (*(unsigned int*)value & 1 << i) {
				if (t > 0) format += ", ";
				format += elements[i];
				t++;
			}
		}
		if (!format.size())
			format = "-";
		g_Render->_drawList->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - 20.f, pos.y + size.y), false);
		g_Render->DrawString(pos.x + 10, pos.y + size.y / 2, color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha), render::centered_y,
			fonts::menu_desc, format.c_str());
		g_Render->filled_rect_gradient(pos.x + size.x - 20.f, pos.y, 20.f, size.y,
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, 0),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, 0),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, alpha),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, alpha));
		g_Render->_drawList->PopClipRect();
		auto base = ImVec2(pos.x + size.x - 10, pos.y + size.y / 2);
		g_Render->_drawList->AddTriangleFilled(
			ImVec2(base.x - 2, base.y + 1), ImVec2(base.x + 2, base.y + 1), ImVec2(base.x, base.y - 1), color_t(255, 255, 255, new_alpha).u32());

		for (size_t i = 0; i < elements.size(); i++) {
			pos.y += size.y;
			g_Render->FilledRect(pos.x, pos.y, size.x, size.y,
				color_t(50 + 15.f * animations[i], 50 + 15.f * animations[i], 50 + 15.f * animations[i], new_alpha));

			auto clr2 = color_t(200 + animations[i] * 55.f, 200 + animations[i] * 55.f, 200 + animations[i] * 55.f, new_alpha);

			g_Render->DrawString(pos.x + 10, pos.y + size.y / 2,
				((*(unsigned int*)value) & 1 << i) ?
				color_t(100 + 50.f * animations[i], 100 + 50.f * animations[i], 255.f - 25.f * animations[i], new_alpha) : clr2, render::centered_y,
				fonts::menu_desc, elements[i].c_str());
		}
		g_Render->DrawLine(pos.x, pos.y + size.y - 1, pos.x + size.x, pos.y + size.y - 1, color_t(0, 0, 0, new_alpha));
	}
	else {
		auto new_alpha = clamp(alpha * (1.f - open_animation), 0.f, 255.f);
		auto clr2 = color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, new_alpha);
		g_Render->filled_rect_gradient(pos.x, pos.y, size.x, size.y,
			clr2, clr2,
			clr, clr);
		g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, size.y + 2, color_t(0, 0, 0, new_alpha));

		string format;
		int t = 0;
		for (int i = 0; i < elements.size(); i++) {
			if (*(unsigned int*)value & 1 << i) {
				if (t > 0) format += ", ";
				format += elements[i];
				t++;
			}
		}
		if (!format.size())
			format = "-";
		g_Render->_drawList->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - 20.f, pos.y + size.y), true);
		g_Render->DrawString(pos.x + 10, pos.y + size.y / 2, color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha), render::centered_y,
			fonts::menu_desc, format.c_str());
		g_Render->filled_rect_gradient(pos.x + size.x - 20.f, pos.y, 20.f, size.y,
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, 0),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, 0),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, new_alpha),
			color_t(50 - animation * 10.f, 50 - animation * 10.f, 50 - animation * 10.f, new_alpha));

		auto base = ImVec2(pos.x + size.x - 10, pos.y + size.y / 2);
		g_Render->_drawList->PopClipRect();
		g_Render->_drawList->AddTriangleFilled(
			ImVec2(base.x - 2, base.y - 1), ImVec2(base.x + 2, base.y - 1), ImVec2(base.x, base.y + 2), color_t(255, 255, 255, new_alpha).u32());
	}
}

int c_multicombo::get_total_offset() {
	if (should_render)
		if (!should_render())
			return 0;
	return 23 + (label.size() > 0) * after_text_offset;
};
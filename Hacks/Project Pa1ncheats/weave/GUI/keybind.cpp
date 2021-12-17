#include "keybind.h"
#include "child.h"

bool c_keybind::update()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c) return false;
	auto wnd = (c_window*)c->get_parent();
	if (!wnd) return false;
	if (wnd->g_active_element != this && wnd->g_active_element != nullptr) return true;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	bool h = hovered();

	if (wnd->get_active_tab_index() != this->tab && wnd->get_tabs().size() > 0)
		return false;
	if (h) wnd->g_hovered_element = this;
	// bullshit animations
	if (h) {
		if (animation < 1.f) animation += animation_speed;
	}
	else {
		if (animation > 0.f) animation -= animation_speed;
	}

	bind->type = clamp<unsigned short>(bind->type, 0, 4);
	bind->key = clamp<unsigned short>(bind->key, 0, 255);

	animation = clamp(animation, 0.f, 1.f);
	
	if (binder.active) {
		for (auto i = 0; i < 256; i++) {
			if (wnd->key_updated(i)) {
				if (i == VK_ESCAPE) {
					bind->key = 0;
					binder.active = false;
					wnd->g_active_element = nullptr;
					return true;
				}
				else {
					bind->key = i;
					binder.active = false;
					wnd->g_active_element = nullptr;
					return true;
				}
			}
		}

		return true;
	}


	if (h && c->hovered() && wnd->left_click()) {
		binder.open = true;
		wnd->g_active_element = this;
		return true;
	}
	if (binder.open) {
		auto size = Vector2D(g_size, 20);
		pos.y += size.y;
		h = g_mouse.x >= pos.x && g_mouse.y >= pos.y
			&& g_mouse.x <= pos.x + size.x && g_mouse.y < pos.y + size.y;
		if (wnd->is_click() && h) {
			binder.open = false;
			wnd->g_active_element = nullptr;
			wnd->reset_mouse();
			return true;
		}
		h = g_mouse.x >= pos.x && g_mouse.y >= pos.y
			&& g_mouse.x <= pos.x + size.x && g_mouse.y < pos.y + size.y * (binder.elements.size() + 1);
		if ((wnd->is_click() || wnd->left_click()) && !h) {
			binder.open = false;
			wnd->g_active_element = nullptr;
			wnd->reset_mouse();
			return true;
		}
		for (size_t i = 0; i < binder.elements.size(); i++) {
			pos.y += 20;
			h = g_mouse.x >= pos.x && g_mouse.y >= pos.y
				&& g_mouse.x <= pos.x + size.x && g_mouse.y < pos.y + size.y;
			if (h) {
				if (binder.animations[i] < 1.f) binder.animations[i] += animation_speed;
			}
			else
			{
				if (binder.animations[i] > 0.f) binder.animations[i] -= animation_speed;
			}
			if (binder.animations[i] > 1.f) binder.animations[i] = 1.f;
			else if (binder.animations[i] < 0.f) binder.animations[i] = 0.f;
			if (wnd->is_click() && h) {
				bind->type = i;
				binder.open = false;
				wnd->g_active_element = nullptr;
				wnd->reset_mouse();
				return true;
			}
		}
		wnd->g_active_element = this;
	}
	else {
		auto size = Vector2D(g_size, 20);
		h = g_mouse.x > pos.x && g_mouse.y > pos.y
			&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y;
		if (h && c->hovered() && wnd->is_click() && bind->type > 0) {
			binder.active = true;
			wnd->g_active_element = this;
			return true;
		}
		if (h) {
			if (binder.animation < 1.f) binder.animation += animation_speed;
		}
		else {
			if (binder.animation > 0.f) binder.animation -= animation_speed;
		}
		if (binder.animation > 1.f) binder.animation = 1.f;
		else if (binder.animation < 0.f) binder.animation = 0.f;
	}

	return true;
}

bool c_keybind::hovered()
{
	if (should_render)
		if (!should_render())
			return false;
	c_child* c = (c_child*)child;
	if (!c->hovered())
		return false;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto alpha = (int)(c->get_transparency() * 2.55f);
	auto size = Vector2D(g_size, 20);
	return g_mouse.x > pos.x && g_mouse.y > pos.y
		&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y;
}

void c_keybind::render() {
	if (should_render)
		if (!should_render())
			return;
	c_child* c = (c_child*)child;
	auto pos = c->get_cursor_position() - Vector2D(10, 5);
	auto wnd = (c_window*)c->get_parent();
	auto size = Vector2D(g_size, 20);
	if (!wnd) return;
	if (wnd->get_active_tab_index() != this->tab
		&& wnd->get_tabs().size() > 0) return;

	auto alpha = (int)(wnd->get_transparency() * 2.55f);
	auto clr = color_t(40 + animation * 10.f, 40 + animation * 10.f, 40 + animation * 10.f, alpha);
	auto clr2 = color_t(40 - animation * 10.f, 40 - animation * 10.f, 40 - animation * 10.f, alpha);

	g_Render->filled_rect_gradient(pos.x, pos.y, size.x, size.y,
		clr2, clr2,
		clr, clr);
	g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, size.y + 2,
		color_t(0, 0, 0, alpha));
	string format = label + ": %s";
	g_Render->DrawString(pos.x + size.x / 2, pos.y + size.y / 2, binder.active ? 
		color_t(200 + 55.f * animation, 100 + 50.f * animation, 25.f - 25.f * animation, alpha) :
		color_t(200 + 55.f * animation, 200 + 55.f * animation, 200 + 55.f * animation, alpha),
		render::centered_x | render::centered_y, fonts::menu_desc, format.c_str(), KeyStrings[bind->key].c_str());


	if (binder.open) {

		pos.y += size.y;
		auto size = Vector2D(g_size, 20);
		g_Render->Rect(pos.x - 1, pos.y - 1, size.x + 2, (size.y * (binder.elements.size() + 1)) + 2, color_t(0, 0, 0, alpha));
		g_Render->FilledRect(pos.x, pos.y, size.x, size.y,
			color_t(25.f, 25.f, 25.f, alpha));

		auto base = ImVec2(pos.x + size.x - 10, pos.y + size.y / 2);
		g_Render->_drawList->AddTriangleFilled(
			ImVec2(base.x - 3, base.y + 2), ImVec2(base.x + 3, base.y + 2), ImVec2(base.x, base.y - 4), color_t(255, 255, 255, alpha).u32());

		g_Render->DrawString(pos.x + 10, pos.y + size.y / 2, color_t(255, 255, 255, alpha), render::centered_y,
			fonts::menu_desc, "%s (%s)", KeyStrings[bind->key].c_str(), binder.elements[bind->type].c_str());

		for (size_t i = 0; i < binder.elements.size(); i++) {
			pos.y += size.y;
			g_Render->FilledRect(pos.x, pos.y, size.x, size.y,
				color_t(25.f + 15.f * binder.animations[i], 25.f + 15.f * binder.animations[i], 25.f + 15.f * binder.animations[i], alpha));

			auto clr2 = color_t(200 + binder.animations[i] * 55.f, 200 + binder.animations[i] * 55.f, 200 + binder.animations[i] * 55.f, alpha);
			g_Render->DrawString(pos.x + 10, pos.y + size.y / 2, clr2, render::centered_y,
				fonts::menu_desc, binder.elements[i].c_str());
		}
		g_Render->DrawLine(pos.x, pos.y + size.y, pos.x + size.x, pos.y + size.y, color_t(0, 0, 0, alpha));
	}
	else {
		auto base = ImVec2(pos.x + size.x - 10, pos.y + size.y / 2);
		g_Render->_drawList->AddTriangleFilled(
			ImVec2(base.x - 4, base.y - 2), ImVec2(base.x + 4, base.y - 2), ImVec2(base.x, base.y + 5), color_t(255, 255, 255, alpha).u32());
	}
}

#include "tab.h"
#include "child.h"
#include "../Variables.h"

void c_tab::draw(Vector2D pos, Vector2D size) {
	c_child* c = (c_child*)child;
	auto alpha = (int)(c->get_transparency() * 2.55f);
	bool hovered = g_mouse.x > pos.x && g_mouse.y > pos.y
		&& g_mouse.x < pos.x + size.x && g_mouse.y < pos.y + size.y;
	if (hovered && !c->g_active_element && !c->is_holding_menu()) {
		c->g_hovered_element = this;
		if (c->is_click()) {
			c->set_active_tab_index(this->index);
			c->reset_mouse();
		}
		
		if (animation < 1.f) animation += animation_speed;
	}
	else {
		if (animation > 0.f) animation -= animation_speed;
	}

	animation = clamp(animation, 0.f, 1.f);
	
	//auto clr = color_t(40 + animation * 10.f, 40 + animation * 10.f, 40 + animation * 10.f, alpha);
	//auto clr2 = color_t(40 - animation * 10.f, 40 - animation * 10.f, 40 - animation * 10.f, alpha);

	if (this->index == 0)
	{
		g_Render->FilledRect(pos.x, pos.y + size.y - 1, size.x - 2, 2, c->get_active_tab_index() == this->index ? color_t(vars.misc.menu_color) : color_t(85, 85, 85, alpha));
	}
	else if (this->index == 7)
	{
		g_Render->FilledRect(pos.x + 2, pos.y + size.y - 1, size.x - 2, 2, c->get_active_tab_index() == this->index ? color_t(vars.misc.menu_color) : color_t(85, 85, 85, alpha));
	}
	else//shit code i hate it OwO
	{
		g_Render->FilledRect(pos.x + 2, pos.y + size.y - 1, size.x - 4, 2, c->get_active_tab_index() == this->index ? color_t(vars.misc.menu_color) : color_t(85, 85, 85, alpha));
	}
	//g_Render->FilledRect(pos.x, pos.y + size.y - 1, size.x, 1, c->get_active_tab_index() == this->index ? color_t(100, 100, 255, 255) : color_t(60, 60, 60, 255));
	auto t_clr = c->get_active_tab_index() == this->index ? color_t(200, 200, 200, alpha) : color_t(120, 120, 120, alpha);
	
	g_Render->DrawString(pos.x + size.x / 2, pos.y + size.y / 2, t_clr,
		render::centered_x | render::centered_y, fonts::menu_main, get_title().c_str());
}
void c_tab::special_render() {

}
void c_tab::render() {

}
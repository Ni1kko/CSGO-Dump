#include "..\cmenu.hpp"
#include "..\..\sdk\misc\Color.hpp"
void spectrum(int x, int y, int w, int h) {
	static int GradientTexture = 0;
	static std::unique_ptr<Color[]> Gradient = nullptr;
	if (!Gradient)
	{
		Gradient = std::make_unique<Color[]>(w * h);
		for (int i = 0; i < w; i++)
		{
			int div = w / 6;
			int phase = i / div;
			float t = (i % div) / (float)div;
			int r, g, b;
			switch (phase) {
			case(0):
				r = 255;
				g = 255 * t;
				b = 0;
				break;
			case(1):
				r = 255 * (1.f - t);
				g = 255;
				b = 0;
				break;
			case(2):
				r = 0;
				g = 255;
				b = 255 * t;
				break;
			case(3):
				r = 0;
				g = 255 * (1.f - t);
				b = 255;
				break;
			case(4):
				r = 255 * t;
				g = 0;
				b = 255;
				break;
			case(5):
				r = 255;
				g = 0;
				b = 255 * (1.f - t);
				break;
			}
			for (int k = 0; k < h; k++) {
				float sat = k / (float)h;
				int _r = r + sat * (128 - r);
				int _g = g + sat * (128 - g);
				int _b = b + sat * (128 - b);
				*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
			}
		}
		GradientTexture = g_csgo.m_surface()->CreateNewTextureID(true);
		g_csgo.m_surface()->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
	}
	g_csgo.m_surface()->DrawSetColor(Color(255, 255, 255, 255));
	g_csgo.m_surface()->DrawSetTexture(GradientTexture);
	g_csgo.m_surface()->DrawTexturedRect(x, y, x + w, y + h);
}
auto color_from_pen(int x, int y, int w, int h, Vector2D mouse) -> Color {
	int div = w / 6;
	int phase = mouse.x / div;
	float t = ((int)mouse.x % div) / (float)div;
	int r, g, b;
	switch (phase) {
	case(0):
		r = 255;
		g = 255 * t;
		b = 0;
		break;
	case(1):
		r = 255 * (1.f - t);
		g = 255;
		b = 0;
		break;
	case(2):
		r = 0;
		g = 255;
		b = 255 * t;
		break;
	case(3):
		r = 0;
		g = 255 * (1.f - t);
		b = 255;
		break;
	case(4):
		r = 255 * t;
		g = 0;
		b = 255;
		break;
	case(5):
		r = 255;
		g = 0;
		b = 255 * (1.f - t);
		break;
	}
	float sat = mouse.y / h;
	int rgb[3] = {
		r + sat * (128 - r),
		g + sat * (128 - g),
		b + sat * (128 - b)
	};
	return Color(rgb[0], rgb[1], rgb[2], 255);
}
C_ColorSelector::C_ColorSelector(Color * n_color, std::string n_label) {
	area.w = 15;
	area.h = 8;
	parent = g_cfg.menu.last_group;
	preview_color = n_color;
	color = *preview_color;
	label = n_label;
	brightness = 1.0f;
}
void C_ColorSelector::draw() {
	render::get().rect_filled(area.x, area.y, area.w, area.h, *preview_color);
	render::get().rect(area.x, area.y, area.w, area.h, Color::Black);
	rect_t n_area = rect_t
	(
		area.x,
		area.y + area.h,
		234,
		254
	);
	if (open) {
		render::get().rect_filled(n_area.x, n_area.y, n_area.w, n_area.h, Color{ 25, 25, 25 });
		render::get().rect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);
		n_area = rect_t(
			n_area.x + 5,
			n_area.y + 5,
			n_area.w - 10,
			n_area.h - 10
		);
		render::get().rect_filled(n_area.x, n_area.y, n_area.w, n_area.h, Color{ 35, 35, 35, 255 });
		render::get().rect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);
		n_area = rect_t(
			n_area.x + 4,
			n_area.y + 4,
			n_area.w - 8,
			n_area.h - 28
		);
		spectrum(n_area.x, n_area.y, 216, 216);
		render::get().rect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);
		rect_t slider_area = rect_t(
			n_area.x,
			n_area.y + 218,
			216, 11
		);
		render::get().gradient(slider_area.x, slider_area.y, 108, 11, Color::White, color, GRADIENT_HORIZONTAL);
		render::get().gradient(slider_area.x + 108, slider_area.y, 108, 11, color, Color::Black, GRADIENT_HORIZONTAL);
		render::get().rect(slider_area.x, slider_area.y, 216, 11, Color::Black);
		slider_area.y += 12;
		float ratio = (brightness - 0.0f) / (2.0f - 0.0f);
		float location = ratio * 216;
		int points[6] = {
			slider_area.x + location,
			slider_area.y,
			slider_area.x + location + 4,
			slider_area.y + 6,
			slider_area.x + location - 4,
			slider_area.y + 6,
		};
		render::get().triangle(Vector2D(points[0], points[1]), Vector2D(points[2], points[3]), Vector2D(points[4], points[5]), Color(153, 153, 153));
	}
}
void C_ColorSelector::update() {
	auto recalculate_color = [this]() -> void {
		float new_brightness = 2.0f - brightness;
		int new_colors[3] = {
			new_brightness * color.r(),
			new_brightness * color.g(),
			new_brightness * color.b()
		};
		if (new_brightness > 1.0f) {
			new_colors[0] = color.r() + (new_brightness - 1.0f) * (255 - color.r());
			new_colors[1] = color.g() + (new_brightness - 1.0f) * (255 - color.g());
			new_colors[2] = color.b() + (new_brightness - 1.0f) * (255 - color.b());
		}
		*preview_color = Color(new_colors[0], new_colors[1], new_colors[2]);
	};
	if (!open) { return; }
	parent->focus_subcontrol(this);
	POINT mouse; GetCursorPos(&mouse);
	rect_t open_area = rect_t(
		area.x, area.y + area.h,
		234, 254
	);
	if (menu::get().key_press(VK_LBUTTON) && !open_area.contains_point(mouse)) {
		open = false;
		parent->reset_block();
	}
	open_area = rect_t(
		area.x + 9, area.y + area.h + 9,
		216, 216
	);
	rect_t slider_area = rect_t(
		area.x + 9,
		area.y + area.h + 227,
		216, 11
	);
	if (GetAsyncKeyState(VK_LBUTTON) && slider_area.contains_point(mouse)) {
		dragging = true;
	}
	if (dragging) {
		if (GetAsyncKeyState(VK_LBUTTON)) {
		float
		new_x,
		ratio;
			new_x = mouse.x - slider_area.x;
			if (new_x < 0) { new_x = 0; }
			if (new_x > slider_area.w) { new_x = 216; }
			ratio = new_x / float(slider_area.w);
			brightness = 2.0f * ratio;
			recalculate_color();
		}
		else {
			dragging = false;
		}
	}
	if (GetAsyncKeyState(VK_LBUTTON) && open_area.contains_point(mouse) && !dragging) {
		color = color_from_pen(open_area.x, open_area.y, 216, 216, Vector2D(mouse.x - open_area.x, mouse.y - open_area.y));
		recalculate_color();
	}
}
void C_ColorSelector::click() {
	if (open) {
		parent->reset_block();
	}
	open = !open;
}
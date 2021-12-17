#pragma once

#include "../features.hpp"
#include "../../../source-sdk/classes/client_class.hpp"
#include "../../menu/fgui_menu.hpp"

std::vector<float> get_glow_colors() {
	fgui::color col = vars::colorpicker["#glow_color"]->get_color();

	std::vector<float>fcol{
		(float)col.m_red / 255,
		(float)col.m_green / 255,
		(float)col.m_blue / 255,
		(float)col.m_alpha / 255
	};

	return fcol;
};



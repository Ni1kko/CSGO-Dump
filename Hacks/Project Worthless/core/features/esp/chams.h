#pragma once

#include "../features.hpp"
#include "../../../source-sdk/classes/client_class.hpp"
#include "../../menu/fgui_menu.hpp"

std::vector<float> get_chams_colors() {
	const auto col = vars::colorpicker["#regular_chams_color"]->get_color();
	const auto col_invis = vars::colorpicker["#regular_chams_color_xqz"]->get_color();

	std::vector<float> fcol {
		(float)col.m_red / 255,
		 (float)col.m_green / 255,
		 (float)col.m_blue / 255,
		 (float)col.m_alpha / 255
	};

	return fcol;
};

std::vector<float> get_invisible_chams_colors() {
	const auto col_invis = vars::colorpicker["#regular_chams_color_xqz"]->get_color();

	std::vector<float> fcol{
		(float)col_invis.m_red / 255,
		 (float)col_invis.m_green / 255,
		 (float)col_invis.m_blue / 255,
		 (float)col_invis.m_alpha / 255
	};

	return fcol;
};

std::vector<float> get_glow_chams_colors() {
	const auto col_glow = vars::colorpicker["#chams_glow_color"]->get_color();

	std::vector<float> fcol{
		(float)col_glow.m_red / 255,
		(float)col_glow.m_green / 255,
		(float)col_glow.m_blue / 255,
		(float)col_glow.m_alpha / 255
	};

	return fcol;
}


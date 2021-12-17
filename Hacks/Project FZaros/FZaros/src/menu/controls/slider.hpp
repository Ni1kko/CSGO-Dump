#pragma once
#include "../menu.hpp"

class c_slider : public c_element {
private:
	float min_value;
	float max_value;
	bool round;

	std::wstring display_format;

public:
	c_slider( const std::string &label, const std::string &var_name, float min_value, float max_value, bool round_number, const std::wstring &display_format );
	c_slider( const std::string &label, const std::string &var_name, float min_value, float max_value, bool round_number = false, const std::string &display_format = "%.0f%%" );

	void think( ) override;
	void draw( ) override;

};

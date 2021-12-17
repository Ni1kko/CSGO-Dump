#pragma once
#include "../menu.hpp"

class c_checkbox : public c_element {
public:
	c_checkbox( const std::string &label, const std::string &var_name );

	void think( ) override;
	void draw( ) override;

};

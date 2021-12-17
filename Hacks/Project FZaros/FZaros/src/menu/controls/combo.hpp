#pragma once
#include "../menu.hpp"

class c_combo : public c_element {
private:
	std::vector<std::string> items;

public:
	c_combo( const std::string &label, const std::string &var_name, const std::vector<std::string> &items = { "None" } );

	void think( ) override;
	void draw( ) override;

};

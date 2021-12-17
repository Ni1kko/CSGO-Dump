#pragma once
#include "../menu.hpp"

class c_multicombo : public c_element {
private:
	std::vector<std::string> items;

public:
	c_multicombo( const std::string &label, const std::string &var_name, const std::vector<std::string> &items = { "None" } );

	void think( ) override;
	void draw( ) override;

};

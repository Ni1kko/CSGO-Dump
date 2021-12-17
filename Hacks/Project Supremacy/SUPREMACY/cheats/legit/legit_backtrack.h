#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class legitbacktrack : public singleton< legitbacktrack > {
public:
	void create_move();
	void paint_traverse();
};
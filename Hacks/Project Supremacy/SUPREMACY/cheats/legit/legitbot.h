#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class legitbot : public singleton< legitbot > {
public:
	void create_move();
};
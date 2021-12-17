#pragma once

#include "..\..\includes.hpp"

class circlestrafer : public singleton< circlestrafer > {
public:
	void start();
	void strafe();

	bool active;
	float right_movement;
	float strafe_angle;
};
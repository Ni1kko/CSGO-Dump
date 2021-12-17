#pragma once
#include "../Interfaces/Interfaces.h"
#include "../singleton.h"

class c_fps : public singleton<c_fps> {
public:
	int get_fps();

private:
	std::vector<int> data;
};
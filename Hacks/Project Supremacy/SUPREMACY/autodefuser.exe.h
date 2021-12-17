#pragma once
#include "includes.hpp"
class autodefuser : public singleton<autodefuser>
{
public:
	void defuserapplication(CUserCmd* cmd);
};
#pragma once

#include <map>

#include "..\sdk\interfaces\ICvar.hpp"

class C_ConvarSpoofer {
public:
	void init();
	void release();

	void add(const char *name);
	void spoof();
	SpoofedConvar * get(const char *name);
	bool ready();
};

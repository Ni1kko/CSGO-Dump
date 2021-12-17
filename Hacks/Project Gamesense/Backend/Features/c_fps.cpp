#include "c_fps.h"

int c_fps::get_fps() {
	int curFps = int(1.f / Interfaces::Globals->absoluteframetime);

	data.push_back(curFps);
	if (data.size() > 10000)
		data.erase(data.begin());

	int absFps = 0;
	for (auto fps : data)
		absFps += fps;

	absFps /= data.size();

	return absFps;
}
#pragma once
#include "../sdk/point.hpp"

class CInputManager
{
private:
	CPoint mousePosition;
	CPoint prevMousePosition;

	bool keyState[256];
	bool prevKeyState[256];

public:
	void update();

	bool isKeyPressed(int key);
	bool isKeyDown(int key);
	bool isKeyReleased(int key);

	CPoint getMousePos();
	CPoint getPrevMousePos();

	bool isHovered(const CPoint &min, const CPoint &max);

};

extern CInputManager* g_Input;
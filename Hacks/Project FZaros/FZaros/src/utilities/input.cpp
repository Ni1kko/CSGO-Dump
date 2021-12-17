#include <windows.h>
#include "input.hpp"

void CInputManager::update()
{
	HWND gameWindow = FindWindow("Valve001", "Counter-Strike: Global Offensive");
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(gameWindow, &mousePos);

	this->prevMousePosition = this->mousePosition;
	this->mousePosition = CPoint(mousePos.x, mousePos.y);

	for (int i = 0; i < 256; i++)
	{
		this->prevKeyState[i] = this->keyState[i];
		this->keyState[i] = GetAsyncKeyState(i);
	}
}

bool CInputManager::isKeyPressed(int key)
{
	return this->prevKeyState[key] == false && this->keyState[key] == true;
}

bool CInputManager::isKeyDown(int key)
{
	return this->keyState[key] == true;
}

bool CInputManager::isKeyReleased(int key)
{
	return this->prevKeyState[key] == true && this->keyState[key] == false;
}

CPoint CInputManager::getMousePos()
{
	return this->mousePosition;
}

CPoint CInputManager::getPrevMousePos()
{
	return this->prevMousePosition;
}

bool CInputManager::isHovered(const CPoint &min, const CPoint &max)
{
	return this->mousePosition.x >= min.x && this->mousePosition.y >= min.y &&
		this->mousePosition.x <= max.x && this->mousePosition.y <= max.y;
}

CInputManager* g_Input = new CInputManager();
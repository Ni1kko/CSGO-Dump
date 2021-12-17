#pragma once
#include "Includes.h"
#include "ReadProcessMemory.h"
class CheatManager
{
private:
	void CoutOffset(LPCSTR name, DWORD offset);
	void Coutfloat(LPCSTR name, float option);
	void Coutbool(LPCSTR name, int boolean);
	float ReadConfig(LPCSTR Option, bool Float);
	DWORD ReadInt(LPCTSTR Offsetname);
public:
	bool Shutdown = false;
	void Options();
	float AimbotFOV = 1.4f;
	float AimbotSMOOTH = 1.f;
	int TriggerbotModeToggle = 0;
	int WantAim = 1;
	int Bonetype = 8;
	int WantESP = 1;
	int WantChams = 1;
	int WantBhop = 1;
	int WantChamsR = 1;
	float ChamsBrightness = 30.f;

	void InitializeCheat();
}; extern CheatManager CM;
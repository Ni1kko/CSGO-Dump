#pragma once
#include "Includes.h"

class InitMemory
{
private:
	bool GetApp();
	DWORD GetModule(LPCSTR module);
public:
	void StartMemory(void);
	DWORD PID;
	DWORD ClientDLL;
	DWORD ServerDLL;
	DWORD EngineDLL;
	HANDLE Hdl;
};
extern InitMemory Init;


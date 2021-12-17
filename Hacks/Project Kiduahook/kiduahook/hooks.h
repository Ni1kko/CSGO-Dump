#pragma once

#include "main.h"

using BeginFrameFn = void( __thiscall* )( void* );
extern BeginFrameFn oBeginFrame;

using IsBoxVisibleFn = int( __stdcall* )( const Vector&, const Vector& );
extern IsBoxVisibleFn oIsBoxVisible;

using OverrideConfigFn = bool( __thiscall* )( IMaterialSystem*, MaterialSystem_Config_t*, bool );
extern OverrideConfigFn oOverrideConfig;

using FireEventFn = void( __thiscall* )( void* );
extern FireEventFn oFireEvent;

typedef float( __thiscall* ViewModelFn_t )( void* );
extern ViewModelFn_t ViewModelOriginal;

typedef bool( __thiscall *SetupBones_t )( void*, matrix3x4_t*, int, int, float );
extern SetupBones_t oSetupBones;

typedef bool( __fastcall *InPrediction_t )( void* ecx );
extern InPrediction_t oInPrediction;

typedef bool(__thiscall* WriteUserCmdFn)(void* ecx, int slot, void* buf, int from, int to, bool newcmd);
extern WriteUserCmdFn oWriteUserCmd;

//(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
using ListLeavesInBoxFn = int(__thiscall*)(void*, const Vector&, const Vector&, unsigned short*, int);
extern ListLeavesInBoxFn oListLeavesInBox;

extern WNDPROC OldWindowProc;
extern HWND hwndWindow;

class CHooks
{
public:
	void InitHooks();
};

extern CHooks g_Hooks;
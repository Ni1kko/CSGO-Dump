#pragma once

#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <map>
#include <ostream>
#include <Shlobj.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <cmath>
#include <float.h>
#include <codecvt>
using namespace std;
#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>
#include <algorithm>
#include <iterator>

#include "SDK/SDK Headers/IEffects.h"

/* Some other shit */
#include "ConsoleColours.h"
#include "Tools/Menu/SettingsManager.h"

DWORD WINAPI CheatMain( LPVOID lpThreadParameter );

#include "VMTManager.h"

/*SDK*/
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_internal.h"
#include "SDK/Math/Vectors.h"
#include "SDK/Math/Math.h"
#include "Utils/Utils.h"
#include "SDK/SDK.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <playsoundapi.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

/* Cheat And Hooks */
class CHackManager;
extern CHackManager Hacks;

namespace INIT
{
	extern HMODULE Dll;
	extern HWND Window;
	extern WNDPROC OldWindow;
}

#include "Tools/Menu/Controls.h"
#include "Hooks.h"
#include "MiscDefinitions.h"
#include "IMDLCache.h"
#include "Vars.h"

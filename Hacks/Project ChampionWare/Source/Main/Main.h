#pragma once

#pragma warning(disable:4996)

//#define WIN32_LEAN_AND_MEAN




#include <Windows.h>


#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <TlHelp32.h>
#include <WinInet.h>
#include <atlstr.h>


using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "wininet.lib")

#include "../config.h"
#include "../CSX/CSX.h"
#include "../SDK/SDK.h"

using namespace SDK;

#include "../Settings/Settings.h"
#include "../Engine/Engine.h"
#include "../Cheat/Client.h"
#include "../WinAPI/WinAPI.h"
#include "../Xor/xor2.h"
#include "../Globals/Globals.h"

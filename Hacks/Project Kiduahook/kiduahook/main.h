#pragma once

#pragma warning(disable: 4244) //float -> int
#pragma warning(disable: 4409) //Unzulässige Anweisungsgröße
#pragma warning(disable: 4800) //Variable wird auf booleschen Wert gesetzt
#pragma warning(disable: 4806) //Variable wird auf booleschen Wert gesetzt
#pragma warning(disable: 4018) //Konflikt zwischen signed und unsigned
#pragma warning(disable: 4305) //Verkürzung double in float
#pragma warning(disable: 4127) //Addresse einer lokalen Variablen oder eines temporären Werts wird zurückgegeben

#define _CRT_SECURE_NO_WARNINGS

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#include <windows.h>
#include <vector>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <wininet.h>
#include <set>
#include <array>
#include <memory>
#include <random>
#include <numeric>
#include <unordered_map>
#include <Psapi.h>
#include <map>
#include <Shlobj.h>
#include <deque>
#include <direct.h>
#include <future>

#pragma comment(lib, "Winmm.lib")

#include "vector.h"
#include "checksum_crc.h"
#include "vmtmanager.h"
#include "debug.h"
#include "patternscan.h"
#include "interfaces.h"
#include "sdk.h"
#include "drawing.h"
#include "netvarmanager.h"
#include "offset.h"

#include "awall.h"
#include "math.h"

#include "vars.h"

#include "gui.h"

#include "createmove.h"
#include "painttraverse.h"
#include "beginframe.h"
#include "runcommand.h"
#include "framestagenotify.h"
#include "overrideview.h"
#include "inprediction.h"
#include "drawmodel.h"
#include "drawmodelstatsoverlay.h"
#include "viewmodel.h"
#include "packetstart.h"
#include "wndproc.h"
#include "svcheats.h"
#include "overrideconfig.h"
#include "setupbones.h"
#include "doextraboneprocessing.h"
#include "calcviewmodelview.h"
#include "csmshadow.h"
#include "sceneend.h"
#include "ishltv.h"
#include "writeusercmd.h"

#include "hooks.h"

#include "history.h"
#include "nadepred.h"
#include "visuals.h"
#include "rageaimbot.h"
#include "hitmarker.h"
#include "misc.h"
#include "resolver.h"
#include "skinchanger.h"
#include "antiaim.h"
#pragma once
#include "../../singleton.h"
#include "../../lazyimports.h"
#include "../../Hashing/xorstr.h"
#include "../Client/sha1.hpp"
#include "winreg.h"

#include <Windows.h>
#include <string>
#include <array>
#include <bitset>
#include <vector>

class c_hwid : public singleton<c_hwid> {
public:
	std::string get_hwid();
};
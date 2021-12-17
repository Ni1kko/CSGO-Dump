#pragma once
#include "JSON.h"
#include "../valve_sdk\csgostructs.hpp"
#include "../options.hpp"
#include <unordered_map>
#include <array>
#include <algorithm>
#include <fstream>

namespace Configs
{
	void DumpClassIDs(const char* fileName);

	void total_sheja_sejvit_kenfig(std::string fileName);
	void total_sheja_loud_kenfig(std::string fileName);
}
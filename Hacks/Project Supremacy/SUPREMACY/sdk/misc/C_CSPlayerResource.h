#pragma once

#include <Windows.h>
#include "..\..\utils\netmanager.hpp"

class CPlayerResource { };

class C_CSPlayerResource : public CPlayerResource {
public:
	int GetPing(int index) {
		return *(int*)(uintptr_t(this) + netvars::get().get_offset("CPlayerResource", "m_iPing") + index * 4);
	}

	int GetKills(int index) {
		return *(int*)(uintptr_t(this) + netvars::get().get_offset("CPlayerResource", "m_iKills") + index * 4);
	}
};
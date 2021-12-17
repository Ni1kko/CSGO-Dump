#pragma once

#include "..\includes.hpp"

#include "..\sdk\math\Vector.hpp"
#include "..\sdk\misc\CUserCmd.hpp"

#include "..\sdk\structs.hpp"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / g_csgo.m_globals( )->m_interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( g_csgo.m_globals( )->m_interval_per_tick *( t ) )

class entity_t;
class player_t;
class IClientEntity;
class CGameTrace;

struct datamap_t;

struct Box {
	int x = -1, y = -1, w = -1, h = -1;
};

namespace util {
	int epoch_time();

	unsigned long pattern_scan(const char * module_name, const char * signature);
	uint64_t FindSignature(const char* szModule, const char* szSignature);
	DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern);
	bool visible(const Vector & start, const Vector & end, player_t * player, player_t * local);

	float server_time(CUserCmd * m_pcmd = nullptr);
	float lerp_time();
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
	bool good_backtrack_tick(float m_flSimulationTime);
	bool get_bbox(player_t * e, Box & box);
	bool get_bbox_world(entity_t * e, Box & box);
	void trace_line(vec3_t & start, vec3_t & end, unsigned int mask, IClientEntity * ignore, CGameTrace * tr);
	void movement_fix(vec3_t & wish_angle, CUserCmd * m_pcmd);
	unsigned int find_in_datamap(datamap_t * map, const char *name);
	template <typename t> t clamp_value(t value, t min, t max) {
		if (value > max) {
			return max;
		}
		if (value < min) {
			return min;
		}
		return value;
	}

	template<class Type>
	static Type hook_manual(uintptr_t* vftable, uint32_t index, Type fnNew) {
		DWORD Dummy;
		Type fnOld = (Type)vftable[index];
		VirtualProtect((void*)(vftable + index * 0x4), 0x4, PAGE_EXECUTE_READWRITE, &Dummy);
		vftable[index] = (uintptr_t)fnNew;
		VirtualProtect((void*)(vftable + index * 0x4), 0x4, Dummy, &Dummy);
		return fnOld;
	}
}
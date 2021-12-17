#pragma once
#include "Hooks.h"

class VarMapEntry_t {
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t {
	VarMapping_t() {
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

class CLagcompensation {
	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}

public:
	void disable_interpolation(IClientEntity* m_entity) 
	{
		VarMapping_t* map = GetVarMap(m_entity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++) {
			VarMapEntry_t *e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
			
		}
	}
};
extern CLagcompensation lagcompensation;
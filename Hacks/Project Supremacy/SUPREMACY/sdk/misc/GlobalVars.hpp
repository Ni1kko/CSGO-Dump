#pragma once

class CGlobalVarsBase {
public:
	float     m_realtime;                     // 0x0000
	int       m_framecount;                   // 0x0004
	float     m_absoluteframetime;            // 0x0008
	float     m_absoluteframestarttimestddev; // 0x000C
	float     m_curtime;                      // 0x0010
	float     m_frametime;                    // 0x0014
	int       m_maxclients;                   // 0x0018
	int       m_tickcount;                    // 0x001C
	float     m_interval_per_tick;            // 0x0020
	float     m_interpolation_amount;         // 0x0024
	int       m_simTicksThisFrame;            // 0x0028
	int       m_network_protocol;             // 0x002C
	void*     m_pSaveData;                    // 0x0030
	bool      m_bClient;                    // 0x0031
	bool      m_bRemoteClient;              // 0x0032

private:
	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	int       m_nTimestampNetworkingBase;
	// 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to Set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	int       m_nTimestampRandomizeWindow;
};
#pragma once
#include "includes.hpp"
#include <deque>
#include <array>
inline Vector AngleVector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
	int32_t m_nFlags;
};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
	Vector origin;
	Vector angs;
	int32_t flags;
};
struct whocaresaboutcode
{
	int32_t m_nFlags;
};

struct who_reads_that_isGay
{
	matrix3x4_t* matrix;
	Vector originX;
};

class BackTrack
{
	int latest_tick;

public:
	bool IsTickValid(int tick);
	lbyRecords records[64];
	bool RunLBYBackTrack(CUserCmd * cmd, Vector & aimPoint);
	void rageBackTrack(CUserCmd * cmd, player_t * pLocal);

	void UpdateRecord(int i);
	void Update(int tick_count);
	void legitBackTrack(CUserCmd* cmd, player_t* pLocal);
};

struct CTickRecord {
	CTickRecord() {}
	CTickRecord(player_t* ent)
	{
		m_flLowerBodyYawTarget = ent->m_flLowerBodyYawTarget();
		m_angEyeAngles = ent->m_angEyeAngles();
		m_flCycle = ent->m_flCycle();
		m_flSimulationTime = ent->m_flSimulationTime();
		m_nSequence = ent->m_nSequence();
		m_vecOrigin = ent->abs_origin();
		m_vecVelocity = ent->m_vecVelocity();
		//m_flPoseParameter = ent->getPoseParams();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_flDuckSpeed = ent->m_flDuckSpeed();
		m_flDuckAmount = ent->m_flDuckAmount();
	}

	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}

	bool operator>(const CTickRecord& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const CTickRecord& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const CTickRecord& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const CTickRecord& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const CTickRecord& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}

	matrix3x4_t	matrix[128];
	bool matrixBuilt;
	float m_flDuckAmount = 0.f;
	float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	vec3_t m_angEyeAngles = vec3_t(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	//std::array<float, 24> m_flPoseParameter = {};
	vec3_t m_angAbsAngles = vec3_t(0, 0, 0);
	int tickcount = 0;
};

struct C_PlayerStored {
	weapon_t* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	vec3_t EyeAngles = vec3_t(0, 0, 0);
	CTickRecord ValidTick = CTickRecord();
};

struct CBacktrack_Info {
	CBacktrack_Info() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord()) {}

	bool shot = false;
	C_PlayerStored player = C_PlayerStored();
	int tick_count = 0;
	CTickRecord temporaryRecord = CTickRecord();

	player_t* Entity;
	std::deque<CTickRecord> TickRecords;
};

class CBacktracking : public singleton<CBacktracking> {
	friend class CLagcompensation;
	friend class CResolver;

public:
	int ShotBackTrackedTick(player_t* ent);
	void ShotBackTrackStoreFSN(player_t* ent);
	void ShotBackTrackBeforeAimbot(player_t* ent);
	void ShotBackTrackAimbotStart(player_t* ent);
	void ShotBackTrackAimbotEnd(player_t* ent);
	void ClearRecord(player_t* Entity);

public:
	bool bTickIsValid(int tick);
	void SaveTemporaryRecord(player_t* ent, CTickRecord record = CTickRecord());
	void RestoreTemporaryRecord(player_t* ent);
	std::array<CBacktrack_Info, 32> arr_infos;

private:
	void ApplyTickrecord(player_t* ent, CTickRecord record);
	void ApplyTickRecordFSN(player_t* pEntity, CTickRecord record);
	void ApplyTickRecordCM(player_t* pEntity, CTickRecord record);

private:
};

extern backtrackData headPositions[64][12];
extern who_reads_that_isGay hdfdunn[64][12];
extern whocaresaboutcode commitdie[64][12];
extern CBacktracking* bt_2;
extern BackTrack* backtracking;
extern CTickRecord * tickrec;

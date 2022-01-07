#pragma once
#include "Hooks.h"
#include "SDK.h"
#include <deque>
#include "singleton.hpp"
#include <array>
struct lby_rec
{
	int tick_count;
	float lby;
	Vector head_position;
	float yaw;
	int32_t m_nFlags;
};
struct backtrack_data
{
	float simtime;
	Vector hitboxPos;
	Vector origin;
	Vector angles;
	int32_t flags;
	matrix3x4* ys;
};
struct c_tickrec {
	c_tickrec() {}
	c_tickrec(IClientEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYaw();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flCycle = ent->GetCycle();
		m_flSimulationTime = ent->GetSimulationTime();
		m_nSequence = ent->GetSequence();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		m_flPoseParameter = moneycounter(ent);
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_flDuckSpeed = ent->GetDuckSpeed();
		m_flDuckAmount = ent->GetDuckAmount();
	}
	std::array<float, 24> moneycounter(IClientEntity* pEnt) {
		return *(std::array<float, 24>*)((uintptr_t)pEnt + 0x2764); // + m_flPoseParameter
	}
	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}
	bool operator>(const c_tickrec& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const c_tickrec& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const c_tickrec& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const c_tickrec& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const c_tickrec& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}
	float m_flDuckAmount = 0.f;
	float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
};
struct c_ent_stored {
	C_BaseCombatWeapon* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	QAngle EyeAngles = QAngle(0, 0, 0);
	c_tickrec ValidTick = c_tickrec();
};
struct c_backtrackinfo {
	c_backtrackinfo() : shot(false), player(c_ent_stored()), tick_count(0), temporaryRecord(c_tickrec()) {}
	bool shot = false;
	c_ent_stored player = c_ent_stored();
	int tick_count = 0;
	c_tickrec temporaryRecord = c_tickrec();
	IClientEntity* Entity;
	std::deque<c_tickrec> TickRecords;
};
class c_newbacktrack
{
	int latest_tick;
	bool is_valid_tick(int tick);
	int getChockedTicks(IClientEntity * ent);
	void update_record(int i);
public:
	lby_rec records[64];
	bool isValidTick(int simTime);
	void legitBackTrackT(CUserCmd * cmd, IClientEntity * pLocal);
	void legitBackTrack(CUserCmd * cmd, IClientEntity * pLocal);
	bool run_lby_backtrack(CUserCmd* m_pcmd, Vector& aim_point);
	void update(int tick_count);
public:
	int ShotBackTrackedTick(IClientEntity* ent);
	void ShotBackTrackStoreFSN(IClientEntity* ent);
	
	void ShotBackTrackBeforeAimbot(IClientEntity* ent);
	void ShotBackTrackAimbotStart(IClientEntity* ent);
	void ShotBackTrackAimbotEnd(IClientEntity* ent);
	void ClearRecord(IClientEntity* Entity);
public:
	bool bTickIsValid(int tick);
	void SaveTemporaryRecord(IClientEntity* ent, c_tickrec record = c_tickrec());
	void RestoreTemporaryRecord(IClientEntity* ent);
	std::array<c_backtrackinfo, 32> arr_infos;
private:
	void ApplyTickrecord(IClientEntity* ent, c_tickrec record);
	void ApplyTickRecordFSN(IClientEntity* pEntity, c_tickrec record);
	void ApplyTickRecordCM(IClientEntity* pEntity, c_tickrec record);
private:
};
extern backtrack_data head_positions[64][12];
extern c_newbacktrack* new_backtracking;